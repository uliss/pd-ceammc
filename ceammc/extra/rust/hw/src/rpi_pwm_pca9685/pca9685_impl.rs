use std::ffi::CString;

use crate::{
    hw_msg_cb, hw_notify_cb,
    i2c::{i2c_impl::create_i2c_bus, I2cAddress},
    rpi_pwm_pca9685::{HW_PCA9685_MAX_FREQ_HZ, HW_PCA9685_MIN_FREQ_HZ, HW_PCA9685_OSC_VALUE},
    send_debug, send_error,
};
use log::{debug, error};
use pwm_pca9685::{Address, Channel, Pca9685, ProgrammableAddress};

use super::{hw_pca9685, Pca9685Worker, Reply, Request, HW_PCA9685_ALL_CHAN};

const PWM_MAX: u16 = 4096;

fn to_channel(ch: u8) -> Channel {
    match ch {
        0 => Channel::C0,
        1 => Channel::C1,
        2 => Channel::C2,
        3 => Channel::C3,
        4 => Channel::C4,
        5 => Channel::C5,
        6 => Channel::C6,
        7 => Channel::C7,
        8 => Channel::C8,
        9 => Channel::C9,
        10 => Channel::C10,
        11 => Channel::C11,
        12 => Channel::C12,
        13 => Channel::C13,
        14 => Channel::C14,
        15 => Channel::C15,
        HW_PCA9685_ALL_CHAN => Channel::All,
        _ => Channel::All,
    }
}

impl Into<ProgrammableAddress> for crate::rpi_pwm_pca9685::hw_pca8695_prog_address {
    fn into(self) -> ProgrammableAddress {
        use crate::rpi_pwm_pca9685::hw_pca8695_prog_address;
        use pwm_pca9685::ProgrammableAddress::*;

        match self {
            hw_pca8695_prog_address::Subaddress1 => Subaddress1,
            hw_pca8695_prog_address::Subaddress2 => Subaddress2,
            hw_pca8695_prog_address::Subaddress3 => Subaddress3,
            hw_pca8695_prog_address::AllCall => AllCall,
        }
    }
}

fn phase_to_raw_pwm_wrapped(x: f32) -> u16 {
    (((((x * PWM_MAX as f32).round() as i64) % PWM_MAX as i64) + PWM_MAX as i64) % PWM_MAX as i64)
        as u16
}

fn phase_to_raw_pwm_clipped(x: f32) -> u16 {
    (x.clamp(0.0, 1.0) * PWM_MAX as f32).round() as u16
}

struct FreqData {
    freq: f32,
}

impl FreqData {
    fn new(freq: f32) -> Self {
        let freq = freq.clamp(HW_PCA9685_MIN_FREQ_HZ as f32, HW_PCA9685_MAX_FREQ_HZ as f32);
        FreqData { freq }
    }

    fn set_freq(&mut self, freq: f32) {
        *self = Self::new(freq);
    }

    fn set_period_ms(&mut self, period: f32) {
        self.set_freq(1000.0 / period);
    }

    fn prescale(&self) -> u8 {
        (HW_PCA9685_OSC_VALUE as f32 / self.freq)
            .round()
            .clamp(0.0, 255.0) as u8
    }

    fn calc_width(&self, width_ms: f32) -> u16 {
        let period_ms = 1000.0 / self.freq;
        let width_ms = width_ms.clamp(0.0, period_ms);

        ((PWM_MAX as f32 * width_ms) / period_ms)
            .round()
            .clamp(0.0, PWM_MAX as f32) as u16
    }
}

impl hw_pca9685 {
    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: hw_notify_cb,
        on_msg: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (worker, rx, tx) = Pca9685Worker::new(on_msg);

