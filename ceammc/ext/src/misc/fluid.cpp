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

#include "fluidsynth.h"
#include "sfloader/fluid_sfont.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

constexpr const char* DEFAULT_SOUNDFONT = "sf2/WaveSine.sf2";

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
    , reverb_room_(nullptr)
    , reverb_damp_(nullptr)
    , reverb_width_(nullptr)
    , reverb_level_(nullptr)
    , gain_(nullptr)
    , polyphony_(nullptr)
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

    reverb_room_ = new FluidSynthProperty(
        "@reverb_room", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_reverb_roomsize(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            return fluid_synth_set_reverb_roomsize(synth, v) == FLUID_OK;
        });
    addProperty(reverb_room_);

    reverb_damp_ = new FluidSynthProperty(
        "@reverb_damp", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_reverb_damp(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            return fluid_synth_set_reverb_damp(synth, v) == FLUID_OK;
        });
    addProperty(reverb_damp_);

    reverb_width_ = new FluidSynthProperty(
        "@reverb_width", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_reverb_width(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            return fluid_synth_set_reverb_width(synth, v) == FLUID_OK;
        });
    addProperty(reverb_width_);

    reverb_level_ = new FluidSynthProperty(
        "@reverb_level", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_reverb_level(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            return fluid_synth_set_reverb_level(synth, v) == FLUID_OK;
        });
    addProperty(reverb_level_);

    gain_ = new FluidSynthProperty(
        "@gain", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_gain(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            fluid_synth_set_gain(synth, v);
            return true;
        });

    if (gain_->infoT().setConstraints(PropValueConstraints::CLOSED_RANGE))
        (void)gain_->infoT().setRangeFloat(0, 10);

    addProperty(gain_);

    polyphony_ = new FluidSynthProperty(
        "@poly", synth_,
        [](fluid_synth_t* synth) -> t_float {
            return fluid_synth_get_polyphony(synth);
        },
        [](fluid_synth_t* synth, t_float v) -> bool {
            return fluid_synth_set_polyphony(synth, v) == FLUID_OK;
        });

    if (polyphony_->infoT().setConstraints(PropValueConstraints::CLOSED_RANGE))
        (void)polyphony_->infoT().setRangeFloat(1, 1024);

    addProperty(polyphony_);

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

    if (synth_) {
        fluid_synth_all_sounds_off(synth_, -1);
        fluid_synth_set_sample_rate(synth_, samplerate());
    }
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

void Fluid::m_note(t_symbol* s, const AtomListView& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 3 && lst[0].isFloat() && lst[1].isFloat() && lst[2].isFloat()) {
        int chan = lst[0].asInt();
        int key = lst[1].asInt();
        int vel = lst[2].asInt();

        fluid_synth_noteon(synth_, chan - 1, key, vel);
    } else if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int key = lst[0].asInt();
        int vel = lst[1].asInt();
        fluid_synth_noteon(synth_, 0, key, vel);
    } else {
        METHOD_ERR(s) << "CHAN KEY VEL or KEY VEL expected: " << lst;
    }
}

void Fluid::m_cc(t_symbol* s, const AtomListView& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 3 && lst[0].isFloat() && lst[1].isFloat() && lst[2].isFloat()) {
        int chan = lst[0].asInt();
        int key = lst[1].asInt();
        int vel = lst[2].asInt();

        fluid_synth_cc(synth_, chan, key, vel);
    } else if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int key = lst[0].asInt();
        int vel = lst[1].asInt();
        fluid_synth_cc(synth_, 0, key, vel);
    } else {
        METHOD_ERR(s) << "CHAN CCNUM VAL or CCNUM VAL expected: " << lst;
    }
}

void Fluid::m_prog(t_symbol* s, const AtomListView& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int chan = lst[0].asInt();
        int program = lst[1].asInt();
        fluid_synth_program_change(synth_, chan - 1, program);
    } else {
        METHOD_ERR(s) << "CHAN PROGRAM expected: " << lst;
    }
}

void Fluid::m_bank(t_symbol* s, const AtomListView& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int chan = lst[0].asInt();
        int bank = lst[1].asInt();
        fluid_synth_bank_select(synth_, chan - 1, bank);

        int sf_id;
        int bank_num;
        int prog_num;

        fluid_synth_get_program(synth_, chan - 1, &sf_id, &bank_num, &prog_num);
        fluid_synth_program_change(synth_, chan - 1, prog_num);
    } else {
        METHOD_ERR(s) << "CHAN BANK expected: " << lst;
    }
}

void Fluid::m_bend(t_symbol* s, const AtomListView& lst)
{
    if (synth_ == nullptr)
        return;

    if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        int chan = lst[0].asInt();
        int val = lst[1].asInt();
        fluid_synth_pitch_bend(synth_, chan - 1, val);
    } else {
        METHOD_ERR(s) << "CHAN VAL expected: " << lst;
    }
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

