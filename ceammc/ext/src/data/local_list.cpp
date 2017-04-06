#include "ceammc_factory.h"
#include "local_base.h"

using namespace ceammc;

class LocalList : public LocalBase<AtomList> {
public:
    LocalList(const PdArgs& a)
        : LocalBase<AtomList>(a, "local.list")
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

extern "C" void setup_local0x2elist()
{
    ObjectFactory<LocalList> obj("local.list");
    obj.addMethod("set", &LocalList::m_set);
}
