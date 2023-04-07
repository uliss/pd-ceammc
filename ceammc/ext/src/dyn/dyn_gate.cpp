#include "dyn_gate.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
using namespace ceammc;

class DynGate : public faust_dyn_gate_tilde {
    UIProperty* threshold_;

public:
    DynGate(const PdArgs& args)
        : faust_dyn_gate_tilde(args)
    {
        CEAMMC_DEFINE_PROP_VAR(threshold)
        CEAMMC_DEFINE_PROP_VAR(attack)
        CEAMMC_DEFINE_PROP_VAR(hold)
        CEAMMC_DEFINE_PROP_VAR(release)

        threshold_ = findUIProperty(sym_prop_threshold);
        checkUIProperties({ threshold_ });

        bindPositionalArgsToProps({
            sym_prop_threshold,
            sym_prop_attack,
            sym_prop_hold,
            sym_prop_release,
        });

        createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv)
    {
        if (threshold_)
            threshold_->setList(lv);
    }
};

void setup_dyn_gate_tilde()
{
    SoundExternalFactory<DynGate> obj("dyn.gate~");
    obj.setXletsInfo({ "signal: input", "float: threshold (dbfs)" }, { "signal: output" });

    obj.setDescription("mono signal gate");
    obj.setCategory("dyn");
    obj.setKeywords({ "gate" });
}
