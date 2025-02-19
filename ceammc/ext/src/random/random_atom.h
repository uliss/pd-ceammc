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
#ifndef RANDOM_ATOM_H
#define RANDOM_ATOM_H

#include "ceammc_object.h"
#include "ceammc_random.h"

#include <cstdint>

using namespace ceammc;

class RandomAtom : public BaseObject {
    using UniformDist = std::uniform_int_distribution<uint32_t>;
    using DiscreteDist = std::discrete_distribution<uint32_t>;

private:
    random::RandomGen gen_;
    ListProperty* atoms_;
    BoolProperty* nonrep_;
    std::vector<t_float> weights_;
    DiscreteDist discrete_;
    UniformDist uniform_;
    uint32_t last_idx_;
    bool is_uniform_;

public:
    RandomAtom(const PdArgs& args);
    void onBang() override;

    void onInlet(size_t n, const AtomListView& lv) override;

private:
    uint32_t genIndex();
    void updateUniformDistrib();
};

void setup_random_atom();

#endif // RANDOM_ATOM_H
