/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "an_aubio_onset_tilde.h"
#include "ceammc_factory.h"
#include "ceammc_property_extra.h"

#define CEAMMC_AUBIO_ONSET_PLIST threshold_, delay_, speedlim_, silence_threshold_, compression_
#define CEAMMC_AUBIO_ONSET_RESTORE threshold_, silence_threshold_, speedlim_

template <typename P, typename... Args>
void updatePtr(P ptr, Args... args)
{
    using expand_type = int[];
    // compiler warning unused suppress via cast to (void)
    (void)expand_type { (args->setAubioPtr(ptr), 0)... };
}

template <typename... Args>
void saveValues(Args... args)
{
    using expand_type = int[];
    // compiler warning unused suppress via cast to (void)
    (void)expand_type { (args->save(), 0)... };
}

template <typename... Args>
void restoreValues(Args... args)
{
    using expand_type = int[];
    // compiler warning unused suppress via cast to (void)
    (void)expand_type { (args->restore(), 0)... };
}

AubioOnset::AubioOnset(const PdArgs& args)
    : SoundExternal(args)
    , buffer_size_(nullptr)
    , hop_size_(nullptr)
    , threshold_(nullptr)
    , delay_(nullptr)
    , method_(nullptr)
    , speedlim_(nullptr)
    , silence_threshold_(nullptr)
    , compression_(nullptr)
    , awhitening_(nullptr)
    , active_(true)
    , dsp_pos_(0)
    , last_ms_(0)
    , tick_(this, &AubioOnset::clock_tick)
{
    buffer_size_ = new IntPropertyMinEq("@bs", positionalFloatArgument(0, 1024), 64);
    createProperty(buffer_size_);

    hop_size_ = new IntProperty("@hs", positionalFloatArgument(2, -1), -1);
    createProperty(hop_size_);

    threshold_ = new OnsetFloatProperty(
        "@threshold",
        nullptr,
        [](aubio_onset_t* o) { return aubio_onset_get_threshold(o); },
        [](aubio_onset_t* o, smpl_t t) { return aubio_onset_set_threshold(o, t); });
    createProperty(threshold_);

    delay_ = new OnsetFloatProperty(
        "@delay", nullptr,
        [](aubio_onset_t* o) { return aubio_onset_get_delay_ms(o); },
        [](aubio_onset_t* o, smpl_t ms) { return aubio_onset_set_delay_ms(o, ms); });
    delay_->info().setUnits(PropertyInfoUnits::MSEC);
    createProperty(delay_);

    speedlim_ = new OnsetFloatProperty(
        "@speedlim", nullptr,
        [](aubio_onset_t* o) { return aubio_onset_get_minioi_ms(o); },
        [](aubio_onset_t* o, smpl_t ms) { return aubio_onset_set_minioi_ms(o, ms); });
    speedlim_->info().setUnits(PropertyInfoUnits::MSEC);
    createProperty(speedlim_);

    silence_threshold_ = new OnsetFloatProperty(
        "@silence", nullptr,
        [](aubio_onset_t* o) { return aubio_onset_get_silence(o); },
        [](aubio_onset_t* o, smpl_t t) { return aubio_onset_set_silence(o, t); });
    silence_threshold_->info().setUnits(PropertyInfoUnits::DB);
    createProperty(silence_threshold_);

    compression_ = new OnsetFloatProperty(
        "@compression", nullptr,
        [](aubio_onset_t* o) { return aubio_onset_get_compression(o); },
        [](aubio_onset_t* o, smpl_t v) { return aubio_onset_set_compression(o, v); });
    compression_->info().setMin(0);
    createProperty(compression_);

    awhitening_ = new OnsetUIntProperty(
        "@awhitening", nullptr,
        [](aubio_onset_t* o) { return aubio_onset_get_awhitening(o); },
        [](aubio_onset_t* o, uint_t v) { return aubio_onset_set_awhitening(o, v); });
    awhitening_->info().setType(PropertyInfoType::BOOLEAN);
    createProperty(awhitening_);

    method_ = new SymbolEnumProperty("@method", gensym("default"));
#define S(a) gensym(a)
    method_->appendEnum(S("energy"));
    method_->appendEnum(S("hfc"));
    method_->appendEnum(S("specdiff"));
    method_->appendEnum(S("complexdomain"));
    method_->appendEnum(S("complex"));
    method_->appendEnum(S("phase"));
    method_->appendEnum(S("wphase"));
    method_->appendEnum(S("mkl"));
    method_->appendEnum(S("kl"));
    method_->appendEnum(S("specflux"));
    method_->appendEnum(S("centroid"));
    method_->appendEnum(S("spread"));
    method_->appendEnum(S("skewness"));
    method_->appendEnum(S("kurtosis"));
    method_->appendEnum(S("slope"));
    method_->appendEnum(S("decrease"));
    method_->appendEnum(S("rolloff"));
#undef S
    if (positionalSymbolArgument(1))
        method_->set(Atom(positionalSymbolArgument(1)));
    createProperty(method_);

    createProperty(new PointerProperty<bool>("@active", &active_, false));

    createOutlet();
    createOutlet();
}

