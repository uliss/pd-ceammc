#include "dyn_comp2.h"
#include "ceammc_factory.h"
#include "dyn_comp_base.h"
using namespace ceammc;

class DynComp2 : public DynCompBase<faust_dyn_comp2_tilde> {
public:
    DynComp2(const PdArgs& args)
        : DynCompBase<faust_dyn_comp2_tilde>(args)
    {
        createOutlet();
        setBargraphOutputFn([this](const BargraphData& bg) {
            if (!bg.empty())
                floatTo(2, *bg[0]);
        });
    }
};

void setup_dyn_comp2_tilde()
{
    SoundExternalFactory<DynComp2> obj("dyn.comp2~");
    obj.setXletsInfo({ "signal: left input", "signal: right input" },
        { "signal: left output", "signal: right output", "float: compression level" });
    obj.addMethod("preset", &DynComp2::m_preset);

    obj.setDescription("stereo dynamic range compressor");
    obj.setCategory("dyn");
    obj.setKeywords({ "compressor" });
}
