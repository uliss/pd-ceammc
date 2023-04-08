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
#ifndef CEAMMC_PROPERTY_BPM_H
#define CEAMMC_PROPERTY_BPM_H

#include "ceammc_property.h"

namespace ceammc {

class BpmProperty : public SymbolProperty {
    t_float bpm_ { 0 };
    t_float beat_num_ { 0 };
    t_float beat_div_ { 0 };

public:
    enum BeatDivision {
        BEAT_1 = 1,
        BEAT_2 = 2,
        BEAT_4 = 4,
        BEAT_8 = 8,
        BEAT_16 = 16,
        BEAT_32 = 32,
        BEAT_64 = 64,
        BEAT_128 = 128
    };

public:
    BpmProperty(const std::string& name,
        t_float init,
        std::uint16_t num = 1,
        BeatDivision div = BEAT_4,
        PropValueAccess access = PropValueAccess::READWRITE);

    bool setList(const AtomListView& lv) override;
    bool setAtom(const Atom& a) override;
    bool setFloat(t_float f) override;
    bool setSymbol(t_symbol* s) override;

    // overwrite parent
    bool setValue(t_symbol* s);
    bool setValue(const Atom& a);

    t_float ratio() const;

    t_float bpm() const { return bpm_; }
    bool setBpm(t_float bpm);

    std::uint16_t beatNum() const { return beat_num_; }
    bool setBeatNum(std::uint16_t beatNum);

    t_float beatDivision() const { return beat_div_; }
    bool setBeatDivision(t_float beatDiv);

    bool isQuater() const { return beat_div_ == 0.25; }

    t_float beatDurationMs() const;
    t_float wholeNoteDurationMs() const;
    t_float barDurationMs() const;
};

}

#endif // CEAMMC_PROPERTY_BPM_H
