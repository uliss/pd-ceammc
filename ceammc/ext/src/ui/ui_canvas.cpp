/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "ui_canvas.h"
#include "args/argcheck2.h"
#include "ceammc_base64.h"
#include "ceammc_containers.h"
#include "ceammc_format.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_ui.h"
#include "fmt/core.h"
#include "lex/parser_color.h"
#include "lex/parser_numeric.h"
#include "ui_canvas.tcl.h"

namespace {

constexpr float GLOBAL_SCALE = 2;

void set_parsed_color(cairo_t* c, const AtomListView& lv, double r = 0, double g = 0, double b = 0)
{
    parser::RgbHexFullMatch p;

    if (lv.isSymbol()) {
        if (p.parse(lv.asSymbol()->s_name))
            cairo_set_source_rgb(c, p.red() / 255.0, p.green() / 255.0, p.blue() / 255.0);
        else
            cairo_set_source_rgb(c, r, g, b);
    } else if (lv.size() == 3)
        cairo_set_source_rgb(c, lv[0].asFloat(), lv[1].asFloat(), lv[2].asFloat());
    else
        cairo_set_source_rgb(c, r, g, b);
}

inline bool operator==(const Atom& a, const char* str)
{
    if (!a.isSymbol())
        return false;

    return std::strcmp(a.asT<t_symbol*>()->s_name, str) == 0;
}

#define PARSE_PERCENT(method_name, arg_name, atom, field, total)                                    \
    {                                                                                               \
        if (!p.parseAs(atom, parser::TYPE_PERCENT)) {                                               \
            UI_ERR << fmt::format(method_name ": can't parse " arg_name ": '{}'", to_string(atom)); \
            return;                                                                                 \
        } else {                                                                                    \
            field = p.asFloat();                                                                    \
            if (p.isPercent())                                                                      \
                field *= total;                                                                     \
                                                                                                    \
            p.reset();                                                                              \
        }                                                                                           \
    }

}

UINotify::UINotify(UICanvas* cnv)
    : cnv_(cnv)
{
    Dispatcher::instance().subscribe(this, cnv_->subscriberId());
}

UINotify::~UINotify()
{
    Dispatcher::instance().unsubscribe(this);
}

bool UINotify::notify(int code)
{
    return cnv_->notify();
}

UICanvas::UICanvas()
    : surface_(nullptr, &cairo_surface_destroy)
    , ctx_(nullptr, &cairo_destroy)
    , ui_notify_(this)
    , worker_quit_(false)
{
    static int image_counter = 0;
    image_id_ = ++image_counter;
}

UICanvas::~UICanvas()
{
    if (worker_.joinable()) {
        try {
            worker_quit_ = true;
            clearDrawQueue();
            worker_notify_.notifyOne();
            worker_.join();
        } catch (std::exception& e) {
            UI_ERR << fmt::format("exception thrown while stopping worker thread: '{}'", e.what());
        }
    }
}

class DrawCommandVisitor : public boost::static_visitor<void> {
    CairoSurface& surface_;
    CairoContext& ctx_;
    UICanvasInQueue& queue_;

public:
    DrawCommandVisitor(CairoSurface& surface, CairoContext& ctx, UICanvasInQueue& in)
        : surface_(surface)
        , ctx_(ctx)
        , queue_(in)
    {
    }

    void operator()(const draw::DrawCircle& c) const
    {
        if (ctx_)
            cairo_arc(ctx_.get(), c.x, c.y, c.r, 0, M_PI * 2);
    }

    void operator()(const draw::DrawRect& c) const
    {
        if (ctx_)
            cairo_rectangle(ctx_.get(), c.x, c.y, c.w, c.h);
    }

    void operator()(const draw::DrawBackground&) const
    {
        if (ctx_ && surface_) {
            cairo_rectangle(ctx_.get(), 0, 0,
                cairo_image_surface_get_width(surface_.get()),
                cairo_image_surface_get_height(surface_.get()));
            cairo_fill(ctx_.get());
        }
    }

    void operator()(const draw::MoveTo& c) const
    {
        if (ctx_)
            cairo_move_to(ctx_.get(), c.x, c.y);
    }

    void operator()(const draw::MoveBy& c) const
    {
        if (ctx_)
            cairo_rel_move_to(ctx_.get(), c.dx, c.dy);
    }

    void operator()(const draw::SetStrokeWidth& c) const
    {
        if (ctx_)
            cairo_set_line_width(ctx_.get(), c.w);
    }

    void operator()(const draw::SetFontSize& sz) const
    {
        if (ctx_)
            cairo_set_font_size(ctx_.get(), sz.size);
    }

