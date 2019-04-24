//
//  list_unzip.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 23/12/16.
//
//

#include "list_unzip.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "datatype_mlist.h"

using namespace ceammc;

static const size_t MIN_OUTLET = 2;
static const size_t MAX_OUTLET = 20;

static t_symbol* SYM_MIN;
static t_symbol* SYM_PAD;

ListUnzip::ListUnzip(const PdArgs& a)
    : ListBase(a)
    , out_count_(clip(atomlistToValue<size_t>(a.args, MIN_OUTLET), MIN_OUTLET, MAX_OUTLET))
    , pad_(0.f)
    , method_(0)
{
    initOutlets();
    initProperties();
}

void ListUnzip::onBang()
{
    for (size_t i = out_lists_.size(); i > 0; i--)
        listTo(i - 1, out_lists_[i - 1]);
}

void ListUnzip::onList(const AtomList& l)
{
    clearOutputList();

    if (method_->value() == SYM_MIN)
        list::deinterleaveMinLength(l, out_lists_);
    else if (method_->value() == SYM_PAD)
        list::deinterleavePadWith(l, pad_, out_lists_);

    onBang();
}

void ListUnzip::clearOutputList()
{
    for (size_t i = 0; i < out_lists_.size(); i++)
        out_lists_[i].clear();
}

void ListUnzip::initOutlets()
{
    for (size_t i = 0; i < out_count_; i++) {
        out_lists_.push_back(AtomList());
        createOutlet();
    }
}

void ListUnzip::initProperties()
{
    method_ = new SymbolEnumProperty("@method", SYM_MIN);
    method_->appendEnum(SYM_PAD);
    createProperty(method_);
    createProperty(new SymbolEnumAlias("@min", method_, SYM_MIN));
    createCbProperty("@pad", &ListUnzip::getPadValue, &ListUnzip::setPadValue);
}

AtomList ListUnzip::getPadValue() const
{
    return listFrom(pad_);
}

void ListUnzip::setPadValue(const AtomList& l)
{
    pad_ = atomlistToValue<Atom>(l, Atom(0.f));
    method_->setValue(SYM_PAD);
}

void setup_list_unzip()
{
    SYM_MIN = gensym("min");
    SYM_PAD = gensym("pad");

    ObjectFactory<ListUnzip> obj("list.unzip");
    obj.addAlias("list.deinterleave");
    obj.processData<DataTypeMList>();
}
