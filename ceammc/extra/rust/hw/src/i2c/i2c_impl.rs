use std::ffi::CString;

use log::{debug, error};
use rppal::i2c::I2c;

use crate::{hw_msg_cb, hw_notify_cb};

use super::hw_i2c;

impl hw_i2c {
    pub fn new(addr: u8, notify: hw_notify_cb, on_err: hw_msg_cb) -> Result<Self, CString> {
        let (tx, rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("worker thread start");

            let mut i2c = I2c::new().map_err(|err| {
                error!("{err}");
                err.to_string()
            })?;

            i2c.set_slave_address(addr as u16)
                .map_err(|e| e.to_string())?;

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

        Ok(hw_i2c { tx, on_err })
    }

    fn scan_devices(i2c: &mut I2c) -> Result<(), String> {
        for i in 0..127 {
            i2c.set_slave_address(i).map_err(|e| e.to_string())?;
            // i2c.read(buffer)
        }

        todo!()
    }
}
