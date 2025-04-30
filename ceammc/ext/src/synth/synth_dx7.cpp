#include "synth_dx7.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "synth_faust_with_freq.h"
#include "synth_rust.hpp"

#include "fmt/core.h"

using namespace ceammc;

struct OperatorProps {
    UIProperty *r1, *r2, *r3, *r4;
    UIProperty *l1, *l2, *l3, *l4;
    UIProperty *level, *vel_sens, *op_mode, *freq, *detune, *rate;
};

static t_symbol* gen_op(int i, const char* name)
{
    return gensym(fmt::format("@op{}:{}", i, name).c_str());
}

class SynthDx7 : public SynthWithFreq<faust_synth_dx7_tilde> {
    ceammc_synth_dx7_sysex* dx7_ { nullptr };
    std::vector<OperatorProps> operator_props_;

public:
    explicit SynthDx7(const PdArgs& args)
        : SynthWithFreq<faust_synth_dx7_tilde>(args)
    {
        dx7_ = ceammc_synth_dx7_sysex_new(
            { this, [](void* user, ceammc_synth_msg_level level, const char* msg) {
                 auto obj = static_cast<SynthDx7*>(user);
                 switch (level) {
                 case ceammc_synth_msg_level::DEBUG:
                     Debug(obj) << msg;
                     break;
                 case ceammc_synth_msg_level::POST:
                     Post(obj) << msg;
                     break;
                 case ceammc_synth_msg_level::ERROR:
                     Error(obj) << msg;
                     break;
                 default:
                     break;
                 }
             } });
    }

    ~SynthDx7()
    {
        ceammc_synth_dx7_sysex_free(dx7_);
    }

    void initDone()
    {
        SynthWithFreq<faust_synth_dx7_tilde>::initDone();

        for (int i = 0; i < 6; i++) {
            OperatorProps p { nullptr };
            p.r1 = findUIProperty(gen_op(i, "R1"));
            p.r2 = findUIProperty(gen_op(i, "R2"));
            p.r3 = findUIProperty(gen_op(i, "R3"));
            p.r4 = findUIProperty(gen_op(i, "R4"));

            p.l1 = findUIProperty(gen_op(i, "L1"));
            p.l2 = findUIProperty(gen_op(i, "L2"));
            p.l3 = findUIProperty(gen_op(i, "L3"));
            p.l4 = findUIProperty(gen_op(i, "L4"));

            p.level = findUIProperty(gen_op(i, "level"));
            p.vel_sens = findUIProperty(gen_op(i, "vel"));
            // p.mod_sens = findUIProperty(gen_op(i, "mod"));
            p.op_mode = findUIProperty(gen_op(i, "opMode"));
            p.freq = findUIProperty(gen_op(i, "freq"));
            p.detune = findUIProperty(gen_op(i, "detune"));
            p.rate = findUIProperty(gen_op(i, "rate"));

            operator_props_.push_back(p);
        }
    }

    void m_read_sysex(t_symbol* s, const AtomListView& lv)
    {
        auto fname = to_string(lv);
        auto path = findInStdPaths(fname.c_str());
        if (path.empty()) {
            METHOD_ERR(s) << fmt::format("file not found: '{}'", fname);
            return;
        }

        if (ceammc_synth_dx7_sysex_read_file(dx7_, path.c_str()))
            ceammc_synth_dx7_sysex_info(dx7_);
    }

    void m_load_preset(t_symbol* s, const AtomListView& lv)
    {
        static const args::ArgChecker chk("PRESET:a");
        if (!chk.check(lv, this, s))
            return chk.usage(this, s);

        if (lv.isFloat()) {
            ceammc_synth_dx7_sysex_load(dx7_, lv.intAt(0, 0), this, //
                [](void* user, //
                    const char* name, //
                    uint8_t algorithm_id, //
                    bool oscillator_key_sync, //
                    uint8_t feedback_level, //
                    uint8_t transpose) //
                {
                    auto obj = static_cast<SynthDx7*>(user);
                    obj->setProperty("algorithm", Atom(algorithm_id));
                    obj->setProperty("feedback", Atom(feedback_level)); //
                },
                [](void* user, //
                    uint8_t lfo_speed, //
                    uint8_t lfo_delay, //
                    uint8_t lfo_pitch_mod_depth, //
                    uint8_t lfo_pitch_mod_sensitivity, //
                    uint8_t lfo_amplitude_mod_depth, //
                    uint8_t lfo_waveform,
                    bool lfo_key_sync) //
                {
                    auto obj = static_cast<SynthDx7*>(user);
                    obj->setProperty("lfo_delay", Atom(lfo_delay));
                    obj->setProperty("lfo_speed", Atom(lfo_speed));
                    obj->setProperty("lfo_depth", Atom(lfo_pitch_mod_depth));
                    //
                },
                [](void* user, //
                    uint8_t levels[4], //
                    uint8_t rates[4]) //
                {},
                [](void* user, //
                    size_t idx,
                    const uint8_t* levels, //
                    const uint8_t* rates, //
                    uint8_t scaling_break_point, //
                    uint8_t scaling_left_depth, //
                    uint8_t scaling_right_depth, //
                    uint8_t scaling_left_curve, //
                    uint8_t scaling_right_curve, //
                    int8_t detune, //
                    uint8_t rate_scaling, //
                    uint8_t velocity_sensitivity, //
                    uint8_t modulation_sensitivity, //
                    uint8_t output_level, //
                    uint8_t is_fixed, //
                    uint8_t frequency_course, //
                    uint8_t frequency_fine //
                ) {
                    auto obj = static_cast<SynthDx7*>(user);
                    if (idx >= obj->operator_props_.size())
                        return;

                    auto& op = obj->operator_props_[idx];
                    op.l1->setValue(levels[0]);
                    op.l2->setValue(levels[1]);
                    op.l3->setValue(levels[2]);
                    op.l4->setValue(levels[3]);

                    op.r1->setValue(rates[0]);
                    op.r2->setValue(rates[1]);
                    op.r3->setValue(rates[2]);
                    op.r4->setValue(rates[3]);

                    op.detune->setValue(detune);
                    op.rate->setValue(rate_scaling);
                    op.vel_sens->setValue(velocity_sensitivity);
                    op.level->setValue(output_level);
                    op.op_mode->setValue(is_fixed);
                    op.freq->setValue(frequency_course);

                    //
                });
        }
    }
};

void setup_synth_dx7_tilde()
{
    SoundExternalFactory<SynthDx7> obj("synth.dx7~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("note", &SynthDx7::m_note);
    obj.addMethod("read", &SynthDx7::m_read_sysex);
    obj.addMethod("load", &SynthDx7::m_load_preset);

    obj.setDescription("FAUST dx7 implementation");
    obj.setCategory("synth");
    obj.setKeywords({ "dx7", "synth", "fm" });
}
