#include "list_min.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

static t_symbol* SYM_FLOAT;
static t_symbol* SYM_SYMBOL;
static t_symbol* SYM_ANY;

ListMin::ListMin(const PdArgs& a)
    : BaseObject(a)
    , type_(nullptr)
{
    createOutlet();

    type_ = new SymbolEnumProperty("@type", { SYM_FLOAT, SYM_SYMBOL, SYM_ANY });
    addProperty(type_);

    addProperty(new SymbolEnumAlias("@float", type_, SYM_FLOAT));
    addProperty(new SymbolEnumAlias("@symbol", type_, SYM_SYMBOL));
    addProperty(new SymbolEnumAlias("@any", type_, SYM_ANY));
}

void ListMin::onList(const AtomList& l)
{
    if (type_->value() == SYM_ANY)
        min(l.begin(), l.end());
    else if (type_->value() == SYM_FLOAT)
        min(l.begin_atom_filter(isFloat), l.end_atom_filter());
    else if (type_->value() == SYM_SYMBOL)
        min(l.begin_atom_filter(isSymbol), l.end_atom_filter());
}

void ListMin::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void setup_list_min()
{
    SYM_FLOAT = gensym("float");
    SYM_SYMBOL = gensym("symbol");
    SYM_ANY = gensym("any");

    ObjectFactory<ListMin> obj("list.min");
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdSymbolFn();

    obj.processData<DataTypeMList>();

    obj.setDescription("returns smallest element in the list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "min", "compare" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListMin::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListMin::setOutletsInfo(obj.classPointer(), { "atom" });
}
