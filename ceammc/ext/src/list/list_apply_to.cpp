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
        [this](const AtomListView& lv) -> bool {
            setIndexes(lv);
            return true;
        })
        ->setArgIndex(0);
}

void ListApplyTo::onList(const AtomListView& lv)
{
    const int N = lv.size();
    mapped_.clear();
    mapped_.reserve(N);
    normalizeIndexes(N);

    on_loop_ = true;

    for (cur_idx_ = 0; cur_idx_ < N; cur_idx_++) {
        // found index to process
        auto it = std::find(norm_idxs_.begin(), norm_idxs_.end(), cur_idx_);

        // no proccessing required
        if (it == norm_idxs_.end())
            mapped_.append(lv[cur_idx_]);
        else
            atomTo(1, lv[cur_idx_]);
    }

    on_loop_ = false;

    listTo(0, mapped_);
}

void ListApplyTo::onInlet(size_t n, const AtomListView& lv)
{
    switch (n) {
    case 1:
        return setIndexes(lv);
    case 2:
        return loopProcess(lv);
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

void ListApplyTo::setIndexes(const AtomListView& lv)
{
    // set unique float indexes
    AtomList out;
    lv.filter(isFloat, out);
    auto idx = list::uniqueSorted(out);
    idxs_.clear();
    idxs_.reserve(idx.size());

    for (auto& el : lv)
        idxs_.push_back(el.asInt());
}

void ListApplyTo::loopProcess(const AtomListView& lv)
{
    if (!on_loop_) {
        OBJ_ERR << "not in process loop";
        return;
    }

    if (lv.size() != 1) {
        OBJ_ERR << "single modified value expected: " << lv;
        return;
    }

    mapped_.append(lv[0]);
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
