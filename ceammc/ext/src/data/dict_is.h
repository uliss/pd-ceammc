/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef DICT_IS_H
#define DICT_IS_H

#include "ceammc_data.h"
#include "ceammc_object.h"

using namespace ceammc;

class IsDict : public BaseObject {
public:
    IsDict(const PdArgs& args);
    void onFloat(t_float) override;
    void onSymbol(t_symbol*) override;
    void onList(const AtomList&) override;
    void onAny(t_symbol*, const AtomListView&) override;
    void onData(const Atom& ptr) override;
};

void setup_is_dict();

#endif // DICT_IS_H
