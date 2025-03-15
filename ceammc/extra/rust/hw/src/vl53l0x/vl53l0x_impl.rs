use std::{
    ffi::CString,
    sync::{atomic::AtomicBool, Arc},
    time::Duration,
};

use log::{debug, error};
use vl53l0x::VL53L0x;

use crate::{hw_msg_cb, hw_notify_cb, i2c::I2cAddress, process_err, send_reply, vl53l0x::Reply};

use super::{hw_sensor_vl53l0x, hw_sensor_vl53l0x_data_cb, LaserSensorWorker, Request};

impl hw_sensor_vl53l0x {
    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: hw_notify_cb,
        on_data: hw_sensor_vl53l0x_data_cb,
        on_err: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (worker, rx, rep_tx) = LaserSensorWorker::new(on_err);

        worker.spawn(rep_tx.clone(), notify, move || -> Result<(), String> {
            let i2c = crate::i2c::i2c_impl::create_i2c_bus(i2c_bus, &rep_tx, notify)?;
            debug!("i2c init: {i2c:?}");

            let sensor = Arc::new(std::sync::Mutex::new(
                match i2c_addr {
                    I2cAddress::Invalid(addr) => {
                        return Err(format!("invalid i2c address: {addr}"));
                    }
                    I2cAddress::Addr(addr) => VL53L0x::with_address(i2c, addr),
                    _ => VL53L0x::new(i2c),
                }
                .map_err(|err| process_err(format!("{err:?}"), &rep_tx, notify))?,
            ));
            debug!("vk53l0x init");

            let poll_mode = Arc::new(AtomicBool::new(false));

            while let Ok(req) = rx.recv() {
                match req {
                    Request::ReadMM => {
                        match sensor
                            .lock()
                            .unwrap()
                            .read_range_single_millimeters_blocking()
                        {
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

                                let sensor = sensor.clone();
                                let tx = rep_tx.clone();
                                let poll_mode = poll_mode.clone();

                                sensor
                                    .lock()
                                    .unwrap()
                                    .start_continuous(0)
                                    .map_err(|err| process_err(err, &rep_tx, notify))
                                    .unwrap_or_default();

                                std::thread::spawn(move || {
                                    debug!("start poll loop");

                                    loop {
                                        match sensor.lock().unwrap().read_range_mm() {
                                            Ok(res) => {
                                                if !send_reply(Reply::Distance(res), &tx, notify) {
                                                    break;
                                                }
                                            }
                                            Err(err) => match err {
                                                pwm_pca9685::nb::Error::WouldBlock => {
                                                    continue;
                                                }
                                                _ => {
                                                    process_err(format!("{err:?}"), &tx, notify);
                                                    break;
                                                }
                                            },
                                        }

                                        if !&poll_mode.load(std::sync::atomic::Ordering::SeqCst) {
                                            break;
                                        }

                                        std::thread::sleep(Duration::from_millis(25));
                                    }

                                    debug!("exit poll loop");
                                });
                            }
                        } else {
                            sensor
                                .lock()
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
                        sensor
                            .lock()
                            .unwrap()
                            .set_address(addr)
                            .map_err(|err| process_err(format!("{err:?}"), &rep_tx, notify))
                            .unwrap_or_default();
                    }
                }
            }
            // to stop poll sensor thread
            poll_mode.store(false, std::sync::atomic::Ordering::SeqCst);

            Ok(())
        });

        Ok(Self { worker, on_data })
    }

    pub fn process_reply(vc: *const Self) -> bool {
        if vc.is_null() {
            error!("NULL vl53l0x pointer");
            false
        } else {
            let vc = unsafe { &*vc };

            vc.worker.process_reply(&|rep| match rep {
                super::Reply::Error(level, msg) => {
                    vc.worker.caller_error(&msg);
                }
                super::Reply::Distance(mm) => {
                    (vc.on_data.cb)(vc.on_data.user, mm);
                }
            });

            true
        }
    }

    pub fn send_request(vc: *const Self, req: Request) -> bool {
        if vc.is_null() {
            error!("NULL vc53l0x pointer");
            false
        } else {
            let vc = unsafe { &*vc };

            vc.worker.send_request(req)
        }
    }
}
