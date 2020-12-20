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
#include "conv_dbfs2amp.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

static t_float dbfs2amp_(t_float v)
{
    static const t_float threshold = -144;
    if (v <= threshold)
        return 0;

    return convert::dbfs2amp(v);
}

Dbfs2amp::Dbfs2amp(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void Dbfs2amp::onFloat(t_float v)
{
    floatTo(0, dbfs2amp_(v));
}

void Dbfs2amp::onList(const AtomList& args)
{
    listTo(0, args.mapFloat(dbfs2amp_));
}

Dbfs2ampTilde::Dbfs2ampTilde(const PdArgs& args)
    : SoundExternal(args)
{
    createSignalOutlet();
}

void Dbfs2ampTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    for (size_t i = 0; i < BS; i++)
        out[0][i] = dbfs2amp_(in[0][i]);
}

void setup_conv_dbfs2amp()
{
    ObjectFactory<Dbfs2amp> obj1("conv.dbfs2amp");
    obj1.addAlias("dbfs->amp");

    ObjectFactory<Dbfs2amp> obj2("conv.dbfs2amp~");
    obj1.addAlias("dbfs->amp~");
}