void Fluid::m_panic(t_symbol* s, const AtomListView& lst)
{
    if (synth_ == nullptr)
        return;

    fluid_synth_system_reset(synth_);
}

void Fluid::m_reset(t_symbol* s, const AtomListView& lst)
{
    if (synth_ == nullptr)
        return;

    int n = fluid_synth_count_midi_channels(synth_);

    for (int i = 0; i < n; i++)
        fluid_synth_reset_basic_channel(synth_, i);
}

void Fluid::m_notesOff(t_symbol* s, const AtomListView& lst)
{
    if (synth_ == nullptr)
        return;

    int chan = lst.floatAt(0, 0);
    fluid_synth_all_notes_off(synth_, chan - 1);
}

void Fluid::m_soundsOff(t_symbol* s, const AtomListView& lst)
{
    if (synth_ == nullptr)
        return;

    int chan = lst.floatAt(0, 0);
    fluid_synth_all_sounds_off(synth_, chan - 1);
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

void Fluid::m_get_bend_sens(t_symbol* s, const AtomListView& lv)
{
    constexpr int ALL_CHANNELS = -1;

    if (synth_ == nullptr)
        return;

    // by default: first channel
    int chan = lv.intAt(0, 1) - 1;
    int sens = 0;
    const int n = fluid_synth_count_midi_channels(synth_);

    if (chan == ALL_CHANNELS) {
        int sens = 0;
        Atom res[n];

        for (int i = 0; i < n; i++) {
            if (fluid_synth_get_pitch_wheel_sens(synth_, i, &sens) == FLUID_FAILED) {
                METHOD_ERR(s) << "can't get pitch bend sensivity from channel [" << i << "]";
                return;
            }

            res[i] = sens;
        }

        anyTo(0, gensym("bendsens"), AtomListView(res, n));

    } else if (chan >= 0 && chan < n) {
        if (fluid_synth_get_pitch_wheel_sens(synth_, chan, &sens) == FLUID_FAILED) {
            METHOD_ERR(s) << "can't get pitch bend sensivity for channel [" << chan << "]";
            return;
        }

        Atom val(sens);
        anyTo(0, gensym("bendsens"), val);
    } else {
        METHOD_ERR(s) << "invalid channel: " << chan;
    }
}

void Fluid::m_set_bend_sens(t_symbol* s, const AtomListView& lv)
{
    constexpr int ALL_CHANNELS = -1;

    if (synth_ == nullptr)
        return;

    // by default: first channel
    int chan = lv.intAt(0, 1) - 1;
    int val = lv.intAt(1, 0);
    const int n = fluid_synth_count_midi_channels(synth_);

    if (chan == ALL_CHANNELS) {
        for (int i = 0; i < n; i++) {
            if (fluid_synth_pitch_wheel_sens(synth_, chan, val) != FLUID_OK) {
                METHOD_ERR(s) << "can't set pitch bend sensivity for channel [" << i << "]: " << val;
                return;
            }
        }
    } else if (chan >= 0 && chan < n) {
        if (fluid_synth_pitch_wheel_sens(synth_, chan, val) != FLUID_OK) {
            METHOD_ERR(s) << "can't set pitch bend sensivity for channel [" << chan << "]: " << val;
            return;
        }
    } else {
        METHOD_ERR(s) << "invalid channel: " << chan;
    }
}

void Fluid::m_tune_cent(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT)) {
        METHOD_ERR(s) << "CHANNEL:int TUNE(cents:float) expected, got: " << lv;
        return;
    }

    if (!synth_)
        return;

    const auto chan = lv.intAt(0, 0);
    const auto value = lv.floatAt(1, 0);

    if (chan < 0 || chan > 16) {
        METHOD_ERR(s) << "channel in [0..16] range expected, got: " << chan;
        return;
    }

    if (value < -100 || value > 100) {
        METHOD_ERR(s) << "tune in [-100..100.0] range expected, got: " << chan;
        return;
    }

    if (FLUID_OK != fluid_synth_set_gen(synth_, chan, GEN_FINETUNE, value)) {
        METHOD_ERR(s) << "can't set fine tuning: " << chan << ' ' << value;
        return;
    }
}

void Fluid::m_tune_semi(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT)) {
        METHOD_ERR(s) << "CHANNEL:int TUNE(semitones:float) expected, got: " << lv;
        return;
    }

    if (!synth_)
        return;

    const auto chan = lv.intAt(0, 0);
    const t_float value = lv.floatAt(1, 0);

    if (chan < 0 || chan > 16) {
        METHOD_ERR(s) << "channel in [0..16] range expected, got: " << chan;
        return;
    }

    if (value < -64 || value > 63) {
        METHOD_ERR(s) << "tune in [-64..63] range expected, got: " << chan;
        return;
    }

    t_float semi = 0;
    const t_float cents = std::modf(value, &semi) * 100;

    if (FLUID_OK != fluid_synth_set_gen(synth_, chan, GEN_COARSETUNE, semi)
        || FLUID_OK != fluid_synth_set_gen(synth_, chan, GEN_FINETUNE, cents)) {
        METHOD_ERR(s) << "can't set semitone tuning: " << lv;
        return;
    }
}

