#include "st_bpm_array.h"
#include "ceammc_factory.h"

#include <algorithm>

ArrayBPMDetect::ArrayBPMDetect(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    if (!positionalArguments().empty()) {
        if (!array_.open(positionalSymbolArgument(0))) {
            OBJ_ERR << "can't open array";
        }
    }
}

void ArrayBPMDetect::onBang()
{
    if (!array_.update()) {
        OBJ_ERR << "invalid array: " << array_.name();
        return;
    }

    soundtouch::BPMDetect detector(1, sys_getsr());

    float buffer[1024];
    Array::iterator it = array_.begin();
    while (it < (array_.end() - 1024)) {
        std::copy(it, it + 1024, buffer);
        detector.inputSamples(buffer, 1024);
        it += 1024;
    }

    floatTo(0, detector.getBpm());
}

extern "C" void setup_soundtouch0x2ebpm()
{
    ObjectFactory<ArrayBPMDetect> obj("soundtouch.bpm");
}
