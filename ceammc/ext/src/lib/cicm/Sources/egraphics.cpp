/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "egraphics.h"

#include "ceammc_convert.h"
#include "ceammc_syms.h"

static const float k = 0.55228474983079356430692996582365594804286956787109f;
static const char HexDigits[] = "0123456789ABCDEF";

const t_rgba rgba_black = { 0.f, 0.f, 0.f, 1.f };
const t_rgba rgba_greydark = { 0.3f, 0.3, 0.3f, 1.f };
const t_rgba rgba_grey = { 0.5f, 0.5, 0.5f, 1.f };
const t_rgba rgba_greylight = { 0.8f, 0.8, 0.8f, 1.f };
const t_rgba rgba_white = { 1.f, 1.f, 1.f, 1.f };
const t_rgba rgba_blue = { 0.f, 0.f, 1.f, 1.f };
const t_rgba rgba_green = { 0.f, 1.f, 0.f, 1.f };
const t_rgba rgba_red = { 1.f, 0.f, 0.f, 1.f };

static void egraphics_apply_matrix(t_elayer* g, t_egobj* gobj);

void egraphics_set_line_width(t_elayer* g, float width)
{
    g->e_line_width = (int)pd_clip_min(width, 0.);
}

void egraphics_set_line_capstyle(t_elayer* g, t_capstyle style)
{
    g->e_line_capstyle = style;
}

void egraphics_set_color_rgba(t_elayer* g, const t_rgba* rgba)
{
    g->e_color = rgba_to_hex_int(*rgba);
}

void egraphics_set_color_rgb(t_elayer* g, const t_rgb* rgb)
{
    g->e_color = rgb_to_hex_int(*rgb);
}

void egraphics_set_color_hsla(t_elayer* g, const t_hsla* hsla)
{
    t_rgba color = hsla_to_rgba(*hsla);
    g->e_color = rgba_to_hex_int(color);
}

void egraphics_set_color_hsl(t_elayer* g, const t_hsl* hsl)
{
    t_rgb color = hsl_to_rgb(*hsl);
    g->e_color = rgb_to_hex_int(color);
}

static void egraphics_paint(t_elayer* g, bool filled, bool preserved)
{
    if (g->e_new_objects.e_type != E_GOBJ_INVALID && g->e_new_objects.e_points.size() > 0) {
        g->e_objects.emplace_back();
        t_egobj* nobj = &g->e_objects.back();
        nobj->e_filled = filled;
        nobj->e_type = g->e_new_objects.e_type;
        nobj->e_color = g->e_color;
        nobj->e_width = g->e_line_width;
        nobj->e_capstyle = g->e_line_capstyle;
        nobj->e_dashstyle = g->e_line_dashstyle;
        nobj->e_smooth = g->e_line_smooth;
        nobj->e_justify = g->e_new_objects.e_justify;
        nobj->e_anchor = g->e_new_objects.e_anchor;
        nobj->e_text = g->e_new_objects.e_text;
        nobj->e_image = g->e_new_objects.e_image;

        if (preserved) {
            nobj->e_points = g->e_new_objects.e_points;
        } else {
            nobj->e_points = std::move(g->e_new_objects.e_points);
            g->e_new_objects.e_type = E_GOBJ_INVALID;
        }

        egraphics_apply_matrix(g, nobj);
    }
}

void egraphics_fill_preserve(t_elayer* g)
{
    egraphics_paint(g, true, true);
}

void egraphics_fill(t_elayer* g)
{
    egraphics_paint(g, true, false);
}

void egraphics_stroke_preserve(t_elayer* g)
{
    egraphics_paint(g, false, true);
}

void egraphics_stroke(t_elayer* g)
{
    egraphics_paint(g, false, false);
}

void etext_layout_draw(t_etext* textlayout, t_elayer* g)
{
    g->e_objects.emplace_back();
    auto& last = g->e_objects.back();

    last.e_type = E_GOBJ_TEXT;
    last.e_points.assign({ { textlayout->c_rect.x, textlayout->c_rect.y },
        { textlayout->c_rect.x + textlayout->c_rect.width, textlayout->c_rect.y + textlayout->c_rect.height } });
    last.e_color = rgba_to_hex_int(textlayout->c_color);

    last.e_font = textlayout->c_font;
    last.e_justify = textlayout->c_justify;
    last.e_text = textlayout->c_text;
    last.e_anchor = textlayout->c_anchor;
    egraphics_apply_matrix(g, &last);
}

void egraphics_move_to(t_elayer* g, const float x, const float y)
{
    if (g->e_state == EGRAPHICS_OPEN) {
        auto& p = g->e_new_objects.e_points;
        g->e_new_objects.e_type = E_GOBJ_PATH;
        p.reserve(p.size() + 2);
        p.push_back({ E_PATH_MOVE, 0 });
        p.push_back({ x, y });
    }
}

