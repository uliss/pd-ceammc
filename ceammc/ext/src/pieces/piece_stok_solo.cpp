/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "piece_stok_solo.h"
#include "args/argcheck.h"
#include "ceammc_clock.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "stok_solo_data.h"

#include <boost/iostreams/stream.hpp>
#include <functional>

using namespace ceammc;

constexpr int NUM_CYCLES = 6;
constexpr int MAX_PERIODS = 11;
constexpr int NUM_CONTROLS = 6;
constexpr int SCHEME_MIN = 1;
constexpr int SCHEME_MAX = 6;
constexpr int SCHEME_DEFAULT = 2;
constexpr int CYCLE_MIN = 0;
constexpr int CYCLE_MAX = 5;

#if 1
#define SOLO_DBG OBJ_DBG
#else
#define SOLO_DBG boost::iostreams::stream<boost::iostreams::null_sink>((boost::iostreams::null_sink()))
#endif

class PieceStokhausenSolo : public faust_piece_stok_solo_tilde {
    ClockLambdaFunction clock_;
    std::vector<UIProperty*> cycles_;
    std::vector<UIProperty*> delays_;
    UIProperty *mic1_, *mic2_;
    UIProperty *fb1_, *fb2_;
    UIProperty *out1_, *out2_;
    IntProperty* scheme_idx_;
    solo::Scheme scheme_;
    solo::SoloEventList events_;
    int current_period_ { -1 };
    double start_time_ { 0 };
    bool is_running_ { false };

public:
    PieceStokhausenSolo(const PdArgs& args)
        : clock_([this]() { onClock(); })
        , faust_piece_stok_solo_tilde(args)
    {
        initCycles();
        initDelays();

        mic1_ = findUIProperty("@in1");
        mic2_ = findUIProperty("@in2");
        fb1_ = findUIProperty("@fb1.gain");
        fb2_ = findUIProperty("@fb2.gain");
        out1_ = findUIProperty("@out1.gain");
        out2_ = findUIProperty("@out2.gain");

        scheme_idx_ = new IntProperty("@scheme", SCHEME_DEFAULT);
        scheme_idx_->setArgIndex(0);
        scheme_idx_->checkClosedRange(SCHEME_MIN, SCHEME_MAX);
        scheme_idx_->setSuccessFn([this](Property*) {
            scheme_.setScheme(scheme_idx_->value());
            events_.clear();
            syncScheme();
        });
        addProperty(scheme_idx_);

        scheme_.setScheme(SCHEME_DEFAULT);
        syncScheme();

        createCbFloatProperty("@total_length", [this]() -> t_float { return scheme_.lengthSec(); });
        createCbFloatProperty("@time", [this]() -> t_float { return currentTimeMs(); });
        createCbListProperty("@info", [this]() -> AtomList {
            auto info = scheme_.atTime(currentTimeMs() * 0.001);
            return AtomList(info.phase, info.cycle, info.cycle_phase, info.cycle_period, info.period_phase);
        });

        createOutlet();
    }

    void onClock(bool scheduleNext = true)
    {
        if (events_.isValidCurrent()) {
            onEvent(*events_.currentPtr());

            events_.moveSame([this](const solo::SoloEvent& ev) { onEvent(ev); });

            if (events_.isValidNext()) {
                if (scheduleNext) {
                    clock_.delay(events_.timeToNextEvent());
                }

                events_.moveNext({});
            }
        }
    }

    void initCycles()
    {
        cycles_.reserve(NUM_CYCLES);

        for (int i = 0; i < NUM_CYCLES; i++) {
            char buf[16];
            sprintf(buf, "@cycle%d", i);
            auto prop = findUIProperty(buf);
            if (prop)
                cycles_.push_back(prop);
        }

        if (cycles_.size() != NUM_CYCLES)
            OBJ_ERR << "can't init cycles";
    }

    void initDelays()
    {
        delays_.reserve(NUM_CYCLES);

        for (int i = 0; i < NUM_CYCLES; i++) {
            char buf[16];
            sprintf(buf, "@delay%d", i);
            auto prop = findUIProperty(buf);
            if (prop)
                delays_.push_back(prop);
        }

        if (delays_.size() != NUM_CYCLES)
            OBJ_ERR << "can't init delays";
    }

    void onEvent(const solo::SoloEvent& ev)
    {
        using namespace solo;

        SOLO_DBG << ev.toString();

        switchCycle(ev.cycle());
        switchPeriod(ev.period());

        switch (ev.track()) {
        case TRACK_MIC1:
            mic1_->setValue(ev.value(), true);
            break;
        case TRACK_MIC2:
            mic2_->setValue(ev.value(), true);
            break;
        case TRACK_FB1:
            fb1_->setValue(ev.value(), true);
            break;
        case TRACK_FB2:
            fb2_->setValue(ev.value(), true);
            break;
        case TRACK_OUT1:
            out1_->setValue(ev.value(), true);
            break;
        case TRACK_OUT2:
            out2_->setValue(ev.value(), true);
            break;
        default:
            break;
        }
    }

    void syncScheme()
    {
        resetUI();
        syncDelays();
        syncTimeLine();
        events_.reset();
    }

    void syncTimeLine()
    {
        events_.clear();
        events_.addScheme(scheme_);
    }

