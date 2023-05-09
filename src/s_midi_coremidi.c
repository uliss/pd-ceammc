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

#include "m_pd.h"
#include "s_stuff.h"

#include <CoreMIDI/MIDIDriver.h>
#include <CoreMIDI/MIDIServices.h>
#include <CoreMedia/CMSimpleQueue.h>
#include <CoreServices/CoreServices.h>
#include <mach/mach_time.h>
#include <stdio.h>

enum {
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
    MIDI_SYSEXEND = 0xf7
};

enum {
    MIDI_QUEUE_SIZE = 1024,
    MAX_COREMIDI_DEVS = 256,
    MAX_MIDI_MAP_SIZE = 16
};

typedef struct DeviceInfo {
    MIDIEntityRef ref;
    Byte portno;
    Boolean is_connected;
} DeviceInfo;

typedef enum MidiFSMState {
    MIDI_FSM_STATE_INIT = 0,
    MIDI_FSM_STATE_SEND = -1,
    MIDI_FSM_STATE_NOTE_OFF = 0x80,
    MIDI_FSM_STATE_NOTE_ON = 0x90,
    MIDI_FSM_STATE_POLYAFTERTOUCH = 0xa0,
    MIDI_FSM_STATE_CONTROLCHANGE = 0xb0,
    MIDI_FSM_STATE_PROGRAMCHANGE = 0xc0,
    MIDI_FSM_STATE_AFTERTOUCH = 0xd0,
    MIDI_FSM_STATE_PITCHBEND = 0xe0,
    MIDI_FSM_STATE_MIDI_SYSEX = 0xf0,
    MIDI_FSM_STATE_SYSEX_FIXED = 1024
} MidiFSMState;

typedef struct MidiFSMParser {
    MidiFSMState state;
    Byte buffer[65536];
    MIDIPacketList* packetlist;
    MIDIPacket* current_packet;
    MIDIEndpointRef endpoint;
    ByteCount data_written;
    ByteCount max_data_len;
} MidiFSMParser;

static size_t coremidi_nsrc = 0;
static DeviceInfo coremidi_src[MAX_COREMIDI_DEVS];
static size_t coremidi_ndest = 0;
static MIDIEntityRef coremidi_dest[MAX_COREMIDI_DEVS];
static MIDIEntityRef virtual_src_output;
static MIDIEntityRef virtual_dest_input;
static int virtual_dest_input_portno;
static MIDIClientRef coremidi_client;
static MIDIPortRef input_port;
static MIDIPortRef output_port;
static CMSimpleQueueRef input_queue;
// map: pd portno -> endpoint index in coremidi_dest
static int portno_to_dest_index_map[MAX_MIDI_MAP_SIZE];
static MidiFSMParser dest_midi_parsers[MAX_MIDI_MAP_SIZE];

static void coremidi_init();
static Boolean coremidi_init_sources();
static Boolean coremidi_init_destinations();
static void coremidi_print_error(OSStatus err, const char* operation);
static void coremidi_getindevs(char* indevlist, int* nindevs, int maxndev, int devdescsize);
static void coremidi_getoutdevs(char* outdevlist, int* noutdevs, int maxndev, int devdescsize);
static void coremidi_notify(const MIDINotification* msg, void* refCon);
static void coremidi_read(const MIDIPacketList* lst, void* refCon, void* connRefCon);
static void coremidi_append_src_device(MIDIEntityRef dev);
static void coremidi_append_dest_device(MIDIEntityRef dest);
static int coremidi_unique_id(Boolean isInput);
static CFStringRef coremidi_copy_device_name(MIDIEndpointRef dev);
static Boolean coremidi_create_client();
static Boolean coremidi_create_virtual_source();
static Boolean coremidi_create_virtual_destination();
static Boolean coremidi_create_input_port();
static Boolean coremidi_create_output_port();
static void coremidi_connect_sources(int nmidiin, int* midiinvec);
static Boolean coremidi_connect_source(DeviceInfo* dev, int portno);
static void coremidi_disconnect_sources();
static void coremidi_send_packet_list(MIDIEntityRef dest, MIDIPacketList* pktlst);

