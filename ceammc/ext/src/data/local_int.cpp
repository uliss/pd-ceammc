#include "ceammc_factory.h"
#include "local_base.h"

using namespace ceammc;

class LocalInt : public LocalBase<int> {
public:
    LocalInt(const PdArgs& a)
        : LocalBase<int>(a, "local.int")
    {
    }

    void onBang()
    {
        floatTo(0, ref());
    }

    void onFloat(float v)
    {
        ref() = static_cast<int>(v);
        onBang();
    }

    void m_set(t_symbol* /*s*/, const AtomList& lst)
    {
        if (lst.size() > 0 && lst.at(0).isFloat()) {
            ref() = static_cast<int>(lst.at(0).asFloat());
        } else {
            OBJ_ERR << "invalid arguments given: " << lst;
        }
    }
};

extern "C" void setup_local0x2eint()
{
    ObjectFactory<LocalInt> obj("local.int");
    obj.addMethod("set", &LocalInt::m_set);
}
