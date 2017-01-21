//
//  2bpfvline.cpp
//  bpf2vline
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

class Bpf2Vline : public BaseObject
{
    vector<AtomList> vline;
    
    SymbolEnumProperty* method_;
    
    
    
public:
    Bpf2Vline(const PdArgs& a)
    : BaseObject(a)
    
    
    {
        createOutlet();
        
        initProperties();
        // parse creation arguments and properties
        parseArguments();
    }
    
    void onBang()
    {
        for (int i = 0; i< this->vline.size(); i++)
        {
            listTo(0, this->vline.at(i));
        }
        
    }
    
    void onList(const AtomList& l)
    {
        BPF func;
        func.setBPFList(l);
        this->vline = func.getVline();
        
        onBang();
    }
    
    void initProperties()
    {
        
    }
    
};

extern "C" void setup_conv0x2ebpf2vline()
{
    ObjectFactory<Bpf2Vline> obj("conv.bpf2vline");
}