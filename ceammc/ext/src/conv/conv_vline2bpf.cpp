//
//  2vlinebpf.cpp
//  vline2bpf
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

class Vline2Bpf : public BaseObject {
    AtomList out_list_;

    SymbolEnumProperty* method_;

public:
    Vline2Bpf(const PdArgs& a)
        : BaseObject(a)

    {
        createOutlet();

        initProperties();
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

extern "C" void setup_conv0x2evline2bpf()
{
    ObjectFactory<Vline2Bpf> obj("conv.vline2bpf");
}
