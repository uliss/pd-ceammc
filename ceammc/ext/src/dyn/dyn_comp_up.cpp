#include "dyn_comp_up.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
using namespace ceammc;

class DynCompUp : public faust_dyn_comp_up_tilde {
    UIProperty* strength_ { 0 };

public:
    DynCompUp(const PdArgs& args)
        : faust_dyn_comp_up_tilde(args)
    {
        createInlet();
        createOutlet();

        CEAMMC_DEFINE_PROP_VAR(strength);
        CEAMMC_DEFINE_PROP_VAR(threshold);
        CEAMMC_DEFINE_PROP_VAR(attack);
        CEAMMC_DEFINE_PROP_VAR(release);

        strength_ = findUIProperty(sym_prop_strength);

        bindPositionalArgsToProps({ sym_prop_strength,
            sym_prop_threshold,
            sym_prop_attack,
            sym_prop_release });

        setMetersOutputFn([this](const MetersData& bg) { outputMetersTo(1); });
    }

    void onInlet(size_t n, const AtomListView& lv) final
    {
        if (strength_)
            strength_->set(lv);
    }
};

void setup_dyn_comp_up_tilde()
{
    SoundExternalFactory<DynCompUp> obj("dyn.comp_up~");
    obj.addAlias("comp.up~");

    obj.setXletsInfo({ "signal: input", "float: set strength" },
        { "signal: output", "float: compression level (db)" });
    obj.addMethod("reset", &::DynCompUp::m_reset);

    obj.setDescription("mono upward compressor");
    obj.setCategory("dyn");
    obj.setKeywords({ "compressor", "upward" });
}
