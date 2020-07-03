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
#include "an_aubio_onset.h"
#include "ceammc_factory.h"
#include "ceammc_property_callback.h"

constexpr int DEFAULT_BUFFER_SIZE = 1024;
constexpr int MIN_BUFFER_SIZE = 64;
constexpr uint_t AUBIO_OK = 0;

AubioOnset::AubioOnset(const PdArgs& args)
    : BaseObject(args)
    , array_name_(&s_)
    , buffer_size_(nullptr)
    , hop_size_(nullptr)
    , method_(nullptr)
    , threshold_(nullptr)
    , silence_threshold_(nullptr)
    , speedlim_(nullptr)
    , onset_(new_aubio_onset(
                 DEFAULT_METHOD,
                 DEFAULT_BUFFER_SIZE,
                 DEFAULT_BUFFER_SIZE / 2,
                 44100),
          del_aubio_onset)
{
    createCbSymbolProperty(
        "@array",
        [this]() -> t_symbol* { return array_name_; },
        [this](t_symbol* s) -> bool { return setArray(s); })
        ->setArgIndex(0);

    buffer_size_ = new IntProperty("@bs", DEFAULT_BUFFER_SIZE);
    buffer_size_->checkMinEq(MIN_BUFFER_SIZE);
    buffer_size_->setUnitsSamp();
    buffer_size_->setInitOnly();
    buffer_size_->setArgIndex(1);

    hop_size_ = new HopSizeProperty(buffer_size_);
    hop_size_->setArgIndex(3);

    method_ = new OnsetMethodProperty();
    method_->setArgIndex(2);

    // order is important
    addProperty(buffer_size_);
    addProperty(hop_size_);
    addProperty(method_);

    addProperty(new SymbolEnumAlias("@hfc", method_, gensym("hfc")));
    addProperty(new SymbolEnumAlias("@energy", method_, gensym("energy")));
    addProperty(new SymbolEnumAlias("@complex", method_, gensym("complex")));
    addProperty(new SymbolEnumAlias("@phase", method_, gensym("phase")));
    addProperty(new SymbolEnumAlias("@wphase", method_, gensym("wphase")));
    addProperty(new SymbolEnumAlias("@specdiff", method_, gensym("specdiff")));
    addProperty(new SymbolEnumAlias("@kl", method_, gensym("kl")));
    addProperty(new SymbolEnumAlias("@mkl", method_, gensym("mkl")));
    addProperty(new SymbolEnumAlias("@specflux", method_, gensym("specflux")));

    threshold_ = new OnsetFloatProperty(
        "@threshold",
        [this]() -> t_float { return aubio_onset_get_threshold(onset_.get()); },
        [this](t_float t) -> bool { return aubio_onset_set_threshold(onset_.get(), t) == AUBIO_OK; });
    addProperty(threshold_);

    speedlim_ = new OnsetFloatProperty(
        "@speedlim",
        [this]() -> t_float { return aubio_onset_get_minioi_ms(onset_.get()); },
        [this](t_float ms) -> bool { return aubio_onset_set_minioi_ms(onset_.get(), ms) == AUBIO_OK; });
    speedlim_->setUnitsMs();
    speedlim_->setFloatCheck(PropValueConstraints::GREATER_EQUAL, 1);
    addProperty(speedlim_);

    silence_threshold_ = new OnsetFloatProperty(
        "@silence",
        [this]() -> t_float { return aubio_onset_get_silence(onset_.get()); },
        [this](t_float t) -> bool { return aubio_onset_set_silence(onset_.get(), t) == AUBIO_OK; });
    silence_threshold_->setUnitsDb();
    silence_threshold_->setFloatCheck(PropValueConstraints::CLOSED_RANGE, -80, 0);
    addProperty(silence_threshold_);

    auto delay = createCbFloatProperty(
        "@delay",
        [this]() -> t_float { return aubio_onset_get_delay_ms(onset_.get()); },
        [this](t_float ms) -> bool { return aubio_onset_set_delay_ms(onset_.get(), ms) == AUBIO_OK; });
    delay->setUnitsMs();
    delay->setFloatCheck(PropValueConstraints::GREATER_EQUAL, 0);

    createCbFloatProperty(
        "@compression",
        [this]() -> t_float { return aubio_onset_get_compression(onset_.get()); },
        [this](t_sample v) -> bool { return aubio_onset_set_compression(onset_.get(), v) == AUBIO_OK; })
        ->setFloatCheck(PropValueConstraints::GREATER_EQUAL, 0);

    createCbBoolProperty(
        "@awhitening",
        [this]() -> bool { return aubio_onset_get_awhitening(onset_.get()); },
        [this](bool v) -> bool { return aubio_onset_set_awhitening(onset_.get(), v) == AUBIO_OK; });

    createOutlet();
}