        worker.spawn(tx.clone(), notify, move || {
            let i2c = create_i2c_bus(i2c_bus, &tx, notify)?;
            debug!("I2C init: {i2c:?}");

            let address = match i2c_addr {
                I2cAddress::Default => Address::default(),
                I2cAddress::Alt => return Err(format!("no alternative address")),
                I2cAddress::Invalid(x) => return Err(format!("invalid I2c address: {x}")),
                I2cAddress::Addr(addr) => Address::from(addr),
            };
            debug!("using I2C addr: {address:?}");

            send_debug(
                &tx,
                notify,
                format!("connected to bus:{} addr:{:?}", i2c.bus(), address).as_str(),
            );

            let mut pwm = Pca9685::new(i2c, address).map_err(|err| err.to_string())?;
            let mut pwm_freq = FreqData::new(50.0);

            pwm.set_prescale(pwm_freq.prescale())
                .map_err(|err| err.to_string())?;

            while let Ok(req) = rx.recv() {
                debug!("{req:?}");

                match req {
                    Request::Enable(state) => {
                        let _ = if state { pwm.enable() } else { pwm.disable() }.unwrap_or_else(
                            |err| {
                                send_error(&tx, notify, err.to_string().as_str());
                            },
                        );
                    }
                    Request::SetChanOnOff(chan, on, off) => {
                        pwm.set_channel_on_off(to_channel(chan), on, off)
                            .unwrap_or_else(|err| {
                                send_error(&tx, notify, err.to_string().as_str());
                            });
                    }
                    Request::SetFreq(freq_hz) => {
                        pwm_freq.set_freq(freq_hz);
                        pwm.set_prescale(pwm_freq.prescale()).unwrap_or_else(|err| {
                            send_error(&tx, notify, err.to_string().as_str());
                        });
                    }
                    Request::SetPolarity(polarity) => {
                        pwm.set_output_logic_state(match polarity {
                            crate::rpi_pwm::hw_rpi_pwm_polarity::NORMAL => {
                                pwm_pca9685::OutputLogicState::Direct
                            }
                            crate::rpi_pwm::hw_rpi_pwm_polarity::INVERSE => {
                                pwm_pca9685::OutputLogicState::Inverted
                            }
                        })
                        .unwrap_or_else(|err| {
                            send_error(&tx, notify, err.to_string().as_str());
                        });
                    }
                    Request::SetPeriod(period_ms) => {
                        pwm_freq.set_period_ms(period_ms);
                        pwm.set_prescale(pwm_freq.prescale()).unwrap_or_else(|err| {
                            send_error(&tx, notify, err.to_string().as_str());
                        });
                    }
                    Request::SetChanPulseWidth(chan, width_ms, phase) => {
                        let chan = to_channel(chan);

                        let on_pos = phase_to_raw_pwm_wrapped(phase);
                        let off_pos = on_pos + pwm_freq.calc_width(width_ms);

                        if on_pos == 0 && off_pos >= PWM_MAX {
                            pwm.set_channel_full_on(chan, on_pos)
                        } else {
                            pwm.set_channel_on_off(chan, on_pos, off_pos)
                        }
                        .unwrap_or_else(|err| {
                            send_error(&tx, notify, err.to_string().as_str());
                        });
                    }
                    Request::SetChanDutyCycle(chan, duty, phase) => {
                        let chan = to_channel(chan);

                        let on_pos: u16;
                        let off_pos: u16;

                        match phase {
                            Some(phase) => {
                                on_pos = phase_to_raw_pwm_wrapped(phase);
                                off_pos = phase_to_raw_pwm_wrapped(phase + duty);
                            }
                            None => {
                                on_pos = 0;
                                off_pos = phase_to_raw_pwm_clipped(duty);
                            }
                        }

                        if on_pos == 0 && off_pos == PWM_MAX {
                            pwm.set_channel_full_on(chan, on_pos)
                        } else {
                            pwm.set_channel_on_off(chan, on_pos, off_pos)
                        }
                        .unwrap_or_else(|err| {
                            send_error(&tx, notify, err.to_string().as_str());
                        });
                    }
                    Request::SetChanConst(chan, value, delay) => {
                        let chan = to_channel(chan);
                        let delay = phase_to_raw_pwm_wrapped(delay);

                        if value {
                            pwm.set_channel_full_on(chan, delay)
                        } else {
                            pwm.set_channel_full_off(chan)
                        }
                        .unwrap_or_else(|err| {
                            send_error(&tx, notify, err.to_string().as_str());
                        });
                    }
                    Request::UseProgAddress(sub_addr, i2c_addr) => {
                        let sub_addr = sub_addr.into();
                        pwm.set_programmable_address(sub_addr, i2c_addr)
                            .and_then(|_| pwm.enable_programmable_address(sub_addr))
                            .and_then(|_| pwm.set_address(Address::from(i2c_addr)))
                            .unwrap_or_else(|err| {
                                send_error(&tx, notify, err.to_string().as_str());
                            });
                    }
                    Request::DisableProgAddress(addr) => {
                        pwm.disable_programmable_address(addr.into())
                            .unwrap_or_else(|err| {
                                send_error(&tx, notify, err.to_string().as_str());
                            });
                    }
                    Request::Restart => {
                        pwm.restart(&mut rppal::hal::Delay::new())
                            .unwrap_or_else(|err| {
                                send_error(&tx, notify, err.to_string().as_str());
                            });
                    }
                    Request::EnableRestartAndDisable => {
                        pwm.enable_restart_and_disable().unwrap_or_else(|err| {
                            send_error(&tx, notify, err.to_string().as_str());
                        });
                    }
                }
            }

            //
            Ok(())
        });

