/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef BASE_LOG_H
#define BASE_LOG_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
using namespace ceammc;

class BaseLog : public BaseObject {
    SymbolEnumProperty* mode_;
    SymbolEnumAlias* error_;
    SymbolEnumAlias* post_;
    SymbolEnumAlias* debug_;
    SymbolEnumAlias* verbose_;
    SymbolEnumAlias* stdout_;
    SymbolEnumAlias* stderr_;
    SymbolEnumAlias* null_;
    ListProperty* prefix_;

public:
    BaseLog(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;
};

void setup_base_log();

#endif // BASE_LOG_H
