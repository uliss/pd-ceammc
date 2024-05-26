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
#include "args/argcheck.h"
#include "ceammc_factory.h"

SymbolEqual::SymbolEqual(const PdArgs& a)
    : BaseObject(a)
{
    pattern_ = new SymbolProperty("@cmp", nullptr);
    pattern_->setArgIndex(0);
    addProperty(pattern_);

    createInlet();
    createOutlet();
}

void SymbolEqual::onSymbol(t_symbol* s)
{
    floatTo(0, s == pattern_->value());
}

void SymbolEqual::onList(const AtomListView& lv)
{
    if (lv.isSymbol())
        return onSymbol(lv.asSymbol());

    static const args::ArgChecker chk("s s");
    if (!chk.check(lv, this))
        return chk.usage(this, &s_list);

    auto sym = lv[0].asSymbol();
    pattern_->setSymbol(lv[1].asSymbol());
    onSymbol(sym);
}

void SymbolEqual::onInlet(size_t n, const AtomListView& lv)
{
    pattern_->set(lv);
}

t_symbol* SymbolEqual::pattern() const
{
    return pattern_->value();
}

void setup_symbol_equal()
{
    ObjectFactory<SymbolEqual> obj("symbol.equal");

    obj.setDescription("check symbols for equality");
    obj.setCategory("symbol");
    obj.setKeywords({ "symbol", "compare" });
}
