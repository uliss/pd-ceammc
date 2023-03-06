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

#include "ceammc_poll_dispatcher.h"

#include <boost/variant.hpp>
#include <string>

namespace ceammc {
namespace draw {

    struct DrawCircle {
        float x, y, r;
    };

    struct DrawRect {
        float x, y, w, h;
    };

    struct DrawLine {
        float x0, y0, x1, y1;
    };

    struct DrawText {
        float x, y;
        std::string str;
    };

    struct MoveTo {
        float x, y;
    };

    struct SetStrokeWidth {
        float w;
    };

    struct SetFontSize {
        float size;
    };

    struct SetColorRGBA {
        float r, g, b, a;
    };

    struct CreateImage {
        int w, h;
    };

    struct DrawBackground { };
    struct DrawFill { };
    struct DrawStroke { };
    struct DrawSave { };
    struct DrawRestore { };
    struct SyncImage {
        SyncImage(SubscriberId pid, float pzoom)
            : id(pid)
            , zoom(pzoom)
        {
        }
        SubscriberId id;
        float zoom;
    };

    using DrawCommand = boost::variant<
        CreateImage,
        DrawBackground,
        DrawCircle,
        DrawFill,
        DrawLine,
        DrawRect,
        DrawStroke,
        DrawSave,
        DrawRestore,
        DrawText,
        MoveTo,
        SetColorRGBA,
        SetFontSize,
        SetStrokeWidth,
        SyncImage

        >;

}
}

#endif // UI_CANVAS_IMPL_H
