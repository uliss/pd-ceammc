#include "flt_hpf12.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltHpf12 : public faust_flt_hpf12_tilde {
    Property* freq_;

public:
    FltHpf12(const PdArgs& args)
        : faust_flt_hpf12_tilde(args)
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

void setup_flt_hpf12_tilde()
{
    SoundExternalFactory<FltHpf12> obj("flt.hpf12~");
    obj.setXletsInfo({ "signal: input", "float: freq" }, { "signal: output" });

    obj.setDescription("High-pass second order Butterworth filter");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "highpass"});
}
