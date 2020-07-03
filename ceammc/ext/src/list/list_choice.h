#ifndef LIST_CHOICE_H
#define LIST_CHOICE_H

#include "list_base.h"

using namespace ceammc;

class ListChoice : public ListBase {
    BoolProperty* no_repeat_;
    size_t prev_idx_;

public:
    ListChoice(const PdArgs& a);
    void onList(const AtomList& l) override;
};

void setup_list_choice();

#endif // LIST_CHOICE_H
