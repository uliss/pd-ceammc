use std::ffi::CString;

use log::{debug, error};
use rppal::gpio::Gpio;

use crate::{hw_msg_cb, hw_notify_cb};

use super::{hw_max7219, Request};

#[derive(Debug, PartialEq)]
struct float_fmt {
    dot_pos: u8,
    leading_zero: bool,
}

fn float2digits(v: f32, precision: u8) -> Option<(i32, Option<float_fmt>)> {
    let mut float_str = format!("{v:.0$}", precision.clamp(0, 7) as usize);
    let dot_pos = float_str
        .find('.')
        .map(|pos| float_str.len() - (pos + 1))
        .map(|x| float_fmt {
            dot_pos: x as u8,
            leading_zero: v.abs() < 1.0,
        });

    // println!("'{float_str}': {dot_pos:?}");
    float_str.retain(|c| c != '.');
    if float_str.len() >= 8 {
        return None;
    }

    float_str.parse::<i32>().map(|x| (x, dot_pos)).ok()
}

impl hw_max7219 {
    pub fn new(displays: usize, _notify: hw_notify_cb, on_err: hw_msg_cb) -> Result<Self, CString> {
        let (tx, rx) = std::sync::mpsc::channel::<Request>();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("worker thread start");

            let gpio = Gpio::new().map_err(|err| {
                error!("{err}");
                err.to_string()
            })?;

            debug!("GPIO init done");

            const MOSI_PIN: u8 = 10; // [DATA] BCM GPIO 10 (physical pin 19)
            const SCLK_PIN: u8 = 11; // [CLK]  BCM GPIO 11 (physical pin 23)
            const CS_PIN: u8 = 8; //    [CS]   SS:   Ss0 BCM GPIO 8 (physical pin 24)

            let data = gpio
                .get(MOSI_PIN)
                .map_err(|err| {
                    error!("{err}");
                    err.to_string()
                })?
                .into_output_low();

            let sclk = gpio
                .get(SCLK_PIN)
                .map_err(|err| {
                    error!("{err}");
                    err.to_string()
                })?
                .into_output_low();

            let cs = gpio
                .get(CS_PIN)
                .map_err(|err| {
                    error!("{err}");
                    err.to_string()
                })?
                .into_output_low();

            debug!(
                "init GPIO pins: DATA={} CS={} CLK={}",
                data.pin(),
                cs.pin(),
                sclk.pin()
            );

            let mut display =
                max7219::MAX7219::from_pins(displays, data, cs, sclk).map_err(|err| {
                    let err = match err {
                        max7219::DataError::Spi => "SPI init error",
                        max7219::DataError::Pin => "Pin init error",
                    };
                    error!("{err}");
                    err
                })?;

            // display.set_decode_mode(0, mode);

            // make sure to wake the display up
            display.power_on().unwrap();
            display.clear_display(0).unwrap();
            display.clear_display(1).unwrap();
            display.clear_display(2).unwrap();
            display.clear_display(3).unwrap();

            // display.write_raw(0, &[1, 1, 1, 1, 1, 1, 1, 1]).unwrap();
            // display.write_raw(1, &[3, 3, 3, 3, 3, 3, 3, 3]).unwrap();
            // display.write_raw(2, &[5, 5, 5, 5, 5, 5, 5, 5]).unwrap();
            // display
            //     .write_raw(3, &[0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff])
            //     .unwrap();

            while let Ok(req) = rx.recv() {
                debug!("{req:?}");

                match req {
                    Request::Intensity(addr, val) => match addr {
                        Some(addr) => display
                            .set_intensity(addr, val.clamp(0, 255) as u8)
                            .unwrap(),
                        None => {
                            for x in 0..4 {
                                display.set_intensity(x, val.clamp(0, 255) as u8).unwrap();
                            }
                        }
                    },
                    Request::PowerOn(state) => {
                        if state {
                            display.power_on().unwrap();
                        } else {
                            display.power_off().unwrap();
                        }
                    }
                    Request::Clear(addr) => match addr {
                        Some(addr) => {
                            display.clear_display(addr).unwrap();
                        }
                        None => {
                            for x in 0..4 {
                                display.clear_display(x).unwrap();
                            }
                        }
                    },
                    Request::WriteInt(addr, value) => {
                        display.write_integer(addr, value).unwrap_or_else(|_| {
                            error!("integer overflow");
                        });
                    }
                    Request::WriteHex(addr, value) => {
                        display.write_hex(addr, value).unwrap_or_else(|_| {
                            error!("hex overflow");
                        });
                    }
                    Request::WriteFloat(addr, v, precision) => match float2digits(v, precision) {
                        Some((digits, dots)) => {
                            display.write_integer(addr, digits).unwrap_or_else(|_| {});
                        }
                        None => {}
                    },
                    Request::WriteDigit(addr, digit, data) => {
                        display
                            .write_raw_byte(addr, digit, data)
                            .unwrap_or_else(|_| {
                                error!("write digit overflow");
                            });
                    }
                }
            }

            debug!("worker thread done");

            Ok(())
        });

        Ok(hw_max7219 { tx, on_err })
    }

    pub fn send(&self, req: Request) -> bool {
        if let Err(err) = self.tx.send(req) {
            error!("{err}");
            self.on_err.exec(err.to_string().as_str());
            false
        } else {
            true
        }
    }
}

