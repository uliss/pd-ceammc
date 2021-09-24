/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "fluid.h"
#include "ceammc_args.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"
#include "fmt/format.h"
#include "proto/proto_midi_cc.h"

#include "fluidsynth.h"
#include "midi/midi_names.h"
#include "sfloader/fluid_sfont.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

constexpr const char* DEFAULT_SOUNDFONT = "sf2/WaveSine.sf2";
constexpr const char* PAN_VALUE_NAME = "pan position";
constexpr const char* BEND_VALUE_NAME = "pitchbend";
constexpr const char* BEND_SENS_VALUE_NAME = "pitchbend sensivity";
constexpr const char* TUNE_CENTS_VALUE_NAME = "tune(cents)";
constexpr const char* TUNE_SEMITONES_VALUE_NAME = "tune(semitones)";
constexpr const char* HOLD_VALUE_NAME = "hold pedal";
constexpr const char* SOSTENUTO_VALUE_NAME = "sostenuto pedal";
constexpr const char* SOFT_VALUE_NAME = "soft pedal";
constexpr const char* LEGATO_VALUE_NAME = "legato pedal";

template <int IN_LEFT, int IN_CENTER, int IN_RIGHT>
t_float pan_to_fluid(t_float v)
{
    // note: -500..500 range found in fluid sources: fluid_conv.c
    constexpr int OUT_LEFT = -500;
    constexpr int OUT_CENTER = 0;
    constexpr int OUT_RIGHT = 500;

    return (v <= IN_CENTER)
        ? convert::lin2lin<t_float, IN_LEFT, IN_CENTER>(v, OUT_LEFT, OUT_CENTER)
        : convert::lin2lin<t_float, IN_CENTER, IN_RIGHT>(v, OUT_CENTER, OUT_RIGHT);
}

static int float_to_sint14(t_float f)
{
    return (f <= 0)
        ? convert::lin2lin<t_float, -1, 0>(f, 0x2000, 0)
        : convert::lin2lin<t_float, 0, 1>(f, 0, 0x1fff);
}

static int float_to_uint14(t_float f)
{
    return (f <= 0)
        ? convert::lin2lin<t_float, -1, 0>(f, 0, 0x2000)
        : convert::lin2lin<t_float, 0, 1>(f, 0x2000, 0x3fff);
}

class FluidSynthProperty : public Property {
public:
    using FluidFnGetter = std::function<t_float(fluid_synth_t*)>;
    using FluidFnSetter = std::function<bool(fluid_synth_t*, t_float)>;

private:
    fluid_synth_t* synth_;
    FluidFnGetter getter_;
    FluidFnSetter setter_;

public:
    FluidSynthProperty(const std::string& name, fluid_synth_t* synth, FluidFnGetter getter, FluidFnSetter setter)
        : Property(PropertyInfo(name, PropValueType::FLOAT))
        , synth_(synth)
        , getter_(getter)
        , setter_(setter)
    {
        if (!setter_)
            setReadOnly();
    }

    AtomList get() const override
    {
        if (!synth_) {
            PROP_ERR() << "null synth";
            return {};
        }

        return { getter_(synth_) };
    }

    bool getFloat(t_float& v) const override
    {
        if (!synth_)
            return false;

        v = getter_(synth_);
        return true;
    }

    bool setFloat(t_float v) override
    {
        if (!synth_) {
            PROP_ERR() << "null synth";
            return false;
        }

        return setter_(synth_, v);
    }

    bool setList(const AtomListView& lst) override
    {
        if (!lst.isFloat()) {
            PROP_ERR() << "float value expected, got: " << lst;
            return false;
        }

        return setFloat(lst.asT<t_float>());
    }
};

