#include "fx_drone_box.h"
#include "ceammc_factory.h"

using namespace ceammc;

constexpr const char* STR_PROP_PITCH = "@pitch";

class FxDroneBox : public faust_fx_drone_box_tilde {
    UIProperty* pitch_;

public:
    FxDroneBox(const PdArgs& args)
        : faust_fx_drone_box_tilde(args)
        , pitch_(static_cast<UIProperty*>(property(STR_PROP_PITCH)))
    {
        createInlet();
        bindPositionalArgsToProps({ gensym(STR_PROP_PITCH) });
    }

    void onInlet(size_t n, const AtomListView& lv)
    {
        pitch_->set(lv);
    }
};

void setup_fx_drone_box_tilde()
{
    SoundExternalFactory<FxDroneBox> obj("fx.drone_box~");

    obj.setDescription("mono sympathetic resonance generator");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "drone" });
    obj.setXletsInfo({ "signal: input", "float: resonance pitch (MIDI pitch)" }, { "signal: output" });
}
