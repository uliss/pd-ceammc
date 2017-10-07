#ifndef LIST_SPLIT_H
#define LIST_SPLIT_H

#include "ceammc_object.h"

using namespace ceammc;

class ListSplit : public BaseObject {
    SizeTProperty* index_;

public:
    ListSplit(const PdArgs& args);

    void onList(const AtomList& l);
};

#endif // LIST_SPLIT_H
