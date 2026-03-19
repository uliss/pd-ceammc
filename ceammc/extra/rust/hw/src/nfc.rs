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
mod pn532_impl;
#[cfg(target_os = "linux")] 
mod pn532_timer;

pub enum Request {
    ReadAll,
}

#[derive(Debug, PdMessage)]
pub enum Reply {
    Message(hw_msg_level, CString),
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
pub extern "C" fn ceammc_hw_pn532_new(
    i2c_bus: i8,
    i2c_addr: i8,
    notify: hw_notify_cb,
    on_msg: hw_msg_cb,
    on_key: hw_nfc_pn532_cb,
) -> *mut hw_nfc_pn532 {
    rpi_check!(null_mut(), {
        match hw_nfc_pn532::new(i2c_bus, I2cAddress::new(i2c_addr), notify, on_msg, on_key) {
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
pub extern "C" fn ceammc_hw_nfc_pn532_free(nfc: *mut hw_nfc_pn532) {
    rpi_check!((), {
        if !nfc.is_null() {
            drop(unsafe { Box::from_raw(nfc) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_nfc_pn532_proc_reply(nfc: *const hw_nfc_pn532) -> bool {
    rpi_check!({ hw_nfc_pn532::process_reply(nfc) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_nfc_pn532_readall(nfc: *const hw_nfc_pn532) -> bool {
    rpi_check!({ hw_nfc_pn532::send_request(nfc, Request::ReadAll) });
}
