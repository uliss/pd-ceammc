use std::{
    array::TryFromSliceError,
    ffi::CString,
    time::{Duration, Instant},
};

use ceammc_rs_msg::msg_notify;
use log::{debug, error, info};
use mpu6050_dmp::{
    accel::{Accel, AccelFullScale},
    address::Address,
    gyro::{Gyro, GyroFullScale},
    quaternion::Quaternion,
    sensor::Mpu6050,
    temperature::Temperature,
    yaw_pitch_roll::YawPitchRoll,
};

use crate::{
    hw_msg_cb,
    i2c::{i2c_impl::create_i2c_bus, I2cAddress},
    rpi_gyro::{hw_mpu6050_accel_fullscale, hw_mpu6050_gyro_fullscale, Mpu6050Worker},
    send_error, send_info, send_reply,
};

use super::{hw_mpu6050, hw_mpu6050_data_cb, Request};

impl hw_mpu6050 {
    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: msg_notify,
        on_msg: hw_msg_cb,
        on_data: hw_mpu6050_data_cb,
    ) -> Result<Self, CString> {
        let (mut worker, rx, tx) = Mpu6050Worker::new(on_msg, Some(128));

        worker.spawn(tx.clone(), notify, move || -> Result<(), String> {
            let i2c = create_i2c_bus(i2c_bus)?;
            debug!("I2c init: {i2c:?}");

            let mut mpu = match i2c_addr {
                I2cAddress::Default => Mpu6050::new(i2c, Address::default()),
                I2cAddress::Alt => Mpu6050::new(i2c, Address(0x69)),
                I2cAddress::Auto => {
                    return Err(format!("i2c auto address is not supported"));
                }
                I2cAddress::Invalid(addr) => {
                    return Err(format!("invalid i2c address: {addr}"));
                }
                I2cAddress::Addr(addr) => Mpu6050::new(i2c, Address(addr)),
            }
            .map_err(|err| format!("MPU6050 init: {err:?}"))?;

            let mut delay = rppal::hal::Delay::default();
            mpu.initialize_dmp(&mut delay)
                .map_err(|err| format!("MPU6050 DMP init: {err:?}"))?;

            // Configure FIFO
            mpu.enable_fifo().map_err(|err| format!("{err:?}"))?;
            info!("FIFO enabled");

            // Main loop demonstrating FIFO usage
            let mut buffer = [0u8; 256]; // Buffer for FI

            let poll_time = Duration::from_millis(20);
            let mut poll_mode = false;

            let mut acc_scale = AccelFullScale::G2;
            mpu.set_accel_full_scale(acc_scale)
                .map_err(|err| format!("can't set MPU acceleration full-scale {acc_scale:?}: {err:?}"))?;
            let mut gyro_scale = GyroFullScale::Deg250;
            mpu.set_gyro_full_scale(gyro_scale)
                .map_err(|err| format!("can't set MPU gyro full-scale {gyro_scale:?}: {err:?}"))?;

            'outer: loop {
                'request_loop: loop {
                    match rx.try_recv() {
                        Ok(crate::WorkerCommand::Quit) => break 'outer,
                        Ok(crate::WorkerCommand::Command(req)) => {
                            //
                            debug!("{req:?}");

                            match req {
                                Request::Poll(state) => {
                                    poll_mode = state;
                                }
                                Request::Calibrate => {
                                    send_info(&tx, notify, "Calibrating Sensor ...").to_err()?;

                                    if let Ok(_) = mpu6050_dmp::calibration_blocking::collect_mean_values(
                                        &mut mpu,
                                        &mut delay,
                                        mpu6050_dmp::accel::AccelFullScale::G2,
                                        mpu6050_dmp::calibration::ReferenceGravity::ZN,
                                    )
                                    .map_err(|err| format!("calibration error: {err:?}"))
                                    {
                                        send_info(&tx, notify, "Sensor Calibrated").to_err()?;
                                    }
                                }
                                Request::SetAccelScale(scale) => {
                                    acc_scale = match scale {
                                        hw_mpu6050_accel_fullscale::G2 => AccelFullScale::G2,
                                        hw_mpu6050_accel_fullscale::G4 => AccelFullScale::G4,
                                        hw_mpu6050_accel_fullscale::G8 => AccelFullScale::G8,
                                        hw_mpu6050_accel_fullscale::G16 => AccelFullScale::G16,
                                    };
                                }
                                Request::SetGyroScale(scale) => {
                                    gyro_scale = match scale {
                                        hw_mpu6050_gyro_fullscale::Deg250 => GyroFullScale::Deg250,
                                        hw_mpu6050_gyro_fullscale::Deg500 => GyroFullScale::Deg500,
                                        hw_mpu6050_gyro_fullscale::Deg1000 => GyroFullScale::Deg1000,
                                        hw_mpu6050_gyro_fullscale::Deg2000 => GyroFullScale::Deg2000,
                                    };
                                }
                            }
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
                                mpu.reset_fifo().map_err(|err| format!("{err:?}"))?
                            } else {
                                if fifo_count >= 28 {
                                    let buf = mpu.read_fifo(&mut buffer).map_err(|err| format!("{err:?}"))?;
                                    let q = Quaternion::from_bytes(&buf[..16])
                                        .ok_or(format!("quaternion error"))?
                                        .normalize();
                                    let ypr = YawPitchRoll::from(q);
                                    debug!("{:?}", ypr);

                                    send_reply(super::Reply::YawPitchRoll(ypr.yaw, ypr.pitch, ypr.roll), &tx, notify)
                                        .to_err()?;

                                    let accel = Accel::from_bytes(
                                        buf[16..22]
                                            .try_into()
                                            .map_err(|err: TryFromSliceError| err.to_string())?,
                                    )
                                    .scaled(acc_scale);
                                    send_reply(super::Reply::Accel(accel.x(), accel.y(), accel.z()), &tx, notify)
                                        .to_err()?;

                                    let gyro = Gyro::from_bytes(
                                        buf[22..28]
                                            .try_into()
                                            .map_err(|err: TryFromSliceError| err.to_string())?,
                                    )
                                    .scaled(gyro_scale);
                                    send_reply(super::Reply::Gyro(gyro.x(), gyro.y(), gyro.z()), &tx, notify)
                                        .to_err()?;
                                }
                            }

                            let temp = mpu
                                .temperature()
                                .map_err(|err| format!("{err:?}"))
                                .unwrap_or(Temperature::new(0))
                                .celsius();

                            send_reply(super::Reply::Temperature(temp), &tx, notify).to_err()?;
                        }
                        Err(err) => send_error(&tx, notify, &format!("{err:?}")).to_err()?,
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

    pub fn process_reply_ptr(mpu: *const Self) -> bool {
        if mpu.is_null() {
            error!("NULL mpu pointer");
            false
        } else {
            let mpu = unsafe { &*mpu };
            mpu.worker.process_reply(&|rep| match rep {
                super::Reply::Message(level, msg) => mpu.worker.pd_message(level, &msg),
                super::Reply::YawPitchRoll(yaw, pitch, roll) => mpu.on_data.ypr(yaw, pitch, roll),
                super::Reply::Temperature(t) => mpu.on_data.temp(t),
                super::Reply::Accel(x, y, z) => mpu.on_data.accel(x, y, z),
                super::Reply::Gyro(x, y, z) => mpu.on_data.gyro(x, y, z),
            })
        }
    }

    pub fn send_request_ptr(mpu: *const Self, req: Request) -> bool {
        if mpu.is_null() {
            error!("NULL mpu pointer");
            false
        } else {
            let mpu = unsafe { &*mpu };
            mpu.worker.send_request(req)
        }
    }
}
