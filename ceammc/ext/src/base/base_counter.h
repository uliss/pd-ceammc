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
#ifndef BASE_COUNTER_H
#define BASE_COUNTER_H

#include "ceammc_object.h"
using namespace ceammc;

class BaseCounter : public BaseObject {
    IntProperty* from_;
    IntProperty* to_;
    IntProperty* repeat_;
    int ri_, i_;
    bool done_;

public:
    BaseCounter(const PdArgs& args);
    void onBang() override;
    void onInlet(size_t n, const AtomList& l) override;

    void m_reset(t_symbol*, const AtomListView& lv);

private:
    void next();
    void reset();
    bool shouldRepeat() const;
};

void setup_base_counter();

#endif // BASE_COUNTER_H
