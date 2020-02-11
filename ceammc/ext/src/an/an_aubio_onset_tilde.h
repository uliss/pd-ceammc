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
#ifndef AN_AUBIO_ONSET_TILDE_H
#define AN_AUBIO_ONSET_TILDE_H

#include <functional>
#include <memory>

#include "aubio.h"
#include "ceammc_clock.h"
#include "ceammc_property_extra.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

template <typename T, typename AubioStruct>
class AubioProperty : public Property {
public:
    typedef AubioStruct* AubioPtr;
    typedef std::function<T(AubioStruct*)> AubioGetter;
    typedef std::function<uint_t(AubioStruct*, T)> AubioSetter;

public:
    AubioProperty(const std::string& name, AubioPtr onset,
        AubioGetter getter,
        AubioSetter setter)
        : Property(PropertyInfo(name, PropertyInfo::toType<T>()))
        , aubio_ptr_(onset)
        , getter_(getter)
        , setter_(setter)
    {
    }

    bool set(const AtomList& lst) final
    {
        return setValue(atomlistToValue<T>(lst, T()));
    }

    AtomList get() const final
    {
        return AtomList(Atom(value()));
    }

    T value() const
    {
        if (aubio_ptr_)
            return getter_(aubio_ptr_);
        else
            return T();
    }

    bool setValue(T v)
    {
        if (aubio_ptr_)
            return setter_(aubio_ptr_, v) == 0;

        return false;
    }

    void setAubioPtr(const AubioPtr& p)
    {
        aubio_ptr_ = p;
    }

    void save()
    {
        saved_value_ = value();
    }

    void restore()
    {
        setValue(saved_value_);
    }

private:
    AubioPtr aubio_ptr_;
    AubioGetter getter_;
    AubioSetter setter_;
    T saved_value_;
};

typedef AubioProperty<smpl_t, aubio_onset_t> OnsetFloatProperty;
typedef AubioProperty<int, aubio_onset_t> OnsetUIntProperty;

struct FVecDeleter {
    void operator()(fvec_t* v) { del_fvec(v); }
};

typedef std::unique_ptr<fvec_t, FVecDeleter> FVecPtr;
typedef std::shared_ptr<aubio_onset_t> OnsetPtr;

class AubioOnset : public SoundExternal {
    IntPropertyMinEq* buffer_size_;
    IntProperty* hop_size_;
    OnsetFloatProperty* threshold_;
    OnsetFloatProperty* delay_;
    OnsetFloatProperty* speedlim_;
    OnsetFloatProperty* silence_threshold_;
    OnsetFloatProperty* compression_;
    OnsetUIntProperty* awhitening_;
    SymbolEnumProperty* method_;

    int dsp_pos_;
    t_float last_ms_;

    FVecPtr in_, out_;
    OnsetPtr onset_;

    ClockMemberFunction<AubioOnset> tick_;

public:
    AubioOnset(const PdArgs& args);

    void parseProperties() final;

    void processBlock(const t_sample** in, t_sample** out) final;
    void samplerateChanged(size_t sr) final;

    void m_reset(t_symbol* m, const AtomList&);

private:
    void clock_tick();
    void initOnset(uint_t sr);
    void updateMethodProperty();
    static void propCallback(BaseObject* this_, t_symbol* name);
};

void setup_an_onset_tilde();

#endif // AN_AUBIO_ONSET_TILDE_H
