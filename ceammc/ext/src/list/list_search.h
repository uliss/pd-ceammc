#ifndef LIST_SEARCH_H
#define LIST_SEARCH_H

#include "ceammc_dataatomlist.h"
#include "ceammc_object.h"

using namespace ceammc;

class DataTypeMList;

class ListSearch : public BaseObject {
    DataAtomList subj_;

public:
    ListSearch(const PdArgs& args);
    void parseProperties() override;

    void onList(const AtomList& lst) override;
    void onInlet(size_t n, const AtomList& lst) override;
    void onDataT(const DataTypeMList& lst);
};

void setup_list_search();

#endif // LIST_SEARCH_H
