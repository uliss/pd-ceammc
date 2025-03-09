use std::ffi::CString;

use log::{debug, error};
use rppal::i2c::I2c;
use vl53l0x::VL53L0x;

use crate::{hw_msg_cb, hw_notify_cb, process_err};

use super::{hw_sensor_vl53l0x, Request, hw_sensor_vl53l0x_data_cb};

impl hw_sensor_vl53l0x {
    pub fn new(
        i2c_bus: i8,
        i2d_addr: u8,
        notify: hw_notify_cb,
        on_data: hw_sensor_vl53l0x_data_cb,
        on_err: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("worker start");

            let i2c = I2c::new().map_err(|err| process_err(err.to_string(), &rep_tx, notify))?;
            debug!("i2c init: {i2c:?}");

            let mut lv = VL53L0x::new(i2c)
                .map_err(|err| process_err(format!("{err:?}"), &rep_tx, notify))?;

            debug!("vk53l0x init");

            while let Ok(req) = req_rx.recv() {
                debug!("{req:?}");

                match req {
                    crate::vl53l0x::Request::ReadMM => {
                        match lv.read_range_single_millimeters_blocking() {
                            Ok(res) => {
                                debug!("distance: {res}mm");
                            }
                            Err(err) => {
                                process_err(format!("{err:?}"), &rep_tx, notify);
                            }
                        }
                    }
                }
            }

            debug!("worker done");

            Ok(())
        });

        Ok(Self {
            tx: req_tx,
            rx: rep_rx,
            on_data,
            on_err,
        })
    }

    pub fn process_reply(vc: *const Self) -> bool {
        if vc.is_null() {
            error!("NULL vc53l0x pointer");
            false
        } else {
            let vc = unsafe { &*vc };

            while let Ok(rep) = vc.rx.try_recv() {
                match rep {
                    super::Reply::Error(msg) => {
                        vc.on_err.exec_raw(msg.as_ptr());
                    }
                    super::Reply::Distance(mm) => {
                       (vc.on_data.cb)(vc.on_data.user, mm);
                    },
                }
            }

            true
        }
    }

    pub fn send_request(vc: *const Self, req: Request) -> bool {
        if vc.is_null() {
            error!("NULL vc53l0x pointer");
            false
        } else {
            let vc = unsafe { &*vc };

            if let Err(err) = vc.tx.send(req) {
                vc.on_err.exec(err.to_string().as_str());
                false
            } else {
                true
            }
        }
    }
}