static inline bool is_on_line(const t_pt& p1, const t_pt& p2, const t_pt& p3) // returns true if p3 is on line p1, p2
{
    // kx + b
    const auto k = (p2.y - p1.y) / (p2.x - p1.x);
    return std::round(((k * (p3.x - p1.x)) - (p3.y - p1.y)) * 8) == 0;
}

void egraphics_line_to(t_elayer* g, const float x, const float y)
{
    constexpr t_pt p_line = { E_PATH_LINE, 0 };

    if (g->e_state == EGRAPHICS_OPEN) {
        auto& p = g->e_new_objects.e_points;
        const size_t n = p.size();
        if (g->e_optimize && n >= 4 && p[n - 2] == p_line && p[n - 4] == p_line) {
            if (int(p[n - 1].y) == int(y) && int(p[n - 3].y) == int(y)) {
                p[n - 1].x = x;
                return;
            } else if (is_on_line(p[n - 3], p[n - 1], { x, y })) {
                p[n - 1] = { x, y };
                return;
            }
        }

        g->e_new_objects.e_type = E_GOBJ_PATH;
        p.reserve(n + 2);
        p.push_back(p_line);
        p.push_back({ x, y });
    }
}

void egraphics_close_path(t_elayer* g)
{
    if (g->e_state == EGRAPHICS_OPEN && g->e_new_objects.e_points.size() >= 1) {
        g->e_new_objects.e_type = E_GOBJ_PATH;
        g->e_new_objects.e_points.push_back({ E_PATH_CLOSE, 0 });
    }
}

void egraphics_line(t_elayer* g, const float x0, const float y0, const float x1, const float y1)
{
    if (g->e_state == EGRAPHICS_OPEN) {
        auto& p = g->e_new_objects.e_points;
        g->e_new_objects.e_type = E_GOBJ_PATH;
        p.reserve(p.size() + 4);
        p.push_back({ E_PATH_MOVE, 0 });
        p.push_back({ x0, y0 });
        p.push_back({ E_PATH_LINE, 0 });
        p.push_back({ x1, y1 });
    }
}

void egraphics_line_fast(t_elayer* g, float x0, float y0, float x1, float y1)
{
    egraphics_line(g, x0, y0, x1, y1);
    egraphics_stroke(g);
}

void egraphics_curve_to(t_elayer* g, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy)
{
    if (g->e_state == EGRAPHICS_OPEN) {
        auto& p = g->e_new_objects.e_points;
        g->e_new_objects.e_type = E_GOBJ_PATH;
        p.reserve(p.size() + 4);

        p.push_back({ E_PATH_CURVE, 0 });
        p.push_back({ ctrl1x, ctrl1y });
        p.push_back({ ctrl2x, ctrl2y });
        p.push_back({ endx, endy });
    }
}

void egraphics_curve(t_elayer* g, float startx, float starty,
    float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy)
{
    if (g->e_state == EGRAPHICS_OPEN) {
        auto& p = g->e_new_objects.e_points;
        g->e_new_objects.e_type = E_GOBJ_PATH;
        p.reserve(p.size() + 6);

        p.push_back({ E_PATH_MOVE, 0 });
        p.push_back({ startx, starty });
        p.push_back({ E_PATH_CURVE, 0 });
        p.push_back({ ctrl1x, ctrl1y });
        p.push_back({ ctrl2x, ctrl2y });
        p.push_back({ endx, endy });
    }
}

void egraphics_rectangle(t_elayer* g, float x, float y, float width, float height)
{
    if (g->e_state == EGRAPHICS_OPEN) {
        g->e_new_objects.e_type = E_GOBJ_SHAPE;

        auto& p = g->e_new_objects.e_points;
        p.assign({ { E_SHAPE_RECT, 0 },
            { x, y },
            { x + width, y + height } });
    }
}

void egraphics_oval(t_elayer* g, float xc, float yc, float radiusx, float radiusy)
{
    if (g->e_state == EGRAPHICS_OPEN) {
        g->e_new_objects.e_type = E_GOBJ_SHAPE;

        auto& p = g->e_new_objects.e_points;
        p.assign({ { E_SHAPE_OVAL, 0 },
            { xc - radiusx, yc - radiusy },
            { xc + radiusx, yc + radiusy } });
    }
}

void egraphics_circle(t_elayer* g, float xc, float yc, float radius)
{
    egraphics_oval(g, xc, yc, radius, radius);
}

