#include "noise_colored.h"
#include "ceammc_factory.h"

using namespace ceammc;

constexpr const char* PROP_ALPHA = "@alpha";

class NoiseColored : public faust_noise_colored_tilde {
    UIProperty* alpha_ { nullptr };

public:
    NoiseColored(const PdArgs& args)
        : faust_noise_colored_tilde(args)
        , alpha_(findUIProperty(PROP_ALPHA))
    {
        bindPositionalArgsToProps({ gensym(PROP_ALPHA) });
    }

    void onFloat(t_float f) final
    {
        if (alpha_)
            alpha_->setValue(f, true);
    }

    void m_white(t_symbol*, const AtomListView&) { onFloat(0); }
    void m_pink(t_symbol*, const AtomListView&) { onFloat(-0.5); }
    void m_brown(t_symbol*, const AtomListView&) { onFloat(-1); }
    void m_blue(t_symbol*, const AtomListView&) { onFloat(0.5); }
    void m_velvet(t_symbol*, const AtomListView&) { onFloat(1); }
};

void setup_noise_colored_tilde()
{
    SoundExternalFactory<NoiseColored> obj("noise.colored~", OBJECT_FACTORY_DEFAULT);
    obj.setXletsInfo({ "float: set roll-off factor" }, { "signal: output" });

    obj.addMethod("white", &NoiseColored::m_white);
    obj.addMethod("pink", &NoiseColored::m_pink);
    obj.addMethod("brown", &NoiseColored::m_brown);
    obj.addMethod("blue", &NoiseColored::m_blue);
    obj.addMethod("velvet", &NoiseColored::m_velvet);

    obj.setDescription("colored noise generator with an arbitrary spectral roll");
    obj.setCategory("noise");
    obj.setKeywords({ "noise", "colored", "white", "red", "brown", "blue", "velvet" });
}
