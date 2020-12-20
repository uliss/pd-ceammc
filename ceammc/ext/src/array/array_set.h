#ifndef ARRAY_SET_H
#define ARRAY_SET_H

#include "array_base.h"

class ArraySet : public ArrayMod {
    FlagProperty* normalize_;

public:
    ArraySet(const PdArgs& a);
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& values) override;
    void set(const AtomList& v);
};

extern "C" void setup_array0x2eset();

#endif // ARRAY_SET_H
