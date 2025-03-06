use std::{collections::HashMap, ffi::CString};

use embedded_graphics::{
    mono_font::{
        iso_8859_5::FONT_6X10, iso_8859_5::FONT_6X12, iso_8859_5::FONT_6X13, iso_8859_5::FONT_7X13,
        MonoTextStyleBuilder,
    },
    pixelcolor::BinaryColor,
    prelude::Point,
    text::{Baseline, Text},
    Drawable,
};

use log::{debug, error};
use rppal::{gpio::Gpio, spi::Spi};
use ssd1306::{
    prelude::{DisplayConfig, DisplayRotation, SPIInterface},
    size::DisplaySize128x64,
    Ssd1306,
};

use crate::{hw_msg_cb, hw_notify_cb};

use super::{hw_display_ssd1306, Reply, Request};

fn send_error(tx: &std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb, msg: &str) {
    error!("{msg}");
    if let Err(err) = tx.send(Reply::Error(CString::new(msg).unwrap_or_default())) {
        error!("reply send error: {err}");
    } else {
        notify.notify();
    }
}

fn proc_err<E>(err: E, tx: &std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb) -> String
where
    E: std::fmt::Display,
{
    let str = err.to_string();
    send_error(tx, notify, str.as_str());
    str
}

impl hw_display_ssd1306 {
    pub fn new_spi(
        spi_bus: i8,
        dc_pin: u8,
        cs_pin: u8,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("thread started");

            // let i2c = I2c::new().map_err(|err| proc_err(err, &rep_tx, notify))?;
            // let i2c_iface = ssd1306::I2CDisplayInterface::new(i2c);

            let gpio = Gpio::new().map_err(|err| proc_err(err, &rep_tx, notify))?;
            let dc = gpio
                .get(dc_pin)
                .map_err(|err| proc_err(err, &rep_tx, notify))?
                .into_output();

            let cs = gpio
                .get(cs_pin)
                .map_err(|err| proc_err(err, &rep_tx, notify))?
                .into_output();

            debug!("GPIO init: DC=GPIO_{dc_pin:02} CS=GPIO_{cs_pin:02}");

            let bus = match spi_bus {
                0 => rppal::spi::Bus::Spi0,
                1 => rppal::spi::Bus::Spi1,
                2 => rppal::spi::Bus::Spi2,
                3 => rppal::spi::Bus::Spi3,
                4 => rppal::spi::Bus::Spi4,
                5 => rppal::spi::Bus::Spi5,
                6 => rppal::spi::Bus::Spi6,
                _ => rppal::spi::Bus::Spi0,
            };

            let spi = Spi::new(
                bus,
                rppal::spi::SlaveSelect::Ss0,
                1_000_000,
                rppal::spi::Mode::Mode0,
            )
            .map_err(|err| proc_err(err, &rep_tx, notify))?;

            debug!("SPI init: {spi:?}");

            let spi_iface = SPIInterface::new(spi, dc, cs);
            let mut display = Ssd1306::new(spi_iface, DisplaySize128x64, DisplayRotation::Rotate0)
                .into_buffered_graphics_mode();

            display
                .init()
                .map_err(|_| proc_err("display error", &rep_tx, notify))?;
            display.clear_buffer();

            let mut font_map = HashMap::new();
            font_map.insert("FONT6x10", &FONT_6X10);
            font_map.insert("FONT_6X12", &FONT_6X12);
            font_map.insert("FONT_6X13", &FONT_6X13);
            font_map.insert("FONT_7X13", &FONT_7X13);

            let mut text_style = MonoTextStyleBuilder::new()
                .font(&FONT_6X10)
                .text_color(BinaryColor::On)
                .build();

            while let Ok(req) = req_rx.recv() {
                debug!("{req:?}");
                
                match req {
                    Request::DrawText(cstr, x, y) => {
                        Text::with_baseline(
                            cstr.into_string().unwrap().as_str(),
                            Point::new(x as i32, y as i32),
                            text_style,
                            Baseline::Top,
                        )
                        .draw(&mut display)
                        .map_err(|_| {
                            proc_err("display error", &rep_tx, notify);
                        })
                        .unwrap_or_default();
                    }
                    Request::Clear(flush) => {
                        display.clear_buffer();

                        if flush {
                            display.flush().unwrap_or_else(|_| {
                                proc_err("display error", &rep_tx, notify);
                            });
                        }
                    }
                    Request::Flush => {
                        display.flush().unwrap_or_else(|_| {
                            proc_err("display error", &rep_tx, notify);
                        });
                    }
                    Request::SetFont(font) => {
                        let font = font.to_string_lossy();
                        match font_map.get(font.as_ref()) {
                            Some(ft) => {
                                text_style.font = *ft;
                            }
                            None => send_error(
                                &rep_tx,
                                notify,
                                format!("font not found: {font:?}").as_str(),
                            ),
                        }
                    }
                }
            }

            debug!("thread stopped");
            Ok(())
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
