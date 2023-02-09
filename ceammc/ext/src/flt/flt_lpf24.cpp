#include "flt_lpf24.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltLpf24 : public faust_flt_lpf24_tilde {
    Property* freq_;

public:
    FltLpf24(const PdArgs& args)
        : faust_flt_lpf24_tilde(args)
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

void setup_flt_lpf24_tilde()
{
    SoundExternalFactory<FltLpf24> obj("flt.lpf24~");
    obj.setXletsInfo({ "signal: input", "float: freq" }, { "signal: output" });

    obj.setDescription("Low-pass fourth order Butterworth filter");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "lowpass"});
}
