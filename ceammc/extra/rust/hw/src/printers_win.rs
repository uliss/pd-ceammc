use crate::printers::PrinterList;
use winprint::printer::PrinterDevice;

pub fn get_printers() -> PrinterList {
    let printers = PrinterDevice::all().expect("Failed to get printers");
    todo!()
}
