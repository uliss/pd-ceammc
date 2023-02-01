#include "path_exists.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_platform.h"
#include "datatype_string.h"

using namespace ceammc;

static Atom exists(const Atom& a)
{
    if (!a.isSymbol())
        return a;

    return Atom(platform::path_exists(a.asSymbol()->s_name) ? 1.f : 0.f);
}

PathExists::PathExists(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void PathExists::onSymbol(t_symbol* s)
{
    boolTo(0, platform::path_exists(s->s_name));
}

void PathExists::onList(const AtomListView& lv)
{
    SmallAtomList out;
    lv.map(exists, out);
    listTo(0, out.view());
}

void PathExists::onDataT(const StringAtom& str)
{
    boolTo(0, platform::path_exists(str->str().c_str()));
}

void setup_path_exists()
{
    ObjectFactory<PathExists> obj("path.exists");
    obj.processData<DataTypeString>();
}
