#include "flt_filterbank5x1.h"
#include "ceammc_factory.h"

#include <array>

using namespace ceammc;

constexpr size_t NUM_BANDS = 5;
static const std::array<const char*, NUM_BANDS> STR_BANDS = {
    "@f250",
    "@f500",
    "@f1000",
    "@f2000",
    "@f4000",
};

class FltFilterBank5x1 : public faust_flt_filterbank5x1_tilde {
    std::array<Property*, NUM_BANDS> bands_;

public:
    FltFilterBank5x1(const PdArgs& args)
        : faust_flt_filterbank5x1_tilde(args)
    {
        bindPositionalArgsToProps({
            gensym(STR_BANDS[0]),
            gensym(STR_BANDS[1]),
            gensym(STR_BANDS[2]),
            gensym(STR_BANDS[3]),
            gensym(STR_BANDS[4]),
        });

        for (size_t i = 0; i < NUM_BANDS; i++)
            bands_[i] = property(STR_BANDS[i]);
    }

    void onList(const AtomListView& lv) override
    {
        const size_t N = std::min<size_t>(lv.size(), bands_.size());

        for (size_t i = 0; i < N; i++)
            bands_[i]->set(lv.subView(i, 1));
    }
};

void setup_flt_filterbank5x1_tilde()
{
    SoundExternalFactory<FltFilterBank5x1> obj("flt.fbank5x1~");
    obj.addMethod("reset", &FltFilterBank5x1::m_reset);
    obj.setXletsInfo({ "input signal" },
        {
            "LowPass: 250Hz",
            "BandPass: 500Hz",
            "BandPass: 1000Hz",
            "BandPass: 2000Hz",
            "HighPass: 4000Hz",
        });
}
