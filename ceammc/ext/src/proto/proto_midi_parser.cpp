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
#include "proto_midi_parser.h"

using namespace ceammc::midi;

MidiParser::MidiParser()
    : state_(STATE_INIT)
    , data_len_(0)
    , max_data_len_(0)
{
    reset();
}

MidiParser::Result MidiParser::push(Byte byte)
{
    switch (state_) {
    case STATE_INIT: {
        const bool is_status_byte = 0x80 & byte;
        if (!is_status_byte)
            return resetError(ERROR_STATUS_BYTE_EXPECTED);

        const Byte status = byte & 0xf0;
        switch (status) {
        case STATE_NOTE_ON:
        case STATE_NOTE_OFF:
        case STATE_CONTROLCHANGE:
        case STATE_POLYAFTERTOUCH:
        case STATE_PITCHBEND:
            state_ = static_cast<State>(status);
            max_data_len_ = 3;
            pushData(byte);
            return ok();
        case STATE_PROGRAMCHANGE:
        case STATE_AFTERTOUCH:
            state_ = static_cast<State>(status);
            max_data_len_ = 2;
            pushData(byte);
            return ok();
        case STATE_SYSTEM:
            if (byte > MIDI_SYSEXEND) { // realtime message
                // process immidiately
                if (sys_realtime_cb_)
                    sys_realtime_cb_(byte);

                return ok();
            } else if (byte == MIDI_SYSEX) { // sysex
                state_ = STATE_SYSTEM_EXCLUSIVE;
                max_data_len_ = 1;
                pushData(byte);
                return ok();
            } else if (byte == MIDI_TUNEREQUEST) { // tune request
                // process immidiately
                if (sys_common_cb_)
                    sys_common_cb_(byte, 0, 0);

                return ok();
            } else if (byte == MIDI_TIMECODE) { // timecode
                state_ = STATE_SYSTEM_COMMON;
                max_data_len_ = 2;
                pushData(byte);
                return ok();
            } else if (byte == MIDI_SONGPOS) { // song position
                state_ = STATE_SYSTEM_COMMON;
                max_data_len_ = 3;
                pushData(byte);
                return ok();
            } else if (byte == MIDI_SONGSELECT) { // song select
                state_ = STATE_SYSTEM_COMMON;
                max_data_len_ = 2;
                pushData(byte);
                return ok();
            } else { // unknown status byte
                return resetError(ERROR_UNEXPECTED);
            }
            break;
        default:
            return resetError(ERROR_STATUS_BYTE_EXPECTED);
        }
    } break;
    // data bytes
    case STATE_NOTE_ON:
    case STATE_NOTE_OFF:
    case STATE_CONTROLCHANGE:
    case STATE_POLYAFTERTOUCH:
    case STATE_PITCHBEND:
    case STATE_PROGRAMCHANGE:
    case STATE_AFTERTOUCH:
    case STATE_SYSTEM_COMMON: {
        const bool is_status_byte = 0x80 & byte;
        if (is_status_byte) {
            // if realtime message
            if (byte > MIDI_SYSEXEND) {
                if (sys_realtime_cb_) // process immidiately
                    sys_realtime_cb_(byte);

                // no reset
                return ok();
            } else
                return resetError(ERROR_UNEXPECTED);
        } else {
            if (!pushData(byte))
                return resetError(ERROR_BUFFER_OVERFLOW);

            if (dataComplete()) {
                runCallbacks();
                return resetOk();
            } else
                return ok();
        }
    } break;
    case STATE_SYSTEM_EXCLUSIVE:
    case STATE_SYSTEM: {
        const bool is_status_byte = 0x80 & byte;
        if (is_status_byte) {
            if (byte == MIDI_SYSEXEND) { // sysex end
                max_data_len_++;
                if (!pushData(byte))
                    return resetError(ERROR_BUFFER_OVERFLOW);

                if (sys_exclusive_cb_)
                    sys_exclusive_cb_(data_len_, data_);

                // sysex done
                return resetOk();
            } else if (byte > MIDI_SYSEXEND) { // realtime message
                // process immidiately
                if (sys_realtime_cb_)
                    sys_realtime_cb_(byte);

                return ok();
            } else
                return resetError(ERROR_STATUS_BYTE_EXPECTED);

        } else { // append sysex data
            max_data_len_++;
            if (!pushData(byte))
                return resetError(ERROR_BUFFER_OVERFLOW);

            return ok();
        }
    } break;
    default:
        return resetError(ERROR_UNKNOWN_STATE);
    }
}

MidiParser::Result MidiParser::resetError(MidiParser::Error err)
{
    State st = state_;
    reset();
    return { st, err };
}

void MidiParser::runCallbacks()
{
#define CALL2(fn, data)           \
    {                             \
        if (fn)                   \
            fn(data[0], data[1]); \
    }

#define CALL3(fn, data)                    \
    {                                      \
        if (fn)                            \
            fn(data[0], data[1], data[2]); \
    }

    switch (state_) {
    case STATE_NOTE_ON:
        CALL3(noteon_cb_, data_)
        break;
    case STATE_NOTE_OFF:
        CALL3(noteoff_cb_, data_)
        break;
    case STATE_CONTROLCHANGE:
        CALL3(cc_cb_, data_)
        break;
    case STATE_POLYAFTERTOUCH:
        CALL3(poly_cb_, data_)
        break;
    case STATE_PITCHBEND:
        CALL3(pitchbend_cb_, data_)
        break;
    case STATE_PROGRAMCHANGE:
        CALL2(prg_cb_, data_)
        break;
    case STATE_AFTERTOUCH:
        CALL2(atouch_cb_, data_)
        break;
    case STATE_SYSTEM_COMMON:
        CALL3(sys_common_cb_, data_);
        break;
    default:
        break;
    }

#undef CALL2
#undef CALL3
}
