//
//  conv_lin2lin.cpp
//  lin2lin
//
//  Created by Alex Nadzharov on 03/02/17.
//
//

#include "ceammc.hpp"
#include "ceammc_factory.h"
//#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class Lin2lin : public BaseObject {
    AtomList out_list_;

    FloatProperty* input_min;
    FloatProperty* input_max;
    FloatProperty* output_min;
    FloatProperty* output_max;

    SymbolEnumProperty* method_;

public:
    Lin2lin(const PdArgs& a)
        : BaseObject(a)

    {
        createInlet();
        createInlet();
        createInlet();
        createInlet();

        createOutlet();

        initProperties();
        // parse creation arguments and properties
        parseArguments();
    }

    void onBang()
    {
        listTo(0, out_list_);
    }

    void onFloat(float value)
    {

        float input_range = (this->input_max->value() - this->input_min->value());
        float output_range = (this->output_max->value() - this->output_min->value());

        value = (input_range == 0) ? 0 : (value - this->input_min->value()) / input_range;
        value = value * output_range + output_min->value();

        out_list_ = AtomList(Atom(value));

        onBang();
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (n == 1) { /*input range min*/
            this->input_min->setValue(l.at(0).asFloat());
        }

        if (n == 2) { /*input range max*/
            this->input_max->setValue(l.at(0).asFloat());
        }

        if (n == 3) { /*output range min*/
            this->output_min->setValue(l.at(0).asFloat());
        }

        if (n == 4) { /*output range max*/
            this->output_max->setValue(l.at(0).asFloat());
        }
    }

    void initProperties()
    {
        this->input_min = new FloatProperty("input_min");
        this->input_max = new FloatProperty("input_max");
        this->output_min = new FloatProperty("output_min");
        this->output_max = new FloatProperty("output_max");
    }
};

extern "C" void setup_conv0x2elin2lin()
{
    ObjectFactory<Lin2lin> obj("conv.lin2lin");
}