#include "list_search.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListSearch::ListSearch(const PdArgs& args)
    : ListBase(args)
    , subj_(args.args)
{
    createInlet();
    createOutlet();
}

void ListSearch::parseProperties()
{
    // empty
}

void ListSearch::onList(const AtomList& lst)
{
    AtomList idxs;
    idxs.reserve(subj_.size());

    for (size_t i = 0; i < subj_.size(); i++) {
        idxs.append(lst.findPos(subj_[i]));
    }

    listTo(0, idxs);
}

void ListSearch::onInlet(size_t n, const AtomList& lst)
{
    subj_ = lst;
}

void setup_list_search()
{
    ObjectFactory<ListSearch> obj("list.search");
    obj.processData<DataTypeMList>();

    obj.setDescription("search specified items in input list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "search", "find" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 6);

    ListSearch::setInletsInfo(obj.classPointer(), { "list or Mlist", "list: set search items" });
    ListSearch::setOutletsInfo(obj.classPointer(), { "list: list of element indexes in input list, -1 if not found" });
}
