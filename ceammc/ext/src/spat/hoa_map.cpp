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

#define _USE_MATH_DEFINES
#include <cmath>

#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "hoa_map.h"

static t_symbol* SYM_POL;
static t_symbol* SYM_CAR;

HoaMap::HoaMap(const PdArgs& args)
    : HoaBase(args)
    , nins_(nullptr)
    , ramp_(nullptr)
    , mode_(nullptr)
{
    nins_ = new IntPropertyMinEq("@nsrc", positionalFloatArgument(1, 3), 1);
    createProperty(nins_);

    ramp_ = new RampProperty(
        this, [](HoaMap* map, const AtomList& l) {
            map->lines_->setRamp(l.floatAt(0, 0) / 1000. * sys_getsr());
        },
        "@ramp", 100, 0);

    createProperty(ramp_);

    mode_ = new SymbolEnumProperty("@mode", SYM_CAR);
    mode_->appendEnum(SYM_POL);
    createProperty(mode_);
}

void HoaMap::parseProperties()
{
    HoaBase::parseProperties();

    const size_t NINS = nins_->value();
    nins_->setReadonly(true);

    map_.reset(new MultiEncoder2d(order(), NINS));
    lines_.reset(new PolarLines2d(map_->getNumberOfSources()));
    lines_->setRamp(ramp_->value() / 1000. * sys_getsr());

    for (size_t i = 0; i < map_->getNumberOfSources(); i++) {
        lines_->setRadiusDirect(i, 1);
        lines_->setAzimuthDirect(i, 0.);
    }

    createSignalInlets(NINS);
    createSignalOutlets(map_->getNumberOfHarmonics());

    in_buf_.resize(numInputChannels() * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(numOutputChannels() * HOA_DEFAULT_BLOCK_SIZE);

    lines_vec_.resize(map_->getNumberOfSources() * 2);
}

void HoaMap::setupDSP(t_signal** sp)
{
    signalInit(sp);

    lines_->setRamp(ramp_->value() / 1000. * samplerate());

    //    if (map_->getNumberOfSources() == 1) {
    //        if (count[1] && count[2])

    //            object_method(dsp, gensym("dsp_add64"), x, (method)hoa_map_tilde_perform_in1_in2, 0, NULL);
    //        else if (count[1] && !count[2])
    //            object_method(dsp, gensym("dsp_add64"), x, (method)hoa_map_tilde_perform_in1, 0, NULL);
    //        else if (!count[1] && count[2])
    //            object_method(dsp, gensym("dsp_add64"), x, (method)hoa_map_tilde_perform_in2, 0, NULL);
    //        else if (!count[1] && !count[2])
    //            object_method(dsp, gensym("dsp_add"), x, (method)hoa_map_tilde_perform, 0, NULL);
    //    } else {
    dsp_add(dspPerformMultiSource, 1, static_cast<void*>(this));
    //    }
}

void HoaMap::blockSizeChanged(size_t bs)
{
    in_buf_.resize(numInputChannels() * bs);
    out_buf_.resize(numOutputChannels() * bs);
}

void HoaMap::processBlock(const t_sample** in, t_sample** out)
{
}

void HoaMap::processMultiSource()
{
    const size_t NSRC = map_->getNumberOfSources();
    const size_t NINS = numInputChannels();
    const size_t NOUTS = numOutputChannels();
    const size_t BS = blockSize();

    t_sample** in = inputBlocks();
    t_sample** out = outputBlocks();

    for (size_t i = 0; i < NINS; i++)
        Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);

    for (size_t i = 0; i < BS; i++) {
        lines_->process(lines_vec_.data());

        for (size_t j = 0; j < NSRC; j++)
            map_->setRadius(j, lines_vec_[j]);

        for (size_t j = 0; j < NSRC; j++)
            map_->setAzimuth(j, lines_vec_[j + NSRC]);

        map_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);
    }

    for (size_t i = 0; i < NOUTS; i++)
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
}

void HoaMap::m_polar(t_symbol* s, const AtomList& l)
{
    if (!checkArgs(l, ARG_INT, ARG_FLOAT, ARG_FLOAT)) {
        METHOD_ERR(s) << "IDX RADIUS ANGLE expected: " << l;
        return;
    }

    int idx = l[0].asInt();
    if (idx < 0 || idx >= (int)map_->getNumberOfSources()) {
        METHOD_ERR(s) << "invalid source index: " << idx;
        return;
    }

    lines_->setRadius(idx, l[1].asFloat());
    lines_->setAzimuth(idx, l[2].asFloat() - M_PI_2);
}

void setup_spat_hoa_map()
{
    SYM_POL = gensym("sym");
    SYM_CAR = gensym("car");

    SoundExternalFactory<HoaMap> obj("hoa.2d.map~");
    obj.addAlias("hoa.map~");

    obj.addMethod("polar", &HoaMap::m_polar);
}
