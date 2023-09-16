#include "dyn_gate2.h"
#include "ceammc_faust_factory.h"
#include "dyn_gate_base.h"
using namespace ceammc;

class DynGate2 : public DynGateT<faust_dyn_gate2_tilde> {
public:
    DynGate2(const PdArgs& args)
        : DynGateT<faust_dyn_gate2_tilde>(args)
    {
    }
};

void setup_dyn_gate2_tilde()
{
    FaustFactory<DynGate2> obj("dyn.gate2~");
    obj.setXletsInfo(
        { "signal: left input", "signal: right input", "float: threshold (dbfs)" },
        { "signal: left output", "signal: right output" });

    obj.setDescription("stereo signal gate");
    obj.setCategory("dyn");
    obj.setKeywords({ "gate" });
}
