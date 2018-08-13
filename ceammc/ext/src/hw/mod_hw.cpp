#include "mod_hw.h"
#include "hw_arduino.h"
#include "hw_udmx.h"
#include "hw_usb.h"

void ceammc_hw_setup()
{
    hw_setup_arduino();

#ifdef WITH_LIBUSB_01
    setup_hw_udmx();
#endif

#ifdef WITH_LIBUSB_1
    setup_hw_usb();
#endif
}
