#include "midi_pitch_quantize.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

namespace ceammc {
CEAMMC_DEFINE_SYM_HASH(clamp);
CEAMMC_DEFINE_SYM_HASH(fold);
CEAMMC_DEFINE_SYM_HASH(reject);
}

using namespace ceammc;

static constexpr int MIDI_MIN = 0;
static constexpr int MIDI_MIN_DEF = 0;
static constexpr int MIDI_MAX = 127;
static constexpr int MIDI_MAX_DEF = 127;

MidiPitchQuantize::MidiPitchQuantize(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

    min_ = new IntProperty("@min", MIDI_MIN_DEF);
    min_->checkClosedRange(MIDI_MIN, MIDI_MAX);
    min_->setArgIndex(0);
    addProperty(min_);

    max_ = new IntProperty("@max", MIDI_MAX_DEF);
    max_->checkClosedRange(MIDI_MIN, MIDI_MAX);
    max_->setArgIndex(1);
    addProperty(max_);

    mode_ = new SymbolEnumProperty("@mode", { str_reject, str_fold, str_clamp });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@reject", mode_, sym_reject()));
    addProperty(new SymbolEnumAlias("@fold", mode_, sym_fold()));
    addProperty(new SymbolEnumAlias("@clamp", mode_, sym_clamp()));
}

MaybeFloat MidiPitchQuantize::quantize(t_float note) const
{
    const auto pitch = static_cast<t_int>(std::lround(note));
    if (pitch < MIDI_MIN || pitch > MIDI_MAX)
        return { };

    const auto lo = min_->value();
    const auto hi = max_->value();

    if (lo >= hi) {
        OBJ_ERR << fmt::format("invalid pitch range: [{}..{}]", lo, hi);
        return { };
    }

    if (pitch >= lo && pitch <= hi)
        return static_cast<t_float>(pitch);

    switch (crc32_hash(mode_->value())) {
    case hash_reject:
        return { };
    case hash_clamp:
        return clip(pitch, lo, hi);
    case hash_fold:
    default: {
        // fold: preserve pitch class, shift by whole octaves.
        auto p = pitch;

        while (p < lo)
            p += 12;

        while (p > hi)
            p -= 12;

        if (p < lo || p > hi)
            p = clip(p, lo, hi); // degenerate range

        return static_cast<t_float>(p);
    }
    }
}

MaybeFloat MidiPitchQuantize::quantize(const Atom& note) const
{
    if (note.isFloat()) {
        return quantize(note.asT<t_float>());
    } else {
        OBJ_ERR << "float MIDI pitch is expected, got: " << note;
        return { };
    }
}

void MidiPitchQuantize::onFloat(t_float note)
{
    auto q = quantize(note);
    if (q.has_value())
        floatTo(0, q.get());
}

void MidiPitchQuantize::onList(const AtomListView& lv)
{
    const auto N = lv.size();
    const auto is_valid = N > 0 && lv[0].isFloat();
    if (!is_valid)
        return;

    auto q = quantize(lv[0]);
    if (!q.has_value())
        return;

    auto pitch = q.get();

    switch (N) {
    case 1: {
        return floatTo(0, pitch);
    }
    case 2: {
        AtomArray<2> p;
        p[0] = pitch;
        p[1] = lv[1];
        listTo(0, p.view());
        break;
    }
    default: {
        AtomList out(lv);
        out[0] = pitch;
        listTo(0, out);
    } break;
    }
}

void setup_midi_pitch_quantize()
{
    ObjectFactory<MidiPitchQuantize> obj("midi.pitch.quantize");
}
