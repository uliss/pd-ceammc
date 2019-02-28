#include "flt_moog_vcf.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_RES = gensym("@res");

class FltMoogVcf : public faust_flt_moog_vcf_tilde {
public:
    FltMoogVcf(const PdArgs& args)
        : faust_flt_moog_vcf_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_RES });
    }
};

void setup_flt_moog_vcf_tilde()
{
    SoundExternalFactory<FltMoogVcf> obj("flt.moog_vcf~");
}