// MIDI parser finite state machine
static void midi_parser_init(MidiFSMParser* parser, MIDIEndpointRef endpoint);
static void midi_parser_reset(MidiFSMParser* parser);
static void midi_parser_send(MidiFSMParser* parser);
static Boolean midi_parser_append_byte(MidiFSMParser* parser, Byte byte);
static void midi_parser_input(MidiFSMParser* parser, Byte byte);

void midi_parser_init(MidiFSMParser* parser, MIDIEndpointRef endpoint)
{
    parser->state = MIDI_FSM_STATE_INIT;
    parser->packetlist = (MIDIPacketList*)parser->buffer;
    parser->current_packet = MIDIPacketListInit(parser->packetlist);
    parser->endpoint = endpoint;
    parser->data_written = 0;
    parser->max_data_len = 0;
}

void midi_parser_reset(MidiFSMParser* parser)
{
    parser->state = MIDI_FSM_STATE_INIT;
    parser->packetlist = (MIDIPacketList*)parser->buffer;
    parser->packetlist->numPackets = 0;
    parser->current_packet = MIDIPacketListInit(parser->packetlist);
    parser->current_packet->length = 0;
    parser->data_written = 0;
    parser->max_data_len = 0;
}

void midi_parser_send(MidiFSMParser* parser)
{
    coremidi_send_packet_list(parser->endpoint, parser->packetlist);
}

Boolean midi_parser_append_byte(MidiFSMParser* parser, Byte byte)
{
    if (parser->data_written >= parser->max_data_len) {
        pd_error(0, "[coremidi] extra midi byte: %d", (int)byte);
        return FALSE;
    }

    if (parser->packetlist->numPackets == 0)
        parser->current_packet = MIDIPacketListAdd(parser->packetlist, sizeof(parser->buffer), parser->current_packet, 0, 0, &byte);

    if (parser->current_packet->length >= sizeof(parser->current_packet->data)) {
        MIDIPacket* res = MIDIPacketListAdd(parser->packetlist, sizeof(parser->buffer), parser->current_packet, 0, 1, &byte);
        if (res == NULL) { // no room
            pd_error(0, "[coremidi] packet is too big");
            return FALSE;
        } else {
            parser->current_packet = res;
            parser->data_written++;
            return TRUE;
        }
    } else {
        parser->current_packet->data[parser->current_packet->length++] = byte;
        parser->data_written++;
        return TRUE;
    }
}

