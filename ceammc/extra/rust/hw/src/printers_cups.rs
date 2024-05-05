use crate::printers::hw_printer_state;
use crate::printers::PrinterInfo;
use crate::printers::PrinterList;
use cups_sys::*;
use std::ffi::CStr;
use std::ffi::CString;
use std::mem;
use std::os::raw::c_char;
use std::ptr;
use std::ptr::null_mut;

fn is_true(s: *const c_char) -> bool {
    unsafe { CStr::from_ptr(s) }.to_string_lossy() == "true"
}

fn to_cstr(s: *mut c_char) -> CString {
    if s.is_null() {
        return CString::new("").unwrap_or_default();
    } else {
        unsafe { CStr::from_ptr(s).to_owned() }
    }
}

fn to_str(s: *mut c_char) -> String {
    if s.is_null() {
        return String::new();
    } else {
        unsafe { CStr::from_ptr(s).to_str() }
            .unwrap_or_default()
            .to_string()
    }
}

/**
 * Returns a string value of an key on cups options (If the key was not found return a empty string)
 */
fn cups_read_options(dest: &cups_dest_s, info: &mut PrinterInfo) {
    let opts = unsafe { std::slice::from_raw_parts(dest.options, dest.num_options as usize) };

    for option in opts {
        let name = to_str(option.name);

        match name.as_str() {
            "printer-info" => info.name = to_cstr(option.value),
            "printer-location" => info.location = to_cstr(option.value),
            "device-uri" => info.uri = to_cstr(option.value),
            "printer-make-and-model" => info.driver_name = to_cstr(option.value),
            "printer-is-shared" => info.is_shared = is_true(option.value),
            "printer-state" => {
                let state = to_str(option.value);
                info.state = match state.as_str() {
                    "3" => hw_printer_state::READY,
                    "4" => hw_printer_state::PRINTING,
                    "5" => hw_printer_state::PAUSED,
                    _ => {
                        println!("cups state: {state}");
                        hw_printer_state::UNKNOWN
                    }
                }
            }
            _ => {
                // println!("unknown option: {name}");
            }
        }
    }
}

pub fn get_printers() -> PrinterList {
    let mut printers = vec![];
    let mut cups_dests: *mut cups_dest_t = null_mut();
    let num_dests = unsafe { cupsGetDests(&mut cups_dests as *mut _) };

    let dests = unsafe { std::slice::from_raw_parts_mut(cups_dests, num_dests as usize) };

    for dest in dests {
        // skip null names
        if dest.name.is_null() {
            continue;
        }

        let mut info = PrinterInfo::default();
        cups_read_options(dest, &mut info);
        if info.is_shared {
            continue;
        }
        info.system_name = to_cstr(dest.name);
        info.is_default = dest.is_default != 0;
        printers.push(info);
    }

    unsafe { cupsFreeDests(num_dests, cups_dests) };

    printers
}

pub fn get_default_printer() -> Option<PrinterInfo> {
    let mut dests: *mut cups_dest_t = unsafe { mem::zeroed() };
    let num_dests = unsafe { cupsGetDests(&mut dests as *mut _) };

    // Get the default printer.
    let dest = unsafe { cupsGetDest(ptr::null(), ptr::null(), num_dests, dests) };

    let res = if dest.is_null() {
        None
    } else {
        let dest = unsafe { &*dest };

        // skip null names
        if dest.name.is_null() {
            return None;
        } else {
            let mut info = PrinterInfo::default();
            cups_read_options(dest, &mut info);
            if info.is_shared {
                return None;
            }

            info.system_name = to_cstr(dest.name);
            return Some(info);
        }
    };

    unsafe { cupsFreeDests(num_dests, dests) };

    res
}

use crate::hw_error_cb;
use crate::printers::hw_print_options;
use std::path::Path;

pub fn print_file(
    printer: *const c_char,
    path: &str,
    opts: &hw_print_options,
    on_err: hw_error_cb,
    _on_debug: hw_error_cb,
) -> i32 {
    // get printer name
    let printer = if !printer.is_null() {
        unsafe { CStr::from_ptr(printer).to_owned() }
    } else {
        let def = unsafe { cupsGetDefault() };
        if def.is_null() {
            on_err.exec(format!("can't get default printer").as_str());
            return crate::printers::JOB_ERROR;
        } else {
            unsafe { CStr::from_ptr(def).to_owned() }
        }
    };

    // check path
    let path = Path::new(path);
    if !path.exists() {
        on_err.exec(format!("file not found: {path:?}").as_str());
        return crate::printers::JOB_ERROR;
    }

    let basename = path
        .file_name()
        .unwrap_or_default()
        .to_str()
        .unwrap_or_default();
    let path = CString::new(path.to_str().unwrap_or_default()).unwrap_or_default();

    let job_title = CString::new(format!("PureData print job: '{basename}'")).unwrap_or_default();

    let mut options: *mut cups_option_t = null_mut();
    let mut num_opts: i32 = 0;
    if opts.landscape {
        println!("landscape = true");
        unsafe {
            num_opts = cupsAddOption(
                CUPS_ORIENTATION.as_ptr() as *const c_char,
                CUPS_ORIENTATION_LANDSCAPE.as_ptr() as *const c_char,
                num_opts,
                &mut options as *mut _,
            )
        }
    }

    let job_id: i32 = unsafe {
        cupsPrintFile2(
            null_mut(),
            printer.as_ptr(),
            path.as_ptr(),
            job_title.as_ptr(),
            num_opts,
            options,
        )
    };

    if !options.is_null() {
        unsafe {
            cupsFreeOptions(num_opts, options);
        }
    }

    if job_id == 0 {
        let err = unsafe { cupsLastErrorString() };
        on_err.exec_raw(err);
    }

    job_id
}

#[cfg(test)]
mod tests {
    // use super::*;

    // #[test]
    // fn empty() {

    // }
}
