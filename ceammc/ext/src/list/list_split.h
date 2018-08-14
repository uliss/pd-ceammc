#ifndef LIST_SPLIT_H
#define LIST_SPLIT_H

#include "ceammc_object.h"

using namespace ceammc;
class DataTypeMList;

class ListSplit : public BaseObject {
    SizeTProperty* index_;

public:
    ListSplit(const PdArgs& args);

    void onList(const AtomList& l) override;
    void onDataT(const DataTypeMList& lst);
};

void setup_list_split();

#endif // LIST_SPLIT_H