void midi_parser_input(MidiFSMParser* parser, Byte byte)
{
    switch (parser->state) {
    case MIDI_FSM_STATE_INIT: {
        const Boolean is_status_byte = 0x80 & byte;
        if (!is_status_byte) {
            pd_error(0, "[coremidi] unexpected raw midi value: %d", (int)byte);
            return;
        }

        const Byte status = byte & 0xf0;
        switch (status) {
        case MIDI_FSM_STATE_NOTE_ON:
        case MIDI_FSM_STATE_NOTE_OFF:
        case MIDI_FSM_STATE_CONTROLCHANGE:
        case MIDI_FSM_STATE_POLYAFTERTOUCH:
        case MIDI_FSM_STATE_PITCHBEND:
            parser->state = status; // new state
            parser->max_data_len = 3;
            midi_parser_append_byte(parser, byte);
            parser->current_packet->timeStamp = 0;
            break;
        case MIDI_FSM_STATE_PROGRAMCHANGE:
        case MIDI_FSM_STATE_AFTERTOUCH:
            parser->state = status;
            parser->max_data_len = 2;
            midi_parser_append_byte(parser, byte);
            parser->current_packet->timeStamp = 0;
            break;
        case MIDI_FSM_STATE_MIDI_SYSEX:
            // if realtime message
            if (byte > MIDI_SYSEXEND || byte == MIDI_TUNEREQUEST) { // send immidiately
                parser->packetlist->numPackets = 1;
                parser->packetlist->packet[0].length = 1;
                parser->packetlist->packet[0].timeStamp = 0;
                parser->packetlist->packet[0].data[0] = byte;
                midi_parser_send(parser);
                midi_parser_reset(parser);
            } else if (byte == MIDI_TIMECODE) { // fixed messages
                parser->state = MIDI_FSM_STATE_SYSEX_FIXED;
                parser->max_data_len = 2;
                midi_parser_append_byte(parser, byte);
                parser->current_packet->timeStamp = 0;
            } else if (byte == MIDI_SONGPOS) { // song position
                parser->state = MIDI_FSM_STATE_SYSEX_FIXED;
                parser->max_data_len = 3;
                midi_parser_append_byte(parser, byte);
                parser->current_packet->timeStamp = 0;
            } else if (byte == MIDI_SONGSELECT) { // song select
                parser->state = MIDI_FSM_STATE_SYSEX_FIXED;
                parser->max_data_len = 2;
                midi_parser_append_byte(parser, byte);
                parser->current_packet->timeStamp = 0;
            } else {
                parser->state = status;
                parser->current_packet->timeStamp = 0;
                parser->max_data_len = 1;
                if (!midi_parser_append_byte(parser, byte)) {
                    pd_error(0, "[coremidi] sysex error");
                    midi_parser_reset(parser);
                }
            }
            break;
        default:
            // not a status byte in init state
            pd_error(0, "[coremidi] unexpected raw midi value: %d", (int)byte);
            midi_parser_reset(parser);
            break;
        }
    } break;
    // data bytes
    case MIDI_FSM_STATE_NOTE_ON:
    case MIDI_FSM_STATE_NOTE_OFF:
    case MIDI_FSM_STATE_CONTROLCHANGE:
    case MIDI_FSM_STATE_POLYAFTERTOUCH:
    case MIDI_FSM_STATE_PITCHBEND:
    case MIDI_FSM_STATE_PROGRAMCHANGE:
    case MIDI_FSM_STATE_AFTERTOUCH:
    case MIDI_FSM_STATE_SYSEX_FIXED: {
        const Boolean is_status_byte = 0x80 & byte;
        if (is_status_byte) {
            // if realtime message
            if (byte > MIDI_SYSEXEND) {
                parser->max_data_len++; // increase packet length
                if (!midi_parser_append_byte(parser, byte)) {
                    midi_parser_reset(parser);
                    pd_error(0, "[coremidi] sysex realtime error");
                }
            } else {
                pd_error(0, "[coremidi] unexpected status byte value: %d", (int)byte);
                midi_parser_reset(parser);
            }
        } else {
            if (midi_parser_append_byte(parser, byte)) {
                if (parser->max_data_len == parser->data_written) {
                    midi_parser_send(parser);
                    midi_parser_reset(parser);
                }
            } else
                midi_parser_reset(parser);
        }
    } break;
    case MIDI_FSM_STATE_MIDI_SYSEX: {
        const Boolean is_status_byte = 0x80 & byte;
        if (is_status_byte) {
            if (byte == MIDI_SYSEXEND) {
                parser->max_data_len++;
                if (midi_parser_append_byte(parser, byte))
                    midi_parser_send(parser);
                else
                    pd_error(0, "[coremidi] sysex error");

                midi_parser_reset(parser);
            } else if (byte > MIDI_SYSEXEND) {
                parser->max_data_len++; // increase packet length
                if (!midi_parser_append_byte(parser, byte)) {
                    midi_parser_reset(parser);
                    pd_error(0, "[coremidi] sysex realtime error");
                }
            } else {
                midi_parser_reset(parser);
                pd_error(0, "[coremidi] unexpected status byte while sysex");
            }
        } else { // append sysex data
            parser->max_data_len++;
            if (!midi_parser_append_byte(parser, byte)) {
                pd_error(0, "[coremidi] sysex error");
                midi_parser_reset(parser);
            }
        }
    } break;
    default:
        break;
    }
}

