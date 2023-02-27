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
#include "file_size.h"
#include "ceammc_factory.h"
#include "datatype_string.h"

#include "filesystem.hpp"
namespace fs = ghc::filesystem;

FileSize::FileSize(const PdArgs& args)
    : PathAsyncBase(args)
{
    createOutlet();
}

void FileSize::onSymbol(t_symbol* s)
{
    file_ = s->s_name;
    runTask();
}

void FileSize::onDataT(const StringAtom& s)
{
    file_ = s->str();
    runTask();
}

void FileSize::processResult()
{
    floatTo(0, result());
}

FileSize::FutureResult FileSize::createTask()
{
    return std::async(
        taskLaunchType(),
        [](const std::string& path) -> int {
            fs::path p(path);
            if (fs::is_regular_file(p))
                return fs::file_size(p);

            throw std::runtime_error("can't get file size: " + path);
        },
        file_);
}

void setup_file_size()
{
    ObjectFactory<FileSize> obj("file.size");
    obj.processData<DataTypeString>();
}
