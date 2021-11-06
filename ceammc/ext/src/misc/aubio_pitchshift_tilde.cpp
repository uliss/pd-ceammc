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
#include "aubio_pitchshift_tilde.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

#include "aubio/src/aubio.h"
#include "aubio/src/effects/pitchshift.h"

#define CONSTEXPR_STR_HASH(str)             \
    constexpr const char* str_##str = #str; \
    constexpr auto hash_##str = #str##_hash;

CONSTEXPR_STR_HASH(compound)
CONSTEXPR_STR_HASH(soft)
CONSTEXPR_STR_HASH(percussive)
CONSTEXPR_STR_HASH(crisp)
CONSTEXPR_STR_HASH(mixed)
CONSTEXPR_STR_HASH(smooth)

constexpr size_t DEFAULT_BLOCK_SIZE = 64;

AubioPitchshiftTilde::AubioPitchshiftTilde(const PdArgs& args)
    : SoundExternal(args)
    , impl_(nullptr, del_aubio_pitchshift)
    , in_(DEFAULT_BLOCK_SIZE)
    , out_(DEFAULT_BLOCK_SIZE)
    , scale_(1)
    , transpose_(0)
{
    createInlet();
    createSignalOutlet();

    impl_.reset(new_aubio_pitchshift("default", 0, DEFAULT_BLOCK_SIZE, 44100));
    if (!impl_)
        OBJ_ERR << "can't create ";

    createCbFloatProperty(
        "@scale",
        [this]() { return impl_ ? aubio_pitchshift_get_pitchscale(impl_.get())
                                : 1; },
        [this](t_float scale) -> bool {
            if (impl_ && (aubio_pitchshift_set_pitchscale(impl_.get(), scale) == 0)) {
                scale_ = scale;
                return true;
            } else
                return false;
        })
        ->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 0.25, 4);

    {
        auto p = createCbFloatProperty(
            "@transpose",
            [this]() { return impl_ ? aubio_pitchshift_get_transpose(impl_.get())
                                    : 0; },
            [this](t_float transpose) -> bool {
                if (impl_ && (aubio_pitchshift_set_transpose(impl_.get(), transpose) == 0)) {
                    transpose_ = transpose;
                    return true;
                } else
                    return false;
            });
        p->setArgIndex(0);
        p->setFloatCheck(PropValueConstraints::CLOSED_RANGE, -24, 24);
        p->setUnits(PropValueUnits::SEMITONE);
    };

    createCbFloatProperty("@latency",
        [this]() -> t_float {
            if (impl_)
                return aubio_pitchshift_get_latency(impl_.get());
            else
                return -1;
        })
        ->setUnits(PropValueUnits::SAMP);

    {
        window_ = new SymbolEnumProperty("@window", { "standard", "short", "long" });
        window_->setSuccessFn([this](Property*) {
            auto v = window_->value();
            if (v == gensym("standard"))
                options_[WINDOW] = "WindowStandard";
            else if (v == gensym("short"))
                options_[WINDOW] = "WindowShort";
            else if (v == gensym("long"))
                options_[WINDOW] = "WindowLong";

            update();
        });
        addProperty(window_);

        quality_ = new SymbolEnumProperty("@quality", { "speed", "quality", "consistency" });
        quality_->setSuccessFn([this](Property*) {
            auto v = quality_->value();
            if (v == gensym("speed"))
                options_[QUALITY] = "PitchHighSpeed";
            else if (v == gensym("quality"))
                options_[QUALITY] = "PitchHighQuality";
            else if (v == gensym("consistency"))
                options_[QUALITY] = "PitchHighConsistency";

            update();
        });
        addProperty(quality_);

        smooth_ = new BoolProperty("@smooth", false);
        smooth_->setSuccessFn([this](Property*) {
            options_[SMOOTH] = (smooth_->value()) ? "SmoothingOn" : "SmoothingOff";
            update();
        });
        addProperty(smooth_);

        phase_ = new BoolProperty("@phase", true);
        phase_->setSuccessFn([this](Property*) {
            options_[PHASE] = (phase_->value()) ? "PhaseLaminar" : "PhaseIndependent";
            update();
        });
        addProperty(phase_);

        formant_ = new BoolProperty("@formant", false);
        formant_->setSuccessFn([this](Property*) {
            options_[FORMANT] = (formant_->value()) ? "FormantShifted" : "FormantPreserved";
            update();
        });
        addProperty(formant_);

        detector_ = new SymbolEnumProperty("@detector", { str_compound, str_percussive, str_soft });
        detector_->setSuccessFn([this](Property*) {
            switch (crc32_hash(detector_->value()->s_name)) {
            case hash_compound:
                options_[DETECTOR] = "DetectorCompound";
                break;
            case hash_percussive:
                options_[DETECTOR] = "DetectorPercussive";
                break;
            case hash_soft:
                options_[DETECTOR] = "DetectorSoft";
            default:
                break;
            }

            update();
        });
        addProperty(detector_);

        transient_ = new SymbolEnumProperty("@trans", { str_crisp, str_mixed, str_smooth });
        transient_->setSuccessFn([this](Property*) {
            switch (crc32_hash(transient_->value()->s_name)) {
            case hash_crisp:
                options_[TRANSIENT] = "TransientsCrisp";
                break;
            case hash_mixed:
                options_[TRANSIENT] = "TransientsMixed";
                break;
            case hash_smooth:
                options_[TRANSIENT] = "TransientsSmooth";
            default:
                break;
            }

            update();
        });
        addProperty(transient_);
    }
}

AubioPitchshiftTilde::~AubioPitchshiftTilde()
{
    // for std::unique_ptr
}

void AubioPitchshiftTilde::setupDSP(t_signal** sig)
{
    SoundExternal::setupDSP(sig);

    update();

    const auto bs = blockSize();

    in_.resize(bs);
    out_.resize(bs);
}

void AubioPitchshiftTilde::processBlock(const t_sample** in, t_sample** out)
{
    if (!impl_)
        return;

    const auto bs = blockSize();
    fvec_t fin, fout;

    fin.length = in_.size();
    fin.data = in_.data();
    fout.length = out_.size();
    fout.data = out_.data();

    for (size_t i = 0; i < bs; i++)
        in_[i] = in[0][i];

    aubio_pitchshift_do(impl_.get(), &fin, &fout);

    for (size_t i = 0; i < bs; i++)
        out[0][i] = out_[i];
}

void AubioPitchshiftTilde::onInlet(size_t n, const AtomListView& lv)
{
    property(gensym("@transpose"))->set(lv);
}

void AubioPitchshiftTilde::update()
{
    opt_string_.clear();

    for (auto& kv : options_) {
        if (!opt_string_.empty())
            opt_string_ += ',';

        opt_string_.append(kv.second);
    }

    if (opt_string_.empty())
        opt_string_ = "default";

    const auto bs = blockSize() > 0 ? blockSize()
                                    : DEFAULT_BLOCK_SIZE;
    const auto sr = samplerate() > 0 ? samplerate() : 44100;

    OBJ_LOG << opt_string_;

    impl_.reset(new_aubio_pitchshift(opt_string_.c_str(), transpose_, bs, sr));

    if (!impl_) {
        OBJ_ERR << "can't create pitchshift";
        return;
    }
}

void setup_aubio_pitchshift_tilde()
{
    SoundExternalFactory<AubioPitchshiftTilde> obj("fx.rb_pitchshift~");
    obj.setXletsInfo({ "signal: input", "float: transpose in semitones" }, { "signal: output" });
}
