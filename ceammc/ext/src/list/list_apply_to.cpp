#include "list_apply_to.h"
#include "datatype_mlist.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ListApplyTo::ListApplyTo(const ceammc::PdArgs& args)
    : BaseObject(args)
    , cur_idx_(0)
    , on_loop_(false)
{
    createInlet();
    createInlet();
    createOutlet();
    createOutlet();

    onInlet(1, positionalArguments());
}

bool ListApplyTo::processAnyProps(t_symbol* sel, const AtomList& lst)
{
    // no props processing
    return true;
}

void ListApplyTo::onList(const AtomList& lst)
{
    const int N = lst.size();
    mapped_.clear();
    mapped_.reserve(N);
    normalizeIndexes(N);

    on_loop_ = true;

    for (cur_idx_ = 0; cur_idx_ < N; cur_idx_++) {
        // found index to process
        auto it = std::find(norm_idxs_.begin(), norm_idxs_.end(), cur_idx_);

        // no proccessing required
        if (it == norm_idxs_.end())
            mapped_.append(lst[cur_idx_]);
        else
            atomTo(1, lst[cur_idx_]);
    }

    on_loop_ = false;

    listTo(0, mapped_);
}

void ListApplyTo::onInlet(size_t n, const AtomList& lst)
{
    switch (n) {
    case 1:
        return setIndexes(lst);
    case 2:
        return loopProcess(lst);
    default:
        break;
    }
}

void ListApplyTo::onDataT(const DataTPtr<DataTypeMList>& dptr)
{
    const int N = dptr->size();
    mapped_.clear();
    mapped_.reserve(N);
    normalizeIndexes(N);

    on_loop_ = true;

    for (cur_idx_ = 0; cur_idx_ < N; cur_idx_++) {
        // found index to process
        auto it = std::find(norm_idxs_.begin(), norm_idxs_.end(), cur_idx_);

        // no proccessing required
        if (it == norm_idxs_.end())
            mapped_.append((*dptr)[cur_idx_].toAtom());
        else
            atomTo(1, (*dptr)[cur_idx_].toAtom());
    }

    on_loop_ = false;
    dataTo(0, DataTPtr<DataTypeMList>(DataTypeMList(mapped_)));
}

void ListApplyTo::setIndexes(const AtomList& lst)
{
    // set unique float indexes
    auto idx = list::uniqueSorted(lst.filtered(isFloat));
    idxs_.clear();
    idxs_.reserve(idx.size());

    for (auto& el : lst)
        idxs_.push_back(el.asInt());
}

void ListApplyTo::loopProcess(const AtomList& v)
{
    if (!on_loop_) {
        OBJ_ERR << "not in process loop";
        return;
    }

    if (v.size() != 1) {
        OBJ_ERR << "single modified value expected: " << v;
        return;
    }

    mapped_.append(v[0]);
}

void ListApplyTo::normalizeIndexes(size_t N)
{
    norm_idxs_.clear();
    norm_idxs_.reserve(idxs_.size());

    for (auto i : idxs_) {
        int pos = relativeIndex(i, N);
        if (pos < 0)
            continue;

        norm_idxs_.push_back(pos);
    }
}

void setup_list_apply_to()
{
    ObjectFactory<ListApplyTo> obj("list.apply_to");
    obj.processData<DataTypeMList>();
    obj.mapFloatToList();
    obj.mapSymbolToList();
}