    void syncDelays()
    {
        auto CCNT = scheme_.cycleCount();
        auto DCNT = delays_.size();
        if (CCNT != DCNT) {
            OBJ_ERR << fmt::format("number of cycles != number of delays: {}!={}", CCNT, DCNT);
            return;
        }

        for (size_t i = 0; i < CCNT; i++)
            delays_[i]->setValue(scheme_.cycles()[i].periodLength(), true);
    }

    void switchCycle(solo::SoloCycle c)
    {
        for (size_t i = 0; i < cycles_.size(); i++) {
            auto prop = cycles_[i];
            if (i == c) {
                if (prop->value() != 1) {
                    OBJ_DBG << fmt::format("switch on cycle {:c}", 'A' + i);
                    prop->setValue(1, true);
                    char buf[2] = { char('A' + i), 0 };
                    anyTo(2, gensym("cycle"), gensym(buf));
                }
            } else {
                if (prop->value() == 1) {
                    OBJ_DBG << fmt::format("switch off cycle {:c}", 'A' + i);
                    prop->setValue(0, true);
                }
            }
        }
    }

    void switchPeriod(int p)
    {
        if (current_period_ != p) {
            anyTo(2, gensym("period"), p);
            current_period_ = p;
        }
    }

    void resetState()
    {
        resetUI();
        events_.reset();
    }

    void do_start(bool on)
    {
        if (on) {
            syncScheme();
            clock_.exec();
            start_time_ = clock_getlogicaltime();
        } else
            clock_.unset();

        is_running_ = on;
    }

    void m_start(t_symbol* s, const AtomListView& lv)
    {
        do_start(lv.boolAt(0, true));
    }

    void m_stop(t_symbol* s, const AtomListView& lv)
    {
        do_start(!lv.boolAt(0, true));
    }

    void m_next(t_symbol* s, const AtomListView& lv)
    {
        onClock(false);
    }

    UIProperty* findUIProperty(const char* name)
    {
        auto res = dynamic_cast<UIProperty*>(property(name));
        if (!res)
            OBJ_ERR << fmt::format("property '{}' not found", name);

        return res;
    }

    void m_cycle(t_symbol* s, const AtomListView& lv)
    {
        auto usage = [this](t_symbol* s) { METHOD_ERR(s) << fmt::format("usage: {} CYCLE[0-5A-F]", s->s_name); };

        if (!lv.isAtom()) {
            usage(s);
            return;
        }

        int cycle = -1;
        if (lv.isInteger()) {
            cycle = lv.asT<t_int>();
            if (cycle < CYCLE_MIN || cycle > CYCLE_MAX) {
                usage(s);
                return;
            }
        } else if (lv.isSymbol()) {
            switch (crc32_hash(lv.asT<t_symbol*>())) {
            case "A"_hash:
                cycle = 0;
                break;
            case "B"_hash:
                cycle = 1;
                break;
            case "C"_hash:
                cycle = 2;
                break;
            case "D"_hash:
                cycle = 3;
                break;
            case "E"_hash:
                cycle = 4;
                break;
            case "F"_hash:
                cycle = 4;
                break;
            default:
                usage(s);
                return;
            }
        } else
            return usage(s);

        auto N = std::min<size_t>(cycles_.size(), NUM_CYCLES);
        for (size_t i = 0; i < N; i++) {
            auto p = cycles_[i];
            if (p)
                p->setValue(i == cycle ? 1 : 0, true);
        }
    }

    void m_period(t_symbol* s, const AtomListView& lv)
    {
        static const args::ArgChecker chk("PERIOD:i[0,11]");
        if (!chk.check(lv, this))
            return chk.usage(this, s);

        int period_idx = -1;
        for (size_t i = 0; i < cycles_.size(); i++) {
            auto s = cycles_[i];
            if (s && s->value() > 0) {
                period_idx = i;
                break;
            }
        }

        if (period_idx < 0) {
            METHOD_ERR(s) << "no active section found";
            return;
        }

        auto part = lv.intAt(0, -1);

        solo::Scheme scheme(2);

        switch (period_idx) {
        case 0:

            break;
        }
    }

    void m_reset(t_symbol* s, const AtomListView& lv)
    {
        faust_piece_stok_solo_tilde::m_reset(s, lv);
        syncScheme();
        do_start(false);
    }

    void dump() const
    {
        OBJ_POST << "absolute events:";
        int idx = 0;
        for (auto& e : events_.data()) {
            OBJ_POST << "    [" << (idx++) << "] " << e.toString() << " " << e.absTimeMsec();
        }

        BaseObject::dump();
    }

    float currentTimeMs() const
    {
        auto total = scheme_.lengthSec() * 1000;
        auto t = clock_gettimesince(start_time_);
        return (is_running_ && t >= 0 && t <= total) ? t : 0;
    }
};

void setup_piece_stok_solo()
{
    SoundExternalFactory<PieceStokhausenSolo> obj("piece.stok_solo~");
    obj.addMethod("cycle", &PieceStokhausenSolo::m_cycle);
    obj.addMethod("period", &PieceStokhausenSolo::m_period);
    obj.addMethod("start", &PieceStokhausenSolo::m_start);
    obj.addMethod("stop", &PieceStokhausenSolo::m_stop);
    obj.addMethod("next", &PieceStokhausenSolo::m_next);
    obj.addMethod("reset", &PieceStokhausenSolo::m_reset);
}
