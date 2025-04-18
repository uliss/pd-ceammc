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

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

namespace ceammc {

SymbolEnumProperty::SymbolEnumProperty(const char* name, t_symbol* def, PropValueAccess access)
    : EnumProperty<t_symbol*>(name, def, access)
{
}

SymbolEnumProperty::SymbolEnumProperty(const char* name, std::initializer_list<t_symbol*> values, PropValueAccess access)
    : EnumProperty(name, values, access)
{
}

SymbolEnumProperty::SymbolEnumProperty(const char* name, std::initializer_list<const char*> values, PropValueAccess access)
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
        return setIndex(lv[0].asT<t_int>());
    } else {
        PROP_ERR() << "symbol or enum index expected, got: " << lv;
        return false;
    }
}

bool SymbolEnumProperty::setSymbol(t_symbol* s)
{
    return setValue(s);
}

SymbolFloatEnumProperty::SymbolFloatEnumProperty(const char* name,
    std::initializer_list<std::pair<t_symbol*, t_float>> values,
    PropValueAccess access)
    : SymbolEnumProperty(name, values.begin()->first, access)
{
    data_.push_back(values.begin()->second);

    for (size_t i = 1; i < values.size(); i++) {
        auto& p = *(values.begin() + i);
        appendEnum(p.first);
        data_.push_back(p.second);
    }
}

SymbolFloatEnumProperty::SymbolFloatEnumProperty(const char* name,
    std::initializer_list<std::pair<const char*, t_float>> values,
    PropValueAccess access)
    : SymbolEnumProperty(name, gensym(values.begin()->first), access)
{
    data_.push_back(values.begin()->second);

    for (size_t i = 1; i < values.size(); i++) {
        auto& p = *(values.begin() + i);
        appendEnum(gensym(p.first));
        data_.push_back(p.second);
    }
}

t_float SymbolFloatEnumProperty::valuePair() const
{
    auto idx = index();
    return idx < data_.size() ? data_[idx] : 0;
}

}
