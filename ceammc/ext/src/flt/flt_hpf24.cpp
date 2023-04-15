#include "flt_hpf24.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FltHpf24 : public faust_flt_hpf24_tilde {
    Property* freq_;

public:
    FltHpf24(const PdArgs& args)
        : faust_flt_hpf24_tilde(args)
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

void setup_flt_hpf24_tilde()
{
    FaustFactory<FltHpf24> obj("flt.hpf24~");
    obj.setXletsInfo({ "signal: input", "float: freq" }, { "signal: output" });

    obj.setDescription("High-pass fourth order Butterworth filter");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "highpass"});
}
