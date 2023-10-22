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
#ifndef DYN_GATE_BASE_H
#define DYN_GATE_BASE_H

#include "ceammc_crc32.h"
#include "ceammc_faust.h"

namespace ceammc {

template <typename T>
class DynGateT : public T {
    faust::UIProperty* threshold_ { nullptr };

public:
    DynGateT(const PdArgs& args)
        : T(args)
    {
        CEAMMC_DEFINE_PROP_VAR(threshold)
        CEAMMC_DEFINE_PROP_VAR(attack)
        CEAMMC_DEFINE_PROP_VAR(hold)
        CEAMMC_DEFINE_PROP_VAR(release)

        threshold_ = this->findUIProperty(sym_prop_threshold);

        this->bindPositionalArgsToProps({
            sym_prop_threshold,
            sym_prop_attack,
            sym_prop_hold,
            sym_prop_release,
        });

        this->createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv) final
    {
        if (threshold_)
            threshold_->setList(lv);
    }
};

}

#endif // DYN_GATE_BASE_H
