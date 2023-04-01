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
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "stok_solo_data.h"

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
    std::vector<UIProperty*> cycles_;
    UIProperty *mic1_, *mic2_;
    UIProperty *fb1_, *fb2_;
    UIProperty *out1_, *out2_;
    IntProperty* scheme_idx_;
    solo::Scheme scheme_;

public:
    PieceStokhausenSolo(const PdArgs& args)
        : faust_piece_stok_solo_tilde(args)
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
        scheme_idx_->setSuccessFn([this](Property*) { scheme_.set(scheme_idx_->value()); });
        addProperty(scheme_idx_);

        scheme_.set(SCHEME_DEFAULT);
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

    void setDelays(int n[])
    {
    }
};

void setup_piece_stok_solo()
{
    SoundExternalFactory<PieceStokhausenSolo> obj("piece.stok_solo~");
    obj.addMethod("cycle", &PieceStokhausenSolo::m_cycle);
    obj.addMethod("period", &PieceStokhausenSolo::m_period);
}
