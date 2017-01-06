//
//  2bpfenv.cpp
//  bpf2env
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

class Bpf2Env : public BaseObject
{
    AtomList out_list_;
    
    SymbolEnumProperty* method_;
    Atom type_;
    
    
public:
    Bpf2Env(const PdArgs& a)
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

extern "C" void setup_conv0x2ebpf2env()
{
    ObjectFactory<Bpf2Env> obj("conv.bpf2env");
}
