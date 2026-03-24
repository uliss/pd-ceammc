use std::ffi::CString;

use log::{debug, error};
use rppal::i2c::I2c;

use crate::{hw_msg_cb, hw_notify_cb, process_err, MakePdMessage};

use super::hw_i2c;

pub enum DetectMethod {
    QuickWrite,
    ReceiveByte,
    ReadByte,
    WriteRead,
}

pub fn try_i2c_device(i2c: &mut I2c, i2c_addr: u16, method: DetectMethod) -> Result<(), String> {
    i2c.set_slave_address(i2c_addr).map_err(|err| err.to_string())?;

    match match method {
        DetectMethod::QuickWrite => i2c.write(&[]).map(|_| ()),
        DetectMethod::ReceiveByte => i2c.smbus_receive_byte().map(|_| ()),
        DetectMethod::ReadByte => {
            let mut buf = [0u8; 1];
            i2c.read(&mut buf).map(|_| ())
        }
        DetectMethod::WriteRead => {
            let mut buf = [0u8; 1];
            i2c.write_read(&[0x00], &mut buf).map(|_| ())
        }
    } {
        Ok(_) => Ok(()),
        Err(_) => {
            let msg = format!("device 0x{:02x} is not responding: check connection!", i2c_addr);
            log::error!("{msg}");
            Err(msg)
        }
    }
}

fn check_kernel_i2c_modules() -> Result<(), String> {
    const PROC_MODULES: &str = "/proc/modules";
    const I2C_MODULE: &str = "i2c_dev";
    let modules_content =
        std::fs::read_to_string(PROC_MODULES).map_err(|err| format!("can't read {PROC_MODULES}: {err}"))?;

    for line in modules_content.lines() {
        if line.starts_with(I2C_MODULE) {
            return Ok(());
        }
    }

    Err(format!("module {I2C_MODULE} is not loaded"))
}

#[allow(non_snake_case)]
pub fn create_i2c_bus<Reply>(bus: i8, tx: &std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb) -> Result<I2c, String>
where
    Reply: MakePdMessage<Reply>,
{
    match bus {
        crate::i2c::HW_I2C_DEFAULT_BUS => Ok(I2c::new().map_err(|err| process_err(err, tx, notify))?),
        bus if bus >= 0 && bus < crate::i2c::HW_I2C_MAX_BUS => match I2c::with_bus(bus as u8) {
            Ok(i2c) => Ok(i2c),
            Err(err) => {
                check_kernel_i2c_modules()?;
                Err(process_err(err, tx, notify))
            }
        },
        _ => {
            return Err(process_err(format!("invalid I2C bus: {bus}"), tx, notify));
        }
    }
}

impl hw_i2c {
    pub fn new(addr: u8, _notify: hw_notify_cb, on_msg: hw_msg_cb) -> Result<Self, CString> {
        let (tx, rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("worker thread start");

            let mut i2c = I2c::new().map_err(|err| {
                error!("{err}");
                err.to_string()
            })?;

            i2c.set_slave_address(addr as u16).map_err(|e| e.to_string())?;

            debug!("i2c init done");

            while let Ok(req) = rx.recv() {
                match req {
                    crate::i2c::Request::ScanDevices => {
                        Self::scan_devices(&mut i2c)?;
                    }
                }
            }

            debug!("worker thread done");

            Ok(())
        });

        Ok(hw_i2c {
            _tx: tx,
            _on_err: on_msg,
        })
    }

    fn scan_devices(i2c: &mut I2c) -> Result<(), String> {
        for i in 0..127 {
            i2c.set_slave_address(i).map_err(|e| e.to_string())?;
            // i2c.read(buffer)
        }

        todo!()
    }
}