void coremidi_print_error(OSStatus err, const char* operation)
{
    if (err == noErr)
        return;

    char error_str[20];
    *(UInt32*)(error_str + 1) = CFSwapInt32HostToBig(err);
    // if printable 4-digit code
    if (isprint(error_str[1])
        && isprint(error_str[2])
        && isprint(error_str[3])
        && isprint(error_str[4])) {
        error_str[0] = error_str[5] = '\''; // add quotes
        error_str[6] = '\0';
    } else
        sprintf(error_str, "%d", (int)err);

    pd_error(0, "[coremidi] system error: %s (%s)", operation, error_str);
}

void coremidi_append_src_device(MIDIEntityRef src)
{
    if (src == virtual_src_output) // skip virtual source
        return;

    CFStringRef name = NULL;
    OSStatus rc = MIDIObjectGetStringProperty(src, kMIDIPropertyName, &name);
    if (rc != noErr) {
        coremidi_print_error(rc, "MIDIObjectGetStringProperty");
        return;
    }

    if (coremidi_nsrc >= MAX_COREMIDI_DEVS) {
        fprintf(stdout, "[coremidi] too many sources: %d\n", (int)coremidi_nsrc);
        return;
    }

    coremidi_src[coremidi_nsrc].ref = src;
    coremidi_src[coremidi_nsrc].is_connected = FALSE;

    // output info
    char buf[MAXPDSTRING];
    CFStringGetCString(name, buf, MAXPDSTRING, kCFStringEncodingUTF8);
    if (sys_verbose)
        fprintf(stdout, "[coremidi] source (%d): %s\n", (int)coremidi_nsrc, buf);

    coremidi_nsrc++;
}

void coremidi_remove_src_device(MIDIEntityRef src)
{
    long idx = -1;

    // find index
    for (size_t i = 0; i < coremidi_nsrc; i++) {
        if (coremidi_src[i].ref == src) {
            idx = (long)i;
            break;
        }
    }

    if (idx < 0 || idx >= coremidi_nsrc)
        return;

    // disconnect device
    if (coremidi_src[idx].is_connected) {
        MIDIPortDisconnectSource(input_port, coremidi_src[idx].ref);
        coremidi_src[idx].is_connected = FALSE;
    }

    // remove/shift device refs
    for (size_t i = idx; i < (coremidi_nsrc - 1); i++)
        coremidi_src[i] = coremidi_src[i + 1];

    if (coremidi_nsrc > 0)
        coremidi_nsrc--;
}

void coremidi_append_dest_device(MIDIEntityRef dest)
{
    if (dest == virtual_dest_input) // skip virtual destinations
        return;

    CFStringRef name = NULL;
    OSStatus rc = MIDIObjectGetStringProperty(dest, kMIDIPropertyName, &name);
    if (rc != noErr) {
        coremidi_print_error(rc, "MIDIObjectGetStringProperty");
        return;
    }

    if (coremidi_ndest >= MAX_COREMIDI_DEVS) {
        fprintf(stdout, "[coremidi] too many destinations: %d\n", (int)coremidi_ndest);
        return;
    }

    coremidi_dest[coremidi_ndest] = dest;

    char buf[MAXPDSTRING];
    CFStringGetCString(name, buf, MAXPDSTRING, kCFStringEncodingUTF8);
    if (sys_verbose)
        fprintf(stdout, "[coremidi] destination (%d): %s\n", (int)coremidi_ndest, buf);

    coremidi_ndest++;
}

void coremidi_remove_dest_device(MIDIEntityRef dest)
{
    long idx = -1;

    // find index
    for (size_t i = 0; i < coremidi_ndest; i++) {
        if (coremidi_dest[i] == dest) {
            idx = (long)i;
            break;
        }
    }

    if (idx < 0 || idx >= coremidi_ndest)
        return;

    // remove/shift dev refs
    for (size_t i = idx; i < (coremidi_ndest - 1); i++)
        coremidi_dest[i] = coremidi_dest[i + 1];

    if (coremidi_ndest > 0)
        coremidi_ndest--;
}

