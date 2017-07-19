#include "BPMDetect.h"

#include "array_bpm.h"
#include "ceammc_factory.h"

static const size_t BUF_SIZE = 1024;

ArrayBPM::ArrayBPM(const PdArgs& a)
    : ArrayBase(a)
{
    createOutlet();
}

void ArrayBPM::onBang()
{
    if (!checkArray())
        return;

    soundtouch::BPMDetect bpm(1, sys_getsr());

    float buffer[BUF_SIZE];
    const size_t N = array_.size();
    for (size_t i = 0, hop = BUF_SIZE; i < N; i += BUF_SIZE) {
        if ((i + BUF_SIZE) > N)
            hop = N - i;

        std::copy(array_.begin() + i, array_.begin() + i + hop, buffer);
        bpm.inputSamples(buffer, BUF_SIZE);
    }

    floatTo(0, bpm.getBpm());
}

extern "C" void setup_array0x2ebpm()
{
    ObjectFactory<ArrayBPM> obj("array.bpm");
}
