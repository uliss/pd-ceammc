#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use crate::gpio::hw_gpio_mode;
use crate::i2c::I2cAddress;
use ceammc_rs_msg::{msg_cb, msg_notify};
use std::ffi::c_void;
use std::ptr::null_mut;

#[derive(Debug)]
pub enum Request {
    ConfigPin(u8, hw_gpio_mode),
    WriteAllPins(u8),
    WritePin { pin: u8, value: bool },
    ReadPin(u8),
    ReadAllPins,
}

pub enum Reply {
    ReadAll { mask: u8, state: u8 },
    ReadPin { pin: u8, state: bool },
}

#[repr(C)]
pub struct hw_pcf8574_cb {
    user: *mut c_void,
    on_read_all: extern "C" fn(user: *mut c_void, mask: u8, state: u8),
    on_read_pin: extern "C" fn(user: *mut c_void, pin: u8, state: bool),
}

impl hw_pcf8574_cb {
    pub fn read_all(&self, mask: u8, state: u8) {
        (self.on_read_all)(self.user, mask, state)
    }

    pub fn read_pin(&self, pin: u8, state: bool) {
        (self.on_read_pin)(self.user, pin, state)
    }
}

/// The PCF8574 is an 8-bit I/O expander for I²C-bus.
/// It supports up to 8 devices on the same bus via programmable addresses.
pub struct hw_pcf8574 {
    pub worker: ceammc_rs_msg::Client<Request, Reply>,
    pub cb: hw_pcf8574_cb,
}

#[no_mangle]
/// create new GPIO expander device
/// @param i2c_bus - i2c bus number
/// @param i2c_addr - i2c device address
/// @param pin_interrupt - interrupt GPIO pin
/// @param clear_on_exit - set pins to low after exit
/// @param notify - caller notify callback
/// @param on_msg - caller callback on message from worker
/// @param on_data - caller callback on data from worker
/// @return pointer to device or nullptr on error
pub extern "C" fn ceammc_hw_pcf8574_new(
    i2c_bus: i8,
    i2c_addr: i8,
    pin_interrupt: Option<&u8>,
    clear_on_exit: bool,
    notify: msg_notify,
    on_msg: msg_cb,
    on_data: hw_pcf8574_cb,
) -> *mut hw_pcf8574 {
    rpi_check!(null_mut(), {
        match hw_pcf8574::new(
            i2c_bus,
            I2cAddress::new(i2c_addr),
            pin_interrupt.cloned(),
            clear_on_exit,
            notify,
            on_msg,
            on_data,
        ) {
            Ok(adc) => return Box::into_raw(Box::new(adc)),
            Err(err) => {
                on_msg.error_cstr(err);
                return null_mut();
            }
        }
    });
}

#[no_mangle]
/// free device
/// @param dev - device handle (nullable)
pub extern "C" fn ceammc_hw_pcf8674_free(dev: *mut hw_pcf8574) {
    rpi_check!((), {
        if !dev.is_null() {
            drop(unsafe { Box::from_raw(dev) })
        }
    });
}

#[no_mangle]
/// process replies from worker
/// @param dev - device handle (nullable)
pub extern "C" fn ceammc_hw_pcf8574_process_reply(dev: *mut hw_pcf8574) -> bool {
    rpi_check!({ hw_pcf8574::process_reply_ptr(dev) });
}

#[no_mangle]
/// write value to all pins configured for output
/// @param dev - device handle (nullable)
/// @param value - value for all pins
pub extern "C" fn ceammc_hw_pcf8674_write_all(dev: *mut hw_pcf8574, value: u8) -> bool {
    rpi_check!({ hw_pcf8574::send_request_ptr(dev, Request::WriteAllPins(value)) });
}

#[no_mangle]
/// write value to all pins configured for output
/// @param dev - device handle (nullable)
/// @param pin - pin index
/// @param value - value
pub extern "C" fn ceammc_hw_pcf8674_write_pin(dev: *mut hw_pcf8574, pin: u8, value: bool) -> bool {
    rpi_check!({ hw_pcf8574::send_request_ptr(dev, Request::WritePin { pin, value }) });
}

#[no_mangle]
/// configure pin mode
/// @param dev - device handle
/// @param pin - pin index
/// @param mode - pin mode
pub extern "C" fn ceammc_hw_pcf8674_config_pin(dev: *mut hw_pcf8574, pin: u8, mode: hw_gpio_mode) -> bool {
    rpi_check!({ hw_pcf8574::send_request_ptr(dev, Request::ConfigPin(pin, mode)) });
}

#[no_mangle]
/// read all device pins configured for input
/// @param dev - device handle (nullable)
pub extern "C" fn ceammc_hw_pcf8674_read_all(dev: *mut hw_pcf8574) -> bool {
    rpi_check!({ hw_pcf8574::send_request_ptr(dev, Request::ReadAllPins) });
}

#[no_mangle]
/// read specified pin value
/// @param dev - device handle (nullable)
/// @param pin - pin index
pub extern "C" fn ceammc_hw_pcf8674_read_pin(dev: *mut hw_pcf8574, pin: u8) -> bool {
    rpi_check!({ hw_pcf8574::send_request_ptr(dev, Request::ReadPin(pin)) });
}
