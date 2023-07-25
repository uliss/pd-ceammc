#include "flt_bpf24.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltBpf24 : public faust_flt_bpf24_tilde {
    UIProperty *freq_, *q_;

public:
    FltBpf24(const PdArgs& args)
        : faust_flt_bpf24_tilde(args)
        , freq_(findUIProperty("@freq"))
        , q_(findUIProperty("@q"))
    {
        if (freq_)
            freq_->setArgIndex(0);

        if (q_)
            q_->setArgIndex(1);

        createInlet();
        createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv) final
    {
        if (freq_ && n == 1)
            freq_->setList(lv);

        if (q_ && n == 2)
            q_->setList(lv);
    }
};

void setup_flt_bpf24_tilde()
{
    SoundExternalFactory<FltBpf24> obj("flt.bpf24~");
    obj.addMethod("reset", &FltBpf24::m_reset);
    obj.setXletsInfo({ "input signal", "float: set freq", "float: set Q" }, { "output signal" });

    obj.setDescription("Four pole band-pass butterworth filter");
    obj.setCategory("flt");
    obj.setKeywords({ "filter", "biquad", "bpf" });
}
