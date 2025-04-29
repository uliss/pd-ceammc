/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "seq_arp.h"
#include "args/argcheck.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

#include <algorithm>

CEAMMC_DEFINE_SYM_HASH(first)
CEAMMC_DEFINE_SYM_HASH(last)
CEAMMC_DEFINE_SYM_HASH(random)
CEAMMC_DEFINE_SYM_HASH(all)
CEAMMC_DEFINE_SYM_HASH(f)
CEAMMC_DEFINE_SYM_HASH(l)
CEAMMC_DEFINE_SYM_HASH(r)
CEAMMC_DEFINE_SYM_HASH(a)

SeqArp::SeqArp(const PdArgs& args)
    : BaseObject(args)
    , chord_(nullptr)
    , nactive_(0)
    , asr_clock_([this]() {
        switch (asr_data_.state) {
        case AsrData::ATTACK:
            m_on(&s_, Atom(asr_data_.on_mode));

            if (allOn()) {
                asr_data_.state = AsrData::RELEASE;
                asr_clock_.delay(asr_data_.holdDurationMs());
            } else {
                asr_clock_.delay(asr_data_.step_duration_ms);
            }

            break;
        case AsrData::RELEASE:
            m_off(&s_, Atom(asr_data_.off_mode));

            if (allOff()) {
                asr_data_.state = AsrData::DONE;
            } else
                asr_clock_.delay(asr_data_.step_duration_ms);

            break;
        default:
            break;
        }
    })
{
    on_offs_.reserve(32);

    chord_ = new ListProperty("@chord");
    chord_->setArgIndex(0);
    chord_->setAtomCheckFn([](const Atom& a) -> bool { return a.isFloat(); });
    chord_->setSuccessFn([this](Property*) {
        on_offs_.assign(chord_->value().size(), 0);
        nactive_ = 0;
        asr_clock_.unset();
    });
    addProperty(chord_);

    createOutlet();
    createOutlet();
}

void SeqArp::onBang()
{
    m_on(&s_, {});
}

void SeqArp::m_on(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("MODE:s=first|last|random|all|f|l|r|a?"
                                      " VEL:f[1,127]?"
                                      " NOTES:i>0?");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    const auto& chord = chord_->value();
    if (chord.empty()) {
        METHOD_ERR(s) << "empty chord";
        return;
    }

    const auto mode = lv.symbolAt(0, sym_first());
    const auto velocity = lv.floatAt(1, 127);
    const auto count = lv.intAt(2, 1);

    processOnOff(crc32_hash(mode), true, velocity, count);
}

void SeqArp::m_off(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("MODE:s=first|last|random|all|f|l|r|a? NOTES:i>0?");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    const auto& chord = chord_->value();
    if (chord.empty()) {
        METHOD_ERR(s) << "empty chord";
        return;
    }

    const auto mode = lv.symbolAt(0, sym_first());
    const int count = lv.intAt(1, 1);

    processOnOff(crc32_hash(mode), false, 0, count);
}

void SeqArp::m_reset(t_symbol* s, const AtomListView& lv)
{
    std::fill(on_offs_.begin(), on_offs_.end(), 0);
    nactive_ = 0;
    asr_clock_.unset();
}

void SeqArp::m_asr(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STEP_DUR:f>0"
                                      " ATTACK:s=first|last|random|all|f|l|r|a"
                                      " HOLD:i>0"
                                      " RELEASE:s=first|last|random|all|f|l|r|a");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    const auto& chord = chord_->value();
    if (chord.empty()) {
        METHOD_ERR(s) << "empty chord";
        return;
    }

    asr_data_.step_duration_ms = lv.floatAt(0, 1);
    asr_data_.on_mode = lv.symbolAt(1, sym_all());
    asr_data_.hold_steps = lv.intAt(2, 1);
    asr_data_.off_mode = lv.symbolAt(3, sym_all());
    asr_data_.state = AsrData::ATTACK;
    asr_clock_.exec();
}

