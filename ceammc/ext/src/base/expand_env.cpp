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
    addProperty(expand_any_);
}

void ExpandEnv::onBang()
{
    bangTo(0);
}

void ExpandEnv::onFloat(t_float v)
{
    floatTo(0, v);
}

void ExpandEnv::onSymbol(t_symbol* s)
{
    symbolTo(0, expandEnv(s));
}

void ExpandEnv::onList(const AtomList& l)
{
    listTo(0, l.mapSymbol(&expandEnv));
}

void ExpandEnv::onAny(t_symbol* sel, const AtomListView& v)
{
    AtomList l = v;
    anyTo(0, expand_any_->value() ? expandEnv(sel) : sel, l.mapSymbol(&expandEnv));
}

void setup_base_expand_env()
{
    ObjectFactory<ExpandEnv> obj("expand_env");

    obj.setDescription("Substitute environment variables in data stream");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({"expand_env", "test"});
    obj.setCategory("flow");
    obj.setSinceVersion(0, 1);
}
