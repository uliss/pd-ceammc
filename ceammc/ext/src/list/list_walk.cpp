#include "list_walk.h"

#include <cstdlib>

using namespace ceammc;
using namespace ceammc::list;

static t_symbol* SYM_SINGLE;
static t_symbol* SYM_CLIP;
static t_symbol* SYM_WRAP;
static t_symbol* SYM_FOLD;

ListWalk::ListWalk(const PdArgs& a)
    : BaseObject(a)
    , walk_mode_(nullptr)
    , current_pos_(0)
    , length_(1)
    , forward_(true)
    , single_done_(false)
{
    createOutlet();
    createOutlet();

    createProperty(new PointerProperty<bool>("@direction", &forward_, false));
    createProperty(new PointerProperty<int>("@length", &length_, false));
    createProperty(new PointerProperty<AtomList>("@value", &lst_));

    walk_mode_ = new SymbolEnumProperty("@mode", SYM_SINGLE);
    walk_mode_->appendEnum(SYM_WRAP);
    walk_mode_->appendEnum(SYM_CLIP);
    walk_mode_->appendEnum(SYM_FOLD);
    createProperty(walk_mode_);

    // aliases
    createProperty(new SymbolEnumAlias("@single", walk_mode_, SYM_SINGLE));
    createProperty(new SymbolEnumAlias("@loop", walk_mode_, SYM_WRAP));
    createProperty(new SymbolEnumAlias("@wrap", walk_mode_, SYM_WRAP));
    createProperty(new SymbolEnumAlias("@clip", walk_mode_, SYM_CLIP));
    createProperty(new SymbolEnumAlias("@fold", walk_mode_, SYM_FOLD));

    createCbProperty("@size", &ListWalk::p_size);
    createCbProperty("@index", &ListWalk::p_index, &ListWalk::p_set_index);
    property("@index")->info().setType(PropertyInfoType::INTEGER);

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
    if (walk_mode_->value() != SYM_FOLD)
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

    if (walk_mode_->value() == SYM_SINGLE) {
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
    } else if (walk_mode_->value() == SYM_WRAP) {
        if (calcWrapIndex(pos, lst_.size(), &idx))
            current_pos_ = idx;
    } else if (walk_mode_->value() == SYM_CLIP) {
        if (calcClipIndex(pos, lst_.size(), &idx))
            current_pos_ = idx;
    } else if (walk_mode_->value() == SYM_FOLD) {
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
    if (walk_mode_->value() == SYM_SINGLE) {
        if (single_done_)
            return;

        listTo(0, lst_.slice(current_pos_, current_pos_ + length_ - 1));
    }
    //! clip
    else if (walk_mode_->value() == SYM_CLIP) {
        listTo(0, list::sliceClip(lst_, current_pos_, length_));
    }
    //! wrap/loop
    else if (walk_mode_->value() == SYM_WRAP) {
        listTo(0, list::sliceWrap(lst_, current_pos_, length_));
    }
    //! fold
    else if (walk_mode_->value() == SYM_FOLD) {
        listTo(0, list::sliceFold(lst_, current_pos_, length_));
    }

    // last element
    if ((current_pos_ + 1) == lst_.size())
        bangTo(1);
}

void setup_list_walk()
{
    SYM_SINGLE = gensym("single");
    SYM_CLIP = gensym("clip");
    SYM_WRAP = gensym("wrap");
    SYM_FOLD = gensym("fold");

    ObjectFactory<ListWalk> obj("list.walk");
    obj.addMethod("current", &ListWalk::m_current);
    obj.addMethod("next", &ListWalk::m_next);
    obj.addMethod("prev", &ListWalk::m_prev);
    obj.addMethod("reset", &ListWalk::m_reset);
}
