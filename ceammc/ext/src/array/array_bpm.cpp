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

    int SR = sys_getsr();
    soundtouch::BPMDetect bpm(1, SR ? SR : 44100);

    float buffer[BUF_SIZE];
    const size_t N = array_.size();
    Array::iterator it = array_.begin();
    for (size_t i = 0, hop = BUF_SIZE; i < N; i += BUF_SIZE) {
        if ((i + BUF_SIZE) > N)
            hop = N - i;

        std::copy(it, it + hop, buffer);
        it += hop;
        bpm.inputSamples(buffer, BUF_SIZE);
    }

    floatTo(0, bpm.getBpm());
}

void setup_array_bpm()
{
    ObjectFactory<ArrayBPM> obj("array.bpm");

    obj.setDescription("BPM calculator");
    obj.setCategory("array");
    obj.setKeywords({"array", "bpm"});
}
