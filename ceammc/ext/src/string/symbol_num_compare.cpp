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
#include "symbol_num_compare.h"
#include "ceammc_factory.h"

#include <cctype>
#include <cstring>

SymbolNumCompare::SymbolNumCompare(const PdArgs& a)
    : BaseObject(a)
    , cmp_with_(nullptr)
{
    createInlet(&cmp_with_);
    createOutlet();

    addProperty(new PointerProperty<t_symbol*>("@with", &cmp_with_, PropValueAccess::READWRITE))
        ->setArgIndex(0);
}

void SymbolNumCompare::onSymbol(t_symbol* s)
{
    if (!cmp_with_) {
        OBJ_ERR << "compare pattern is not specified";
        return;
    }

    int s1 = numPart(s);
    int s2 = numPart(cmp_with_);

    floatTo(0, int(s1 < s2));
}

void SymbolNumCompare::onList(const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_SYMBOL))
        return;

    cmp_with_ = lv[1].asSymbol();
    onSymbol(lv[0].asSymbol());
}

int SymbolNumCompare::numPart(t_symbol* s)
{
    if (!s)
        return 0;

    int res = 0;
    const char* ptr = s->s_name;
    while (*ptr != '\0' && isdigit(*ptr)) {
        res = 10 * res + int(*ptr - '0');
        ++ptr;
    }

    return res;
}

void setup_symbol_num_compare()
{
    ObjectFactory<SymbolNumCompare> obj("symbol.num_compare");

    obj.setDescription("numeric symbol compare");
    obj.setCategory("symbol");
    obj.setKeywords({"symbol", "num_compare", "compare"});
}
