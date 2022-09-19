#include "list_reduce.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListReduce::ListReduce(const PdArgs& a)
    : BaseObject(a)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void ListReduce::onList(const AtomListView& lv)
{
    if (lv.empty())
        return;

    accum_ = lv[0];
    StaticAtomList<2> pair { 0.f, 0.f };

    for (size_t i = 1; i < lv.size(); i++) {
        pair[0] = accum_;
        pair[1] = lv[i];
        listTo(1, pair.view());
    }

    atomTo(0, accum_);
}

void ListReduce::onInlet(size_t n, const AtomListView& lv)
{
    if (n != 1)
        return;

    accum_ = lv.empty() ? Atom(0.) : lv[0];
}

void ListReduce::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void setup_list_reduce()
{
    ObjectFactory<ListReduce> obj("list.reduce");
    obj.addAlias("list.foldl");
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdSymbolFn();
    obj.processData<DataTypeMList>();

    obj.setDescription("apply function of two arguments cumulatively to the list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "min", "compare" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListReduce::setInletsInfo(obj.classPointer(), { "list or Mlist", "atom: from side-chain" });
    ListReduce::setOutletsInfo(obj.classPointer(), { "atom", "list: ACCUM ATOM" });
}
