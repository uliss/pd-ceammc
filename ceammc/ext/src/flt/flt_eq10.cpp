#include "flt_eq10.h"
#include "ceammc_factory.h"

using namespace ceammc;
;

template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept
{
    return N;
}

class FltEq10 : public faust_flt_eq10_tilde {
public:
    FltEq10(const PdArgs& args)
        : faust_flt_eq10_tilde(args)
    {
        dsp_->instanceClear();
    }

    void onList(const AtomList& l) override
    {
        static t_symbol* SYM_PROP_FREQ_31 = gensym("@f31");
        static t_symbol* SYM_PROP_FREQ_62 = gensym("@f62");
        static t_symbol* SYM_PROP_FREQ_125 = gensym("@f125");
        static t_symbol* SYM_PROP_FREQ_250 = gensym("@f250");
        static t_symbol* SYM_PROP_FREQ_500 = gensym("@f500");
        static t_symbol* SYM_PROP_FREQ_1000 = gensym("@f1000");
        static t_symbol* SYM_PROP_FREQ_2000 = gensym("@f2000");
        static t_symbol* SYM_PROP_FREQ_4000 = gensym("@f4000");
        static t_symbol* SYM_PROP_FREQ_8000 = gensym("@f8000");
        static t_symbol* SYM_PROP_FREQ_16000 = gensym("@f16000");

        static t_symbol* freq_pack[] = {
            SYM_PROP_FREQ_31,
            SYM_PROP_FREQ_62,
            SYM_PROP_FREQ_125,
            SYM_PROP_FREQ_250,
            SYM_PROP_FREQ_500,
            SYM_PROP_FREQ_1000,
            SYM_PROP_FREQ_2000,
            SYM_PROP_FREQ_4000,
            SYM_PROP_FREQ_8000,
            SYM_PROP_FREQ_16000
        };

        const size_t N = std::min<size_t>(l.size(), countof(freq_pack));

        for (size_t i = 0; i < N; i++) {
            if (!l[i].isFloat())
                continue;

            setProperty(freq_pack[i], l.view(i, 1));
        }
    }
};

void setup_flt_eq10_tilde()
{
    SoundExternalFactory<FltEq10> obj("flt.eq10~");
    obj.addMethod("reset", &FltEq10::m_reset);
}
