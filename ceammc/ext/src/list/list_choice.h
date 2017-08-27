#ifndef LIST_CHOICE_H
#define LIST_CHOICE_H

#include "ceammc_object.h"

using namespace ceammc;

class ListChoice : public BaseObject {
public:
    ListChoice(const PdArgs& a);
    void onList(const AtomList& l);
};

#endif // LIST_CHOICE_H
