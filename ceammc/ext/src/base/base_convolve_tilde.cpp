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
#include "base_convolve_tilde.h"
#include "ceammc_array.h"
#include "ceammc_dsp.h"
#include "ceammc_factory.h"
#include "ceammc_sound.h"

#include "FFTConvolver.h"
#include "ceammc_soxr_resampler.h"
#include "fmt/core.h"

constexpr size_t MIN_IR_SIZE = 0;
constexpr size_t DEF_IR_SIZE = 48000;
constexpr size_t MAX_IR_SIZE = 48000 * 5;
constexpr size_t CONV_BLOCKSIZE = 1024;

static_assert(MIN_IR_SIZE <= DEF_IR_SIZE, "");
static_assert(DEF_IR_SIZE <= MAX_IR_SIZE, "");

static_assert(std::is_same<float, fftconvolver::Sample>::value, "");

BaseConvolveTilde::BaseConvolveTilde(const PdArgs& args)
    : SoundExternal(args)
    , conv_(new fftconvolver::FFTConvolver())
{
    createSignalOutlet();

    max_size_ = new IntProperty("@maxsize", DEF_IR_SIZE);
    max_size_->checkClosedRange(MIN_IR_SIZE, MAX_IR_SIZE);
    addProperty(max_size_);

    norm_ = new BoolProperty("@norm", true);
    addProperty(norm_);
}

void BaseConvolveTilde::setupDSP(t_signal** sig)
{
    SoundExternal::setupDSP(sig);

    if (load_state_ == LOAD_OK) {
        conv_->reset();
        bool rc = conv_->init(CONV_BLOCKSIZE, ir_data_.data(), ir_data_.size());
        if (!rc) {
            OBJ_ERR << "can't init FFTConvolver";
            load_state_ = NOT_LOADED;
        } else
            OBJ_DBG << fmt::format("IR data size: {}", ir_data_.size());
    } else {
        OBJ_ERR << "IR data not loaded";
    }
}

void BaseConvolveTilde::processBlock(const t_sample** in, t_sample** out)
{
    if (load_state_ != LOAD_OK)
        return;

#if PD_FLOATSIZE == 32
    conv_->process(&in[0][0], &out[0][0], blockSize());
#endif
}

void BaseConvolveTilde::m_load_file(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, s))
        return;

    auto fname = lv[0].asSymbol()->s_name;
    auto path = findInStdPaths(fname);
    if (path.empty()) {
        OBJ_ERR << fmt::format("IR file not found: '{}'", fname);
        return;
    }

    dsp::SuspendGuard dsp;

    if (loadIRFromFile(path.c_str())) {
        load_state_ = LOAD_OK;
        if (norm_->value())
            normalizeIR();
    } else
        load_state_ = NOT_LOADED;
}

void BaseConvolveTilde::m_load_array(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, s))
        return;

    dsp::SuspendGuard dsp;

    if (loadIRFromArray(lv[0].asSymbol()->s_name)) {
        load_state_ = LOAD_OK;
        if (norm_->value())
            normalizeIR();
    } else
        load_state_ = NOT_LOADED;
}

void BaseConvolveTilde::m_set(t_symbol* s, const AtomListView& lv)
{
    auto N = std::min<size_t>(lv.size(), MAX_IR_SIZE);

    dsp::SuspendGuard dsp;

    ir_data_.clear();
    ir_data_.resize(N);

    for (size_t i = 0; i < N; i++)
        ir_data_[i] = lv[i].asFloat();

    load_state_ = LOAD_OK;

    if (norm_->value())
        normalizeIR();
}

void BaseConvolveTilde::dump() const
{
    SoundExternal::dump();
}

