use std::{
    ffi::CString,
    time::{Duration, Instant},
};

use log::{debug, error, info};
use mpu6050_dmp::{
    address::Address, quaternion::Quaternion, sensor::Mpu6050, temperature::Temperature,
    yaw_pitch_roll::YawPitchRoll,
};

use crate::{
    hw_msg_cb, hw_notify_cb,
    i2c::{i2c_impl::create_i2c_bus, I2cAddress},
    process_err,
    rpi_gyro::Mpu6050Worker,
    send_reply,
};

use super::{hw_mpu6050, hw_mpu6050_data_cb};

impl hw_mpu6050 {
    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
        on_data: hw_mpu6050_data_cb,
    ) -> Result<Self, CString> {
        let (worker, rx, tx) = Mpu6050Worker::new(on_err);

        worker.spawn(tx.clone(), notify, move || {
            let i2c = create_i2c_bus(i2c_bus, &tx, notify)?;
            debug!("I2c init: {i2c:?}");

            let mut mpu = match i2c_addr {
                I2cAddress::Default => Mpu6050::new(i2c, Address::default()),
                I2cAddress::Alt => Mpu6050::new(i2c, Address(0x69)),
                I2cAddress::Invalid(addr) => {
                    return Err(format!("invalid i2c address: {addr}"));
                }
                I2cAddress::Addr(addr) => Mpu6050::new(i2c, Address(addr)),
            }
            .map_err(|err| process_err(format!("MPU6050 init: {err:?}"), &tx, notify))?;

            let mut delay = rppal::hal::Delay::default();
            mpu.initialize_dmp(&mut delay)
                .map_err(|err| process_err(format!("MPU6050 DMP init: {err:?}"), &tx, notify))?;

            info!("Calibrating Sensor ...");
            mpu6050_dmp::calibration_blocking::collect_mean_values(
                &mut mpu,
                &mut delay,
                mpu6050_dmp::accel::AccelFullScale::G2,
                mpu6050_dmp::calibration::ReferenceGravity::ZN,
            )
            .map_err(|err| format!("{err:?}"))?;

            info!("Sensor Calibrated");

            // Configure FIFO
            mpu.enable_fifo().map_err(|err| format!("{err:?}"))?;
            info!("FIFO enabled");

            // Main loop demonstrating FIFO usage
            let mut buffer = [0u8; 256]; // Buffer for FI

            let poll_time = Duration::from_millis(20);
            let poll_mode = true;

            'outer: loop {
                'request_loop: loop {
                    match rx.try_recv() {
                        Ok(req) => {
                            debug!("{req:?}");
                        }
                        Err(err) => match err {
                            std::sync::mpsc::TryRecvError::Empty => break 'request_loop, // just no request
                            std::sync::mpsc::TryRecvError::Disconnected => {
                                break 'outer;
                            }
                        },
                    }
                }

                if poll_mode {
                    let now = Instant::now();

                    match mpu.get_fifo_count() {
                        Ok(fifo_count) => {
                            if fifo_count >= 256 {
                                // FIFO is full - reset to prevent overflow
                                info!("FIFO full - resetting");
                                mpu.reset_fifo()
                                    .map_err(|err| format!("{err:?}"))
                                    .unwrap_or_default();
                            } else {
                                if fifo_count >= 28 {
                                    let buf = mpu.read_fifo(&mut buffer).unwrap();
                                    let q = Quaternion::from_bytes(&buf[..16]).unwrap().normalize();
                                    let ypr = YawPitchRoll::from(q);
                                    debug!("{:?}", ypr);

                                    send_reply(
                                        super::Reply::YawPitchRoll(ypr.yaw, ypr.pitch, ypr.roll),
                                        &tx,
                                        notify,
                                    );
                                }
                            }

                            let temp = mpu
                                .temperature()
                                .map_err(|err| format!("{err:?}"))
                                .unwrap_or(Temperature::new(0))
                                .celsius();

                            send_reply(super::Reply::Temperature(temp), &tx, notify);
                        }
                        Err(err) => {
                            process_err(format!("{err:?}"), &tx, notify);
                        }
                    }

                    let elapsed = now - Instant::now();

                    if elapsed < poll_time {
                        std::thread::sleep(poll_time - elapsed);
                    }
                } else {
                    std::thread::sleep(poll_time);
                }
            }

            Ok(())
        });

        Ok(Self { worker, on_data })
    }

    pub fn process_reply_ptr(mpu: *mut Self) -> bool {
        if mpu.is_null() {
            error!("NULL mpu pointer");
            false
        } else {
            let mpu = unsafe { &*mpu };
            mpu.worker.process_reply(&|rep| match rep {
                super::Reply::Error(msg) => mpu.worker.on_err.exec_raw(msg.as_ptr()),
                super::Reply::YawPitchRoll(yaw, pitch, roll) => {
                    (mpu.on_data.cb_ypr)(mpu.on_data.user, yaw, pitch, roll)
                }
                super::Reply::Temperature(t) => (mpu.on_data.cb_temp)(mpu.on_data.user, t),
            })
        }
    }
}