Fluid::Fluid(const PdArgs& args)
    : SoundExternal(args)
    , synth_(nullptr)
    , sound_font_(&s_)
{
    createSignalOutlet();
    createSignalOutlet();

    fluid_settings_t* settings = new_fluid_settings();
    if (settings == nullptr) {
        OBJ_ERR << "couldn't create synth settings";
        return;
    }

    // settings:
    fluid_settings_setnum(settings, "synth.midi-channels", 16);

    // Create fluidsynth instance:
    synth_ = new_fluid_synth(settings);

    if (synth_ == nullptr)
        OBJ_ERR << "couldn't create synth";

    createCbSymbolProperty(
        "@sf", [this]() -> t_symbol* { return sound_font_; },
        [this](t_symbol* s) -> bool { return propSetSoundFont(s); })
        ->setArgIndex(0);

    createCbSymbolProperty("@version",
        []() -> t_symbol* { return gensym(FLUIDSYNTH_VERSION); });

    createCbProperty("@soundfonts", &Fluid::propSoundFonts);

    auto reverb_room = new FluidSynthProperty(
        "@reverb_room", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_reverb_roomsize(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            return fluid_synth_set_reverb_roomsize(synth, v) == FLUID_OK;
        });
    addProperty(reverb_room);

    auto reverb_damp = new FluidSynthProperty(
        "@reverb_damp", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_reverb_damp(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            return fluid_synth_set_reverb_damp(synth, v) == FLUID_OK;
        });
    addProperty(reverb_damp);

    auto reverb_width = new FluidSynthProperty(
        "@reverb_width", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_reverb_width(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            return fluid_synth_set_reverb_width(synth, v) == FLUID_OK;
        });
    addProperty(reverb_width);

    auto reverb_level = new FluidSynthProperty(
        "@reverb_level", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_reverb_level(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            return fluid_synth_set_reverb_level(synth, v) == FLUID_OK;
        });
    addProperty(reverb_level);

    // @gain
    auto gain = new FluidSynthProperty(
        "@gain", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_gain(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            fluid_synth_set_gain(synth, v);
            return true;
        });

    if (gain->infoT().setConstraints(PropValueConstraints::CLOSED_RANGE))
        (void)gain->infoT().setRangeFloat(0, 10);

    addProperty(gain);

    // @volume
    auto volume = new FluidSynthProperty(
        "@volume", synth_,
        [](fluid_synth_t* synth) -> t_float { return convert::amp2dbfs(fluid_synth_get_gain(synth)); },
        [](fluid_synth_t* synth, t_float v) -> bool {
            fluid_synth_set_gain(synth, convert::dbfs2amp(v));
            return true;
        });
    volume->setUnits(PropValueUnits::DB);
    addProperty(volume);

    auto polyphony = new FluidSynthProperty(
        "@poly", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_polyphony(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            return fluid_synth_set_polyphony(synth, v) == FLUID_OK;
        });

    if (polyphony->infoT().setConstraints(PropValueConstraints::CLOSED_RANGE))
        (void)polyphony->infoT().setRangeFloat(1, 1024);

    addProperty(polyphony);

    {
        auto p = new FluidSynthProperty(
            "@bufsize", synth_,
            [](fluid_synth_t* synth) -> t_float {
                return fluid_synth_get_internal_bufsize(synth);
            },
            nullptr);
        p->setUnits(PropValueUnits::SAMP);
        addProperty(p);
    }

    {
        auto p = new FluidSynthProperty(
            "@avoices", synth_,
            [](fluid_synth_t* synth) -> t_float {
                return fluid_synth_get_active_voice_count(synth);
            },
            nullptr);

        addProperty(p);
    }

    createCbIntProperty("@n", [this]() {
        return (!synth_) ? 0 : fluid_synth_count_midi_channels(synth_);
    });

    bindMidiParser();
}

Fluid::~Fluid()
{
    if (synth_) {
        fluid_settings_t* settings = fluid_synth_get_settings(synth_);
        delete_fluid_settings(settings);
    }

    delete_fluid_synth(synth_);
}

void Fluid::initDone()
{
    SoundExternal::initDone();

    // load default soundfont
    if (sound_font_ == &s_)
        propSetSoundFont(gensym(DEFAULT_SOUNDFONT));
}

void Fluid::onList(const AtomList& lst)
{
    m_note(&s_, lst);
}

void Fluid::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);

    if (synth_)
        fluid_synth_set_sample_rate(synth_, samplerate());
}

