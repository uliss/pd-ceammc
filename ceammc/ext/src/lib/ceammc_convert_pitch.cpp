#include "ceammc_convert.h"
#include "ceammc_log.h"

#include <cstdint>
#include <cstring>

class SPN {
public:
    enum State {
        ST_INIT,
        ST_NOTE,
        ST_REST,
        ST_ALT_SHARP,
        ST_ALT_FLAT,
        ST_OCTAVE,
        ST_DONE,
        ST_ERROR
    };

private:
    State st_;
    uint8_t note_;
    int8_t alt_;
    uint8_t oct_;
    char err_[64];
    static const int defOct = 4;

public:
    SPN()
        : st_(ST_INIT)
        , note_(0)
        , alt_(0)
        , oct_(0)
    {
    }

    State state() const { return st_; }
    const char* error() const { return err_; }

    void reset()
    {
        st_ = ST_INIT;
        note_ = 0;
        alt_ = 0;
        oct_ = 0;
        err_[0] = '\0';
    }

    bool isNote() const { return note_ != 0 && !isRest(); }
    bool isRest() const { return note_ == 'R'; }
    int midi() const { return ((oct_ + 1) * 12 + midiNote(note_)) + alt_; }

    State setState(State st, const char* msg = nullptr)
    {
        st_ = st;
        if (msg) {
            strncpy(err_, msg, sizeof(err_));
            err_[sizeof(err_) - 1] = '\0';
        }

        return st;
    }

    State put(char c)
    {
        switch (st_) {
        case ST_INIT: {
            if (c >= 'A' && c <= 'G') {
                note_ = c;
                return setState(ST_NOTE);
            } else if (c == 'R') {
                note_ = c;
                return setState(ST_REST);
            } else {
                return setState(ST_ERROR, "Note name expected");
            }
        } break;
        case ST_REST: {
            if (c == '\0')
                return setState(ST_DONE);
            else
                return setState(ST_ERROR, "extra character after Rest");
        } break;
        case ST_NOTE: {
            if (c == '#') {
                alt_++;
                return setState(ST_ALT_SHARP);
            } else if (c == 'b') {
                alt_--;
                return setState(ST_ALT_FLAT);
            } else if (isdigit(c)) {
                oct_ = c - '0';
                return setState(ST_OCTAVE);
            } else if (c == '\0') {
                oct_ = defOct;
                return setState(ST_DONE);
            } else {
                return setState(ST_ERROR, "Alteration or Octave expected");
            }
        } break;
        case ST_ALT_FLAT: {
            if (c == 'b') {
                alt_--;
                return setState(ST_ALT_FLAT);
            } else if (isdigit(c)) {
                oct_ = c - '0';
                return setState(ST_OCTAVE);
            } else if (c == '\0') {
                oct_ = defOct;
                return setState(ST_DONE);
            } else {
                return setState(ST_ERROR, "Flat or Octave expected");
            }
        } break;
        case ST_ALT_SHARP: {
            if (c == '#') {
                alt_++;
                return setState(ST_ALT_SHARP);
            } else if (isdigit(c)) {
                oct_ = c - '0';
                return setState(ST_OCTAVE);
            } else if (c == '\0') {
                oct_ = defOct;
                return setState(ST_DONE);
            } else {
                return setState(ST_ERROR, "Sharp or Octave expected");
            }
        } break;
        case ST_OCTAVE: {
            if (c == '\0') {
                return setState(ST_DONE);
            } else {
                return setState(ST_ERROR, "extra characters after OCTAVE");
            }
        } break;
        default:
            return setState(ST_ERROR, "extra characters");
        }
    }

    static int midiNote(uint8_t n)
    {
        auto deg = (n >= 'C') ? (n - 'C') : (n - 'A' + 5);
        return 2 * deg - (deg > 2);
    }
};

static SPN spn;

int ceammc::convert::spn2midi(const char* p)
{
    const char* pp = p;
    spn.reset();
    int guard = 0;

    while (true) {
        switch (spn.put(*pp)) {
        case SPN::ST_ERROR:
            LIB_ERR << "SPN parse: " << spn.error();
            return MIDI_NONE;
        case SPN::ST_DONE:
            return spn.isRest() ? MIDI_REST : spn.midi();
        default:
            ++pp;
        }

        if (++guard > 16) {
            LIB_ERR << "SPN parse loop";
            return MIDI_NONE;
        }
    }
}
