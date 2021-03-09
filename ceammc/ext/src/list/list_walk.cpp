#include "list_walk.h"
#include "ceammc_property_enum.h"

#include <cstdlib>

using namespace ceammc;
using namespace ceammc::list;

static t_symbol* SYM_SINGLE;
static t_symbol* SYM_CLIP;
static t_symbol* SYM_WRAP;
static t_symbol* SYM_FOLD;

ListWalk::ListWalk(const PdArgs& a)
    : BaseObject(a)
    , lst_(nullptr)
    , walk_mode_(nullptr)
    , current_pos_(0)
    , length_(1)
    , forward_(true)
    , single_done_(false)
{
    createOutlet();
    createOutlet();

    addProperty(new PointerProperty<bool>("@direction", &forward_, PropValueAccess::READWRITE));
    addProperty(new PointerProperty<int>("@length", &length_, PropValueAccess::READWRITE));

    walk_mode_ = new SymbolEnumProperty("@mode", { SYM_SINGLE, SYM_WRAP, SYM_CLIP, SYM_FOLD });
    addProperty(walk_mode_);

    lst_ = new ListProperty("@value");
    lst_->setArgIndex(0);
    addProperty(lst_);

    // aliases
    addProperty(new SymbolEnumAlias("@single", walk_mode_, SYM_SINGLE));
    addProperty(new SymbolEnumAlias("@loop", walk_mode_, SYM_WRAP));
    addProperty(new SymbolEnumAlias("@wrap", walk_mode_, SYM_WRAP));
    addProperty(new SymbolEnumAlias("@clip", walk_mode_, SYM_CLIP));
    addProperty(new SymbolEnumAlias("@fold", walk_mode_, SYM_FOLD));

    createCbIntProperty(
        "@size",
        [this]() -> int { return lst_->value().size(); })
        ->setIntCheck(PropValueConstraints::GREATER_EQUAL, 0);

    createCbProperty("@index", &ListWalk::p_index, &ListWalk::p_set_index);
}

void ListWalk::onBang() { onFloat(1); }

void ListWalk::onFloat(t_float v)
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
    lst_->set(l);
    current_pos_ = 0;
    single_done_ = false;
}

void ListWalk::m_current(t_symbol*, const AtomListView&) { current(); }
void ListWalk::m_next(t_symbol*, const AtomListView& l) { next(l.toT<int>(1)); }
void ListWalk::m_prev(t_symbol*, const AtomListView& l) { prev(l.toT<int>(1)); }

void ListWalk::m_reset(t_symbol*, const AtomListView&)
{
    current_pos_ = 0;
    single_done_ = false;
}

AtomList ListWalk::p_index() const
{
    if (walk_mode_->value() != SYM_FOLD)
        return AtomList(current_pos_);
    else {
        size_t idx = 0;
        list::calcFoldIndex(current_pos_, lst_->value().size(), &idx);
        return AtomList(idx);
    }
}

void ListWalk::p_set_index(const AtomList& l)
{
    int idx = atomlistToValue<int>(l, 0);
    if (idx < 0)
        idx += lst_->value().size();

    current_pos_ = std::max(0, std::min<int>(idx, lst_->value().size() - 1));
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
    if (lst_->value().empty()) // error message in current()
        return;

    size_t idx = 0;

    if (walk_mode_->value() == SYM_SINGLE) {
        if (pos < 0) {
            current_pos_ = 0;
            single_done_ = true;
        } else if (pos < lst_->value().size()) {
            current_pos_ = pos;
            single_done_ = false;
        } else {
            current_pos_ = lst_->value().size() - 1;
            single_done_ = true;
        }
    } else if (walk_mode_->value() == SYM_WRAP) {
        if (calcWrapIndex(pos, lst_->value().size(), &idx))
            current_pos_ = idx;
    } else if (walk_mode_->value() == SYM_CLIP) {
        if (calcClipIndex(pos, lst_->value().size(), &idx))
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

    const auto& l = lst_->value();

    if (l.empty()) {
        OBJ_ERR << "empty list";
        return;
    }

    //! single
    if (walk_mode_->value() == SYM_SINGLE) {
        if (single_done_)
            return;

        listTo(0, l.slice(current_pos_, current_pos_ + length_ - 1));
    }
    //! clip
    else if (walk_mode_->value() == SYM_CLIP) {
        if (length_ == 1)
            atomTo(0, *l.clipAt(current_pos_));
        else
            listTo(0, list::sliceClip(l, current_pos_, length_));
    }
    //! wrap/loop
    else if (walk_mode_->value() == SYM_WRAP) {
        if (length_ == 1)
            atomTo(0, *l.wrapAt(current_pos_));
        else
            listTo(0, list::sliceWrap(l, current_pos_, length_));
    }
    //! fold
    else if (walk_mode_->value() == SYM_FOLD) {
        if (length_ == 1)
            atomTo(0, *l.foldAt(current_pos_));
        else
            listTo(0, list::sliceFold(l, current_pos_, length_));
    }

    // last element
    if ((current_pos_ + 1) == l.size())
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

    obj.setDescription("Walks thru the list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "walk", "iterate" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListWalk::setInletsInfo(obj.classPointer(), { "bang:    output current element, then move to next position\n"
                                                  "float N: output current element, then move N-steps forward/back\n"
                                                  "list:    set list, reset position, no output\n"
                                                  "current: output current element\n"
                                                  "next N:  move to next N, then output\n"
                                                  "prev N:  move to prev N, then output\n"
                                                  "reset:   reset position, no output" });
    ListWalk::setOutletsInfo(obj.classPointer(), { "atom", "bang: when last element reached" });
}
