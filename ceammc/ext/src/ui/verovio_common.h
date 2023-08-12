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
#ifndef VEROVIO_COMMON_H
#define VEROVIO_COMMON_H

#include "cairo.h"
#include "resvg_common.h"
#include "ui_canvas_cairo.h"

#include <string>

#ifdef WITH_VEROVIO
#include "ceammc_filesystem.h"
#include "ceammc_platform.h"
#include "fmt/core.h"
#include "verovio/tools/c_wrapper.h"
#include <memory>

namespace {

using Verovio = std::unique_ptr<void, void (*)(void*)>;

constexpr const char* VEROVIO_RES = "/music/verovio";

cairo_surface_t* load_music(const ceammc::draw::DrawMusic& mus, const std::string& lib_path, ceammc::UICanvasInQueue& out)
{
    using namespace ceammc;

    auto res_path = lib_path + VEROVIO_RES;
    OUT_DBG(fmt::format("verovio resource directory: '{}'", res_path));

    // check dir
    if (!platform::path_exists(res_path.c_str())) {
        OUT_ERR(fmt::format("verovio resource directory not found: '{}'", res_path));
        return nullptr;
    }

    // check at least one file
    if (!platform::path_exists((res_path + "/Bravura.xml").c_str())) {
        OUT_ERR(fmt::format("verovio resource directory seems empty: '{}'", res_path));
        return nullptr;
    }

    Verovio vo(vrvToolkit_constructorResourcePath(res_path.c_str()), vrvToolkit_destructor);

    OUT_DBG(fmt::format("verovio version: {}", vrvToolkit_getVersion(vo.get())));

    if (!vrvToolkit_setOptions(vo.get(), R"({"scale": 75})")) {
        OUT_ERR("can't set option: scale=75");
        return nullptr;
    }

    switch (mus.format) {
    case draw::FORMAT_ABC:
        // set ABC option
        if (!vrvToolkit_setOptions(vo.get(), R"({"inputFrom": "abc"})")) {
            OUT_ERR("can't set option: inputFrom=abc");
            return nullptr;
        }

        // load ABC string
        if (!vrvToolkit_loadData(vo.get(), mus.data.c_str())) {
            OUT_ERR(fmt::format("can't load data: '{}'", mus.data));
            return nullptr;
        }

        break;
    case draw::FORMAT_MUSICXML: {
        // set MusicXML option
        if (!vrvToolkit_setOptions(vo.get(), R"({"inputFrom": "xml"})")) {
            OUT_ERR("can't set option: inputFrom=xml");
            return nullptr;
        }

        auto res = ceammc::fs::readFileContent(mus.data.c_str());
        std::string xml;
        if (res.matchValue(xml)) {
            // load MusicXML string
            if (!vrvToolkit_loadData(vo.get(), xml.c_str())) {
                OUT_ERR(fmt::format("can't load data: '{}'", mus.data));
                return nullptr;
            }
        } else {
            OUT_ERR(fmt::format("can't read file: '{}'", mus.data));
            return nullptr;
        }
    } break;
    default:
        break;
    }

    auto svg = vrvToolkit_renderToSVG(vo.get(), 1, true);

    ReSvgOpt opt = { resvg_options_create(), resvg_options_destroy };
    //    resvg_options_load_system_fonts(opt);
    ReSvgTree tree { resvg_tree_new(), revsg_tree_delete };
    int err = resvg_parse_tree_from_data(svg, strlen(svg), opt.get(), tree.get());
    if (err != RESVG_OK)
        return nullptr;

    vo.release();

    return resvg_to_surface(*tree, out);
}
}
#else
namespace {
cairo_surface_t* load_music(const ceammc::draw::DrawMusic& mus, const std::string& lib_path, ceammc::UICanvasInQueue& out)
{
    OUT_ERR("compiled without music support");
    return nullptr;
}
}
#endif

#endif // VEROVIO_COMMON_H
