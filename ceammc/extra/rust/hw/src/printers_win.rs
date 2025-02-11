use crate::hw_msg_cb;
use crate::printers::hw_print_options;
use crate::printers::hw_printer_state;
use crate::printers::PrinterInfo;
use crate::printers::PrinterList;
use crate::printers::JOB_ERROR;
use printers;
use printers::printer::PrinterState;
use std::ffi::{CStr, CString};
use std::os::raw::c_char;
use std::path::Path;
use winprint::printer::FilePrinter;
use winprint::printer::PdfiumPrinter;
use winprint::printer::PrinterDevice;
// use winprint::printer::XpsPrinter;
// use winprint::ticket::PrintCapabilities;

fn to_cstr(str: &String) -> CString {
    CString::new(str.as_bytes()).unwrap_or_default()
}

fn to_pstate(state: &PrinterState) -> hw_printer_state {
    match state {
        PrinterState::READY => hw_printer_state::READY,
        PrinterState::PAUSED => hw_printer_state::PAUSED,
        PrinterState::PRINTING => hw_printer_state::PRINTING,
        PrinterState::UNKNOWN => hw_printer_state::UNKNOWN,
    }
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
            state: to_pstate(&p.state),
        };
        res.push(info);
    }

    return res;
}

pub fn get_default_printer() -> Option<PrinterInfo> {
    for p in printers::get_printers().iter() {
        if p.is_default {
            return Some(PrinterInfo {
                name: to_cstr(&p.name),
                system_name: to_cstr(&p.system_name),
                driver_name: to_cstr(&p.driver_name),
                uri: to_cstr(&p.uri),
                location: to_cstr(&p.location),
                is_default: p.is_default,
                is_shared: p.is_shared,
                state: to_pstate(&p.state),
            });
        }
    }

    None
}

pub fn print_file(
    printer: *const c_char,
    path: &str,
    opts: &hw_print_options,
    on_err: hw_msg_cb,
    on_debug: hw_msg_cb,
) -> i32 {
    let mut printer = unsafe { CStr::from_ptr(printer) }
        .to_str()
        .unwrap_or_default()
        .to_owned();

    if printer.is_empty() || printer.starts_with("@") {
        on_debug.exec(format!("searching default printer ...").as_str());
        for p in printers::get_printers().iter() {
            if p.is_default {
                printer = p.name.clone();
                break;
            }
        }
    }

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

    on_debug.exec(format!("printing file '{basename}' on the '{printer}'").as_str());

    match PrinterDevice::all() {
        Ok(printers) => match printers.into_iter().find(|x| x.name() == printer) {
            Some(p) => {
                on_debug.exec(format!("found default printer: {}", p.name()).as_str());

                // let caps = PrintCapabilities::fetch(&p);
                if opts.landscape {
                    on_err.exec("landscape mode printing not yet supported on Windows");
                }

                let p = PdfiumPrinter::new(p);

                // let p = XpsPrinter::new(p);
                match p.print(path, Default::default()) {
                    Ok(_) => return 1,
                    Err(_err) => {
                        on_err.exec(format!("print error: {_err}").as_str());
                        return JOB_ERROR;
                    }
                }
            }
            None => {
                on_err.exec(format!("printer not found: {printer}").as_str());
                return JOB_ERROR;
            }
        },
        Err(_) => return JOB_ERROR,
    };
}

#[cfg(test)]
mod tests {
    use std::{os::raw::c_void, ptr::null_mut};

    use super::*;

    #[test]
    fn get_printers() {
        let p = super::get_printers();
        assert!(p.len() > 0);
        println!("\n\nPRINTER_LIST: {p:?}");
    }

    #[test]
    fn get_default_printert() {
        let p = super::get_default_printer();
        println!("\n\nDEFAULT PRINTER:\n{p:?}");
    }

    extern "C" fn test_err(user: *mut c_void, msg: *const c_char) {
        let err = unsafe { CStr::from_ptr(msg) }.to_str().unwrap_or_default();
        println!("\n\nEROROROROROR: {err}\n\n");
    }

    const FILE: &str = "c:\\Users\\serge\\Documents\\test1.pdf!!";

    #[test]
    fn print_file() {
        let p = CString::default();
        let opts = hw_print_options::default();
        let cb = hw_msg_cb {
            user: null_mut(),
            cb: Some(test_err),
        };

        let rc = super::print_file(p.as_ptr(), FILE, &opts, cb);
        assert_eq!(rc, JOB_ERROR);
    }
}