void egraphics_arc_oval(t_elayer* g, float xc, float yc, float radiusx, float radiusy, float angle1, float angle2)
{
    if (g->e_state == EGRAPHICS_OPEN) {
        g->e_new_objects.e_type = E_GOBJ_SHAPE;

        auto& p = g->e_new_objects.e_points;
        p.assign({ { E_SHAPE_ARC, 0 },
            { xc - radiusx, yc - radiusy },
            { xc + radiusx, yc + radiusy },
            { (angle1 / EPD_PI) * 180.f, ((angle2 - angle1) / EPD_PI) * 180.f } });
    }
}

void egraphics_poly(t_elayer* g, const std::vector<t_pt>& points)
{
    if (points.empty())
        return;

    if (g->e_state == EGRAPHICS_OPEN) {
        g->e_new_objects.e_type = E_GOBJ_PATH;

        auto& p = g->e_new_objects.e_points;
        p.reserve(p.size() + 2 * points.size() + 1);

        p.push_back({ E_PATH_MOVE, 0 });
        p.push_back(points.front());

        for (size_t i = 1; i < points.size(); i++) {
            p.push_back({ E_PATH_LINE, 0 });
            p.push_back(points[i]);
        }

        p.push_back({ E_PATH_CLOSE, 0 });
    }
}

void egraphics_arc(t_elayer* g, float xc, float yc, float radius, float angle1, float angle2)
{
    egraphics_arc_oval(g, xc, yc, radius, radius, angle1, angle2);
}

static void rotate(const float cosz, const float sinz, t_pt* p1)
{
    const float rx = p1->x * cosz - p1->y * sinz;
    p1->y = (p1->x * sinz + p1->y * cosz);
    p1->x = rx;
}

static void create_small_arc(const float r, const float start, const float extend, t_pt ct, t_pt* p2, t_pt* p3, t_pt* p4)
{
    const float a = extend;
    const float cosz = cosf(a * 0.5f + start);
    const float sinz = sinf(a * 0.5f + start);
    t_pt p1;
    p4->x = (float)(r * cosf(a * 0.5f));
    p4->y = (float)(r * sinf(a * 0.5f));
    p1.x = p4->x;
    p1.y = -p4->y;
    p2->x = (float)(p1.x + k * tan(a * 0.5f) * p4->y);
    p2->y = (float)(p1.y + k * tan(a * 0.5f) * p4->x);
    p3->x = p2->x;
    p3->y = -p2->y;

    rotate(cosz, sinz, p2);
    rotate(cosz, sinz, p3);
    rotate(cosz, sinz, p4);
    p2->x += ct.x;
    p2->y += ct.y;
    p3->x += ct.x;
    p3->y += ct.y;
    p4->x += ct.x;
    p4->y += ct.y;
}

