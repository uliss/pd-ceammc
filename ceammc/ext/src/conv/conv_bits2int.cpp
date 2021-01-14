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
#include "conv_bits2int.h"
#include "ceammc_factory.h"

ConvBits2Int::ConvBits2Int(const PdArgs& args)
    : BaseObject(args)
    , reversed_(nullptr)
{

    reversed_ = new BoolProperty("@rev", false);
    addProperty(reversed_);

    createOutlet();
}

void ConvBits2Int::onFloat(t_float f)
{
    if (f == 0)
        floatTo(0, 0);
    else if (f == 1)
        floatTo(0, 1);
    else
        OBJ_ERR << "0 or 1 value expected, got: " << f;
}

void ConvBits2Int::onList(const AtomList& lst)
{
    uint64_t res = 0;

    if (reversed_->value()) {
        for (size_t i = 0; i < lst.size(); i++) {
            const auto v = lst[i].asInt(-1);
            if (v == 1)
                res |= (0x1 << i);
            else if (v != 0)
                OBJ_ERR << "0 or 1 expected, got: " << lst[i] << ", ignoring";
        }
    } else {
        const size_t N = lst.size();
        for (size_t i = 0; i < lst.size(); i++) {
            const auto& a = lst[N - (i + 1)];
            const auto v = a.asInt(-1);
            if (v == 1)
                res |= (0x1 << i);
            else if (v != 0)
                OBJ_ERR << "0 or 1 expected, got: " << a << ", ignoring";
        }
    }

    floatTo(0, res);
}

void setup_conv_bits2int()
{
    ObjectFactory<ConvBits2Int> obj("conv.bits2int");
    obj.addAlias("bits->int");
}
