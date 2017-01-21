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

class Bpf2List : public BaseObject
{
    AtomList out_list_;
    
    SymbolEnumProperty* method_;
    Atom size_;
    
    
public:
    Bpf2List(const PdArgs& a)
    : BaseObject(a)
    , size_(8)
    
    {
        createOutlet();
        
        initProperties();
        // parse creation arguments and properties
        parseArguments();
    }
    
    void onBang()
    {
        listTo(0, this->out_list_);
    }
    
    void onList(const AtomList& l)
    {
        BPF func;
        func.setBPFList(l);
        //temporary
        func.initRange();
        
        this->out_list_ = func.getBpfList();
        
        
        //postatom(this->out_list_.size(), this->out_list_.toPdData());
        //post("");
        //this->out_list_ = func.getVector(this->size_.asInt());
        
        onBang();
    }
    
    void initProperties()
    {
        
    }
    
};

extern "C" void setup_conv0x2ebpf2list()
{
    ObjectFactory<Bpf2List> obj("conv.bpf2list");
}
