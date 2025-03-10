use std::{ffi::CString, sync::Arc, time::Duration};

use log::{debug, error};
use vl53l0x::VL53L0x;

use crate::{hw_msg_cb, hw_notify_cb, i2c::I2cAddress, process_err, send_reply, vl53l0x::Reply};

use super::{hw_sensor_vl53l0x, hw_sensor_vl53l0x_data_cb, Request};

impl hw_sensor_vl53l0x {
    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: hw_notify_cb,
        on_data: hw_sensor_vl53l0x_data_cb,
        on_err: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("worker start");

            let i2c = crate::i2c::i2c_impl::create_i2c_bus(i2c_bus, &rep_tx, notify)?;
            debug!("i2c init: {i2c:?}");

            let lv = match i2c_addr {
                I2cAddress::Invalid(addr) => {
                    return Err(format!("invalid i2c address: {addr}"));
                }
                I2cAddress::Addr(addr) => VL53L0x::with_address(i2c, addr),
                _ => VL53L0x::new(i2c),
            }
            .map_err(|err| process_err(format!("{err:?}"), &rep_tx, notify))?;

            let lv = Arc::new(std::sync::Mutex::new(lv));

            let poll_mode = Arc::new(std::sync::atomic::AtomicBool::new(false));

            debug!("vk53l0x init");

            while let Ok(req) = req_rx.recv() {
                debug!("{req:?}");

                match req {
                    Request::ReadMM => {
                        match lv.lock().unwrap().read_range_single_millimeters_blocking() {
                            Ok(res) => {
                                debug!("distance: {res}mm");
                                send_reply(Reply::Distance(res), &rep_tx, notify);
                            }
                            Err(err) => {
                                process_err(format!("{err:?}"), &rep_tx, notify);
                            }
                        }
                    }
                    Request::Poll(state) => {
                        if state {
                            if poll_mode.load(std::sync::atomic::Ordering::SeqCst) {
                                process_err(format!("already polling"), &rep_tx, notify);
                            } else {
                                poll_mode.store(true, std::sync::atomic::Ordering::SeqCst);

                                let lv2 = lv.clone();
                                let tx2 = rep_tx.clone();

                                std::thread::scope(|s| {
                                    lv2.lock()
                                        .unwrap()
                                        .start_continuous(0)
                                        .map_err(|err| process_err(err, &rep_tx, notify))
                                        .unwrap_or_default();

                                    s.spawn(|| {
                                        debug!("start poll loop");

                                        loop {
                                            match lv2.lock().unwrap().read_range_mm() {
                                                Ok(res) => {
                                                    debug!("distance: {res}mm");
                                                    send_reply(Reply::Distance(res), &tx2, notify);
                                                }
                                                Err(err) => match err {
                                                    pwm_pca9685::nb::Error::WouldBlock => {
                                                        debug!("no data");
                                                    }
                                                    _ => {
                                                        process_err(
                                                            format!("{err:?}"),
                                                            &tx2,
                                                            notify,
                                                        );
                                                        break;
                                                    }
                                                },
                                            }

                                            if !&poll_mode.load(std::sync::atomic::Ordering::SeqCst)
                                            {
                                                break;
                                            }

                                            std::thread::sleep(Duration::from_millis(30));
                                        }

                                        debug!("exit poll loop");
                                    });
                                });
                            }
                        } else {
                            lv.lock()
                                .unwrap()
                                .stop_continuous()
                                .map_err(|err| process_err(err, &rep_tx, notify))
                                .unwrap_or_default();

                            if !poll_mode.load(std::sync::atomic::Ordering::SeqCst) {
                                process_err(format!("not polling"), &rep_tx, notify);
                            } else {
                                poll_mode.store(false, std::sync::atomic::Ordering::SeqCst);
                            }
                        }
                    }
                    Request::SetAddress(addr) => {
                        lv.lock()
                            .unwrap()
                            .set_address(addr)
                            .map_err(|err| process_err(format!("{err:?}"), &rep_tx, notify))
                            .unwrap_or_default();
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
                    }
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
