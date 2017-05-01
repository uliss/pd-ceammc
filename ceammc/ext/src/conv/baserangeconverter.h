/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef BASERANGECONVERTER_H
#define BASERANGECONVERTER_H

#include "ceammc_object.h"

using namespace ceammc;

class BaseRangeConverter : public BaseObject {
    FloatProperty* input_from_;
    FloatProperty* input_to_;
    FloatProperty* output_from_;
    FloatProperty* output_to_;
    SymbolEnumProperty* clip_;
    t_symbol* no_clip_;
    t_symbol* clip_min_;
    t_symbol* clip_max_;
    t_symbol* clip_minmax_;

public:
    BaseRangeConverter(const PdArgs& a, t_float iMin = 0, t_float iMax = 127, t_float oMin = 0, t_float oMax = 1);

    inline t_float in_from() const { return input_from_->value(); }
    inline t_float in_to() const { return input_to_->value(); }
    inline t_float out_from() const { return output_from_->value(); }
    inline t_float out_to() const { return output_to_->value(); }

    inline bool clipMin() const { return clip_->value() == clip_min_; }
    inline bool clipMax() const { return clip_->value() == clip_max_; }
    inline bool clipMinMax() const { return clip_->value() == clip_minmax_; }
    inline bool noClip() const { return clip_->value() == no_clip_; }

    bool doClip(t_float& v) const;
};

#endif // BASERANGECONVERTER_H
