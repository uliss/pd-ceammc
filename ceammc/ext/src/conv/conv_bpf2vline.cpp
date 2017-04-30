//
//  2bpfvline.cpp
//  bpf2vline
//
//  Created by Alex Nadzharov on 06/01/17.
//
//

#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

#include "ceammc_bpf.h"

using namespace ceammc;

class Bpf2Vline : public BaseObject {
    vector<AtomList> vline_;

public:
    Bpf2Vline(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onBang()
    {
        for (size_t i = 0; i < vline_.size(); i++)
            listTo(0, vline_.at(i));
    }

    void onList(const AtomList& l)
    {
        BPF func;
        //temporary
        func.initRange();

        func.setBPFList(l);
        vline_ = func.getVline();

        onBang();
    }
};

extern "C" void setup_conv0x2ebpf2vline()
{
    ObjectFactory<Bpf2Vline> obj("conv.bpf2vline");
}
