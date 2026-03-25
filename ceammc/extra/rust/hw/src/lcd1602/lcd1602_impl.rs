use std::ffi::CString;

use log::{debug, error, warn};

use crate::{
    hw_msg_cb, hw_notify_cb,
    i2c::{i2c_impl::create_i2c_bus, I2cAddress},
    lcd1602::Reply,
    send_debug, send_error,
};

fn to_greek(ch: char) -> Option<char> {
    match ch {
        '\u{03B1}' => Some('\u{E0}'),
        '\u{03B2}' => Some('\u{E2}'),
        '\u{03B5}' => Some('\u{E3}'),
        '\u{03BC}' => Some('\u{E4}'),
        '\u{03C3}' => Some('\u{E5}'),
        '\u{03C1}' => Some('\u{E6}'),
        '\u{03F3}' => Some('\u{EA}'),
        '\u{03B8}' => Some('\u{F2}'),
        '\u{03A9}' => Some('\u{F4}'),
        '\u{03A3}' => Some('\u{F6}'),
        '\u{03C0}' => Some('\u{F7}'),
        _ => None,
    }
}

fn to_ascii(ch: char) -> Option<char> {
    match ch {
        '0'..='9'
        | 'a'..='z'
        | 'A'..='Z'
        | ' '
        | '!'
        | '"'
        | '#'
        | '$'
        | '%'
        | '&'
        | '\''
        | '('
        | ')'
        | '*'
        | '+'
        | ','
        | '-'
        | '.'
        | '/'
        | ':'
        | ';'
        | '<'
        | '='
        | '>'
        | '?'
        | '@'
        | '['
        | ']'
        | '^'
        | '_'
        | '`'
        | '{'
        | '|'
        | '}' => Some(ch),
        _ => None,
    }
}

fn encode_str(str: &CString) -> Vec<char> {
    let str = str.to_string_lossy().to_string();
    let mut res = vec![];
    for ch in str.chars() {
        match ch {
            '¥' => res.push('\u{5C}'),
            '→' | '￫' => res.push('\u{7E}'),
            '←' | '￩' => res.push('\u{7F}'),
            'ä' => res.push('\u{E1}'),
            '\u{221E}' => res.push('\u{F3}'),
            _ => match to_ascii(ch).or_else(|| to_greek(ch)) {
                Some(ch) => res.push(ch),
                None => match to_greek(ch) {
                    Some(ch) => res.push(ch),
                    None => warn!("character is not supported: '{ch}'"),
                },
            },
        }
    }
    res
}

use super::{hw_hd44780, Hd44780Worker, Request};

