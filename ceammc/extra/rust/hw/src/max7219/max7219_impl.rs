use std::ffi::CString;

use log::{debug, error};
use rppal::gpio::Gpio;

use crate::{hw_msg_cb, hw_notify_cb};

use super::{hw_max7219, Request};

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
                    Request::WriteInt(addr, value) => display.write_integer(addr, value).unwrap(),
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
