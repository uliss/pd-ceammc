#include "noise_colored.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

using namespace ceammc;

constexpr const char* PROP_ALPHA = "@alpha";
constexpr t_float A_WHITE = 0;
constexpr t_float A_PINK = -0.5;
constexpr t_float A_BROWN = -1;
constexpr t_float A_BLUE = 0.5;
constexpr t_float A_VELVET = 1;

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

    void onSymbol(t_symbol* s) final
    {
        switch (crc32_hash(s)) {
        case "brown"_hash:
            return onFloat(A_BROWN);
        case "pink"_hash:
            return onFloat(A_PINK);
        case "white"_hash:
            return onFloat(A_WHITE);
        case "blue"_hash:
            return onFloat(A_BLUE);
        case "velvet"_hash:
            return onFloat(A_VELVET);
        default:
            OBJ_ERR << fmt::format("unknown noise type: {}, supported values are: white, pink, brown, blue and velvet");
            break;
        }
    }

    void m_white(t_symbol*, const AtomListView&) { onFloat(A_WHITE); }
    void m_pink(t_symbol*, const AtomListView&) { onFloat(A_PINK); }
    void m_brown(t_symbol*, const AtomListView&) { onFloat(A_BROWN); }
    void m_blue(t_symbol*, const AtomListView&) { onFloat(A_BLUE); }
    void m_velvet(t_symbol*, const AtomListView&) { onFloat(A_VELVET); }
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
