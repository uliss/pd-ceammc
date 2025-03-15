#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{
    ffi::{c_void, CString},
    ptr::null_mut,
};

use lib_macro::PdError;
use log::error;

use crate::{
    hw_msg_cb, hw_msg_level, hw_notify_cb, i2c::I2cAddress, HwThreadWorker, MakePdMessage,
};

#[cfg(target_os = "linux")]
mod mpu6050_impl;

type Mpu6050Worker = HwThreadWorker<Request, Reply>;

#[repr(C)]
pub struct hw_mpu6050_data_cb {
    user: *mut c_void,
    cb_ypr: extern "C" fn(user: *mut c_void, yaw: f32, pitch: f32, roll: f32),
    cb_temp: extern "C" fn(user: *mut c_void, temp: f32),
}

pub struct hw_mpu6050 {
    worker: Mpu6050Worker,
    on_data: hw_mpu6050_data_cb,
}

#[derive(Debug)]
pub enum Request {
    Poll(bool),
    Calibrate,
}

#[derive(PdError)]
pub enum Reply {
    Message(hw_msg_level, CString),
    YawPitchRoll(f32, f32, f32),
    Temperature(f32),
}

#[no_mangle]
pub extern "C" fn ceammc_hw_mpu6050_new(
    i2c_bus: i8,
    i2c_addr: i8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
    on_data: hw_mpu6050_data_cb,
) -> *mut hw_mpu6050 {
    rpi_check!(null_mut(), {
        match hw_mpu6050::new(i2c_bus, I2cAddress::new(i2c_addr), notify, on_err, on_data) {
            Ok(mpu) => return Box::into_raw(Box::new(mpu)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.error_cstr(err);
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

#[no_mangle]
pub extern "C" fn ceammc_hw_mpu6050_process_reply(mpu: *mut hw_mpu6050) -> bool {
    rpi_check!({ hw_mpu6050::process_reply_ptr(mpu) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_mpu6050_poll(mpu: *mut hw_mpu6050, state: bool) -> bool {
    rpi_check!({ hw_mpu6050::send_request_ptr(mpu, Request::Poll(state)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_mpu6050_calibrate(mpu: *mut hw_mpu6050) -> bool {
    rpi_check!({ hw_mpu6050::send_request_ptr(mpu, Request::Calibrate) });
}
