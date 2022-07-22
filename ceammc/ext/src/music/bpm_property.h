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
#ifndef BPM_PROPERTY_H
#define BPM_PROPERTY_H

#include "ceammc_property.h"

namespace ceammc {

class BpmProperty : public Property {
    t_float value_ { 0 };
    t_float beatlen_ { 0 };

public:
    BpmProperty(const std::string& name,
        t_float init,
        PropValueAccess access = PropValueAccess::READWRITE,
        t_float beatlen = 0.25);

    AtomList get() const override;
    bool setList(const AtomListView& lv) override;
    bool setFloat(t_float f) override;
    bool getFloat(t_float& f) const override;

    AtomList defaultValue() const;

    inline t_float value() const { return value_; }
    bool setValue(t_float f);

    t_float beatlen() const { return beatlen_; }
    bool setBeatlen(t_float beatlen);

    t_float beatPeriodMs() const;
    t_float wholePeriodMs() const;
};

}

#endif // BPM_PROPERTY_H
