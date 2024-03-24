use crate::printers::hw_printer_state;
use crate::printers::PrinterInfo;
use crate::printers::PrinterList;
use cups_sys::*;
use std::ffi::c_char;
use std::ffi::CStr;
use std::ffi::CString;
use std::mem;
use std::ptr;

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
fn read_options(dest: &cups_dest_s, info: &mut PrinterInfo) {
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
                println!("unknown option: {name}");
            }
        }
    }
}

pub fn get_printers() -> PrinterList {
    let mut printers = vec![];

    let mut cups_dests: *mut cups_dest_t = unsafe { mem::zeroed() };
    let num_dests = unsafe { cupsGetDests(&mut cups_dests as *mut _) };
    let dests = unsafe { std::slice::from_raw_parts(cups_dests, num_dests as usize) };

        for dest in dests {
            // skip null names
            if dest.name.is_null() {
                continue;
            }

            let mut info = PrinterInfo::default();
            read_options(dest, &mut info);
            if info.is_shared {
                continue;
            }

            info.system_name = to_cstr(dest.name);
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
            return None
        } else {
            let mut info = PrinterInfo::default();
            read_options(dest, &mut info);
            if info.is_shared {
                return None;
            }

            info.system_name = to_cstr(dest.name);
            return Some(info)
        }
    };

    unsafe { cupsFreeDests(num_dests, dests) };

    res
}