#[cfg(test)]
mod tests {
    // use super::*;

    use crate::max7219::max7219_impl::{float2digits, float_fmt};

    fn make_fmt_float(v: i32, dot_pos: u8, leading_zero: bool) -> Option<(i32, Option<float_fmt>)> {
        Some((
            v,
            Some(float_fmt {
                dot_pos,
                leading_zero,
            }),
        ))
    }

    fn make_fmt_int(v: i32) -> Option<(i32, Option<float_fmt>)> {
        Some((v, None))
    }

    #[test]
    fn digits() {
        assert_eq!(float2digits(0.0, 0), make_fmt_int(0));
        assert_eq!(float2digits(0.0, 1), make_fmt_float(0, 1, true));
        assert_eq!(float2digits(0.0, 2), make_fmt_float(0, 2, true));
        assert_eq!(float2digits(0.5, 0), make_fmt_int(0));
        assert_eq!(float2digits(0.5, 1), make_fmt_float(5, 1, true));
        assert_eq!(float2digits(0.5, 2), make_fmt_float(50, 2, true));
        assert_eq!(float2digits(0.5, 3), make_fmt_float(500, 3, true));
        assert_eq!(float2digits(0.5, 4), make_fmt_float(5000, 4, true));
        assert_eq!(float2digits(-0.5, 4), make_fmt_float(-5000, 4, true));
        assert_eq!(float2digits(-0.5, 5), make_fmt_float(-50000, 5, true));
        assert_eq!(float2digits(-0.5, 6), None); // -0.5000000 - overflow
        assert_eq!(float2digits(0.25, 0), make_fmt_int(0));
        assert_eq!(float2digits(0.25, 1), make_fmt_float(2, 1, true));
        assert_eq!(float2digits(0.251, 1), make_fmt_float(3, 1, true));
        assert_eq!(float2digits(0.12345, 5), make_fmt_float(12345, 5, true));
        assert_eq!(float2digits(1.0, 0), make_fmt_int(1));
        assert_eq!(float2digits(12.0, 0), make_fmt_int(12));
        assert_eq!(float2digits(123.21, 0), make_fmt_int(123));
        assert_eq!(float2digits(123.21, 1), make_fmt_float(1232, 1, false));
        assert_eq!(float2digits(123.21, 2), make_fmt_float(12321, 2, false));
        assert_eq!(float2digits(123.21, 3), make_fmt_float(123210, 3, false));
    }
}
