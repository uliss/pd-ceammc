#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListMin : public BaseObject {
    SymbolEnumProperty* type_;
    t_symbol* alias_float_;
    t_symbol* alias_symbol_;
    t_symbol* alias_any_;

public:
    ListMin(const PdArgs& a)
        : BaseObject(a)
        , type_(0)
        , alias_float_(gensym("float"))
        , alias_symbol_(gensym("symbol"))
        , alias_any_(gensym("any"))
    {
        createOutlet();

        type_ = new SymbolEnumProperty("@type", "float");
        type_->appendEnum("symbol");
        type_->appendEnum("any");
        createProperty(type_);

        createProperty(new SymbolEnumAlias("@float", type_, alias_float_));
        createProperty(new SymbolEnumAlias("@symbol", type_, alias_symbol_));
        createProperty(new SymbolEnumAlias("@any", type_, alias_any_));
    }

    void onList(const AtomList& l)
    {
        if (l.empty())
            return;

        const Atom* min = 0;

        if (type_->value() == alias_any_)
            min = l.min();
        else if (type_->value() == alias_float_)
            min = l.filtered(isFloat).min();
        else if (type_->value() == alias_symbol_)
            min = l.filtered(isSymbol).min();

        if (min != 0)
            atomTo(0, *min);
    }
};

extern "C" void setup_list0x2emin()
{
    ObjectFactory<ListMin> obj("list.min");
    obj.mapFloatToList();
    obj.mapSymbolToList();
}
