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
#include "midi_vrand.h"
#include "ceammc_args.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <ctime>
#include <memory>

CEAMMC_DEFINE_SYM_HASH(uniform);
CEAMMC_DEFINE_SYM_HASH(gauss);
CEAMMC_DEFINE_SYM_HASH(linup);
CEAMMC_DEFINE_SYM_HASH(lindown);
CEAMMC_DEFINE_SYM_HASH(cmin);
CEAMMC_DEFINE_SYM_HASH(cmax);
CEAMMC_DEFINE_SYM_HASH(cmean);
CEAMMC_DEFINE_SYM_HASH(add);
CEAMMC_DEFINE_SYM_HASH(sub);
CEAMMC_DEFINE_SYM_HASH(assign);

static std::unique_ptr<ArgChecker> onlist_chk;

MidiVRrand::MidiVRrand(const PdArgs& args)
    : BaseObject(args)
    , min_(nullptr)
    , max_(nullptr)
    , dist_(nullptr)
    , mode_(nullptr)
    , seed_(nullptr)
{
    min_ = new FloatProperty("@min", 0);
    min_->setArgIndex(0);
    addProperty(min_);

    max_ = new FloatProperty("@max", 127);
    max_->setArgIndex(1);
    addProperty(max_);

    dist_ = new SymbolEnumProperty("@dist", {
                                                str_uniform,
                                                str_gauss,
                                                str_linup,
                                                str_lindown,
                                                str_cmin,
                                                str_cmax,
                                                str_cmean,
                                            });
    addProperty(dist_);

    mode_ = new SymbolEnumProperty("@mode", { str_assign, str_add, str_sub });
    addProperty(mode_);
    addProperty(new SymbolEnumAlias("@assign", mode_, sym_assign()));
    addProperty(new SymbolEnumAlias("@add", mode_, sym_add()));
    addProperty(new SymbolEnumAlias("@sub", mode_, sym_sub()));

    seed_ = new SizeTProperty("@seed", 0);
    seed_->setSuccessFn([this](Property*) {
        const auto sd = seed_->value();
        gen_.seed(sd ? sd : time(nullptr));
    });
    addProperty(seed_);

    createOutlet();
}

void MidiVRrand::onFloat(t_float note)
{
    if (note < 0 || note > 127) {
        OBJ_ERR << "int value in [0..127] range is expected, got: " << note;
        return;
    }

    StaticAtomList<2> msg { note, velocity(64) };
    listTo(0, msg.view());
}

void MidiVRrand::onList(const AtomListView& lv)
{
    if (!onlist_chk->check(lv)) {
        OBJ_ERR << "NOTE VEL [DUR] expected, got: " << lv;
        return;
    }

    const size_t N = lv.size();
    assert(N == 2 || N == 3);

    StaticAtomList<3> msg { lv[0], velocity(lv[1].asFloat()) };
    if (N > 2)
        msg.push_back(lv[2]);

    listTo(0, msg.view());
}

t_float MidiVRrand::generate()
{
    const auto d = dist_->value();
    switch (crc32_hash(d)) {
    case hash_uniform:
        return std::uniform_int_distribution<int>(min_->value(), max_->value())(gen_);
    case hash_gauss: {
        const t_float mu = (min_->value() + max_->value()) / 2;
        const t_float sigma = (max_->value() - min_->value()) / 3;
        return std::normal_distribution<t_float>(mu, sigma)(gen_);
    }
    case hash_linup: {
        const std::array<t_float, 2> i = { min_->value(), max_->value() };
        const std::array<t_float, 2> w = { 0, 1 };

        return std::piecewise_linear_distribution<t_float>(
            i.begin(),
            i.end(),
            w.begin())(gen_);
    }
    case hash_lindown: {
        const std::array<t_float, 2> i = { min_->value(), max_->value() };
        const std::array<t_float, 2> w = { 1, 0 };

        return std::piecewise_linear_distribution<t_float>(
            i.begin(),
            i.end(),
            w.begin())(gen_);
    }
    case hash_cmin:
        return min_->value();
    case hash_cmax:
        return max_->value();
    case hash_cmean:
        return ((max_->value() + min_->value()) / 2);
    default:
        OBJ_ERR << "unknown distribution: " << d;
        return 0;
    }
}

t_float MidiVRrand::velocity(t_float orig)
{
    if (orig == 0)
        return 0;

    switch (crc32_hash(mode_->value())) {
    case hash_assign:
        return clip<int, 0, 127>(generate());
    case hash_add:
        return clip<int, 0, 127>(orig + generate());
    case hash_sub:
        return clip<int, 0, 127>(orig - generate());
    default:
        return clip<int, 0, 127>(generate());
    }
}

void setup_midi_vrand()
{
    onlist_chk.reset(new ArgChecker("f0..127 f0..127 f?"));

    ObjectFactory<MidiVRrand> obj("midi.vrand");
}
