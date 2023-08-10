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
#ifndef UI_CANVAS_IMPL_H
#define UI_CANVAS_IMPL_H

#include "ceammc_notify.h"

#include <boost/variant.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace ceammc {
namespace draw {

    enum MusicFormat : std::int8_t {
        FORMAT_ABC,
        FORMAT_MUSICXML,
        FORMAT_HUMDRUM,
        FORMAT_MEI
    };

    struct ClosePath { };
    struct NewPath { };
    struct NewSubPath { };

    struct Circle {
        float x, y, r;
    };

    struct Rect {
        float x, y, w, h;
    };

    struct Line {
        float x0, y0, x1, y1;
    };

    struct Arc {
        float x, y, r, a0, a1;
    };

    struct Curve {
        float x0, y0, x1, y1, x2, y2, x3, y3;
    };

    struct Polygon {
        std::vector<float> data;
    };

    struct Text {
        float x, y;
        std::string str;
    };

    struct DrawImage {
        float x, y;
        std::string path;
    };

    struct LineTo {
        float x, y;
    };

    struct MoveTo {
        float x, y;
    };

    struct MoveBy {
        float dx, dy;
    };

    struct Rotate {
        float angle;
    };

    struct Translate {
        float x, y;
    };

    struct Scale {
        float x, y;
    };

    struct SetStrokeWidth {
        float w;
    };

    struct SetLineCap {
        int type;
        SetLineCap(int t)
            : type(t)
        {
        }
    };

    struct SetDash {
        static const size_t MAX_DASHES = 4;
        std::uint8_t dashes[MAX_DASHES] { 2, 0, 0, 0 };
        std::uint8_t n { 1 };
    };

    struct SetFont {
        std::string family;
        std::uint8_t slant, weight, freetype { 0 };
    };

    struct SetFontSize {
        float size;
    };

    struct SetColorRGBA {
        float r, g, b, a;
    };

    struct SetMatrix {
        float xx;
        float yx;
        float xy;
        float yy;
        float x0;
        float y0;
    };

    struct ResetMatrix { };

    struct Save { };
    struct Restore { };

    struct DrawBackground { };
    struct DrawFill {
        DrawFill(bool v)
            : preserve(v)
        {
        }
        bool preserve;
    };
    struct DrawStroke {
        DrawStroke(bool v)
            : preserve(v)
        {
        }
        bool preserve;
    };

    struct DrawMusic {
        std::string data;
        std::int16_t x, y, pixel;
        MusicFormat format;
    };

    struct QRCode {
        std::string text;
        std::int16_t x, y, pixel;
    };

    struct Create {
        int w, h;
    };

    struct Sync {
        Sync(SubscriberId pid, float pzoom)
            : id(pid)
            , zoom(pzoom)
        {
        }
        SubscriberId id;
        float zoom;
    };

    using NextVariant = boost::variant<
        Curve,
        Arc,
        DrawImage,
        LineTo,
        Polygon,
        Save,
        SetFont,
        SetMatrix,
        ResetMatrix,
        Scale,
        ClosePath,
        NewPath,
        NewSubPath,
        QRCode,
        DrawMusic
        //
        >;

    using DrawCommand = boost::variant<
        Create,
        DrawBackground,
        Circle,
        DrawFill,
        Line,
        Rect,
        Restore,
        DrawStroke,
        Text,
        MoveBy,
        MoveTo,
        Rotate,
        SetColorRGBA,
        SetDash,
        SetFontSize,
        SetLineCap,
        SetStrokeWidth,
        Sync,
        Translate,

        NextVariant //
        >;
}
}

#endif // UI_CANVAS_IMPL_H
