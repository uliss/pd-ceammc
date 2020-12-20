#include "flt_moog_vcf.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltMoogVcf : public faust_flt_moog_vcf_tilde {
public:
    FltMoogVcf(const PdArgs& args)
        : faust_flt_moog_vcf_tilde(args)
    {
        static t_symbol* SYM_PROP_RES = gensym("@res");

        bindPositionalArgsToProps({ SYM_PROP_RES });
    }
};

void setup_flt_moog_vcf_tilde()
{
    SoundExternalFactory<FltMoogVcf> obj("flt.moog_vcf~");
    obj.addMethod("reset", &FltMoogVcf::m_reset);
}
