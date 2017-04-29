//
//  2bpflist.cpp
//  bpf2list
//
//  Created by Alex Nadzharov on 06/01/17.
//
//

#include "ceammc.hpp"
#include "ceammc_factory.h"
//#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

#include "ceammc_bpf.h"

using namespace ceammc;

class Bpf2List : public BaseObject {
    AtomList out_list_;

    SymbolEnumProperty* method_;
    Atom size_;

public:
    Bpf2List(const PdArgs& a)
        : BaseObject(a)
        , size_(16)

    {
        createOutlet();
        createInlet();

        initProperties();
    }

    void onBang()
    {
        listTo(0, this->out_list_);
    }

    void onList(const AtomList& l)
    {
        BPF func;
        func.empty();
        //temporary
        func.initRange();

        func.setBPFList(l);

        this->out_list_ = func.getVector(this->size_.asInt());

        onBang();
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (n == 1) {
            this->size_ = l.at(0);
            if (this->size_.asFloat() < 2)
                this->size_ = Atom(2.0f);
        }
    }

    void initProperties()
    {
        createCbProperty("@size", &Bpf2List::getSize, &Bpf2List::setSize);
    }

    AtomList getSize() const { return AtomList(Atom(this->size_)); }

    void setSize(const AtomList& l)
    {
        this->size_ = l.at(0).asInt();
    }
};

extern "C" void setup_conv0x2ebpf2list()
{
    ObjectFactory<Bpf2List> obj("conv.bpf2list");
}
