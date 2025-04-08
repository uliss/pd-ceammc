use std::ffi::CString;

use log::{debug, error};
use rppal::i2c::I2c;

use crate::{hw_msg_cb, hw_notify_cb, process_err, MakePdMessage};

use super::hw_i2c;

#[allow(non_snake_case)]
pub fn create_i2c_bus<Reply>(
    bus: i8,
    tx: &std::sync::mpsc::Sender<Reply>,
    notify: hw_notify_cb,
) -> Result<I2c, String>
where
    Reply: MakePdMessage<Reply>,
{
    match bus {
        crate::i2c::HW_I2C_DEFAULT_BUS => {
            Ok(I2c::new().map_err(|err| process_err(err, tx, notify))?)
        }
        bus if bus >= 0 && bus < crate::i2c::HW_I2C_MAX_BUS => {
            Ok(I2c::with_bus(bus as u8).map_err(|err| process_err(err, tx, notify))?)
        }
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
