#include "is_data.h"
#include "ceammc_factory.h"

IsData::IsData(const PdArgs& args)
    : BaseObject(args)
    , in_list_(nullptr)
{
    in_list_ = new FlagProperty("@in_list");
    addProperty(in_list_);

    createOutlet();
    createOutlet();
}

void IsData::onBang()
{
    boolTo(0, false);
}

void IsData::onSymbol(t_symbol* s)
{
    boolTo(0, false);
}

void IsData::onList(const AtomList& lst)
{
    if (in_list_->value() && lst.findPos(isData) >= 0) {
        listTo(1, lst);
        boolTo(0, true);
    } else
        boolTo(0, false);
}

void IsData::onAny(t_symbol* s, const AtomListView&)
{
    boolTo(0, false);
}

void IsData::onData(const Atom& d)
{
    atomTo(1, d);
    boolTo(0, true);
}

bool IsData::processAnyProps(t_symbol* sel, const AtomListView& lst)
{
    return false;
}

void IsData::onFloat(t_float f)
{
    boolTo(0, false);
}

void setup_is_data()
{
    ObjectFactory<IsData> obj("is_data");
    obj.processData();

    obj.setDescription("checks if input data has *data* type");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({"predicate", "data"});
    obj.setCategory("predicates");
    obj.setSinceVersion(0, 6);
}