bool Fluid::propSetSoundFont(t_symbol* s)
{
    if (!synth_) {
        OBJ_ERR << "NULL synth";
        return false;
    }

    std::string filename = findInStdPaths((s == gensym("default"))
            ? DEFAULT_SOUNDFONT
            : s->s_name);

    if (filename.empty()) {
        filename = platform::find_in_exernal_dir(owner(), s->s_name);

        if (filename.empty()) {
            OBJ_ERR << "soundfont is not found: " << s;
            return false;
        }
    }

    if (fluid_synth_sfload(synth_, filename.c_str(), 0) >= 0) {

        OBJ_DBG << "loaded soundfont: " << filename;
        fluid_synth_program_reset(synth_);

        sound_font_ = s;
    } else {
        OBJ_ERR << "can't load soundfont: " << s;
        return false;
    }

    return true;
}

AtomList Fluid::propSoundFonts() const
{
    AtomList res;
    const int N = fluid_synth_sfcount(synth_);

    for (unsigned int i = 0; int(i) < N; i++) {
        fluid_sfont_t* sf = fluid_synth_get_sfont(synth_, i);
        const char* name = fluid_sfont_get_name(sf);
        res.append(Atom(gensym(name)));
    }

    return res;
}

void Fluid::m_note(t_symbol* s, const AtomListView& lv)
{
    auto res = midiByteValue3(s, "NOTE", "VEL", lv);
    if (!res.ok)
        return;

    // play on first channel
    if (res.chan == -1)
        res.chan = 0;

    auto fn = [&res](fluid_synth_t* synth, int chan, uint8_t val) -> bool {
        return fluid_synth_noteon(synth, chan, res.n, val) == FLUID_OK;
    };
    callFluidChannelFn(s, res.chan, fn, res.value, "note", lv);
}

void Fluid::m_cc(t_symbol* s, const AtomListView& lv)
{
    const auto res = midiByteValue3(s, "CC", "VAL", lv);
    if (!res.ok)
        return;

    auto fn = [&res](fluid_synth_t* synth, int chan, uint8_t val) -> bool {
        return fluid_synth_cc(synth, chan, res.n, val) == FLUID_OK;
    };
    callFluidChannelFn(s, res.chan, fn, res.value, "cc", lv);
}

void Fluid::m_prog(t_symbol* s, const AtomListView& lv)
{
    const auto res = midiByteValue2(s, "PROG", lv, 0x1fff);
    if (!res.ok)
        return;

    auto fn = [](fluid_synth_t* synth, int chan, int val) -> bool {
        return fluid_synth_program_change(synth, chan, val) == FLUID_OK;
    };
    callFluidChannelFn(s, res.chan, fn, res.value, "program", lv);
}

void Fluid::m_bank(t_symbol* s, const AtomListView& lv)
{
    const auto res = midiByteValue2(s, "BANK", lv, 0x1fff);
    if (!res.ok)
        return;

    auto fn = [](fluid_synth_t* synth, int chan, int val) -> bool {
        return fluid_synth_bank_select(synth, chan, val) == FLUID_OK;
    };
    callFluidChannelFn(s, res.chan, fn, res.value, "bank", lv);
}

void Fluid::setBend(t_symbol* method, int chan, int value, const AtomListView& args)
{
    auto fn = [](fluid_synth_t* synth, int chan, int value) -> bool {
        return fluid_synth_pitch_bend(synth, chan, value) == FLUID_OK;
    };

    callFluidChannelFn(method, chan, fn, value, BEND_VALUE_NAME, args);
}

void Fluid::setPan(t_symbol* method, int chan, float fluid_pan_value, const AtomListView& args)
{
    auto fn = [](fluid_synth_t* synth, int chan, int value) -> bool {
        return fluid_synth_set_gen(synth, chan, GEN_PAN, value) == FLUID_OK;
    };

    callFluidChannelFn(method, chan, fn, fluid_pan_value, PAN_VALUE_NAME, args);
}

int Fluid::countMidiChannels() const
{
    return synth_ ? fluid_synth_count_midi_channels(synth_) : 0;
}

