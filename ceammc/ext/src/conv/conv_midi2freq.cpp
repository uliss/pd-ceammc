#include "conv_midi2freq.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_music_temperament.h"

static t_symbol* SYM_EQ12;
static t_symbol* SYM_JUST;
static t_symbol* SYM_GANASSI;
static t_symbol* SYM_MEANTONE;
static t_symbol* SYM_KIRN3;
static t_symbol* SYM_PYTH;
static t_symbol* SYM_RAMEAU;
static t_symbol* SYM_VALOTTI;
static t_symbol* SYM_ZARLINO;

static music::TemperamentType to_temp(t_symbol* s)
{
    typedef std::pair<t_symbol*, music::TemperamentType> TPair;
    static const TPair TEMPS[] = {
        { gensym("eq"), music::TemperamentType::EQT_12_PERFECT_OCTAVE },
        { gensym("just"), music::TemperamentType::JUST },
        { gensym("ganassi"), music::TemperamentType::GANASSI_1543 },
        { gensym("meantone"), music::TemperamentType::MEANTONE },
        { gensym("kirnberger3"), music::TemperamentType::KIRNBERGER_III_1779 },
        { gensym("pythagorean"), music::TemperamentType::PYTHAGOREAN },
        { gensym("rameau"), music::TemperamentType::RAMEAU },
        { gensym("valotti"), music::TemperamentType::VALOTTI_1754 },
        { gensym("zarlino"), music::TemperamentType::ZARLINO_2_7 },
    };

    auto it = std::find_if(std::begin(TEMPS), std::end(TEMPS), [&s](const TPair& el) { return el.first == s; });
    return it == std::end(TEMPS) ? music::TemperamentType::EQT_12_PERFECT_OCTAVE : it->second;
}

Midi2Freq::Midi2Freq(const PdArgs& args)
    : BaseObject(args)
    , base_a_(nullptr)
    , prop_temperament_(nullptr)
{
    constexpr t_float MIN_A_FREQ = 200;
    constexpr t_float MAX_A_FREQ = 600;
    constexpr t_float DEF_A_FREQ = 440;
    base_a_ = new FloatProperty("@a", DEF_A_FREQ);
    base_a_->setArgIndex(0);
    base_a_->checkClosedRange(MIN_A_FREQ, MAX_A_FREQ);
    base_a_->setUnitsHz();
    addProperty(base_a_);

    prop_temperament_ = new SymbolEnumProperty("@t",
        { SYM_EQ12, SYM_JUST, SYM_GANASSI, SYM_MEANTONE, SYM_KIRN3, SYM_PYTH, SYM_RAMEAU, SYM_VALOTTI, SYM_ZARLINO });
    addProperty(prop_temperament_);

    createOutlet();
}

void Midi2Freq::onFloat(t_float f)
{
    if (prop_temperament_->value() == SYM_EQ12)
        floatTo(0, convert::midi2freq(f, base_a_->value()));
    else
        floatTo(0, music::to_freq(f, to_temp(prop_temperament_->value()), base_a_->value()));
}

void Midi2Freq::onList(const AtomListView& lv)
{
    SmallAtomList res;
    const auto A_BASE = base_a_->value();
    lv.mapFloat([A_BASE](t_float f) { return convert::midi2freq(f, A_BASE); }, res);
    listTo(0, res.view());
}

void setup_conv_midi2freq()
{
    SYM_EQ12 = gensym("eq");
    SYM_JUST = gensym("just");
    SYM_GANASSI = gensym("ganassi");
    SYM_MEANTONE = gensym("meantone");
    SYM_KIRN3 = gensym("kirnberger3");
    SYM_PYTH = gensym("pythagorean");
    SYM_RAMEAU = gensym("rameau");
    SYM_VALOTTI = gensym("valotti");
    SYM_ZARLINO = gensym("zarlino");

    ObjectFactory<Midi2Freq> obj("conv.midi2freq");
    obj.addAlias("midi->freq");
    obj.addAlias("m->f");
}
