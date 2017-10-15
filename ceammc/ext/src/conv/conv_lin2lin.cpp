//
//  conv_lin2lin.cpp
//  lin2lin
//
//  Created by Alex Nadzharov on 03/02/17.
//
//

#include "conv_lin2lin.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"

Lin2Lin::Lin2Lin(const PdArgs& a)
    : BaseRangeConverter(a, 0, 127, 0, 1)
{
    createOutlet();
}

void Lin2Lin::onFloat(float value)
{
    if (doClip(value))
        return floatTo(0, value);

    const t_float x0 = in_from();
    const t_float x1 = in_to();
    const t_float y0 = out_from();
    const t_float y1 = out_to();

    if (x0 == x1) {
        OBJ_ERR << "invalid input range: " << x0 << '-' << x1;
        return;
    }

    floatTo(0, convert::lin2lin<t_float>(value, x0, x1, y0, y1));
}

extern "C" void setup_conv0x2elin2lin()
{
    ObjectFactory<Lin2Lin> obj("conv.lin2lin");
    obj.addAlias("lin->lin");
}
