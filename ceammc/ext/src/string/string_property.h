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
#ifndef STRING_PROPERTY_H
#define STRING_PROPERTY_H

#include "ceammc_data.h"
#include "ceammc_property.h"
#include "datatype_string.h"

namespace ceammc {

class StringProperty : public Property {
    StringAtom str_;

public:
    StringProperty(const std::string& name, const std::string& def = "");

    const DataTypeString& value() const { return *str_; }
    bool setValue(const std::string& str);
    bool setValue(const DataTypeString& str);

    AtomList get() const override;

    bool setSymbol(t_symbol*) override;
    bool setList(const AtomListView& lv) override;

    const std::string& str() const { return str_->str(); }
};

}

#endif // STRING_PROPERTY_H
