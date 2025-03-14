#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{ffi::CString, ptr::null_mut};

use lib_macro::PdError;
use log::error;

use crate::{hw_msg_cb, hw_notify_cb, i2c::I2cAddress, HwThreadWorker, MakePdError};

#[cfg(target_os = "linux")]
mod mpu6050_impl;

type Mpu6050Worker = HwThreadWorker<Request, Reply>;

pub struct hw_mpu6050 {
    worker: Mpu6050Worker,
}

#[derive(Debug)]
pub enum Request {
    MeasureChan(u8),
}

#[derive(PdError)]
pub enum Reply {
    Error(CString),
}

#[no_mangle]
pub extern "C" fn ceammc_hw_mpu6050_new(
    i2c_bus: i8,
    i2c_addr: i8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_mpu6050 {
    rpi_check!(null_mut(), {
        match hw_mpu6050::new(i2c_bus, I2cAddress::new(i2c_addr), notify, on_err) {
            Ok(mpu) => return Box::into_raw(Box::new(mpu)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_mpu6050_free(mpu: *mut hw_mpu6050) {
    rpi_check!((), {
        if !mpu.is_null() {
            drop(unsafe { Box::from_raw(mpu) })
        }
    });
}
