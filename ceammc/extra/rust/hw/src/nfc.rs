#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use crate::{HwThreadWorker, MakePdMessage};
use ceammc_rs_msg::{msg_cb, msg_level, msg_notify};
use lib_macro::PdMessage;
use log::error;
use std::{
    ffi::{c_void, CString},
    ptr::null_mut,
};

#[cfg(target_os = "linux")]
mod pn532_impl;
#[cfg(target_os = "linux")]
mod pn532_timer;

pub enum Request {
    ReadAll,
}

#[derive(Debug, PdMessage)]
pub enum Reply {
    Message(msg_level, CString),
    AllKeys(u16),
}

type NfcWorker = HwThreadWorker<Request, Reply>;

pub struct hw_nfc_pn532 {
    worker: NfcWorker,
    cb: hw_nfc_pn532_cb,
}

#[repr(C)]
pub struct hw_nfc_pn532_cb {
    user: *mut c_void,
    on_all_keys: extern "C" fn(user: *mut c_void, state: u16),
}

impl hw_nfc_pn532_cb {
    pub(crate) fn all_keys(&self, st: u16) {
        (self.on_all_keys)(self.user, st)
    }
}

#[no_mangle]
/// create new pn532 device handle
/// @i2c_bus - i2c bus number
/// @notify - notify callback, when device get some information
/// @on_msg - message callback called on device message
/// @on_data - data callback
/// @return pointer to handle or nullptr on error
pub extern "C" fn ceammc_hw_pn532_new(
    i2c_bus: i8,
    notify: msg_notify,
    on_msg: msg_cb,
    on_data: hw_nfc_pn532_cb,
) -> *mut hw_nfc_pn532 {
    rpi_check!(null_mut(), {
        match hw_nfc_pn532::new(i2c_bus, notify, on_msg, on_data) {
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
/// delete pn532 handle
/// @param nfc - device handle, nullable
pub extern "C" fn ceammc_hw_nfc_pn532_free(nfc: *mut hw_nfc_pn532) {
    rpi_check!((), {
        if !nfc.is_null() {
            drop(unsafe { Box::from_raw(nfc) })
        }
    });
}

#[no_mangle]
/// process reply from device
/// @param nfc - device handle, nullable
pub extern "C" fn ceammc_hw_nfc_pn532_proc_reply(nfc: *const hw_nfc_pn532) -> bool {
    rpi_check!({ hw_nfc_pn532::process_reply(nfc) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_nfc_pn532_readall(nfc: *const hw_nfc_pn532) -> bool {
    rpi_check!({ hw_nfc_pn532::send_request(nfc, Request::ReadAll) });
}
