use std::ffi::CString;

use embedded_graphics::{
    mono_font::{ascii::FONT_6X10, MonoTextStyleBuilder},
    pixelcolor::BinaryColor,
    prelude::{PixelIteratorExt, Point},
    text::{Baseline, Text},
    Drawable,
};
use log::{debug, error};
use rppal::{gpio::Gpio, i2c::I2c, spi::Spi};
use ssd1306::{prelude::DisplayRotation, size::DisplaySize128x64, I2CDisplayInterface, Ssd1306};

use crate::{hw_msg_cb, hw_notify_cb};

use super::{hw_display_ssd1306, Request};

impl hw_display_ssd1306 {
    pub fn new(addr: i8, notify: hw_notify_cb, on_err: hw_msg_cb) -> Result<Self, CString> {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || {
            debug!("thread started");

            let i2c = I2c::new().unwrap();
            let i2c_iface = I2CDisplayInterface::new(i2c);

            // let gpio = Gpio::new().unwrap();
            // let dc = gpio.get(16).unwrap().into_output();
            // let spi = Spi::new(
            //     rppal::spi::Bus::Spi0,
            //     rppal::spi::SlaveSelect::Ss0,
            //     1_000_000,
            //     rppal::spi::Mode::Mode0,
            // )
            // .unwrap();

            // let spi_iface = SPIInterface::new(spi, dc);
            // spi_iface.
            let mut display = Ssd1306::new(i2c_iface, DisplaySize128x64, DisplayRotation::Rotate0)
                .into_buffered_graphics_mode();

            // display.into_buffered_graphics_mode().reset(rst, delay)

            // display.reset(rst, delay)

            // Spam some characters to the display
            // for c in 97..123 {
            //     let _ = display.write_str(unsafe { core::str::from_utf8_unchecked(&[c]) });
            // }
            // for c in 65..91 {
            //     let _ = display.write_str(unsafe { core::str::from_utf8_unchecked(&[c]) });
            // }

            //
            // display.
            // display.init().unwrap();
            // display.clear_buffer();
            // display.clear().unwrap();

            let text_style = MonoTextStyleBuilder::new()
                .font(&FONT_6X10)
                .text_color(BinaryColor::On)
                .build();

            // Text::with_baseline("Hello world!", Point::zero(), text_style, Baseline::Top)
            //     .draw(&mut display)
            //     .unwrap();

            // Text::with_baseline("Hello Rust!", Point::new(0, 16), text_style, Baseline::Top)
            //     .draw(&mut display)
            //     .unwrap();

            // display.flush().unwrap();

            // display.set_display_on(true);
            // display.
            // display.flush().unwrap();

            while let Ok(req) = req_rx.recv() {
                match req {
                    Request::DrawText(cstr, x, y) => {
                        display.clear_buffer();

                        Text::with_baseline(
                            cstr.into_string().unwrap().as_str(),
                            Point::new(x as i32, y as i32),
                            text_style,
                            Baseline::Top,
                        )
                        .draw(&mut display)
                        .unwrap();

                        display.flush().unwrap();
                    }
                }
            }

            debug!("thread stopped");
        });

        Ok(Self {
            tx: req_tx,
            rx: rep_rx,
            on_err,
        })
    }

    pub fn process_reply(display: *const Self) -> bool {
        if display.is_null() {
            error!("NULL display pointer");
            false
        } else {
            let display = unsafe { &*display };
            while let Ok(rep) = display.rx.try_recv() {
                match rep {
                    super::Reply::Error(cstr) => {
                        display.on_err.exec_raw(cstr.as_ptr());
                    }
                }
            }

            true
        }
    }

    pub fn send_request(display: *const Self, req: Request) -> bool {
        if display.is_null() {
            error!("NULL display pointer");
            false
        } else {
            let display = unsafe { &*display };
            if let Err(err) = display.tx.send(req) {
                error!("send request error: {err}");
                display.on_err.exec(err.to_string().as_str());
                false
            } else {
                true
            }
        }
    }
}
