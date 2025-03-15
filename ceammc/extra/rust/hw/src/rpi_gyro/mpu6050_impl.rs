use std::{
    ffi::CString,
    time::{Duration, Instant},
};

use log::{debug, info};
use mpu6050_dmp::{
    address::Address, calibration::CalibrationParameters, quaternion::Quaternion, sensor::Mpu6050,
    yaw_pitch_roll::YawPitchRoll,
};

use crate::{
    hw_msg_cb, hw_notify_cb,
    i2c::{i2c_impl::create_i2c_bus, I2cAddress},
    process_err,
    rpi_gyro::Mpu6050Worker,
};

use super::hw_mpu6050;

impl hw_mpu6050 {
    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
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

            // // Configure calibration parameters
            // let calibration_params = CalibrationParameters::new(
            //     mpu6050_dmp::accel::AccelFullScale::G2,
            //     mpu6050_dmp::gyro::GyroFullScale::Deg2000,
            //     mpu6050_dmp::calibration::ReferenceGravity::ZN,
            // );

            // info!("Calibrating Sensor");
            // mpu.calibrate(&mut delay, &calibration_params)
            //     .map_err(|err| format!("{err:?}"))?;

            // info!("Sensor Calibrated");

            // Configure FIFO
            mpu.enable_fifo().map_err(|err| format!("{err:?}"))?;
            info!("FIFO enabled");

            // Main loop demonstrating FIFO usage
            let mut buffer = [0u8; 1024]; // Buffer for FI

            let poll_time = Duration::from_millis(10);
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

                    // Get FIFO count
                    let fifo_count = mpu.get_fifo_count().map_err(|err| format!("{err:?}"))?;
                    info!("FIFO Count: {} bytes", fifo_count);

                    if fifo_count >= 1024 {
                        // FIFO is full - reset to prevent overflow
                        info!("FIFO full - resetting");
                        mpu.reset_fifo().map_err(|err| format!("{err:?}"))?;
                        continue;
                    }

                    if fifo_count >= 28 {
                        let buf = mpu.read_fifo(&mut buffer).unwrap();
                        let q = Quaternion::from_bytes(&buf[..16]).unwrap().normalize();
                        let ypr = YawPitchRoll::from(q);
                        debug!("{:?}", ypr);
                    }

                    // // get roll and pitch estimate
                    // let angles = mpu.get_acc_angles().map_err(|err| format!("{err:?}"))?;

                    // // get sensor temp
                    // let temp = mpu.get_temp().map_err(|err| format!("{err:?}"))?;

                    // // get gyro data, scaled with sensitivity
                    // let gyro = mpu.get_gyro().map_err(|err| format!("{err:?}"))?;

                    // // get accelerometer data, scaled with sensitivity
                    // let acc = mpu.get_acc().map_err(|err| format!("{err:?}"))?;

                    let temp = mpu.temperature().map_err(|err| format!("{err:?}"))?;
                    println!("Temperature: {}°C", temp.celsius());

                    // debug!("angles: {angles}, gyro: {gyro}, acc: {acc}");

                    let elapsed = now - Instant::now();

                    debug!("elapsed: {}us", elapsed.as_micros());

                    if elapsed < poll_time {
                        std::thread::sleep(poll_time - elapsed);
                    }
                } else {
                    std::thread::sleep(poll_time);
                }
            }

            Ok(())
        });

        Ok(Self { worker })
    }
}
