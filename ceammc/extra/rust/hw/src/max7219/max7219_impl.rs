use std::ffi::CString;

use log::{debug, error};
// use rpi_embedded::{gpio::Gpio, spi::Spi};
use rppal::{gpio::Gpio, spi::Spi};

use crate::{hw_msg_cb, hw_notify_cb};

use super::{hw_max7219, Request};

impl hw_max7219 {
    pub fn new(addr: u8, notify: hw_notify_cb, on_err: hw_msg_cb) -> Result<Self, CString> {
        let (tx, rx) = std::sync::mpsc::channel::<Request>();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("worker thread start");

            let spi = Spi::new(
                rppal::spi::Bus::Spi0,
                rppal::spi::SlaveSelect::Ss0,
                320000000,
                rppal::spi::Mode::Mode0,
            )
            .unwrap();

            let gpio = Gpio::new().unwrap();

            let sck = gpio.get(11).unwrap().into_output_low();
            let cs = gpio.get(8).unwrap().into_output_low();
            let mosi = gpio.get(10).unwrap().into_output_low();

            // MISO: BCM GPIO 9 (physical pin 21)
            // MOSI: BCM GPIO 10 (physical pin 19)
            // SCLK: BCM GPIO 11 (physical pin 23)
            // SS: Ss0 BCM GPIO 8 (physical pin 24), Ss1 BCM GPIO 7 (physical pin 26)

            let mut display = max7219::MAX7219::from_pins(4, mosi, cs, sck).unwrap();

            // make sure to wake the display up
            display.power_on().unwrap();
            display.write_str(0, b"12345678", 0).unwrap();

            while let Ok(req) = rx.recv() {
                debug!("{req:?}");

                match req {
                    Request::Intensity(addr, val) => {
                        display.set_intensity(addr, val).unwrap();
                    }
                    Request::WriteString(addr, msg) => {
                        // write given octet of ASCII characters with dots specified by 3rd param bits
                        let str = msg.as_bytes();
                        display.write_str(addr, b"________", 0).unwrap();
                    }
                    Request::PowerOn(state) => {
                        if state {
                            display.power_on().unwrap();
                        } else {
                            display.power_off().unwrap();
                        }
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
