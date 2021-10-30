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
#include "path_normalize.h"
#include "ceammc_factory.h"
#include "datatype_string.h"

#include "filesystem.hpp"
namespace fs = ghc::filesystem;

PathNormalize::PathNormalize(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void PathNormalize::onSymbol(t_symbol* s)
{
    fs::path p(s->s_name);
    symbolTo(0, gensym(p.lexically_normal().generic_string().c_str()));
}

void PathNormalize::onDataT(const StringAtom& s)
{
    fs::path p(s->str());
    atomTo(0, StringAtom(p.lexically_normal().generic_string()));
}

void setup_path_normalize()
{
    ObjectFactory<PathNormalize> obj("path.normalize");
    obj.addAlias("path.norm");
    obj.processData<DataTypeString>();
}
