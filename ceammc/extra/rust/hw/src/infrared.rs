#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{
    ffi::{c_char, c_void, CString},
    ptr::null_mut,
};

use lib_macro::PdError;
use log::error;

use crate::{hw_msg_cb, hw_notify_cb, ptr_to_cstr, HwThreadWorker, MakePdMessage, hw_msg_level};

#[cfg(target_os = "linux")]
mod infrared_impl;
#[cfg(target_os = "linux")]
mod irp;

#[derive(Debug)]
pub enum Request {
    SetProtocol(CString),
}

#[derive(PdError, Debug)]
pub enum Reply {
    Message(hw_msg_level, CString),
    Key(CString, i64),
}

type InfraredWorker = HwThreadWorker<Request, Reply>;

#[repr(C)]
pub struct hw_infrared_key_cb {
    user: *mut c_void, // can be null
    cb: extern "C" fn(user: *mut c_void, key: *const c_char, value: i64),
}

pub struct hw_infrared {
    worker: InfraredWorker,
    on_key: hw_infrared_key_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_infrared_new(
    pin: u8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
    on_key: hw_infrared_key_cb,
) -> *mut hw_infrared {
    rpi_check!(null_mut(), {
        match hw_infrared::new(pin, notify, on_err, on_key) {
            Ok(ir) => return Box::into_raw(Box::new(ir)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.error_cstr(err);
                return null_mut();
            }
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_infrared_free(ir: *mut hw_infrared) {
    rpi_check!((), {
        if !ir.is_null() {
            drop(unsafe { Box::from_raw(ir) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_infrared_free_process_reply(ir: *const hw_infrared) -> bool {
    rpi_check!({ hw_infrared::process_reply_ptr(ir) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_infrared_set_protocol(
    ir: *const hw_infrared,
    proto: *const c_char,
) -> bool {
    rpi_check!({ hw_infrared::send_request_ptr(ir, Request::SetProtocol(ptr_to_cstr(proto))) });
}
