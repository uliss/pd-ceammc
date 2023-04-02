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
#include "args/argcheck2.h"
#include "ceammc_clock.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "stok_solo_data.h"

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

class PieceStokhausenSolo : public faust_piece_stok_solo_tilde {
    ClockLambdaFunction clock_;
    std::vector<UIProperty*> cycles_;
    UIProperty *mic1_, *mic2_;
    UIProperty *fb1_, *fb2_;
    UIProperty *out1_, *out2_;
    IntProperty* scheme_idx_;
    solo::Scheme scheme_;
    solo::SoloEventList events_;

public:
    PieceStokhausenSolo(const PdArgs& args)
        : clock_([this]() {
            if (events_.isValidCurrent()) {
                onEvent(*events_.currentPtr());

                events_.moveSame([this](const solo::SoloEvent& ev) { onEvent(ev); });

                if (events_.isValidNext()) {
                    clock_.delay(events_.timeToNextEvent());
                    events_.moveNext({});
                }
            }
        })
        , faust_piece_stok_solo_tilde(args)
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
            OBJ_ERR << "can't init section properties";

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
            syncTimeLine();
        });
        addProperty(scheme_idx_);

        scheme_.setScheme(SCHEME_DEFAULT);
        syncTimeLine();

        createCbFloatProperty("@total_length", [this]() -> t_float { return scheme_.schemeLength(); });
    }

    void onEvent(const solo::SoloEvent& ev)
    {
        using namespace solo;
        switch (ev.track()) {
            //        case EVENT_PART_1
        }
    }

    void syncTimeLine()
    {
        using namespace solo;

        for (auto& t : scheme_.tracks()) {
            float pos = 0;
            for (auto& p : t) {
                pos += p.fullLengthTime();
                addPeriodToTimeLine(t.track, p, pos + p.offsetTime());
            }
        }
    }

    void addPeriodToTimeLine(solo::SoloEventTrack part, const solo::Period& p, double pos)
    {
        using namespace solo;

        switch (p.event) {
        case solo::EVENT_OFF: {
            events_.add(SoloEvent(part, SOLO_EVENT_OFF, pos));
        } break;
        case solo::EVENT_ON: {
            events_.add(SoloEvent(part, SOLO_EVENT_ON, pos));
        } break;
        default:
            break;
        }
    }

    void m_start(t_symbol* s, const AtomListView& lv)
    {
        const auto on = lv.boolAt(0, true);
        if (on) {
            events_.reset();
            clock_.exec();
        } else
            clock_.unset();
    }

    void m_stop(t_symbol* s, const AtomListView& lv)
    {
        const auto off = lv.boolAt(0, true);
        if (off) {
            clock_.unset();
        } else {
            events_.reset();
            clock_.exec();
        }
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
            cycle = lv.asT<int>();
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

    void setMic(t_float a, t_float b)
    {
        if (mic1_)
            mic1_->setValue(a, true);

        if (mic2_)
            mic2_->setValue(a, true);
    }

    void setFb(t_float a, t_float b)
    {
        if (fb1_)
            fb1_->setValue(a, true);

        if (fb2_)
            fb2_->setValue(a, true);
    }

    void setOut(t_float a, t_float b)
    {
        if (out1_)
            out1_->setValue(a, true);

        if (out2_)
            out2_->setValue(a, true);
    }

    void dump() const
    {
        OBJ_POST << "absolute events:";
        int idx = 0;
        for (auto& e : events_.data()) {
            OBJ_POST << "    [" << (idx++) << "] " << /* e.toString() <<*/ " " << e.absTimeMsec();
        }

        BaseObject::dump();
    }
};

void setup_piece_stok_solo()
{
    SoundExternalFactory<PieceStokhausenSolo> obj("piece.stok_solo~");
    obj.addMethod("cycle", &PieceStokhausenSolo::m_cycle);
    obj.addMethod("period", &PieceStokhausenSolo::m_period);
    obj.addMethod("start", &PieceStokhausenSolo::m_start);
    obj.addMethod("stop", &PieceStokhausenSolo::m_stop);
}
