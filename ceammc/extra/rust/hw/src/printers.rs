use cups_sys::*;
use std::{
    ffi::{c_char, CStr, CString},
    os::raw::c_void,
};

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

#[no_mangle]
pub extern "C" fn ceammc_hw_get_printers(
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, info: &hw_printer_info)>,
) -> usize {
    cb.map_or(0, |f| {
        #[cfg(target_os = "macos")]
        let printers = printers_cups::get_printers();

        for p in printers.iter().map(|p| hw_printer_info {
            name: p.name.as_ptr(),
            system_name: p.system_name.as_ptr(),
            driver_name: p.driver_name.as_ptr(),
            uri: p.uri.as_ptr(),
            location: p.location.as_ptr(),
            is_default: p.is_default,
            is_shared: p.is_shared,
            state: p.state,
        }) {
            f(user, &p);
        }

        printers.len()
    })
}

#[cfg(target_os = "macos")]
#[no_mangle]
pub extern "C" fn ceammc_hw_print_file(printer_name: *const c_char, path: *const c_char) -> bool {
    let job_title = CString::new("PureData print job").unwrap_or_default();
    // let path = CString::new("/Users/serge/Documents/Pd/ceam/beil.txt").unwrap_or_default();
    let job_id: i32 = unsafe {
        cupsPrintFile(
            cupsGetDefault(),
            // File to print.
            path,
            // Name of the print job.
            job_title.as_ptr(),
            0,
            std::ptr::null_mut(),
        )
    };

    if job_id == 0 {
        println!("error: {}", unsafe { cupsLastError() });
        let err = unsafe { cupsLastErrorString() };
        let err = unsafe { CStr::from_ptr(err) }.to_str().unwrap_or_default();
        println!("strerror: {err}");
    }

    println!("job id: {job_id}");
    true
}
