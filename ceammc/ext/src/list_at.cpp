#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListAt : public BaseObject {
    IntProperty* pos_;
    SymbolEnumProperty* at_method_;

public:
    ListAt(const PdArgs& a)
        : BaseObject(a)
    {
        createInlet();
        createOutlet();

        pos_ = new IntProperty("@index", 0);
        createProperty(pos_);

        at_method_ = new SymbolEnumProperty("@method", "rel");
        at_method_->appendEnum("clip");
        at_method_->appendEnum("wrap");
        at_method_->appendEnum("fold");
        createProperty(at_method_);

        createProperty(new SymbolEnumAlias("@clip", at_method_, gensym("clip")));
        createProperty(new SymbolEnumAlias("@wrap", at_method_, gensym("wrap")));
        createProperty(new SymbolEnumAlias("@fold", at_method_, gensym("fold")));

        parseArguments();
        pos_->setValue(atomlistToValue<int>(args(), 0));
    }

    void onInlet(size_t idx, const AtomList& l)
    {
        if (idx != 1)
            return;

        pos_->setValue(atomlistToValue<int>(l, 0));
    }

    void onList(const AtomList& l)
    {
        const Atom* a;

        if (at_method_->is("clip"))
            a = l.clipAt(pos_->value());
        else if (at_method_->is("wrap"))
            a = l.wrapAt(pos_->value());
        else if (at_method_->is("fold"))
            a = l.foldAt(pos_->value());
        else
            a = l.relativeAt(pos_->value());

        if (a == 0) {
            ERR << "invalid index value: " << pos_->value();
            return;
        }

        atomTo(0, *a);
    }
};

extern "C" void setup_list0x2eat()
{
    ObjectFactory<ListAt> obj("list.at");
}
