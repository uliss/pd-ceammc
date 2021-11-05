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
#include "path_parent.h"
#include "ceammc_factory.h"
#include "datatype_string.h"

#include "filesystem.hpp"
namespace fs = ghc::filesystem;

PathParent::PathParent(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void PathParent::onSymbol(t_symbol* s)
{
    symbolTo(0, gensym(fs::path(s->s_name).parent_path().c_str()));
}

void PathParent::onDataT(const StringAtom& s)
{
    atomTo(0, StringAtom(fs::path(s->str()).parent_path().string()));
}

void setup_path_parent()
{
    ObjectFactory<PathParent> obj("path.parent");
    obj.processData<DataTypeString>();
}
