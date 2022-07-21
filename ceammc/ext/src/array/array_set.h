#ifndef ARRAY_SET_H
#define ARRAY_SET_H

#include "array_base.h"

class ArraySet : public ArrayMod {
    FlagProperty* normalize_;

public:
    ArraySet(const PdArgs& a);
    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;
    void set(const AtomListView& lv);
};

void setup_array_set();

#endif // ARRAY_SET_H
