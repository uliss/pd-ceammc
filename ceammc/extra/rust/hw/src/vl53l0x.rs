#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{ffi::CString, ptr::null_mut};

use log::error;

use crate::{hw_msg_cb, hw_notify_cb, MakePdError};

#[cfg(target_os = "linux")]
mod vl53l0x_impl;

#[derive(Debug)]
pub enum Request {
    ReadMM,
}

#[derive(Debug)]
pub enum Reply {
    Error(CString),
}

impl MakePdError<Reply> for Reply {
    fn pd_err(msg: CString) -> Reply {
        Reply::Error(msg)
    }
}

pub struct hw_sensor_vl53l0x {
    tx: std::sync::mpsc::Sender<Request>,
    rx: std::sync::mpsc::Receiver<Reply>,
    on_err: hw_msg_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_sensor_vl53l0x_new(
    i2c_bus: i8,
    i2d_addr: u8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_sensor_vl53l0x {
    rpi_check!(null_mut(), {
        match hw_sensor_vl53l0x::new(i2c_bus, i2d_addr, notify, on_err) {
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
