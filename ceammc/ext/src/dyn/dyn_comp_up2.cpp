#include "dyn_comp_up2.h"
#include "ceammc_factory.h"
#include "dyn_comp_base.h"
using namespace ceammc;

class DynCompUp2 : public DynCompNewT<faust_dyn_comp_up2_tilde> {
public:
    DynCompUp2(const PdArgs& args)
        : DynCompNewT<faust_dyn_comp_up2_tilde>(args)
    {
        createInlet();
        createOutlet();

        setBargraphOutputFn([this](const BargraphData& bg) {
            if (!bg.empty())
                floatTo(2, *bg[0]);
        });
    }
};

void setup_dyn_comp_up2_tilde()
{
    SoundExternalFactory<DynCompUp2> obj("dyn.comp_up2~");
    obj.addAlias("comp.up2~");

    obj.setXletsInfo({ "signal: input", "float: set strength" },
        { "signal: output", "float: compression level (db)" });
    obj.addMethod("preset", &::DynCompUp2::m_reset);

    obj.setDescription("stereo upward compressor with channel linking");
    obj.setCategory("dyn");
    obj.setKeywords({ "compressor", "upward" });
}
