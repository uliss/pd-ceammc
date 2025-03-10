use std::{collections::HashMap, ffi::CString};

use embedded_graphics::{
    image::{Image, ImageRaw},
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
    mode::BufferedGraphicsMode,
    prelude::{DisplayConfig, DisplayRotation, SPIInterfaceNoCS, WriteOnlyDataCommand},
    size::{DisplaySize, DisplaySize128x64},
    Ssd1306,
};

use crate::{
    hw_msg_cb, hw_notify_cb,
    i2c::{i2c_impl::create_i2c_bus, I2cAddress},
};

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
    fn process_loop<DI, SIZE>(
        display: &mut Ssd1306<DI, SIZE, BufferedGraphicsMode<SIZE>>,
        tx: &std::sync::mpsc::Sender<Reply>,
        rx: &std::sync::mpsc::Receiver<Request>,
        notify: hw_notify_cb,
    ) where
        DI: WriteOnlyDataCommand,
        SIZE: DisplaySize,
    {
        let mut font_map = HashMap::new();
        font_map.insert("FONT6x10", &FONT_6X10);
        font_map.insert("FONT_6X12", &FONT_6X12);
        font_map.insert("FONT_6X13", &FONT_6X13);
        font_map.insert("FONT_7X13", &FONT_7X13);

        let mut text_style = MonoTextStyleBuilder::new()
            .font(&FONT_6X10)
            .text_color(BinaryColor::On)
            .build();

        while let Ok(req) = rx.recv() {
            match req {
                Request::Clear(flush) => {
                    display.clear_buffer();

                    if flush {
                        display.flush().unwrap_or_else(|_| {
                            proc_err("display error", tx, notify);
                        });
                    }
                }
                Request::Flush => {
                    display.flush().unwrap_or_else(|_| {
                        proc_err("display error", tx, notify);
                    });
                }
                Request::DrawText(cstr, x, y) => {
                    Text::with_baseline(
                        cstr.into_string().unwrap().as_str(),
                        Point::new(x as i32, y as i32),
                        text_style,
                        Baseline::Top,
                    )
                    .draw(display)
                    .map_err(|_| {
                        proc_err("display error", tx, notify);
                    })
                    .unwrap_or_default();
                }
                Request::SetFont(font) => {
                    let font = font.to_string_lossy();
                    match font_map.get(font.as_ref()) {
                        Some(ft) => {
                            text_style.font = *ft;
                        }
                        None => {
                            send_error(tx, notify, format!("font not found: {font:?}").as_str())
                        }
                    }
                }
                Request::Invert(state) => {
                    display.set_invert(state).unwrap_or_else(|_| {
                        proc_err("display error", tx, notify);
                    });
                }
                Request::Mirror(state) => {
                    display.set_mirror(state).unwrap_or_else(|_| {
                        proc_err("display error", tx, notify);
                    });
                }
                Request::SwitchOn(state) => {
                    display.set_display_on(state).unwrap_or_else(|_| {
                        proc_err("display error", tx, notify);
                    });
                }
                Request::SetPixel(x, y, value) => {
                    display.set_pixel(x as u32, y as u32, value);
                }
                Request::SetData(data) => {
                    display.draw(data.as_slice()).unwrap_or_else(|_| {
                        proc_err("display error", tx, notify);
                    });
                }
                Request::DrawBitmap(x, y, w, data) => {
                    let raw_image = ImageRaw::<BinaryColor>::new(data.as_slice(), w.into());
                    let image = Image::new(
                        &raw_image,
                        Point {
                            x: x.into(),
                            y: y.into(),
                        },
                    );
                    image.draw(display).unwrap_or_else(|_| {
                        proc_err("display error", tx, notify);
                    });
                }
            }
        }
    }

    pub fn new_i2c(
        bus: i8,
        addr: I2cAddress,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("thread started");

            let i2c = create_i2c_bus(bus, &rep_tx, notify)?;
            debug!("I2C init: {i2c:?}");

            let i2c_iface = match addr {
                I2cAddress::Default => ssd1306::I2CDisplayInterface::new(i2c),
                I2cAddress::Alt => ssd1306::I2CDisplayInterface::new_alternate_address(i2c),
                I2cAddress::Invalid(addr) => {
                    return Err(format!("invalid i2c address: {addr}"));
                }
                I2cAddress::Addr(addr) => {
                    ssd1306::I2CDisplayInterface::new_custom_address(i2c, addr)
                }
            };

            let mut display = Ssd1306::new(i2c_iface, DisplaySize128x64, DisplayRotation::Rotate0)
                .into_buffered_graphics_mode();

            display
                .init()
                .map_err(|_| proc_err("display error", &rep_tx, notify))?;
            display.clear_buffer();
            display.flush().unwrap_or_default();

            Self::process_loop(&mut display, &rep_tx, &req_rx, notify);

            debug!("thread done");
            Ok(())
        });

        Ok(Self {
            tx: req_tx,
            rx: rep_rx,
            on_err,
        })
    }

    pub fn new_spi(
        spi_bus: i8,
        dc_pin: u8,
        cs_pin: u8,
        freq: u32,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("thread started");

            let gpio = Gpio::new().map_err(|err| proc_err(err, &rep_tx, notify))?;
            let dc = gpio
                .get(dc_pin)
                .map_err(|err| proc_err(err, &rep_tx, notify))?
                .into_output_low();

            debug!("GPIO init: DC=GPIO_{dc_pin:02}");

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

            let cs = match cs_pin {
                0 => rppal::spi::SlaveSelect::Ss0,
                1 => rppal::spi::SlaveSelect::Ss1,
                2 => rppal::spi::SlaveSelect::Ss2,
                3 => rppal::spi::SlaveSelect::Ss3,
                4 => rppal::spi::SlaveSelect::Ss4,
                5 => rppal::spi::SlaveSelect::Ss5,
                6 => rppal::spi::SlaveSelect::Ss6,
                7 => rppal::spi::SlaveSelect::Ss7,
                8 => rppal::spi::SlaveSelect::Ss8,
                9 => rppal::spi::SlaveSelect::Ss9,
                10 => rppal::spi::SlaveSelect::Ss10,
                11 => rppal::spi::SlaveSelect::Ss11,
                12 => rppal::spi::SlaveSelect::Ss12,
                13 => rppal::spi::SlaveSelect::Ss13,
                14 => rppal::spi::SlaveSelect::Ss14,
                15 => rppal::spi::SlaveSelect::Ss15,
                _ => {
                    let msg = format!("invalid CS value: {cs_pin}");
                    send_error(&rep_tx, notify, msg.as_str());
                    return Err(msg);
                }
            };

            let spi = Spi::new(bus, cs, freq, rppal::spi::Mode::Mode0)
                .map_err(|err| proc_err(err, &rep_tx, notify))?;

            debug!("SPI init: {spi:?} freq={freq} cs={cs}");

            let spi_iface = SPIInterfaceNoCS::new(spi, dc);
            let mut display = Ssd1306::new(spi_iface, DisplaySize128x64, DisplayRotation::Rotate0)
                .into_buffered_graphics_mode();

            display
                .init()
                .map_err(|_| proc_err("display error", &rep_tx, notify))?;
            display.clear_buffer();

            Self::process_loop(&mut display, &rep_tx, &req_rx, notify);

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
