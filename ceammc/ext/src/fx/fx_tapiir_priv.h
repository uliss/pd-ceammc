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
#ifndef FX_TAPIIR_PRIV_H
#define FX_TAPIIR_PRIV_H

#ifndef FAUST_MACRO
#define FAUST_MACRO
#endif
#include "fx_tapiir.h"

#include <array>

constexpr size_t NUM_TAPS = 6;
constexpr size_t NUM_CHAN = 2;

using namespace ceammc;

class FxTapiir : public faust_fx_tapiir_tilde {
    std::array<UIProperty*, NUM_TAPS> tap_delays_;
    std::array<UIProperty*, NUM_TAPS> tap_gains_;
    std::array<UIProperty*, NUM_TAPS * NUM_TAPS> tap_fb_;
    std::array<UIProperty*, NUM_TAPS * NUM_CHAN> tap_inputs_;
    std::array<UIProperty*, NUM_TAPS * NUM_CHAN> tap_outputs_;

public:
    FxTapiir(const PdArgs& args);

    void m_random(t_symbol* s, const AtomListView& lv);

private:
    void initTapGroupProps();

    void getTapFb(size_t tapn, AtomList& res);
    bool setTapFb(size_t tapn, const AtomListView& lv);

    template <size_t N>
    void createTapFbProp()
    {
        char name[] = "@fbsX";
        name[4] = N + '0';

        createCbListProperty(
            name, [this]() -> AtomList {
                AtomList res;
                getTapFb(N, res);
                return res; },
            [this](const AtomList& l) -> bool {
                return setTapFb(N, l);
            });
    }
};

void setup_fx_tapiir_tilde();

#endif // FX_TAPIIR_PRIV_H
