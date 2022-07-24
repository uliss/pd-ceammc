/*****************************************************************************
 * Copyright 2022 Serge Poltavski. All rights reserved.
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
#include "path_pattern.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"
#include "fmt/core.h"
#include "new_file_pattern.h"

#include "filesystem.hpp"
namespace fs = ghc::filesystem;

fs::path make_new_filename(const fs::path& pattern)
{
    auto ext = pattern.extension();
    auto stem = pattern.stem();
    std::string subst;
    // checking if file exists in cycle
    for (int i = 1; i < 1000; i++) {
        subst.clear();

        switch (path::make_new_filename_by_pattern(stem.string(), subst, i)) {
        case path::PATTERN_DONE: {
            fs::path new_path(pattern);
            new_path.replace_filename(subst + ext.string());
            if (fs::exists(new_path)) {
                if (fs::is_regular_file(new_path))
                    continue;

                LIB_ERR << fmt::format("pattern is not a regular file: '{}'", new_path.string());
                return {};
            } else
                return new_path;

        } break;
        case path::PATTERN_NOMATCH: {
            if (fs::exists(pattern)) {
                fs::path new_path(pattern);
                new_path.replace_filename(subst + ext.string());
                // substituted pattern file exists
                if (fs::exists(new_path)) {
                    if (fs::is_regular_file(new_path))
                        continue;

                    LIB_ERR << fmt::format("pattern is not a regular file: '{}'", pattern.string());
                    return {};
                } else
                    return new_path;
            } else
                return pattern;

        } break;
        default:
            LIB_ERR << fmt::format("invalid pattern: '{}'", pattern.string());
            return {};
        }
    }

    LIB_ERR << fmt::format("can't substitue pattern: '{}'", pattern.string());
    return {};
}

PathPattern::PathPattern(const PdArgs& args)
    : BaseObject(args)
    , pattern_(nullptr)
{
    pattern_ = new SymbolProperty("@pattern", &s_);
    pattern_->setArgIndex(0);
    addProperty(pattern_);

    createOutlet();
}

void PathPattern::onBang()
{
    auto name = pattern_->value();
    if (!name || !name->s_name || !name->s_name[0]) {
        OBJ_ERR << "empty pattern";
        return;
    }

    fs::path path(name->s_name);
    fs::path new_path;

    if (path.is_absolute()) {
        new_path = make_new_filename(path);
    } else {
        auto cnv = canvas();
        if (!cnv) {
            OBJ_ERR << fmt::format("null canvas pointer");
            return;
        }

        auto dir = canvas_getdir(cnv);
        if (!dir) {
            OBJ_ERR << fmt::format("canvas_getdir() failed");
            return;
        }

        fs::path full_path(dir->s_name);
        full_path /= path;

        new_path = make_new_filename(full_path);
    }

    if (!new_path.empty())
        symbolTo(0, gensym(new_path.string().c_str()));
}

void PathPattern::onSymbol(t_symbol* path)
{
    pattern_->setValue(path);
    onBang();
}

bool PathPattern::checkPattern(t_symbol* name)
{
    if (!name || !name->s_name || !name->s_name[0]) {
        OBJ_ERR << "no filename pattern specified";
        return false;
    }

    return true;
}

void setup_path_pattern()
{
    ObjectFactory<PathPattern> obj("path.pattern");
}