    void operator()(const draw::SetColorRGBA& c) const
    {
        if (ctx_)
            cairo_set_source_rgba(ctx_.get(), c.r, c.g, c.b, c.a);
    }

    void operator()(const draw::DrawSave&) const
    {
        if (ctx_)
            cairo_save(ctx_.get());
    }

    void operator()(const draw::DrawRestore&) const
    {
        if (ctx_)
            cairo_restore(ctx_.get());
    }

    void operator()(const draw::CreateImage& c) const
    {
        bool do_update = surface_ && ctx_;

        if (do_update) {
            auto cur_w = cairo_image_surface_get_width(surface_.get());
            auto cur_h = cairo_image_surface_get_height(surface_.get());

            const bool same_size = (c.w == cur_w && c.h == cur_h);

            if (!same_size) {
                CairoSurface new_image(cairo_image_surface_create(CAIRO_FORMAT_RGB24,
                                           c.w * GLOBAL_SCALE, c.h * GLOBAL_SCALE),
                    &cairo_surface_destroy);
                CairoContext new_ctx(cairo_create(new_image.get()), &cairo_destroy);

                cairo_save(new_ctx.get());
                cairo_set_source_rgb(new_ctx.get(), 1, 1, 1);
                cairo_rectangle(new_ctx.get(), 0, 0, c.w * GLOBAL_SCALE, c.h * GLOBAL_SCALE);
                cairo_fill(new_ctx.get());
                cairo_set_source_surface(new_ctx.get(), surface_.get(), 0, 0);
                cairo_paint(new_ctx.get());
                cairo_restore(new_ctx.get());
                cairo_surface_flush(new_image.get());

                surface_ = std::move(new_image);
                ctx_ = std::move(new_ctx);
            }
        } else {
            surface_.reset(cairo_image_surface_create(CAIRO_FORMAT_RGB24, c.w * GLOBAL_SCALE, c.h * GLOBAL_SCALE));
            ctx_.reset(cairo_create(surface_.get()));
        }

        cairo_set_antialias(ctx_.get(), CAIRO_ANTIALIAS_BEST);
        cairo_scale(ctx_.get(), GLOBAL_SCALE, GLOBAL_SCALE);

        if (!do_update) {
            cairo_save(ctx_.get());
            cairo_set_source_rgb(ctx_.get(), 1, 1, 1);
            cairo_rectangle(ctx_.get(), 0, 0, c.w * GLOBAL_SCALE, c.h * GLOBAL_SCALE);
            cairo_fill(ctx_.get());
            cairo_restore(ctx_.get());
        }
    }

    void operator()(const draw::DrawFill& f) const
    {
        if (ctx_) {
            f.preserve
                ? cairo_fill_preserve(ctx_.get())
                : cairo_fill(ctx_.get());
        }
    }

    void operator()(const draw::DrawLine& l) const
    {
        if (ctx_) {
            cairo_move_to(ctx_.get(), l.x0, l.y0);
            cairo_line_to(ctx_.get(), l.x1, l.y1);
        }
    }

    void operator()(const draw::DrawStroke& s) const
    {
        if (ctx_) {
            s.preserve
                ? cairo_stroke_preserve(ctx_.get())
                : cairo_stroke(ctx_.get());
        }
    }

    void operator()(const draw::DrawText& t) const
    {
        if (ctx_) {
            cairo_move_to(ctx_.get(), t.x, t.y);
            cairo_show_text(ctx_.get(), t.str.c_str());
            cairo_stroke(ctx_.get());
        }
    }

