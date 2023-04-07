#include "dyn_comp_peak2.h"
#include "ceammc_factory.h"
#include "dyn_comp_base.h"
using namespace ceammc;

class DynCompPeak2 : public DynCompNewT<faust_dyn_comp_peak2_tilde> {
public:
    DynCompPeak2(const PdArgs& args)
        : DynCompNewT<faust_dyn_comp_peak2_tilde>(args)
    {
        createInlet();
        createOutlet();

        setMetersOutputFn([this](const MetersData& bg) { outputMetersTo(2); });
    }
};

void setup_dyn_comp_peak2_tilde()
{
    SoundExternalFactory<DynCompPeak2> obj("dyn.comp_peak2~");
    obj.addAlias("comp.peak2~");

    obj.setXletsInfo(
        {
            "signal: left input",
            "signal: right input",
            "float: set compression strength",
        },
        {
            "signal: left output",
            "signal: right output",
            "float: compression level",
        });
    obj.addMethod("reset", &DynCompPeak2::m_reset);

    obj.setDescription("stereo dynamic range peak feedforward compressor");
    obj.setCategory("dyn");
    obj.setKeywords({ "compressor" });
}
