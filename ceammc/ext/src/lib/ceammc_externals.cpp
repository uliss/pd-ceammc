/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "ceammc_externals.h"

namespace ceammc {

static ExternalSet ui_ext, base_ext, faust_ext, flext_ext;

const ExternalSet& ui_external_set()
{
    return ui_ext;
}

const ExternalSet& base_external_set()
{
    return base_ext;
}

const ExternalSet& faust_external_set()
{
    return faust_ext;
}

const ExternalSet& flext_external_set()
{
    return flext_ext;
}

void register_ui_external(t_class* c)
{
    ui_ext.insert(c);
}

void register_base_external(t_class* c)
{
    base_ext.insert(c);
}

void register_faust_external(t_class* c)
{
    faust_ext.insert(c);
}

void register_flext_external(t_class* c)
{
    flext_ext.insert(c);
}

}
