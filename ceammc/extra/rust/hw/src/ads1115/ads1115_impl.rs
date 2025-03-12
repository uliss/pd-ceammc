use std::ffi::CString;

use ads1x1x::{channel, Ads1x1x, ChannelId, FullScaleRange, ModeChangeError, TargetAddr};
use log::debug;
use pwm_pca9685::nb::block;

use crate::{
    ads1115::{Reply, Request},
    hw_msg_cb, hw_notify_cb,
    i2c::{i2c_impl::create_i2c_bus, I2cAddress},
    send_error, send_reply,
};

use super::{hw_i2c_ads1115, Ads1115Worker};

impl hw_i2c_ads1115 {
    fn to_fsr(range: u8) -> FullScaleRange {
        use ads1x1x::FullScaleRange as FSR;

        match range {
            0 => FSR::Within0_256V,
            1 => FSR::Within0_512V,
            2 => FSR::Within1_024V,
            3 => FSR::Within2_048V,
            4 => FSR::Within4_096V,
            _ => FSR::Within6_144V,
        }
    }

    pub fn new_oneshot(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (worker, rx, tx) = Ads1115Worker::new(on_err);

        worker.spawn(tx.clone(), notify, move || {
            let i2c = create_i2c_bus(i2c_bus, &tx, notify)?;
            debug!("I2C init: {i2c:?}");

            let mut adc = Ads1x1x::new_ads1115(i2c, TargetAddr::default());

            while let Ok(req) = rx.recv() {
                match req {
                    Request::Measure(chan, diff) => {
                        match match (chan, diff) {
                            (0, false) => block!(adc.read(channel::SingleA0)),
                            (1, false) => block!(adc.read(channel::SingleA1)),
                            (2, false) => block!(adc.read(channel::SingleA2)),
                            (3, false) => block!(adc.read(channel::SingleA3)),
                            (0, true) => block!(adc.read(channel::DifferentialA0A1)),
                            (1, true) => block!(adc.read(channel::DifferentialA0A3)),
                            (2, true) => block!(adc.read(channel::DifferentialA1A3)),
                            (3, true) => block!(adc.read(channel::DifferentialA2A3)),
                            _ => {
                                send_error(&tx, notify, format!("invalid channel: chan").as_str());
                                continue;
                            }
                        } {
                            Ok(res) => {
                                debug!("measure: {res}");
                                send_reply(Reply::Measure(chan, res), &tx, notify);
                            }
                            Err(err) => {
                                send_error(&tx, notify, format!("measure error: {err:?}").as_str());
                            }
                        }
                    }
                    Request::SetFullScaleRange(range) => {
                        adc.set_full_scale_range(Self::to_fsr(range))
                            .map_err(|err| format!("SetFullScaleRange: {err:?}"))
                            .unwrap_or_default();
                    }
                    Request::MeasureAll(diff) => {
                        let res = if !diff {
                            let a0 = block!(adc.read(channel::SingleA0)).unwrap_or_default();
                            let a1 = block!(adc.read(channel::SingleA1)).unwrap_or_default();
                            let a2 = block!(adc.read(channel::SingleA2)).unwrap_or_default();
                            let a3 = block!(adc.read(channel::SingleA3)).unwrap_or_default();
                            (a0, a1, a2, a3)
                        } else {
                            let d0 =
                                block!(adc.read(channel::DifferentialA0A1)).unwrap_or_default();
                            let d1 =
                                block!(adc.read(channel::DifferentialA0A3)).unwrap_or_default();
                            let d2 =
                                block!(adc.read(channel::DifferentialA1A3)).unwrap_or_default();
                            let d3 =
                                block!(adc.read(channel::DifferentialA2A3)).unwrap_or_default();
                            (d0, d1, d2, d3)
                        };

                        send_reply(Reply::MeasureAll(res.into()), &tx, notify);
                    }
                }
            }

            Ok(())
        });

        Ok(Self { worker })
    }

    fn ch<CH>(chan: CH) {

    }

    pub fn new_continuos(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (worker, rx, tx) = Ads1115Worker::new(on_err);

        worker.spawn(tx.clone(), notify, move || {
            let i2c = create_i2c_bus(i2c_bus, &tx, notify)?;
            debug!("I2C init: {i2c:?}");

            let mut adc = Ads1x1x::new_ads1115(i2c, TargetAddr::default())
                .into_continuous()
                .map_err(|ModeChangeError::I2C(err, _)| format!("I2c error: {err:?}"))?;

            let mut current_chan = 0;
            // let mut poll_enabled = true;

            Self::ch(channel::SingleA0);

            loop {
                match adc.read() {
                    Ok(res) => {
                        send_reply(Reply::Measure(current_chan, res), &tx, notify);
                    }
                    Err(err) => {}
                }

                match rx.recv() {
                    Ok(req) => match req {
                        Request::Measure(_, _) => todo!(),
                        Request::MeasureAll(_) => todo!(),
                        Request::SetFullScaleRange(range) => {
                            adc.set_full_scale_range(Self::to_fsr(range))
                                .map_err(|err| format!("SetFullScaleRange: {err:?}"))
                                .unwrap_or_default();
                        }
                    },
                    Err(err) => {
                        break;
                    }
                }
            }

            Ok(())
        });

        Ok(Self { worker })
    }
}
