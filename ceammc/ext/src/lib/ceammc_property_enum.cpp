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
#include "ceammc_property_enum.h"
#include "fmt/format.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

namespace ceammc {

SymbolEnumProperty::SymbolEnumProperty(const std::string& name, t_symbol* def, PropValueAccess access)
    : EnumProperty<t_symbol*>(name, def, access)
{
}

SymbolEnumProperty::SymbolEnumProperty(const std::string& name, std::initializer_list<t_symbol*> values, PropValueAccess access)
    : EnumProperty(name, values, access)
{
}

SymbolEnumProperty::SymbolEnumProperty(const std::string& name, std::initializer_list<const char*> values, PropValueAccess access)
    : EnumProperty(name, (values.size() == 0) ? gensym("???") : gensym(*values.begin()), access)
{
    for (size_t i = 1; i < values.size(); i++)
        appendEnum(gensym(*(values.begin() + i)));
}

bool SymbolEnumProperty::setList(const AtomListView& lv)
{
    if (!emptyCheck(lv))
        return false;

    if (lv.isSymbol())
        return setValue(lv[0].asT<t_symbol*>());
    else if (lv.isInteger()) {
        auto idx = lv[0].asT<int>();
        if (idx < 0 || idx >= (int)numEnums()) {
            PROP_ERR() << fmt::format("invalid enum index, expecting value in [{}..{}] range, got: {}",
                0, numEnums() - 1, idx);
            return false;
        } else
            return setValue(info().enumValues()[idx].asT<t_symbol*>());
    } else {
        PROP_ERR() << "symbol or enum index expected, got: " << lv;
        return false;
    }
}

}