int coremidi_unique_id(Boolean isInput)
{
    CFBundleRef bundle = CFBundleGetMainBundle();
    if (bundle) {
        CFStringRef ref = CFBundleGetIdentifier(bundle);
        if (ref) {
            CFStringRef str_id = CFStringCreateWithFormat(kCFAllocatorDefault, NULL,
                isInput ? CFSTR("%@.input") : CFSTR("%@.output"), ref);
            CFHashCode id = CFHash(str_id);
            CFRelease(str_id);
            return id;
        }
    }

    int maj = 0;
    int min = 0;
    int fix = 0;
    sys_getversion(&maj, &min, &fix);

    CFStringRef str_id = CFStringCreateWithFormat(kCFAllocatorDefault, NULL,
        CFSTR("pd-%d.%d.%d.%s"), maj, min, fix, isInput ? "input" : "output");

    CFHashCode id = CFHash(str_id);
    CFRelease(str_id);
    return id;
}

Boolean coremidi_init_sources()
{
    const ItemCount N = MIDIGetNumberOfSources();

    for (ItemCount i = 0; i < N; i++) {
        MIDIEndpointRef src = MIDIGetSource(i);
        if (!src) {
            pd_error(0, "[coremidi] can't open source: %d", (int)i);
            break;
        }

        coremidi_append_src_device(src);
    }

    return TRUE;
}

Boolean coremidi_init_destinations()
{
    const ItemCount N = MIDIGetNumberOfDestinations();

    for (ItemCount i = 0; i < N; i++) {
        MIDIEndpointRef dest = MIDIGetDestination(i);
        if (!dest) {
            pd_error(0, "[coremidi] can't open destination: %d", (int)i);
            break;
        }

        coremidi_append_dest_device(dest);
    }

    return TRUE;
}

void coremidi_notify(const MIDINotification* msg, void* refCon)
{
    if (!msg)
        return;

    const MIDIObjectAddRemoveNotification* add_remove_msg = (const MIDIObjectAddRemoveNotification*)msg;

    switch (msg->messageID) {
    case kMIDIMsgObjectAdded:

        switch (add_remove_msg->childType) {
        case kMIDIObjectType_Source:
            coremidi_append_src_device(add_remove_msg->child);
            break;
        case kMIDIObjectType_Destination:
            coremidi_append_dest_device(add_remove_msg->child);
            break;
        default:
            break;
        }

        break;

    case kMIDIMsgObjectRemoved:

        switch (add_remove_msg->childType) {
        case kMIDIObjectType_Source:
            coremidi_remove_src_device(add_remove_msg->child);
            break;
        case kMIDIObjectType_Destination: {
            coremidi_remove_dest_device(add_remove_msg->child);
        } break;
        default:
            break;
        }

        break;

    case kMIDIMsgSetupChanged:
    case kMIDIMsgPropertyChanged:
    default:
        break;
    }
}

// NOTE: Called on a separate high-priority thread, not the main runloop
void coremidi_read(const MIDIPacketList* lst, void* refCon, void* connRefCon)
{
    const MIDIPacket* p = &lst->packet[0];
    DeviceInfo* src = (DeviceInfo*)connRefCon;

    if (src == NULL && virtual_dest_input_portno < 0) {
        // virtual port is not opened
        return;
    }

    // when this callback called for virtual source connRefCon pointer is not passed
    // so we are using virtual_dest_input_portno which should be set while midi opening
    const Byte portno = (src != NULL) ? src->portno : virtual_dest_input_portno;

    for (UInt32 i = 0; i < lst->numPackets; i++) {
        for (UInt16 j = 0; j < p->length; j++) {
            // pack midi byte and port to UInt64 avoiding insertion of 0 into the queue
            UInt64 val = (portno << 24) | (p->data[j] << 16) | 0xFF;
            CMSimpleQueueEnqueue(input_queue, (void*)val);
        }

        p = MIDIPacketNext(p);
    }
}

