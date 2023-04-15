#include "dyn_gate.h"
#include "ceammc_faust_factory.h"
#include "dyn_gate_base.h"
using namespace ceammc;

class DynGate : public DynGateT<faust_dyn_gate_tilde> {
public:
    DynGate(const PdArgs& args)
        : DynGateT<faust_dyn_gate_tilde>(args)
    {
    }
};

void setup_dyn_gate_tilde()
{
    FaustFactory<DynGate> obj("dyn.gate~");
    obj.setXletsInfo({ "signal: input", "float: threshold (dbfs)" }, { "signal: output" });

    obj.setDescription("mono signal gate");
    obj.setCategory("dyn");
    obj.setKeywords({ "gate" });
}
