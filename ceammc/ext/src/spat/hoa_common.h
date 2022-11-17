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

// MSYS temp define hack
#ifdef _WIN32
#ifndef _WINDOWS
#define _WINDOWS 1
#endif
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wextra-semi"
#pragma clang diagnostic ignored "-Wextra"
#include "Hoa.hpp"
// temporal fix: this file not exists in newest Hoa-Library-Light
#include "Hoa_Scope.hpp"
#include "Hoa_Tools.hpp"
#pragma clang diagnostic pop

#include "ceammc_sound_external.h"
using namespace ceammc;

constexpr size_t HOA_MIN_ORDER = 1;
constexpr size_t HOA_MAX_ORDER = 63;
constexpr size_t HOA_DEFAULT_ORDER = 1;
constexpr size_t HOA_DEFAULT_BLOCK_SIZE = 64;

typedef hoa::Optim<hoa::Hoa2d, t_sample> Optim2d;
typedef hoa::Decoder<hoa::Hoa2d, t_sample> Decoder2d;
typedef hoa::DecoderBinaural<hoa::Hoa2d, t_sample, hoa::hrir::Sadie_D2_2D> DecoderBinaural2d;
typedef hoa::DecoderIrregular<hoa::Hoa2d, t_sample> DecoderIrregular2d;
typedef hoa::DecoderRegular<hoa::Hoa2d, t_sample> DecoderRegular2d;
typedef hoa::Encoder<hoa::Hoa2d, t_sample> Encoder2d;
typedef hoa::MultiEncoder<hoa::Hoa2d, t_sample> MultiEncoder2d;
typedef hoa::PolarLines<hoa::Hoa2d, t_sample> PolarLines2d;
typedef hoa::Rotate<hoa::Hoa2d, t_sample> Rotate2d;
typedef hoa::Wider<hoa::Hoa2d, t_sample> Wider2d;
typedef hoa::Scope<hoa::Hoa2d, t_sample> Scope2d;

typedef hoa::Signal<t_sample> Signal;
typedef std::vector<t_sample> Buffer;

class HoaBase : public SoundExternal {
private:
    IntProperty* order_;

public:
    HoaBase(const PdArgs& args)
        : SoundExternal(args)
        , order_(nullptr)
    {
        order_ = new IntProperty("@order", positionalConstant<HOA_DEFAULT_ORDER, HOA_MIN_ORDER, HOA_MAX_ORDER>(0));
        order_->setInitOnly();
        order_->checkClosedRange(HOA_MIN_ORDER, HOA_MAX_ORDER);
        order_->setArgIndex(0);
        addProperty(order_);
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

protected:
    IntProperty* prop_order() { return order_; }
};

#endif // HOA_COMMON_H
