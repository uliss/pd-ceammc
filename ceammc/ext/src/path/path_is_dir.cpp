/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "path_is_dir.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"
#include "datatype_string.h"

PathIsDir::PathIsDir(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();
}

void PathIsDir::onSymbol(t_symbol* s)
{
    std::string path = platform::expand_tilde_path(s->s_name);

    if (platform::is_dir(path.c_str())) {
        symbolTo(1, s);
        boolTo(0, true);
    } else
        boolTo(0, false);
}

void PathIsDir::onDataT(const StringAtom& sa)
{
    std::string path = platform::expand_tilde_path(sa->str());

    if (platform::is_dir(path.c_str())) {
        atomTo(1, sa);
        boolTo(0, true);
    } else
        boolTo(0, false);
}

void setup_path_is_dir()
{
    ObjectFactory<PathIsDir> obj("path.is_dir");
    obj.processData<DataTypeString>();
}
