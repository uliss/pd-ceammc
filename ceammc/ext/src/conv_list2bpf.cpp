//
//  2listbpf.cpp
//  list2bpf
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

class List2Bpf : public BaseObject
{
    AtomList out_list_;
    
    SymbolEnumProperty* method_;
    Atom size_;
    
    
public:
    List2Bpf(const PdArgs& a)
    : BaseObject(a)
    , size_(4)
    
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

extern "C" void setup_conv0x2elist2bpf()
{
    ObjectFactory<List2Bpf> obj("conv.list2bpf");
}