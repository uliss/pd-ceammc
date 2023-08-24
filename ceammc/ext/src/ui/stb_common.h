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
#ifndef STB_COMMON_H
#define STB_COMMON_H

#include "cairo.h"
#include "fmt/core.h"
#include "resvg_common.h"
#include "ui_canvas_cairo.h"

#define WITH_STB

#ifdef WITH_STB
#define STBI_NO_HDR
#define STBI_ONLY_JPEG
#define STBI_ONLY_BMP
#define STBI_ONLY_GIF
#define STB_IMAGE_IMPLEMENTATION
#include "../../extra/stb/stb_image.h"

namespace {

void stb_rgba2bgra(unsigned char* data, int w, int h)
{
    for (int i = 0; i < (w * h * 4); i += 4) {
        auto r = data[i + 0];
        data[i + 0] = data[i + 2];
        data[i + 2] = r;
    }
}

cairo_surface_t* load_stb(const char* path, ceammc::UICanvasInQueue& out)
{
    using namespace ceammc;

    int x, y, nchan;
    auto data = stbi_load(path, &x, &y, &nchan, 4);
    if (!data) {
        OUT_ERR(fmt::format("can't open file '{}': {}", path, stbi_failure_reason()));
        return nullptr;
    }

    OUT_DBG(fmt::format("image size: {}x{}", x, y));
    //    OUT_DBG(fmt::format("image strides: w:{} h:{} c:{}", layout.width_stride, layout.height_stride, layout.channel_stride));
    auto surface = cairo_image_surface_create_for_data(data,
        CAIRO_FORMAT_ARGB32, x, y, cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, x));

    auto surface_data = cairo_image_surface_get_data(surface);
    if (!surface_data) {
        OUT_ERR("no data");
        cairo_surface_destroy(surface);
        stbi_image_free(data);
        return nullptr;
    }

    stb_rgba2bgra(data, x, y);
    cairo_surface_mark_dirty(surface);
    stbi_image_free(data);

    return surface;
}
}
#else
namespace {
cairo_surface_t* load_rimg(const char* path, ceammc::UICanvasInQueue& out)
{
    OUT_ERR(fmt::format("can't load file: '{}'. compiled without STB support", path));
    return nullptr;
}
}
#endif

#endif // STB_COMMON_H