        Ok(Self { worker })
    }

    pub fn process_reply_ptr(pwm: *const Self) -> bool {
        if pwm.is_null() {
            error!("NULL pca8695 pointer");
            false
        } else {
            let pwm = unsafe { &*pwm };
            pwm.worker.process_reply(&|rep| match rep {
                Reply::Message(level, msg) => pwm.worker.pd_message(level, &msg),
            })
        }
    }

    pub fn send_request_ptr(pwm: *const Self, req: Request) -> bool {
        if pwm.is_null() {
            error!("NULL pca8695 pointer");
            false
        } else {
            let pwm = unsafe { &*pwm };
            pwm.worker.send_request(req)
        }
    }
}

#[cfg(test)]
mod tests {
    use crate::rpi_pwm_pca9685::pca9685_impl::{
        phase_to_raw_pwm_clipped, phase_to_raw_pwm_wrapped,
    };

    use super::FreqData;

    #[test]
    fn convert() {
        assert_eq!(phase_to_raw_pwm_wrapped(0.0), 0);
        assert_eq!(phase_to_raw_pwm_wrapped(0.25), 1024);
        assert_eq!(phase_to_raw_pwm_wrapped(0.5), 2048);
        assert_eq!(phase_to_raw_pwm_wrapped(0.75), 3072);
        assert_eq!(phase_to_raw_pwm_wrapped(1.0), 0);
        assert_eq!(phase_to_raw_pwm_wrapped(1.25), 1024);
        assert_eq!(phase_to_raw_pwm_wrapped(-0.25), 3072);
        assert_eq!(phase_to_raw_pwm_wrapped(-0.5), 2048);

        assert_eq!(phase_to_raw_pwm_clipped(0.0), 0);
        assert_eq!(phase_to_raw_pwm_clipped(0.25), 1024);
        assert_eq!(phase_to_raw_pwm_clipped(0.5), 2048);
        assert_eq!(phase_to_raw_pwm_clipped(0.75), 3072);
        assert_eq!(phase_to_raw_pwm_clipped(1.0), 4096);
        assert_eq!(phase_to_raw_pwm_clipped(1.25), 4096);
        assert_eq!(phase_to_raw_pwm_clipped(-0.25), 0);
        assert_eq!(phase_to_raw_pwm_clipped(-0.5), 0);
    }

    #[test]
    fn freq() {
        let f = FreqData::new(50.0);
        assert_eq!(f.freq, 50.0);
        assert_eq!(f.calc_width(10.0), 2048);
        assert_eq!(f.calc_width(5.0), 1024);
    }
}