Fluid::MidiChanValue3 Fluid::midiByteValue3(t_symbol* method,
    const char* nName,
    const char* vName,
    const AtomListView& lv) const
{
    int chan = 0;
    int n = 0;
    int v = 0;

    if (lv.size() == 3 && lv[0].isInteger() && lv[1].isFloat() && lv[2].isFloat()) {
        chan = lv[0].asT<int>();
        n = lv[1].asT<int>();
        v = lv[2].asT<int>();
    } else if (lv.size() == 2 && lv[0].isFloat() && lv[1].isFloat()) {
        n = lv[0].asT<int>();
        v = lv[1].asT<int>();
    } else {
        METHOD_ERR(method) << fmt::format("CHAN[0..16]? {}[0..127] {}[0..127] expected, got: ", nName, vName)
                           << lv;
        return { 0, 0, 0, false };
    }

    if (chan < 0 || chan > countMidiChannels()) {
        METHOD_ERR(method) << fmt::format(
            "channel value is out of [0..{}] range: {}", countMidiChannels(), chan);

        return { 0, 0, 0, false };
    }

    if (n < 0 || n > 0x7f) {
        METHOD_ERR(method) << fmt::format("{} is out of [0..127] range: {}", nName, n);
        return { 0, 0, 0, false };
    }

    if (v < 0 || v > 0x7f) {
        METHOD_ERR(method) << fmt::format("{} is out of [0..127] range: {}", vName, v);
        return { 0, 0, 0, false };
    }

    return { chan - 1, uint8_t(n), uint16_t(v), true };
}

Fluid::MidiChanValue2 Fluid::midiByteValue2(t_symbol* method,
    const char* valueName,
    const AtomListView& lv,
    int maxValue) const
{
    int chan = 0;
    int val = 0;

    if (lv.size() == 2 && lv[0].isInteger() && lv[1].isFloat()) {
        chan = lv[0].asT<int>();
        val = lv[1].asT<int>();
    } else if (lv.size() == 1 && lv[0].isFloat()) {
        val = lv[0].asT<int>();
    } else {
        METHOD_ERR(method) << fmt::format("CHAN[0..16]? {}[0..127] expected, got: ", valueName)
                           << lv;
        return { 0, 0, false };
    }

    if (chan < 0 || chan > countMidiChannels()) {
        METHOD_ERR(method) << fmt::format(
            "channel value is out of [0..{}] range: {}", countMidiChannels(), chan);
        return { 0, 0, false };
    }

    if (val < 0 || val > maxValue) {
        METHOD_ERR(method) << fmt::format("{} is out of [0..{}] range: {}", valueName, maxValue, val);
        return { 0, 0, false };
    }

    return { chan - 1, uint16_t(val), true };
}

void Fluid::m_bend(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<int>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "(CHANNEL:i)? BEND:i[0..0x3FFF] expected, got: " << lv;
        return;
    }

    if (!checkChanValue<int>(s, BEND_VALUE_NAME, ch, 0, 0x3FFF))
        return;

    setBend(s, ch.chan, ch.value, lv);
}

void Fluid::m_bend_int(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<int>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "(CHANNEL:i)? BEND:i[-0x2000..0x1FFF] expected, got: " << lv;
        return;
    }

    if (!checkChanValue<int>(s, BEND_VALUE_NAME, ch, -0x2000, 0x1FFF))
        return;

    setBend(s, ch.chan, ch.value + 0x2000, lv);
}

void Fluid::m_bend_float(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<t_float>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "(CHANNEL:i)? BEND:i[-1..+1] expected, got: " << lv;
        return;
    }

    if (!checkChanValue<t_float>(s, BEND_VALUE_NAME, ch, -1, 1))
        return;

    setBend(s, ch.chan, float_to_uint14(ch.value), lv);
}

void Fluid::m_gen(t_symbol* s, const AtomListView& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 3 && lst[0].isFloat() && lst[1].isFloat() && lst[2].isFloat()) {
        int chan = lst[0].asInt();
        int param = lst[1].asInt();
        int value = lst[2].asInt();

        fluid_synth_set_gen(synth_, chan - 1, param, value);
    } else if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int param = lst[0].asInt();
        int value = lst[1].asInt();
        fluid_synth_set_gen(synth_, 0, param, value);
    } else {
        METHOD_ERR(s) << "CHAN PARAM VAL or PARAM VAL expected: " << lst;
    }
}

