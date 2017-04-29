/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "expand_env.h"
#include "ceammc_platform.h"

extern "C" void expand_env_setup()
{
    ObjectFactory<ExpandEnv> obj("expand_env");
}

static t_symbol* expandEnv(t_symbol* s)
{
    return gensym(platform::expandenv(s->s_name).c_str());
}

ExpandEnv::ExpandEnv(const PdArgs& a)
    : BaseObject(a)
    , expand_any_(0)
{
    createOutlet();

    expand_any_ = new FlagProperty("@any");
    createProperty(expand_any_);
}

void ExpandEnv::onBang()
{
    bangTo(0);
}

void ExpandEnv::onFloat(float v)
{
    floatTo(0, v);
}

void ExpandEnv::onSymbol(t_symbol* s)
{
    symbolTo(0, expandEnv(s));
}

void ExpandEnv::onList(const AtomList& l)
{
    listTo(0, l.map(&expandEnv));
}

void ExpandEnv::onAny(t_symbol* sel, const AtomList& l)
{
    anyTo(0, expand_any_->value() ? expandEnv(sel) : sel, l.map(&expandEnv));
}
