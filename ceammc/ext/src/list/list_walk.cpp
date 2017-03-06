#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListWalk : public BaseObject {
    AtomList lst_;
    SymbolEnumProperty* walk_mode_;
    t_symbol* m_single_;
    t_symbol* m_clip_;
    t_symbol* m_wrap_;
    t_symbol* m_fold_;
    int current_pos_;
    bool forward_;

public:
    ListWalk(const PdArgs& a)
        : BaseObject(a)
        , walk_mode_(0)
        , m_single_(gensym("single"))
        , m_clip_(gensym("clip"))
        , m_wrap_(gensym("wrap"))
        , m_fold_(gensym("fold"))
        , current_pos_(0)
        , forward_(true)
    {
        createOutlet();

        createProperty(new PointerProperty<bool>("@direction", &forward_, false));
        createProperty(new PointerProperty<int>("@index", &current_pos_, false));
        createProperty(new PointerProperty<AtomList>("@value", &lst_));
        walk_mode_ = new SymbolEnumProperty("@mode", "single");
        walk_mode_->appendEnum("wrap");
        walk_mode_->appendEnum("clip");
        walk_mode_->appendEnum("fold");
        createProperty(walk_mode_);

        // aliases
        createProperty(new SymbolEnumAlias("@single", walk_mode_, gensym("single")));
        createProperty(new SymbolEnumAlias("@loop", walk_mode_, gensym("wrap")));
        createProperty(new SymbolEnumAlias("@wrap", walk_mode_, gensym("wrap")));
        createProperty(new SymbolEnumAlias("@clip", walk_mode_, gensym("clip")));
        createProperty(new SymbolEnumAlias("@fold", walk_mode_, gensym("fold")));

        parseArguments();
        lst_ = args();
    }

    void onBang() { next(); }

    void onList(const AtomList& l)
    {
        lst_ = l;
        current_pos_ = 0;
    }

    void m_next(t_symbol*, const AtomList& l) { next(atomlistToValue<int>(l, 1)); }
    void m_prev(t_symbol*, const AtomList& l) { prev(atomlistToValue<int>(l, 1)); }
    void m_reset(t_symbol*, const AtomList& l) { current_pos_ = 0; }

private:
    void next(int step = 1)
    {
        if (forward_) {
            go_to(current_pos_);
            current_pos_ += step;
        } else {
            go_to(current_pos_);
            current_pos_ -= step;
        }
    }

    void prev(int step = 1)
    {
        if (forward_) {
            go_to(current_pos_);
            current_pos_ -= step;
        } else {
            go_to(current_pos_);
            current_pos_ += step;
        }
    }

    void go_to(int pos)
    {
        Atom* a = 0;

        if (walk_mode_->value() == m_single_) {
            if (pos < 0 || pos >= lst_.size())
                return;

            a = &lst_[pos];
        }

        else if (walk_mode_->value() == m_clip_) {
            a = lst_.clipAt(pos);
        }

        else if (walk_mode_->value() == m_wrap_) {
            a = lst_.wrapAt(pos);
        }

        else if (walk_mode_->value() == m_fold_) {
            a = lst_.foldAt(pos);
        }

        if (a != 0)
            atomTo(0, *a);
    }
};

extern "C" void setup_list0x2ewalk()
{
    ObjectFactory<ListWalk> obj("list.walk");
    obj.addMethod("next", &ListWalk::m_next);
    obj.addMethod("prev", &ListWalk::m_prev);
    obj.addMethod("reset", &ListWalk::m_reset);
}
