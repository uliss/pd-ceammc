#include "ceammc_factory.h"
#include "global_base.h"

using namespace ceammc;

class GlobalFloat : public GlobalBase<float> {
public:
    GlobalFloat(const PdArgs& a)
        : GlobalBase<float>(a, "global.float")
    {
    }

    void onBang()
    {
        floatTo(0, ref());
    }

    void onFloat(float v)
    {
        ref() = v;
        onBang();
    }

    void m_set(t_symbol* /*s*/, const AtomList& lst)
    {
        if (lst.size() > 0 && lst.at(0).isFloat()) {
            ref() = lst.at(0).asFloat();
        } else {
            OBJ_ERR << "invalid arguments given: " << lst;
        }
    }
};

extern "C" void setup_global0x2efloat()
{
    ObjectFactory<GlobalFloat> obj("global.float");
    obj.addMethod("set", &GlobalFloat::m_set);
}
