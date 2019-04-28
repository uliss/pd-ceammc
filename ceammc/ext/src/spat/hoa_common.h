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
#ifndef HOA_COMMON_H
#define HOA_COMMON_H

#include <vector>

#include "Hoa.hpp"
#include "m_pd.h"

#include "ceammc_sound_external.h"
using namespace ceammc;

static const int HOA_MIN_ORDER = 1;
static const int HOA_MAX_ORDER = 63;
static const int HOA_DEFAULT_ORDER = 4;
static const int HOA_MAXBLKSIZE = 8912;
static const int HOA_DEFAULT_BLOCK_SIZE = 64;

typedef hoa::Decoder<hoa::Hoa2d, t_sample> Decoder2d;
typedef hoa::DecoderBinaural<hoa::Hoa2d, t_sample, hoa::hrir::Sadie_D2_2D> DecoderBinaural2d;
typedef hoa::DecoderIrregular<hoa::Hoa2d, t_sample> DecoderIrregular2d;
typedef hoa::DecoderRegular<hoa::Hoa2d, t_sample> DecoderRegular2d;
typedef hoa::Encoder<hoa::Hoa2d, t_sample> Encoder2d;
typedef hoa::Rotate<hoa::Hoa2d, t_sample> Rotate2d;
typedef hoa::Signal<t_sample> Signal;
typedef std::vector<t_sample> Buffer;

class HoaBase : public SoundExternal {
private:
    IntPropertyClosedRange* order_;

public:
    HoaBase(const PdArgs& args)
        : SoundExternal(args)
        , order_(nullptr)
    {
    }

    void parseProperties() override
    {
        SoundExternal::parseProperties();

        order_ = new IntPropertyClosedRange("@order",
            positionalFloatArgument(0, HOA_DEFAULT_ORDER), HOA_MIN_ORDER, HOA_MAX_ORDER);
        createProperty(order_);
    }

    int order() const { return order_->value(); }

    void createSignalInlets(size_t n)
    {
        for (size_t i = numInputChannels(); i < n; i++)
            createSignalInlet();
    }

    void createSignalOutlets(size_t n)
    {
        for (size_t i = 0; i < n; i++)
            createSignalOutlet();
    }

    void setupDSP(t_signal** sp) override
    {
        const size_t old_bs = blockSize();
        SoundExternal::setupDSP(sp);

        const size_t new_sz = blockSize();
        if (new_sz != old_bs)
            blocksizeChanged(new_sz);
    }

    virtual void blocksizeChanged(size_t bs)
    {
    }
};

#endif // HOA_COMMON_H
