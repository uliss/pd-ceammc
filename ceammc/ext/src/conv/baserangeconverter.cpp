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
#include "baserangeconverter.h"

BaseRangeConverter::BaseRangeConverter(const PdArgs& a,
    t_float iMin, t_float iMax, t_float oMin, t_float oMax)
    : BaseObject(a)
    , input_from_(0)
    , input_to_(0)
    , output_from_(0)
    , output_to_(0)
    , clip_(0)
    , no_clip_(gensym("noclip"))
    , clip_min_(gensym("min"))
    , clip_max_(gensym("max"))
    , clip_minmax_(gensym("minmax"))
{
    input_from_ = new FloatProperty("@in_from", positionalFloatArgument(0, iMin));
    input_to_ = new FloatProperty("@in_to", positionalFloatArgument(1, iMax));
    output_from_ = new FloatProperty("@out_from", positionalFloatArgument(2, oMin));
    output_to_ = new FloatProperty("@out_to", positionalFloatArgument(3, oMax));

    clip_ = new SymbolEnumProperty("@clip", "minmax");
    clip_->appendEnum("noclip");
    clip_->appendEnum("min");
    clip_->appendEnum("max");

    createProperty(input_from_);
    createProperty(input_to_);
    createProperty(output_from_);
    createProperty(output_to_);
    createProperty(clip_);

    createProperty(new SymbolEnumAlias("@noclip", clip_, no_clip_));
    createProperty(new SymbolEnumAlias("@min", clip_, clip_min_));
    createProperty(new SymbolEnumAlias("@max", clip_, clip_max_));
    createProperty(new SymbolEnumAlias("@minmax", clip_, clip_minmax_));
}

bool BaseRangeConverter::doClip(t_float& v) const
{
    if (clipMinMax()) {
        if (v <= in_from()) {
            v = out_from();
            return true;
        }

        if (v >= in_to()) {
            v = out_to();
            return true;
        }
    }

    if (clipMin() && v <= in_from()) {
        v = out_from();
        return true;
    }

    if (clipMax() && v >= in_to()) {
        v = out_to();
        return true;
    }

    return false;
}
