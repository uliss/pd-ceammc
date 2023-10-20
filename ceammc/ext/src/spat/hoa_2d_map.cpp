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
#include <cmath>

#include "args/argcheck2.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "hoa_2d_map.h"

CEAMMC_DEFINE_STR(mute)
CEAMMC_DEFINE_STR(polar)
CEAMMC_DEFINE_STR(pol)
CEAMMC_DEFINE_STR(cartesian)
CEAMMC_DEFINE_STR(car)

Hoa2dMap::Hoa2dMap(const PdArgs& args)
    : HoaBase<hoa::Hoa2d>(args)
    , nins_(nullptr)
    , ramp_(nullptr)
{
    nins_ = new IntProperty("@nsrc", 1);
    nins_->setArgIndex(1);
    nins_->setInitOnly();
    nins_->checkMinEq(1);
    addProperty(nins_);

    ramp_ = new FloatProperty("@ramp", 100);
    ramp_->checkMinEq(0);
    ramp_->setSuccessFn(
        [this](Property*) {
            lines_->setRamp(ramp_->value() / 1000. * sys_getsr());
        });

    addProperty(ramp_);

    mode_ = new SymbolEnumProperty("@mode", { str_pol, str_car });
    mode_->setArgIndex(2);
    addProperty(mode_);
}

void Hoa2dMap::initDone()
{
    map_.reset(new MultiEncoder2d(order(), nins_->value()));
    lines_.reset(new PolarLines2d(map_->getNumberOfSources()));
    lines_->setRamp(ramp_->value() / 1000. * sys_getsr());

    for (size_t i = 0; i < map_->getNumberOfSources(); i++) {
        lines_->setRadiusDirect(i, 1);
        lines_->setAzimuthDirect(i, 0.);
    }

    const size_t NINS = (nins_->value() == 1) ? 3 : nins_->value();
    createSignalInlets(NINS);
    createSignalOutlets(map_->getNumberOfHarmonics());

    in_buf_.resize(numInputChannels() * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(numOutputChannels() * HOA_DEFAULT_BLOCK_SIZE);

    lines_vec_.resize(map_->getNumberOfSources() * 2);

    if (nins_->value() > 1) {
        for (short i = 0; i < nins_->value(); i++) {
            in_info_.emplace_back("in: %h", i + 1);
        }
    }
}

void Hoa2dMap::setupDSP(t_signal** sp)
{
    signalInit(sp);

    lines_->setRamp(ramp_->value() / 1000. * samplerate());

    if (map_->getNumberOfSources() == 1) {
        dsp_add(dspPerformIn1In2, 1, static_cast<void*>(this));
    } else {
        dsp_add(dspPerformMultiSource, 1, static_cast<void*>(this));
    }
}

void Hoa2dMap::blockSizeChanged(size_t bs)
{
    in_buf_.resize(numInputChannels() * bs);
    out_buf_.resize(numOutputChannels() * bs);
}

void Hoa2dMap::processBlock(const t_sample** in, t_sample** out)
{
}

void Hoa2dMap::processMultiSource()
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

void Hoa2dMap::processIn1In2()
{
    const size_t NOUTS = numOutputChannels();
    const size_t BS = blockSize();

    t_sample** in = inputBlocks();
    t_sample** out = outputBlocks();

    const bool polar_mode = (mode_->index() == 0);

    if (polar_mode) {
        for (size_t i = 0; i < BS; i++) {
            map_->setRadius(0, in[1][i]);
            map_->setAzimuth(0, in[2][i]);
            map_->process(&in[0][i], &out_buf_[NOUTS * i]);
        }
    } else {
        for (size_t i = 0; i < BS; i++) {
            auto x = in[1][i];
            auto y = in[2][i];
            map_->setRadius(0, hoa::Math<float>::radius(x, y));
            map_->setAzimuth(0, hoa::Math<float>::azimuth(x, y));
            map_->process(&in[0][i], &out_buf_[NOUTS * i]);
        }
    }

    for (size_t i = 0; i < NOUTS; i++)
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
}

bool Hoa2dMap::checkSourceIdx(int idx) const
{
    if (idx < 1 || idx > (int)map_->getNumberOfSources()) {
        OBJ_ERR << "invalid source index: " << idx;
        return false;
    } else
        return true;
}

void Hoa2dMap::setPolar(int idx, t_float r, t_float azimuth)
{
    if (!checkSourceIdx(idx))
        return;

    lines_->setRadius(idx - 1, r);
    lines_->setAzimuth(idx - 1, azimuth);
}

void Hoa2dMap::setCartesian(int idx, t_float x, t_float y)
{
    if (!checkSourceIdx(idx))
        return;

    lines_->setRadius(idx - 1, hoa::Math<float>::radius(x, y));
    lines_->setAzimuth(idx - 1, hoa::Math<float>::azimuth(x, y));
}

void Hoa2dMap::setMute(int idx, bool value)
{
    if (!checkSourceIdx(idx))
        return;

    map_->setMute(idx - 1, value);
}

void Hoa2dMap::onList(const AtomListView& lv)
{
    static const args::ArgChecker is_polar("IDX:i>=1 s=pol|polar RADIUS:f AZIMUTH:f ELEV:f?");
    static const args::ArgChecker is_cartes("IDX:i>=1 s=car|cartesian X:f Y:f Z:f?");
    static const args::ArgChecker is_mute("IDX:i>=1 s=mute VALUE:b");

    if (is_polar.checkSilent(lv))
        return setPolar(lv[0].asT<int>(), lv[2].asT<t_float>(), lv[3].asT<t_float>());
    else if (is_cartes.checkSilent(lv)) {
        return setCartesian(lv[0].asT<int>(), lv[2].asT<t_float>(), lv[3].asT<t_float>());
    } else if (is_mute.checkSilent(lv)) {
        return setMute(lv[0].asT<int>(), lv[2].asT<bool>());
    } else {
        OBJ_ERR << "invalid list: " << lv;
        is_polar.usage(this);
        is_cartes.usage(this);
        is_mute.usage(this);
    }
}

void Hoa2dMap::m_polar(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT, ARG_FLOAT)) {
        METHOD_ERR(s) << "IDX RADIUS ANGLE expected: " << lv;
        return;
    }

    setPolar(lv[0].asT<int>(), lv[1].asT<t_float>(), lv[2].asT<t_float>());
}

