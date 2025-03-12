#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{ffi::CString, ptr::null_mut};

use log::error;

use crate::{hw_msg_cb, hw_notify_cb, i2c::I2cAddress, HwThreadWorker, MakePdError};

#[cfg(target_os = "linux")]
mod ads1115_impl;

type Ads1115Worker = HwThreadWorker<Request, Reply>;

pub struct hw_i2c_ads1115 {
    worker: Ads1115Worker,
}
pub enum Request {
    Measure(u8, bool),
    MeasureAll(bool),
    SetFullScaleRange(u8),
}

pub enum Reply {
    Error(CString),
    Measure(u8, i16),
    MeasureAll([i16; 4]),
}

impl MakePdError<Reply> for Reply {
    fn pd_err(msg: CString) -> Reply {
        Reply::Error(msg)
    }
}

#[no_mangle]
pub extern "C" fn ceammc_hw_ads1115_new(
    i2c_bus: i8,
    i2c_addr: i8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_i2c_ads1115 {
    rpi_check!(null_mut(), {
        match hw_i2c_ads1115::new_oneshot(i2c_bus, I2cAddress::new(i2c_addr), notify, on_err) {
            Ok(adc) => return Box::into_raw(Box::new(adc)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_ads1115_free(adc: *mut hw_i2c_ads1115) {
    rpi_check!((), {
        if !adc.is_null() {
            drop(unsafe { Box::from_raw(adc) })
        }
    });
}
