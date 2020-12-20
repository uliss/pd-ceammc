#ifndef ARRAY_BPM_H
#define ARRAY_BPM_H

#include "array_base.h"
using namespace ceammc;

class ArrayBPM : public ArrayBase {
public:
    ArrayBPM(const PdArgs& a);
    void onBang() override;
};

void setup_array_bpm();

#endif // ARRAY_BPM_H
