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

#include "ceammc_music_theory_tempo.h"
#include "ceammc_property.h"

namespace ceammc {

class BpmProperty : public SymbolProperty {
    music::Tempo tempo_;
    mutable bool dirty_ { false };

public:
    BpmProperty(const std::string& name,
        const music::Tempo& tempo,
        PropValueAccess access = PropValueAccess::READWRITE);

    bool setList(const AtomListView& lv) override;
    bool setAtom(const Atom& a) override;
    bool setFloat(t_float f) override;
    bool setSymbol(t_symbol* s) override;
    bool getSymbol(t_symbol*& s) const override;
    AtomList get() const override;

    // overwrite parent
    bool setValue(t_symbol* s);
    bool setValue(const Atom& a);

    const music::Tempo& tempo() const { return tempo_; }

    t_float bpm() const { return tempo_.bpm(); }
    bool setBpm(t_float bpm);

    int beatDivision() const { return tempo_.division(); }
    bool setBeatDivision(int beatDiv);

    music::Duration beatDuration() const { return tempo_.beatDuration(); }
    bool isNull() const { return tempo_.isNull(); }

    t_float timeMs() const { return tempo_.beatDurationMs(); }
    t_float wholeNoteDurationMs() const { return tempo_.wholeNoteDurationMs(); }

private:
    void sync() const;
};

}

#endif // CEAMMC_PROPERTY_BPM_H
