#include "dyn_comp_peak_ff.h"
#include "ceammc_factory.h"
#include "dyn_comp_base.h"
using namespace ceammc;

class DynCompPeakFF : public DynCompNewT<faust_dyn_comp_peak_ff_tilde> {
public:
    DynCompPeakFF(const PdArgs& args)
        : DynCompNewT<faust_dyn_comp_peak_ff_tilde>(args)
    {
        createInlet();
        createOutlet();

        setBargraphOutputFn([this](const BargraphData& bg) {
            if (!bg.empty())
                floatTo(1, *bg[0]);
        });
    }
};

void setup_dyn_comp_peak_ff_tilde()
{
    SoundExternalFactory<DynCompPeakFF> obj("dyn.comp_peak_ff~");
    obj.setXletsInfo({ "signal: input", "float: set compression strength" }, { "signal: output", "float: compression level" });
    obj.addMethod("preset", &DynCompPeakFF::m_reset);

    obj.setDescription("mono dynamic range peak feed forward compressor");
    obj.setCategory("dyn");
    obj.setKeywords({ "compressor" });
}
