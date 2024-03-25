use cups_sys::*;
use std::{
    ffi::{c_char, CStr, CString},
    os::raw::c_void,
};

use crate::hw_error_cb;
use crate::printers_cups;

#[allow(non_camel_case_types)]
#[derive(Clone, Copy, Default, Debug)]
#[repr(C)]
pub enum hw_printer_state {
    READY,
    PAUSED,
    PRINTING,
    #[default]
    UNKNOWN,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct hw_printer_info {
    name: *const c_char,
    system_name: *const c_char,
    driver_name: *const c_char,
    uri: *const c_char,
    location: *const c_char,
    is_default: bool,
    is_shared: bool,
    state: hw_printer_state,
}

impl hw_printer_info {
    fn new(p: &PrinterInfo) -> Self {
        hw_printer_info {
            name: p.name.as_ptr(),
            system_name: p.system_name.as_ptr(),
            driver_name: p.driver_name.as_ptr(),
            uri: p.uri.as_ptr(),
            location: p.location.as_ptr(),
            is_default: p.is_default,
            is_shared: p.is_shared,
            state: p.state,
        }
    }
}

#[derive(Default, Debug)]
pub struct PrinterInfo {
    pub name: CString,
    pub system_name: CString,
    pub driver_name: CString,
    pub uri: CString,
    pub location: CString,
    pub is_default: bool,
    pub is_shared: bool,
    pub state: hw_printer_state,
}

pub type PrinterList = Vec<PrinterInfo>;

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct hw_printer_info_cb {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, info: &hw_printer_info)>,
}

impl hw_printer_info_cb {
    fn exec(&self, info: &hw_printer_info) {
        self.cb.map(|f| f(self.user, info));
    }
}

#[no_mangle]
pub extern "C" fn ceammc_hw_get_printers(info_cb: hw_printer_info_cb) -> usize {
    #[cfg(target_os = "macos")]
    let printers = printers_cups::get_printers();

    for p in printers.iter().map(|p| hw_printer_info::new(&p)) {
        info_cb.exec(&p);
    }

    printers.len()
}

#[no_mangle]
pub extern "C" fn ceammc_hw_printer_default(info_cb: hw_printer_info_cb) -> bool {
    #[cfg(target_os = "macos")]
    let def_print = printers_cups::get_default_printer();

    match def_print {
        Some(info) => {
            info_cb.exec(&hw_printer_info::new(&info));

            true
        }
        None => false,
    }
}

pub const JOB_ERROR: i32 = -1;

#[derive(Debug, Default, Clone)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub struct hw_print_options {
    pub landscape: bool,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_print_file(
    printer: *const c_char,
    path: *const c_char,
    opts: *const hw_print_options,
    on_err: hw_error_cb,
) -> i32 {
    // get printer name
    let printer = if printer.is_null() {
        unsafe { CStr::from_ptr(printer).to_owned() }
    } else {
        let def = unsafe { cupsGetDefault() };
        if def.is_null() {
            on_err.exec(format!("can't get default printer").as_str());
            return JOB_ERROR;
        } else {
            unsafe { CStr::from_ptr(def).to_owned() }
        }
    };

    let path = unsafe { CStr::from_ptr(path).to_str().unwrap_or_default() };
    let opts = if opts.is_null() {
        hw_print_options::default()
    } else {
        unsafe { &*opts }.clone()
    };

    #[cfg(target_os = "macos")]
    printers_cups::print_file(&printer, path, &opts, on_err)
}
