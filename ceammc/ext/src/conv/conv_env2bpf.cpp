//
//  2envbpf.cpp
//  env2bpf
//
//  Created by Alex Nadzharov on 06/01/17.
//
//

#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class Env2Bpf : public BaseObject {
    AtomList out_list_;

public:
    Env2Bpf(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onBang()
    {
        listTo(0, out_list_);
    }

    void onList(const AtomList& l)
    {
        onBang();
    }
};

extern "C" void setup_conv0x2eenv2bpf()
{
    ObjectFactory<Env2Bpf> obj("conv.env2bpf");
}
