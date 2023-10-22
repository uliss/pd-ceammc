#include "fx_wahwah.h"
#include "args/argcheck2.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

constexpr const char* PROP_ANGLE = "@angle";

class FxWahwah : public faust_fx_wahwah_tilde {
    UIProperty* angle_;

public:
    FxWahwah(const PdArgs& args)
        : faust_fx_wahwah_tilde(args)
        , angle_(findUIProperty(PROP_ANGLE))
    {
        bindPositionalArgsToProps({ gensym(PROP_ANGLE), gensym("@speed") });

        createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv)
    {
        static const args::ArgChecker chk("ANGLE:f[0,1]");
        if (!chk.check(lv, this))
            return chk.usage(this);

        if (angle_)
            angle_->setValue(lv.asT<t_float>(), true);
    }
};

void setup_fx_wahwah_tilde()
{
    FaustFactory<FxWahwah> obj("fx.wahwah~");

    obj.setDescription("digitized CryBaby wah pedal");
    obj.setCategory("fx");
    obj.setKeywords({ "wahwah", "guitar", "crybaby" });
    obj.setXletsInfo({ "signal: input", "float: angle (0..1)" }, { "signal: fx output" });
}
