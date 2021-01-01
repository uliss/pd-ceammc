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
#include "string_replace.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_string.h"

static t_symbol* REPLACE_ALL;
static t_symbol* REPLACE_FIRST;
static t_symbol* REPLACE_LAST;

StringReplace::StringReplace(const PdArgs& a)
    : BaseObject(a)
    , mode_(nullptr)
{
    createInlet();
    createInlet();
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { REPLACE_ALL, REPLACE_FIRST, REPLACE_LAST });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@all", mode_, REPLACE_ALL));
    addProperty(new SymbolEnumAlias("@first", mode_, REPLACE_FIRST));
    addProperty(new SymbolEnumAlias("@last", mode_, REPLACE_LAST));

    createCbSymbolProperty(
        "@from",
        [this]() -> t_symbol* { return gensym(from_.c_str()); },
        [this](t_symbol* s) -> bool { from_ = s->s_name; return true; })
        ->setArgIndex(0);

    createCbSymbolProperty(
        "@to",
        [this]() -> t_symbol* { return gensym(to_.c_str()); },
        [this](t_symbol* s) -> bool { to_ = s->s_name; return true; })
        ->setArgIndex(1);
}

void StringReplace::onSymbol(t_symbol* s)
{
    onDataT(StringAtom(s));
}

void StringReplace::onDataT(const StringAtom& str)
{
    if (mode_->value() == REPLACE_ALL) {
        atomTo(0, StringAtom(str->replaceAll(from_, to_)));
    } else if (mode_->value() == REPLACE_FIRST) {
        atomTo(0, StringAtom(str->replaceFirst(from_, to_)));
    } else if (mode_->value() == REPLACE_LAST) {
        atomTo(0, StringAtom(str->replaceLast(from_, to_)));
    }
}

void StringReplace::onInlet(size_t n, const AtomListView& l)
{
    if (n == 1) {
        from_ = to_string(l);
        return;
    }

    if (n == 2) {
        to_ = to_string(l);
        return;
    }
}

void setup_string_replace()
{
    REPLACE_ALL = gensym("all");
    REPLACE_FIRST = gensym("first");
    REPLACE_LAST = gensym("last");

    ObjectFactory<StringReplace> obj("string.replace");
    obj.processData<DataTypeString>();
    obj.addAlias("str.replace");
}
