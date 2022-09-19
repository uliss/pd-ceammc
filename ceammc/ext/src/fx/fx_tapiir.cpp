#include "fx_tapiir.h"
#include "ceammc_args.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_units.h"
#include "fx_tapiir_priv.h"
#include "lex/parser_music.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <random>

using namespace ceammc;

static bool is_tap_delay_prop(const char* str)
{
    return str[0] == '@'
        && str[1] == 't'
        && str[2] == 'a'
        && str[3] == 'p'
        && std::isdigit(str[4])
        && str[5] == '.'
        && str[6] == 'd'
        && str[7] == 'e'
        && str[8] == 'l'
        && str[9] == 'a'
        && str[10] == 'y'
        && str[11] == '\0';
}

static bool is_tap_gain_prop(const char* str)
{
    return str[0] == '@'
        && str[1] == 't'
        && str[2] == 'a'
        && str[3] == 'p'
        && std::isdigit(str[4])
        && str[5] == '.'
        && str[6] == 'g'
        && str[7] == 'a'
        && str[8] == 'i'
        && str[9] == 'n'
        && str[10] == '\0';
}

static bool is_tap_fb_prop(const char* str)
{
    return str[0] == '@'
        && str[1] == 't'
        && str[2] == 'a'
        && str[3] == 'p'
        && std::isdigit(str[4])
        && str[5] == '.'
        && str[6] == 'f'
        && str[7] == 'b'
        && std::isdigit(str[8])
        && str[9] == '\0';
}

static bool is_tap_output_prop(const char* str)
{
    return str[0] == '@'
        && str[1] == 'o'
        && str[2] == 'u'
        && str[3] == 't'
        && std::isdigit(str[4])
        && str[5] == '.'
        && str[6] == 't'
        && str[7] == 'a'
        && str[8] == 'p'
        && std::isdigit(str[9])
        && str[10] == '\0';
}

static bool is_tap_input_prop(const char* str)
{
    return str[0] == '@'
        && str[1] == 't'
        && str[2] == 'a'
        && str[3] == 'p'
        && std::isdigit(str[4])
        && str[5] == '.'
        && str[6] == 'i'
        && str[7] == 'n'
        && std::isdigit(str[8])
        && str[9] == '\0';
}

FxTapiir::FxTapiir(const PdArgs& args)
    : faust_fx_tapiir_tilde(args)
{
    initTapGroupProps();

    createCbListProperty(
        "@outs0", [this]() -> AtomList {
            AtomList res;
            res.reserve(NUM_TAPS);

            for (size_t i = 0; i < NUM_TAPS; i++)
                res.append(tap_outputs_[i]->value());
            return res; },
        [this](const AtomListView& lv) -> bool {
            const size_t N = std::min(NUM_TAPS, lv.size());
            for (size_t i = 0; i < N; i++) {
                if (!tap_outputs_[i]->set(lv.subView(i, 1)))
                    return false;
            }
            return true;
        });

    createCbListProperty(
        "@outs1", [this]() -> AtomList {
            AtomList res;
            res.reserve(NUM_TAPS);

            for (size_t i = 0; i < NUM_TAPS; i++)
                res.append(tap_outputs_[NUM_TAPS + i]->value());
            return res; },
        [this](const AtomListView& lv) -> bool {
            const size_t N = std::min(NUM_TAPS, lv.size());
            for (size_t i = 0; i < N; i++) {
                if (!tap_outputs_[NUM_TAPS + i]->set(lv.subView(i, 1)))
                    return false;
            }
            return true;
        });

    createCbListProperty(
        "@ins0", [this]() -> AtomList {
            AtomList res;
            res.reserve(NUM_TAPS);

            for (size_t i = 0; i < NUM_TAPS; i++)
                res.append(tap_inputs_[i]->value());
            return res; },
        [this](const AtomListView& lv) -> bool {
            const size_t N = std::min(NUM_TAPS, lv.size());
            for (size_t i = 0; i < N; i++) {
                if (!tap_inputs_[i]->set(lv.subView(i, 1)))
                    return false;
            }
            return true;
        });

    createCbListProperty(
        "@ins1", [this]() -> AtomList {
            AtomList res;
            res.reserve(NUM_TAPS);

            for (size_t i = 0; i < NUM_TAPS; i++)
                res.append(tap_inputs_[NUM_TAPS + i]->value());
            return res; },
        [this](const AtomListView& lv) -> bool {
            const size_t N = std::min(NUM_TAPS, lv.size());
            for (size_t i = 0; i < N; i++) {
                if (!tap_inputs_[NUM_TAPS + i]->set(lv.subView(i, 1)))
                    return false;
            }
            return true;
        });

    createCbListProperty(
        "@delays", [this]() -> AtomList {
            AtomList res;
            res.reserve(NUM_TAPS);
            for(auto& d: tap_delays_)
                res.append(d->value());

            return res; },
        [this](const AtomListView& lv) -> bool {
            const size_t N = std::min(NUM_TAPS, lv.size());
            for (size_t i = 0; i < N; i++) {
                if (!tap_delays_[i]->set(lv.subView(i, 1)))
                    return false;
            }
            return true;
        })
        ->setUnitsMs();

    createCbListProperty(
        "@gains", [this]() -> AtomList {
            AtomList res;
            res.reserve(NUM_TAPS);
            for(auto& d: tap_gains_)
                res.append(d->value());
            return res; },
        [this](const AtomListView& lv) -> bool {
            const size_t N = std::min(NUM_TAPS, lv.size());
            for (size_t i = 0; i < N; i++) {
                if (!tap_gains_[i]->set(lv.subView(i, 1)))
                    return false;
            }
            return true;
        })
        ->setUnitsDb();

    createTapFbProp<0>();
    createTapFbProp<1>();
    createTapFbProp<2>();
    createTapFbProp<3>();
    createTapFbProp<4>();
    createTapFbProp<5>();
}

