use std::ffi::CString;

use log::{debug, error};

use crate::{
    hw_msg_cb, hw_notify_cb,
    i2c::{i2c_impl::create_i2c_bus, I2cAddress},
    lcd1602::Reply,
    send_debug, send_error,
};

use super::{hw_lcd1602, Lcd1602Worker, Request};

impl hw_lcd1602 {
    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        rows: u8,
        on_msg: hw_msg_cb,
        notify: hw_notify_cb,
    ) -> Result<Self, CString> {
        // const LCD_ADDRESS: u8 = 0x27; // Address depends on hardware, see link below

        let (worker, rx, tx) = Lcd1602Worker::new(on_msg);
        worker.spawn(tx.clone(), notify, move || {
            let mut i2c = create_i2c_bus(i2c_bus, &tx, notify)?;
            debug!("I2C init");

            let addr = match i2c_addr {
                I2cAddress::Default => 0x27,
                I2cAddress::Alt => 0x3f,
                I2cAddress::Invalid(x) => return Err(format!("invalid i2c address: {x}")),
                I2cAddress::Addr(addr) => addr,
            };

            let bus = i2c.bus();
            debug!("try LCD init with: bus={bus}, addr={addr}");

            let rows = match rows {
                2 => 2,
                4 => 4,
                _ => 0,
            };

            let mut delay = rppal::hal::Delay::new();
            let mut lcd = lcd_lcm1602_i2c::sync_lcd::Lcd::new(&mut i2c, &mut delay)
                .with_address(addr)
                .with_rows(rows)
                .with_cursor_on(false)
                .with_cursor_blink(false)
                .init()
                .map_err(|err| format!("LCD init error: {err}"))?;

            send_debug(
                &tx,
                notify,
                format!("connected to display: bus={bus} addr={addr} rows={rows}").as_str(),
            );

            while let Ok(req) = rx.recv() {
                debug!("{:?}", &req);

                use crate::lcd1602::Request;

                match &req {
                    Request::WriteText(msg) => {
                        lcd.write_str(msg.to_string_lossy().as_ref()).unwrap_or_else(|e| {
                            send_error(&tx, notify, e.to_string().as_str());
                        });
                    }
                    Request::Clear => {
                        lcd.clear().unwrap_or_else(|e| {
                            send_error(&tx, notify, e.to_string().as_str());
                        });
                    }
                    Request::Backlight(state) => {
                        lcd.backlight(match state {
                            true => lcd_lcm1602_i2c::Backlight::On,
                            false => lcd_lcm1602_i2c::Backlight::Off,
                        })
                        .unwrap_or_else(|e| {
                            send_error(&tx, notify, e.to_string().as_str());
                        });
                    }
                    Request::CursorOn(state) => {
                        lcd.cursor_on(*state).unwrap_or_else(|e| {
                            send_error(&tx, notify, e.to_string().as_str());
                        });
                    }
                    Request::CursorBlink(state) => {
                        lcd.cursor_blink(*state).unwrap_or_else(|e| {
                            send_error(&tx, notify, e.to_string().as_str());
                        });
                    }
                    Request::CursorPos(row, col) => {
                        let shift = match rows {
                            2 => *row * 0x40 + *col,
                            4 => (*row * 10) + ((*row & 0x1) + 30) + *col,
                            _ => *col,
                        };

                        lcd.set_cursor(0, shift).unwrap_or_else(|e| {
                            send_error(&tx, notify, e.to_string().as_str());
                        });
                    }
                    Request::CursorMove(dir) => {
                        match dir {
                            a if *a < 0 => lcd.scroll_cursor_left().map_err(|e| e.to_string()),
                            b if *b > 0 => lcd.scroll_cursor_right().map_err(|e| e.to_string()),
                            _ => Err(format!("invalid direction value: {dir}")),
                        }
                        .unwrap_or_else(|e| {
                            send_error(&tx, notify, e.as_str());
                        });
                    }
                    Request::TextScroll(dir) => {
                        match dir {
                            a if *a < 0 => lcd.scroll_display_left().map_err(|e| e.to_string()),
                            b if *b > 0 => lcd.scroll_display_right().map_err(|e| e.to_string()),
                            _ => Err(format!("invalid direction value: {dir}")),
                        }
                        .unwrap_or_else(|e| {
                            send_error(&tx, notify, e.as_str());
                        });
                    }
                    Request::SetFont(font) => {
                        use crate::lcd1602::hw_hd44780_font;
                        lcd.font_mode(match font {
                            hw_hd44780_font::FONT_5x8 => lcd_lcm1602_i2c::Font::Font5x8,
                            hw_hd44780_font::FONT_5x10 => lcd_lcm1602_i2c::Font::Font5x10,
                        })
                        .unwrap_or_else(|e| {
                            send_error(&tx, notify, e.to_string().as_str());
                        });
                    }
                }
            }

            //
            Ok(())
        });

        Ok(Self { worker })
    }

    pub fn send_request_ptr(lcd: *const Self, req: Request) -> bool {
        if lcd.is_null() {
            error!("NULL lcd pointer");
            false
        } else {
            let lcd = unsafe { &*lcd };
            lcd.worker.send_request(req)
        }
    }

    pub fn process_reply_ptr(lcd: *const Self) -> bool {
        if lcd.is_null() {
            error!("NULL lcd pointer");
            false
        } else {
            let lcd = unsafe { &*lcd };
            lcd.worker.process_reply(&|rep| match rep {
                Reply::Message(level, msg) => lcd.worker.pd_message(level, &msg),
            })
        }
    }
}
