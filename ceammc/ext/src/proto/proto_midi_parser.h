/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef PROTO_MIDI_PARSER_H
#define PROTO_MIDI_PARSER_H

#include <cstdint>
#include <functional>

namespace ceammc {
namespace midi {
    using Byte = std::uint8_t;
    using SysRealtimeCb = std::function<void(Byte)>;
    using SysCommonCb = std::function<void(Byte, Byte, Byte)>;
    using SysExcCb = std::function<void(size_t, Byte*)>;
    using Byte3Cb = std::function<void(Byte, Byte, Byte)>;
    using Byte2Cb = std::function<void(Byte, Byte)>;

    enum MidiMessage {
        /* channel voice messages */
        MIDI_NOTEOFF = 0x80,
        MIDI_NOTEON = 0x90,
        MIDI_POLYAFTERTOUCH = 0xa0,
        MIDI_CONTROLCHANGE = 0xb0,
        MIDI_PROGRAMCHANGE = 0xc0,
        MIDI_AFTERTOUCH = 0xd0,
        MIDI_PITCHBEND = 0xe0,
        /* system common messages */
        MIDI_SYSEX = 0xf0,
        MIDI_TIMECODE = 0xf1,
        MIDI_SONGPOS = 0xf2,
        MIDI_SONGSELECT = 0xf3,
        MIDI_TUNEREQUEST = 0xf6,
        MIDI_SYSEXEND = 0xf7,
        MIDI_TIMECLOCK = 0xf8,
        MIDI_TIMETICK = 0xf9,
        MIDI_START = 0xfa,
        MIDI_CONTINUE = 0xfb,
        MIDI_STOP = 0xfc,
        MIDI_ACTIVE_SENSE = 0xfe,
        MIDI_SYSTEM_RESET = 0xff
    };

    class MidiParser {
    public:
        enum State : std::int16_t {
            STATE_INIT = 0,
            STATE_SEND = -1,
            STATE_NOTE_OFF = 0x80,
            STATE_NOTE_ON = 0x90,
            STATE_POLYAFTERTOUCH = 0xa0,
            STATE_CONTROLCHANGE = 0xb0,
            STATE_PROGRAMCHANGE = 0xc0,
            STATE_AFTERTOUCH = 0xd0,
            STATE_PITCHBEND = 0xe0,
            STATE_SYSTEM = 0xf0,
            STATE_SYSTEM_COMMON = 1024,
            STATE_SYSTEM_EXCLUSIVE
        };

        enum Error : std::uint8_t {
            NO_ERROR,
            ERROR_UNEXPECTED,
            ERROR_STATUS_BYTE_EXPECTED,
            ERROR_BUFFER_OVERFLOW,
            ERROR_UNKNOWN_STATE,
        };

        struct Result {
            State state;
            Error err;
        };

        static const std::uint16_t BUFSIZE = 1024;

    private:
        State state_;
        Byte data_[BUFSIZE];
        std::uint32_t data_len_;
        std::uint32_t max_data_len_;

        SysRealtimeCb sys_realtime_cb_;
        SysCommonCb sys_common_cb_;
        SysExcCb sys_exclusive_cb_;
        Byte3Cb noteon_cb_, noteoff_cb_, cc_cb_, pitchbend_cb_, poly_cb_;
        Byte2Cb atouch_cb_, prg_cb_;

    public:
        MidiParser();

        Result push(Byte byte);

        void setNoteOnFn(Byte3Cb cb) { noteon_cb_ = cb; }
        void setNoteOffFn(Byte3Cb cb) { noteoff_cb_ = cb; }
        void setAfterTouchFn(Byte2Cb cb) { atouch_cb_ = cb; }
        void setRealtimeFn(SysRealtimeCb cb) { sys_realtime_cb_ = cb; }

    private:
        void reset()
        {
            state_ = STATE_INIT;
            data_len_ = 0;
            max_data_len_ = 0;
        }

        bool pushData(Byte b)
        {
            if (data_len_ >= max_data_len_ || data_len_ >= BUFSIZE)
                return false;

            data_[data_len_++] = b;
            return true;
        }

        bool dataComplete() const { return data_len_ == max_data_len_; }

        Result resetError(Error err);

        Result resetOk()
        {
            auto st = state_;
            reset();
            return { st, NO_ERROR };
        }

        Result ok() const { return { state_, NO_ERROR }; }

        void runCallbacks();
    };
}
}

#endif // PROTO_MIDI_PARSER_H
