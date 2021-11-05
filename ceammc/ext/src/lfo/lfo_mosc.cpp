#include "lfo_mosc.h"
#include "ceammc_factory.h"
#include "ceammc_property_enum.h"
using namespace ceammc;

class LfoMosc : public faust_lfo_mosc_tilde {
    SymbolEnumProperty* wave_;
    Property* index_;

public:
    LfoMosc(const PdArgs& args)
        : faust_lfo_mosc_tilde(args)
        , wave_(nullptr)
        , index_(property(gensym("@windex")))
    {
        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));

        wave_ = new SymbolEnumProperty("@wave", {
                                                    "sin",
                                                    "saw",
                                                    "tri",
                                                    "sqr",
                                                    "pulse",
                                                    "+sin",
                                                    "+saw",
                                                    "+tri",
                                                    "+sqr",
                                                    "+pulse",
                                                });
        wave_->setArgIndex(1);
        wave_->setSuccessFn([this](Property*) {
            auto idx = wave_->enumIndex(wave_->value());
            index_->setList(Atom(idx));
        });
        addProperty(wave_);

        index_->setSuccessFn([this](Property*) {
            int idx = 0;
            if (index_->getInt(idx)) {
                auto& vals = wave_->infoT().enumValues();
                if (idx >= 0 && idx < vals.size())
                    wave_->setValue(vals[idx].asSymbol());
            }
        });
    }

    void onInlet(size_t n, const AtomListView& lv) override
    {
        if (lv.empty())
            dsp_->instanceClear();
        else if (lv.isInteger())
            index_->set(lv);
        else
            OBJ_ERR << "usage: bang to reset phase or index to select waveform";
    }
};

void setup_lfo_mosc_tilde()
{
    SoundExternalFactory<LfoMosc> obj("lfo.mosc~");
    obj.addMethod("reset", &LfoMosc::m_reset);
    obj.setXletsInfo({ "freq in hz", "bang: reset phase\n"
                                     "int: select waveform\n"
                                     "  - sin(0), saw(1), tri(2), sqr(3), pulse(4)\n"
                                     "  - +sin(5), +saw(6), +tri(7), +sqr(8), +pulse(9)" },
        { "oscillator output" });
}
