#include "flt_dcblock.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltDcBlock : public faust_flt_dcblock_tilde {
public:
    FltDcBlock(const PdArgs& args)
        : faust_flt_dcblock_tilde(args)
    {
    }
};

void setup_flt_dcblock_tilde()
{
    SoundExternalFactory<FltDcBlock> obj("flt.dcblock~");

    obj.setDescription("mono DC blocker");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "dcblock"});
}
