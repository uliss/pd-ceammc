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
#ifndef STRING_ENDS_WITH_H
#define STRING_ENDS_WITH_H

#include "ceammc_data.h"
#include "ceammc_object.h"

using namespace ceammc;

class StringEndsWith : public BaseObject {
    std::string suffix_;

public:
    StringEndsWith(const PdArgs& a);
    void onSymbol(t_symbol* s) override;
    void onInlet(size_t, const AtomListView& l) override;
    void onDataT(const StringAtom& str);
};

void setup_string_ends_with();

#endif // STRING_ENDS_WITH_H
