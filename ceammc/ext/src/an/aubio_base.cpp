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
#include "aubio_base.h"

HopSizeProperty::HopSizeProperty(IntProperty* buf_size)
    : IntProperty("@hs", 0)
    , bs_(buf_size)
{
    setUnitsSamp();
    checkNonNegative();
    setIntCheckFn(
        [this](int v) -> bool {
            const auto BS = bs_->value();
            if (v < 0 || v > BS)
                return false;
            else
                return true;
        },
        "value >= 0 and <= buffer size expected");
}

int HopSizeProperty::value() const
{
    auto v = IntProperty::value();
    if (v == 0 || v > bs_->value())
        return bs_->value() / 2;
    return v;
}

AtomList HopSizeProperty::get() const
{
    return { t_float(value()) };
}

OnsetMethodProperty::OnsetMethodProperty()
    : SymbolEnumProperty("@method",
        { DEFAULT_METHOD, "energy", "hfc", "specdiff", "complexdomain", "complex", "phase", "wphase", "kl", "specflux", "centroid", "spread", "skewness", "kurtosis", "slope", "decrease", "rolloff" })
{
}

OnsetFloatProperty::OnsetFloatProperty(const std::string& name, PropertyFloatGetter getter, PropertyFloatSetter setter)
    : CallbackProperty(
        name,
        getter,
        [this, setter](t_float f) -> bool {
            save();
            setter_was_called_ = true;
            return setter(f);
        })
    , fget_(getter)
    , fset_(setter)
    , setter_was_called_(false)
{
}
