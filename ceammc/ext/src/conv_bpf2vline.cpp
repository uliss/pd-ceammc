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

using namespace ceammc;

class Bpf2Vline : public BaseObject
{
    AtomList out_list_;
    
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

extern "C" void setup_conv0x2ebpf2vline()
{
    ObjectFactory<Bpf2Vline> obj("conv.bpf2vline");
}