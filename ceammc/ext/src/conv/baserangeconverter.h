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

#include <type_traits>

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

template <typename T>
class BaseRangeConverterT : public T {
protected:
    FloatProperty* input_from_;
    FloatProperty* input_to_;
    FloatProperty* output_from_;
    FloatProperty* output_to_;
    SymbolEnumProperty* clip_;
    t_symbol* SYM_NO_CLIP;
    t_symbol* SYM_MIN;
    t_symbol* SYM_MAX;
    t_symbol* SYM_MINMAX;

public:
    enum ClipType {
        NO_CLIP = 0,
        CLIP_MIN,
        CLIP_MAX,
        CLIP_MINMAX
    };

    BaseRangeConverterT(const PdArgs& args, t_float iMin = 0, t_float iMax = 127, t_float oMin = 0, t_float oMax = 1)
        : T(args)
        , input_from_(nullptr)
        , input_to_(nullptr)
        , output_from_(nullptr)
        , output_to_(nullptr)
        , clip_(nullptr)
    {
        SYM_NO_CLIP = gensym("noclip");
        SYM_MIN = gensym("min");
        SYM_MAX = gensym("max");
        SYM_MINMAX = gensym("minmax");

        input_from_ = new FloatProperty("@in_from", iMin);
        input_from_->setArgIndex(0);
        input_to_ = new FloatProperty("@in_to", iMax);
        input_to_->setArgIndex(1);
        output_from_ = new FloatProperty("@out_from", oMin);
        output_from_->setArgIndex(2);
        output_to_ = new FloatProperty("@out_to", oMax);
        output_to_->setArgIndex(3);

        clip_ = new SymbolEnumProperty("@clip", { SYM_MINMAX, SYM_NO_CLIP, SYM_MIN, SYM_MAX });

        T::addProperty(input_from_);
        T::addProperty(input_to_);
        T::addProperty(output_from_);
        T::addProperty(output_to_);
        T::addProperty(clip_);

        T::addProperty(new SymbolEnumAlias("@noclip", clip_, SYM_NO_CLIP));
        T::addProperty(new SymbolEnumAlias("@min", clip_, SYM_MIN));
        T::addProperty(new SymbolEnumAlias("@max", clip_, SYM_MAX));
        T::addProperty(new SymbolEnumAlias("@minmax", clip_, SYM_MINMAX));
    }

    inline t_float in_from() const { return input_from_->value(); }
    inline t_float in_to() const { return input_to_->value(); }
    inline t_float out_from() const { return output_from_->value(); }
    inline t_float out_to() const { return output_to_->value(); }

    inline bool clipMin() const { return clip_->value() == SYM_MIN; }
    inline bool clipMax() const { return clip_->value() == SYM_MAX; }
    inline bool clipMinMax() const { return clip_->value() == SYM_MINMAX; }
    inline bool noClip() const { return clip_->value() == SYM_NO_CLIP; }

    ClipType clipType() const
    {
        auto v = clip_->value();
        if (v == SYM_NO_CLIP)
            return NO_CLIP;
        else if (v == SYM_MIN)
            return CLIP_MIN;
        else if (v == SYM_MAX)
            return CLIP_MAX;
        else if (v == SYM_MINMAX)
            return CLIP_MINMAX;
        else
            return NO_CLIP;
    }

    bool doClip(t_float& v, ClipType type) const
    {
        switch (type) {
        case CLIP_MINMAX:
            if (v <= in_from()) {
                v = out_from();
                return true;
            }

            if (v >= in_to()) {
                v = out_to();
                return true;
            }
            break;
        case CLIP_MIN:
            if (v <= in_from()) {
                v = out_from();
                return true;
            }
            break;
        case CLIP_MAX:
            if (v >= in_to()) {
                v = out_to();
                return true;
            }
            break;
        default:
            break;
        }

        return false;
    }
};

typedef BaseRangeConverterT<ceammc::BaseObject> BaseRangeConverter;
typedef BaseRangeConverterT<ceammc::SoundExternal> BaseRangeConverterTilde;

#endif // BASERANGECONVERTER_H