void coremidi_init()
{
    static Boolean done = FALSE;

    if (done == TRUE) // run only once
        return;

    // init input queue
    OSStatus err = CMSimpleQueueCreate(NULL, MIDI_QUEUE_SIZE, &input_queue);
    if (err != noErr) {
        coremidi_print_error(err, "CMSimpleQueueCreate");
        return;
    }

    coremidi_init_sources();
    coremidi_init_destinations();

    if (!coremidi_create_client())
        return;

    if (!coremidi_create_virtual_source())
        return;

    if (!coremidi_create_input_port())
        return;

    if (!coremidi_create_virtual_destination())
        return;

    if (!coremidi_create_output_port())
        return;

    done = TRUE;
}

CFStringRef coremidi_copy_device_name(MIDIEndpointRef dev)
{
    CFStringRef dev_name = NULL;
    OSStatus rc = MIDIObjectGetStringProperty(dev, kMIDIPropertyDisplayName, &dev_name);
    if (rc != noErr) { // virtual sources has no display name properties
        rc = MIDIObjectGetStringProperty(dev, kMIDIPropertyName, &dev_name);
        if (rc != noErr) {
            coremidi_print_error(rc, "MIDIObjectGetStringProperty");
            return NULL;
        }
    }

    CFMutableStringRef safe_dev_name = CFStringCreateMutableCopy(kCFAllocatorDefault, 256, dev_name);
    CFRange range = { 0, CFStringGetLength(safe_dev_name) };
    CFStringFindAndReplace(safe_dev_name, CFSTR("{"), CFSTR("("), range, kCFCompareEqualTo);
    CFStringFindAndReplace(safe_dev_name, CFSTR("}"), CFSTR(")"), range, kCFCompareEqualTo);
    return safe_dev_name;
}

void coremidi_getindevs(char* indevlist, int* nindevs, int maxndev, int devdescsize)
{
    int n = 0;

    for (int i = 0; i < coremidi_nsrc && i < maxndev; i++) {
        CFStringRef dev_name = coremidi_copy_device_name(coremidi_src[i].ref);
        if (!dev_name)
            break;

        char* pname = &indevlist[devdescsize * n++];

        CFRange range = { 0, CFStringGetLength(dev_name) };
        CFIndex nbytes = 0;
        CFStringGetBytes(dev_name, range, kCFStringEncodingUTF8, 0, false, (UInt8*)pname, devdescsize - 1, &nbytes);
        pname[nbytes] = '\0';
        CFRelease(dev_name);

        if (sys_verbose)
            post("[coremidi] (%d) source: %s", n, pname);
    }

    *nindevs = n;
}

void coremidi_getoutdevs(char* outdevlist, int* noutdevs, int maxndev, int devdescsize)
{
    int n = 0;

    for (int i = 0; i < coremidi_ndest && i < maxndev; i++) {
        CFStringRef dev_name = coremidi_copy_device_name(coremidi_dest[i]);
        if (!dev_name)
            break;

        char* pname = &outdevlist[devdescsize * n++];

        CFRange range = { 0, CFStringGetLength(dev_name) };
        CFIndex nbytes = 0;
        CFStringGetBytes(dev_name, range, kCFStringEncodingUTF8, 0, false, (UInt8*)pname, devdescsize - 1, &nbytes);
        pname[nbytes] = '\0';
        CFRelease(dev_name);

        if (sys_verbose)
            post("[coremidi] (%d) destination: %s", n, pname);
    }

    *noutdevs = n;
}

Boolean coremidi_create_client()
{
    OSStatus err = MIDIClientCreate(CFSTR("PureData MIDI"), coremidi_notify, NULL, &coremidi_client);
    if (err != noErr) {
        coremidi_print_error(err, "MIDIClientCreate");
        return FALSE;
    }

    return TRUE;
}

Boolean coremidi_create_virtual_source()
{
    OSStatus err = MIDISourceCreate(coremidi_client, CFSTR("from PureData"), &virtual_src_output);
    if (err != noErr) {
        coremidi_print_error(err, "MIDISourceCreate");
        return FALSE;
    }

    // append to destination list
    coremidi_append_dest_device(virtual_src_output);

    // set unique id
    MIDIObjectSetIntegerProperty(virtual_src_output, kMIDIPropertyUniqueID, coremidi_unique_id(TRUE));
    return TRUE;
}

