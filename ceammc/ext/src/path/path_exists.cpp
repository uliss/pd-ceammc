#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_platform.h"

using namespace ceammc;

static Atom exists(const Atom& a)
{
    if (!a.isSymbol())
        return a;

    return Atom(platform::path_exists(a.asSymbol()->s_name) ? 1.f : 0.f);
}

class PathExists : public BaseObject {
public:
    PathExists(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onSymbol(t_symbol* s)
    {
        floatTo(0, platform::path_exists(s->s_name) ? 1 : 0);
    }

    void onList(const AtomList& l)
    {
        listTo(0, l.map(exists));
    }
};

extern "C" void setup_path0x2eexists()
{
    ObjectFactory<PathExists> obj("path.exists");
}
