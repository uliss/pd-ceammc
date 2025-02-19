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

fn map_char(c: char) -> u8 {
    match c as char {
        ' ' => 0b0000_0000, // "blank"
        '.' => 0b1000_0000,
        '-' => 0b0000_0001, // -
        '_' => 0b0000_1000, // _
        '0' => 0b0111_1110,
        '1' => 0b0011_0000,
        '2' => 0b0110_1101,
        '3' => 0b0111_1001,
        '4' => 0b0011_0011,
        '5' => 0b0101_1011,
        '6' => 0b0101_1111,
        '7' => 0b0111_0000,
        '8' => 0b0111_1111,
        '9' => 0b0111_1011,
        'a' | 'A' | 'а' | 'А' => 0b0111_0111,
        'В' | 'B' => 0b0111_1111,
        'б' | 'Б' => 0b0101_1111,
        'b' | 'ь' | 'Ь' => 0b0001_1111,
        'c' | 'с' => 0b001_1101,
        'C' | 'С' => 0b0100_1110,
        'd' => 0b0011_1101,
        'e' | 'E' | 'Е' | 'е' => 0b0100_1111,
        'f' | 'F' => 0b0100_0111,
        'g' | 'G' => 0b0101_1110,
        'H' | 'Н' => 0b0011_0111,
        'h' => 0b0011_0111,
        'i' | 'I' => 0b0011_0000,
        'j' | 'J' => 0b0011_1100,
        // K undoable
        'l' | 'L' => 0b0000_1110,
        // M undoable
        'n' | 'N' => 0b0001_0101,
        'o' | 'O' | 'о' | 'О' => 0b0111_1110,
        'p' | 'P' | 'р' | 'Р' => 0b0110_0111,
        'q' => 0b0111_0011,
        'r' | 'г' | 'R' => 0b0000_1101,
        's' | 'S' => 0b0101_1011,
        't' | 'T' => 0b0001_0111,
        'U' => 0b0011_1110,
        'u' => 0b0001_1100,
        'Г' => 0b0100_0110,
        'П' | 'п' => 0b0111_0110,
        'У' | 'у' => 0b0011_1011,
        'ч' | 'Ч' => 0b0001_1011,
        'э' | 'Э' | 'з' | 'З' => 0b0111_1001,
        // V undoable
        // W undoable
        // X undoable
        // Y undoable
        // Z undoable
        _ => 0b1110_0101, // ?
    }
}

fn encode_string(str: &String, dots: u8) -> [u8; 8] {
    let bytes = str.chars().map(|x| map_char(x)).collect::<Vec<_>>();

    let mut buf: [u8; 8] = [0; 8];
    for (idx, (b, c)) in buf.iter_mut().zip(bytes.iter().rev()).enumerate() {
        *b = *c;
        if dots & (1 << idx) > 0 {
            *b |= 0b1000_0000;
        }
    }

    buf
}

fn float2str(v: f32, precision: u8) -> Option<([u8; 8], u8)> {
    const MAX_LEN: usize = 8;

    let mut buf: [u8; MAX_LEN] = *b"        ";
    let mut str = format!("{v:.0$}", precision.clamp(0, 8) as usize);
    let mut dot_pos = str.find('.');

    str.retain(|c| c != '.');
    if str.len() > MAX_LEN {
        str.truncate(MAX_LEN);
    } else if str.len() < MAX_LEN {
        let pad_len = MAX_LEN - str.len();
        // pad string
        for _ in 0..pad_len {
            str.insert(0, ' ');
        }
        // move dot
        dot_pos.as_mut().map(|pos| {
            *pos += pad_len;
            pos
        });
    }

    for (i, c) in str.as_bytes().iter().enumerate() {
        buf[i] = *c;
    }

    let mut dots = 0;
    dot_pos.map(|pos| {
        if pos > 0 && pos < MAX_LEN {
            dots = 0b1000_0000 >> (pos - 1);
        } else {
            dots = 0b0000_0001;
        }
    });

    Some((buf, dots))
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

            // make sure to wake the display up
            display.power_on().unwrap();

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
                    Request::WriteFloat(addr, v, precision) => match float2str(v, precision) {
                        Some((digits, dots)) => {
                            display
                                .write_str(addr, &digits, dots)
                                .unwrap_or_else(|_| {});
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
                    Request::WriteString(addr, str, align, dots) => {
                        let mut str = str.clone();
                        str.truncate(8);
                        if str.len() < 8 {
                            let pad_len = 8 - str.len();

                            match align {
                                crate::max7219::hw_max7219_string_align::Left => {
                                    for _ in 0..pad_len {
                                        str.push(' ');
                                    }
                                }
                                crate::max7219::hw_max7219_string_align::Right => {
                                    for _ in 0..pad_len {
                                        str.insert(0, ' ');
                                    }
                                }
                                crate::max7219::hw_max7219_string_align::Center => {
                                    for idx in 0..pad_len {
                                        if idx % 2 == 0 {
                                            str.insert(0, ' ');
                                        } else {
                                            str.push(' ');
                                        }
                                    }
                                }
                            }
                        }

                        display
                            .write_raw(addr, &encode_string(&str, dots))
                            .unwrap_or_else(|_| {
                                error!("write str overflow");
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

    use crate::max7219::max7219_impl::float2str;

    #[test]
    fn to_str() {
        assert_eq!(float2str(0.0, 0), Some((*b"       0", 0b00000000)));
        assert_eq!(float2str(0.0, 1), Some((*b"      00", 0b00000010)));
        assert_eq!(float2str(0.0, 2), Some((*b"     000", 0b0000_0100)));
        assert_eq!(float2str(0.0, 3), Some((*b"    0000", 0b0000_1000)));
        assert_eq!(float2str(0.0, 4), Some((*b"   00000", 0b0001_0000)));
        assert_eq!(float2str(0.0, 5), Some((*b"  000000", 0b0010_0000)));
        assert_eq!(float2str(0.0, 6), Some((*b" 0000000", 0b0100_0000)));
        assert_eq!(float2str(0.0, 7), Some((*b"00000000", 0b1000_0000)));
        assert_eq!(float2str(0.0, 8), Some((*b"00000000", 0b1000_0000)));
        assert_eq!(float2str(1.23456789, 0), Some((*b"       1", 0b0000_0000)));
        assert_eq!(float2str(1.23456789, 1), Some((*b"      12", 0b0000_0010)));
        assert_eq!(float2str(1.23456789, 4), Some((*b"   12346", 0b0001_0000)));
        assert_eq!(float2str(200.5, 8), Some((*b"20050000", 0b0010_0000)));
        assert_eq!(float2str(123456789.5, 8), Some((*b"12345679", 0b0000_0001)));
        assert_eq!(float2str(-12.5, 2), Some((*b"   -1250", 0b0000_0100)));
    }
}
