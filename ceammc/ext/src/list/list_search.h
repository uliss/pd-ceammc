#ifndef LIST_SEARCH_H
#define LIST_SEARCH_H

#include "ceammc_data.h"
#include "list_base.h"

using namespace ceammc;

class ListSearch : public ListBase {
    AtomList subj_;

public:
    ListSearch(const PdArgs& args);
    void parseProperties() override;

    void onList(const AtomListView& lv) override;
    void onInlet(size_t n, const AtomListView& lv) override;
};

void setup_list_search();

#endif // LIST_SEARCH_H