    void operator()(const draw::SyncImage& c) const
    {
        auto write_mem_fn = [](void* closure,
                                const unsigned char* data,
                                unsigned int length)
            -> cairo_status_t {
            auto buf = static_cast<std::vector<std::uint8_t>*>(closure);
            std::copy(data, data + length, std::back_inserter(*buf));
            return CAIRO_STATUS_SUCCESS;
        };

        std::vector<std::uint8_t> buf;
        if (c.zoom == 1) {
            CairoSurface new_image(cairo_image_surface_create(
                                       cairo_image_surface_get_format(surface_.get()),
                                       cairo_image_surface_get_width(surface_.get()) / GLOBAL_SCALE,
                                       cairo_image_surface_get_height(surface_.get()) / GLOBAL_SCALE),
                &cairo_surface_destroy);

            CairoContext new_ctx(cairo_create(new_image.get()), &cairo_destroy);
            cairo_scale(new_ctx.get(), 1 / GLOBAL_SCALE, 1 / GLOBAL_SCALE);
            cairo_set_source_surface(new_ctx.get(), surface_.get(), 0, 0);
            cairo_paint(new_ctx.get());
            auto st = cairo_surface_write_to_png_stream(new_image.get(), write_mem_fn, &buf);
            if (CAIRO_STATUS_SUCCESS == st) {
                queue_.enqueue(DrawResult { DRAW_RESULT_IMAGE, base64_encode(buf.data(), buf.size()) });
                queue_.enqueue(DrawResult { DRAW_RESULT_DEBUG, "SyncImage added" });
            }

        } else {
            if (CAIRO_STATUS_SUCCESS == cairo_surface_write_to_png_stream(surface_.get(), write_mem_fn, &buf)) {
                queue_.enqueue(DrawResult { DRAW_RESULT_IMAGE, base64_encode(buf.data(), buf.size()) });
                queue_.enqueue(DrawResult { DRAW_RESULT_DEBUG, "SyncImage added" });
            }
        }

        Dispatcher::instance().send({ c.id, 0 });
    }
};

void UICanvas::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    draw::CreateImage cmd;
    cmd.w = width();
    cmd.h = height();
    out_queue_.enqueue(cmd);

    worker_ = std::move(std::thread([this]() {
        while (!worker_quit_) {
            draw::DrawCommand cmd;
            while (out_queue_.try_dequeue(cmd)) // process all available commands
                boost::apply_visitor(DrawCommandVisitor(surface_, ctx_, in_queue_), cmd);

            worker_notify_.waitFor(100); // sleep 100ms if no notification
        }
    }));
}

void UICanvas::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_minmax(newrect->width, 8, 512);
    newrect->height = pd_clip_minmax(newrect->height, 8, 512);

    draw::CreateImage cmd;
    cmd.w = newrect->width;
    cmd.h = newrect->height;
    out_queue_.enqueue(cmd);

    sys_vgui("::ui::cnv::create_image ui_canvas_%d %d %d\n", image_id_, newrect->width, newrect->height);
}

void UICanvas::paint()
{
    if (!surface_ || !ctx_)
        return;

    sys_vgui("::ui::cnv::update %s %lx %d %d ui_canvas_%d {%s}\n",
        asEBox()->b_canvas_id->s_name, asEBox(),
        (int)width(), (int)height(),
        image_id_, data_.c_str());
}

void UICanvas::m_fill(const AtomListView& lv)
{
    draw::SetColorRGBA color;

    if ((lv.size() == 1 || lv.size() == 2) && lv[0].isSymbol()) {
        parser::ColorFullMatch p;
        auto str = lv[0].asT<t_symbol*>()->s_name;
        if (p.parse(str)) {
            color.a = p.norm_alpha();
            color.b = p.norm_blue();
            color.g = p.norm_green();
            color.r = p.norm_red();
        } else {
            UI_ERR << fmt::format("fill: can't parse color '{}'", str);
            return;
        }
    } else {
        UI_ERR << "usage: fill COLOR preserve?";
        return;
    }

    bool preserve = (lv.size() == 2 && lv[1] == "preserve");

    out_queue_.enqueue(color);
    out_queue_.enqueue(draw::DrawFill(preserve));
}

void UICanvas::m_line(const AtomListView& lv)
{
    if (lv.size() != 4) {
        UI_ERR << "usage: line X0 Y0 X1 Y1";
        return;
    }

    draw::DrawLine cmd;
    parser::NumericFullMatch p;

    PARSE_PERCENT("line", "X0", lv[0], cmd.x0, width());
    PARSE_PERCENT("line", "Y0", lv[1], cmd.y0, height());
    PARSE_PERCENT("line", "X1", lv[2], cmd.x1, width());
    PARSE_PERCENT("line", "Y1", lv[3], cmd.y1, height());

    out_queue_.enqueue(cmd);
}

void UICanvas::m_rect(const AtomListView& lv)
{
    if (lv.size() != 4) {
        UI_ERR << "usage: rect X Y WIDTH HEIGHT";
        return;
    }

    draw::DrawRect cmd;
    parser::NumericFullMatch p;

    PARSE_PERCENT("rect", "X", lv[0], cmd.x, width());
    PARSE_PERCENT("rect", "Y", lv[1], cmd.y, height());
    PARSE_PERCENT("rect", "WIDTH", lv[2], cmd.w, width());
    PARSE_PERCENT("rect", "HEIGHT", lv[3], cmd.h, height());

    out_queue_.enqueue(cmd);
}

