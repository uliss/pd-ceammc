/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef SIMPLE_CONVERTER_H
#define SIMPLE_CONVERTER_H

#include <functional>

#include "ceammc_containers.h"
#include "ceammc_object.h"
#include "ceammc_sound_external.h"
using namespace ceammc;

class SimpleConverter : public BaseObject {
    std::function<t_float(t_float)> fn_;

public:
    template <typename Fn>
    SimpleConverter(const PdArgs& args, Fn fn)
        : BaseObject(args)
        , fn_(fn)
    {
        createOutlet();
    }

    void onFloat(t_float v) override
    {
        floatTo(0, fn_(v));
    }

    void onList(const AtomListView& lv) override
    {
        SmallAtomList res;
        res.reserve(lv.size());
        lv.mapFloat(fn_, res);
        listTo(0, res.view());
    }
};

class SimpleConverterTilde : public SoundExternal {
    std::function<t_float(t_float)> fn_;

public:
    template <typename Fn>
    SimpleConverterTilde(const PdArgs& args, Fn fn)
        : SoundExternal(args)
        , fn_(fn)
    {
        createSignalOutlet();
    }

    void processBlock(const t_sample** in, t_sample** out) override
    {
        const size_t BS = blockSize();

        for (size_t i = 0; i < BS; i += 8) {
            out[0][i + 0] = fn_(in[0][i + 0]);
            out[0][i + 1] = fn_(in[0][i + 1]);
            out[0][i + 2] = fn_(in[0][i + 2]);
            out[0][i + 3] = fn_(in[0][i + 3]);
            out[0][i + 4] = fn_(in[0][i + 4]);
            out[0][i + 5] = fn_(in[0][i + 5]);
            out[0][i + 6] = fn_(in[0][i + 6]);
            out[0][i + 7] = fn_(in[0][i + 7]);
        }
    }
};

#endif // SIMPLE_CONVERTER_H
