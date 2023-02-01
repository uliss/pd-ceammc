#include "flt_eq10.h"
#include "ceammc_factory.h"

#include <array>

using namespace ceammc;

constexpr auto NUM_BANDS = 10;
static const std::array<const char*, NUM_BANDS> STR_BANDS = {
    "@f31",
    "@f62",
    "@f125",
    "@f250",
    "@f500",
    "@f1000",
    "@f2000",
    "@f4000",
    "@f8000",
    "@f16000",
};

class FltEq10 : public faust_flt_eq10_tilde {
    std::array<Property*, NUM_BANDS> bands_;

public:
    FltEq10(const PdArgs& args)
        : faust_flt_eq10_tilde(args)
    {
        dsp_->instanceClear();

        for (size_t i = 0; i < NUM_BANDS; i++)
            bands_[i] = property(STR_BANDS[i]);
    }

    void onList(const AtomListView& lv) override
    {
        const size_t N = std::min<size_t>(lv.size(), bands_.size());

        for (size_t i = 0; i < N; i++) {
            if (!lv[i].isFloat())
                continue;

            bands_[i]->set(lv.subView(i, 1));
        }
    }
};

void setup_flt_eq10_tilde()
{
    SoundExternalFactory<FltEq10> obj("flt.eq10~");
    obj.addMethod("reset", &FltEq10::m_reset);
}
