#include "dyn_comp2.h"
#include "ceammc_faust_factory.h"
#include "dyn_comp_base.h"
using namespace ceammc;

class DynComp2 : public DynCompBase<faust_dyn_comp2_tilde> {
public:
    DynComp2(const PdArgs& args)
        : DynCompBase<faust_dyn_comp2_tilde>(args)
    {
        createOutlet();
        setMetersOutputFn([this](const MetersData& bg) { outputMetersTo(2); });
    }
};

void setup_dyn_comp2_tilde()
{
    FaustFactory<DynComp2> obj("dyn.comp2~");
    obj.setXletsInfo({ "signal: left input", "signal: right input" },
        { "signal: left output", "signal: right output", "float: compression level" });
    obj.addMethod("preset", &DynComp2::m_preset);

    obj.setDescription("stereo dynamic range compressor");
    obj.setCategory("dyn");
    obj.setKeywords({ "compressor" });
}