void Hoa2dMap::m_cartesian(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT, ARG_FLOAT)) {
        METHOD_ERR(s) << "IDX X Y expected: " << lv;
        return;
    }

    setCartesian(lv[0].asT<int>(), lv[1].asT<t_float>(), lv[2].asT<t_float>());
}

void Hoa2dMap::m_mute(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, ARG_BOOL)) {
        METHOD_ERR(s) << "SRC_IDX STATE expected: " << lv;
        return;
    }

    setMute(lv[0].asInt(), lv[1].asBool(true));
}

const char* Hoa2dMap::annotateInlet(size_t n) const
{

    if (nins_->value() == 1) {
        switch (n) {
        case 0:
            return "input signal";
        case 1:
            return "radius";
        case 2:
            return "rotation";
        default:
            return "";
        }
    } else {
        if (n < in_info_.size())
            return in_info_[n].txt();
        else
            return "";
    }
}

void setup_spat_hoa_2d_map()
{
    SoundExternalFactory<Hoa2dMap> obj("hoa.2d.map~");
    obj.addAlias("hoa.map~");

    obj.addMethod(str_polar, &Hoa2dMap::m_polar);
    obj.addMethod(str_pol, &Hoa2dMap::m_polar);
    obj.addMethod(str_mute, &Hoa2dMap::m_mute);
    obj.addMethod(str_cartesian, &Hoa2dMap::m_cartesian);
    obj.addMethod(str_car, &Hoa2dMap::m_cartesian);
}
