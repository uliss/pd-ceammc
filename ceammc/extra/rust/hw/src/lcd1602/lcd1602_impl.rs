use std::ffi::CString;

use log::{debug, error};

use crate::{hw_msg_cb, hw_notify_cb, lcd1602::Reply};

use super::{hw_lcd1602, Request};

impl hw_lcd1602 {
    pub fn new(
        rows: u8,
        addr: Option<u8>,
        on_err: hw_msg_cb,
        notify: hw_notify_cb,
    ) -> Result<Self, CString> {
        const LCD_ADDRESS: u8 = 0x27; // Address depends on hardware, see link below

        let (tx, rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel::<Reply>();

        std::thread::spawn(move || {
            debug!("worker thread start");

            let i2c = rppal::i2c::I2c::new().map_err(|err| {
                error!("{err}");
                rep_tx
                    .send(Reply::Error(
                        CString::new(err.to_string()).unwrap_or_default(),
                    ))
                    .map(|_| notify.notify())
                    .unwrap_or_else(|err| {
                        error!("{err}");
                    });
            });

            if i2c.is_err() {
                return;
            }

            let mut i2c = i2c.unwrap();

            let mut delay = rppal::hal::Delay::new();
            let addr = addr.unwrap_or(LCD_ADDRESS);

            let lcd = lcd_lcm1602_i2c::sync_lcd::Lcd::new(&mut i2c, &mut delay)
                .with_address(addr)
                .with_rows(rows)
                .with_cursor_on(false)
                .with_cursor_blink(false)
                .init();

            match lcd {
                Ok(mut lcd) => {
                    debug!("I2c: {addr}");

                    while let Ok(req) = rx.recv() {
                        debug!("{:?}", &req);

                        match &req {
                            crate::lcd1602::Request::WriteText(msg) => {
                                lcd.write_str(msg.as_str()).unwrap_or_else(|e| {
                                    error!("{e}");
                                });
                            }
                            crate::lcd1602::Request::Clear => {
                                lcd.clear().unwrap_or_else(|e| error!("{e}"));
                            }
                            crate::lcd1602::Request::Backlight(state) => {
                                let _ = lcd.backlight(match state {
                                    true => lcd_lcm1602_i2c::Backlight::On,
                                    false => lcd_lcm1602_i2c::Backlight::Off,
                                });
                            }
                            Request::CursorOn(state) => {
                                let _ = lcd.cursor_on(*state);
                            }
                            Request::CursorBlink(state) => {
                                let _ = lcd.cursor_blink(*state);
                            }
                            Request::CursorPos(row, col) => {
                                let _ = lcd.set_cursor(*row, *col);
                            }
                            Request::CursorMove(dir) => {
                                match dir {
                                    a if *a < 0 => {
                                        let _ = lcd.scroll_cursor_left();
                                    }
                                    b if *b > 0 => {
                                        let _ = lcd.scroll_cursor_right();
                                    }
                                    _ => {}
                                };
                            }
                            Request::TextScroll(dir) => {
                                match dir {
                                    a if *a < 0 => {
                                        let _ = lcd.scroll_display_left();
                                    }
                                    b if *b > 0 => {
                                        let _ = lcd.scroll_display_right();
                                    }
                                    _ => {}
                                };
                            }
                        }
                    }
                }
                Err(err) => {
                    rep_tx
                        .send(Reply::Error(
                            CString::new(err.to_string()).unwrap_or_default(),
                        ))
                        .map(|_| notify.notify())
                        .unwrap_or_else(|e| error!("{e}"));
                }
            }

            debug!("worker thread done");
        });

        Ok(hw_lcd1602 {
            rx: rep_rx,
            tx,
            on_err,
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

    pub fn process_reply(&self) {
        while let Ok(reply) = self.rx.try_recv() {
            match reply {
                Reply::Error(cstr) => {
                    self.on_err.exec_raw(cstr.as_ptr());
                }
            }
        }
    }
}