void FxTapiir::m_random(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, ARG_FLOAT)) {
        METHOD_ERR(s) << "expected: fb|gains|inputs|outputs|delays MIN MAX";
        return;
    }

    std::mt19937 dev(time(nullptr));
    auto sel = lv[0].asSymbol();

    if (sel == gensym("delays")) {
        auto min = clip_min<t_float>(lv[1].asFloat(), 0);
        auto max = clip_min<t_float>(lv[2].asFloat(), 0);

        std::uniform_real_distribution<t_float> dist(min, max);
        for (auto p : tap_delays_)
            p->setValue(dist(dev), true);
    } else if (sel == gensym("fb")) {
        auto min = clip<t_float>(lv[1].asFloat(), 0, 0.999);
        auto max = clip<t_float>(lv[2].asFloat(), 0, 0.999);

        std::uniform_real_distribution<t_float> dist(min, max);
        for (auto p : tap_fb_)
            p->setValue(dist(dev), true);
    } else if (sel == gensym("inputs")) {
        auto min = clip<t_float>(lv[1].asFloat(), 0, 0.999);
        auto max = clip<t_float>(lv[2].asFloat(), 0, 0.999);

        std::uniform_real_distribution<t_float> dist(min, max);
        for (auto p : tap_inputs_)
            p->setValue(dist(dev), true);
    } else if (sel == gensym("gains")) {
        auto min = clip<t_float>(lv[1].asFloat(), -60, 0);
        auto max = clip<t_float>(lv[2].asFloat(), -60, 0);

        std::uniform_real_distribution<t_float> dist(min, max);
        for (auto p : tap_gains_)
            p->setValue(dist(dev), true);
    } else if (sel == gensym("outputs")) {
        auto min = clip<t_float>(lv[1].asFloat(), 0, 0.999);
        auto max = clip<t_float>(lv[2].asFloat(), 0, 0.999);

        std::uniform_real_distribution<t_float> dist(min, max);
        for (auto p : tap_outputs_)
            p->setValue(dist(dev), true);
    }
}

