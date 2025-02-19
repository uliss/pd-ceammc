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
#include "ui_canvas_cairo.h"
#include "ceammc_base64.h"
#include "ceammc_poll_dispatcher.h"

#include "cairo-ft.h"
#include "fmt/core.h"
#include "qrcodegen.hpp"
#include "resvg_common.h"
#include "rimg_common.h"
#include "verovio_common.h"

#include <boost/algorithm/string.hpp>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#ifdef WITH_STB
#include "stb_common.h"
#endif

ceammc::DrawCommandVisitor::DrawCommandVisitor(CairoSurface& surface, CairoContext& ctx, UICanvasInQueue& in)
    : surface_(surface)
    , ctx_(ctx)
    , queue_(in)
    , ft_lib_(nullptr)
{
    auto err = FT_Init_FreeType(&ft_lib_);
    if (err != 0)
        queue_.enqueue(DrawResult { DRAW_RESULT_ERROR, "can't init Freetype library" });
}

ceammc::DrawCommandVisitor::~DrawCommandVisitor()
{
    if (ft_lib_)
        FT_Done_FreeType(ft_lib_);
}

void ceammc::DrawCommandVisitor::setLibraryPath(const std::string& path)
{
    library_path_ = path;
}

void ceammc::DrawCommandVisitor::operator()(const draw::NextVariant& n) const
{
    n.apply_visitor(*this);
}

void ceammc::DrawCommandVisitor::operator()(const draw::ClosePath&) const
{
    if (ctx_)
        cairo_close_path(ctx_.get());
}

void ceammc::DrawCommandVisitor::operator()(const draw::NewPath&) const
{
    if (ctx_)
        cairo_new_path(ctx_.get());
}

void ceammc::DrawCommandVisitor::operator()(const draw::NewSubPath&) const
{
    if (ctx_)
        cairo_new_sub_path(ctx_.get());
}

void ceammc::DrawCommandVisitor::operator()(const draw::Arc& a) const
{
    if (ctx_)
        cairo_arc(ctx_.get(), a.x, a.y, a.r, a.a0, a.a1);
}

void ceammc::DrawCommandVisitor::operator()(const draw::Circle& c) const
{
    if (ctx_)
        cairo_arc(ctx_.get(), c.x, c.y, c.r, 0, M_PI * 2);
}

