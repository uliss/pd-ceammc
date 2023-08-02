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
#ifndef UI_CANVAS_CAIRO_H
#define UI_CANVAS_CAIRO_H

#include <memory>

#include <boost/variant.hpp>
#include <cairo.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "readerwriterqueue.h"
#include "ui_canvas_impl.h"

namespace {
constexpr float GLOBAL_SCALE = 2;
}

namespace ceammc {

enum DrawResultType {
    DRAW_RESULT_IMAGE,
    DRAW_RESULT_DEBUG,
    DRAW_RESULT_ERROR
};

struct DrawResult {
    DrawResultType type;
    std::string data;
};

using CairoContext = std::unique_ptr<cairo_t, void (*)(cairo_t*)>;
using CairoSurface = std::unique_ptr<cairo_surface_t, void (*)(cairo_surface_t*)>;
using UICanvasInQueue = moodycamel::ReaderWriterQueue<DrawResult>;

class DrawCommandVisitor : public boost::static_visitor<void> {
    CairoSurface& surface_;
    CairoContext& ctx_;
    UICanvasInQueue& queue_;
    FT_Library ft_lib_;
    cairo_user_data_key_t user_key_;
    std::string library_path_;

public:
    DrawCommandVisitor(CairoSurface& surface, CairoContext& ctx, UICanvasInQueue& in);
    ~DrawCommandVisitor();

    void setLibraryPath(const std::string& path);

    void operator()(const draw::NextVariant& n) const;

    void operator()(const draw::ClosePath&) const;
    void operator()(const draw::NewPath&) const;
    void operator()(const draw::NewSubPath&) const;

    void operator()(const draw::Arc& a) const;
    void operator()(const draw::Circle& c) const;
    void operator()(const draw::Curve& c) const;
    void operator()(const draw::Line& l) const;
    void operator()(const draw::LineTo& l) const;
    void operator()(const draw::MoveBy& c) const;
    void operator()(const draw::MoveTo& c) const;
    void operator()(const draw::Polygon& p) const;
    void operator()(const draw::Rect& c) const;
    void operator()(const draw::Text& t) const;
    void operator()(const draw::QRCode& q) const;

    void operator()(const draw::Create& c) const;
    void operator()(const draw::Sync& c) const;

    void operator()(const draw::DrawBackground&) const;
    void operator()(const draw::DrawFill& f) const;
    void operator()(const draw::DrawImage& c) const;
    void operator()(const draw::DrawMusic&) const;
    void operator()(const draw::Restore&) const;
    void operator()(const draw::Save&) const;
    void operator()(const draw::DrawStroke& s) const;

    void operator()(const draw::ResetMatrix& m) const;
    void operator()(const draw::Rotate& r) const;
    void operator()(const draw::Scale& s) const;
    void operator()(const draw::SetColorRGBA& c) const;
    void operator()(const draw::SetDash& dash) const;
    void operator()(const draw::SetFont& ft) const;
    void operator()(const draw::SetFontSize& sz) const;
    void operator()(const draw::SetLineCap& cap) const;
    void operator()(const draw::SetMatrix& m) const;
    void operator()(const draw::SetStrokeWidth& c) const;
    void operator()(const draw::Translate& r) const;
};

}

#endif // UI_CANVAS_CAIRO_H
