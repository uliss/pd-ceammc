pub mod obs;
pub mod obs_ffi;
mod common_ffi;

use env_logger;

use std::{ffi::{c_char, CStr}, sync::Once};
static LOG_INIT: Once = Once::new();

/// init rust env_logger
/// logger config is done with a RUST_LOG env variable
#[no_mangle]
pub extern "C" fn ceammc_proto_log_init() {
    LOG_INIT.call_once(|| {
        env_logger::builder().format_timestamp(None).init();
    });
}

#[macro_export]
macro_rules! fn_error {
    ($s:literal, $($arg:tt)*)
    =>
    (error!(concat!(function_name!(), "(): ", $s), $($arg)*));
}

#[macro_export]
macro_rules! fn_debug {
    ($s:literal, $($arg:tt)*)
    =>
    (error!(concat!(function_name!(), "(): ", $s), $($arg)*));
}

fn str_from_cstr(str: *const c_char) -> Result<String, String> {
    if str.is_null() {
        Err("null string pointer".to_owned())
    } else {
        Ok(unsafe { CStr::from_ptr(str).to_str() }
            .map_err(|err| err.to_string())?
            .to_owned())
    }
}
