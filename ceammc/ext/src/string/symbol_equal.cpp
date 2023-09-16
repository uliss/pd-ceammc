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
#include "symbol_equal.h"
#include "ceammc_factory.h"

SymbolEqual::SymbolEqual(const PdArgs& a)
    : BaseObject(a)
    , pattern_(nullptr)
{
    createInlet(&pattern_);
    createOutlet();

    pattern_ = parsedPosArgs().symbolAt(0, nullptr);
}

void SymbolEqual::onSymbol(t_symbol* s)
{
    floatTo(0, s == pattern_);
}

void SymbolEqual::onList(const AtomListView& lv)
{
    if (lv.size() < 1)
        return;

    t_symbol* s = lv[0].asSymbol();
    if (lv.size() > 1) {
        if (!lv[1].getSymbol(&pattern_))
            pattern_ = 0;
    }

    floatTo(0, s == pattern_);
}

t_symbol* SymbolEqual::pattern() const
{
    return pattern_;
}

void setup_symbol_equal()
{
    ObjectFactory<SymbolEqual> obj("symbol.equal");

    obj.setDescription("check symbols for equality");
    obj.setCategory("symbol");
    obj.setKeywords({"symbol", "compare"});
}
