#include "midi_arp.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

#include <ctime>
#include <random>

CEAMMC_DEFINE_HASH(up);
CEAMMC_DEFINE_HASH(down);
CEAMMC_DEFINE_HASH(tri);
CEAMMC_DEFINE_HASH(random);

MidiArp::MidiArp(const PdArgs& args)
    : BaseObject(args)
    , external_clock_(nullptr)
    , clock_([this]() { playNote(); })
    , delay_(nullptr)
    , min_notes_(nullptr)
    , pass_(nullptr)
    , mode_(nullptr)
    , seed_(nullptr)
    , cur_note_idx_(0)
    , prev_note_(-1)
    , state_(STATE_EMPTY)
    , gen_(time(0))
{
    createInlet();
    createOutlet();

    external_clock_ = new BoolProperty("@external", false);
    addProperty(external_clock_);

    delay_ = new FloatProperty("@delay", 100);
    addProperty(delay_);

    pass_ = new BoolProperty("@pass", false);
    addProperty(pass_);

    min_notes_ = new IntProperty("@min_notes", 1);
    addProperty(min_notes_);

    mode_ = new SymbolEnumProperty("@mode", { str_up, str_down, str_tri, str_random });
    addProperty(mode_);

    seed_ = new IntProperty("@seed", -1);
    seed_->setSuccessFn([this](Property*) {
        if (seed_->value() < 0)
            gen_.seed(time(0));
        else
            gen_.seed(seed_->value());
    });
    addProperty(seed_);
}

MidiArp::~MidiArp()
{
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
                cur_note_idx_ = 0;
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
                cur_note_idx_ = 0;
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
            if (notes_.size() < min_notes_->value()) {
                state_ = STATE_NOARP;
                cur_note_idx_ = 0;
                if (prev_note_ >= 0) {
                    sendNote(prev_note_, 0);
                    prev_note_ = -1;
                }
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

    cur_note_idx_ %= notes_.size();

    if (prev_note_ >= 0)
        sendNote(prev_note_, 0);

    sendNote(notes_[cur_note_idx_], 127);

    prev_note_ = notes_[cur_note_idx_];
    cur_note_idx_ = nextNote();

    if (!external_clock_->value())
        clock_.delay(delay_->value());
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
    if (it != notes_.end())
        notes_.erase(it, notes_.end());
}

int MidiArp::nextNote()
{
    const auto N = notes_.size();

    if (N == 0)
        return cur_note_idx_;

    switch (crc32_hash(mode_->value())) {
    case hash_up:
        return (cur_note_idx_ + 1) % N;
    case hash_down:
        return (cur_note_idx_ == 0) ? (N - 1) : ((cur_note_idx_ - 1) % N);
    case hash_random: {
        std::uniform_int_distribution<int> dist(1, N - 1);
        return (cur_note_idx_ + dist(gen_)) % N;
    } break;
    default:
        return cur_note_idx_;
    }
}

void setup_midi_arp()
{
    ObjectFactory<MidiArp> obj("midi.arp");
}