void Fluid::m_panic(t_symbol* s, const AtomListView& lv)
{
    if (synth_)
        fluid_synth_system_reset(synth_);
}

void Fluid::m_reset(t_symbol* s, const AtomListView& lv)
{
    if (!synth_)
        return;

    int chan = lv.intAt(0, 0);
    if (chan < 0 || chan > countMidiChannels()) {
        METHOD_ERR(s) << fmt::format("invalid midi channel: {}", chan);
        return;
    }

    auto fn = [](fluid_synth_t* synth, int chan, int) {
        return fluid_synth_reset_basic_channel(synth, chan) == FLUID_OK;
    };
    callFluidChannelFn(s, chan - 1, fn, 0, "reset", lv);
}

void Fluid::m_notesOff(t_symbol* s, const AtomListView& lv)
{
    if (!synth_)
        return;

    int chan = lv.intAt(0, 0);
    if (chan < 0 || chan > countMidiChannels()) {
        METHOD_ERR(s) << fmt::format("invalid midi channel: {}", chan);
        return;
    }

    auto fn = [](fluid_synth_t* synth, int chan, int) {
        return fluid_synth_all_notes_off(synth, chan) == FLUID_OK;
    };
    callFluidChannelFn(s, chan - 1, fn, 0, "notes_off", lv);
}

void Fluid::m_soundsOff(t_symbol* s, const AtomListView& lv)
{
    if (!synth_)
        return;

    int chan = lv.intAt(0, 0);
    if (chan < 0 || chan > countMidiChannels()) {
        METHOD_ERR(s) << fmt::format("invalid midi channel: {}", chan);
        return;
    }

    auto fn = [](fluid_synth_t* synth, int chan, int) {
        return fluid_synth_all_sounds_off(synth, chan) == FLUID_OK;
    };
    callFluidChannelFn(s, chan - 1, fn, 0, "sounds_off", lv);
}

void Fluid::m_sysex(t_symbol* s, const AtomListView& lv)
{
    if (!synth_)
        return;

    const auto N = lv.size();
    char data[N];
    for (size_t i = 0; i < N; i++)
        data[i] = lv[i].asT<int>();

    char small_reply[512];
    int reply_len = 512;

    auto res = fluid_synth_sysex(synth_, data, N, small_reply, &reply_len, nullptr, 0);

    if (reply_len != 0 && res == FLUID_FAILED) {
        METHOD_ERR(s) << "length";

        char big_reply[reply_len];
        auto res = fluid_synth_sysex(synth_, data, N, big_reply, &reply_len, nullptr, 0);
        if (res == FLUID_OK) {
            Atom res[reply_len];
            for (int i = 0; i < reply_len; i++)
                res[i] = big_reply[i];

            anyTo(0, gensym("sysex"), AtomListView(res, reply_len));
        }
    } else if (res == FLUID_OK) {
        METHOD_ERR(s) << "ok";

        if (reply_len > 0) {
            Atom res[reply_len];
            for (int i = 0; i < reply_len; i++)
                res[i] = small_reply[i];

            anyTo(0, gensym("sysex"), AtomListView(res, reply_len));
        } else {
            METHOD_ERR(s) << "no reply: " << reply_len;
        }
    } else
        METHOD_ERR(s) << "sysex message not handled: " << lv;
}

void Fluid::m_set_bend_sens(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<int>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "CHAN:i? BEND_SENS:f[0..64] expected, got: " << lv;
        return;
    }

    if (!checkChanValue<int>(s, BEND_SENS_VALUE_NAME, ch, 0, 64))
        return;

    auto fn = [](fluid_synth_t* synth, int chan, int val) {
        return fluid_synth_pitch_wheel_sens(synth, chan, val) == FLUID_OK;
    };
    callFluidChannelFn(s, ch.chan, fn, ch.value, BEND_SENS_VALUE_NAME, lv);
}

