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
#include "udmx_handle.h"
#include "ceammc_convert.h"
#include "ceammc_log.h"
#include "m_pd.h"

using namespace ceammc;

#define USBDEV_SHARED_VENDOR 0x16C0 /* VOTI */
#define USBDEV_SHARED_PRODUCT 0x05DC

#define DMX_ERR LIB_ERR << "uDMX: "
#define DMX_DBG LIB_DBG << "uDMX: "

static int usbGetStringAscii(usb_dev_handle* dev, int index, int langid, char* buf, int buflen)
{
    char buffer[256];
    int rval, i;

    if ((rval = usb_control_msg(dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR, (USB_DT_STRING << 8) + index, langid, buffer, sizeof(buffer), 1000)) < 0)
        return rval;
    if (buffer[1] != USB_DT_STRING)
        return 0;
    if ((unsigned char)buffer[0] < rval)
        rval = (unsigned char)buffer[0];
    rval /= 2;
    /* lossy conversion to ISO Latin1 */
    for (i = 1; i < rval; i++) {
        if (i > buflen) /* destination buffer overflow */
            break;
        buf[i - 1] = buffer[2 * i];
        if (buffer[2 * i + 1] != 0) /* outside of ISO Latin1 range */
            buf[i - 1] = '?';
    }
    buf[i - 1] = 0;
    return i - 1;
}

uDMX_Handle::uDMX_Handle()
    : handle_(nullptr)
{
}

uDMX_Handle::~uDMX_Handle()
{
    close();
}

bool uDMX_Handle::open()
{
    if (handle_) {
        LIB_ERR << "uDMX: already connected";
        return false;
    }

    return findDevice();
}

bool uDMX_Handle::close()
{
    if (handle_) {
        DMX_DBG << "uDMX: closing device";
        usb_close(handle_);
        handle_ = nullptr;
        return true;
    }

    return false;
}

bool uDMX_Handle::findDevice()
{
    if (handle_ != nullptr)
        usb_close(handle_);

    handle_ = nullptr;
    struct usb_bus* bus;
    struct usb_device* dev;

    usb_find_busses();
    usb_find_devices();
    for (bus = usb_busses; bus; bus = bus->next) {
        for (dev = bus->devices; dev; dev = dev->next) {
            if (dev->descriptor.idVendor == USBDEV_SHARED_VENDOR
                && dev->descriptor.idProduct == USBDEV_SHARED_PRODUCT) {

                char string[256];
                int len;
                handle_ = usb_open(dev); /* we need to open the device in order to query strings */
                if (!handle_) {
                    pd_error(0, "Warning: cannot open USB device: %s", usb_strerror());
                    continue;
                }
                /* now find out whether the device actually is obdev's Remote Sensor: */
                len = usbGetStringAscii(handle_, dev->descriptor.iManufacturer, 0x0409, string, sizeof(string));
                if (len < 0) {
                    post("uDMX: warning: cannot query manufacturer for device: %s", usb_strerror());
                    goto skipDevice;
                }
                /* post("uDMX: seen device from vendor ->%s<-", string); */
                if (strcmp(string, "www.anyma.ch") != 0)
                    goto skipDevice;
                len = usbGetStringAscii(handle_, dev->descriptor.iProduct, 0x0409, string, sizeof(string));
                if (len < 0) {
                    post("uDMX: warning: cannot query product for device: %s", usb_strerror());
                    goto skipDevice;
                }
                /* post("uDMX: seen product ->%s<-", string); */
                if (strcmp(string, "uDMX") == 0)
                    break;
            skipDevice:
                usb_close(handle_);
                handle_ = NULL;
            }
        }
        if (handle_)
            break;
    }

    if (!handle_)
        return false;
    else {
        LIB_DBG << "connected to uDMX";
        return true;
    }
}

bool uDMX_Handle::sendByte(int n, int ch)
{
    if (!handle_) {
        // try to connect
        auto ok = findDevice();
        if (!ok) {
            DMX_ERR << "send failed";
            return false;
        }
    }

    n = clip<int>(n, 0, 255);
    DMX_DBG << "send " << n << "->" << ch;

    char buffer[4];
    int nBytes = usb_control_msg(handle_, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
        cmd_SetSingleChannel, n, ch, buffer, sizeof(buffer), 5000);

    if (nBytes < 0) {
        DMX_ERR << "USB error: " << usb_strerror();
        return false;
    }

    return true;
}

bool uDMX_Handle::send(const ByteList& data, int ch)
{
    if (!handle_) {
        // try to connect
        auto ok = findDevice();
        if (!ok) {
            DMX_ERR << "send failed";
            return false;
        }
    }

    DMX_DBG << "send: ";
    for (auto& b : data)
        DMX_DBG << b;

    int nBytes = usb_control_msg(handle_, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
        cmd_SetChannelRange, data.size(), ch, (char*)data.data(), data.size(), 5000);

    if (nBytes < 0)
        return false;

    return true;
}
