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
#ifndef PROP_GET_H
#define PROP_GET_H

#include "ceammc_object.h"
using namespace ceammc;

class PropGet : public BaseObject {
    std::vector<t_symbol*> props_;

public:
    PropGet(const PdArgs& args);

    void onBang() override;
    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override;
    void onAny(t_symbol* s, const AtomListView&) override;

    const char* annotateOutlet(size_t n) const override;

private:
    void processObjectProps(t_object* dest, const std::vector<t_symbol*>& props);
    void processAbstractionProps(t_glist* dest, const std::vector<t_symbol*>& props);

    t_object* findDestination();
    void outputProperties(t_object* dest, const std::vector<t_symbol*>& props);
};

void setup_prop_get();

#endif // PROP_GET_H