Boolean coremidi_create_virtual_destination()
{
    OSStatus err = MIDIDestinationCreate(coremidi_client, CFSTR("to PureData"), coremidi_read, NULL, &virtual_dest_input);
    if (err != noErr) {
        coremidi_print_error(err, "MIDIDestinationCreate");
        return FALSE;
    }

    // append to input sources
    coremidi_append_src_device(virtual_dest_input);

    // set unique id
    MIDIObjectSetIntegerProperty(virtual_dest_input, kMIDIPropertyUniqueID, coremidi_unique_id(FALSE));
    return TRUE;
}

Boolean coremidi_create_input_port()
{
    OSStatus err = MIDIInputPortCreate(coremidi_client, CFSTR("Pd input"), coremidi_read, NULL, &input_port);
    if (err != noErr) {
        coremidi_print_error(err, "MIDIInputPortCreate");
        return FALSE;
    }

    return TRUE;
}

Boolean coremidi_create_output_port()
{
    OSStatus err = MIDIOutputPortCreate(coremidi_client, CFSTR("Pd output"), &output_port);
    if (err != noErr) {
        coremidi_print_error(err, "MIDIOutputPortCreate");
        return FALSE;
    }

    return TRUE;
}

Boolean coremidi_connect_source(DeviceInfo* dev, int portno)
{
    OSStatus err = MIDIPortConnectSource(input_port, dev->ref, dev);
    if (err != noErr) {
        coremidi_print_error(err, "MIDIPortConnectSource");
        return FALSE;
    }

    dev->is_connected = TRUE;
    dev->portno = portno;

    if (sys_verbose) {
        CFStringRef str = coremidi_copy_device_name(dev->ref);

        UInt8 buf[255];
        CFRange range = { 0, CFStringGetLength(str) };
        CFIndex nbytes = 0;
        CFStringGetBytes(str, range, kCFStringEncodingUTF8, 0, false, buf, sizeof(buf) - 1, &nbytes);
        buf[nbytes] = 0;
        CFRelease(str);

        post("[coremidi] connect to: %s", buf);
    }

    return TRUE;
}

void coremidi_connect_sources(int nmidiin, int* midiinvec)
{
    virtual_dest_input_portno = -1;

    for (int i = 0; i < nmidiin; i++) {
        int dev_idx = midiinvec[i];
        if (dev_idx < 0 || dev_idx >= coremidi_nsrc) {
            pd_error(0, "[coremidi] invalid device index: %d", dev_idx);
            continue;
        }

        if (coremidi_src[dev_idx].ref == virtual_dest_input) {
            // skip virtual destination and set portno for virtual input
            virtual_dest_input_portno = i;
            continue;
        }

        if (!coremidi_connect_source(&coremidi_src[dev_idx], i))
            continue;
    }
}

void coremidi_disconnect_sources()
{
    for (size_t i = 0; i < coremidi_nsrc; i++) {
        if (!coremidi_src[i].is_connected)
            continue;

        OSStatus err = MIDIPortDisconnectSource(input_port, coremidi_src[i].ref);
        if (err != noErr) {
            coremidi_print_error(err, "MIDIPortDisconnectSource");
            pd_error(0, "[coremidi] can't disconnect from source: %d", (int)i);
            continue;
        }

        coremidi_src[i].is_connected = FALSE;

        if (sys_verbose)
            post("[coremidi] disconnect from: %d", (int)i);
    }
}

void coremidi_send_packet_list(MIDIEndpointRef dest, MIDIPacketList* pktlst)
{
    if (dest == virtual_src_output) { // sending to virtual source is a special case
        pktlst->packet[0].timeStamp = mach_absolute_time();
        OSStatus err = MIDIReceived(virtual_src_output, pktlst);
        if (err != noErr)
            coremidi_print_error(err, "MIDIReceived");
    } else {
        OSStatus err = MIDISend(output_port, dest, pktlst);
        if (err != noErr)
            coremidi_print_error(err, "MIDISend");
    }
}

