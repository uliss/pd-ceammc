use crate::printers::hw_printer_state;
use crate::printers::PrinterInfo;
use crate::printers::PrinterList;
use cups_sys::*;
use std::ffi::c_char;
use std::ffi::CStr;
use std::ffi::CString;
use std::mem;

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
    // let mut value = "".to_string();

    for i in 1..dest.num_options {
        let option = unsafe { dest.options.offset(i as isize) };
        let option = unsafe { &*option };

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

    let mut dests: *mut cups_dest_t = unsafe { mem::zeroed() };
    let num_printers = unsafe { cupsGetDests(&mut dests as *mut _) };

    if num_printers > 0 {
        for i in 0..num_printers {
            println!("n:{i}");
            let pi = unsafe { &*dests.offset(i as isize) };

            // skip null names
            if pi.name.is_null() {
                continue;
            }

            let mut info = PrinterInfo::default();
            read_options(pi, &mut info);
            if info.is_shared {
                continue;
            }

            info.system_name = to_cstr(pi.name);
            printers.push(info);
        }
    }

    unsafe { cupsFreeDests(num_printers, dests) };

    printers
}
