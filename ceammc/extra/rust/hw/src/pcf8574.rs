use crate::i2c::I2cAddress;
use ceammc_rs_msg::{msg_cb, msg_notify};
use std::ffi::c_void;
use std::ptr::null_mut;

pub enum Request {
    SetPins(u8),
    GetPins { mask: u8 },
}

pub enum Reply {
    InputPins { mask: u8, state: u8 },
}

#[allow(non_camel_case_types)]
pub struct hw_pcf_8574_cb {
    user: *mut c_void,
    on_input: extern "C" fn(user: *mut c_void, mask: u8, state: u8),
}

impl hw_pcf_8574_cb {
    pub fn input_pins(&self, mask: u8, state: u8) {
        (self.on_input)(self.user, mask, state)
    }
}

#[allow(non_camel_case_types)]
/// The PCF8574 is an 8-bit I/O expander for I²C-bus.
/// It supports up to 8 devices on the same bus via programmable addresses.
pub struct hw_pcf_8574 {
    pub worker: ceammc_rs_msg::Client<Request, Reply>,
    pub cb: hw_pcf_8574_cb,
}

#[no_mangle]
/// create new GPIO expander device
/// @param i2c_bus - i2c bus number
/// @param i2c_addr - i2c device address
/// @param notify - caller notify callback
/// @param on_msg - caller callback on message from worker
/// @param on_data - caller callback on data from worker
/// @return pointer to device or nullptr on error
pub fn ceammc_hw_pcf8574_new(
    i2c_bus: i8,
    i2c_addr: i8,
    notify: msg_notify,
    on_msg: msg_cb,
    on_data: hw_pcf_8574_cb,
) -> *mut hw_pcf_8574 {
    rpi_check!(null_mut(), {
        match hw_pcf_8574::new(i2c_bus, I2cAddress::new(i2c_addr), notify, on_msg, on_data) {
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
pub extern "C" fn ceammc_hw_pcf8674_free(dev: *mut hw_pcf_8574) {
    rpi_check!((), {
        if !dev.is_null() {
            drop(unsafe { Box::from_raw(dev) })
        }
    });
}

#[no_mangle]
/// process replies from worker
/// @param dev - device handle (nullable)
pub extern "C" fn ceammc_hw_pcf8574_process_reply(dev: *mut hw_pcf_8574) -> bool {
    rpi_check!({ hw_pcf_8574::process_reply_ptr(dev) });
}
