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
#ifndef LIST_GEN_H
#define LIST_GEN_H

#include "ceammc_object.h"

using namespace ceammc;

class ListGenerate : public BaseObject {
    AtomList gen_values_;
    IntProperty* count_;
    bool in_process_;

public:
    ListGenerate(const PdArgs& a);

    void onBang() override;
    void onFloat(t_float v) override;
    void onInlet(size_t n, const AtomListView& lv) override;

    bool loopbackDetected();
};

void setup_list_gen();

#endif // LIST_GEN_H
