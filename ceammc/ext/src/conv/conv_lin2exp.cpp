//
//  conv_lin2exp.cpp
//  lin2exp
//
//  Created by Alex Nadzharov on 03/02/17.
//
//

#include "conv_lin2exp.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"

Lin2Exp::Lin2Exp(const PdArgs& a)
    : BaseRangeConverter(a, 0, 127, 0.01f, 1)
{
    createOutlet();
}

void Lin2Exp::onFloat(t_float value)
{
    const t_float x0 = in_from();
    const t_float x1 = in_to();
    const t_float y0 = out_from();
    const t_float y1 = out_to();

    if (doClip(value, clipType()))
        return floatTo(0, value);

    if (x0 == x1) {
        OBJ_ERR << "invalid input range: " << x0 << ".." << x1;
        return;
    }

    if ((y0 <= 0 && 0 <= y1) || (y1 <= 0 && 0 <= y0)) {
        OBJ_ERR << "invalid output range: " << y0 << ".." << y1 << ". Should not contain 0.";
        return;
    }

    floatTo(0, convert::lin2exp(value, x0, x1, y0, y1));
}

void Lin2Exp::onList(const AtomListView& lv)
{
    const t_float x0 = in_from();
    const t_float x1 = in_to();
    const t_float y0 = out_from();
    const t_float y1 = out_to();

    auto type = clipType();

    auto fn = [this, x0, x1, y0, y1, type](t_float value) {
        if (doClip(value, type))
            return value;

        return convert::lin2exp(value, x0, x1, y0, y1);
    };

    SmallAtomList res;
    res.reserve(lv.size());
    lv.mapFloat(fn, res);
    listTo(0, res.view());
}

void setup_conv_lin2exp()
{
    ObjectFactory<Lin2Exp> obj("conv.lin2exp");
    obj.addAlias("lin->exp");

    obj.setXletsInfo({ "float or list" }, { "list" });
}
