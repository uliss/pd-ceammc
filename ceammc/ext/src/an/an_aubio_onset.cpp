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
    : BaseObject(args)
    , array_name_(positionalSymbolArgument(0, &s_))
    , buffer_size_(nullptr)
    , hop_size_(nullptr)
    , threshold_(nullptr)
    , delay_(nullptr)
    , method_(nullptr)
    , speedlim_(nullptr)
    , silence_threshold_(nullptr)
    , compression_(nullptr)
    , awhitening_(nullptr)
{
    if (array_name_ != &s_)
        array_.open(array_name_);

    createCbProperty("@array", &AubioOnset::propArray, &AubioOnset::propSetArray)
        ->info()
        .setType(PropertyInfoType::SYMBOL);

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
    if (array_name_ == 0 || !array_.open(array_name_)) {
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
    const auto HOP_LAST_IDX = HOP_N - 1;
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

void AubioOnset::parseProperties()
{
    BaseObject::parseProperties();

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

    BaseObject::parseProperties();

    setPropertyCallback(propCallback);

    buffer_size_->setReadonly(true);
    hop_size_->setReadonly(true);

    in_.reset(new_fvec(hop_size_->value()));
    out_.reset(new_fvec(1));
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

void setup_an_onset()
{
    ObjectFactory<AubioOnset> obj("an.onset");
}