void Fluid::m_tune_cent(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<t_float>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "CHAN:i? TUNE:f[-100..100.0] expected, got:" << lv;
        return;
    }

    if (!checkChanValue<t_float>(s, TUNE_CENTS_VALUE_NAME, ch, -100, 100))
        return;

    auto fn = [](fluid_synth_t* synth, int chan, t_float value) {
        return fluid_synth_set_gen(synth, chan, GEN_FINETUNE, value) == FLUID_OK;
    };
    callFluidChannelFn(s, ch.chan, fn, ch.value, TUNE_CENTS_VALUE_NAME, lv);
}

void Fluid::m_tune_semi(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<t_float>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "CHAN:i? TUNE:f[-64..63.0] expected, got:" << lv;
        return;
    }

    if (!checkChanValue<t_float>(s, TUNE_SEMITONES_VALUE_NAME, ch, -64, 63))
        return;

    auto fn = [](fluid_synth_t* synth, int chan, t_float value) {
        t_float semi = 0;
        const t_float cents = std::modf(value, &semi) * 100;

        return fluid_synth_set_gen(synth, chan, GEN_COARSETUNE, semi) == FLUID_OK
            && fluid_synth_set_gen(synth, chan, GEN_FINETUNE, cents) == FLUID_OK;
    };
    callFluidChannelFn(s, ch.chan, fn, ch.value, TUNE_CENTS_VALUE_NAME, lv);
}

void Fluid::m_tune_octave(t_symbol* s, const AtomListView& lv)
{
    static ArgChecker chk("i i f f f f f f f f f f f f");

    if (!synth_)
        return;

    Error err(this);
    chk.setOut(err);
    if (!chk.check(lv))
        return;

    int tune_bank = lv[0].asInt();
    int tune_prog = lv[1].asInt();
    t_symbol* name = gensym("custom tuning");
    double pitches[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    for (size_t i = 0; i < 12 && (i + 2) < lv.size(); i++)
        pitches[i] = lv[i + 2].asT<t_float>();

    auto rc = fluid_synth_activate_octave_tuning(synth_, tune_bank, tune_prog, name->s_name, pitches, 1);
    if (rc != FLUID_OK) {
        METHOD_ERR(s) << "can't set tuning: " << lv;
        return;
    }

    selectTune(tune_bank, tune_prog);
}

void Fluid::m_tune_select(t_symbol* s, const AtomListView& lv)
{
    int bank = lv.intAt(0, 0);
    int prog = lv.intAt(1, 0);

    selectTune(bank, prog);
}

void Fluid::m_pan(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<int>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "(CHAN:i)? PAN:f[0..+0x3fff] expected, got: " << lv;
        return;
    }

    if (!checkChanValue<int>(s, PAN_VALUE_NAME, ch, 0, 0x3fff))
        return;

    setPan(s, ch.chan, pan_to_fluid<0, 0x2000, 0x3fff>(ch.value), lv);
}

void Fluid::m_pan_float(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<t_float>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "(CHAN:i)? PAN:f[-1..+1] expected, got: " << lv;
        return;
    }

    if (!checkChanValue<t_float>(s, PAN_VALUE_NAME, ch, -1, 1))
        return;

    setPan(s, ch.chan, pan_to_fluid<-1, 0, 1>(ch.value), lv);
}

void Fluid::m_pan_int(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<int>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "(CHAN:i)? PAN:f[-8192..+8191] expected, got: " << lv;
        return;
    }

    if (!checkChanValue<int>(s, PAN_VALUE_NAME, ch, -0x2000, 0x1fff))
        return;

    setPan(s, ch.chan, pan_to_fluid<-0x2000, 0, 0x1fff>(ch.value), lv);
}

void Fluid::m_hold_pedal(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<int>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "(CHAN:i)? VAL(0|1) expected, got: " << lv;
        return;
    }

    if (!checkChanValue<int>(s, HOLD_VALUE_NAME, ch, 0, 1))
        return;

    auto fn = [](fluid_synth_t* synth, int chan, bool on) {
        return fluid_synth_cc(synth, chan, CC_HOLD_PEDAL, on ? 127 : 0) == FLUID_OK;
    };
    callFluidChannelFn(s, ch.chan, fn, ch.value, HOLD_VALUE_NAME, lv);
}

