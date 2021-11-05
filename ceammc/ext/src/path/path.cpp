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
#include "path.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_string.h"

#include "filesystem.hpp"
namespace fs = ghc::filesystem;

Path::Path(const PdArgs& args)
    : PathAsyncBase<std::string>(args)
    , state_(INIT)
    , max_depth_(nullptr)
    , async_quit_(false)
{
    createOutlet();

    path_ = to_string(parsedPosArgs());

    max_depth_ = new IntProperty("@depth", 3);
    max_depth_->checkClosedRange(0, 10);
    addProperty(max_depth_);

    max_entries_ = new IntProperty("@maxnum", 0);
    addProperty(max_entries_);
}

Path::~Path()
{
    async_quit_ = true;
}

void Path::onBang()
{
    atomTo(0, StringAtom(path_));
}

void Path::onSymbol(t_symbol* s)
{
    path_ = s->s_name;
}

void Path::m_search(t_symbol* s, const AtomListView& lv)
{
    needle_ = to_string(lv);
    state_ = SEARCH;
    runTask();
}

void Path::m_append(t_symbol* s, const AtomListView& lv)
{
    path_ = fs::path(path_) / fs::path(to_string(lv));
}

void Path::processResult()
{
    switch (state_) {
    case SEARCH:
        atomTo(0, StringAtom(result()));
        break;
    default:
        break;
    }

    state_ = INIT;
}

Path::FutureResult Path::createTask()
{
    switch (state_) {
    case SEARCH:
        OBJ_DBG << "search for '" << needle_ << "' in '" << path_ << "'";

        return std::async(
            taskLaunchType(),
            [](const std::string& path, const std::string& name, int max_depth, int max_num, std::atomic_bool& flag_quit) -> std::string {
                const fs::path dir(path);
                if (!fs::is_directory(dir))
                    throw std::runtime_error("not a directory: '" + path + "'");

                // non-recursive search optimization
                if (max_depth == 0) {
                    const auto fname = dir / fs::path(name).filename();
                    return fs::exists(fname) ? fname.string() : "";
                }

                const auto fname = fs::path(name).filename();
                auto it = fs::recursive_directory_iterator(dir);
                while (it != fs::recursive_directory_iterator()) {
                    try {
                        if (flag_quit)
                            break;

                        if (--max_num == 0)
                            break;

                        if (max_depth > 0 && it.depth() >= max_depth)
                            it.disable_recursion_pending();

                        if (it->path().filename() == fname)
                            return it->path().lexically_normal().string();

                        ++it;

                    } catch (std::exception& e) {
                        it.pop();
                    }
                }

                return {};
            },
            path_, needle_, max_depth_->value(), max_entries_->value(), std::ref(async_quit_));
        break;
    }
}

void setup_path_object()
{
    ObjectFactory<Path> obj("path");
    obj.parseArgsMode(PdArgs::PARSE_UNQUOTE);

    obj.addMethod("search", &Path::m_search);
}
