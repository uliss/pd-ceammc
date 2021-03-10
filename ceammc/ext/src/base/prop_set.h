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
#ifndef PROP_SET_H
#define PROP_SET_H

#include "ceammc_object.h"
using namespace ceammc;

class PropSet : public BaseObject {
    std::vector<t_symbol*> props_;

public:
    PropSet(const PdArgs& args);

    void parseProperties() override;
    void onInlet(size_t n, const AtomListView& lst) override;
    const char* annotateInlet(size_t n) const override;

private:
    void processCanvasProps(t_glist* dest, t_symbol* s, const AtomList& lst);
    void processObjectProps(t_object* dest, t_symbol* s, const AtomList& lst);
};

void setup_prop_set();

#endif // PROP_SET_H
