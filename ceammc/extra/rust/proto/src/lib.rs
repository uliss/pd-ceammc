mod common_ffi;
pub mod obs;
pub mod obs_ffi;
pub mod vlc;
pub mod vlc_ffi;

use env_logger;
use smol_str::SmolStr;

use std::{
    ffi::{c_char, CStr},
    str::FromStr,
    sync::Once,
};
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

#[allow(non_camel_case_types)]
#[repr(C)]
pub union rust_value {
    pub str_val: *const c_char,
    pub float_val: f64,
}

#[allow(non_camel_case_types)]
#[repr(u8)]
pub enum rust_atom_type {
    Float,
    String,
    Null,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct rust_atom {
    pub data: rust_value,
    pub atom_type: rust_atom_type,
}

#[derive(Debug, PartialEq)]
pub enum RustAtom {
    Str(SmolStr),
    Float(f64),
    Null,
}

impl RustAtom {
    fn from_str(s: &str) -> Self {
        RustAtom::Str(SmolStr::from_str(s).unwrap_or_default())
    }

    fn is_float(self: &Self) -> bool {
        match self {
            &RustAtom::Float(_) => true,
            _ => false,
        }
    }

    fn to_float(self: &Self) -> Option<f64> {
        match &self {
            &RustAtom::Float(f) => Some(*f),
            _ => None,
        }
    }

    fn to_str(self: &Self) -> Option<SmolStr> {
        match &self {
            &RustAtom::Str(s) => Some(s.clone()),
            _ => None,
        }
    }

    fn is_null(self: &Self) -> bool {
        match self {
            &RustAtom::Null => true,
            _ => false,
        }
    }

    fn is_str(self: &Self) -> bool {
        match self {
            &RustAtom::Str(_) => true,
            _ => false,
        }
    }
}

impl rust_atom {
    pub fn as_safe_value(&self) -> RustAtom {
        unsafe {
            match self.atom_type {
                rust_atom_type::Float => RustAtom::Float(self.data.float_val),
                rust_atom_type::String => RustAtom::Str(
                    SmolStr::from_str(&CStr::from_ptr(self.data.str_val).to_string_lossy())
                        .unwrap_or_default(),
                ),
                _ => RustAtom::Null,
            }
        }
    }
}
