#include "list_search.h"
#include "ceammc_factory.h"

ListSearch::ListSearch(const PdArgs& args)
    : BaseObject(args)
    , subj_(positionalArguments())
{
    createInlet();
    createOutlet();
}

void ListSearch::onList(const AtomList& lst)
{
    AtomList idxs;
    idxs.reserve(subj_.size());

    for (size_t i = 0; i < subj_.size(); i++) {
        if (subj_[i].isAtom()) {
            // search raw atom
            idxs.append(lst.findPos(subj_[i].toAtom()));
        } else {
            int idx = -1;
            for (size_t k = 0; k < lst.size(); k++) {
                // skip non-data atoms
                if (!lst[k].isData())
                    continue;

                // compare data
                if (DataAtom(lst[k]) == subj_[i]) {
                    idx = k;
                    break;
                }
            }

            idxs.append(idx);
        }
    }

    listTo(0, idxs);
}

void ListSearch::onInlet(size_t n, const AtomList& lst)
{
    subj_.set(lst);
}

void setup_list_search()
{
    ObjectFactory<ListSearch> obj("list.search");
}
