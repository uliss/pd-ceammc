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
#ifndef RESVG_COMMON_H
#define RESVG_COMMON_H

#include "cairo.h"
#include "fmt/core.h"
#include "ui_canvas_cairo.h"

#ifndef NDEBUG
#define OUT_DBG(msg)                                        \
    {                                                       \
        out.enqueue(DrawResult { DRAW_RESULT_DEBUG, msg }); \
    }
#else
#define OUT_DBG(msg)
#endif

#define OUT_ERR(msg)                                        \
    {                                                       \
        using namespace ceammc;                             \
        out.enqueue(DrawResult { DRAW_RESULT_ERROR, msg }); \
    }

#ifdef WITH_RESVG
#include "c-api/resvg.h"

#include <memory>

namespace {

using ReSvgTree = std::unique_ptr<resvg_render_tree*, void (*)(resvg_render_tree**)>;
using ReSvgOpt = std::unique_ptr<resvg_options, void (*)(resvg_options*)>;

// RGBA -> BGRA
void rgba2bgra(unsigned char* data, int w, int h)
{
    for (int i = 0; i < (w * h * 4); i += 4) {
        auto r = data[i + 0];
        data[i + 0] = data[i + 2];
        data[i + 2] = r;
    }
}

resvg_render_tree** resvg_tree_new()
{
    return new resvg_render_tree*;
}

void revsg_tree_delete(resvg_render_tree** t)
{
    if (t) {
        if (*t)
            resvg_tree_destroy(*t);

        delete t;
    }
}

cairo_surface_t* resvg_to_surface(resvg_render_tree* tree, ceammc::UICanvasInQueue& out)
{
    using namespace ceammc;

    auto size = resvg_get_image_size(tree);
    const auto w = (int)size.width;
    const auto h = (int)size.height;

    OUT_DBG(fmt::format("svg size: {}x{}", w, h));

    auto surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);

    /* resvg doesn't support stride, so cairo_surface_t should have no padding */
    assert(cairo_image_surface_get_stride(surface) == (int)size.width * 4);

    auto surface_data = cairo_image_surface_get_data(surface);
    if (!surface_data) {
        OUT_ERR("no data");
        cairo_surface_destroy(surface);
        return nullptr;
    }

    //    OUT_DBG(fmt::format("mtx: a={} b={} c={} d={} e={} f={}", mtx.a, mtx.b, mtx.c, mtx.d, mtx.e, mtx.f));
    resvg_render(tree, resvg_transform_identity(), w, h, (char*)surface_data);

    rgba2bgra(surface_data, w, h);
    cairo_surface_mark_dirty(surface);

    return surface;
}

cairo_surface_t* load_svg(const char* path, ceammc::UICanvasInQueue& out)
{
    ReSvgOpt opt { resvg_options_create(), resvg_options_destroy };
    resvg_options_load_system_fonts(opt.get());

    ReSvgTree tree { resvg_tree_new(), revsg_tree_delete };
    auto err = resvg_parse_tree_from_file(path, opt.get(), tree.get());
    if (err != RESVG_OK)
        return nullptr;

    return resvg_to_surface(*tree, out);
}

}

#else
namespace {
cairo_surface_t* load_svg(const char* path, ceammc::UICanvasInQueue& out)
{
    OUT_ERR(fmt::format("can't load '{}': compiled without SVG support", path));
    return nullptr;
}
}
#endif

#endif // RESVG_COMMON_H
