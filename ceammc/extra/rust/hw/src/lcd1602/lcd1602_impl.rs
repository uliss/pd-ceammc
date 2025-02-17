use std::ffi::CString;

use log::{debug, error};

use crate::{hc_sr04::hw_sr04_cb, hw_msg_cb, hw_notify_cb};

use super::{hw_lcd1602, Request};

impl hw_lcd1602 {
    pub fn new(
        addr: Option<u8>,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
        on_data: hw_sr04_cb,
    ) -> Result<Self, CString> {
        const LCD_ADDRESS: u8 = 0x27; // Address depends on hardware, see link below

        // Create a I2C instance, needs to implement embedded_hal::blocking::i2c::Write, this
        // particular uses the arduino_hal crate for avr microcontrollers like the arduinos.
        // let dp = rppal::hal::Peripherals::take().unwrap();
        // let pins = arduino_hal::pins!(dp);
        let mut i2c = rppal::i2c::I2c::new().unwrap();
        //     dp.TWI, //
        //     pins.a4.into_pull_up_input(), // use respective pins
        //     pins.a5.into_pull_up_input(),
        //     50000,
        // );

        let mut delay = rppal::hal::Delay::new();
        let (tx, rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || {
            debug!("worker thread start");

            let mut lcd = lcd_lcm1602_i2c::sync_lcd::Lcd::new(&mut i2c, &mut delay)
                .with_address(addr.unwrap_or(LCD_ADDRESS))
                .with_rows(2)
                .with_cursor_on(true)
                .init()
                .unwrap();

            while let Ok(req) = rx.recv() {
                match req {
                    crate::lcd1602::Request::WriteText(msg) => {
                        lcd.write_str(msg.as_str()).unwrap_or_else(|e| {
                            error!("{e}");
                        });
                    }
                    crate::lcd1602::Request::Clear => {
                        lcd.clear().unwrap_or_else(|e| error!("{e}"));
                    }
                    crate::lcd1602::Request::Backlight(state) => {
                        lcd.backlight(match state {
                            true => lcd_lcm1602_i2c::Backlight::On,
                            false => lcd_lcm1602_i2c::Backlight::Off,
                        })
                        .unwrap();
                    }
                }
            }

            debug!("worker thread done");
        });

        Ok(hw_lcd1602 {
            tx,
            on_err,
            on_data,
        })
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

mod test {}
