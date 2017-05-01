//
//  conv_lin2exp.cpp
//  lin2exp
//
//  Created by Alex Nadzharov on 03/02/17.
//
//

#include "conv_lin2exp.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"

Lin2Exp::Lin2Exp(const PdArgs& a)
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
    createInlet();
    createInlet();
    createInlet();
    createInlet();

    createOutlet();

    input_from_ = new FloatProperty("@in_from", positionalFloatArgument(0, 0));
    input_to_ = new FloatProperty("@in_to", positionalFloatArgument(1, 127));
    output_from_ = new FloatProperty("@out_from", positionalFloatArgument(2, 0.01));
    output_to_ = new FloatProperty("@out_to", positionalFloatArgument(3, 1));

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

void Lin2Exp::onFloat(float value)
{
    const t_float x0 = input_from_->value();
    const t_float x1 = input_to_->value();
    const t_float y0 = output_from_->value();
    const t_float y1 = output_to_->value();

    t_symbol* clip_method = clip_->value();

    if (clip_method == clip_minmax_) {
        if (value <= x0)
            return floatTo(0, y0);
        if (value >= x1)
            return floatTo(0, y1);
    }

    if (clip_method == clip_min_ && value < x0)
        return floatTo(0, y0);

    if (clip_method == clip_max_ && value >= x1)
        return floatTo(0, y1);

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

void Lin2Exp::onInlet(size_t n, const AtomList& l)
{
    if (l.empty())
        return;

    if (n == 1) { /*input range from*/
        input_from_->setValue(l.at(0).asFloat());
    }

    if (n == 2) { /*input range to*/
        input_to_->setValue(l.at(0).asFloat());
    }

    if (n == 3) { /*output range from*/
        output_from_->setValue(l.at(0).asFloat());
    }

    if (n == 4) { /*output range to*/
        output_to_->setValue(l.at(0).asFloat());
    }
}

extern "C" void setup_conv0x2elin2exp()
{
    ObjectFactory<Lin2Exp> obj("conv.lin2exp");
    obj.addAlias("lin->exp");
}
