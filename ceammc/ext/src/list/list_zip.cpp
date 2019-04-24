//
//  list_interleave.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 23/12/16.
//

#include "list_zip.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

#include <cassert>
#include <sstream>

using namespace ceammc;

static const size_t MIN_INLET = 2;
static const size_t MAX_INLET = 20;

static t_symbol* SYM_MIN;
static t_symbol* SYM_PAD;
static t_symbol* SYM_CLIP;
static t_symbol* SYM_WRAP;
static t_symbol* SYM_FOLD;

ListZip::ListZip(const PdArgs& a)
    : BaseObject(a)
    , in_count_(clip(atomlistToValue<size_t>(a.args, MIN_INLET), MIN_INLET, MAX_INLET))
    , method_(0)
    , pad_(0.f)
{
    initLists();
    initInlets();
    createOutlet();
    initProperties();
}

void ListZip::onBang()
{
    listTo(0, out_list_);
}

void ListZip::onList(const AtomList& l)
{
    in_list_[0] = l;
    const t_symbol* m = method_->value();

    if (m == SYM_MIN)
        out_list_ = list::interleaveMinLength(in_list_);
    else if (m == SYM_PAD)
        out_list_ = list::interleavePadWith(in_list_, pad_);
    else if (m == SYM_CLIP)
        out_list_ = list::interleaveClip(in_list_);
    else if (m == SYM_WRAP)
        out_list_ = list::interleaveWrap(in_list_);
    else if (m == SYM_FOLD)
        out_list_ = list::interleaveFold(in_list_);

    onBang();
}

void ListZip::onInlet(size_t n, const AtomList& l)
{
    if (n >= in_count_) {
        OBJ_ERR << "invalid inlet index: " << n << ", total inlet count = " << in_count_;
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
    assert(in_count_ > 1);

    for (size_t i = 0; i < in_count_ - 1; i++)
        createInlet();
}

void ListZip::initLists()
{
    in_list_.reserve(in_count_);

    for (size_t i = 0; i < in_count_; i++)
        in_list_.push_back(AtomList());

    for (size_t i = 0; i < in_count_; i++) {
        std::ostringstream s;
        s << "@l" << i;
        PointerProperty<AtomList>* p = new PointerProperty<AtomList>(s.str().c_str(), &in_list_[i], true);
        createProperty(p);
    }
}

void ListZip::initProperties()
{
    // interleave methods:
    // @min - interleave by shortests list (by default)
    // @pad - pad with specified value (@pad_value property), zero by default
    // @wrap - pad with wrapped values
    // @fold - pad with fold values
    method_ = new SymbolEnumProperty("@method", SYM_MIN);
    method_->appendEnum(SYM_PAD);
    method_->appendEnum(SYM_CLIP);
    method_->appendEnum(SYM_WRAP);
    method_->appendEnum(SYM_FOLD);
    createProperty(method_);

    // adding aliases
    createProperty(new SymbolEnumAlias("@min", method_, SYM_MIN));
    createProperty(new SymbolEnumAlias("@clip", method_, SYM_CLIP));
    createProperty(new SymbolEnumAlias("@wrap", method_, SYM_WRAP));
    createProperty(new SymbolEnumAlias("@fold", method_, SYM_FOLD));

    createCbProperty("@pad", &ListZip::getPadValue, &ListZip::setPadValue);
}

AtomList ListZip::getPadValue() const
{
    return listFrom(pad_);
}

void ListZip::setPadValue(const AtomList& l)
{
    pad_ = atomlistToValue<Atom>(l, Atom(0.f));
    method_->setValue(SYM_PAD);
}

void setup_list_zip()
{
    SYM_MIN = gensym("min");
    SYM_PAD = gensym("pad");
    SYM_CLIP = gensym("clip");
    SYM_WRAP = gensym("wrap");
    SYM_FOLD = gensym("fold");

    ObjectFactory<ListZip> obj("list.zip");
    obj.addAlias("list.interleave");
}
