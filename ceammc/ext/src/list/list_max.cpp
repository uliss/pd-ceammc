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

    type_ = new SymbolEnumProperty("@type", "float");
    type_->appendEnum("symbol");
    type_->appendEnum("any");
    createProperty(type_);

    createProperty(new SymbolEnumAlias("@float", type_, SYM_FLOAT));
    createProperty(new SymbolEnumAlias("@symbol", type_, SYM_SYMBOL));
    createProperty(new SymbolEnumAlias("@any", type_, SYM_ANY));
}

void ListMax::onList(const AtomList& l)
{
    if (l.empty())
        return;

    const Atom* max = 0;

    if (type_->value() == SYM_ANY)
        max = l.max();
    else if (type_->value() == SYM_FLOAT)
        max = l.filtered(isFloat).max();
    else if (type_->value() == SYM_SYMBOL)
        max = l.filtered(isSymbol).max();

    if (max != 0)
        atomTo(0, *max);
}

static bool isComparable(const Atom& a) { return a.isFloat() || a.isSymbol(); }

void ListMax::onDataT(const DataTypeMList& lst)
{
    onList(lst.toList().filtered(isComparable));
}

void setup_list_max()
{
    ObjectFactory<ListMax> obj("list.max");
    obj.mapFloatToList();
    obj.mapSymbolToList();

    obj.processData<DataTypeMList>();
}