void Fluid::m_sostenuto_pedal(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<int>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "(CHAN:i)? VAL(0|1) expected, got: " << lv;
        return;
    }

    if (!checkChanValue<int>(s, SOSTENUTO_VALUE_NAME, ch, 0, 1))
        return;

    auto fn = [](fluid_synth_t* synth, int chan, bool on) {
        return fluid_synth_cc(synth, chan, CC_SOSTENUTO_PEDAL, on ? 127 : 0) == FLUID_OK;
    };
    callFluidChannelFn(s, ch.chan, fn, ch.value, SOSTENUTO_VALUE_NAME, lv);
}

void Fluid::m_soft_pedal(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<int>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "(CHAN:i)? VAL(0|1) expected, got: " << lv;
        return;
    }

    if (!checkChanValue<int>(s, SOFT_VALUE_NAME, ch, 0, 1))
        return;

    auto fn = [](fluid_synth_t* synth, int chan, bool on) {
        return fluid_synth_cc(synth, chan, CC_SOFT_PEDAL, on ? 127 : 0) == FLUID_OK;
    };
    callFluidChannelFn(s, ch.chan, fn, ch.value, SOFT_VALUE_NAME, lv);
}

void Fluid::m_legato_pedal(t_symbol* s, const AtomListView& lv)
{
    const auto ch = channelValue<int>(lv);
    if (!ch.ok) {
        METHOD_ERR(s) << "(CHAN:i)? VAL(0|1) expected, got: " << lv;
        return;
    }

    if (!checkChanValue<int>(s, LEGATO_VALUE_NAME, ch, 0, 1))
        return;

    auto fn = [](fluid_synth_t* synth, int chan, bool on) {
        return fluid_synth_cc(synth, chan, CC_LEGATO_PEDAL, on ? 127 : 0) == FLUID_OK;
    };
    callFluidChannelFn(s, ch.chan, fn, ch.value, LEGATO_VALUE_NAME, lv);
}

void Fluid::m_midi(t_symbol* s, const AtomListView& lv)
{
    for (auto& byte : lv) {
        if (byte.isFloat()) {
            auto res = midi_parser_.push(byte.asT<int>());
            if (res.err != midi::MidiParser::NO_ERROR)
                METHOD_ERR(s) << res.errStr();
        }
    }
}

void Fluid::dump() const
{
    SoundExternal::dump();

    const int N = fluid_synth_sfcount(synth_);

    OBJ_DBG << " presets:";
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 128; j++) {
            fluid_preset_t* preset = NULL;
            fluid_sfont_t* sf = NULL;

            for (int k = 0; k < N; k++) {
                sf = fluid_synth_get_sfont(synth_, k);
                preset = fluid_sfont_get_preset(sf, i, j);

                if (preset != NULL)
                    break;
            }

            if (preset != NULL) {
                const char* sf_name = fluid_sfont_get_name(sf);
                const char* preset_name = fluid_preset_get_name(preset);

                post("  '%s': soundfont '%s', bank %d, program %d",
                    preset_name, sf_name, i, j);
            }
        }
    }

    int NMIDI = fluid_synth_count_midi_channels(synth_);
    OBJ_DBG << " channels:";

    for (int i = 0; i < NMIDI; i++) {
        fluid_preset_t* preset = fluid_synth_get_channel_preset(synth_, i);

        if (preset != NULL) {
            const char* preset_name = fluid_preset_get_name(preset);
            int sf_id;
            int bank_num;
            int prog_num;
            fluid_sfont_t* sf;

            fluid_synth_get_program(synth_, i, &sf_id, &bank_num, &prog_num);
            sf = fluid_synth_get_sfont_by_id(synth_, sf_id);

            post("  %d: soundfont '%s', bank %d, program %d: '%s'",
                i + 1, fluid_sfont_get_name(sf), bank_num, prog_num, preset_name);
        } else
            post("  channel %d: no preset", i + 1);
    }

    OBJ_DBG << "gain: " << fluid_synth_get_gain(synth_);
}

void Fluid::selectTune(int bank, int prog)
{
    if (!synth_)
        return;

    if (FLUID_OK != fluid_synth_activate_tuning(synth_, 0, bank, prog, 1))
        OBJ_ERR << "can't select tuning: " << bank << ':' << prog;
}

