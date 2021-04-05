/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "path_search.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"
#include "datatype_string.h"

#include <exception>
#include <iostream>

extern "C" {
#include "s_stuff.h"
}

#include "filesystem.hpp"
namespace fs = ghc::filesystem;

static std::string searchFileTask(
    const std::vector<std::string>& user_paths,
    const std::vector<std::string>& sys_paths,
    const std::string& file,
    bool recursive,
    std::atomic_bool& quit)
{
    std::vector<std::string> relative_user_paths;

    for (auto& p : user_paths) {
        const fs::path user_path(p);

        if (user_path.is_absolute()) {
            auto st = fs::status(user_path);
            // abs search path not found
            if (!fs::is_directory(st))
                continue;

            const fs::path fpath = user_path / file;
            std::cerr << "trying user path: " << fpath << "\n";

            if (fs::exists(fpath)) {
                std::cerr << "found in user: " << fpath << "\n";
                return fpath.string();
            } else if (recursive) {
                for (auto& entry : fs::recursive_directory_iterator(user_path)) {
                    if (quit)
                        return {};

                    auto st = fs::status(entry);
                    if (!fs::is_directory(st))
                        continue;

                    const fs::path fpath = entry / file;
                    std::cerr << "trying user recursive path: " << fpath << "\n";
                    if (fs::exists(fpath)) {
                        std::cerr << "found in user resursive: " << fpath << "\n";
                        return fpath.string();
                    }
                }
            }
        } else {
            relative_user_paths.push_back(p);
        }
    }

    // search in pd standard paths
    for (const auto& p : sys_paths) {
        fs::path path(p);
        if (path.is_absolute()) {
            // abs search path not found
            if (!ghc::filesystem::exists(path))
                continue;

            path.append(file);
            std::cerr << "trying standard path: " << path << "\n";

            if (ghc::filesystem::exists(path)) {
                std::cerr << "found in standard: " << path << "\n";
                return path.string();
            } else if (relative_user_paths.size() > 0) {
                for (const auto& rpath : relative_user_paths) {
                    ghc::filesystem::path path(p);
                    path.append(rpath);
                    path.append(file);

                    if (ghc::filesystem::exists(path)) {
                        std::cerr << "found in relative: " << path << "\n";
                        return path.string();
                    }
                }
            }
        }
    }

    return {};
}

PathSearch::PathSearch(const PdArgs& args)
    : PathAsyncBase(args)
    , paths_(nullptr)
    , recursive_(nullptr)
    , search_stop_(false)
{
    createOutlet();
    createOutlet();

    paths_ = new ListProperty("@paths");
    paths_->setArgIndex(0);
    addProperty(paths_);

    recursive_ = new BoolProperty("@recursive", false);
    addProperty(recursive_);
}

PathSearch::~PathSearch()
{
    search_stop_ = true;
}

void PathSearch::onSymbol(t_symbol* s)
{
    needle_ = s->s_name;
    runTask();
}

void PathSearch::onDataT(const StringAtom& a)
{
    needle_ = a->str();
    runTask();
}

void PathSearch::processResult()
{
    if (result().empty()) {
        OBJ_ERR << "file not found";
        bangTo(1);
    } else {
        symbolTo(0, gensym(result().c_str()));
    }
}

PathSearch::FutureResult PathSearch::createTask()
{
    std::vector<std::string> user_paths;
    user_paths.reserve(paths_->value().size());
    for (auto& s : paths_->value()) {
        if (s.isSymbol()) {
            auto path = s.asT<t_symbol*>()->s_name;
            user_paths.push_back(platform::expand_tilde_path(path));
        }
    }

    std::vector<std::string> sys_paths;
    if (canvas()) {
        // patch directory
        auto cnv_dir = canvas_info_dir(canvas());
        if (cnv_dir)
            sys_paths.push_back(cnv_dir->s_name);

        // patch search paths
        for (auto c : canvas_info_paths(canvas())) {
            if (c.isSymbol())
                sys_paths.push_back(c.asT<t_symbol*>()->s_name);
        }
    }

    // Pd search paths
    for (auto p = STUFF->st_searchpath; p != nullptr; p = p->nl_next)
        sys_paths.push_back(p->nl_string);

    // home directory
    sys_paths.push_back(platform::home_directory());

    for (auto& s : user_paths)
        OBJ_DBG << "user_paths: " << s;

    for (auto& s : sys_paths)
        OBJ_DBG << "sys_paths: " << s;

    return std::async(taskLaunchType(),
        searchFileTask, user_paths, sys_paths, needle_, recursive_->value(), std::ref(search_stop_));
}

void setup_path_search()
{
    ObjectFactory<PathSearch> obj("path.search");
    obj.parseArgsMode(PdArgs::PARSE_UNQUOTE);
    obj.parsePosProps(PdArgs::PARSE_UNQUOTE);
    obj.processData<DataTypeString>();
}
