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
#ifndef DYN_COMP_BASE_H
#define DYN_COMP_BASE_H

#include "args/argcheck2.h"
#include "ceammc_crc32.h"
#include "ceammc_faust.h"
#include "ceammc_object.h"

namespace ceammc {

CEAMMC_DEFINE_HASH(vocal)
CEAMMC_DEFINE_HASH(drums)
CEAMMC_DEFINE_HASH(bass)
CEAMMC_DEFINE_HASH(strings)

template <typename T>
class DynCompBase : public T {
    faust::UIProperty *ratio_, *threshold_, *attack_, *release_;

public:
    DynCompBase(const PdArgs& args)
        : T(args)
    {
        static_assert(std::is_base_of<faust::FaustExternalBase, T>::value, "");

        CEAMMC_DEFINE_PROP_VAR(ratio);
        CEAMMC_DEFINE_PROP_VAR(threshold);
        CEAMMC_DEFINE_PROP_VAR(attack);
        CEAMMC_DEFINE_PROP_VAR(release);

        ratio_ = this->findUIProperty(sym_prop_ratio);
        threshold_ = this->findUIProperty(sym_prop_threshold);
        attack_ = this->findUIProperty(sym_prop_attack);
        release_ = this->findUIProperty(sym_prop_release);

        this->bindPositionalArgsToProps({ sym_prop_ratio,
            sym_prop_threshold,
            sym_prop_attack,
            sym_prop_release });
    }

    void setCompress(t_float ratio, t_float threshold, t_float attack, t_float release)
    {
        if (!this->checkUIProperties({ ratio_, threshold_, attack_, release_ }))
            return;

        ratio_->setValue(ratio, true);
        threshold_->setValue(threshold, true);
        attack_->setValue(attack, true);
        release_->setValue(release, true);
    }

    void m_preset(t_symbol* m, const AtomListView& lv)
    {
        static const args::ArgChecker chk("PRESET:s=vocal|drums|bass|strings");
        if (!chk.check(lv, this))
            return chk.usage(this, m);

        switch (crc32_hash(lv.symbolAt(0, &s_))) {
        case hash_vocal:
            setCompress(2, -8, 2, 40);
            break;
        case hash_bass:
            setCompress(5, -10, 8, 40);
            break;
        case hash_drums:
            setCompress(10, -15, 1, 1);
            break;
        case hash_strings:
            setCompress(2, -10, 40, 100);
            break;
        default:
            OBJ_ERR << "unknown preset: " << lv;
            break;
        }
    }
};

}

#endif // DYN_COMP_BASE_H
