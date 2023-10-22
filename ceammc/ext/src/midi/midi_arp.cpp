#include "midi_arp.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

#include <cstdint>

CEAMMC_DEFINE_HASH(up);
CEAMMC_DEFINE_HASH(up1);
CEAMMC_DEFINE_HASH(down);
CEAMMC_DEFINE_HASH(down1);
CEAMMC_DEFINE_HASH(tri);
CEAMMC_DEFINE_HASH(tri1);
CEAMMC_DEFINE_HASH(random);
CEAMMC_DEFINE_HASH(random1);

MidiArp::MidiArp(const PdArgs& args)
    : BaseObject(args)
    , external_clock_(nullptr)
    , clock_([this]() { if(on_->value()) playNote(); })
    , t_(nullptr)
    , min_notes_(nullptr)
    , pass_(nullptr)
    , on_(nullptr)
    , mode_(nullptr)
    , seed_(nullptr)
    , phase_(0)
    , prev_note_(-1)
    , state_(STATE_EMPTY)
{
    createInlet();
    createOutlet();
    createOutlet();

    external_clock_ = new BoolProperty("@external", false);
    addProperty(external_clock_);

    t_ = new FloatProperty("@t", 100);
    t_->checkClosedRange(1, 1000);
    t_->setArgIndex(0);
    t_->setUnitsMs();
    addProperty(t_);

    pass_ = new BoolProperty("@pass", false);
    addProperty(pass_);

    on_ = new BoolProperty("@on", true);
    on_->setSuccessFn([this](Property*) {
        if (on_->value())
            playNote();
        else
            releasePrevious();
    });
    addProperty(on_);

    min_notes_ = new IntProperty("@min_notes", 1);
    min_notes_->checkMinEq(1);
    addProperty(min_notes_);

    mode_ = new SymbolEnumProperty("@mode", { str_up, str_up1, str_down, str_down1, str_tri, str_tri1, str_random, str_random1 });
    mode_->setSuccessFn([this](Property*) { phase_ = 0; });
    addProperty(mode_);

    seed_ = new random::SeedProperty(gen_);
    addProperty(seed_);
}

MidiArp::~MidiArp()
{
    releasePrevious();

    for (auto& n : notes_)
        sendNote(n, 0);

    notes_.clear();
    state_ = STATE_EMPTY;
}

void MidiArp::onList(const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_BYTE, ARG_BYTE))
        return;

    const int note = lv[0].asT<int>();
    const int vel = lv[1].asT<int>();
    if (note < 0 || note > 127) {
        OBJ_ERR << fmt::format("MIDI note in [0..127] range expected, got: {}", note);
        return;
    }

    if (vel < 0 || vel > 127) {
        OBJ_ERR << fmt::format("MIDI velocity in [0..127] range expected, got: {}", vel);
        return;
    }

    if (pass_->value())
        return sendNote(note, vel);

    onEvent((vel > 0) ? NOTE_ON : NOTE_OFF, note);

    if (state_ == STATE_ARP && !external_clock_->value())
        playNote();
}

void MidiArp::onInlet(size_t n, const AtomListView& lv)
{
    if (state_ == STATE_ARP && external_clock_->value())
        playNote();
}

void MidiArp::onEvent(MidiArpEvent ev, std::uint8_t note)
{
    switch (state_) {
    case STATE_EMPTY: {
        switch (ev) {
        case NOTE_ON:
            notes_.push_back(note);
            if (notes_.size() < min_notes_->value()) {
                state_ = STATE_NOARP;
            } else {
                state_ = STATE_ARP;
                phase_ = 0;
            }
            break;
        case NOTE_OFF:
        case PLAY_NOTE:
        default:
            return;
        }
    } break;
    case STATE_NOARP: {
        switch (ev) {
        case NOTE_OFF: {
            removeNote(note);
            if (notes_.empty())
                state_ = STATE_EMPTY;

        } break;
        case NOTE_ON: {
            addNote(note, 127);
            if (notes_.size() >= min_notes_->value()) {
                state_ = STATE_ARP;
                phase_ = 0;
                prev_note_ = -1;
            }
        } break;
        case PLAY_NOTE:
        default:
            break;
        }

    } break;
    case STATE_ARP: {
        switch (ev) {
        case NOTE_ON:
            addNote(note, 127);
            break;
        case NOTE_OFF: {
            removeNote(note);
            releasePrevious();

            if (notes_.size() < min_notes_->value()) {
                state_ = STATE_NOARP;
                phase_ = 0;
            }
            if (notes_.empty())
                state_ = STATE_EMPTY;

        } break;
        case PLAY_NOTE:
            playNote();
            break;
        }
    } break;
    }
}

void MidiArp::playNote()
{
    if (notes_.empty())
        return;

    releasePrevious();

    // note: should be after prev_note off!
    // if @min_notes changes between notes to prevent hanging notes
    if (notes_.size() < min_notes_->value())
        return;

    auto n = currentNote();
    floatTo(1, phase_);
    sendNote(n, 127);
    prev_note_ = n;

    nextNote();

    if (!external_clock_->value())
        clock_.delay(t_->value());
}

void MidiArp::sendNote(std::uint8_t note, std::uint8_t vel)
{
    std::array<Atom, 2> msg { note, vel };
    listTo(0, AtomListView(msg.data(), msg.size()));
}

void MidiArp::addNote(std::uint8_t note, std::uint8_t vel)
{
    notes_.insert(std::upper_bound(notes_.begin(), notes_.end(), note), note);
}

void MidiArp::removeNote(std::uint8_t note)
{
    auto it = std::remove(notes_.begin(), notes_.end(), note);
    if (it != notes_.end()) {
        notes_.erase(it, notes_.end());
        sendNote(note, 0);
    }
}

void MidiArp::nextNote()
{
    const auto N = notes_.size();

    if (N < 2)
        return;

    switch (crc32_hash(mode_->value())) {
    case hash_random:
        phase_ = (phase_ + gen_.gen_uniform_uint(1, N - 1)) % N;
        break;
    case hash_random1:
        break;
    case hash_tri:
    case hash_tri1:
        phase_ = (phase_ + 1) % (2 * (N - 1));
        break;
    default:
        phase_ = (phase_ + 1) % N;
        break;
    }
}

std::uint32_t MidiArp::currentNote() const
{
    const auto N = notes_.size();

    switch (crc32_hash(mode_->value())) {
    case hash_tri:
        return notes_[foldInteger(phase_, N)];
    case hash_tri1:
        return notes_[foldInteger<int>(phase_ + N - 1, N)];
    case hash_up:
        return notes_[wrapInteger(phase_, N)];
    case hash_up1:
        return notes_[wrapInteger<int>(phase_ + N - 1, N)];
    case hash_down:
        return notes_[N - (wrapInteger(phase_, N) + 1)];
    case hash_down1:
        return notes_[N - (wrapInteger<int>(phase_ + N - 1, N) + 1)];
    case hash_random1:
        return notes_[gen_.gen_uniform_uint(0, N - 1)];
    case hash_random:
    default:
        return notes_[wrapInteger(phase_, N)];
    }
}

void MidiArp::releasePrevious()
{
    if (prev_note_ >= 0) {
        sendNote(prev_note_, 0);
        prev_note_ = -1;
    }
}

void setup_midi_arp()
{
    ObjectFactory<MidiArp> obj("midi.arp");
    obj.setXletsInfo({ "list: NOTE VEL", "bang: if @external = 1" }, { "list: NOTE VEL", "int: phase" });

    obj.setDescription("MIDI arpeggiator");
    obj.setCategory("midi");
    obj.setKeywords({"midi", "arpeggiator"});
}
