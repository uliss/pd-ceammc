use std::ffi::CString;

use log::{debug, error};
use pwm_pca9685::{Address, Channel, Pca9685};
use rppal::i2c::I2c;

use crate::{
    hw_msg_cb, hw_notify_cb,
    rpi_pwm_pca9685::{
        HW_PCA9685_MAX_FREQ_HZ, HW_PCA9685_MAX_PERIOD_MS, HW_PCA9685_MIN_FREQ_HZ,
        HW_PCA9685_MIN_PERIOD_MS, HW_PCA9685_OSC_VALUE,
    },
    str_to_cstr,
};

use super::{hw_pca9685, Reply, Request, HW_PCA9685_ALL_CHAN};

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

fn f32_to_pos(x: f32) -> u16 {
    (((((x * 4096.0).round() as i64) % 4096) + 4096) % 4096) as u16
}

impl hw_pca9685 {
    pub fn new(bus: i8, notify: hw_notify_cb, on_err: hw_msg_cb) -> Result<Self, CString> {
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

            pwm.set_prescale(100)
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
                        let freq_hz = freq_hz
                            .clamp(HW_PCA9685_MIN_FREQ_HZ as f32, HW_PCA9685_MAX_FREQ_HZ as f32);
                        let prescale =
                            ((HW_PCA9685_OSC_VALUE as f32 / freq_hz).round() as u8).clamp(0, 255);

                        debug!("set PWM freq: {freq_hz}Hz (prescale: {prescale})");

                        pwm.set_prescale(prescale)
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
                        let period =
                            period_ms.clamp(HW_PCA9685_MIN_PERIOD_MS, HW_PCA9685_MAX_PERIOD_MS);
                        let prescale = ((HW_PCA9685_OSC_VALUE as f32 * period * 0.001).round()
                            as u8)
                            .clamp(0, 255);

                        debug!("set PWM freq: {period_ms}ms (prescale: {prescale})");

                        pwm.set_prescale(prescale)
                            .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;
                    }
                    Request::SetChanPulseWidth(chan, width_ms, phase) => {
                        
                        // pwm.set_channel_on(chan, 0)
                        // .and_then(|_| {
                        //     pwm.set_channel_off(chan, (4095.0 * duty).round() as u16)?;
                        //     Ok(())
                        // })
                        // .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;
                    }
                    Request::SetChanDutyCycle(chan, duty, phase) => {
                        let chan = to_channel(chan);
                        pwm.set_channel_on(chan, f32_to_pos(phase))
                            .and_then(|_| {
                                pwm.set_channel_off(chan, f32_to_pos(phase + duty))?;
                                Ok(())
                            })
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
    use crate::rpi_pwm_pca9685::pca9685_impl::f32_to_pos;

    #[test]
    fn convert() {
        assert_eq!(f32_to_pos(0.0), 0);
        assert_eq!(f32_to_pos(0.25), 1024);
        assert_eq!(f32_to_pos(0.5), 2048);
        assert_eq!(f32_to_pos(0.75), 3072);
        assert_eq!(f32_to_pos(1.0), 0);
        assert_eq!(f32_to_pos(1.25), 1024);
        assert_eq!(f32_to_pos(-0.25), 3072);
        assert_eq!(f32_to_pos(-0.5), 2048);
        assert_eq!(f32_to_pos(-0.75), 1024);
        assert_eq!(f32_to_pos(-1.0), 0);
        assert_eq!(f32_to_pos(-1.25), 3072);
    }
}
