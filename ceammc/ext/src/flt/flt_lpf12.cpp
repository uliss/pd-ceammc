#include "flt_lpf12.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltLpf12 : public faust_flt_lpf12_tilde {
    Property* freq_;

public:
    FltLpf12(const PdArgs& args)
        : faust_flt_lpf12_tilde(args)
        , freq_(property("@freq"))
    {
        bindPositionalArgsToProps({ gensym("@freq") });
        createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv)
    {
        if (freq_)
            freq_->set(lv);
    }
};

void setup_flt_lpf12_tilde()
{
    SoundExternalFactory<FltLpf12> obj("flt.lpf12~");
    obj.setXletsInfo({ "signal: input", "float: freq" }, { "signal: output" });

    obj.setDescription("Low-pass second order Butterworth filter");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "lowpass"});
}
