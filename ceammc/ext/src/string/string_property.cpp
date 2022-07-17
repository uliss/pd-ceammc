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
#include "string_property.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "ceammc_string.h"
#include "lex/parser_strings.h"

namespace ceammc {

StringProperty::StringProperty(const std::string& name, const std::string& def)
    : Property(PropertyInfo(name, PropValueType::SYMBOL))
    , str_(def)
{
}

bool StringProperty::setValue(const std::string& str)
{
    str_->set(str);
    return true;
}

bool StringProperty::setValue(const DataTypeString& str)
{
    str_ = StringAtom(str);
    return true;
}

AtomList StringProperty::get() const
{
    return str_;
}

bool StringProperty::setSymbol(t_symbol* s)
{
    str_->set(s);
    return true;
}

bool StringProperty::setList(const AtomListView& lv)
{
    if (!emptyCheck(lv))
        return false;

    if (lv.isFloat()) {
        str_->set(to_string(lv));
        return true;
    } else if (lv.isData()) {
        auto dptr = lv.asD<DataTypeString>();
        if (dptr) {
            str_ = StringAtom(*dptr);
            return true;
        } else {
            LogPdObject(nullptr, LOG_ERROR).stream() << errorPrefix() << "not a string data: " << lv;
            return false;
        }
    } else if (lv.size() > 0) {
        string::SmallString buf;
        const std::string str_lst = to_string(lv);
        if (string::unquote_and_unescape(str_lst.c_str(), buf)) {
            str_->set({ buf.data(), buf.size() });
            return true;
        } else if (lv.size() == 1) {
            str_->set(str_lst);
            return true;
        } else {
            LogPdObject(nullptr, LOG_ERROR).stream() << errorPrefix() << "invalid string: " << str_lst;
            return false;
        }
    } else {
        LogPdObject(nullptr, LOG_ERROR).stream() << errorPrefix() << "unknown data: " << lv;
        return false;
    }
}

}
