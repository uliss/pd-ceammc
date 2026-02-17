pub mod bitmap;
pub mod mdns_sd;
pub mod net_iface;
pub mod lib_regex;
pub mod stdpaths;

use env_logger;
use log::error;

use std::{
    ffi::{c_char, c_void, CStr, CString},
    slice::from_raw_parts,
    sync::Once,
};
static LOG_INIT: Once = Once::new();

/// init rust env_logger
/// logger config is done with a RUST_LOG env variable
#[no_mangle]
pub extern "C" fn ceammc_rust_log_init() {
    LOG_INIT.call_once(|| {
        env_logger::builder().format_timestamp(None).init();
    });
}

#[allow(non_camel_case_types)]
#[derive(Debug)]
#[repr(C)]
pub enum core_log_level {
    Debug,
    Info,
    Error,
}

#[allow(non_camel_case_types)]
#[derive(Clone)]
#[repr(C)]
pub struct core_on_msg {
    // nullable
    user: *mut c_void,
    // not NULL
    cb: extern "C" fn(user: *mut c_void, level: core_log_level, msg: *const c_char),
}

impl core_on_msg {
    fn exec_raw(&self, level: core_log_level, msg: &CString) {
        (self.cb)(self.user, level, msg.as_ptr());
    }

    fn error_cstr(&self, msg: &CString) {
        self.exec_raw(core_log_level::Error, msg);
    }

    #[allow(dead_code)]
    fn debug_cstr(&self, msg: &CString) {
        self.exec_raw(core_log_level::Debug, msg);
    }

    #[allow(dead_code)]
    fn info_cstr(&self, msg: &CString) {
        self.exec_raw(core_log_level::Info, msg);
    }

    fn error_str(&self, msg: &str) {
        self.exec_raw(core_log_level::Error, &CString::new(msg).unwrap_or_default());
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Copy)]
pub struct core_notify {
    /// dispatcher ID
    id: usize,
    /// dispatcher callback (not NULL!)
    f: extern "C" fn(id: usize),
}

impl core_notify {
    fn notify(&self) {
        (self.f)(self.id);
    }
}

fn cstr_to_string(cstr: *const c_char) -> String {
    unsafe { CStr::from_ptr(cstr) }
        .to_str()
        .unwrap_or_else(|err| {
            error!("{err}");
            ""
        })
        .to_owned()
}

fn data_to_vec<T>(data: *const T, len: usize) -> Vec<T>
where
    T: Clone,
{
    if data.is_null() {
        error!("NULL data pointer");
        return vec![];
    } else {
        unsafe { from_raw_parts(data, len) }.to_vec()
    }
}
