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
#ifndef STRING_FORMAT_H
#define STRING_FORMAT_H

#include "ceammc_object.h"
#include "datatype_string.h"

using namespace ceammc;

class StringFormat : public BaseObject {
    typedef DataT<DataTypeString> StringPtr;

    AtomList fmt_atoms_;
    std::string fmt_str_;
    StringPtr fmt_result_;
    FlagProperty* int_mode_;

public:
    StringFormat(const PdArgs& a);
    void onData(const AbstractData* d);
    void onBang();
    void onFloat(float v);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& lst);

    void dump() const;

    AtomList propGetFormat() const;
    void propSetFormat(const AtomList& lst);
};

extern "C" void setup_string0x2eformat();

#endif // STRING_FORMAT_H
