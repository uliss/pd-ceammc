#include "dyn_expand.h"
#include "ceammc_factory.h"
#include "dyn_comp_base.h"
using namespace ceammc;

class DynExpand : public DynCompNewT<faust_dyn_expand_tilde> {
public:
    DynExpand(const PdArgs& args)
        : DynCompNewT<faust_dyn_expand_tilde>(args)
    {
        createInlet();
        createOutlet();

        setMetersOutputFn([this](const MetersData& bg) { outputMetersTo(1); });
    }
};

void setup_dyn_expand_tilde()
{
    SoundExternalFactory<DynExpand> obj("dyn.expand~");
    obj.setXletsInfo({ "signal: input", "float: set expander strength" },
        { "signal: output", "float: expander level" });
    obj.addMethod("reset", &DynExpand::m_reset);

    obj.setDescription("mono dynamic range expander");
    obj.setCategory("dyn");
    obj.setKeywords({ "expander" });
}
