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

class Bpf2Env : public BaseObject {
    AtomList out_list_;

    SymbolEnumProperty* method_;
    SymbolEnumProperty* type_;

    t_symbol* gadsr_;
    t_symbol* gahr_;
    t_symbol* gar_;

    //Atom type_;

public:
    Bpf2Env(const PdArgs& a)
        : BaseObject(a)
        , type_(0)

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
        const t_symbol* t = type_->value();

        if ((t == gadsr_) || (t == gahr_)) {
            if (l.size() > 6) {
                out_list_.clear();

                Atom v3 = l[2];
                Atom v5 = l[4];

                out_list_.append(v3);
                out_list_.append(Atom(v5.asFloat() - v3.asFloat()));
                out_list_.append(l[5]);
                out_list_.append(Atom(l[6].asFloat() - v5.asFloat()));
            }
        } else if (t == gar_) {

            if (l.size() > 3) {
                out_list_.clear();

                out_list_.append(l[1]);
                out_list_.append(l[3]);
            }
        }

        onBang();
    }

    void initProperties()
    {
        // envelope types:
        // @adsr, @ahr, @ar
        type_ = new SymbolEnumProperty("@type", "adsr");
        type_->appendEnum("ahr");
        type_->appendEnum("ar");
        createProperty(type_);

        gadsr_ = gensym("adsr");
        gahr_ = gensym("ahr");
        gar_ = gensym("ar");
    }
};

extern "C" void setup_conv0x2ebpf2env()
{
    ObjectFactory<Bpf2Env> obj("conv.bpf2env");
}
