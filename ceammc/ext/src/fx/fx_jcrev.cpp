#include "fx_jcrev.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxJCReverb : public faust_fx_jcrev_tilde {
    Property* drywet_;

public:
    FxJCReverb(const PdArgs& args)
        : faust_fx_jcrev_tilde(args)
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

void setup_fx_jcrev_tilde()
{
    SoundExternalFactory<FxJCReverb> obj("fx.jcrev~");
    obj.addMethod("reset", &FxJCReverb::m_reset);

    obj.setXletsInfo(
        {
            "signal: input",
            "float: drywet mix",
        },
        {
            "signal: 1 output",
            "signal: 2 output",
            "signal: 3 output",
            "signal: 4 output",
        });

    obj.setDescription("Schroeder quad reverberator from 1972");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "room", "reverb", "schroeder"});
}
