#include "flt_fb_comb.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltFbComb : public faust_flt_fb_comb_tilde {
public:
    FltFbComb(const PdArgs& args)
        : faust_flt_fb_comb_tilde(args)
    {
        auto p = property("@a");
        if (p)
            p->setArgIndex(0);
    }
};

void setup_flt_fb_comb_tilde()
{
    SoundExternalFactory<FltFbComb> obj("flt.fb_comb~");
    obj.addMethod("reset", &FltFbComb::m_reset);

    obj.setDescription("feedback comb filter");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "comb"});
}
