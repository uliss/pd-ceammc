#include "list_apply_to.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

ListApplyTo::ListApplyTo(const ceammc::PdArgs& args)
    : BaseObject(args)
    , cur_idx_(0)
    , on_loop_(false)
{
    createInlet();
    createInlet();
    createOutlet();
    createOutlet();

    createCbListProperty(
        "@indexes",
        [this]() -> AtomList {
            AtomList res;
            for (auto i : idxs_)
                res.append(i);

            return res;
        },
        [this](const AtomList& l) -> bool {
            setIndexes(l);
            return true;
        })
        ->setArgIndex(0);
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

void ListApplyTo::onInlet(size_t n, const AtomListView& lst)
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

void ListApplyTo::onDataT(const MListAtom& ml)
{
    const int N = ml->size();
    mapped_.clear();
    mapped_.reserve(N);
    normalizeIndexes(N);

    on_loop_ = true;

    for (cur_idx_ = 0; cur_idx_ < N; cur_idx_++) {
        // found index to process
        auto it = std::find(norm_idxs_.begin(), norm_idxs_.end(), cur_idx_);

        // no proccessing required
        if (it == norm_idxs_.end())
            mapped_.append((*ml)[cur_idx_]);
        else
            atomTo(1, (*ml)[cur_idx_]);
    }

    on_loop_ = false;
    atomTo(0, new DataTypeMList(mapped_));
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
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdSymbolFn();
    obj.noPropsDispatch();

    obj.setDescription("modifies list value at specified position, filtering it via external object");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "functional", "apply" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListApplyTo::setInletsInfo(obj.classPointer(), { "list or Mlist", "list: set list indexes", "atom: processed value from side-chain" });
    ListApplyTo::setOutletsInfo(obj.classPointer(), { "list or Mlist", "atom: to side-chain" });
}
