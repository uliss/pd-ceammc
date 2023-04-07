#include "dyn_expand2.h"
#include "ceammc_factory.h"
#include "dyn_comp_base.h"
using namespace ceammc;

class DynExpand2 : public DynCompNewT<faust_dyn_expand2_tilde> {
public:
    DynExpand2(const PdArgs& args)
        : DynCompNewT<faust_dyn_expand2_tilde>(args)
    {
        createInlet();
        createOutlet();

        setMetersOutputFn([this](const MetersData&) { outputMetersTo(2); });
    }
};

void setup_dyn_expand2_tilde()
{
    SoundExternalFactory<DynExpand2> obj("dyn.expand2~");
    obj.setXletsInfo(
        {
            "signal: left input",
            "signal: right input",
            "float: set expander strength",
        },
        {
            "signal: left output",
            "signal: right output",
            "float: expander level",
        });

    obj.addMethod("reset", &DynExpand2::m_reset);

    obj.setDescription("stereo dynamic range expander");
    obj.setCategory("dyn");
    obj.setKeywords({ "expander" });
}