void Fluid::bindMidiParser()
{
    midi_parser_.setNoteOnFn([this](int chan, int note, int vel) {
        if (synth_)
            fluid_synth_noteon(synth_, chan, note, vel);
    });

    midi_parser_.setNoteOffFn([this](int chan, int note, int) {
        if (synth_)
            fluid_synth_noteoff(synth_, chan, note);
    });

    midi_parser_.setControlChangeFn([this](int chan, int cc, int val) {
        if (!synth_)
            return;

        if (FLUID_OK != fluid_synth_cc(synth_, chan, cc, val))
            OBJ_ERR << "CC failed: " << chan << ' ' << cc << ' ' << val;
    });

    midi_parser_.setAfterTouchFn([this](int chan, int val) {
        if (synth_)
            fluid_synth_channel_pressure(synth_, chan, val);
    });

    midi_parser_.setPolyTouchFn([this](int chan, int key, int val) {
        if (synth_)
            fluid_synth_key_pressure(synth_, chan, key, val);
    });

    midi_parser_.setProgramChangeFn([this](int chan, int prog) {
        if (synth_)
            fluid_synth_program_change(synth_, chan, prog);
    });

    midi_parser_.setPitchWheelFn([this](int chan, int msb, int lsb) {
        if (synth_)
            fluid_synth_pitch_bend(synth_, chan, (msb << 7) | lsb);
    });
}

void Fluid::processBlock(const t_sample** in, t_sample** out)
{
    if (synth_ == nullptr)
        return;

    const auto bs = blockSize();

#if PD_FLOATSIZE == 32
    float* left = out[0];
    float* right = out[1];
    fluid_synth_write_float(synth_, bs, left, 0, 1, right, 0, 1);
#elif PD_FLOATSIZE == 64
    float left[bs];
    float right[bs];

    fluid_synth_write_float(synth_, bs, left, 0, 1, right, 0, 1);

    for (size_t i = 0; i < bs; i++)
        out[0][i] = left[i];

    for (size_t i = 0; i < bs; i++)
        out[1][i] = right[i];
#endif
}

void setup_misc_fluid()
{
    SoundExternalFactory<Fluid> obj("fluid~", OBJECT_FACTORY_DEFAULT);

    obj.addMethod("note", &Fluid::m_note);
    obj.addMethod("cc", &Fluid::m_cc);
    obj.addMethod("prog", &Fluid::m_prog);
    obj.addMethod("bank", &Fluid::m_bank);

    obj.addMethod("bend", &Fluid::m_bend);
    obj.addMethod("bend:i", &Fluid::m_bend_int);
    obj.addMethod("bend:f", &Fluid::m_bend_float);
    obj.addMethod(M_BEND_SENSIVITY, &Fluid::m_set_bend_sens);

    obj.addMethod("gen", &Fluid::m_gen);
    obj.addMethod("panic", &Fluid::m_panic);
    obj.addMethod("reset", &Fluid::m_reset);
    obj.addMethod(M_ALL_NOTES_OFF, &Fluid::m_notesOff);
    obj.addMethod(M_ALL_SOUND_OFF, &Fluid::m_soundsOff);
    obj.addMethod("sysex", &Fluid::m_sysex);
    obj.addMethod("midi", &Fluid::m_midi);

    obj.addMethod("tunesel", &Fluid::m_tune_select);
    obj.addMethod("tune:c", &Fluid::m_tune_cent);
    obj.addMethod(M_TUNE_SEMITONES, &Fluid::m_tune_semi);
    obj.addMethod("tune:12", &Fluid::m_tune_octave);

    obj.addMethod("pan", &Fluid::m_pan);
    obj.addMethod(M_PAN_POSITION_FLOAT, &Fluid::m_pan_float);
    obj.addMethod(M_PAN_POSITION_INT, &Fluid::m_pan_int);

    obj.addMethod(M_HOLD_PEDAL, &Fluid::m_hold_pedal);
    obj.addMethod(M_SOSTENUTO_PEDAL, &Fluid::m_sostenuto_pedal);
    obj.addMethod(M_SOFT_PEDAL, &Fluid::m_soft_pedal);
    obj.addMethod("legato", &Fluid::m_legato_pedal);
}
