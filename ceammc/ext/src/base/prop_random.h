/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef PROP_RANDOM_H
#define PROP_RANDOM_H

#include <random>

#include "ceammc_object.h"
using namespace ceammc;

class PropRandom : public BaseObject {
    std::default_random_engine rnd_;
    std::vector<t_symbol*> props_;
    IntProperty* seed_;

public:
    PropRandom(const PdArgs& args);

    void onBang() override;
    void onInlet(size_t n, const AtomListView&) override;
    const char* annotateInlet(size_t n) const override;

private:
    void randomCanvasProps(t_glist* dest, t_symbol* s);
    void randomObjectProps(t_object* dest, t_symbol* s);
};

void setup_prop_random();

#endif // PROP_RANDOM_H
