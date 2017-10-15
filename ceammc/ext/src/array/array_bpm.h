#ifndef ARRAY_BPM_H
#define ARRAY_BPM_H

#include "array_base.h"
using namespace ceammc;

class ArrayBPM : public ArrayBase {
public:
    ArrayBPM(const PdArgs& a);
    void onBang();
};

extern "C" void setup_array0x2ebpm();

#endif // ARRAY_BPM_H
