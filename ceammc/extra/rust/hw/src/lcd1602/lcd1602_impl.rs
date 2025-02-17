use std::ffi::CString;

use log::{debug, error};

use crate::{hw_msg_cb, hw_notify_cb};

use super::{hw_lcd1602, Request};

impl hw_lcd1602 {
    pub fn new(addr: Option<u8>, notify: hw_notify_cb, on_err: hw_msg_cb) -> Result<Self, CString> {
        const LCD_ADDRESS: u8 = 0x27; // Address depends on hardware, see link below

        let (tx, rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || {
            debug!("worker thread start");

            let mut i2c = rppal::i2c::I2c::new().unwrap();

            let mut delay = rppal::hal::Delay::new();

            let mut lcd = lcd_lcm1602_i2c::sync_lcd::Lcd::new(&mut i2c, &mut delay)
                .with_address(addr.unwrap_or(LCD_ADDRESS))
                .with_rows(2)
                .with_cursor_on(true)
                .with_cursor_blink(true)
                .init()
                .unwrap();

            debug!("I2c: {}", addr.unwrap_or(LCD_ADDRESS));

            while let Ok(req) = rx.recv() {
                debug!("{:?}", &req);
                match &req {
                    crate::lcd1602::Request::WriteText(msg) => {
                        // lcd.return_home().unwrap();
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
                    Request::CursorOn(state) => {
                        lcd.cursor_on(*state).unwrap();
                    }
                    Request::CursorBlink(state) => {
                        lcd.cursor_blink(*state).unwrap();
                    }
                    Request::CursorPos(row, col) => {
                        lcd.set_cursor(*row, *col).unwrap();
                    }
                }
            }

            debug!("worker thread done");
        });

        Ok(hw_lcd1602 { tx, on_err })
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

mod test {} // true
