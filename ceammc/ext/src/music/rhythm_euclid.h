/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef RHYTHM_EUCLID_H
#define RHYTHM_EUCLID_H

#include "ceammc_object.h"
using namespace ceammc;

class RhythmEuclid : public BaseObject {
    IntProperty* n_;
    IntProperty* beats_;
    IntProperty* offset_;
    AtomList pattern_;
    bool changed_;

public:
    RhythmEuclid(const PdArgs& args);

    void initDone() override;
    void onBang() override;
    void onFloat(t_float f) override;
    void onList(const AtomListView& lv) override;

    void onInlet(size_t n, const AtomListView& lv) override;

private:
    bool updatePattern();
};

void setup_rhythm_euclid();

#endif // RHYTHM_EUCLID_H
