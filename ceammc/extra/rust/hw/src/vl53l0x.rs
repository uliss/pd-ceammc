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

use crate::{hw_msg_cb, hw_notify_cb, i2c::I2cAddress, HwThreadWorker, MakePdMessage, hw_msg_level};

#[cfg(target_os = "linux")]
mod vl53l0x_impl;

#[derive(Debug)]
pub enum Request {
    ReadMM,
    Poll(bool),
    SetAddress(u8),
}

#[derive(Debug, PdError)]
pub enum Reply {
    Message(hw_msg_level, CString),
    Distance(u16),
}

#[repr(C)]
pub struct hw_sensor_vl53l0x_data_cb {
    // nullable
    user: *mut c_void,
    // no null
    cb: extern "C" fn(*mut c_void, data: u16),
}

type LaserSensorWorker = HwThreadWorker<Request, Reply>;

pub struct hw_sensor_vl53l0x {
    worker: LaserSensorWorker,
    on_data: hw_sensor_vl53l0x_data_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_sensor_vl53l0x_new(
    i2c_bus: i8,
    i2c_addr: i8,
    notify: hw_notify_cb,
    on_data: hw_sensor_vl53l0x_data_cb,
    on_err: hw_msg_cb,
) -> *mut hw_sensor_vl53l0x {
    rpi_check!(null_mut(), {
        match hw_sensor_vl53l0x::new(i2c_bus, I2cAddress::new(i2c_addr), notify, on_data, on_err) {
            Ok(pwm) => return Box::into_raw(Box::new(pwm)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_sensor_vl53l0x_free(vl: *mut hw_sensor_vl53l0x) {
    rpi_check!((), {
        if !vl.is_null() {
            drop(unsafe { Box::from_raw(vl) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_sensor_vl53l0x_proc_reply(vl: *const hw_sensor_vl53l0x) -> bool {
    rpi_check!({ hw_sensor_vl53l0x::process_reply(vl) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_sensor_vl53l0x_read_mm(display: *const hw_sensor_vl53l0x) -> bool {
    rpi_check!({ hw_sensor_vl53l0x::send_request(display, Request::ReadMM) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_sensor_vl53l0x_poll(
    display: *const hw_sensor_vl53l0x,
    state: bool,
) -> bool {
    rpi_check!({ hw_sensor_vl53l0x::send_request(display, Request::Poll(state)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_sensor_vl53l0x_set_address(
    display: *const hw_sensor_vl53l0x,
    addr: u8,
) -> bool {
    rpi_check!({ hw_sensor_vl53l0x::send_request(display, Request::SetAddress(addr)) });
}
