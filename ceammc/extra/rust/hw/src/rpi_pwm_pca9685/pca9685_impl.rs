use std::ffi::CString;

use log::{debug, error};
use pwm_pca9685::{Address, Channel, Pca9685};
use rppal::i2c::I2c;

use crate::{
    hw_msg_cb, hw_notify_cb,
    rpi_pwm_pca9685::{HW_PCA9685_MAX_FREQ_HZ, HW_PCA9685_MIN_FREQ_HZ, HW_PCA9685_OSC_VALUE},
    str_to_cstr,
};

use super::{hw_pca9685, Reply, Request, HW_PCA9685_ALL_CHAN};

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

fn send_reply(tx: &std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb, rep: Reply) -> bool {
    if let Err(err) = tx.send(rep) {
        error!("send error: {err}");
        false
    } else {
        notify.notify();
        true
    }
}

fn send_error<T>(tx: &std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb, msg: T) -> CString
where
    T: Into<Vec<u8>>,
{
    let cstr = str_to_cstr(msg);
    error!("{}", cstr.to_str().unwrap());
    send_reply(tx, notify, Reply::Error(cstr.clone()));
    cstr
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
    pub fn new(_bus: i8, notify: hw_notify_cb, on_err: hw_msg_cb) -> Result<Self, CString> {
        let (req_tx, req_rx) = std::sync::mpsc::channel::<Request>();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), CString> {
            debug!("thread start");

            let i2c = I2c::new().map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;

            debug!("I2c init: {i2c:?}");

            let address = Address::default();

            debug!("using addr: {address:?}");

            let mut pwm = Pca9685::new(i2c, address)
                .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;

            let mut pwm_freq = FreqData::new(50.0);

            pwm.set_prescale(pwm_freq.prescale())
                .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;

            while let Ok(req) = req_rx.recv() {
                debug!("{req:?}");

                match req {
                    Request::Enable(state) => {
                        let _ = if state { pwm.enable() } else { pwm.disable() }
                            .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;
                    }
                    Request::SetChanOnOff(chan, on, off) => {
                        pwm.set_channel_on_off(to_channel(chan), on, off)
                            .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;
                    }
                    Request::SetFreq(freq_hz) => {
                        pwm_freq.set_freq(freq_hz);
                        pwm.set_prescale(pwm_freq.prescale())
                            .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;
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
                        .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;
                    }
                    Request::SetPeriod(period_ms) => {
                        pwm_freq.set_period_ms(period_ms);
                        pwm.set_prescale(pwm_freq.prescale())
                            .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;
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
                        .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;
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
                        .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;
                    }
                    Request::SetChanConst(chan, value, delay) => {
                        let chan = to_channel(chan);
                        let delay = phase_to_raw_pwm_wrapped(delay);

                        if value {
                            pwm.set_channel_full_on(chan, delay)
                        } else {
                            pwm.set_channel_full_off(chan)
                        }
                        .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;
                    }
                }
            }

            debug!("thread done");
            Ok(())
        });

        Ok(hw_pca9685 {
            tx: req_tx,
            rx: rep_rx,
            on_err,
        })
    }

    pub fn process_reply(pwm: *const Self) -> bool {
        if pwm.is_null() {
            error!("NULL pca8695 pointer");
            false
        } else {
            let pwm = unsafe { &*pwm };
            while let Ok(rep) = pwm.rx.try_recv() {
                match rep {
                    super::Reply::Error(cstring) => {
                        pwm.on_err.exec_raw(cstring.as_ptr());
                    }
                }
            }

            true
        }
    }

    pub fn send_request(pwm: *const Self, req: Request) -> bool {
        if pwm.is_null() {
            error!("NULL pca8695 pointer");
            false
        } else {
            let pwm = unsafe { &*pwm };

            if let Err(err) = pwm.tx.send(req) {
                error!("send error: {err}");
                pwm.on_err.exec(err.to_string().as_str());
                false
            } else {
                true
            }
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
