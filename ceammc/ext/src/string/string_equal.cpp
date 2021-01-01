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
#include "string_equal.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_string.h"
#include "datatype_string.h"

StringEqual::StringEqual(const PdArgs& a)
    : BaseObject(a)
    , pattern_(nullptr)
{
    pattern_ = new StringProperty("@subj");
    pattern_->setArgIndex(0);
    addProperty(pattern_);

    createInlet();
    createOutlet();
}

void StringEqual::onSymbol(t_symbol* s)
{
    boolTo(0, pattern_->str() == s->s_name);
}

void StringEqual::onDataT(const StringAtom& str)
{
    boolTo(0, str->str() == pattern_->str());
}

void StringEqual::onInlet(size_t, const AtomListView& l)
{
    pattern_->setList(l);
}

void setup_string_equal()
{
    ObjectFactory<StringEqual> obj("string.equal");
    obj.processData<DataTypeString>();
    obj.addAlias("str.equal");
    obj.setDescription("check strings or symbols for equality");
    obj.setCategory("string");
    obj.setKeywords({ "compare", "equal" });

    obj.setXletsInfo({ "symbol or String to check", "symbol, quoted string or String" }, { "bool: 1 if strings are equals, 0 otherwise" });
}
