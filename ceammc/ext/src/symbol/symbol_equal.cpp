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
    , pattern_(0)
{
    createInlet(&pattern_);
    createOutlet();

    parseArguments();
    if (args().size() > 0)
        args()[0].getSymbol(&pattern_);
}

void SymbolEqual::onSymbol(t_symbol* s)
{
    floatTo(0, s == pattern_);
}

extern "C" void setup_symbol0x2eequal()
{
    ObjectFactory<SymbolEqual> obj("symbol.equal");
}
