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
#ifndef CEAMMC_PROPERTY_DURATION_H
#define CEAMMC_PROPERTY_DURATION_H

#include "ceammc_property.h"

namespace ceammc {

class DurationProperty : public SymbolProperty {
    std::uint16_t num_ { 0 };
    std::uint16_t div_ { 4 };
    std::uint8_t dots_ { 0 };
    mutable bool dirty_ { false };

public:
    DurationProperty(const std::string& name,
        std::uint16_t num,
        std::uint16_t div,
        std::uint8_t dots = 0,
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

    std::uint16_t beatNum() const { return num_; }
    bool setBeatNum(int beatNum);

    std::uint16_t beatDivision() const { return div_; }
    bool setBeatDivision(int beatDiv);

    t_float durationMs(t_float bpm, int num, int div) const;
    t_float divDurationMs(t_float bpm, int num, int div) const;

    std::uint16_t dots() const { return dots_; }

private:
    void sync() const;
};
}

#endif // CEAMMC_PROPERTY_DURATION_H
