#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListMax : public BaseObject {
    SymbolEnumProperty* type_;
    t_symbol* alias_float_;
    t_symbol* alias_symbol_;
    t_symbol* alias_any_;

public:
    ListMax(const PdArgs& a)
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

        const Atom* max = 0;

        if (type_->value() == alias_any_)
            max = l.max();
        else if (type_->value() == alias_float_)
            max = l.filtered(isFloat).max();
        else if (type_->value() == alias_symbol_)
            max = l.filtered(isSymbol).max();

        if (max != 0)
            atomTo(0, *max);
    }
};

extern "C" void setup_list0x2emax()
{
    ObjectFactory<ListMax> obj("list.max");
    obj.mapFloatToList();
    obj.mapSymbolToList();
}
