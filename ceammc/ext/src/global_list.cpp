#include "ceammc_factory.h"
#include "global_base.h"

using namespace ceammc;

class GlobalList : public GlobalBase<AtomList> {
public:
    GlobalList(const PdArgs& a)
        : GlobalBase<AtomList>(a, "global.list")
    {
    }

    void onBang()
    {
        listTo(0, ref());
    }

    void onList(const AtomList& l)
    {
        ref() = l;
        onBang();
    }

    void m_set(t_symbol* /*s*/, const AtomList& lst)
    {
        ref() = lst;
    }
};

extern "C" void setup_global0x2elist()
{
    ObjectFactory<GlobalList> obj("global.list");
    obj.addMethod("set", &GlobalList::m_set);
}
