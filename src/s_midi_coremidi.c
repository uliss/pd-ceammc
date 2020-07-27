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
    MIDI_QUEUE_SIZE = 1024,
    MAX_COREMIDI_DEVS = 256,
    MAX_MIDI_MAP_SIZE = 16
};

typedef struct DeviceInfo {
    MIDIEntityRef ref;
    Byte portno;
    Boolean is_connected;
} DeviceInfo;

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
static CFStringRef coremidi_get_device_name(MIDIEndpointRef dev);
static Boolean coremidi_create_client();
static Boolean coremidi_create_virtual_source();
static Boolean coremidi_create_virtual_destination();
static Boolean coremidi_create_input_port();
static Boolean coremidi_create_output_port();
static void coremidi_connect_sources(int nmidiin, int* midiinvec);
static Boolean coremidi_connect_source(DeviceInfo* dev, int portno);
static void coremidi_disconnect_sources();
static void coremidi_dispose_input_port();
static void coremidi_dispose_output_port();
static void coremidi_dispose_virtual_source();
static void coremidi_dispose_virtual_destination();
static void coremidi_send_packet_list(MIDIEntityRef dest, MIDIPacketList* pktlst);

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

    error("[coremidi] system error: %s (%s)", operation, error_str);
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
            error("[coremidi] can't open source: %d", (int)i);
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
            error("[coremidi] can't open destination: %d", (int)i);
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
        case kMIDIObjectType_Destination: {
            coremidi_append_dest_device(add_remove_msg->child);
        } break;
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

    done = TRUE;
}

CFStringRef coremidi_get_device_name(MIDIEndpointRef dev)
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

    return dev_name;
}

void coremidi_getindevs(char* indevlist, int* nindevs, int maxndev, int devdescsize)
{
    int n = 0;

    for (int i = 0; i < coremidi_nsrc && i < maxndev; i++) {
        CFStringRef dev_name = coremidi_get_device_name(coremidi_src[i].ref);
        if (!dev_name)
            break;

        char* pname = &indevlist[devdescsize * n++];
        CFStringGetCString(dev_name, pname, devdescsize, kCFStringEncodingUTF8);
        if (sys_verbose)
            post("[coremidi] (%d) source: %s", n, pname);
    }

    *nindevs = n;
}

void coremidi_getoutdevs(char* outdevlist, int* noutdevs, int maxndev, int devdescsize)
{
    int n = 0;

    for (int i = 0; i < coremidi_ndest && i < maxndev; i++) {
        CFStringRef dev_name = coremidi_get_device_name(coremidi_dest[i]);
        if (!dev_name)
            break;

        char* pname = &outdevlist[devdescsize * n++];
        CFStringGetCString(dev_name, pname, devdescsize, kCFStringEncodingUTF8);
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
        CFStringRef str = coremidi_get_device_name(dev->ref);

        char buf[255];
        CFStringGetCString(str, buf, sizeof(buf), kCFStringEncodingUTF8);
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
            error("[coremidi] invalid device index: %d", dev_idx);
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
            error("[coremidi] can't disconnect from source: %d", (int)i);
            continue;
        }

        coremidi_src[i].is_connected = FALSE;

        if (sys_verbose)
            post("[coremidi] disconnect from: %d", (int)i);
    }
}

void coremidi_dispose_input_port()
{
    if (input_port) {
        OSStatus err = MIDIPortDispose(input_port);
        if (err != noErr)
            coremidi_print_error(err, "MIDIPortDispose");

        input_port = 0;
    }
}

void coremidi_dispose_output_port()
{
    if (output_port) {
        OSStatus err = MIDIPortDispose(output_port);
        if (err != noErr)
            coremidi_print_error(err, "MIDIPortDispose");

        output_port = 0;
    }
}

void coremidi_dispose_virtual_source()
{
    if (virtual_src_output) {
        OSStatus err = MIDIEndpointDispose(virtual_src_output);
        if (err != noErr)
            coremidi_print_error(err, "MIDIEndpointDispose");

        coremidi_remove_dest_device(virtual_src_output);
        virtual_src_output = 0;
    }
}

void coremidi_dispose_virtual_destination()
{
    if (virtual_dest_input) {
        OSStatus err = MIDIEndpointDispose(virtual_dest_input);
        if (err != noErr)
            coremidi_print_error(err, "MIDIEndpointDispose");

        coremidi_remove_src_device(virtual_dest_input);
        virtual_dest_input = 0;
    }
}

void coremidi_dispose_client()
{
    if (coremidi_client) {
        OSStatus err = MIDIClientDispose(coremidi_client);
        if (err != noErr)
            coremidi_print_error(err, "MIDIClientDispose");

        coremidi_client = 0;
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

    coremidi_connect_sources(nmidiin, midiinvec);

    // map destinations
    // clean
    for (int i = 0; i < MAX_MIDI_MAP_SIZE; i++)
        portno_to_dest_index_map[i] = -1;

    // map
    for (int i = 0; i < nmidiout && i < MAX_MIDI_MAP_SIZE; i++)
        portno_to_dest_index_map[i] = midioutvec[i];
}

void sys_close_midi(void)
{
    coremidi_disconnect_sources();
    coremidi_dispose_output_port();
    coremidi_dispose_virtual_destination();
    coremidi_dispose_input_port();
    coremidi_dispose_virtual_source();
    coremidi_dispose_client();

    // queue
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
    MIDIPacketListAdd(&packetlist, sizeof(packetlist), packet, 0, sizeof(msg), msg);

    coremidi_send_packet_list(coremidi_dest[devidx], &packetlist);
}

void sys_putmidibyte(int portno, int byte)
{
    if (sys_verbose)
        post("[coremidi] %s: %d", __FUNCTION__, portno);

    if (portno < 0 || portno >= coremidi_ndest)
        return;

    MIDIPacketList packetlist;
    MIDIPacket* packet = MIDIPacketListInit(&packetlist);
    Byte msg = byte;
    MIDIPacketListAdd(&packetlist, sizeof(packetlist), packet, 0, sizeof(msg), &msg);

    coremidi_send_packet_list(coremidi_dest[portno], &packetlist);
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
