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
#ifndef AUBIO_BASE_H
#define AUBIO_BASE_H

#include <functional>
#include <memory>

#include "aubio.h"
#include "ceammc_property_extra.h"

struct FVecDeleter {
    void operator()(fvec_t* v) { del_fvec(v); }
};

typedef std::unique_ptr<fvec_t, FVecDeleter> FVecPtr;
typedef std::shared_ptr<aubio_onset_t> OnsetPtr;

template <typename T, typename AubioStruct>
class AubioProperty : public ceammc::Property {
public:
    typedef AubioStruct* AubioPtr;
    typedef std::function<T(AubioStruct*)> AubioGetter;
    typedef std::function<uint_t(AubioStruct*, T)> AubioSetter;

public:
    AubioProperty(const std::string& name, AubioPtr onset,
        AubioGetter getter,
        AubioSetter setter)
        : Property(ceammc::PropertyInfo(name, ceammc::PropertyInfo::toType<T>()))
        , aubio_ptr_(onset)
        , getter_(getter)
        , setter_(setter)
    {
    }

    bool set(const ceammc::AtomList& lst) final
    {
        return setValue(ceammc::atomlistToValue<T>(lst, T()));
    }

    ceammc::AtomList get() const final
    {
        return ceammc::AtomList(ceammc::Atom(value()));
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

#endif // AUBIO_BASE_H
