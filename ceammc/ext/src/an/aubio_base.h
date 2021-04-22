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
#include <stack>

#include "aubio.h"
#include "ceammc_property_callback.h"
#include "ceammc_property.h"
#include "ceammc_property_enum.h"

using namespace ceammc;

struct FVecDeleter {
    void operator()(fvec_t* v) { del_fvec(v); }
};

using FVecPtr = std::unique_ptr<fvec_t, FVecDeleter>;
using OnsetPtr = std::shared_ptr<aubio_onset_t>;
using TempoPtr = std::unique_ptr<aubio_tempo_t, void(*)(aubio_tempo_t*)>;

class HopSizeProperty : public IntProperty {
    IntProperty* bs_;

public:
    HopSizeProperty(IntProperty* buf_size);
    int value() const;
    AtomList get() const override;
};

class OnsetFloatProperty : public CallbackProperty {
    PropertyFloatGetter fget_;
    PropertyFloatSetter fset_;
    t_float v_;
    bool setter_was_called_;

public:
    OnsetFloatProperty(const std::string& name, PropertyFloatGetter getter, PropertyFloatSetter setter);

    void save() { v_ = fget_(); }
    void restore() { fset_(v_); }
    bool wasChanged() const { return setter_was_called_; }
};

constexpr const char* DEFAULT_METHOD = "default";

class OnsetMethodProperty : public SymbolEnumProperty {
public:
    OnsetMethodProperty();
};

#endif // AUBIO_BASE_H