void AubioOnset::parseProperties()
{
    SoundExternal::parseProperties();

    if (hop_size_->value() <= 0)
        hop_size_->setValue(buffer_size_->value() / 2);

    if (hop_size_->value() > buffer_size_->value()) {
        OBJ_ERR << "invalid value: " << hop_size_->value() << ". should be <= " << buffer_size_->value();
        hop_size_->setValue(buffer_size_->value() / 2);
    }

    initOnset(sys_getsr());

    if (!onset_) {
        OBJ_ERR << "can't init aubio";
        return;
    }

    updatePtr(onset_.get(), CEAMMC_AUBIO_ONSET_PLIST);

    SoundExternal::parseProperties();

    setPropertyCallback(propCallback);

    buffer_size_->setReadonly(true);
    hop_size_->setReadonly(true);

    in_.reset(new_fvec(hop_size_->value()));
    out_.reset(new_fvec(1));
}

void AubioOnset::processBlock(const t_sample** in, t_sample** out)
{
    if (!in_ || !out_ || !onset_ || !active_)
        return;

    const auto BS = blockSize();
    const auto HOP_LAST_IDX = hop_size_->value() - 1;
    for (size_t i = 0; i < BS; i++) {
        fvec_set_sample(in_.get(), in[0][i], dsp_pos_);

        if (dsp_pos_ < HOP_LAST_IDX) {
            dsp_pos_++;
        } else {
            /* block loop */
            aubio_onset_do(onset_.get(), in_.get(), out_.get());
            if (fvec_get_sample(out_.get(), 0) > 0) {
                last_ms_ = aubio_onset_get_last_ms(onset_.get());
                tick_.delay(0);
            }
            /* end of block loop */
            dsp_pos_ = 0;
        }
    }
}

void AubioOnset::samplerateChanged(size_t sr)
{
    saveValues(CEAMMC_AUBIO_ONSET_PLIST);
    initOnset(sr);
    updatePtr(onset_.get(), CEAMMC_AUBIO_ONSET_PLIST);
    restoreValues(CEAMMC_AUBIO_ONSET_PLIST);
}

void AubioOnset::m_reset(t_symbol* m, const AtomList&)
{
    if (onset_)
        aubio_onset_reset(onset_.get());
}

void AubioOnset::clock_tick()
{
    floatTo(1, last_ms_);
    bangTo(0);
}

void AubioOnset::initOnset(uint_t sr)
{
    if (sr == 0)
        sr = 44100;

    onset_.reset(
        new_aubio_onset(
            method_->value()->s_name,
            buffer_size_->value(),
            hop_size_->value(),
            sr),
        del_aubio_onset);
}

void AubioOnset::updateMethodProperty()
{
    saveValues(CEAMMC_AUBIO_ONSET_RESTORE);
    initOnset(sys_getsr());
    updatePtr(onset_.get(), CEAMMC_AUBIO_ONSET_PLIST);
    restoreValues(CEAMMC_AUBIO_ONSET_RESTORE);
}

void AubioOnset::propCallback(BaseObject* this_, t_symbol* name)
{
    AubioOnset* this__ = dynamic_cast<AubioOnset*>(this_);
    if (!this__)
        return;

    if (name == gensym("@method"))
        this__->updateMethodProperty();
}

void setup_an_onset_tilde()
{
    SoundExternalFactory<AubioOnset> obj("an.onset~");
    obj.addMethod("reset", &AubioOnset::m_reset);
}
