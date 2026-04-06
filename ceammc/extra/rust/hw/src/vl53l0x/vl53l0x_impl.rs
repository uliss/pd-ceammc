use std::{
    ffi::CString,
    sync::{atomic::AtomicBool, Arc},
    time::Duration,
};

use ceammc_rs_msg::{msg_cb, msg_notify};
use log::{debug, error};
use vl53l0x::VL53L0x;

use super::{hw_sensor_vl53l0x, hw_sensor_vl53l0x_data_cb, LaserSensorWorker, Request};
use crate::{i2c::I2cAddress, send_debug, send_error, send_reply, vl53l0x::Reply};

impl hw_sensor_vl53l0x {
    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: msg_notify,
        on_data: hw_sensor_vl53l0x_data_cb,
        on_msg: msg_cb,
    ) -> Result<Self, CString> {
        let (mut worker, rx, tx) = LaserSensorWorker::new(on_msg, None);

        worker.spawn(tx.clone(), notify, move || -> Result<(), String> {
            let i2c = crate::i2c::i2c_impl::create_i2c_bus(i2c_bus)?;
            debug!("i2c init: {i2c:?}");

            let bus = i2c.bus();
            let sensor = Arc::new(std::sync::Mutex::new(
                match i2c_addr {
                    I2cAddress::Addr(addr) => VL53L0x::with_address(i2c, addr),
                    I2cAddress::Default => VL53L0x::new(i2c),
                    I2cAddress::Auto => VL53L0x::new(i2c),
                    I2cAddress::Alt => return Err(format!("no alternative device address")),
                    I2cAddress::Invalid(addr) => return Err(format!("invalid i2c address: {addr}")),
                }
                .map_err(|err| format!("{err:?}"))?,
            ));

            send_debug(
                &tx,
                notify,
                format!("vk53l0x init with bus={bus} and addr={i2c_addr:?}").as_str(),
            )
            .to_err()?;

            let poll_mode = Arc::new(AtomicBool::new(false));

            while let Ok(crate::WorkerCommand::Command(req)) = rx.recv() {
                match req {
                    Request::ReadMM => match sensor.lock().unwrap().read_range_single_millimeters_blocking() {
                        Ok(res) => {
                            debug!("distance: {res}mm");
                            send_reply(Reply::Distance(res), &tx, notify).to_err()?;
                        }
                        Err(err) => {
                            send_error(&tx, notify, &format!("{err:?}")).to_err()?;
                        }
                    },
                    Request::Poll(state) => {
                        if state {
                            if poll_mode.load(std::sync::atomic::Ordering::SeqCst) {
                                send_error(&tx, notify, &format!("already polling")).to_err()?
                            } else {
                                poll_mode.store(true, std::sync::atomic::Ordering::SeqCst);

                                let sensor = sensor.clone();
                                let tx = tx.clone();
                                let poll_mode = poll_mode.clone();

                                sensor
                                    .lock()
                                    .unwrap()
                                    .start_continuous(0)
                                    .map_err(|err| err.to_string())?;

                                std::thread::spawn(move || -> Result<(), String> {
                                    debug!("start poll loop");

                                    loop {
                                        match sensor.lock().unwrap().read_range_mm() {
                                            Ok(res) => {
                                                if let Err(_) = send_reply(Reply::Distance(res), &tx, notify).to_err() {
                                                    break;
                                                }
                                            }
                                            Err(err) => match err {
                                                pwm_pca9685::nb::Error::WouldBlock => {
                                                    continue;
                                                }
                                                _ => {
                                                    send_error(&tx, notify, &format!("{err:?}")).to_err()?;
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
                                    Ok(())
                                });
                            }
                        } else {
                            sensor
                                .lock()
                                .unwrap()
                                .stop_continuous()
                                .map_err(|err| err.to_string())?;

                            if !poll_mode.load(std::sync::atomic::Ordering::SeqCst) {
                                send_error(&tx, notify, &format!("not polling")).to_err()?
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
                            .map_err(|err| format!("{err:?}"))
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
                super::Reply::Message(level, msg) => {
                    vc.worker.pd_message(level, &msg);
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
