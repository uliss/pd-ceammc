#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListWalk : public BaseObject {
    AtomList lst_;
    SymbolEnumProperty* order_;
    t_symbol* m_single_;
    t_symbol* m_clip_;
    t_symbol* m_wrap_;
    t_symbol* m_fold_;
    int current_pos_;
    bool forward_;

public:
    ListWalk(const PdArgs& a)
        : BaseObject(a)
        , order_(0)
        , m_single_(gensym("single"))
        , m_clip_(gensym("clip"))
        , m_wrap_(gensym("wrap"))
        , m_fold_(gensym("fold"))
        , current_pos_(0)
        , forward_(true)
    {
        createOutlet();

        createProperty(new PointerProperty<bool>("@direction", &forward_, false));
        createProperty(new PointerProperty<int>("@current_pos", &current_pos_, false));
        createProperty(new PointerProperty<AtomList>("@current_list", &lst_));
        order_ = new SymbolEnumProperty("@pass", "single");
        order_->appendEnum("wrap");
        order_->appendEnum("clip");
        order_->appendEnum("fold");
        createProperty(order_);

        // aliases
        createProperty(new SymbolEnumAlias("@single", order_, gensym("single")));
        createProperty(new SymbolEnumAlias("@loop", order_, gensym("wrap")));
        createProperty(new SymbolEnumAlias("@wrap", order_, gensym("wrap")));
        createProperty(new SymbolEnumAlias("@clip", order_, gensym("clip")));
        createProperty(new SymbolEnumAlias("@fold", order_, gensym("fold")));

        parseArguments();
        lst_ = args();
    }

    void onBang()
    {
        m_next();
    }

    void onList(const AtomList& l)
    {
        lst_ = l;
        current_pos_ = 0;
    }

    void m_next()
    {
        if (forward_)
            go_to(current_pos_ + 1);
        else
            go_to(current_pos_ - 1);
    }

private:
    void go_to(int pos)
    {
        Atom* a = 0;

        if (order_->value() == m_single_) {
            if (pos < 0 || pos >= lst_.size())
                return;

            a = &lst_[pos];
        }

        else if (order_->value() == m_clip_) {
            a = lst_.clipAt(pos);
        }

        else if (order_->value() == m_wrap_) {
            a = lst_.wrapAt(pos);
        }

        else if (order_->value() == m_fold_) {
            a = lst_.foldAt(pos);
        }

        if (a != 0) {
            atomTo(0, *a);
            current_pos_ = pos;
        }
    }
};

extern "C" void setup_list0x2ewalk()
{
    ObjectFactory<ListWalk> obj("list.walk");
}
