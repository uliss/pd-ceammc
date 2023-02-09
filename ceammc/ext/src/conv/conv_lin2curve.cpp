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
#include "conv_lin2curve.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

using namespace ceammc;

Lin2Curve::Lin2Curve(const PdArgs& a)
    : BaseRangeConverter(a)
    , curve_(0)
{
    createOutlet();

    curve_ = new FloatProperty("@curve", -4);
    curve_->setArgIndexNext(output_to_);
    addProperty(curve_);
}

void Lin2Curve::onFloat(t_float value)
{
    const t_float x0 = in_from();
    const t_float x1 = in_to();
    const t_float y0 = out_from();
    const t_float y1 = out_to();
    const t_float curve = curve_->value();

    if (doClip(value, clipType()))
        return floatTo(0, value);

    if (x0 == x1) {
        OBJ_ERR << "invalid input range: " << x0 << ".." << x1;
        return;
    }

    floatTo(0, convert::lin2curve(value, x0, x1, y0, y1, curve));
}

void Lin2Curve::onList(const AtomListView& lv)
{
    const t_float x0 = in_from();
    const t_float x1 = in_to();
    const t_float y0 = out_from();
    const t_float y1 = out_to();
    const t_float curve = curve_->value();

    auto type = clipType();

    auto fn = [this, x0, x1, y0, y1, type, curve](t_float value) {
        if (doClip(value, type))
            return value;

        return convert::lin2curve(value, x0, x1, y0, y1, curve);
    };

    SmallAtomList res;
    res.reserve(lv.size());
    lv.mapFloat(fn, res);
    listTo(0, res.view());
}

void setup_conv_lin2curve()
{
    ObjectFactory<Lin2Curve> obj("conv.lin2curve");
    obj.addAlias("lin->curve");

    obj.setXletsInfo({ "float or list" }, { "list" });

    obj.setDescription("map linear to exponential range with 0 allowed.");
    obj.setCategory("conv");
    obj.setKeywords({"convert", "exponential", "curve"});
}