void UICanvas::m_cicle(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:f Y:f R:f>0");

    if (!chk.check(lv, nullptr))
        return chk.usage();

    draw::DrawCircle c;
    c.x = lv.floatAt(0, 0);
    c.y = lv.floatAt(1, 0);
    c.r = lv.floatAt(2, 0);
    out_queue_.enqueue(c);
}

void UICanvas::m_stroke(const AtomListView& lv)
{
    draw::SetColorRGBA color;

    if ((lv.size() == 1 || lv.size() == 2) && lv[0].isSymbol()) {
        parser::ColorFullMatch p;
        auto str = lv[0].asT<t_symbol*>()->s_name;
        if (p.parse(str)) {
            color.a = p.norm_alpha();
            color.b = p.norm_blue();
            color.g = p.norm_green();
            color.r = p.norm_red();
        } else {
            UI_ERR << fmt::format("stroke: can't parse color '{}'", str);
            return;
        }
    } else {
        UI_ERR << "usage: stroke COLOR preserve?";
        return;
    }

    bool preserve = (lv.size() == 2 && lv[1] == "preserve");

    out_queue_.enqueue(color);
    out_queue_.enqueue(draw::DrawStroke(preserve));
}

void UICanvas::m_text(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:f Y:f TEXT:a+");

    if (!chk.check(lv, nullptr))
        return chk.usage();

    draw::DrawText c;
    c.x = lv.floatAt(0, 0);
    c.y = lv.floatAt(1, 0);
    c.str = to_string(lv.subView(2));
    out_queue_.enqueue(c);
}

void UICanvas::m_moveto(const AtomListView& lv)
{
    if (lv.size() != 2) {
        UI_ERR << "usage: line X Y";
        return;
    }

    draw::MoveTo cmd;
    parser::NumericFullMatch p;

    PARSE_PERCENT("moveto", "X", lv[0], cmd.x, width());
    PARSE_PERCENT("moveto", "Y", lv[1], cmd.y, height());

    out_queue_.enqueue(cmd);
}

void UICanvas::m_color(const AtomListView& lv)
{
    if (lv.size() == 3 || lv.size() == 4) {
        draw::SetColorRGBA rgba;
        rgba.r = lv.floatAt(0, 0);
        rgba.g = lv.floatAt(1, 0);
        rgba.b = lv.floatAt(2, 0);
        rgba.a = lv.floatAt(3, 1);
        out_queue_.enqueue(rgba);
    } else if (lv.isSymbol()) {
        auto str = lv[0].asT<t_symbol*>()->s_name;
        parser::ColorFullMatch p;
        if (p.parse(str)) {
            draw::SetColorRGBA rgba;
            rgba.r = p.norm_red();
            rgba.g = p.norm_green();
            rgba.b = p.norm_blue();
            rgba.a = p.norm_alpha();
            out_queue_.enqueue(rgba);
        } else {
            UI_ERR << fmt::format("can't parse color: '{}'", str);
        }
    } else {
        UI_ERR << "usage: color COLOR or color RED[0-1] GREEN[0-1] BLUE[0-1] ALPHA[0-1]?";
    }
}

void UICanvas::m_moveby(const AtomListView& lv)
{
    static const args::ArgChecker chk("DX:f DY:f");

    if (!chk.check(lv, nullptr))
        return chk.usage();

    draw::MoveBy c;
    c.dx = lv.floatAt(0, 0);
    c.dy = lv.floatAt(1, 0);
    out_queue_.enqueue(c);
}

void UICanvas::m_update()
{
    out_queue_.enqueue(draw::SyncImage(subscriberId(), zoom()));
    worker_notify_.notifyOne();
}

void UICanvas::m_save()
{
    out_queue_.enqueue(draw::DrawSave());
}

void UICanvas::m_restore()
{
    out_queue_.enqueue(draw::DrawRestore());
}

void UICanvas::m_font_size(t_float sz)
{
    draw::SetFontSize c;
    c.size = sz;
    out_queue_.enqueue(c);
}

void UICanvas::m_background(const AtomListView& lv)
{
    out_queue_.enqueue(draw::DrawBackground());
}

void UICanvas::m_clear()
{
    StaticAtomList<3> data = { 1, 1, 1 };
    m_color(data.view());
    m_background({});
    m_update();
}

void UICanvas::m_line_width(const AtomListView& lv)
{
    static const args::ArgChecker chk("WIDTH:f>0");

    if (!chk.check(lv, nullptr))
        return chk.usage();

    draw::SetStrokeWidth c;
    c.w = lv.floatAt(0, 1);
    out_queue_.enqueue(c);
}

