#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use crate::{hw_msg_cb, hw_msg_level, hw_notify_cb, i2c::I2cAddress, HwThreadWorker, MakePdMessage};
use lib_macro::PdMessage;
use log::error;
use std::{
    ffi::{c_void, CString},
    ptr::null_mut,
};

#[cfg(target_os = "linux")]
mod mpr121_impl;

#[derive(Debug)]
pub enum Request {
    ReadAll,
}

#[derive(Debug, PdMessage)]
pub enum Reply {
    Message(hw_msg_level, CString),
    AllTouches { touched: u16, previous: u16 },
}

type Mpr212SensorWorker = HwThreadWorker<Request, Reply>;

pub struct hw_sensor_mpr121 {
    worker: Mpr212SensorWorker,
    cb: hw_mpr121_touch_cb,
}

#[repr(C)]
pub struct hw_mpr121_touch_cb {
    user: *mut c_void,
    on_touch: extern "C" fn(user: *mut c_void, touched: u16, previous: u16),
}

impl hw_mpr121_touch_cb {
    pub(crate) fn all_touches(&self, touched: u16, previous: u16) {
        (self.on_touch)(self.user, touched, previous)
    }
}

#[no_mangle]
pub extern "C" fn ceammc_hw_sensor_mpr121_new(
    i2c_bus: i8,
    i2c_addr: i8,
    notify: hw_notify_cb,
    on_msg: hw_msg_cb,
    on_touch: hw_mpr121_touch_cb,
) -> *mut hw_sensor_mpr121 {
    rpi_check!(null_mut(), {
        match hw_sensor_mpr121::new(i2c_bus, I2cAddress::new(i2c_addr), notify, on_msg, on_touch) {
            Ok(ir) => return Box::into_raw(Box::new(ir)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_msg.error_cstr(err);
                return null_mut();
            }
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_sensor_mpr121_free(mpr: *mut hw_sensor_mpr121) {
    rpi_check!((), {
        if !mpr.is_null() {
            drop(unsafe { Box::from_raw(mpr) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_sensor_mpr121_proc_reply(mpr: *const hw_sensor_mpr121) -> bool {
    rpi_check!({ hw_sensor_mpr121::process_reply(mpr) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_sensor_mpr121_readall(mpr: *const hw_sensor_mpr121) -> bool {
    rpi_check!({ hw_sensor_mpr121::send_request(mpr, Request::ReadAll) });
}
