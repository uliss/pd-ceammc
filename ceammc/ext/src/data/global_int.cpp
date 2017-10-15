#include "ceammc_factory.h"
#include "global_base.h"

using namespace ceammc;

class GlobalInt : public GlobalBase<int> {
public:
    GlobalInt(const PdArgs& a)
        : GlobalBase<int>(a, "global.int")
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

extern "C" void setup_global0x2eint()
{
    ObjectFactory<GlobalInt> obj("global.int");
    obj.addMethod("set", &GlobalInt::m_set);
}
