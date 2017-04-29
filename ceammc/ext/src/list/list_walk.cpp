#include "list_walk.h"

#include <cstdlib>

using namespace ceammc;
using namespace ceammc::list;

ListWalk::ListWalk(const PdArgs& a)
    : BaseObject(a)
    , walk_mode_(0)
    , m_single_(gensym("single"))
    , m_clip_(gensym("clip"))
    , m_wrap_(gensym("wrap"))
    , m_fold_(gensym("fold"))
    , current_pos_(0)
    , length_(1)
    , forward_(true)
    , single_done_(false)
{
    createOutlet();

    createProperty(new PointerProperty<bool>("@direction", &forward_, false));
    createProperty(new PointerProperty<int>("@length", &length_, false));
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

    createCbProperty("@size", &ListWalk::p_size);
    createCbProperty("@index", &ListWalk::p_index, &ListWalk::p_set_index);

    lst_ = positionalArguments();
}

void ListWalk::onBang() { onFloat(1); }

void ListWalk::onFloat(float v)
{
    int step = static_cast<int>(v);

    current();
    if (step == 0)
        return;

    if (forward_)
        toPosition(current_pos_ + step);
    else
        toPosition(current_pos_ - step);
}

void ListWalk::onList(const AtomList& l)
{
    lst_ = l;
    current_pos_ = 0;
    single_done_ = false;
}

void ListWalk::m_current(t_symbol*, const AtomList&) { current(); }
void ListWalk::m_next(t_symbol*, const AtomList& l) { next(atomlistToValue<int>(l, 1)); }
void ListWalk::m_prev(t_symbol*, const AtomList& l) { prev(atomlistToValue<int>(l, 1)); }

void ListWalk::m_reset(t_symbol*, const AtomList&)
{
    current_pos_ = 0;
    single_done_ = false;
}

AtomList ListWalk::p_size() const { return AtomList(lst_.size()); }

AtomList ListWalk::p_index() const
{
    if (walk_mode_->value() != m_fold_)
        return AtomList(current_pos_);
    else {
        size_t idx = 0;
        list::calcFoldIndex(current_pos_, lst_.size(), &idx);
        return AtomList(idx);
    }
}

void ListWalk::p_set_index(const AtomList& l)
{
    int idx = atomlistToValue<int>(l, 0);
    if (idx < 0)
        idx += lst_.size();

    current_pos_ = std::max(0, std::min<int>(idx, lst_.size() - 1));
}

void ListWalk::next(int step)
{
    if (forward_) {
        toPosition(current_pos_ + step);
        current();
    } else {
        toPosition(current_pos_ - step);
        current();
    }
}

void ListWalk::prev(int step)
{
    if (forward_) {
        toPosition(current_pos_ - step);
        current();
    } else {
        toPosition(current_pos_ + step);
        current();
    }
}

void ListWalk::toPosition(int pos)
{
    if (lst_.empty()) // error message in current()
        return;

    size_t idx = 0;

    if (walk_mode_->value() == m_single_) {
        if (pos < 0) {
            current_pos_ = 0;
            single_done_ = true;
        } else if (pos < lst_.size()) {
            current_pos_ = pos;
            single_done_ = false;
        } else {
            current_pos_ = lst_.size() - 1;
            single_done_ = true;
        }
    } else if (walk_mode_->value() == m_wrap_) {
        if (calcWrapIndex(pos, lst_.size(), &idx))
            current_pos_ = idx;
    } else if (walk_mode_->value() == m_clip_) {
        if (calcClipIndex(pos, lst_.size(), &idx))
            current_pos_ = idx;
    } else if (walk_mode_->value() == m_fold_) {
        current_pos_ = pos;
    } else {
        OBJ_ERR << "unsupported list mode: " << walk_mode_->value()->s_name;
    }
}

void ListWalk::current()
{
    if (length_ < 1) {
        OBJ_ERR << "output list length should be greater then zero: " << length_;
        return;
    }

    if (lst_.empty()) {
        OBJ_ERR << "empty list";
        return;
    }

    //! single
    if (walk_mode_->value() == m_single_) {
        if (single_done_)
            return;

        listTo(0, lst_.slice(current_pos_, current_pos_ + length_));
    }
    //! clip
    else if (walk_mode_->value() == m_clip_) {
        listTo(0, list::sliceClip(lst_, current_pos_, length_));
    }
    //! wrap/loop
    else if (walk_mode_->value() == m_wrap_) {
        listTo(0, list::sliceWrap(lst_, current_pos_, length_));
    }
    //! fold
    else if (walk_mode_->value() == m_fold_) {
        listTo(0, list::sliceFold(lst_, current_pos_, length_));
    }
}

extern "C" void setup_list0x2ewalk()
{
    ObjectFactory<ListWalk> obj("list.walk");
    obj.addMethod("current", &ListWalk::m_current);
    obj.addMethod("next", &ListWalk::m_next);
    obj.addMethod("prev", &ListWalk::m_prev);
    obj.addMethod("reset", &ListWalk::m_reset);
}