bool SeqArp::findFirstMatchedNote(size_t from, size_t mode, bool state, size_t* idx) const
{
    switch (mode) {
    case hash_all:
    case hash_a:
    case hash_first:
    case hash_f:
        for (size_t i = from; i < on_offs_.size(); i++) {
            if (on_offs_[i] != state) {
                *idx = i;
                return true;
            }
        }
        break;
    case hash_last:
    case hash_l:
        for (size_t i = from; i < on_offs_.size(); i++) {
            auto ri = on_offs_.size() - (i + 1);
            if (on_offs_[ri] != state) {
                *idx = ri;
                return true;
            }
        }
        break;
    case hash_random:
    case hash_r: {
        auto suitable_notes_num = std::count_if(on_offs_.begin(), on_offs_.end(), [state](std::uint8_t x) {
            return x != state;
        });

        if (suitable_notes_num == 0) {
            *idx = 0;
            return false;
        }

        std::uniform_int_distribution<typeof(suitable_notes_num)> dist(0, suitable_notes_num - 1);
        // choose random suitable note
        auto nth_random = dist(rng_);

        // find nth suitable note
        int count = 0;
        auto which = std::find_if(on_offs_.begin(), on_offs_.end(), [&count, state, nth_random](std::uint8_t x) {
            return (x != state && count++ == nth_random);
        });

        // should not happen
        if (which == on_offs_.end()) {
            return false;
        } else {
            *idx = std::distance(on_offs_.begin(), which);
            return true;
        }

    } break;
    default:
        OBJ_ERR << "invalid mode: " << mode;
        break;
    }

    return false;
}

void SeqArp::outputNoteMessage(size_t idx, t_float vel, int nactive)
{
    if (idx >= chord_->value().size())
        return;

    AtomArray<2> midi;
    midi[0] = chord_->value()[idx];
    midi[1] = vel;

    floatTo(1, nactive);
    listTo(0, midi.view());
}

void SeqArp::processOnOff(size_t mode_hash, bool state, t_float vel, int num)
{
    switch (mode_hash) {
    case hash_first:
    case hash_f: {
        size_t idx = 0;
        while (findFirstMatchedNote(idx, mode_hash, state, &idx) && num-- > 0) {
            on_offs_[idx] = state;
            outputNoteMessage(idx, vel, state ? ++nactive_ : --nactive_);
        }
    } break;
    case hash_last:
    case hash_l: {
        size_t idx = 0;
        while (findFirstMatchedNote(0, mode_hash, state, &idx) && num-- > 0) {
            on_offs_[idx] = state;
            outputNoteMessage(idx, vel, state ? ++nactive_ : --nactive_);
        }
    } break;
    case hash_random:
    case hash_r: {
        size_t idx = 0;
        while (findFirstMatchedNote(0, mode_hash, state, &idx) && num-- > 0) {
            on_offs_[idx] = state;
            outputNoteMessage(idx, vel, state ? ++nactive_ : --nactive_);
        }
    } break;
    case hash_all:
    case hash_a: {
        size_t idx = 0;
        while (findFirstMatchedNote(idx, mode_hash, state, &idx)) {
            on_offs_[idx] = state;
            outputNoteMessage(idx, vel, state ? ++nactive_ : --nactive_);
        }
    } break;
    default:
        OBJ_ERR << "unknown note mode: " << mode_hash;
        break;
    }
}

void setup_seq_arp()
{
    ObjectFactory<SeqArp> obj("seq.arp");
    obj.addMethod("on", &SeqArp::m_on);
    obj.addMethod("off", &SeqArp::m_off);
    obj.addMethod("reset", &SeqArp::m_reset);

    obj.addMethod("asr", &SeqArp::m_asr);

    obj.setDescription("sequence arpeggiator");
    obj.setCategory("seq");
    obj.setKeywords({ "seq", "arpeggio", "pattern" });
}
