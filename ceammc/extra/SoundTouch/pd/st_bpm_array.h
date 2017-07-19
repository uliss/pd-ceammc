#ifndef ST_BPM_ARRAY_H
#define ST_BPM_ARRAY_H

#include "BPMDetect.h"
#include "ceammc_array.h"
#include "ceammc_object.h"
using namespace ceammc;

class ArrayBPMDetect : public BaseObject {
    Array array_;
    bool on_;

public:
    ArrayBPMDetect(const PdArgs& a);
    void onBang();
};

#endif // ST_BPM_ARRAY_H
