#include "list_max.h"
#include "../data/datatype_mlist.h"
#include "ceammc_factory.h"

static t_symbol* SYM_FLOAT = gensym("float");
static t_symbol* SYM_SYMBOL = gensym("symbol");
static t_symbol* SYM_ANY = gensym("any");

ListMax::ListMax(const PdArgs& a)
    : BaseObject(a)
    , type_(0)
{
    createOutlet();

    type_ = new SymbolEnumProperty("@type", SYM_FLOAT);
    type_->appendEnum(SYM_SYMBOL);
    type_->appendEnum(SYM_ANY);
    createProperty(type_);

    createProperty(new SymbolEnumAlias("@float", type_, SYM_FLOAT));
    createProperty(new SymbolEnumAlias("@symbol", type_, SYM_SYMBOL));
    createProperty(new SymbolEnumAlias("@any", type_, SYM_ANY));
}

void ListMax::onList(const AtomList& l)
{
    if (type_->value() == SYM_ANY)
        max(l.begin(), l.end());
    else if (type_->value() == SYM_FLOAT)
        max(l.beginFilter(isFloat), l.endFilter());
    else if (type_->value() == SYM_SYMBOL)
        max(l.beginFilter(isSymbol), l.endFilter());
}

// predicates
static bool is_float(const DataAtom& d)
{
    return d.isAtom() && d.toAtom().isFloat();
}

static bool is_symbol(const DataAtom& d)
{
    return d.isAtom() && d.toAtom().isSymbol();
}

static bool is_atom(const DataAtom& d)
{
    return d.isAtom();
}

void ListMax::onDataT(const DataTypeMList& lst)
{
    if (type_->value() == SYM_ANY)
        maxData(lst.begin_filter(is_atom), lst.end_filter());
    else if (type_->value() == SYM_FLOAT)
        maxData(lst.begin_filter(is_float), lst.end_filter());
    else if (type_->value() == SYM_SYMBOL)
        maxData(lst.begin_filter(is_symbol), lst.end_filter());
}

void setup_list_max()
{
    ObjectFactory<ListMax> obj("list.max");
    obj.mapFloatToList();
    obj.mapSymbolToList();

    obj.processData<DataTypeMList>();
}
