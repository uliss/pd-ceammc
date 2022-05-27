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
#include "ceammc_property_callback.h"
#include "stk_synth_factory.h"

#include "Shakers.h"

#include <algorithm>

using SymType = std::pair<t_symbol*, ShakerType>;
using SymTypeList = std::vector<SymType>;

// clazy:excludeall=non-pod-global-static
static SymTypeList type_list;

#define MACRO_SHAKER_NAMES                                                         \
    {                                                                              \
        "maraca", "cabasa", "sekere", "tambourine", "sleigh_bells",                \
            "bamboo_chimes", "sand_paper", "coke_can", "sticks", "crunch",         \
            "big_rocks", "little_rocks", "next_mug", "penny_mug", "nickle_mug",    \
            "dime_mug", "quarter_mug", "franc_mug", "peso_mug", "guiro", "wrench", \
            "water_drops", "tuned_bamboo_chimes"                                   \
    }

constexpr const char* SHAKER_NAMES[] = MACRO_SHAKER_NAMES;
constexpr ShakerType SHAKER_TYPES[] {
    MARACA,
    CABASA,
    SEKERE,
    TAMBOURINE,
    SLEIGH_BELLS,
    BAMBOO_CHIMES,
    SAND_PAPER,
    COKE_CAN,
    STICKS,
    CRUNCH,
    BIG_ROCKS,
    LITTLE_ROCKS,
    NEXT_MUG,
    PENNY_MUG,
    NICKLE_MUG,
    DIME_MUG,
    QUARTER_MUG,
    FRANC_MUG,
    PESO_MUG,
    GUIRO,
    WRENCH,
    WATER_DROPS,
    TUNED_BAMBOO_CHIMES
};

static_assert(sizeof(SHAKER_NAMES) / sizeof(SHAKER_NAMES[0]) == sizeof(SHAKER_TYPES) / sizeof(SHAKER_TYPES[0]), "");

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

class TypeProperty : public CallbackProperty {
    ShakerType type_;

public:
    TypeProperty(ShakerType type)
        : CallbackProperty(
            "@type",
            [this]() -> Atom { return propType(); },
            [this](const Atom& a) -> bool { return propSetType(a); })
        , type_(type)
    {
        info().setConstraints(PropValueConstraints::ENUM);
        info().clearEnum();
        for (auto s : MACRO_SHAKER_NAMES) {
            if(!info().addEnum(gensym(s))) {
                LIB_ERR << "[@type] can't add enum value: " << s;
                continue;
            }
        }
    }

    Atom propType() const
    {
        auto it = std::find_if(std::begin(type_list), std::end(type_list),
            [this](SymTypeList::value_type& e) { return e.second == type_; });

        if (it == std::end(type_list))
            return {};
        else
            return it->first;
    }

    bool propSetType(const Atom& a)
    {
        if (a.isInteger()) {
            // try instrument index
            int idx = a.asInt();

            if (idx < 0 || idx >= SHAKER_TYPE_MAX) {
                LIB_ERR << "invalid instrument index: " << idx;
                return false;
            } else {
                type_ = static_cast<ShakerType>(idx);
                return true;
            }
        } else if (a.isSymbol()) {
            // try instrument name
            t_symbol* t = a.asSymbol();

            auto type = SynthShakers::nameToType(t);
            if (type == UNKNOWN) {
                LIB_ERR << "invalid instrument name: " << a;
                LIB_ERR << "expected values are:";
                for (auto& i : type_list) {
                    LIB_ERR << "    " << i.first->s_name;
                }

                return false;
            }

            type_ = type;
            return true;
        } else {
            LIB_ERR << "unsuported value: " << a;
            return false;
        }
    }

    int type() const { return type_; }
};

SynthShakers::SynthShakers(const PdArgs& args)
    : StkBase(args, new MyShakers())
    , type_(nullptr)
    , gate_(0)
{
    createCbFloatProperty(
        "@gate", [this]() -> t_float { return gate_; },
        [this](t_float v) -> bool {
            gate_ = v;
            if (gate_ > 0)
                synth_->noteOn(type_->type(), gate_);
            else
                synth_->noteOff(0);

            return true;
        })
        ->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 0, 1);

    type_ = new TypeProperty(typeFromArgs(parsedPosArgs().symbolAt(0, gensym("maraca"))));
    addProperty(type_);

    createCbProperty("@types", &SynthShakers::propTypes);
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
        [s](SymTypeList::value_type& el) { return el.first == s; });

    if (it == std::end(type_list))
        return UNKNOWN;

    return it->second;
}

bool SynthShakers::nameExists(t_symbol* s)
{
    auto it = std::find_if(std::begin(type_list), std::end(type_list),
        [s](SymTypeList::value_type& el) { return el.first == s; });

    return it != std::end(type_list);
}

void setup_synth_shakers()
{
    type_list.reserve(SHAKER_TYPE_MAX);
    for (int i = 0; i < SHAKER_TYPE_MAX; i++)
        type_list.push_back(std::make_pair(gensym(SHAKER_NAMES[i]), SHAKER_TYPES[i]));

    StkSynthFactory<SynthShakers> obj("synth.shakers~");
}
