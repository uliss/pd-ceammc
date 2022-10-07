#include "list_walk.h"
#include "ceammc_crc32.h"
#include "ceammc_property_enum.h"

#include <cstdlib>

using namespace ceammc;
using namespace ceammc::list;

CEAMMC_DEFINE_SYM_HASH(single);
CEAMMC_DEFINE_SYM_HASH(clip);
CEAMMC_DEFINE_SYM_HASH(wrap);
CEAMMC_DEFINE_SYM_HASH(fold);

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

    walk_mode_ = new SymbolEnumProperty("@mode", { sym_single(), sym_wrap(), sym_clip(), sym_fold() });
    addProperty(walk_mode_);

    lst_ = new ListProperty("@value");
    lst_->setArgIndex(0);
    addProperty(lst_);

    // aliases
    addProperty(new SymbolEnumAlias("@single", walk_mode_, sym_single()));
    addProperty(new SymbolEnumAlias("@loop", walk_mode_, sym_wrap()));
    addProperty(new SymbolEnumAlias("@wrap", walk_mode_, sym_wrap()));
    addProperty(new SymbolEnumAlias("@clip", walk_mode_, sym_clip()));
    addProperty(new SymbolEnumAlias("@fold", walk_mode_, sym_fold()));

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

void ListWalk::onList(const AtomListView& lv)
{
    lst_->set(lv);
    current_pos_ = 0;
    single_done_ = false;
}

void ListWalk::m_current(t_symbol*, const AtomListView&) { current(); }
void ListWalk::m_next(t_symbol*, const AtomListView& lv) { next(lv.toT<int>(1)); }
void ListWalk::m_prev(t_symbol*, const AtomListView& lv) { prev(lv.toT<int>(1)); }

void ListWalk::m_reset(t_symbol*, const AtomListView&)
{
    current_pos_ = 0;
    single_done_ = false;
}

AtomList ListWalk::p_index() const
{
    if (walk_mode_->value() != sym_fold())
        return AtomList(current_pos_);
    else {
        size_t idx = 0;
        list::calcFoldIndex(current_pos_, lst_->value().size(), &idx);
        return AtomList(idx);
    }
}

void ListWalk::p_set_index(const AtomListView& lv)
{
    int idx = atomlistToValue<int>(lv, 0);
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

    switch (crc32_hash(walk_mode_->value())) {
    case hash_single:
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
        break;
    case hash_wrap:
        if (calcWrapIndex(pos, lst_->value().size(), &idx))
            current_pos_ = idx;

        break;
    case hash_clip:
        if (calcClipIndex(pos, lst_->value().size(), &idx))
            current_pos_ = idx;

        break;
    case hash_fold:
        current_pos_ = pos;
        break;
    default:
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
    switch (crc32_hash(walk_mode_->value())) {
    case hash_single: {
        if (single_done_)
            return;

        listTo(0, l.slice(current_pos_, current_pos_ + length_ - 1));
    } break;
    //! clip
    case hash_clip: {
        if (length_ == 1)
            atomTo(0, *l.clipAt(current_pos_));
        else
            listTo(0, list::sliceClip(l, current_pos_, length_));
    } break;
    //! wrap/loop
    case hash_wrap: {
        if (length_ == 1)
            atomTo(0, *l.wrapAt(current_pos_));
        else
            listTo(0, list::sliceWrap(l, current_pos_, length_));
    } break;
    //! fold
    case hash_fold: {
        if (length_ == 1)
            atomTo(0, *l.foldAt(current_pos_));
        else
            listTo(0, list::sliceFold(l, current_pos_, length_));
    } break;
    default:
        break;
    }

    // last element
    if ((current_pos_ + 1) == l.size())
        bangTo(1);
}

void setup_list_walk()
{
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