void Fluid::m_tune_set_octave(t_symbol* s, const AtomListView& lv)
{
    static ArgChecker chk("i i s f f f f f f f f f f f f b?");

    if (!synth_)
        return;

    Error err(this);
    chk.setOut(err);
    if (!chk.check(lv))
        return;

    int tune_bank = lv[0].asInt();
    int tune_prog = lv[1].asInt();
    t_symbol* name = lv[2].asSymbol(gensym("unknown"));
    double pitches[12] = { 0 };
    for (size_t i = 0; i < 12 && (i + 3) < lv.size(); i++)
        pitches[i] = lv[i + 3].asT<t_float>();

    for (auto& p : pitches)
        OBJ_DBG << p;

    auto rc = fluid_synth_activate_octave_tuning(synth_, tune_bank, tune_prog, name->s_name, pitches, 1);
    if (rc != FLUID_OK) {
        METHOD_ERR(s) << "can't set tuning: " << lv;
        return;
    }

    if (lv.boolAt(15, false))
        select_tune(tune_bank, tune_prog);
}

void Fluid::m_tune_select(t_symbol* s, const AtomListView& lv)
{
    int bank = lv.intAt(0, 0);
    int prog = lv.intAt(1, 0);

    select_tune(bank, prog);
}

void Fluid::m_pan(t_symbol* s, const AtomListView& lv)
{
    if (!synth_)
        return;

    const auto chan = lv.intAt(0, 0);
    const auto pan = convert::lin2lin_clip<float, -1, 1>(lv.floatAt(1, 0), -500, 500);

    if (FLUID_OK != fluid_synth_set_gen(synth_, chan, GEN_PAN, pan))
        METHOD_ERR(s) << "can't set pan: " << lv;
}

void Fluid::m_hold_pedal(t_symbol* s, const AtomListView& lv)
{
    if (!synth_)
        return;

    if (!checkArgs(lv, ARG_INT, ARG_BOOL)) {
        METHOD_ERR(s) << "CHAN VALUE(0|1) expected, got: " << lv;
        return;
    }

    const auto chan = lv.intAt(0, 0);
    const auto on = lv.boolAt(1, 0);

    if (FLUID_OK != fluid_synth_cc(synth_, chan, 0x40, on ? 127 : 0))
        METHOD_ERR(s) << "can't set hold pedal: " << lv;
}

void Fluid::m_sostenuto_pedal(t_symbol* s, const AtomListView& lv)
{
    if (!synth_)
        return;

    if (!checkArgs(lv, ARG_INT, ARG_BOOL)) {
        METHOD_ERR(s) << "CHAN VALUE(0|1) expected, got: " << lv;
        return;
    }

    const auto chan = lv.intAt(0, 0);
    const auto on = lv.boolAt(1, 0);

    if (FLUID_OK != fluid_synth_cc(synth_, chan, 0x42, on ? 127 : 0))
        METHOD_ERR(s) << "can't set sostenuto pedal: " << lv;
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

void Fluid::select_tune(int bank, int prog)
{
    if (!synth_)
        return;

    if (FLUID_OK != fluid_synth_activate_tuning(synth_, 0, bank, prog, 1))
        OBJ_ERR << "cant select tuning: " << bank << ':' << prog;
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
    obj.addMethod("gen", &Fluid::m_gen);
    obj.addMethod("panic", &Fluid::m_panic);
    obj.addMethod("reset", &Fluid::m_reset);
    obj.addMethod("notes_off", &Fluid::m_notesOff);
    obj.addMethod("sounds_off", &Fluid::m_soundsOff);
    obj.addMethod("sysex", &Fluid::m_sysex);
    obj.addMethod("midi", &Fluid::m_midi);

    obj.addMethod("bendsens?", &Fluid::m_get_bend_sens);
    obj.addMethod("bendsens", &Fluid::m_set_bend_sens);
    obj.addMethod("tune12", &Fluid::m_tune_set_octave);
    obj.addMethod("tunesel", &Fluid::m_tune_select);
    obj.addMethod("tunecent", &Fluid::m_tune_cent);
    obj.addMethod("tunesemi", &Fluid::m_tune_semi);

    obj.addMethod("pan", &Fluid::m_pan);
    obj.addMethod("hold", &Fluid::m_hold_pedal);
    obj.addMethod("sostenuto", &Fluid::m_sostenuto_pedal);
}