void UICanvas::m_node(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:f Y:f TEXT:s SHAPE:s? COLOR:s?");

    if (!ctx_)
        return;

    if (!chk.check(lv, nullptr))
        return chk.usage();

    auto x = lv.floatAt(0, 0);
    auto y = lv.floatAt(1, 0);
    auto txt = to_string(lv[2]);

    //    switch (crc32_hash(lv.symbolAt(3, &s_))) {
    //    case "circle"_hash: {
    //        set_parsed_color(ctx_, lv.subView(4), 0.5, 0.5, 0.5);
    //        cairo_arc(ctx_, x, y, 10, 0, 2 * M_PI);
    //        cairo_fill_preserve(ctx_);
    //        cairo_set_source_rgb(ctx_, 0, 0, 0);
    //        cairo_set_line_width(ctx_, 1);
    //        cairo_stroke(ctx_);

    //        cairo_text_extents_t extents;
    //        cairo_text_extents(ctx_, txt.c_str(), &extents);
    //        auto dx = extents.width / 2 + extents.x_bearing;
    //        auto dy = extents.height / 2 + extents.y_bearing;

    //        cairo_move_to(ctx_, x - dx, y - dy);
    //        cairo_show_text(ctx_, txt.c_str());
    //        syncImage();
    //    } break;
    //    case "rect"_hash: {
    //        cairo_text_extents_t extents;
    //        cairo_text_extents(ctx_, txt.c_str(), &extents);
    //        auto w = extents.width;
    //        auto h = extents.height;
    //        auto dx = w / 2 + extents.x_bearing;
    //        auto dy = h / 2 + extents.y_bearing;

    //        set_parsed_color(ctx_, lv.subView(4), 0.5, 0.5, 0.5);
    //        cairo_rectangle(ctx_,
    //            x - dx - 3,
    //            y - dy - 3,
    //            6 + extents.x_advance,
    //            h + 6 + extents.y_advance);

    //        cairo_fill_preserve(ctx_);
    //        cairo_set_source_rgb(ctx_, 0, 0, 0);
    //        cairo_set_line_width(ctx_, 1);
    //        cairo_stroke(ctx_);

    //        cairo_move_to(ctx_, x - dx, y - dy + h);
    //        cairo_show_text(ctx_, txt.c_str());
    //        syncImage();
    //    } break;
    //    case "tri"_hash:
    //        break;
    //    }
}

bool UICanvas::notify()
{
    DrawResult res;

    while (in_queue_.try_dequeue(res)) {
        switch (res.type) {
        case DRAW_RESULT_DEBUG:
            UI_DBG << res.data;
            break;
        case DRAW_RESULT_ERROR:
            UI_ERR << res.data;
            break;
        case DRAW_RESULT_IMAGE:
            if (data_ != res.data) {
                data_ = res.data;
                redraw();
            }
            break;
        }
    }

    return true;
}

void UICanvas::onZoom(t_float z)
{
    UIObject::onZoom(z);
    m_update();
}

void UICanvas::clearDrawQueue()
{
    draw::DrawCommand cmd;
    while (out_queue_.try_dequeue(cmd))
        ;
}

void UICanvas::setup()
{
    ceammc::UIObjectFactory<UICanvas> obj("ui.canvas", EBOX_GROWINDI);
    obj.addAlias("ui.cnv");

    obj.hideLabelInner();
    obj.setDefaultSize(60, 60);

    obj.addMethod("bg", &UICanvas::m_background);
    obj.addMethod("circle", &UICanvas::m_cicle);
    obj.addMethod("clear", &UICanvas::m_clear);
    obj.addMethod("color", &UICanvas::m_color);
    obj.addMethod("fill", &UICanvas::m_fill);
    obj.addMethod("font_size", &UICanvas::m_font_size);
    obj.addMethod("line", &UICanvas::m_line);
    obj.addMethod("line_width", &UICanvas::m_line_width);
    obj.addMethod("moveby", &UICanvas::m_moveby);
    obj.addMethod("moveto", &UICanvas::m_moveto);
    obj.addMethod("node", &UICanvas::m_node);
    obj.addMethod("rect", &UICanvas::m_rect);
    obj.addMethod("restore", &UICanvas::m_restore);
    obj.addMethod("save", &UICanvas::m_save);
    obj.addMethod("stroke", &UICanvas::m_stroke);
    obj.addMethod("text", &UICanvas::m_text);
    obj.addMethod("update", &UICanvas::m_update);
}

void setup_ui_canvas()
{
    sys_gui(ui_canvas_tcl);

    UICanvas::setup();
}
