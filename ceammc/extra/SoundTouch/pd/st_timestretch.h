#ifndef ST_TIMESTRETCH_H
#define ST_TIMESTRETCH_H

#include "SoundTouch.h"
#include "ceammc_array.h"
#include "ceammc_object.h"
using namespace ceammc;

class TimeStretch : public BaseObject {
    Array array_src_;
    Array array_dest_;
    soundtouch::SoundTouch tstretch_;
    FloatProperty* pitch_;
    FloatProperty* tempo_;
    FloatProperty* rate_;
    FlagProperty* speech_;

public:
    TimeStretch(const PdArgs& a);
    void onBang();
};

#endif // ST_TIMESTRETCH_H
