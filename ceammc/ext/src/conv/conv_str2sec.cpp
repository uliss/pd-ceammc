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
#include "conv_str2sec.h"
#include "ceammc_factory.h"
#include "datatype_string.h"

#include "ceammc_convert.h"

StrToSec::StrToSec(const PdArgs& a)
    : BaseObject(a)
    , on_err_(nullptr)
{
    createOutlet();

    on_err_ = new FloatProperty("@on_err", -1);
    addProperty(on_err_);
}

void StrToSec::onSymbol(t_symbol* s)
{
    floatTo(0, convert::time::str2sec(s->s_name, on_err_->value()));
}

void StrToSec::onAny(t_symbol* s, const AtomListView& /*v*/)
{
    floatTo(0, convert::time::str2sec(s->s_name, on_err_->value()));
}

void StrToSec::onDataT(const StringAtom& str)
{
    floatTo(0, convert::time::str2sec(str->str(), on_err_->value()));
}

void setup_conv_str2sec()
{
    ObjectFactory<StrToSec> obj("conv.str2sec");
    obj.addAlias("str->sec");
    obj.processData<DataTypeString>();
}
