#include "list_min.h"
#include "../data/datatype_mlist.h"
#include "ceammc_factory.h"

static t_symbol* SYM_FLOAT(gensym("float"));
static t_symbol* SYM_SYMBOL(gensym("symbol"));
static t_symbol* SYM_ANY(gensym("any"));

ListMin::ListMin(const PdArgs& a)
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

void ListMin::onList(const AtomList& l)
{
    if (type_->value() == SYM_ANY) {
        auto it = std::min(l.begin(), l.end());
        if (it == l.end())
            return;

        atomTo(0, *it);
    } else if (type_->value() == SYM_FLOAT) {
        auto it = std::min(l.beginFilter(isFloat), l.endFilter());
        if (it == l.endFilter())
            return;

        atomTo(0, *it);
    } else if (type_->value() == SYM_SYMBOL) {
        auto it = std::min(l.beginFilter(isSymbol), l.endFilter());
        if (it == l.endFilter())
            return;

        atomTo(0, *it);
    }
}

void ListMin::onDataT(const DataTypeMList& lst)
{
    auto pred = [](const DataAtom& a) { return a.isAtom() && (a.toAtom().isFloat() || a.toAtom().isSymbol()); };
    onList(lst.toList(pred));
}

void setup_list_min()
{
    ObjectFactory<ListMin> obj("list.min");
    obj.mapFloatToList();
    obj.mapSymbolToList();

    obj.processData<DataTypeMList>();
}
