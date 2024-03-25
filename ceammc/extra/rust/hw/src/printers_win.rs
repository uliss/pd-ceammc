use crate::hw_error_cb;
use crate::printers::hw_print_options;
use crate::printers::PrinterInfo;
use crate::printers::PrinterList;
use printers;
use std::ffi::CString;
use std::ffi::OsStr;
use std::os::raw::c_char;
use winprint::printer::PrinterDevice;
use winprint::ticket::PrintCapabilities;

fn to_cstr(str: &String) -> CString {
    CString::new(str.as_bytes()).unwrap_or_default()
}

pub fn get_printers() -> PrinterList {
    let mut res = PrinterList::new();

    let printers = printers::get_printers();
    for p in printers.iter() {

        let info = PrinterInfo {
            name: to_cstr(&p.name),
            system_name: to_cstr(&p.system_name),
            driver_name: to_cstr(&p.driver_name),
            uri: to_cstr(&p.uri),
            location: to_cstr(&p.location),
            is_default: p.is_default,
            is_shared: p.is_shared,
            state: crate::printers::hw_printer_state::UNKNOWN,
        };
        println!("{}", p.name);
        res.push(info);
        // p.name();
    }

    return res;
}

pub fn print_file(
    printer: *const c_char,
    path: &str,
    opts: &hw_print_options,
    on_err: hw_error_cb,
) -> i32 {
    0
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn get_printers() {
        let p = super::get_printers();
        println!("{p:?}");
    }
}
