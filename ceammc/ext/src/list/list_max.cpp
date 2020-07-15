#include "list_max.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

static t_symbol* SYM_FLOAT;
static t_symbol* SYM_SYMBOL;
static t_symbol* SYM_ANY;

ListMax::ListMax(const PdArgs& a)
    : BaseObject(a)
    , type_(0)
{
    createOutlet();

    type_ = new SymbolEnumProperty("@type", { SYM_FLOAT, SYM_SYMBOL, SYM_ANY });
    addProperty(type_);

    addProperty(new SymbolEnumAlias("@float", type_, SYM_FLOAT));
    addProperty(new SymbolEnumAlias("@symbol", type_, SYM_SYMBOL));
    addProperty(new SymbolEnumAlias("@any", type_, SYM_ANY));
}

void ListMax::onList(const AtomList& l)
{
    if (type_->value() == SYM_ANY)
        max(l.begin(), l.end());
    else if (type_->value() == SYM_FLOAT)
        max(l.begin_atom_filter(isFloat), l.end_atom_filter());
    else if (type_->value() == SYM_SYMBOL)
        max(l.begin_atom_filter(isSymbol), l.end_atom_filter());
}

void ListMax::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void setup_list_max()
{
    SYM_FLOAT = gensym("float");
    SYM_SYMBOL = gensym("symbol");
    SYM_ANY = gensym("any");

    ObjectFactory<ListMax> obj("list.max");
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdSymbolFn();

    obj.processData<DataTypeMList>();

    obj.setDescription("output largest element in the list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "max", "compare" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListMax::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListMax::setOutletsInfo(obj.classPointer(), { "atom" });
}
