/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "synth_shakers.h"
#include "stk_synth_factory.h"

#include "Shakers.h"

typedef std::pair<t_symbol*, ShakerType> SymType;
typedef std::vector<SymType> SymTypeList;

// clazy:excludeall=non-pod-global-static
static SymTypeList type_list;

class MyShakers : public stk::Shakers {
public:
    MyShakers(int t = 0)
        : stk::Shakers(t)
    {
    }

    void noteOn(stk::StkFloat instrument, stk::StkFloat amplitude) override
    {
        int noteNumber = instrument;
        if (shakerType_ != noteNumber)
            setType(noteNumber);

        shakeEnergy_ += amplitude * 1.0 * 0.1;
        if (shakeEnergy_ > 1.0)
            shakeEnergy_ = 1.0;
        if (shakerType_ == 19 || shakerType_ == 20)
            ratchetCount_ += 1;
    }
};

static ShakerType typeFromArgs(t_symbol* s)
{
    auto t = SynthShakers::nameToType(s);
    if (t == UNKNOWN) {
        LIB_ERR << "unknown instrument type: " << s;
        return MARACA;
    }

    return t;
}

SynthShakers::SynthShakers(const PdArgs& args)
    : StkBase(args, new MyShakers())
    , type_(typeFromArgs(positionalSymbolArgument(0, gensym("maraca"))))
    , gate_(0)
{
    {
        Property* p = createCbProperty("@gate", &SynthShakers::propGate, &SynthShakers::propSetGate);
        p->info().setType(PropertyInfoType::FLOAT);
        p->info().setRange(0, 1);
    }

    {
        Property* p = createCbProperty("@type", &SynthShakers::propType, &SynthShakers::propSetType);
        p->info().setType(PropertyInfoType::SYMBOL);

        for (auto t : type_list)
            p->info().addEnum(t.first);

        p->info().setDefault(gensym("maraca"));
    }

    createCbProperty("@types", &SynthShakers::propTypes);
}

AtomList SynthShakers::propGate() const
{
    return Atom(gate_);
}

void SynthShakers::propSetGate(const AtomList& lst)
{
    gate_ = lst.floatAt(0, 0);

    if (gate_ > 0)
        synth_->noteOn(type_, gate_);
    else
        synth_->noteOff(0);
}

AtomList SynthShakers::propType() const
{
    auto it = std::find_if(std::begin(type_list), std::end(type_list),
        [&](SymTypeList::value_type& it) { return it.second == type_; });

    if (it == std::end(type_list))
        return Atom();
    else
        return Atom(it->first);
}

void SynthShakers::propSetType(const AtomList& lst)
{
    // try instrument name
    t_symbol* t = lst.symbolAt(0, 0);

    if (t == nullptr) {
        // try instrument index
        int idx = lst.floatAt(0, -1);

        if (idx < 0 || idx >= SHAKER_TYPE_MAX) {
            OBJ_ERR << "invalid instrument index: " << idx;
            return;
        } else {
            type_ = (ShakerType)idx;
            return;
        }
    }

    auto type = nameToType(t);
    if (type == UNKNOWN) {
        OBJ_ERR << "invalid instrument name: " << lst;
        OBJ_ERR << "expected values are:";
        for (auto& i : type_list) {
            OBJ_ERR << "    " << i.first->s_name;
        }

        return;
    }

    type_ = type;
}

AtomList SynthShakers::propTypes() const
{
    AtomList res;
    res.reserve(type_list.size());

    for (auto& el : type_list)
        res.append(el.first);

    return res;
}

ShakerType SynthShakers::nameToType(t_symbol* s)
{
    auto it = std::find_if(std::begin(type_list), std::end(type_list),
        [&](SymTypeList::value_type& el) { return el.first == s; });

    if (it == std::end(type_list))
        return UNKNOWN;

    return it->second;
}

bool SynthShakers::nameExists(t_symbol* s)
{
    auto it = std::find_if(std::begin(type_list), std::end(type_list),
        [&](SymTypeList::value_type& el) { return el.first == s; });

    return it != std::end(type_list);
}

void setup_synth_shakers()
{
    type_list = {
        { gensym("maraca"), MARACA },
        { gensym("cabasa"), CABASA },
        { gensym("sekere"), SEKERE },
        { gensym("tambourine"), TAMBOURINE },
        { gensym("sleigh_bells"), SLEIGH_BELLS },
        { gensym("bamboo_chimes"), BAMBOO_CHIMES },
        { gensym("sand_paper"), SAND_PAPER },
        { gensym("coke_can"), COKE_CAN },
        { gensym("sticks"), STICKS },
        { gensym("crunch"), CRUNCH },
        { gensym("big_rocks"), BIG_ROCKS },
        { gensym("little_rocks"), LITTLE_ROCKS },
        { gensym("next_mug"), NEXT_MUG },
        { gensym("penny_mug"), PENNY_MUG },
        { gensym("nickle_mug"), NICKLE_MUG },
        { gensym("dime_mug"), DIME_MUG },
        { gensym("quarter_mug"), QUARTER_MUG },
        { gensym("franc_mug"), FRANC_MUG },
        { gensym("peso_mug"), PESO_MUG },
        { gensym("guiro"), GUIRO },
        { gensym("wrench"), WRENCH },
        { gensym("water_drops"), WATER_DROPS },
        { gensym("tuned_bamboo_chimes"), TUNED_BAMBOO_CHIMES }
    };

    StkSynthFactory<SynthShakers> obj("synth.shakers~");
}
