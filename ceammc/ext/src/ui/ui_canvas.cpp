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
#include "ceammc_platform.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_ui.h"
#include "datatype_color.h"
#include "fmt/core.h"
#include "gperf_material_font.h"
#include "lex/parser_color.h"
#include "lex/parser_numeric.h"
#include "lex/parser_units.h"
#include "ui_canvas.tcl.h"

#include <boost/integer/common_factor.hpp>

namespace {

void ceammc_freetype_done(void* lib)
{
    FT_Done_FreeType(static_cast<FT_Library>(lib));
}

bool getColorArgs(const AtomListView& lv, draw::SetColorRGBA& rgba)
{
    if (lv.size() == 3 || lv.size() == 4) {
        rgba.r = lv.floatAt(0, 0);
        rgba.g = lv.floatAt(1, 0);
        rgba.b = lv.floatAt(2, 0);
        rgba.a = lv.floatAt(3, 1);
        return true;
    } else if (lv.isSymbol()) {
        auto str = lv[0].asT<t_symbol*>()->s_name;
        parser::ColorFullMatch p;
        if (p.parse(str)) {
            rgba.r = p.norm_red();
            rgba.g = p.norm_green();
            rgba.b = p.norm_blue();
            rgba.a = p.norm_alpha();
            return true;
        } else {
            LIB_ERR << fmt::format("can't parse color: '{}'", str);
            return false;
        }
    } else if (lv.isA<DataTypeColor>()) {
        auto data = lv.asD<DataTypeColor>();
        rgba.r = data->red();
        rgba.g = data->green();
        rgba.b = data->blue();
        rgba.a = data->alpha();
        return true;
    } else {
        LIB_ERR << fmt::format("invalid color value: '{}'", to_string(lv));
        return false;
    }
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

inline bool set_font_options(t_symbol* s, draw::SetFont* res)
{
    if (strcmp(s->s_name, "italic") == 0) {
        res->slant = CAIRO_FONT_SLANT_ITALIC;
        return true;
    } else if (strcmp(s->s_name, "bold") == 0) {
        res->weight = CAIRO_FONT_WEIGHT_BOLD;
        return true;
    } else if (strcmp(s->s_name, "normal") == 0) {
        res->slant = CAIRO_FONT_SLANT_NORMAL;
        res->weight = CAIRO_FONT_WEIGHT_NORMAL;
        return true;
    } else
        return false;
}

inline cairo_font_weight_t sym2weight(t_symbol* s)
{
    switch (crc32_hash(s)) {
    case "bold"_hash:
        return CAIRO_FONT_WEIGHT_BOLD;
    case "normal"_hash:
    default:
        return CAIRO_FONT_WEIGHT_NORMAL;
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
    , icon_font_(&s_)
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

    draw::Create cmd;
    cmd.w = width();
    cmd.h = height();
    out_queue_.enqueue(cmd);

    worker_ = std::move(std::thread([this]() {
        DrawCommandVisitor visitor(surface_, ctx_, in_queue_);
        visitor.setLibraryPath(externalDir()->s_name);

        while (!worker_quit_) {
            draw::DrawCommand cmd;
            while (out_queue_.try_dequeue(cmd)) // process all available commands
                boost::apply_visitor(visitor, cmd);

            worker_notify_.waitFor(100); // sleep 100ms if no notification
        }
    }));
}

void UICanvas::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_minmax(newrect->w, 8, 512);
    newrect->h = pd_clip_minmax(newrect->h, 8, 512);

    draw::Create cmd;
    cmd.w = newrect->w;
    cmd.h = newrect->h;
    out_queue_.enqueue(cmd);

    sys_vgui("::ui::cnv::create_image ui_canvas_%d %d %d\n", image_id_, newrect->w, newrect->h);
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

void UICanvas::m_abc(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:a Y:a DATA:a+");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    float x, y;
    PARSE_PERCENT("line", "X", lv[0], &x, boxW());
    PARSE_PERCENT("line", "Y", lv[1], &y, boxH());

    draw::DrawMusic cmd;
    cmd.x = x;
    cmd.y = y;
    cmd.data = "X:1\n";
    auto mus = to_string(lv.subView(2), "\n");
    // append key signature (because of segfault in othercase)
    if (strncmp(mus.c_str(), "K:", 2) != 0)
        cmd.data += "K:C";

    cmd.data.append(to_string(lv.subView(2), "\n"));

    out_queue_.enqueue(cmd);
}

void UICanvas::m_arrow(const AtomListView& lv)
{
    static const args::ArgChecker chk("X0:a Y0:a X1:a Y1:a START:s=A|a|X|x|O|o|T|t|V|v|n? END:s=A|a|X|x|O|o|T|t|V|v|n?");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    draw::Line cmd;

    PARSE_PERCENT("line", "X0", lv[0], &cmd.x0, boxW());
    PARSE_PERCENT("line", "Y0", lv[1], &cmd.y0, boxH());
    PARSE_PERCENT("line", "X1", lv[2], &cmd.x1, boxW());
    PARSE_PERCENT("line", "Y1", lv[3], &cmd.y1, boxH());

    auto abeg = lv.symbolAt(4, gensym("D"));
    auto aend = lv.symbolAt(5, gensym("A"));

    const float ANGLE = std::atan2f(cmd.y1 - cmd.y0, cmd.x1 - cmd.x0);
    float SIZE = 0;

    // draw start
    switch (crc32_hash(abeg)) {
    case "O"_hash:
        addLineCircle(cmd.x0, cmd.y0, ANGLE, 6);
        break;
    case "o"_hash:
        addLineCircle(cmd.x0, cmd.y0, ANGLE, 3);
        break;
    case "a"_hash:
        addLineArrow(cmd.x0, cmd.y0, ANGLE, 8);
        break;
    case "A"_hash:
        addLineArrow(cmd.x0, cmd.y0, ANGLE, 14);
        break;
    case "X"_hash:
        addLineCross(cmd.x0, cmd.y0, ANGLE, 8);
        break;
    case "x"_hash:
        addLineCross(cmd.x0, cmd.y0, ANGLE, 4);
        break;
    case "V"_hash:
        addLineTail(cmd.x0, cmd.y0, ANGLE, 8);
        break;
    case "v"_hash:
        addLineTail(cmd.x0, cmd.y0, ANGLE, 4);
        break;
    case "t"_hash:
        addLineEnd(cmd.x0, cmd.y0, ANGLE, 4);
        break;
    case "T"_hash:
        addLineEnd(cmd.x0, cmd.y0, ANGLE, 8);
        break;
    default:
        break;
    }

    out_queue_.enqueue(draw::NewSubPath {});

    switch (crc32_hash(aend)) {
    case "X"_hash:
        addLineCross(cmd.x1, cmd.y1, ANGLE, 8);
        break;
    case "x"_hash:
        addLineCross(cmd.x1, cmd.y1, ANGLE, 4);
        break;
    case "O"_hash:
        addLineCircle(cmd.x1, cmd.y1, ANGLE + M_PI, 6);
        break;
    case "o"_hash:
        addLineCircle(cmd.x1, cmd.y1, ANGLE + M_PI, 3);
        break;
    case "V"_hash:
        addLineTail(cmd.x1, cmd.y1, ANGLE + M_PI, 8);
        break;
    case "v"_hash:
        addLineTail(cmd.x1, cmd.y1, ANGLE + M_PI, 4);
        break;
    case "a"_hash:
        addLineArrow(cmd.x1, cmd.y1, ANGLE + M_PI, 8);
        break;
    case "A"_hash:
        addLineArrow(cmd.x1, cmd.y1, ANGLE + M_PI, 14);
        break;
    case "t"_hash:
        addLineEnd(cmd.x1, cmd.y1, ANGLE, 4);
        break;
    case "T"_hash:
        addLineEnd(cmd.x1, cmd.y1, ANGLE, 8);
        break;
    default:
        break;
    }

    out_queue_.enqueue(draw::NewSubPath {});
    out_queue_.enqueue(cmd);
}

void UICanvas::m_fill(const AtomListView& lv)
{
    draw::SetColorRGBA color;

    if (!getColorArgs(lv.subView(0, 1), color)) {
        UI_ERR << "usage: fill COLOR preserve?";
        return;
    }

    bool preserve = (lv.size() == 2 && (lv[1] == "preserve" || lv[1] == 1));

    out_queue_.enqueue(color);
    out_queue_.enqueue(draw::DrawFill(preserve));
}

void UICanvas::m_font(const AtomListView& lv)
{
    static const args::ArgChecker chk1("FAMILY:s SIZE:f?");
    static const args::ArgChecker chk2("FAMILY:s STYLE:s=normal|italic|bold SIZE:f?");
    static const args::ArgChecker chk3("FAMILY:s ITALIC:s=italic BOLD:s=bold SIZE:f?");

    draw::SetFontSize sz;
    draw::SetFont cmd;
    cmd.slant = CAIRO_FONT_SLANT_NORMAL;
    cmd.weight = CAIRO_FONT_WEIGHT_NORMAL;
    cmd.freetype = 0;

    if (chk1.check(lv, nullptr, nullptr, false)) {
        cmd.family = lv.symbolAt(0, &s_)->s_name;
        out_queue_.enqueue(cmd);

        if (lv.size() == 2) {
            sz.size = lv.floatAt(1, 16);
            out_queue_.enqueue(sz);
        }
    } else if (chk2.check(lv, nullptr, nullptr, false)) {
        cmd.family = lv.symbolAt(0, &s_)->s_name;
        if (!set_font_options(lv.symbolAt(1, &s_), &cmd)) {
            UI_ERR << fmt::format("invalid font option: '{}', expected normal|italic|bold", to_string(lv[1]));
            return;
        }

        out_queue_.enqueue(cmd);
        if (lv.size() == 3) {
            sz.size = lv.floatAt(2, 16);
            out_queue_.enqueue(sz);
        }
    } else if (chk3.check(lv, nullptr, nullptr, false)) {
        cmd.family = lv.symbolAt(0, &s_)->s_name;
        cmd.slant = CAIRO_FONT_SLANT_ITALIC;
        cmd.weight = CAIRO_FONT_WEIGHT_BOLD;

        out_queue_.enqueue(cmd);
        if (lv.size() == 4) {
            sz.size = lv.floatAt(3, 16);
            out_queue_.enqueue(sz);
        }
    } else {
        chk1.usage();
        chk2.usage();
        chk3.usage();
    }
}

void UICanvas::m_line(const AtomListView& lv)
{
    if (lv.size() != 4) {
        UI_ERR << "usage: line X0 Y0 X1 Y1";
        return;
    }

    draw::Line cmd;

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

    draw::LineTo cmd;

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

    draw::Rect cmd;

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

    draw::Circle cmd;

    PARSE_PERCENT("circle", "X", lv[0], &cmd.x, boxW());
    PARSE_PERCENT("circle", "Y", lv[1], &cmd.y, boxH());
    PARSE_PERCENT("circle", "RADIUS", lv[2], &cmd.r, boxW());

    out_queue_.enqueue(cmd);
}

void UICanvas::m_stroke(const AtomListView& lv)
{
    draw::SetColorRGBA color;

    if (!getColorArgs(lv.subView(0, 1), color)) {
        UI_ERR << "usage: stroke COLOR preserve?";
        return;
    }

    bool preserve = (lv.size() == 2 && lv[1] == "preserve");

    out_queue_.enqueue(color);
    out_queue_.enqueue(draw::DrawStroke(preserve));
}

void UICanvas::m_text(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:a Y:a TEXT:a+");

    if (!chk.check(lv, nullptr))
        return chk.usage();

    draw::Text c;
    PARSE_PERCENT("text", "X", lv[0], &c.x, boxW());
    PARSE_PERCENT("text", "Y", lv[1], &c.y, boxH());
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

void UICanvas::m_star(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:a Y:a N:i>=3 RADIUS:f>0? STEP:i>0?");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    float x = 0, y = 0;
    PARSE_PERCENT("star", "X", lv[0], &x, boxW());
    PARSE_PERCENT("star", "Y", lv[1], &y, boxH());

    draw::Polygon cmd;
    auto N = lv.intAt(2, 0);
    auto R = lv.floatAt(3, std::min(boxH(), boxW()) * 0.5);
    auto STEP = lv.intAt(4, 0);

    if (STEP == 0) { // auto step
        STEP = (N - 1) / 2;
    }

    const auto GCD = boost::integer::gcd(N, STEP);

    if (GCD == 1) {
        for (int i = 0; i < N; i++) {
            float a = ((2 * M_PI) * (i * STEP) / N) + M_PI_2;
            cmd.data.push_back(x + std::cos(a) * R);
            cmd.data.push_back(y - std::sin(a) * R);
        }
        out_queue_.enqueue(cmd);
    } else if (N % STEP != 0) { // 10/4, 14/6
        const auto NK = N % STEP;
        for (int k = 0; k < NK; k++) {
            for (int i = 0; i < N / NK; i++) {
                float a = ((2 * M_PI) * ((i * STEP) + k) / N) + M_PI_2;
                cmd.data.push_back(x + std::cos(a) * R);
                cmd.data.push_back(y - std::sin(a) * R);
            }
            out_queue_.enqueue(cmd);
            cmd.data.clear();
        }
    } else { // 6/2 = 3, 8/2
        for (int k = 0; k < STEP; k++) { // 0..2
            for (int i = 0; i < N / STEP; i++) { // 0..3: 0 2 4 1 3 5
                float a = ((2 * M_PI) * (((i * STEP) + k) % N) / N) + M_PI_2;
                cmd.data.push_back(x + std::cos(a) * R);
                cmd.data.push_back(y - std::sin(a) * R);
            }
            out_queue_.enqueue(cmd);
            cmd.data.clear();
        }
    }
}

void UICanvas::m_scale(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:f Y:f?");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    draw::Scale cmd;
    cmd.x = lv.floatAt(0, 1);
    cmd.y = lv.floatAt(1, cmd.x);

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

void UICanvas::m_musicxml(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:a Y:a FILE:s");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    float x, y;
    PARSE_PERCENT("line", "X", lv[0], &x, boxW());
    PARSE_PERCENT("line", "Y", lv[1], &y, boxH());
    auto fname = lv.symbolAt(2, &s_)->s_name;
    auto path = platform::find_in_std_path(canvas(), fname);

    if (path.empty()) {
        UI_ERR << fmt::format("can't find file: '{}'", fname);
        return;
    }

    draw::DrawMusic cmd;
    cmd.x = x;
    cmd.y = y;
    cmd.format = draw::FORMAT_MUSICXML;
    cmd.data = path;

    out_queue_.enqueue(cmd);
}

void UICanvas::m_new_path(const AtomListView& lv)
{
    out_queue_.enqueue(draw::NewPath {});
}

void UICanvas::m_new_subpath(const AtomListView& lv)
{
    out_queue_.enqueue(draw::NewSubPath {});
}

void UICanvas::m_curve(const AtomListView& lv)
{
    if (lv.size() != 8) {
        UI_ERR << "usage: line X0 Y0 X1 Y1 X2 Y2 X3 Y3";
        return;
    }

    draw::Curve cmd;

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
    out_queue_.enqueue(draw::Sync(subscriberId(), zoom()));
    worker_notify_.notifyOne();
}

void UICanvas::m_save()
{
    out_queue_.enqueue(draw::Save());
}

void UICanvas::m_restore()
{
    out_queue_.enqueue(draw::Restore());
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

    draw::Polygon cmd;
    auto N = lv.intAt(2, 0);
    auto R = lv.floatAt(3, std::min(boxH(), boxW()) * 0.5);

    for (int i = 0; i < N; i++) {
        float a = ((2 * M_PI) * i / N) + M_PI_2;
        cmd.data.push_back(x + std::cos(a) * R);
        cmd.data.push_back(y - std::sin(a) * R);
    }

    out_queue_.enqueue(cmd);
}

void UICanvas::m_polar(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:a Y:a ADIV:i[0,12]? NC:i[0,12]? CDIST:i>0?");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    float x = 0, y = 0;
    PARSE_PERCENT("image", "X", lv[0], &x, boxW());
    PARSE_PERCENT("image", "Y", lv[1], &y, boxH());
    const auto ADIV = lv.intAt(2, 0);
    const auto NC = lv.intAt(3, 0);
    const auto CDIST = lv.floatAt(4, 10);

    // save
    out_queue_.enqueue(draw::Save {});
    out_queue_.enqueue(draw::Translate { x, y });

    const float MAXR = std::sqrtf(hypot2());

    // draw circles
    if (NC > 0) {
        out_queue_.enqueue(draw::NewPath {});

        for (int i = 1; i <= NC; i++) {
            float r = i * CDIST;
            if (r > MAXR)
                break;

            out_queue_.enqueue(draw::NewSubPath {});
            out_queue_.enqueue(draw::Circle { 0, 0, r });
        }
    }

    // draw beams
    auto NDIV = ADIV * 4;
    const float R = NC > 0 ? std::min(MAXR, NC * CDIST) : MAXR;
    for (int i = 0; i < NDIV; i++) {
        float a = (2 * M_PI * i) / NDIV;
        out_queue_.enqueue(draw::Line { 0, 0, std::cos(a) * R, std::sin(a) * R });
    }

    out_queue_.enqueue(draw::ClosePath {});

    // restore
    out_queue_.enqueue(draw::Restore {});
}

void UICanvas::m_qrcode(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:a Y:a PIXEL:i>0 TEXT:s");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    float x = 0, y = 0;
    PARSE_PERCENT("qrcode", "X", lv[0], &x, boxW());
    PARSE_PERCENT("qrcode", "Y", lv[1], &y, boxH());
    const std::int16_t pix = lv.intAt(2, 1);
    const auto text = lv.symbolAt(3, &s_)->s_name;

    out_queue_.enqueue(draw::QRCode { text, (int16_t)x, (int16_t)y, (int16_t)pix });
}

void UICanvas::m_font_size(t_float sz)
{
    draw::SetFontSize c;
    c.size = sz;
    out_queue_.enqueue(c);
}

void UICanvas::m_icon(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:a Y:a SIZE:i>=6 ICON:s");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    if (icon_font_ == &s_) {
        constexpr auto FONT_AWESOME = "fonts/MaterialIcons-Regular.ttf";
        auto path = platform::find_in_std_path(canvas(), FONT_AWESOME);
        if (path.empty()) {
            UI_ERR << "FontAwesome not found";
            return;
        }

        icon_font_ = gensym(path.c_str());
    }

    draw::SetFont font;
    font.family = icon_font_->s_name;
    font.slant = 0;
    font.weight = 0;
    font.freetype = 1;

    float x = 0, y = 0;
    PARSE_PERCENT("icon", "X", lv[0], &x, boxW());
    PARSE_PERCENT("icon", "Y", lv[1], &y, boxH());
    const auto ft_size = lv.floatAt(2, 10);
    const auto name = lv.symbolAt(3, &s_)->s_name;
    auto data = MaterialFontHash::in_word_set(name, strlen(name));
    if (!data) {
        UI_ERR << fmt::format("icon not found: '{}'", name);
        return;
    }

    out_queue_.enqueue(font);
    out_queue_.enqueue(draw::SetFontSize { ft_size });
    out_queue_.enqueue(draw::Text { x, y, data->glyph });
}

void UICanvas::m_image(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:a Y:a FILE:s ZOOM:f>0?");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    float x = 0, y = 0;
    PARSE_PERCENT("image", "X", lv[0], &x, boxW());
    PARSE_PERCENT("image", "Y", lv[1], &y, boxH());
    auto path = lv.symbolAt(2, &s_)->s_name;
    float zoom = lv.floatAt(3, 0);

    auto full_path = platform::find_in_std_path(canvas(), path);
    if (full_path.empty()) {
        UI_ERR << fmt::format("can't find file: '{}'", path);
        return;
    }

    if (zoom > 0) {
        out_queue_.enqueue(draw::Save {});
        out_queue_.enqueue(draw::Scale { zoom, zoom });
    }

    draw::DrawImage cmd;
    cmd.path = std::move(full_path);
    cmd.x = x;
    cmd.y = y;
    if (zoom > 0) {
        cmd.x /= zoom;
        cmd.y /= zoom;
    }

    out_queue_.enqueue(cmd);

    if (zoom > 0)
        out_queue_.enqueue(draw::Restore {});
}

void UICanvas::m_background(const AtomListView& lv)
{
    if (!lv.empty()) {
        draw::SetColorRGBA rgba;
        if (!getColorArgs(lv, rgba)) {
            UI_ERR << "usage: bg COLOR or color RED[0-1] GREEN[0-1] BLUE[0-1] ALPHA[0-1]?";
            return;
        }

        out_queue_.enqueue(rgba);
    }

    out_queue_.enqueue(draw::DrawBackground());
}

void UICanvas::m_clear()
{
    StaticAtomList<3> data = { 1, 1, 1 };
    m_background(data.view());
    m_update();
}

void UICanvas::m_close_path()
{
    out_queue_.enqueue(draw::ClosePath {});
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

void UICanvas::m_matrix(const AtomListView& lv)
{
    static const args::ArgChecker chk("XX:f XY:f YY:f YX:f X0:f Y0:f");

    if (lv.empty()) {
        out_queue_.enqueue(draw::ResetMatrix {});
        return;
    }

    if (!chk.check(lv, nullptr))
        return chk.usage();

    draw::SetMatrix cmd;
    cmd.xx = lv.floatAt(0, 1);
    cmd.xy = lv.floatAt(1, 1);
    cmd.yy = lv.floatAt(2, 1);
    cmd.yx = lv.floatAt(3, 1);
    cmd.x0 = lv.floatAt(4, 1);
    cmd.y0 = lv.floatAt(5, 1);
    out_queue_.enqueue(cmd);
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
    draw::Polygon cmd;
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
        if (p.isPercent()) {
            *res *= total;
        }

        return true;
    }
}

void UICanvas::addLineCircle(float& x, float& y, float angle, float size)
{
    out_queue_.enqueue(draw::Arc { x, y, size, 0, float(2 * M_PI) });

    x += std::cosf(angle) * size;
    y += std::sinf(angle) * size;
}

void UICanvas::addLineCross(float x, float y, float angle, float size)
{
    angle -= 3 * M_PI_4;
    float ac = std::cosf(angle) * size;
    float as = std::sinf(angle) * size;
    float x1 = x + ac;
    float y1 = y + as;
    float x2 = x - ac;
    float y2 = y - as;
    //
    float x3 = x + as;
    float y3 = y - ac;
    float x4 = x - as;
    float y4 = y + ac;
    out_queue_.enqueue(draw::Line { x1, y1, x2, y2 });
    out_queue_.enqueue(draw::Line { x3, y3, x4, y4 });
}

void UICanvas::addLineTail(float x, float y, float angle, float size)
{
    angle -= 3 * M_PI_4;
    float ac = std::cosf(angle) * size;
    float as = std::sinf(angle) * size;
    float x1 = x + ac;
    float y1 = y + as;
    float x2 = x + as;
    float y2 = y - ac;
    out_queue_.enqueue(draw::Line { x, y, x1, y1 });
    out_queue_.enqueue(draw::Line { x, y, x2, y2 });
}

void UICanvas::addLineArrow(float x, float y, float angle, float size)
{
    float ca = 0.25;
    float a0 = angle + ca;
    float a1 = a0 - 2 * ca;
    float r = 10;
    float x1 = x + std::cosf(a0) * size;
    float y1 = y + std::sinf(a0) * size;
    float x2 = x + std::cosf(a1) * size;
    float y2 = y + std::sinf(a1) * size;

    out_queue_.enqueue(draw::Line { x, y, x1, y1 });
    out_queue_.enqueue(draw::Line { x, y, x2, y2 });
}

void UICanvas::addLineEnd(float x, float y, float angle, float size)
{
    angle -= M_PI_2;
    float ac = std::cosf(angle) * size;
    float as = std::sinf(angle) * size;
    float x1 = x + ac;
    float y1 = y + as;
    float x2 = x - ac;
    float y2 = y - as;
    out_queue_.enqueue(draw::Line { x1, y1, x2, y2 });
}

void UICanvas::setup()
{
    ceammc::UIObjectFactory<UICanvas> obj("ui.canvas", EBOX_GROWINDI);
    obj.addAlias("ui.cnv");

    obj.hideLabelInner();
    obj.hideProperty("background_color");
    obj.setDefaultSize(120, 60);

    obj.addMethod("abc", &UICanvas::m_abc);
    obj.addMethod("arrow", &UICanvas::m_arrow);
    obj.addMethod("bg", &UICanvas::m_background);
    obj.addMethod("circle", &UICanvas::m_circle);
    obj.addMethod("clear", &UICanvas::m_clear);
    obj.addMethod("close_path", &UICanvas::m_close_path);
    obj.addMethod("crestore", &UICanvas::m_restore);
    obj.addMethod("csave", &UICanvas::m_save);
    obj.addMethod("curve", &UICanvas::m_curve);
    obj.addMethod("dash", &UICanvas::m_dash);
    obj.addMethod("fill", &UICanvas::m_fill);
    obj.addMethod("font", &UICanvas::m_font);
    obj.addMethod("font_size", &UICanvas::m_font_size);
    obj.addMethod("icon", &UICanvas::m_icon);
    obj.addMethod("image", &UICanvas::m_image);
    obj.addMethod("line", &UICanvas::m_line);
    obj.addMethod("line_cap", &UICanvas::m_line_cap);
    obj.addMethod("line_to", &UICanvas::m_line_to);
    obj.addMethod("line_width", &UICanvas::m_line_width);
    obj.addMethod("matrix", &UICanvas::m_matrix);
    obj.addMethod("move_by", &UICanvas::m_move_by);
    obj.addMethod("move_to", &UICanvas::m_move_to);
    obj.addMethod("musicxml", &UICanvas::m_musicxml);
    obj.addMethod("new_path", &UICanvas::m_new_path);
    obj.addMethod("new_subpath", &UICanvas::m_new_subpath);
    obj.addMethod("node", &UICanvas::m_node);
    obj.addMethod("polar", &UICanvas::m_polar);
    obj.addMethod("polygon", &UICanvas::m_polygon);
    obj.addMethod("qrcode", &UICanvas::m_qrcode);
    obj.addMethod("rect", &UICanvas::m_rect);
    obj.addMethod("rotate", &UICanvas::m_rotate);
    obj.addMethod("rpolygon", &UICanvas::m_rpolygon);
    obj.addMethod("scale", &UICanvas::m_scale);
    obj.addMethod("star", &UICanvas::m_star);
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
