#include "flt_speaker.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltSpeaker : public faust_flt_speaker_tilde {
    UIProperty *f1_ { nullptr }, *f2_ { nullptr };

public:
    FltSpeaker(const PdArgs& args)
        : faust_flt_speaker_tilde(args)
    {
        CEAMMC_DEFINE_PROP_VAR(f1);
        CEAMMC_DEFINE_PROP_VAR(f2);

        f1_ = findUIProperty(sym_prop_f1);
        f2_ = findUIProperty(sym_prop_f2);

        bindPositionalArgsToProps({ sym_prop_f1, sym_prop_f2 });

        createInlet();
        createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv) final
    {
        if (f1_ && n == 1)
            f1_->setList(lv);
        else if (f2_ && n == 2)
            f2_->setList(lv);
    }
};

void setup_flt_speaker_tilde()
{
    SoundExternalFactory<FltSpeaker> obj("flt.speaker~");
    obj.addMethod("reset", &FltSpeaker::m_reset);
    obj.setXletsInfo({ "signal: input", "float: freq0", "float: freq1" }, { "signal: output" });

    obj.setDescription("dirt-simple speaker simulator");
    obj.setCategory("flt");
    obj.setKeywords({ "filter", "speaker" });
}
