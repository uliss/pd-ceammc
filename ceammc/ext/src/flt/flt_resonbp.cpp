#include "flt_resonbp.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltResonBp : public faust_flt_resonbp_tilde {
    Property *freq_, *q_;

public:
    FltResonBp(const PdArgs& args)
        : faust_flt_resonbp_tilde(args)
        , freq_(property("@freq"))
        , q_(property("@q"))
    {
        if (freq_)
            freq_->setArgIndex(0);

        if (q_)
            q_->setArgIndex(1);

        createInlet();
        createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv)
    {
        if (n == 1 && freq_)
            freq_->set(lv);

        if (n == 2 && q_)
            q_->set(lv);
    }
};

void setup_flt_resonbp_tilde()
{
    SoundExternalFactory<FltResonBp> obj("flt.resonbp~");
    obj.setXletsInfo({ "signal: input", "float: freq", "float: Q-factor" }, { "signal: output" });
}
