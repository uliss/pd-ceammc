#include "ceammc_factory.h"
#include "local_base.h"

using namespace ceammc;

class LocalFloat : public LocalBase<float> {
public:
    LocalFloat(const PdArgs& a)
        : LocalBase<float>(a, "local.float")
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

extern "C" void setup_local0x2efloat()
{
    ObjectFactory<LocalFloat> obj("local.float");
    obj.addMethod("set", &LocalFloat::m_set);
}
