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
#include "ceammc_containers.h"
#include "ceammc_format.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_ui.h"
#include "fmt/core.h"
#include "lex/parser_color.h"
#include "lex/parser_numeric.h"
#include "lex/parser_units.h"
#include "ui_canvas.tcl.h"

namespace {

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

inline cairo_line_cap_t sym2line_cap(t_symbol* s)
{
    switch (crc32_hash(s)) {
    case "square"_hash:
        return CAIRO_LINE_CAP_SQUARE;
    case "round"_hash:
        return CAIRO_LINE_CAP_ROUND;
    case "butt"_hash:
    default:
        return CAIRO_LINE_CAP_BUTT;
    }
}

#define PARSE_PERCENT(method, arg, value, res, total)      \
    {                                                      \
        if (!parsePercent(method, arg, value, res, total)) \
            return;                                        \
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

    PARSE_PERCENT("line", "X0", lv[0], &cmd.x0, boxW());
    PARSE_PERCENT("line", "Y0", lv[1], &cmd.y0, boxH());
    PARSE_PERCENT("line", "X1", lv[2], &cmd.x1, boxW());
    PARSE_PERCENT("line", "Y1", lv[3], &cmd.y1, boxH());

    out_queue_.enqueue(cmd);
}

void UICanvas::m_line_to(const AtomListView& lv)
{
    if (lv.size() != 2) {
        UI_ERR << "usage: line X Y";
        return;
    }

    draw::DrawLineTo cmd;

    PARSE_PERCENT("line", "X", lv[0], &cmd.x, boxW());
    PARSE_PERCENT("line", "Y", lv[1], &cmd.y, boxH());

    out_queue_.enqueue(cmd);
}

void UICanvas::m_line_cap(const AtomListView& lv)
{
    static const args::ArgChecker chk("TYPE:s=butt|round|square");

    if (!chk.check(lv, nullptr))
        return chk.usage(nullptr, gensym("line_cap"));

    out_queue_.enqueue(draw::SetLineCap(sym2line_cap(lv[0].asT<t_symbol*>())));
}

void UICanvas::m_rect(const AtomListView& lv)
{
    if (lv.size() != 4) {
        UI_ERR << "usage: rect X Y WIDTH HEIGHT";
        return;
    }

    draw::DrawRect cmd;

    PARSE_PERCENT("rect", "X", lv[0], &cmd.x, boxW());
    PARSE_PERCENT("rect", "Y", lv[1], &cmd.y, boxH());
    PARSE_PERCENT("rect", "WIDTH", lv[2], &cmd.w, boxW());
    PARSE_PERCENT("rect", "HEIGHT", lv[3], &cmd.h, boxH());

    out_queue_.enqueue(cmd);
}

void UICanvas::m_circle(const AtomListView& lv)
{
    if (lv.size() != 3) {
        UI_ERR << "usage: circle X Y RADIUS";
        return;
    }

    draw::DrawCircle cmd;

    PARSE_PERCENT("circle", "X", lv[0], &cmd.x, boxW());
    PARSE_PERCENT("circle", "Y", lv[1], &cmd.y, boxH());
    PARSE_PERCENT("circle", "RADIUS", lv[2], &cmd.r, boxW());

    out_queue_.enqueue(cmd);
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

void UICanvas::m_translate(const AtomListView& lv)
{
    if (lv.size() != 2) {
        UI_ERR << "usage: translate X Y";
        return;
    }

    draw::Translate cmd;

    PARSE_PERCENT("translate", "X", lv[0], &cmd.x, boxW());
    PARSE_PERCENT("translate", "Y", lv[1], &cmd.y, boxH());

    out_queue_.enqueue(cmd);
}

void UICanvas::m_move_to(const AtomListView& lv)
{
    if (lv.size() != 2) {
        UI_ERR << "usage: line X Y";
        return;
    }

    draw::MoveTo cmd;

    PARSE_PERCENT("moveto", "X", lv[0], &cmd.x, boxW());
    PARSE_PERCENT("moveto", "Y", lv[1], &cmd.y, boxH());

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

void UICanvas::m_curve(const AtomListView& lv)
{
    if (lv.size() != 8) {
        UI_ERR << "usage: line X0 Y0 X1 Y1 X2 Y2 X3 Y3";
        return;
    }

    draw::DrawCurve cmd;

    PARSE_PERCENT("curve", "X0", lv[0], &cmd.x0, boxW());
    PARSE_PERCENT("curve", "Y0", lv[1], &cmd.y0, boxH());
    PARSE_PERCENT("curve", "X1", lv[2], &cmd.x1, boxW());
    PARSE_PERCENT("curve", "Y1", lv[3], &cmd.y1, boxH());
    PARSE_PERCENT("curve", "X2", lv[4], &cmd.x2, boxW());
    PARSE_PERCENT("curve", "Y2", lv[5], &cmd.y2, boxH());
    PARSE_PERCENT("curve", "X3", lv[6], &cmd.x3, boxW());
    PARSE_PERCENT("curve", "Y3", lv[7], &cmd.y3, boxH());

    out_queue_.enqueue(cmd);
}

void UICanvas::m_dash(const AtomListView& lv)
{
    static const args::ArgChecker chk("DASHES:i>=0{0,4}");

    if (!chk.check(lv, nullptr))
        return chk.usage();

    draw::SetDash dash;
    dash.n = lv.size();
    for (size_t i = 0; i < std::min(lv.size(), dash.MAX_DASHES); i++)
        dash.dashes[i] = lv[i].asInt();

    out_queue_.enqueue(dash);
}

void UICanvas::m_move_by(const AtomListView& lv)
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

void UICanvas::m_ctx_save()
{
    out_queue_.enqueue(draw::DrawSave());
}

void UICanvas::m_ctx_restore()
{
    out_queue_.enqueue(draw::DrawRestore());
}

void UICanvas::m_rotate(const AtomListView& lv)
{
    if (lv.size() != 1) {
        UI_ERR << "usage: rotate ANGLE";
        return;
    }

    draw::Rotate cmd;
    parser::UnitsFullMatch p;
    if (!p.parse(lv[0])) {
        UI_ERR << fmt::format("can't parse angle value: '{}'", to_string(lv[0]));
        return;
    } else if (p.type() != parser::TYPE_INT
        && p.type() != parser::TYPE_FLOAT
        && p.type() != parser::TYPE_RADIAN
        && p.type() != parser::TYPE_DEGREE) {
        UI_ERR << fmt::format("invalid angle value: '{}', radians or degrees are expected", to_string(lv[0]));
        return;
    }

    if (p.type() != parser::TYPE_RADIAN)
        cmd.angle = (p.value() / 180.0) * M_PI;
    else
        cmd.angle = p.value();

    out_queue_.enqueue(cmd);
}

void UICanvas::m_rpolygon(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:a Y:a N:i>=3 RADIUS:f>0?");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    float x = 0, y = 0;
    PARSE_PERCENT("rpolygon", "X", lv[0], &x, boxW());
    PARSE_PERCENT("rpolygon", "Y", lv[1], &y, boxH());

    draw::DrawPolygon cmd;
    auto N = lv.intAt(2, 0);
    auto R = lv.floatAt(3, std::min(boxH(), boxW()) * 0.5);

    for (int i = 0; i < N; i++) {
        float a = ((2 * M_PI) * i / N) + M_PI_2;
        cmd.data.push_back(x + std::cos(a) * R);
        cmd.data.push_back(y - std::sin(a) * R);
    }

    out_queue_.enqueue(cmd);
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
    //    cairo_rota
    //            cairo_arc(ctx_, x, y, 10, 0, 2 * M_PI);
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

void UICanvas::m_polygon(const AtomListView& lv)
{
    if (lv.size() < 6 || (lv.size() & 0x1)) {
        UI_ERR << fmt::format("invalid number of arguments ({}), usage: polygon X0 Y0 X1 Y1 ... XN YN", lv.size());
        return;
    }

    auto w = boxW();
    auto h = boxH();
    draw::DrawPolygon cmd;
    cmd.data.reserve(lv.size());

    for (size_t i = 0; i < lv.size(); i++) {
        auto& a = lv[i];
        auto arg_name = (i & 1) ? "Y" : "X";
        float res = 0;
        if (!parsePercent("polygon", arg_name, a, &res, (i & 1) ? h : w)) {
            UI_ERR << fmt::format("can't parse coord {} at {}: {}", arg_name, i, to_string(a));
            return;
        }

        cmd.data.push_back(res);
    }

    out_queue_.enqueue(cmd);
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

bool UICanvas::parsePercent(const char* methodName, const char* argName, const Atom& a, float* res, float total)
{
    parser::NumericFullMatch p;
    if (!p.parseAs(a, parser::TYPE_PERCENT)) {
        UI_ERR << fmt::format("[{}( can't parse {}: '{}'", methodName, argName, to_string(a));
        return false;
    } else {
        *res = p.asFloat();
        if (p.isPercent())
            *res *= total;

        return true;
    }
}

void UICanvas::setup()
{
    ceammc::UIObjectFactory<UICanvas> obj("ui.canvas", EBOX_GROWINDI);
    obj.addAlias("ui.cnv");

    obj.hideLabelInner();
    obj.setDefaultSize(60, 60);

    obj.addMethod("bg", &UICanvas::m_background);
    obj.addMethod("circle", &UICanvas::m_circle);
    obj.addMethod("clear", &UICanvas::m_clear);
    obj.addMethod("color", &UICanvas::m_color);
    obj.addMethod("ctx_restore", &UICanvas::m_ctx_restore);
    obj.addMethod("ctx_save", &UICanvas::m_ctx_save);
    obj.addMethod("curve", &UICanvas::m_curve);
    obj.addMethod("dash", &UICanvas::m_dash);
    obj.addMethod("fill", &UICanvas::m_fill);
    obj.addMethod("font_size", &UICanvas::m_font_size);
    obj.addMethod("line", &UICanvas::m_line);
    obj.addMethod("line_to", &UICanvas::m_line_to);
    obj.addMethod("line_cap", &UICanvas::m_line_cap);
    obj.addMethod("line_width", &UICanvas::m_line_width);
    obj.addMethod("move_by", &UICanvas::m_move_by);
    obj.addMethod("move_to", &UICanvas::m_move_to);
    obj.addMethod("node", &UICanvas::m_node);
    obj.addMethod("polygon", &UICanvas::m_polygon);
    obj.addMethod("rect", &UICanvas::m_rect);
    obj.addMethod("rotate", &UICanvas::m_rotate);
    obj.addMethod("rpolygon", &UICanvas::m_rpolygon);
    obj.addMethod("stroke", &UICanvas::m_stroke);
    obj.addMethod("text", &UICanvas::m_text);
    obj.addMethod("translate", &UICanvas::m_translate);
    obj.addMethod("update", &UICanvas::m_update);
}

void setup_ui_canvas()
{
    sys_gui(ui_canvas_tcl);

    UICanvas::setup();
    LIB_DBG << "cairo version: " << cairo_version_string();
}