bool AubioOnset::setArray(t_symbol* s)
{
    array_name_ = s;
    if (!array_.open(array_name_)) {
        OBJ_ERR << "array not found: " << s->s_name;
        return false;
    }

    return true;
}

bool AubioOnset::checkArray()
{
    if (array_name_ == nullptr || !array_.open(array_name_)) {
        OBJ_ERR << "invalid array: " << array_.name();
        return false;
    }

    return true;
}

void AubioOnset::onBang()
{
    if (!checkArray())
        return;

    const auto HOP_N = hop_size_->value();
    const auto ARRAY_N = array_.size();

    AtomList ms;

    size_t hop_idx = 0;
    while (true) {
        const auto hop_offset = hop_idx * HOP_N;

        if (hop_offset >= ARRAY_N)
            break;

        for (size_t i = 0; i < HOP_N; i++) {
            const auto hop_pos = hop_offset + i;
            if (hop_pos < ARRAY_N)
                fvec_set_sample(in_.get(), array_[hop_pos], i);
            else
                fvec_set_sample(in_.get(), 0.f, i);
        }

        aubio_onset_do(onset_.get(), in_.get(), out_.get());
        if (fvec_get_sample(out_.get(), 0) > 0)
            ms.append(Atom(aubio_onset_get_last_ms(onset_.get())));

        hop_idx++;
    }

    aubio_onset_reset(onset_.get());
    listTo(0, ms);
}

void AubioOnset::initDone()
{
    BaseObject::initDone();

    // save this properties values to restore them later
    saveSteadyProperties();

    // here we create new aubio onset object and
    // some of our defined properties are reset with default values
    resetAubioOnset(sys_getsr());

    // consistency check
    if (!onset_) {
        OBJ_ERR << "can't create aubio onset object";
        return;
    }

    // restore properties if it was explictly specified in creation arguments
    // each property have information if setter callback was called
    if (threshold_->wasChanged())
        threshold_->restore();

    if (silence_threshold_->wasChanged())
        silence_threshold_->restore();

    if (speedlim_->wasChanged())
        speedlim_->restore();

    // set success callback for changing onset method
    // we need to delete old and create new Onset object every time
    // also we need to save and restore properties
    method_->setSuccessFn([this](Property*) {
        // save properties that should be steady
        saveSteadyProperties();
        // steady properties are changed after this call
        resetAubioOnset(sys_getsr());
        // restore
        restoreSteadyProperties();
    });

    in_.reset(new_fvec(hop_size_->value()));
    out_.reset(new_fvec(1));
}

void AubioOnset::resetAubioOnset(uint_t sr)
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

AtomList AubioOnset::propArray() const
{
    return AtomList(array_name_);
}

void AubioOnset::propSetArray(const AtomList& l)
{
    if (l.empty() || !l[0].isSymbol()) {
        OBJ_ERR << "array name required";
        return;
    }

    setArray(l[0].asSymbol());
}

void AubioOnset::saveSteadyProperties()
{
    threshold_->save();
    silence_threshold_->save();
    speedlim_->save();
}

void AubioOnset::restoreSteadyProperties()
{
    threshold_->restore();
    silence_threshold_->restore();
    speedlim_->restore();
}

void setup_an_onset()
{
    ObjectFactory<AubioOnset> obj("an.onset");
}
