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
#ifndef RIMG_COMMON_H
#define RIMG_COMMON_H

#include "cairo.h"
#include "fmt/core.h"
#include "resvg_common.h"
#include "ui_canvas_cairo.h"

#ifdef WITH_RIMG
#include "rimg.h"

namespace {

void rimg_rgba2bgra(unsigned char* data, int w, int h)
{
    for (int i = 0; i < (w * h * 4); i += 4) {
        auto r = data[i + 0];
        data[i + 0] = data[i + 2];
        data[i + 2] = r;
    }
}

cairo_surface_t* load_rimg(const char* path, ceammc::UICanvasInQueue& out)
{
    using namespace ceammc;

    RImgSampleLayout layout;
    RImgResult result;
    auto rc = rimg_decode_image(path, &layout, &result);
    switch (rc) {
    case RImgResultCode::FileNotFound: {
        OUT_ERR(fmt::format("can't open file: '{}'", path));
        return nullptr;
    } break;
    case RImgResultCode::InvalidFileName: {
        OUT_ERR("invalid file name");
        return nullptr;
    } break;
    case RImgResultCode::InvalidString: {
        OUT_ERR("invalid string");
        return nullptr;
    } break;
    case RImgResultCode::UnknownImageFormat: {
        OUT_ERR(fmt::format("unknown image format: '{}'", path));
        return nullptr;
    } break;
    case RImgResultCode::Ok:
        break;
    default:
        OUT_ERR(fmt::format("unknown result code: {}", (int)rc));
        return nullptr;
    }

    OUT_DBG(fmt::format("image size: {}x{}", layout.width, layout.height));
    OUT_DBG(fmt::format("image strides: w:{} h:{} c:{}", layout.width_stride, layout.height_stride, layout.channel_stride));
    auto surface = cairo_image_surface_create_for_data((unsigned char*)result.data,
        CAIRO_FORMAT_ARGB32,
        layout.width,
        layout.height,
        layout.height_stride);

    /* resvg doesn't support stride, so cairo_surface_t should have no padding */
    assert(cairo_image_surface_get_stride(surface) == (int)layout.width * 4);

    auto surface_data = cairo_image_surface_get_data(surface);
    if (!surface_data) {
        OUT_ERR("no data");
        cairo_surface_destroy(surface);
        rimg_free_result(&result);
        return nullptr;
    }

    rimg_rgba2bgra(surface_data, layout.width, layout.height);
    cairo_surface_mark_dirty(surface);
    rimg_free_result(&result);

    return surface;
}
}
#else
namespace {
cairo_surface_t* load_rimg(const char* path, ceammc::UICanvasInQueue& out)
{
    OUT_ERR(fmt::format("can't load file: '{}'. compiled without RImg support", path));
    return nullptr;
}
}
#endif

#endif // RIMG_COMMON_H
