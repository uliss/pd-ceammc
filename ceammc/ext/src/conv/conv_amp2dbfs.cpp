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
#include "conv_amp2dbfs.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

static const t_float theshold = 1.0 / double(1L << 24);

static t_float a2d(t_float v)
{
    static const t_float low_limit = ceilf(convert::amp2dbfs(theshold));

    if (v <= theshold)
        return low_limit;

    return convert::amp2dbfs(v);
}

Amp2dbfs::Amp2dbfs(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void Amp2dbfs::onFloat(t_float v)
{

    floatTo(0, a2d(v));
}

void Amp2dbfs::onList(const AtomList& lst)
{
    listTo(0, lst.mapFloat(a2d));
}

void setup_conv_amp2dbfs()
{
    ObjectFactory<Amp2dbfs> obj("conv.amp2dbfs");
    obj.addAlias("amp->dbfs");
}
