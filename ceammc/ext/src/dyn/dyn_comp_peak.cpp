#include "dyn_comp_peak.h"
#include "ceammc_faust_factory.h"
#include "dyn_comp_base.h"
using namespace ceammc;

class DynCompPeak : public DynCompNewT<faust_dyn_comp_peak_tilde> {
public:
    DynCompPeak(const PdArgs& args)
        : DynCompNewT<faust_dyn_comp_peak_tilde>(args)
    {
        createInlet();
        createOutlet();

        setMetersOutputFn([this](const MetersData& bg) { outputMetersTo(1); });
    }
};

void setup_dyn_comp_peak_tilde()
{
    FaustFactory<DynCompPeak> obj("dyn.comp_peak~");
    obj.addAlias("comp.peak~");

    obj.setXletsInfo({ "signal: input", "float: set compression strength" }, { "signal: output", "float: compression level" });

    obj.setDescription("mono dynamic range peak feed forward compressor");
    obj.setCategory("dyn");
    obj.setKeywords({ "compressor" });
}
