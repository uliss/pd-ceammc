//
//  conv_lin2lin.cpp
//  lin2lin
//
//  Created by Alex Nadzharov on 03/02/17.
//
//

#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class Lin2lin : public BaseObject {
    FloatProperty* input_from_;
    FloatProperty* input_to_;
    FloatProperty* output_from_;
    FloatProperty* output_to_;
    BoolProperty* clip_;

public:
    Lin2lin(const PdArgs& a)
        : BaseObject(a)
    {
        createInlet();
        createInlet();
        createInlet();
        createInlet();

        createOutlet();

        input_from_ = new FloatProperty("@in_from", positionalFloatArgument(0, 0));
        input_to_ = new FloatProperty("@in_to", positionalFloatArgument(1, 127));
        output_from_ = new FloatProperty("@out_from", positionalFloatArgument(2, 0));
        output_to_ = new FloatProperty("@out_to", positionalFloatArgument(3, 1));
        clip_ = new BoolProperty("@clip", true);

        createProperty(input_from_);
        createProperty(input_to_);
        createProperty(output_from_);
        createProperty(output_to_);
        createProperty(clip_);
    }

    void onFloat(float value)
    {
        if (clip_->value()) {
            value = clip<t_float>(value, std::min(input_from_->value(), input_to_->value()),
                std::max(input_from_->value(), input_to_->value()));
        }

        if (input_from_->value() == input_to_->value()) {
            OBJ_ERR << "invalid input range: " << input_from_->value() << '-' << input_to_->value();
            return;
        }

        floatTo(0, convert::lin2lin<t_float>(value, input_from_->value(), input_to_->value(), output_from_->value(), output_to_->value()));
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (l.empty())
            return;

        if (n == 1) { /*input range from*/
            this->input_from_->setValue(l.at(0).asFloat());
        }

        if (n == 2) { /*input range to*/
            this->input_to_->setValue(l.at(0).asFloat());
        }

        if (n == 3) { /*output range from*/
            this->output_from_->setValue(l.at(0).asFloat());
        }

        if (n == 4) { /*output range to*/
            this->output_to_->setValue(l.at(0).asFloat());
        }
    }
};

extern "C" void setup_conv0x2elin2lin()
{
    ObjectFactory<Lin2lin> obj("conv.lin2lin");
    obj.addAlias("lin->lin");
}
