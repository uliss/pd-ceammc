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
#include "conv_bits2bang.h"
#include "ceammc_factory.h"

constexpr size_t MIN_VOICES = 1;
constexpr size_t MAX_VOICES = 64;
constexpr size_t DEFAULT_VOICES = 8;
constexpr size_t VOICE_INFO_MAX_LEN = 64;

static char voice_info[MAX_VOICES][VOICE_INFO_MAX_LEN] = { { 0 } };

ConvBits2Bang::ConvBits2Bang(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
    , reversed_(nullptr)
    , poly_(nullptr)
{
    n_ = new IntProperty("@n", DEFAULT_VOICES, PropValueAccess::INITONLY);
    n_->checkClosedRange(MIN_VOICES, MAX_VOICES);
    n_->setArgIndex(0);
    addProperty(n_);

    reversed_ = new BoolProperty("@rev", false);
    addProperty(reversed_);

    poly_ = new FlagProperty("@poly");
    addProperty(poly_);
}

void ConvBits2Bang::initDone()
{
    if (!poly_->value()) {
        for (int i = 0; i < n_->value(); i++)
            createOutlet();
    } else
        createOutlet();
}

void ConvBits2Bang::onList(const AtomList& lst)
{
    const bool reversed = reversed_->value();
    const bool poly = poly_->value();

    if (!poly && (int)lst.size() != n_->value()) {
        OBJ_ERR << "length of input list should be equal " << n_->value() << ", got: " << lst;
        return;
    }

    Atom apair[2] = { 0., &s_bang };
    const AtomListView lvpair(apair, 2);

    for (size_t i = 0; i < lst.size(); i++) {
        const size_t aidx = reversed ? (lst.size() - (i + 1)) : i;
        const auto& a = lst[aidx];
        if (a.isFloat() && a.asT<t_float>() != 0) {
            if (poly) {
                apair[0] = i;
                listTo(0, lvpair);
            } else
                bangTo(i);
        }
    }
}

const char* ConvBits2Bang::annotateOutlet(size_t n) const
{
    if (poly_->value())
        return "list: VOICE_IDX bang";
    else
        return voice_info[n];
}

void setup_conv_bits2bang()
{
    // init descriptions
    for (size_t i = 0; i < MAX_VOICES; i++) {
        snprintf(voice_info[i], VOICE_INFO_MAX_LEN, "voice: %d", (int)i);
    }

    ObjectFactory<ConvBits2Bang> obj("conv.bits2bang");
    obj.addAlias("bits->bang");
    obj.setXletsInfo({ "list: bit list (0|1)" }, {});
}
