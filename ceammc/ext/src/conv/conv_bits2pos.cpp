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
#include "conv_bits2pos.h"
#include "ceammc_factory.h"

ConvBits2Pos::ConvBits2Pos(const PdArgs& args)
    : BaseObject(args)
    , reversed_(nullptr)
{
    reversed_ = new BoolProperty("@rev", false);
    addProperty(reversed_);

    createOutlet();
}

void ConvBits2Pos::onFloat(t_float f)
{
    if (f != 0)
        floatTo(0, 0);
}

void ConvBits2Pos::onList(const AtomList& lst)
{
    Atom res[lst.size()];
    size_t n = 0;

    if (reversed_->value()) {
        for (size_t i = 0; i < lst.size(); i++) {
            const auto& a = lst[i];
            if (a.isFloat() && a.asT<t_float>() != 0)
                res[n++] = i;
        }
    } else {
        const auto N = lst.size();
        for (size_t i = 0; i < lst.size(); i++) {
            const auto& a = lst[N - (i + 1)];
            if (a.isFloat() && a.asT<t_float>() != 0)
                res[n++] = i;
        }
    }

    listTo(0, AtomListView(res, n));
}

void setup_conv_bits2pos()
{
    ObjectFactory<ConvBits2Pos> obj("conv.bits2pos");
    obj.addAlias("bits->pos");
    obj.setXletsInfo({ "list: list of bits (0|1)" }, { "list: list of bit positions" });
}
