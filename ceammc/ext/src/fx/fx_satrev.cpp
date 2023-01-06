#include "fx_satrev.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxSatReverb : public faust_fx_satrev_tilde {
    Property* drywet_;

public:
    FxSatReverb(const PdArgs& args)
        : faust_fx_satrev_tilde(args)
        , drywet_(property("@drywet"))
    {
        if (drywet_)
            drywet_->setArgIndex(0);

        createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv)
    {
        if (drywet_)
            drywet_->set(lv);
    }
};

void setup_fx_satrev_tilde()
{
    SoundExternalFactory<FxSatReverb> obj("fx.satrev~");
    obj.addMethod("reset", &FxSatReverb::m_reset);

    obj.setXletsInfo({ "signal: input", "float: drywet mix" }, { "signal: left output", "signal: right output" });
}
