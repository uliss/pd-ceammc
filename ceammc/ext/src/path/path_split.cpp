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
#include "path_split.h"
#include "ceammc_factory.h"
#include "datatype_string.h"

#include "filesystem.hpp"
namespace fs = ghc::filesystem;

PathSplit::PathSplit(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();
}

void PathSplit::onSymbol(t_symbol* s)
{
    fs::path p(s->s_name);

    symbolTo(1, gensym(p.filename().string().c_str()));
    symbolTo(0, gensym(p.parent_path().string().c_str()));
}

void PathSplit::onDataT(const StringAtom& s)
{
    fs::path p(s->str());

    atomTo(1, StringAtom(p.filename().string()));
    atomTo(0, StringAtom(p.parent_path().string()));
}

void setup_path_split()
{
    ObjectFactory<PathSplit> obj("path.split");
    obj.processData<DataTypeString>();

    obj.setXletsInfo({ "symbol: path\n"
                       "data:string: path" },
        { "symbol: dirname\n"
          "data:string: dirname",
            "symbol: basename\n"
            "data:string: basename" });
}