void ceammc::DrawCommandVisitor::operator()(const draw::Curve& c) const
{
    if (ctx_) {
        cairo_move_to(ctx_.get(), c.x0, c.y0);
        cairo_curve_to(ctx_.get(), c.x1, c.y1, c.x2, c.y2, c.x3, c.y3);
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::Rect& c) const
{
    if (ctx_)
        cairo_rectangle(ctx_.get(), c.x, c.y, c.w, c.h);
}

void ceammc::DrawCommandVisitor::operator()(const draw::Polygon& p) const
{
    if (ctx_) {
        if (p.data.size() < 6 || (p.data.size() & 0x1))
            return;

        cairo_move_to(ctx_.get(), p.data[0], p.data[1]);
        for (size_t i = 2; i < p.data.size(); i += 2)
            cairo_line_to(ctx_.get(), p.data[i], p.data[i + 1]);

        cairo_close_path(ctx_.get());
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::DrawImage& c) const
{
    if (ctx_) {
        CairoSurface img = { nullptr, cairo_surface_destroy };

        using namespace boost::algorithm;

        if (ends_with(c.path, ".svg")) {
            img.reset(load_svg(c.path.c_str(), queue_));
        } else if (ends_with(c.path, ".png")) {
            img.reset(cairo_image_surface_create_from_png(c.path.c_str()));
        }

        else {
#ifdef WITH_RIMG
            img.reset(load_rimg(c.path.c_str(), queue_));
#elif WITH_STB
            img.reset(load_stb(c.path.c_str(), queue_));
#endif
        }

        if (!img) {
            queue_.enqueue(DrawResult { DRAW_RESULT_ERROR, fmt::format("can't load image: '{}'", c.path) });
            return;
        }

        cairo_save(ctx_.get());
        cairo_set_source_surface(ctx_.get(), img.get(), c.x, c.y);
        auto err = cairo_status(ctx_.get());
        if (err != CAIRO_STATUS_SUCCESS) {
            queue_.enqueue(DrawResult { DRAW_RESULT_ERROR, fmt::format("cairo error: '{}'", cairo_status_to_string(err)) });
        }
        cairo_paint(ctx_.get());
        err = cairo_status(ctx_.get());
        if (err != CAIRO_STATUS_SUCCESS) {
            queue_.enqueue(DrawResult { DRAW_RESULT_ERROR, fmt::format("cairo error: '{}'", cairo_status_to_string(err)) });
        }

        cairo_restore(ctx_.get());
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::DrawBackground&) const
{
    if (ctx_ && surface_) {
        cairo_save(ctx_.get());
        cairo_identity_matrix(ctx_.get());
        cairo_scale(ctx_.get(), GLOBAL_SCALE, GLOBAL_SCALE);
        cairo_rectangle(ctx_.get(), 0, 0,
            cairo_image_surface_get_width(surface_.get()),
            cairo_image_surface_get_height(surface_.get()));
        cairo_fill(ctx_.get());
        cairo_restore(ctx_.get());
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::DrawMusic& m) const
{
    if (!ctx_)
        return;

    CairoSurface img { load_music(m, library_path_, queue_), cairo_surface_destroy };
    if (!img)
        return;

    cairo_save(ctx_.get());
    cairo_set_source_surface(ctx_.get(), img.get(), m.x, m.y);
    auto err = cairo_status(ctx_.get());
    if (err != CAIRO_STATUS_SUCCESS) {
        queue_.enqueue(DrawResult { DRAW_RESULT_ERROR, fmt::format("cairo error: '{}'", cairo_status_to_string(err)) });
    }
    cairo_paint(ctx_.get());
    err = cairo_status(ctx_.get());
    if (err != CAIRO_STATUS_SUCCESS) {
        queue_.enqueue(DrawResult { DRAW_RESULT_ERROR, fmt::format("cairo error: '{}'", cairo_status_to_string(err)) });
    }

    cairo_restore(ctx_.get());
}

void ceammc::DrawCommandVisitor::operator()(const draw::MoveTo& c) const
{
    if (ctx_)
        cairo_move_to(ctx_.get(), c.x, c.y);
}

void ceammc::DrawCommandVisitor::operator()(const draw::MoveBy& c) const
{
    if (ctx_)
        cairo_rel_move_to(ctx_.get(), c.dx, c.dy);
}

void ceammc::DrawCommandVisitor::operator()(const draw::SetStrokeWidth& c) const
{
    if (ctx_)
        cairo_set_line_width(ctx_.get(), c.w);
}

void ceammc::DrawCommandVisitor::operator()(const draw::SetLineCap& cap) const
{
    if (ctx_)
        cairo_set_line_cap(ctx_.get(), static_cast<cairo_line_cap_t>(cap.type));
}

void ceammc::DrawCommandVisitor::operator()(const draw::SetDash& dash) const
{
    if (ctx_) {
        const auto N = dash.n;
        double dashes[draw::SetDash::MAX_DASHES] = { 0 };
        for (size_t i = 0; i < N; i++)
            dashes[i] = dash.dashes[i];

        if (N > 0
            && (std::any_of(dashes, dashes + N, [](double x) { return x < 0; })
                || std::all_of(dashes, dashes + N, [](double x) { return x == 0; }))) {
            queue_.enqueue(DrawResult { DRAW_RESULT_ERROR, "invalid dash values" });
            return;
        }

        cairo_set_dash(ctx_.get(), dashes, dash.n, 1);
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::SetFontSize& sz) const
{
    if (ctx_)
        cairo_set_font_size(ctx_.get(), sz.size);
}

void ceammc::DrawCommandVisitor::operator()(const draw::SetFont& ft) const
{
    if (ctx_) {
        if (ft.freetype) {
            FT_Face face;
            auto err = FT_New_Face(ft_lib_, ft.family.c_str(), 0, &face);
            if (err != 0) {
                queue_.enqueue(DrawResult { DRAW_RESULT_ERROR, fmt::format("can't load Freetype font: {}", ft.family) });
                return;
            }

            auto fc = cairo_ft_font_face_create_for_ft_face(face, 0);
            if (!fc) {
                queue_.enqueue(DrawResult { DRAW_RESULT_ERROR, fmt::format("can't create cairo font from Freetype: {}", ft.family) });
                return;
            }

            auto status = cairo_font_face_set_user_data(fc, &user_key_,
                face, (cairo_destroy_func_t)FT_Done_Face);

            if (status != 0) {
                cairo_font_face_destroy(fc);
                FT_Done_Face(face);
                queue_.enqueue(DrawResult { DRAW_RESULT_ERROR, fmt::format("can't create font: {}", ft.family) });
                return;
            }

            cairo_set_font_face(ctx_.get(), fc);

        } else {
            cairo_select_font_face(ctx_.get(),
                ft.family.c_str(),
                static_cast<cairo_font_slant_t>(ft.slant),
                static_cast<cairo_font_weight_t>(ft.weight));
        }
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::SetColorRGBA& c) const
{
    if (ctx_)
        cairo_set_source_rgba(ctx_.get(), c.r, c.g, c.b, c.a);
}

void ceammc::DrawCommandVisitor::operator()(const draw::SetMatrix& m) const
{
    if (ctx_) {
        cairo_matrix_t mtx;
        mtx.x0 = m.x0;
        mtx.y0 = m.y0;
        mtx.xx = m.xx;
        mtx.xy = m.xy;
        mtx.yx = m.yx;
        mtx.yy = m.yy;
        cairo_set_matrix(ctx_.get(), &mtx);
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::ResetMatrix&) const
{
    if (ctx_) {
        cairo_identity_matrix(ctx_.get());
        cairo_scale(ctx_.get(), GLOBAL_SCALE, GLOBAL_SCALE);
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::Save&) const
{
    if (ctx_)
        cairo_save(ctx_.get());
}

void ceammc::DrawCommandVisitor::operator()(const draw::Restore&) const
{
    if (ctx_)
        cairo_restore(ctx_.get());
}

void ceammc::DrawCommandVisitor::operator()(const draw::Rotate& r) const
{
    if (ctx_)
        cairo_rotate(ctx_.get(), r.angle);
}

void ceammc::DrawCommandVisitor::operator()(const draw::Translate& r) const
{
    if (ctx_)
        cairo_translate(ctx_.get(), r.x, r.y);
}

void ceammc::DrawCommandVisitor::operator()(const draw::Scale& s) const
{
    if (ctx_)
        cairo_scale(ctx_.get(), s.x, s.y);
}

void ceammc::DrawCommandVisitor::operator()(const draw::Create& c) const
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

void ceammc::DrawCommandVisitor::operator()(const draw::DrawFill& f) const
{
    if (ctx_) {
        f.preserve
            ? cairo_fill_preserve(ctx_.get())
            : cairo_fill(ctx_.get());
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::Ellipse& e) const
{
    if (e.w <= 0 || e.h <= 0)
        return;

    if (ctx_) {
        cairo_matrix_t save_matrix;
        cairo_get_matrix(ctx_.get(), &save_matrix);
        auto cx = e.x + (e.w / 2.0);
        auto cy = e.y + (e.h / 2.0);
        cairo_translate(ctx_.get(), cx, cy);

        if (e.w < e.h)
            cairo_scale(ctx_.get(), e.w / e.h, 1);
        else
            cairo_scale(ctx_.get(), 1, e.h / e.w);

        cairo_translate(ctx_.get(), -cx, -cy);
        cairo_new_path(ctx_.get());
        cairo_arc(
            ctx_.get(),
            cx,
            cy,
            std::max(e.w, e.h) / 2,
            0,
            2 * M_PI);
        cairo_set_matrix(ctx_.get(), &save_matrix);
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::Line& l) const
{
    if (ctx_) {
        cairo_move_to(ctx_.get(), l.x0, l.y0);
        cairo_line_to(ctx_.get(), l.x1, l.y1);
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::LineTo& l) const
{
    if (ctx_)
        cairo_line_to(ctx_.get(), l.x, l.y);
}

void ceammc::DrawCommandVisitor::operator()(const draw::DrawStroke& s) const
{
    if (ctx_) {
        s.preserve
            ? cairo_stroke_preserve(ctx_.get())
            : cairo_stroke(ctx_.get());
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::Text& t) const
{
    if (ctx_) {
        cairo_move_to(ctx_.get(), t.x, t.y);
        cairo_text_path(ctx_.get(), t.str.c_str());
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::QRCode& qr) const
{
    if (ctx_) {
        auto code = qrcodegen::QrCode::encodeText(qr.text.c_str(), qrcodegen::QrCode::Ecc::MEDIUM);
        cairo_save(ctx_.get());
        cairo_translate(ctx_.get(), qr.x, qr.y);
        auto sz = code.getSize();
        for (int x = 0; x < sz; x++) {
            for (int y = 0; y < sz; y++) {
                if (code.getModule(x, y))
                    cairo_rectangle(ctx_.get(), x * qr.pixel, y * qr.pixel, qr.pixel, qr.pixel);
            }
        }
        cairo_restore(ctx_.get());
    }
}

void ceammc::DrawCommandVisitor::operator()(const draw::Sync& c) const
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
#ifndef NDEBUG
            queue_.enqueue(DrawResult { DRAW_RESULT_DEBUG, "SyncImage added" });
#endif
        }

    } else {
        if (CAIRO_STATUS_SUCCESS == cairo_surface_write_to_png_stream(surface_.get(), write_mem_fn, &buf)) {
            queue_.enqueue(DrawResult { DRAW_RESULT_IMAGE, base64_encode(buf.data(), buf.size()) });
#ifndef NDEBUG
            queue_.enqueue(DrawResult { DRAW_RESULT_DEBUG, "SyncImage added" });
#endif
        }
    }

    Dispatcher::instance().send({ c.id, 0 });
}