void FxTapiir::m_pingpong(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, ARG_FLOAT)) {
        METHOD_ERR(s) << "usage: DELAY FEEDBACK" << lv;
        return;
    }

    const auto ms = lv[0].asT<t_float>();
    if (ms < 0) {
        METHOD_ERR(s) << "invalid delays value: " << ms;
        return;
    }

    const auto fb = lv[1].asT<t_float>();
    if (fb < 0 || fb > 1) {
        METHOD_ERR(s) << "invalid feedback value: " << fb;
        return;
    }

    // set delays
    for (auto p : tap_delays_)
        p->setValue(0);

    tap_delays_[0]->setValue(ms);
    tap_delays_[1]->setValue(ms);

    // set inputs
    for (size_t i = 0; i < NUM_CHAN; i++) {
        for (size_t t = 0; t < NUM_TAPS; t++) {
            auto idx = i * NUM_TAPS + t; // see initTapGroupProps()
            if (idx > tap_inputs_.size())
                continue;

            if (t == 0 && i == 0)
                tap_inputs_[idx]->setValue(1);
            else if (t == 1 && i == 1)
                tap_inputs_[idx]->setValue(1);
            else
                tap_inputs_[idx]->setValue(0);
        }
    }

    // set feedbacks
    for (size_t fbi = 0; fbi < NUM_TAPS; fbi++) {
        for (size_t ti = 0; ti < NUM_TAPS; ti++) {
            auto idx = ti * NUM_TAPS + fbi; // see initTapGroupProps()
            if (idx > tap_fb_.size())
                continue;

            if (ti == 0 && fbi == 1)
                tap_fb_[idx]->setValue(fb);
            else if (ti == 1 && fbi == 0)
                tap_fb_[idx]->setValue(fb);
            else
                tap_fb_[idx]->setValue(0);
        }
    }

    // set outputs
    for (size_t oi = 0; oi < NUM_CHAN; oi++) {
        for (size_t ti = 0; ti < NUM_TAPS; ti++) {
            auto idx = oi * NUM_TAPS + ti; // see initTapGroupProps()
            if (idx > tap_outputs_.size())
                continue;

            if (ti == 0 && oi == 0)
                tap_outputs_[idx]->setValue(1);
            else if (ti == 1 && oi == 1)
                tap_outputs_[idx]->setValue(1);
            else
                tap_outputs_[idx]->setValue(0);
        }
    }
}

void FxTapiir::getTapFb(size_t tapn, AtomList& res)
{
    for (size_t i = 0; i < std::min(tap_fb_.size(), NUM_TAPS); i++) {
        const size_t ti = tapn * NUM_TAPS + i;
        res.append(tap_fb_[ti]->value());
    }
}

bool FxTapiir::setTapFb(size_t tapn, const AtomListView& lv)
{
    const size_t N = std::min(NUM_TAPS, lv.size());
    for (size_t i = 0; i < std::min(N, tap_fb_.size()); i++) {
        const size_t ti = tapn * NUM_TAPS + i;
        if (!tap_fb_[ti]->set(lv.subView(i, 1)))
            return false;
    }

    return true;
}

void FxTapiir::initTapGroupProps()
{
    for (auto* p : properties()) {
        const char* name = p->name()->s_name;

        if (is_tap_delay_prop(name)) {
            const size_t tapn = (name[4]) - '0';
            tap_delays_[tapn] = static_cast<UIProperty*>(p);
        } else if (is_tap_gain_prop(name)) {
            const size_t tapn = (name[4]) - '0';
            tap_gains_[tapn] = static_cast<UIProperty*>(p);
        } else if (is_tap_fb_prop(name)) {
            const size_t tapn = (name[4]) - '0';
            const size_t fbn = (name[8]) - '0';
            tap_fb_[tapn * NUM_TAPS + fbn] = static_cast<UIProperty*>(p);
        } else if (is_tap_output_prop(name)) {
            const size_t outn = (name[4]) - '0';
            const size_t tapn = (name[9]) - '0';
            tap_outputs_[outn * NUM_TAPS + tapn] = static_cast<UIProperty*>(p);
        } else if (is_tap_input_prop(name)) {
            const size_t in = (name[8]) - '0';
            const size_t tapn = (name[4]) - '0';
            tap_inputs_[in * NUM_TAPS + tapn] = static_cast<UIProperty*>(p);
        }
    }
}

void setup_fx_tapiir_tilde()
{
    SoundExternalFactory<FxTapiir> obj("fx.tapiir~");
    obj.addMethod("reset", &FxTapiir::m_reset);
    obj.addMethod("random", &FxTapiir::m_random);
    obj.addMethod("pingping", &FxTapiir::m_pingpong);
}
