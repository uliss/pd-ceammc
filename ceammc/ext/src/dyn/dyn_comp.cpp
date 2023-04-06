#include "dyn_comp.h"
#include "args/argcheck2.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
using namespace ceammc;

CEAMMC_DEFINE_HASH(vocal)
CEAMMC_DEFINE_HASH(drums)
CEAMMC_DEFINE_HASH(bass)
CEAMMC_DEFINE_HASH(strings)

class DynComp : public faust_dyn_comp_tilde {
    UIProperty *ratio_, *threshold_, *attack_, *release_;

public:
    DynComp(const PdArgs& args)
        : faust_dyn_comp_tilde(args)
    {
        CEAMMC_DEFINE_PROP_VAR(ratio);
        CEAMMC_DEFINE_PROP_VAR(threshold);
        CEAMMC_DEFINE_PROP_VAR(attack);
        CEAMMC_DEFINE_PROP_VAR(release);

        ratio_ = findUIProperty(sym_prop_ratio);
        threshold_ = findUIProperty(sym_prop_threshold);
        attack_ = findUIProperty(sym_prop_attack);
        release_ = findUIProperty(sym_prop_release);

        bindPositionalArgsToProps({ sym_prop_ratio,
            sym_prop_threshold,
            sym_prop_attack,
            sym_prop_release });

        createOutlet();
        setBargraphOutputFn([this](const BargraphList& bg) {
            for (size_t i = 0; i < bg.size(); i++)
                floatTo(1 + i, *bg[i]);
        });
    }

    void setCompress(t_float ratio, t_float threshold, t_float attack, t_float release)
    {
        if (!checkUIProperties({ ratio_, threshold_, attack_, release_ }))
            return;

        ratio_->setValue(ratio, true);
        threshold_->setValue(threshold, true);
        attack_->setValue(attack, true);
        release_->setValue(release, true);
    }

    void m_preset(t_symbol* m, const AtomListView& lv)
    {
        static const args::ArgChecker chk("PRESET:s=vocal|drums|bass|strings");
        if (!chk.check(lv, this))
            return chk.usage(this, m);

        switch (crc32_hash(lv.symbolAt(0, &s_))) {
        case hash_vocal:
            setCompress(2, -8, 2, 40);
            break;
        case hash_bass:
            setCompress(5, -10, 8, 40);
            break;
        case hash_drums:
            setCompress(10, -15, 1, 1);
            break;
        case hash_strings:
            setCompress(2, -10, 40, 100);
            break;
        default:
            OBJ_ERR << "unknown preset: " << lv;
            break;
        }
    }
};

void setup_dyn_comp_tilde()
{
    SoundExternalFactory<DynComp> obj("dyn.comp~");
    obj.setXletsInfo({ "signal: input" }, { "signal: output", "float: compression level" });
    obj.addMethod("preset", &DynComp::m_preset);

    obj.setDescription("mono dynamic range compressors");
    obj.setCategory("dyn");
    obj.setKeywords({ "compressor" });
}
