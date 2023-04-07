#include "fx_drone_box.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxDroneBox : public faust_fx_drone_box_tilde {
    UIProperty* pitch_ { 0 };

public:
    FxDroneBox(const PdArgs& args)
        : faust_fx_drone_box_tilde(args)
    {
        CEAMMC_DEFINE_PROP_VAR(pitch);
        pitch_ = findUIProperty(sym_prop_pitch);

        createInlet();
        bindPositionalArgsToProps({ sym_prop_pitch });
    }

    void onInlet(size_t n, const AtomListView& lv)
    {
        pitch_->set(lv);
    }
};

void setup_fx_drone_box_tilde()
{
    SoundExternalFactory<FxDroneBox> obj("fx.drone_box~");
    obj.setXletsInfo({ "signal: exciting signal", "float: set drone pitch" }, { "signal: output" });

    obj.setDescription("mono sympathetic resonance generator");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "drone" });
    obj.setXletsInfo({ "signal: input", "float: resonance pitch (MIDI pitch)" }, { "signal: output" });
}
