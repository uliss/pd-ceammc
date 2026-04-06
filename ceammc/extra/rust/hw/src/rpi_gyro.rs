#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{
    ffi::{c_void, CString},
    ptr::null_mut,
};

use ceammc_rs_msg::msg_notify;
use lib_macro::PdMessage;
use log::error;

use crate::{hw_msg_cb, hw_msg_level, i2c::I2cAddress, HwThreadWorker, MakePdMessage};

#[cfg(target_os = "linux")]
mod mpu6050_impl;

type Mpu6050Worker = HwThreadWorker<Request, Reply>;

#[repr(C)]
pub struct hw_mpu6050_data_cb {
    user: *mut c_void,
    cb_ypr: extern "C" fn(user: *mut c_void, yaw: f32, pitch: f32, roll: f32),
    cb_temp: extern "C" fn(user: *mut c_void, temp: f32),
    cb_accel: extern "C" fn(user: *mut c_void, x: f32, y: f32, z: f32),
    cb_gyro: extern "C" fn(user: *mut c_void, x: f32, y: f32, z: f32),
}

impl hw_mpu6050_data_cb {
    pub fn ypr(&self, yaw: f32, pitch: f32, roll: f32) {
        (self.cb_ypr)(self.user, yaw, pitch, roll)
    }

    pub fn accel(&self, x: f32, y: f32, z: f32) {
        (self.cb_accel)(self.user, x, y, z)
    }

    pub fn gyro(&self, x: f32, y: f32, z: f32) {
        (self.cb_gyro)(self.user, x, y, z)
    }

    pub fn temp(&self, temp: f32) {
        (self.cb_temp)(self.user, temp)
    }
}

pub struct hw_mpu6050 {
    worker: Mpu6050Worker,
    on_data: hw_mpu6050_data_cb,
}

#[derive(Debug)]
#[repr(C)]
pub enum hw_mpu6050_accel_fullscale {
    /// ±2g range (16384 LSB/g)
    G2 = 0,
    /// ±4g range (8192 LSB/g)
    G4 = 1,
    /// ±8g range (4096 LSB/g)
    G8 = 2,
    /// ±16g range (2048 LSB/g)
    G16 = 3,
}

#[derive(Debug)]
#[repr(C)]
pub enum hw_mpu6050_gyro_fullscale {
    /// ±250°/s range (131 LSB/°/s)
    Deg250 = 0,
    /// ±500°/s range (65.5 LSB/°/s)
    Deg500 = 1,
    /// ±1000°/s range (32.8 LSB/°/s)
    Deg1000 = 2,
    /// ±2000°/s range (16.4 LSB/°/s)
    Deg2000 = 3,
}

#[derive(Debug)]
pub enum Request {
    Poll(bool),
    Calibrate,
    SetAccelScale(hw_mpu6050_accel_fullscale),
    SetGyroScale(hw_mpu6050_gyro_fullscale),
}

#[derive(PdMessage)]
pub enum Reply {
    Message(hw_msg_level, CString),
    YawPitchRoll(f32, f32, f32),
    Temperature(f32),
    Accel(f32, f32, f32),
    Gyro(f32, f32, f32),
}

#[no_mangle]
/// create mpu6050 device handle
/// @return nullptr on error
/// @param i2c_bus - i2c bus
/// @param i2c_addr - i2c address
/// @param notify - notify callback
/// @param on_msg - message callback
/// @param on_data - data callback
pub extern "C" fn ceammc_hw_mpu6050_new(
    i2c_bus: i8,
    i2c_addr: i8,
    notify: msg_notify,
    on_msg: hw_msg_cb,
    on_data: hw_mpu6050_data_cb,
) -> *mut hw_mpu6050 {
    rpi_check!(null_mut(), {
        match hw_mpu6050::new(i2c_bus, I2cAddress::new(i2c_addr), notify, on_msg, on_data) {
            Ok(mpu) => return Box::into_raw(Box::new(mpu)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_msg.error_cstr(err);
                return null_mut();
            }
        }
    });
}

#[no_mangle]
/// free mpu6050 device handle
/// @param mpu - handle (nullable)
pub extern "C" fn ceammc_hw_mpu6050_free(mpu: *mut hw_mpu6050) {
    rpi_check!((), {
        if !mpu.is_null() {
            drop(unsafe { Box::from_raw(mpu) })
        }
    });
}

#[no_mangle]
/// process replies from device
/// @param mpu - handle (nullable)
pub extern "C" fn ceammc_hw_mpu6050_process_reply(mpu: *mut hw_mpu6050) -> bool {
    rpi_check!({ hw_mpu6050::process_reply_ptr(mpu) });
}

#[no_mangle]
/// turn on/off mpu6050 polling
/// @param mpu - handle (nullable)
/// @param state - poll state
pub extern "C" fn ceammc_hw_mpu6050_poll(mpu: *mut hw_mpu6050, state: bool) -> bool {
    rpi_check!({ hw_mpu6050::send_request_ptr(mpu, Request::Poll(state)) });
}

#[no_mangle]
/// starts device calibration
/// @param mpu - handle (nullable)
pub extern "C" fn ceammc_hw_mpu6050_calibrate(mpu: *mut hw_mpu6050) -> bool {
    rpi_check!({ hw_mpu6050::send_request_ptr(mpu, Request::Calibrate) });
}

#[no_mangle]
/// set device accel sensitivity
/// @param mpu - handle (nullable)
pub extern "C" fn ceammc_hw_mpu6050_set_accel_fullscale(
    mpu: *mut hw_mpu6050,
    scale: hw_mpu6050_accel_fullscale,
) -> bool {
    rpi_check!({ hw_mpu6050::send_request_ptr(mpu, Request::SetAccelScale(scale)) });
}

#[no_mangle]
/// set device gyro sensitivity
/// @param mpu - handle (nullable)
pub extern "C" fn ceammc_hw_mpu6050_set_gyro_fullscale(mpu: *mut hw_mpu6050, scale: hw_mpu6050_gyro_fullscale) -> bool {
    rpi_check!({ hw_mpu6050::send_request_ptr(mpu, Request::SetGyroScale(scale)) });
}
