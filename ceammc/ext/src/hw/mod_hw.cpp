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

void setup_hw_gamepad();
void setup_hw_motu_avb();
void setup_hw_printer();
void setup_hw_rpi_adc_ads1115();
void setup_hw_rpi_display_lcd1602();
void setup_hw_rpi_display_ssd1306();
void setup_hw_rpi_gpio();
void setup_hw_rpi_i2c_pca9685();
void setup_hw_rpi_pwm();
void setup_hw_rpi_rotenc();
void setup_hw_rpi_sensor_dht11();
void setup_hw_rpi_sensor_ir();
void setup_hw_rpi_sensor_mpu6050();
void setup_hw_rpi_sensor_sr04();
void setup_hw_rpi_sensor_vl53l0x();
void setup_hw_rpi_spi_max7219();
void setup_hw_rpi_spi_ws2812();

void ceammc_hw_setup()
{
    hw_setup_arduino();
    hw_setup_serial();

    setup_hw_apple_smc();
    setup_hw_apple_sms();
    setup_hw_display();
    setup_hw_gamepad();
    setup_hw_keyboard_light();
    setup_hw_motu_avb();
    setup_hw_printer();
    setup_hw_rpi_adc_ads1115();
    setup_hw_rpi_display_lcd1602();
    setup_hw_rpi_display_ssd1306();
    setup_hw_rpi_gpio();
    setup_hw_rpi_i2c_pca9685();
    setup_hw_rpi_pwm();
    setup_hw_rpi_rotenc();
    setup_hw_rpi_sensor_dht11();
    setup_hw_rpi_sensor_ir();
    setup_hw_rpi_sensor_mpu6050();
    setup_hw_rpi_sensor_sr04();
    setup_hw_rpi_sensor_vl53l0x();
    setup_hw_rpi_spi_max7219();
    setup_hw_rpi_spi_ws2812();

#ifdef WITH_LIBUSB_01
    setup_hw_udmx();
#endif

#ifdef WITH_LIBUSB_1
    setup_hw_usb();
#endif
}
