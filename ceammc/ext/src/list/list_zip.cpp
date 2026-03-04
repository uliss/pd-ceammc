//
//  list_interleave.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 23/12/16.
//

#include "list_zip.h"
#include "ceammc_crc32.h"
#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"

using namespace ceammc;

constexpr size_t DEFAULT_INLET = 2;
constexpr size_t MIN_INLET = 2;
constexpr size_t MAX_INLET = 20;

CEAMMC_DEFINE_SYM_HASH(min)
CEAMMC_DEFINE_SYM_HASH(pad)
CEAMMC_DEFINE_SYM_HASH(clip)
CEAMMC_DEFINE_SYM_HASH(wrap)
CEAMMC_DEFINE_SYM_HASH(fold)

ListZip::ListZip(const PdArgs& a)
    : BaseObject(a)
    , n_(nullptr)
    , method_(nullptr)
    , pad_(0.f)
{
    n_ = new IntProperty("@n", DEFAULT_INLET, PropValueAccess::INITONLY);
    n_->checkClosedRange(MIN_INLET, MAX_INLET);
    n_->setArgIndex(0);
    addProperty(n_);

    // interleave methods:
    // @min - interleave by shortests list (by default)
    // @pad - pad with specified value (@pad_value property), zero by default
    // @wrap - pad with wrapped values
    // @fold - pad with fold values
    method_ = new SymbolEnumProperty("@method",
        { str_min, str_pad, str_clip, str_wrap, str_fold });
    addProperty(method_);

    // adding aliases
    addProperty(new SymbolEnumAlias("@min", method_, sym_min()));
    addProperty(new SymbolEnumAlias("@clip", method_, sym_clip()));
    addProperty(new SymbolEnumAlias("@wrap", method_, sym_wrap()));
    addProperty(new SymbolEnumAlias("@fold", method_, sym_fold()));

    createCbAtomProperty(
        "@pad",
        [this]() -> Atom { return pad_; },
        [this](const Atom& a) -> bool {
            pad_ = a;
            method_->setValue(sym_pad());
            return true; });

    createCbListProperty("@lists",
        [this]() -> AtomList {
            AtomList res;
            for (auto& x : in_list_)
                res.append(MListAtom(x));

            return res;
        });
}

void ListZip::initDone()
{
    // we should do all of this after property parsing
    initLists();
    initInlets();
    createOutlet();
}

void ListZip::onBang()
{
    listTo(0, out_list_);
}

void ListZip::onList(const AtomListView& lv)
{
    in_list_[0] = lv;
    const t_symbol* m = method_->value();

    if (m == sym_min())
        out_list_ = list::interleaveMinLength(in_list_);
    else if (m == sym_pad())
        out_list_ = list::interleavePadWith(in_list_, pad_);
    else if (m == sym_clip())
        out_list_ = list::interleaveClip(in_list_);
    else if (m == sym_wrap())
        out_list_ = list::interleaveWrap(in_list_);
    else if (m == sym_fold())
        out_list_ = list::interleaveFold(in_list_);

    onBang();
}

void ListZip::onInlet(size_t n, const AtomListView& l)
{
    if (n >= n_->value()) {
        OBJ_ERR << "invalid inlet index: " << n << ", total inlet count = " << n_;
        return;
    }

    in_list_[n] = l;
}

void ListZip::dump() const
{
    BaseObject::dump();

    OBJ_DBG << "input lists:";
    for (size_t i = 0; i < in_list_.size(); i++) {
        OBJ_DBG << in_list_[i];
    }

    OBJ_DBG << "out list: " << out_list_;
}

void ListZip::initInlets()
{
    for (size_t i = 0; i < n_->value() - 1; i++)
        createInlet();
}

void ListZip::initLists()
{
    in_list_.reserve(n_->value());

    for (size_t i = 0; i < n_->value(); i++)
        in_list_.push_back(AtomList());
}

void setup_list_zip()
{
    ObjectFactory<ListZip> obj("list.zip");
    obj.addAlias("list.interleave");

    obj.setDescription("takes n lists from n inlets (specified by argument) and output their elements sequentially (list1-1 list2-1 list1-2 list2-2 etc.).");
    obj.setCategory("list");
    obj.setKeywords({ "list", "functional" });
}
