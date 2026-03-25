use std::{
    ffi::CString,
    time::{Duration, Instant},
};

use ads1x1x::{channel, Ads1x1x, FullScaleRange, TargetAddr};
use log::{debug, error};
use pwm_pca9685::nb::block;

use crate::{
    ads1115::{Reply, Request, HW_ADC_ADS1115_MAX_POLL_TIME_MSEC, HW_ADC_ADS1115_MIN_POLL_TIME_MSEC},
    hw_msg_cb, hw_notify_cb,
    i2c::{i2c_impl::create_i2c_bus, I2cAddress},
    send_debug, send_error, send_reply, WorkerCommand,
};

use super::{hw_i2c_ads1115, hw_i2c_ads1115_data_cb, hw_i2c_ads1115_measure_mode, hw_i2c_ads1115_range, Ads1115Worker};

impl hw_i2c_ads1115 {
    fn to_fsr(range: hw_i2c_ads1115_range) -> FullScaleRange {
        use ads1x1x::FullScaleRange as FSR;
        use hw_i2c_ads1115_range as R;

        match range {
            R::Within_0_256V => FSR::Within0_256V,
            R::Within_0_512V => FSR::Within0_512V,
            R::Within_1_024V => FSR::Within1_024V,
            R::Within_2_048V => FSR::Within2_048V,
            R::Within_4_096V => FSR::Within4_096V,
            R::Within_6_144V => FSR::Within6_144V,
        }
    }

    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        mode: hw_i2c_ads1115_measure_mode,
        notify: hw_notify_cb,
        on_msg: hw_msg_cb,
        on_data: hw_i2c_ads1115_data_cb,
    ) -> Result<Self, CString> {
        let (mut worker, rx, tx) = Ads1115Worker::new(on_msg, Some(32));

        worker.spawn(tx.clone(), notify, move || -> Result<(), String> {
            let i2c = create_i2c_bus(i2c_bus)?;
            debug!("I2C init: {i2c:?}");

            let addr = match i2c_addr {
                I2cAddress::Default | I2cAddress::Addr(0x48) => TargetAddr::default(),
                I2cAddress::Alt | I2cAddress::Addr(0x49) => TargetAddr::Vdd,
                I2cAddress::Addr(0x4A) => TargetAddr::Sda,
                I2cAddress::Addr(0x4B) => TargetAddr::Scl,
                I2cAddress::Auto => TargetAddr::default(),
                _ => return Err(format!("invalid i2c address: {i2c_addr:?}")),
            };

            let i2c_bus = i2c.bus();
            let mut adc = Ads1x1x::new_ads1115(i2c, addr);

            send_debug(
                &tx,
                notify,
                format!("connected to ADS1115 with bus={i2c_bus}, addr={i2c_addr:?}").as_str(),
            )
            .to_err()?;

            let mut poll_mode = false;
            let mut poll_time = Duration::from_millis(super::HW_ADC_ADS1115_DEF_POLL_TIME_MSEC.into());
            let mut measure_mode = mode;

            'outer: loop {
                match rx.try_recv() {
                    Ok(WorkerCommand::Quit) => break 'outer,
                    Ok(WorkerCommand::Command(req)) => {
                        debug!("{req:?}");
                        match req {
                            Request::MeasureChan(chan) => {
                                use ads1x1x::channel::*;
                                use hw_i2c_ads1115_measure_mode::*;
                                // single measure
                                match match (&measure_mode, chan) {
                                    (Single, 0) => block!(adc.read(SingleA0)),
                                    (Single, 1) => block!(adc.read(SingleA1)),
                                    (Single, 2) => block!(adc.read(SingleA2)),
                                    (Single, 3) => block!(adc.read(SingleA3)),
                                    (Diff, 0) => block!(adc.read(DifferentialA0A1)),
                                    (Diff, 1) => block!(adc.read(DifferentialA0A3)),
                                    (Diff, 2) => block!(adc.read(DifferentialA1A3)),
                                    (Diff, 3) => block!(adc.read(DifferentialA2A3)),
                                    _ => {
                                        send_error(&tx, notify, format!("invalid channel: {chan}").as_str())
                                            .to_err()?;
                                        continue;
                                    }
                                } {
                                    Ok(res) => {
                                        debug!("measure: {res}");
                                        send_reply(Reply::Measure(chan, res), &tx, notify).to_err()?;
                                    }
                                    Err(err) => {
                                        send_error(&tx, notify, format!("measure error: {err:?}").as_str()).to_err()?;
                                    }
                                }
                            }
                            Request::MeasureAll => {
                                let result = match measure_mode {
                                    hw_i2c_ads1115_measure_mode::Single => {
                                        let a0 = block!(adc.read(channel::SingleA0)).unwrap_or_default();
                                        let a1 = block!(adc.read(channel::SingleA1)).unwrap_or_default();
                                        let a2 = block!(adc.read(channel::SingleA2)).unwrap_or_default();
                                        let a3 = block!(adc.read(channel::SingleA3)).unwrap_or_default();
                                        (a0, a1, a2, a3)
                                    }
                                    hw_i2c_ads1115_measure_mode::Diff => {
                                        let d0 = block!(adc.read(channel::DifferentialA0A1)).unwrap_or_default();
                                        let d1 = block!(adc.read(channel::DifferentialA0A3)).unwrap_or_default();
                                        let d2 = block!(adc.read(channel::DifferentialA1A3)).unwrap_or_default();
                                        let d3 = block!(adc.read(channel::DifferentialA2A3)).unwrap_or_default();
                                        (d0, d1, d2, d3)
                                    }
                                };

                                send_reply(Reply::MeasureAll(result.into()), &tx, notify).to_err()?;
                            }
                            Request::SetFullScaleRange(range) => {
                                adc.set_full_scale_range(Self::to_fsr(range))
                                    .map_err(|err| format!("SetFullScaleRange: {err:?}"))
                                    .unwrap_or_default();
                            }
                            Request::Poll(state) => poll_mode = state,
                            Request::SetPollTime(msec) => {
                                poll_time = Duration::from_millis(
                                    msec.clamp(HW_ADC_ADS1115_MIN_POLL_TIME_MSEC, HW_ADC_ADS1115_MAX_POLL_TIME_MSEC)
                                        .into(),
                                );
                            }
                            Request::SetMeasureMode(mode) => {
                                measure_mode = mode;
                            }
                        }
                    }
                    Err(err) => match err {
                        std::sync::mpsc::TryRecvError::Empty => {} // just no request
                        std::sync::mpsc::TryRecvError::Disconnected => break 'outer,
                    },
                }

                if poll_mode {
                    let now = Instant::now();
                    let result = match &measure_mode {
                        hw_i2c_ads1115_measure_mode::Single => {
                            let a0 = block!(adc.read(channel::SingleA0)).unwrap_or_default();
                            let a1 = block!(adc.read(channel::SingleA1)).unwrap_or_default();
                            let a2 = block!(adc.read(channel::SingleA2)).unwrap_or_default();
                            let a3 = block!(adc.read(channel::SingleA3)).unwrap_or_default();
                            (a0, a1, a2, a3)
                        }
                        hw_i2c_ads1115_measure_mode::Diff => {
                            let d0 = block!(adc.read(channel::DifferentialA0A1)).unwrap_or_default();
                            let d1 = block!(adc.read(channel::DifferentialA0A3)).unwrap_or_default();
                            let d2 = block!(adc.read(channel::DifferentialA1A3)).unwrap_or_default();
                            let d3 = block!(adc.read(channel::DifferentialA2A3)).unwrap_or_default();
                            (d0, d1, d2, d3)
                        }
                    };

                    send_reply(Reply::MeasureAll(result), &tx, notify).to_err()?;

                    let elapsed = Instant::now() - now;
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

    pub fn send_request_ptr(adc: *mut Self, req: Request) -> bool {
        if adc.is_null() {
            error!("NULL adc pointer");
            false
        } else {
            let adc = unsafe { &*adc };
            adc.worker.send_request(req)
        }
    }

    pub fn process_reply_ptr(adc: *mut Self) -> bool {
        if adc.is_null() {
            error!("NULL adc pointer");
            false
        } else {
            let adc = unsafe { &*adc };
            adc.worker.process_reply(&|rep| match rep {
                Reply::Message(level, err) => adc.worker.pd_message(level, &err),
                Reply::Measure(chan, value) => {
                    (adc.on_data.cb_chan)(adc.on_data.user, chan, value);
                }
                Reply::MeasureAll((a0, a1, a2, a3)) => {
                    (adc.on_data.cb_all)(adc.on_data.user, a0, a1, a2, a3);
                }
            })
        }
    }
}
