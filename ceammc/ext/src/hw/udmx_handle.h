/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef UDMX_HANDLE_H
#define UDMX_HANDLE_H

#include "usb.h"

#include <vector>

typedef std::vector<unsigned char> ByteList;

/*
 *  usb2dmx_cmds.h
 *
 *
 *  Created by Max Egger on 14.02.06.
 *
 */

#define cmd_SetSingleChannel 1
/* usb request for cmd_SetSingleChannel:
    bmRequestType:	ignored by device, should be USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT
    bRequest:		cmd_SetSingleChannel
    wValue:			value of channel to set [0 .. 255]
    wIndex:			channel index to set [0 .. 511]
    wLength:		ignored
*/
#define cmd_SetChannelRange 2
/* usb request for cmd_SetChannelRange:
    bmRequestType:	ignored by device, should be USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT
    bRequest:		cmd_SetChannelRange
    wValue:			number of channels to set [1 .. 512-wIndex]
    wIndex:			index of first channel to set [0 .. 511]
    wLength:		length of data, must be >= wValue
*/

#define cmd_StartBootloader 0x32
// Start Bootloader for Software updates

#define err_BadChannel 1
#define err_BadValue 2

class uDMX_Handle {
    usb_dev_handle* handle_;

public:
    uDMX_Handle();
    ~uDMX_Handle();

    bool open();
    bool close();
    bool findDevice();
    bool sendByte(int b, int ch);
    bool send(const ByteList& data, int ch);
};

#endif // UDMX_HANDLE_H
