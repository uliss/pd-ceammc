#include "is_data.h"
#include "ceammc_factory.h"

IsData::IsData(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();
}

void IsData::onBang()
{
    floatTo(0, 0);
}

void IsData::onSymbol(t_symbol* s)
{
    floatTo(0, 0);
}

void IsData::onList(const AtomList& lst)
{
    floatTo(0, 0);
}

void IsData::onAny(t_symbol* s, const AtomList& lst)
{
    floatTo(0, 0);
}

void IsData::onData(const DataPtr& d)
{
    dataTo(1, d);
    floatTo(0, 1);
}

void IsData::onFloat(t_float f)
{
    floatTo(0, 0);
}

void setup_is_data()
{
    ObjectFactory<IsData> obj("is_data");
    obj.processData();
}