void egraphics_arc_to(t_elayer* g, float cx, float cy, float extend)
{
    if (g->e_state == EGRAPHICS_OPEN) {
        if (g->e_new_objects.e_type == E_GOBJ_PATH && g->e_new_objects.e_points.size() > 0) {
            t_pt p2, p3, p4, c = { cx, cy }, prev = g->e_new_objects.e_points.back();
            float radius = pd_radius(prev.x - cx, prev.y - cy);
            float angle = pd_angle(prev.x - cx, prev.y - cy);

            while (extend > EPD_2PI) {
                extend -= EPD_2PI;
            }
            while (extend < -EPD_2PI) {
                extend += EPD_2PI;
            }

            while (fabsf(extend) >= EPD_PI4) {
                if (extend < 0.) {
                    create_small_arc(radius, angle, -EPD_PI4, c, &p2, &p3, &p4);
                    extend += EPD_PI4;
                    angle -= EPD_PI4;
                } else {
                    create_small_arc(radius, angle, EPD_PI4, c, &p2, &p3, &p4);
                    extend -= EPD_PI4;
                    angle += EPD_PI4;
                }
                egraphics_curve_to(g, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
            }
            if (fabsf(extend) > 1e-6) {
                create_small_arc(radius, angle, extend, c, &p2, &p3, &p4);
                egraphics_curve_to(g, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
            }
        } else {
            g->e_new_objects.e_type = E_GOBJ_INVALID;
        }
    }
}

static void create_small_arc_oval(const float r1, const float r2,
    const float start, const float extend, t_pt ct, t_pt* p2, t_pt* p3, t_pt* p4)
{
    t_pt p1;

    const float a = extend;
    const float cosz = cosf(a * 0.5f + start);
    const float sinz = sinf(a * 0.5f + start);
    const float cosa = cosf(a * 0.5f);
    const float sina = sinf(a * 0.5f);
    const float k1 = (4.f * (1.f - cosf(a * 0.5f)) / sinf(a * 0.5f)) / 3.f;
    const float k2 = (4.f * (1.f - cosf(-a * 0.5f)) / sinf(-a * 0.5f)) / 3.f;
    p4->x = (float)(r2 * cosa);
    p4->y = (float)(r2 * sina);
    p1.x = (float)(r1 * cosa);
    p1.y = (float)(-r1 * sina);
    p2->x = (float)(p1.x + k1 * p4->y);
    p2->y = (float)(p1.y + k1 * p4->x);
    p3->x = (float)(p4->x + k2 * p1.y);
    p3->y = (float)(p4->y + k2 * p1.y);

    rotate(cosz, sinz, p2);
    rotate(cosz, sinz, p3);
    rotate(cosz, sinz, p4);
    p2->x += ct.x;
    p2->y += ct.y;
    p3->x += ct.x;
    p3->y += ct.y;
    p4->x += ct.x;
    p4->y += ct.y;
}

void egraphics_arc_oval_to(t_elayer* g, float cx, float cy, float radius, float extend)
{
    if (g->e_state == EGRAPHICS_OPEN) {
        if (g->e_new_objects.e_type == E_GOBJ_PATH && g->e_new_objects.e_points.size() > 0) {
            t_pt p2, p3, p4, c = { cx, cy }, prev = g->e_new_objects.e_points.back();
            float r1 = pd_radius(prev.x - cx, prev.y - cy);
            float angle = pd_angle(prev.x - cx, prev.y - cy);
            float ratio = (radius - r1) / (fabsf(extend) / EPD_PI4);

            while (extend > EPD_2PI) {
                extend -= EPD_2PI;
            }
            while (extend < -EPD_2PI) {
                extend += EPD_2PI;
            }

            while (fabsf(extend) >= EPD_PI4) {
                if (extend < 0.) {

                    create_small_arc_oval(r1, r1 + ratio, angle, -EPD_PI4, c, &p2, &p3, &p4);
                    extend += EPD_PI4;
                    angle -= EPD_PI4;
                    r1 += ratio;
                } else {
                    create_small_arc_oval(r1, r1 + ratio, angle, EPD_PI4, c, &p2, &p3, &p4);
                    extend -= EPD_PI4;
                    angle += EPD_PI4;
                    r1 += ratio;
                }
                egraphics_curve_to(g, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
            }
            if (fabsf(extend) > 1e-6) {
                create_small_arc_oval(r1, radius, angle, extend, c, &p2, &p3, &p4);
                egraphics_curve_to(g, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
            }
        } else {
            g->e_new_objects.e_type = E_GOBJ_INVALID;
        }
    }
}

int rgba_to_hex_int(const t_rgba& color)
{
    const auto r = static_cast<unsigned int>(color.red * 255.f) & 0xFF;
    const auto g = static_cast<unsigned int>(color.green * 255.f) & 0xFF;
    const auto b = static_cast<unsigned int>(color.blue * 255.f) & 0xFF;

    return (r << 16) | (g << 8) | b;
}

int rgb_to_hex_int(const t_rgb& color)
{
    const auto r = static_cast<unsigned int>(color.red * 255.f) & 0xFF;
    const auto g = static_cast<unsigned int>(color.green * 255.f) & 0xFF;
    const auto b = static_cast<unsigned int>(color.blue * 255.f) & 0xFF;

    return (r << 16) | (g << 8) | b;
}

char* rgba_to_hex(const t_rgba& color)
{
    static char ColBuf[10];

    auto r = (std::uint16_t)(color.red * 255.f);
    auto g = (std::uint16_t)(color.green * 255.f);
    auto b = (std::uint16_t)(color.blue * 255.f);

    ColBuf[0] = '#';
    ColBuf[1] = HexDigits[(r >> 4) & 15];
    ColBuf[2] = HexDigits[r & 15];
    ColBuf[3] = HexDigits[(g >> 4) & 15];
    ColBuf[4] = HexDigits[g & 15];
    ColBuf[5] = HexDigits[(b >> 4) & 15];
    ColBuf[6] = HexDigits[b & 15];
    ColBuf[7] = '\0';
    return &ColBuf[0];
}

t_rgba hex_to_rgba(const char* color)
{
    int hexvalue = (int)strtol(color + 1, NULL, 16);
    t_rgba ncolor;
    ncolor.red = (float)((hexvalue >> 16) & 0xFF) / 255.f;
    ncolor.green = (float)((hexvalue >> 8) & 0xFF) / 255.f;
    ncolor.blue = (float)((hexvalue)&0xFF) / 255.f;
    ncolor.alpha = 1.f;
    return ncolor;
}

t_rgb hex_to_rgb(const char* color)
{
    int hexvalue = (int)strtol(color + 1, NULL, 16);
    t_rgb ncolor;
    ncolor.red = (float)((hexvalue >> 16) & 0xFF) / 255.f;
    ncolor.green = (float)((hexvalue >> 8) & 0xFF) / 255.f;
    ncolor.blue = (float)((hexvalue)&0xFF) / 255.f;
    return ncolor;
}

t_hsla rgba_to_hsla(t_rgba color)
{
    t_hsla ncolor = { 0., 0., 0., 0. };
    float delta, deltar, deltag, deltab;
    float max = color.red;
    float min = color.red;
    if (min > color.green)
        min = color.green;
    if (min > color.blue)
        min = color.blue;
    if (max < color.green)
        max = color.green;
    if (max < color.blue)
        max = color.blue;
    delta = max - min;
    ncolor.alpha = color.alpha;
    ncolor.lightness = (max + min) / 2.f;
    if (delta == 0) {
        ncolor.hue = 0;
        ncolor.saturation = 1;
        ncolor.lightness = 1;
        return ncolor;
    }

    if (max == 0) {
        ncolor.hue = 0;
        ncolor.saturation = 0;
    } else {
        if (ncolor.lightness < 0.5f)
            ncolor.saturation = delta / (max + min);
        else
            ncolor.saturation = delta / (2.f - max - min);

        deltar = (((max - color.red) / 6) + (delta / 2)) / delta;
        deltag = (((max - color.green) / 6) + (delta / 2)) / delta;
        deltab = (((max - color.blue) / 6) + (delta / 2)) / delta;

        if (color.red == max)
            ncolor.hue = deltab - deltag;
        else if (color.green == max)
            ncolor.hue = (1.f / 3.f) + deltar - deltab;
        else if (color.blue == max)
            ncolor.hue = (2.f / 3.f) + deltag - deltar;

        if (ncolor.hue < 0.f)
            ncolor.hue += 1;
        if (ncolor.hue > 1.f)
            ncolor.hue -= 1;
    }

    return ncolor;
}

t_hsl rgb_to_hsl(t_rgb color)
{
    t_hsl ncolor = { 0., 0., 0. };
    float delta, deltar, deltag, deltab;
    float max = color.red;
    float min = color.red;
    if (min > color.green)
        min = color.green;
    if (min > color.blue)
        min = color.blue;
    if (max < color.green)
        max = color.green;
    if (max < color.blue)
        max = color.blue;
    delta = max - min;
    ncolor.lightness = (max + min) / 2.f;
    if (max == 0) {
        ncolor.hue = 0;
        ncolor.saturation = 0;
    } else {
        if (ncolor.lightness < 0.5f)
            ncolor.saturation = delta / (max + min);
        else
            ncolor.saturation = delta / (2.f - max - min);

        deltar = (((max - color.red) / 6) + (delta / 2)) / delta;
        deltag = (((max - color.green) / 6) + (delta / 2)) / delta;
        deltab = (((max - color.blue) / 6) + (delta / 2)) / delta;

        if (color.red == max)
            ncolor.hue = deltab - deltag;
        else if (color.green == max)
            ncolor.hue = (1.f / 3.f) + deltar - deltab;
        else if (color.blue == max)
            ncolor.hue = (2.f / 3.f) + deltag - deltar;

        if (ncolor.hue < 0.f)
            ncolor.hue += 1;
        if (ncolor.hue > 1.f)
            ncolor.hue -= 1;
    }

    return ncolor;
}

static float Hue_2_RGB(float P, float Q, float Tc)
{
    if (Tc < 0.f)
        Tc += 1;
    if (Tc > 1.f)
        Tc -= 1;

    if (Tc < 0.1666666f) // 1/6
        return P + ((Q - P) * 6.f * Tc);
    else if (0.1666666f <= Tc && Tc < 0.5f)
        return Q;
    else if (0.5f <= Tc && Tc < 0.666666f) // 2/3
        return (P + (Q - P) * ((2.f / 3.f) - Tc) * 6.f);
    else
        return P;
}

t_rgb hsl_to_rgb(t_hsl color)
{
    float var1, var2;
    t_rgb ncolor;
    if (color.saturation == 0.f) {
        ncolor.red = color.lightness;
        ncolor.green = color.lightness;
        ncolor.blue = color.lightness;
    } else {
        if (color.lightness < 0.5f)
            var2 = color.lightness * (1.f + color.saturation);
        else
            var2 = (color.lightness + color.saturation) - (color.saturation * color.lightness);

        var1 = 2 * color.lightness - var2;

        ncolor.red = Hue_2_RGB(var1, var2, color.hue + (1.f / 3.f));
        ncolor.green = Hue_2_RGB(var1, var2, color.hue);
        ncolor.blue = Hue_2_RGB(var1, var2, color.hue - (1.f / 3.f));
    }
    return ncolor;
}

t_rgba hsla_to_rgba(t_hsla color)
{
    t_rgba res;
    if (color.saturation == 0.f) {
        res.red = color.lightness;
        res.green = color.lightness;
        res.blue = color.lightness;
    } else {
        float Q;

        if (color.lightness < 0.5f)
            Q = color.lightness * (1.f + color.saturation);
        else
            Q = (color.lightness + color.saturation) - (color.lightness * color.saturation);

        float P = 2.f * color.lightness - Q;

        res.red = Hue_2_RGB(P, Q, color.hue + 0.333333f);
        res.green = Hue_2_RGB(P, Q, color.hue);
        res.blue = Hue_2_RGB(P, Q, color.hue - 0.333333f);
    }

    res.alpha = color.alpha;
    return res;
}

t_rgba rgba_addContrast(t_rgba color, float contrast)
{
    t_rgba new_color = color;
    new_color.red = (float)pd_clip_minmax(new_color.red += contrast, 0., 1.);
    new_color.green = (float)pd_clip_minmax(new_color.green += contrast, 0., 1.);
    new_color.blue = (float)pd_clip_minmax(new_color.blue += contrast, 0., 1.);
    return new_color;
}

t_rgb rgb_addContrast(t_rgb color, float contrast)
{
    t_rgb new_color = color;
    new_color.red = (float)pd_clip_minmax(new_color.red += contrast, 0., 1.);
    new_color.green = (float)pd_clip_minmax(new_color.green += contrast, 0., 1.);
    new_color.blue = (float)pd_clip_minmax(new_color.blue += contrast, 0., 1.);
    return new_color;
}

void rgba_set(t_rgba* color, float red, float green, float blue, float alpha)
{
    color->red = red;
    color->green = green;
    color->blue = blue;
    color->alpha = alpha;
}

void rgb_set(t_rgb* color, float red, float green, float blue)
{
    color->red = red;
    color->green = green;
    color->blue = blue;
}

void hsla_set(t_hsla* color, float hue, float saturation, float lightness, float alpha)
{
    color->hue = hue;
    color->saturation = saturation;
    color->lightness = lightness;
    color->alpha = alpha;
}

void hsl_set(t_hsl* color, float hue, float saturation, float lightness)
{
    color->hue = hue;
    color->saturation = saturation;
    color->lightness = lightness;
}

void egraphics_matrix_init(t_matrix* x, float xx, float yx, float xy, float yy, float x0, float y0)
{
    x->xx = xx;
    x->yx = yx;
    x->xy = xy;
    x->yy = yy;
    x->x0 = x0;
    x->y0 = y0;
}

void egraphics_set_matrix(t_elayer* g, const t_matrix* matrix)
{
    egraphics_matrix_init(&g->e_matrix, matrix->xx, matrix->yx, matrix->xy, matrix->yy, matrix->x0, matrix->y0);
}

void egraphics_rotate(t_elayer* g, float angle)
{
    const float cosRad = cosf(angle);
    const float sinRad = sinf(angle);
    t_matrix temp = g->e_matrix;

    g->e_matrix.xx = temp.xx * cosRad - temp.yx * sinRad;
    g->e_matrix.yx = temp.xx * sinRad + temp.yx * cosRad;
    g->e_matrix.xy = temp.xy * cosRad - temp.yy * sinRad;
    g->e_matrix.yy = temp.xy * sinRad + temp.yy * cosRad;
}

static inline void apply_matrix_to(const t_matrix& mx, t_pt& pt)
{
    auto x_p = pt.x * mx.xx + pt.y * mx.xy + mx.x0;
    auto y_p = pt.x * mx.yx + pt.y * mx.yy + mx.y0;
    pt.x = x_p;
    pt.y = y_p;
}

static void egraphics_apply_matrix(t_elayer* g, t_egobj* gobj)
{
    float x_p, y_p;
    if (gobj->e_type == E_GOBJ_PATH) {
        for (int i = 0; i < gobj->e_points.size();) {
            switch ((int)gobj->e_points[i].x) {
            case E_PATH_MOVE:
            case E_PATH_LINE: {
                x_p = gobj->e_points[i + 1].x * g->e_matrix.xx + gobj->e_points[i + 1].y * g->e_matrix.xy + g->e_matrix.x0;
                y_p = gobj->e_points[i + 1].x * g->e_matrix.yx + gobj->e_points[i + 1].y * g->e_matrix.yy + g->e_matrix.y0;
                gobj->e_points[i + 1].x = x_p;
                gobj->e_points[i + 1].y = y_p;
                i += 2;
            } break;
            case E_PATH_CURVE: {
                x_p = gobj->e_points[i + 1].x * g->e_matrix.xx + gobj->e_points[i + 1].y * g->e_matrix.xy + g->e_matrix.x0;
                y_p = gobj->e_points[i + 1].x * g->e_matrix.yx + gobj->e_points[i + 1].y * g->e_matrix.yy + g->e_matrix.y0;
                gobj->e_points[i + 1].x = x_p;
                gobj->e_points[i + 1].y = y_p;
                x_p = gobj->e_points[i + 2].x * g->e_matrix.xx + gobj->e_points[i + 2].y * g->e_matrix.xy + g->e_matrix.x0;
                y_p = gobj->e_points[i + 2].x * g->e_matrix.yx + gobj->e_points[i + 2].y * g->e_matrix.yy + g->e_matrix.y0;
                gobj->e_points[i + 2].x = x_p;
                gobj->e_points[i + 2].y = y_p;
                x_p = gobj->e_points[i + 3].x * g->e_matrix.xx + gobj->e_points[i + 3].y * g->e_matrix.xy + g->e_matrix.x0;
                y_p = gobj->e_points[i + 3].x * g->e_matrix.yx + gobj->e_points[i + 3].y * g->e_matrix.yy + g->e_matrix.y0;
                gobj->e_points[i + 3].x = x_p;
                gobj->e_points[i + 3].y = y_p;
                i += 4;
            } break;
            default:
                i++;
            }
        }
    } else if (gobj->e_type == E_GOBJ_SHAPE) {
        switch ((int)gobj->e_points[0].x) {
        case E_SHAPE_OVAL: {
            x_p = gobj->e_points[1].x * g->e_matrix.xx + gobj->e_points[1].y * g->e_matrix.xy + g->e_matrix.x0;
            y_p = gobj->e_points[1].x * g->e_matrix.yx + gobj->e_points[1].y * g->e_matrix.yy + g->e_matrix.y0;
            gobj->e_points[1].x = x_p;
            gobj->e_points[1].y = y_p;
            x_p = gobj->e_points[2].x * g->e_matrix.xx + gobj->e_points[2].y * g->e_matrix.xy + g->e_matrix.x0;
            y_p = gobj->e_points[2].x * g->e_matrix.yx + gobj->e_points[2].y * g->e_matrix.yy + g->e_matrix.y0;
            gobj->e_points[2].x = x_p;
            gobj->e_points[2].y = y_p;
        } break;
        case E_SHAPE_RECT: {
            apply_matrix_to(g->e_matrix, gobj->e_points[1]);
            apply_matrix_to(g->e_matrix, gobj->e_points[2]);
        } break;
        default:
            break;
        }
    } else if (gobj->e_type == E_GOBJ_TEXT) {
        apply_matrix_to(g->e_matrix, gobj->e_points[0]);
        apply_matrix_to(g->e_matrix, gobj->e_points[1]);
    }
}

t_etext* etext_layout_create(void)
{
    t_etext* new_text_layout = (t_etext*)malloc(sizeof(t_etext));
    new_text_layout->c_color.red = 0.;
    new_text_layout->c_color.green = 0.;
    new_text_layout->c_color.blue = 0.;
    new_text_layout->c_color.alpha = 1.;
    new_text_layout->c_buf[0] = 0;
    new_text_layout->c_text = &new_text_layout->c_buf[0];
    new_text_layout->c_is_buffer_used = 1;
    new_text_layout->c_justify = ETEXT_JLEFT;
    new_text_layout->c_anchor = ETEXT_LEFT;

    return new_text_layout;
}

void etext_layout_destroy(t_etext* textlayout)
{
    if (!textlayout->c_is_buffer_used)
        free((void*)textlayout->c_text);

    free(textlayout);
}

void etext_layout_set(t_etext* textlayout, const char* text, t_efont* font,
    float x, float y, float width, float height,
    etextanchor_flags anchor, etextjustify_flags justify, etextwrap_flags wrap)
{
    const size_t N = sizeof(textlayout->c_buf);

    // free old memory if malloc memory used
    if (!textlayout->c_is_buffer_used) {
        free((void*)textlayout->c_text);
        textlayout->c_text = 0;
    }

    size_t txt_len = strlen(text);

    // if short text use struct buffer
    if (txt_len < N) {
        strncpy(textlayout->c_buf, text, N);
        textlayout->c_text = textlayout->c_buf;
        textlayout->c_is_buffer_used = 1;
    } else {
        // or use malloc instead
        textlayout->c_is_buffer_used = 0;
        textlayout->c_text = strdup(text);
    }

    textlayout->c_font = font[0];
    textlayout->c_rect.x = (float)x;
    textlayout->c_rect.y = (float)y;
    textlayout->c_rect.width = (float)width;
    textlayout->c_rect.height = (float)height;

    if (wrap == ETEXT_NOWRAP) {
        textlayout->c_rect.width = 0.;
    }

    textlayout->c_anchor = anchor;
    textlayout->c_justify = justify;
}

void etext_layout_settextcolor(t_etext* textlayout, t_rgba* color)
{
    textlayout->c_color = color[0];
}

t_efont* efont_create(t_symbol* family, t_symbol* slant, t_symbol* weight, float size)
{
    using namespace ceammc;

    auto new_font = (t_efont*)malloc(sizeof(t_efont));
    if (new_font) {
        new_font->c_family = family;
        new_font->c_slant = slant;
        if (slant != sym_italic())
            new_font->c_slant = sym_roman();

        new_font->c_weight = weight;
        if (weight != sym_bold())
            new_font->c_weight = sym_normal();

        new_font->c_size = pd_clip_min(size, 1);
        new_font->c_sizereal = new_font->c_size;
    }
    return new_font;
}

void efont_destroy(t_efont* font)
{
    free(font);
}

t_float pd_clip_minmax(t_float aValue, t_float aMinimum, t_float aMaximum)
{
    if (aValue < aMinimum)
        return aMinimum;
    else if (aValue > aMaximum)
        return aMaximum;
    else
        return aValue;
}

t_float pd_clip_min(t_float aValue, t_float aMinimum)
{
    if (aValue < aMinimum)
        return aMinimum;
    else
        return aValue;
}

t_float pd_clip_max(t_float aValue, t_float aMaximum)
{
    if (aValue > aMaximum)
        return aMaximum;
    else
        return aValue;
}

t_float pd_ordinate(t_float radius, t_float angle)
{
    return radius * std::sin(angle);
}

t_float pd_abscissa(t_float radius, t_float angle)
{
    return radius * std::cos(angle);
}

t_float pd_radius(t_float x, t_float y)
{
    return std::sqrt(x * x + y * y);
}

t_float pd_angle(t_float x, t_float y)
{
    return std::atan2(y, x);
}

void egraphics_set_line_dashstyle(t_elayer* g, t_dashstyle style)
{
    g->e_line_dashstyle = style;
}

t_rgba rgba_color_sum(const t_rgba* c1, const t_rgba* c2, float alpha)
{
    t_rgba res;
    res.red = ceammc::clip<float>(c1->red * alpha + c2->red * (1 - alpha), 0, 1);
    res.green = ceammc::clip<float>(c1->green * alpha + c2->green * (1 - alpha), 0, 1);
    res.blue = ceammc::clip<float>(c1->blue * alpha + c2->blue * (1 - alpha), 0, 1);
    res.alpha = 1;
    return res;
}

void egraphics_image(t_elayer* g, float xc, float yc, t_eimage* image)
{
    if (g->e_state == EGRAPHICS_OPEN) {
        g->e_new_objects.e_type = E_GOBJ_SHAPE;
        g->e_new_objects.e_image = image;

        auto& p = g->e_new_objects.e_points;
        p.assign({ { E_SHAPE_IMAGE, 0 }, { xc, yc } });
    }
}

t_eimage* eimage_create(const char* data, int width, int height, etextanchor_flags anchor)
{
    static int image_counter = 1;

    t_eimage* img = new t_eimage;
    img->data_base64 = data;
    img->width = width;
    img->height = height;
    img->anchor = anchor;

    char buf[32];
    snprintf(buf, 32, "ceammc_img%04i", image_counter++);
    img->name = gensym(buf);
    return img;
}

void eimage_destroy(t_eimage* img)
{
    if (!img)
        return;

    delete img;
}

void eimage_copy(t_eimage* dest, const t_eimage* src)
{
    if (dest == src)
        return;

    dest->data_base64 = src->data_base64;
    dest->width = src->width;
    dest->height = src->height;
    dest->anchor = src->anchor;
}

void eimage_set_base64_data(t_eimage* img, const char* base64)
{
    img->data_base64 = base64;
}

void egraphics_set_color_hex(t_elayer* g, uint32_t c)
{
    g->e_color = c;
}

void egraphics_raise(t_elayer* over, t_elayer* l)
{
    sys_vgui("raise %s %s\n", over->e_id->s_name, l->e_id->s_name);
}

void egraphics_set_line_smooth(t_elayer* g, t_smooth smooth)
{
    g->e_line_smooth = smooth;
}

void egraphics_preallocate_points(t_elayer* g, size_t npoints)
{
    g->e_new_objects.e_points.reserve(g->e_new_objects.e_points.size() + npoints);
}

void egraphics_preallocate_objects(t_elayer* g, size_t nobj)
{
    g->e_objects.reserve(g->e_objects.size() + nobj);
}