void sys_do_open_midi(int nmidiin, int* midiinvec, int nmidiout, int* midioutvec)
{
    if (sys_verbose)
        post("[coremidi] %s", __FUNCTION__);

    coremidi_connect_sources(nmidiin, midiinvec);

    // map destinations
    // clean
    for (int i = 0; i < MAX_MIDI_MAP_SIZE; i++)
        portno_to_dest_index_map[i] = -1;

    // map and init fsm
    for (int i = 0; i < nmidiout && i < MAX_MIDI_MAP_SIZE; i++) {
        portno_to_dest_index_map[i] = midioutvec[i];
        midi_parser_init(&dest_midi_parsers[i], coremidi_dest[portno_to_dest_index_map[i]]);
    }
}

void sys_close_midi(void)
{
    if (sys_verbose)
        post("[coremidi] %s", __FUNCTION__);

    coremidi_disconnect_sources();

    // clear queue
    OSStatus err = CMSimpleQueueReset(input_queue);
    if (err != noErr)
        coremidi_print_error(err, "CMSimpleQueueReset");
}

void sys_putmidimess(int portno, int a, int b, int c)
{
    if (sys_verbose)
        post("[coremidi] %s: %d msg: [%d %d %d]", __FUNCTION__, portno, a, b, c);

    if (portno < 0 || portno >= MAX_MIDI_MAP_SIZE)
        return;

    // get destination index by portno
    int devidx = portno_to_dest_index_map[portno];
    if (devidx < 0 || devidx >= coremidi_ndest)
        return;

    MIDIPacketList packetlist;
    MIDIPacket* packet = MIDIPacketListInit(&packetlist);

    Byte msg[3] = { (Byte)a, (Byte)b, (Byte)c };
    ByteCount msg_len = sizeof(msg);
    const int msgtype = ((a & 0xf0) == 0xf0 ? a : (a & 0xf0));
    switch (msgtype) {
    /* 2 data bytes */
    case MIDI_NOTEOFF:
    case MIDI_NOTEON:
    case MIDI_POLYAFTERTOUCH:
    case MIDI_CONTROLCHANGE:
    case MIDI_PITCHBEND:
    case MIDI_SONGPOS:
        msg_len = 3;
        break;
    /* 1 data byte */
    case MIDI_PROGRAMCHANGE:
    case MIDI_AFTERTOUCH:
    case MIDI_TIMECODE:
    case MIDI_SONGSELECT:
        msg_len = 2;
        break;
    default:
        msg_len = 1;
        break;
    }

    MIDIPacketListAdd(&packetlist, sizeof(packetlist), packet, 0, msg_len, msg);
    coremidi_send_packet_list(coremidi_dest[devidx], &packetlist);
}

void sys_putmidibyte(int portno, int byte)
{
    if (sys_verbose)
        post("[coremidi] %s: %d [%d]", __FUNCTION__, portno, byte);

    if (portno < 0 || portno >= coremidi_ndest)
        return;

    // get destination index by portno
    int devidx = portno_to_dest_index_map[portno];
    if (devidx < 0 || devidx >= coremidi_ndest)
        return;

    midi_parser_input(&dest_midi_parsers[portno], byte);
}

void sys_poll_midi(void)
{
    const void* el = NULL;
    while ((el = CMSimpleQueueDequeue(input_queue)) != NULL) {
        // unpack midi value
        UInt64 v = (((UInt64)el) >> 16);
        UInt64 p = (v >> 8);
        sys_midibytein(p & 0xff, v & 0xff);
    }
}

void midi_getdevs(char* indevlist, int* nindevs,
    char* outdevlist, int* noutdevs, int maxndev, int devdescsize)
{
    if (sys_verbose)
        post("[coremidi] %s", __FUNCTION__);

    coremidi_init();

    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true);

    coremidi_getindevs(indevlist, nindevs, maxndev, devdescsize);
    coremidi_getoutdevs(outdevlist, noutdevs, maxndev, devdescsize);
}
