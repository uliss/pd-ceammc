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
#include "fx_infrev.h"
#include "ceammc_factory.h"

// keep in sync with Infinity.cpp
constexpr t_float DEF_DRYWET = 1;
constexpr t_float DEF_SIZE = 0.5;
constexpr t_float DEF_DUMP = 0;
constexpr t_float DEF_FILTER = 1;

constexpr auto PARAM_FILTER = 0;
constexpr auto PARAM_DUMP = 1;
constexpr auto PARAM_SIZE = 2;
constexpr auto PARAM_DRYWET = 3;

FxInfReverb::FxInfReverb(const PdArgs& args)
    : SoundExternal(args)
    , fx_(nullptr)
    , filter_(nullptr)
    , dump_(nullptr)
    , size_(nullptr)
    , drywet_(nullptr)
{
    createSignalInlet();
    createSignalOutlet();
    createSignalOutlet();

    filter_ = new FloatProperty("@filter", DEF_FILTER);
    filter_->checkClosedRange(0, 1);
    filter_->setSuccessFn([this](Property*) { fx_.setParameter(PARAM_FILTER, filter_->value()); });
    addProperty(filter_);

    dump_ = new FloatProperty("@dump", DEF_DUMP);
    dump_->checkClosedRange(0, 1);
    dump_->setSuccessFn([this](Property*) { fx_.setParameter(PARAM_DUMP, dump_->value()); });
    addProperty(dump_);

    size_ = new FloatProperty("@size", DEF_SIZE);
    size_->checkClosedRange(0, 1);
    size_->setSuccessFn([this](Property*) { fx_.setParameter(PARAM_SIZE, size_->value()); });
    addProperty(size_);

    drywet_ = new FloatProperty("@drywet", DEF_DRYWET);
    drywet_->checkClosedRange(0, 1);
    drywet_->setSuccessFn([this](Property*) { fx_.setParameter(PARAM_DRYWET, drywet_->value()); });
    addProperty(drywet_);
}

void FxInfReverb::processBlock(const t_sample** in, t_sample** out)
{
#if PD_FLOATSIZE == 32
    static_assert(std::is_same<t_float, float>::value, "");
#define DSP_METHOD processReplacing
#else
    static_assert(std::is_same<t_float, double>::value, "");
#define DSP_METHOD processDoubleReplacing
#endif

    fx_.DSP_METHOD(const_cast<t_float**>(in), out, blockSize());

#undef DSP_METHOD
}

void FxInfReverb::setupDSP(t_signal** in)
{
    SoundExternal::setupDSP(in);
    fx_.setSampleRate(samplerate());
}

void setup_fx_infrev_tilde()
{
    SoundExternalFactory<FxInfReverb> obj("fx.infrev~");
}
