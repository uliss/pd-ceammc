use std::{
    ffi::{c_char, CStr, CString},
    os::raw::c_void,
};

use crate::hw_error_cb;

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

/// get printers info via specified callback
/// @param info_cb - called on every found printer
/// @return number of printers found
#[no_mangle]
pub extern "C" fn ceammc_hw_get_printers(info_cb: hw_printer_info_cb) -> usize {
    let mut printers: Vec<PrinterInfo> = vec![];

    #[cfg(feature = "cups")]
    {
        printers = crate::printers_cups::get_printers();
    }

    #[cfg(target_os = "windows")]
    {
        printers = crate::printers_win::get_printers();
    }

    for p in printers.iter().map(|p| hw_printer_info::new(&p)) {
        info_cb.exec(&p);
    }

    printers.len()
}

#[no_mangle]
pub extern "C" fn ceammc_hw_printer_default(info_cb: hw_printer_info_cb) -> bool {
    let mut _def_print: Option<PrinterInfo> = None;

    #[cfg(feature = "cups")]
    {
        _def_print = crate::printers_cups::get_default_printer();
    }

    #[cfg(target_os = "windows")]
    {
        _def_print = crate::printers_win::get_default_printer();
    }

    match _def_print {
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
    on_debug: hw_error_cb,
) -> i32 {
    let path = unsafe { CStr::from_ptr(path).to_str().unwrap_or_default() };
    let opts = if opts.is_null() {
        hw_print_options::default()
    } else {
        unsafe { &*opts }.clone()
    };

    #[cfg(feature = "cups")]
    {
        crate::printers_cups::print_file(printer, path, &opts, on_err, on_debug)
    }

    #[cfg(target_os = "windows")]
    {
        crate::printers_win::print_file(printer, path, &opts, on_err, on_debug)
    }
}
