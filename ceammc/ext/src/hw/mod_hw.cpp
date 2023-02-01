#include "mod_hw.h"
#include "hw_apple_smc.h"
#include "hw_apple_sms.h"
#include "hw_arduino.h"
#include "hw_display.h"
#include "hw_kbd_light.h"
#include "hw_serial.h"

#ifdef WITH_LIBUSB_01
#include "hw_udmx.h"
#endif

#ifdef WITH_LIBUSB_1
#include "hw_usb.h"
#endif

void setup_hw_cpu_temp();
void setup_hw_motu_avb();

void ceammc_hw_setup()
{
    hw_setup_arduino();
    hw_setup_serial();
    setup_hw_apple_smc();
    setup_hw_apple_sms();
    setup_hw_cpu_temp();
    setup_hw_display();
    setup_hw_keyboard_light();
    setup_hw_motu_avb();

#ifdef WITH_LIBUSB_01
    setup_hw_udmx();
#endif

#ifdef WITH_LIBUSB_1
    setup_hw_usb();
#endif
}
