#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{ffi::CString, ptr::null_mut};

use log::error;

use crate::{hw_msg_cb, hw_notify_cb, HwThreadWorker, MakePdError};

#[cfg(target_os = "linux")]
mod infrared_impl;

pub enum Request {
    Poll(bool),
}

pub enum Reply {
    Error(CString),
    Data(i64),
}

type InfraredWorker = HwThreadWorker<Request, Reply>;

impl MakePdError<Reply> for Reply {
    fn pd_err(msg: CString) -> Reply {
        Reply::Error(msg)
    }
}

pub struct hw_infrared {
    worker: InfraredWorker,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_infrared_new(
    pin: u8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_infrared {
    rpi_check!(null_mut(), {
        match hw_infrared::new(pin, notify, on_err) {
            Ok(ir) => return Box::into_raw(Box::new(ir)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
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
