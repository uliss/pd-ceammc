#include "dyn_comp.h"
#include "ceammc_factory.h"
#include "dyn_comp_base.h"
using namespace ceammc;

class DynComp : public DynCompBase<faust_dyn_comp_tilde> {
public:
    DynComp(const PdArgs& args)
        : DynCompBase<faust_dyn_comp_tilde>(args)
    {
        createOutlet();
        setMetersOutputFn([this](const MetersData& bg) { outputMetersTo(1); });
    }
};

void setup_dyn_comp_tilde()
{
    SoundExternalFactory<DynComp> obj("dyn.comp~");
    obj.setXletsInfo({ "signal: input" }, { "signal: output", "float: compression level" });
    obj.addMethod("preset", &DynComp::m_preset);

    obj.setDescription("mono dynamic range compressors");
    obj.setCategory("dyn");
    obj.setKeywords({ "compressor" });
}
