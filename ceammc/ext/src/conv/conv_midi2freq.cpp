#include "conv_midi2freq.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_music_temperament.h"

static t_symbol* SYM_EQ12 = gensym("eq");
static t_symbol* SYM_JUST = gensym("just");
static t_symbol* SYM_GANASSI = gensym("ganassi");
static t_symbol* SYM_MEANTONE = gensym("meantone");
static t_symbol* SYM_KIRN3 = gensym("kirnberger3");
static t_symbol* SYM_PYTH = gensym("pythagorean");
static t_symbol* SYM_RAMEAU = gensym("rameau");
static t_symbol* SYM_VALOTTI = gensym("valotti");
static t_symbol* SYM_ZARLINO = gensym("zarlino");

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
    base_a_ = new FloatPropertyClosedRange("@a", positionalFloatArgument(0, 440.f), 200, 600);
    createProperty(base_a_);

    prop_temperament_ = new SymbolEnumProperty("@t", SYM_EQ12);
    prop_temperament_->appendEnum(SYM_JUST);
    prop_temperament_->appendEnum(SYM_GANASSI);
    prop_temperament_->appendEnum(SYM_MEANTONE);
    prop_temperament_->appendEnum(SYM_KIRN3);
    prop_temperament_->appendEnum(SYM_PYTH);
    prop_temperament_->appendEnum(SYM_PYTH);
    prop_temperament_->appendEnum(SYM_VALOTTI);
    prop_temperament_->appendEnum(SYM_ZARLINO);
    createProperty(prop_temperament_);

    createOutlet();
}

void Midi2Freq::onFloat(t_float f)
{
    if (prop_temperament_->value() == SYM_EQ12)
        floatTo(0, convert::midi2freq(f, base_a_->value()));
    else
        floatTo(0, music::to_freq(f, to_temp(prop_temperament_->value()), base_a_->value()));
}

void Midi2Freq::onList(const AtomList& lst)
{
    AtomList res(lst);
    const auto A_BASE = base_a_->value();

    for (auto& el : res) {
        if (!el.isFloat()) {
            el.setFloat(0, true);
            continue;
        }

        el.setFloat(convert::midi2freq(el.asFloat(), A_BASE));
    }

    listTo(0, res);
}

void setup_conv_midi2freq()
{
    ObjectFactory<Midi2Freq> obj("conv.midi2freq");
    obj.addAlias("midi->freq");
    obj.addAlias("m->f");
}
