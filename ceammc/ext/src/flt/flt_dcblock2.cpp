#include "flt_dcblock2.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltDcBlock2 : public faust_flt_dcblock2_tilde {
public:
    FltDcBlock2(const PdArgs& args)
        : faust_flt_dcblock2_tilde(args)
    {
    }
};

void setup_flt_dcblock2_tilde()
{
    SoundExternalFactory<FltDcBlock2> obj("flt.dcblock2~");

    obj.setDescription("stereo DC blocker");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "dcblock"});
}