impl hw_hd44780 {
    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        rows: u8,
        on_msg: hw_msg_cb,
        notify: hw_notify_cb,
    ) -> Result<Self, CString> {
        // const LCD_ADDRESS: u8 = 0x27; // Address depends on hardware, see link below

        let (mut worker, rx, tx) = Hd44780Worker::new(on_msg, None);
        worker.spawn(tx.clone(), notify, move || -> Result<(), String> {
            let mut i2c = create_i2c_bus(i2c_bus)?;
            debug!("I2C init");

            let addrs: Vec<u8> = match i2c_addr {
                I2cAddress::Default => vec![0x27],
                I2cAddress::Alt => vec![0x3f],
                I2cAddress::Auto => vec![0x27, 0x3f],
                I2cAddress::Addr(addr) => vec![addr],
                I2cAddress::Invalid(x) => return Err(format!("invalid i2c address: {x}")),
            };

            let bus = i2c.bus();
            let rows = match rows {
                2 => 2,
                4 => 4,
                _ => 0,
            };

            let mut delay = rppal::hal::Delay::new();
            let mut lcd = None;

            for addr in &addrs {
                debug!("try LCD init with: bus={bus}, addr=0x{addr:02x}");

                lcd = lcd_lcm1602_i2c::sync_lcd::Lcd::new(&mut i2c, &mut delay)
                    .with_address(*addr)
                    .with_rows(rows)
                    .with_cursor_on(false)
                    .with_cursor_blink(false)
                    .init()
                    .ok();

                if lcd.is_some() {
                    send_debug(
                        &tx,
                        notify,
                        format!("connected to display: bus={bus} addr=0x{addr:02x} rows={rows}").as_str(),
                    )
                    .to_err()?;
                    break;
                }
            }

            if lcd.is_none() {
                let addr_lst = addrs.iter().map(|x| format!("0x{x:02x}")).collect::<Vec<_>>().join(" ");
                return Err(format!("can't connect to addresses: [{addr_lst}]"));
            }

            let mut lcd = lcd.unwrap();

            while let Ok(crate::WorkerCommand::Command(req)) = rx.recv() {
                debug!("{:?}", &req);

                use crate::lcd1602::Request;

                match &req {
                    Request::WriteText(msg) => {
                        let bytes: String = encode_str(msg).into_iter().collect();
                        lcd.write_str(bytes.as_ref())
                            .or_else(|e| send_error(&tx, notify, e.to_string().as_str()).to_err())?;
                    }
                    Request::Clear => {
                        lcd.clear()
                            .or_else(|e| send_error(&tx, notify, e.to_string().as_str()).to_err())?;
                    }
                    Request::Backlight(state) => {
                        lcd.backlight(match state {
                            true => lcd_lcm1602_i2c::Backlight::On,
                            false => lcd_lcm1602_i2c::Backlight::Off,
                        })
                        .or_else(|e| send_error(&tx, notify, e.to_string().as_str()).to_err())?;
                    }
                    Request::CursorOn(state) => {
                        lcd.cursor_on(*state)
                            .or_else(|e| send_error(&tx, notify, e.to_string().as_str()).to_err())?;
                    }
                    Request::CursorBlink(state) => {
                        lcd.cursor_blink(*state)
                            .or_else(|e| send_error(&tx, notify, e.to_string().as_str()).to_err())?;
                    }
                    Request::CursorPos(row, col) => {
                        let shift = match rows {
                            2 => *row * 0x40 + *col,
                            4 => ((0x1 & *row) * 0x40) + ((*row > 1) as u8 * 0x14) + *col,
                            _ => *col,
                        };

                        lcd.set_cursor(0, shift)
                            .or_else(|e| send_error(&tx, notify, e.to_string().as_str()).to_err())?;
                    }
                    Request::CursorMove(dir) => {
                        match dir {
                            a if *a < 0 => lcd.scroll_cursor_left().map_err(|e| e.to_string()),
                            b if *b > 0 => lcd.scroll_cursor_right().map_err(|e| e.to_string()),
                            _ => Err(format!("invalid direction value: {dir}")),
                        }
                        .or_else(|e| send_error(&tx, notify, e.as_str()).to_err())?;
                    }
                    Request::TextScroll(dir) => {
                        match dir {
                            a if *a < 0 => lcd.scroll_display_left().map_err(|e| e.to_string()),
                            b if *b > 0 => lcd.scroll_display_right().map_err(|e| e.to_string()),
                            _ => Err(format!("invalid direction value: {dir}")),
                        }
                        .or_else(|e| send_error(&tx, notify, e.as_str()).to_err())?;
                    }
                    Request::SetFont(font) => {
                        use crate::lcd1602::hw_hd44780_font;
                        lcd.font_mode(match font {
                            hw_hd44780_font::FONT_5x8 => lcd_lcm1602_i2c::Font::Font5x8,
                            hw_hd44780_font::FONT_5x10 => lcd_lcm1602_i2c::Font::Font5x10,
                        })
                        .or_else(|e| send_error(&tx, notify, e.to_string().as_str()).to_err())?;
                    }
                    Request::WriteChar(ch) => {
                        let ch = unsafe { char::from_u32_unchecked(*ch) };
                        lcd.write_str(ch.to_string().as_str())
                            .or_else(|e| send_error(&tx, notify, e.to_string().as_str()).to_err())?;
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
