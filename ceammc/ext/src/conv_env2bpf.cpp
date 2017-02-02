//
//  2envbpf.cpp
//  env2bpf
//
//  Created by Alex Nadzharov on 06/01/17.
//
//

#include "ceammc.hpp"
#include "ceammc_factory.h"
//#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class Env2Bpf : public BaseObject {
    AtomList out_list_;

    SymbolEnumProperty* method_;
    Atom type_;

public:
    Env2Bpf(const PdArgs& a)
        : BaseObject(a)
        , type_(gensym("adsr"))

    {
        createOutlet();

        initProperties();
        // parse creation arguments and properties
        parseArguments();
    }

    void onBang()
    {
        listTo(0, out_list_);
    }

    void onList(const AtomList& l)
    {
        onBang();
    }

    void initProperties()
    {
    }
};

extern "C" void setup_conv0x2eenv2bpf()
{
    ObjectFactory<Env2Bpf> obj("conv.env2bpf");
}