bool BaseConvolveTilde::loadIRFromArray(const char* name)
{
    Array src(name);
    if (!src.isValid()) {
        OBJ_ERR << fmt::format("array not found: '{}'", name);
        return false;
    }

    auto N = std::min<size_t>(max_size_->value(), src.size());
    if (N < MIN_IR_SIZE) {
        OBJ_ERR << fmt::format("array '{}' size expected to be >={}, got {}", name, MIN_IR_SIZE, N);
        return false;
    }

    ir_data_.reserve(N);
    ir_data_.clear();

    for (auto it = src.begin(); it != src.begin() + N; ++it)
        ir_data_.push_back(*it);

    OBJ_DBG << fmt::format("{} IR samples loaded from array '{}'", N, name);
    return true;
}

bool BaseConvolveTilde::loadIRFromFile(const char* path)
{
    auto sf = sound::SoundFileFactory::openRead(path);
    if (!sf) {
        OBJ_ERR << fmt::format("can't read IR file: '{}'", path);
        return false;
    }

    auto rr = double(samplerate()) / sf->sampleRate();
    if (sf->sampleRate() != samplerate()) {
        OBJ_DBG << fmt::format("resampling from {} to {} ({})", sf->sampleRate(), samplerate(), rr);
    }

    const auto NCH = sf->channels();
    if (NCH != 1) {
        OBJ_DBG << fmt::format("warning: number of channels={}, using first channel for IR data", NCH);
    }

    auto N = std::min<size_t>(max_size_->value(), sf->frameCount() * rr);
    if (N < MIN_IR_SIZE) {
        OBJ_ERR << fmt::format("number of samples in file '{}' expected to be >={}, got {}", path, MIN_IR_SIZE, N);
        return false;
    }

    SoxrResamplerOptions sox_opts {
        false,
        SoxrResamplerFormat::FLOAT_I,
        SoxrResamplerFormat::FLOAT_S,
    };
    SoxrResampler resampler(sf->sampleRate(), samplerate(), NCH, SoxrResampler::QUICK, sox_opts);

    std::int64_t ntotal = 0;
    if (!resampler.setOutputCallback([this, N, &ntotal](const float* const* data, size_t rframes, bool done) -> bool {
            for (size_t i = 0; i < rframes && ntotal < N; i++, ntotal++)
                ir_data_.push_back(data[0][i]);

            return ntotal < N;
        })) {
        OBJ_ERR << "can't set resampler callback";
        return false;
    }

    constexpr size_t IR_BUF_SIZE = 1024;
    const size_t IR_BUF_FRAMES = IR_BUF_SIZE / NCH;
    float ir_buf[IR_BUF_SIZE];

    ir_data_.clear();
    ir_data_.reserve(N);
    std::int64_t nfread = 0;
    std::int64_t fpos = 0;
    while ((nfread = sf->readFrames(ir_buf, IR_BUF_FRAMES, fpos)) > 0) {
        fpos += nfread;
        auto rc = resampler.process(ir_buf, nfread);

        if (rc != SoxrResampler::ResultCode::Ok) {
            resampler.processDone();
            break;
        }
    }

    OBJ_DBG << fmt::format("{} IR samples total: {}, loaded from file '{}'", size_t(sf->frameCount() * rr), ir_data_.size(), path);
    return true;
}

void BaseConvolveTilde::normalizeIR()
{
    auto it = std::max_element(ir_data_.begin(), ir_data_.end(), [](float a, float b) { return std::abs(a) < std::abs(b); });
    if (it == ir_data_.end())
        return;

    OBJ_DBG << fmt::format("max value: {}", *it);

    if (*it != 0) {
        auto k = 1.f / *it;
        for (auto& x : ir_data_)
            x *= k;
    }
}

// for forwarding declaration of FFTConvolver
BaseConvolveTilde::~BaseConvolveTilde() = default;

void setup_base_convolve_tilde()
{
    SoundExternalFactory<BaseConvolveTilde> obj("convolve~");
    obj.addMethod("load_array", &BaseConvolveTilde::m_load_array);
    obj.addMethod("load_file", &BaseConvolveTilde::m_load_file);
    obj.addMethod("set", &BaseConvolveTilde::m_set);
}
