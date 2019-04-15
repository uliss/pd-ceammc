#include "mod_hw.h"
#include "hw_arduino.h"
#include "hw_serial.h"
#include "hw_udmx.h"
#include "hw_usb.h"

void setup_hw_apple_smc();
void setup_hw_apple_sms();
void setup_hw_cpu_temp();
void setup_hw_display();
void setup_hw_keyboard_light();

void ceammc_hw_setup()
{
    hw_setup_arduino();
    hw_setup_serial();
    setup_hw_apple_smc();
    setup_hw_apple_sms();
    setup_hw_cpu_temp();
    setup_hw_display();
    setup_hw_keyboard_light();

#ifdef WITH_LIBUSB_01
    setup_hw_udmx();
#endif

#ifdef WITH_LIBUSB_1
    setup_hw_usb();
#endif
}
