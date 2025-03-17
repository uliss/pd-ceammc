#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::ptr::null_mut;

use log::error;

use crate::{hw_msg_cb, hw_notify_cb};

#[cfg(target_os = "linux")]
pub mod i2c_impl;

pub const HW_I2C_DEFAULT_BUS: i8 = -1;
pub const HW_I2C_MIN_BUS: i8 = -1;
pub const HW_I2C_MAX_BUS: i8 = 15;

pub const HW_I2C_DEFAULT_ADDR: i8 = -1;
pub const HW_I2C_ALT_ADDR: i8 = -2;
pub const HW_I2C_MIN_ADDR: i8 = HW_I2C_ALT_ADDR;
pub const HW_I2C_MAX_ADDR: i8 = 0x77;

#[derive(Debug)]
pub enum I2cAddress {
    Default,
    Alt,
    Invalid(i8),
    Addr(u8),
}

impl I2cAddress {
    pub fn new(addr: i8) -> I2cAddress {
        match addr {
            HW_I2C_DEFAULT_ADDR => I2cAddress::Default,
            HW_I2C_ALT_ADDR => I2cAddress::Alt,
            addr if addr > 0x7 && addr < 0x78 => I2cAddress::Addr(addr as u8),
            _ => I2cAddress::Invalid(addr),
        }
    }
}

pub enum Request {
    ScanDevices,
}

pub struct hw_i2c {
    // result: Arc<(Mutex<Option<Reply>>, std::sync::Condvar)>,
    _tx: std::sync::mpsc::Sender<Request>,
    _on_err: hw_msg_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_i2c_new(
    addr: u8,
    notify: hw_notify_cb,
    on_msg: hw_msg_cb,
) -> *mut hw_i2c {
    rpi_check!(null_mut(), {
        match hw_i2c::new(addr, notify, on_msg) {
            Ok(i2c) => return Box::into_raw(Box::new(i2c)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_msg.error_cstr(err);
                return null_mut();
            }
        }
    });
}

/// free i2c struct
/// @param i2c - pointer to i2c struct
#[no_mangle]
pub extern "C" fn ceammc_hw_i2c_free(i2c: *mut hw_i2c) {
    rpi_check!((), {
        if !i2c.is_null() {
            drop(unsafe { Box::from_raw(i2c) })
        }
    });
}
