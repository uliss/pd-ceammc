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
    prelude::{Brightness, DisplayConfig, DisplayRotation, SPIInterfaceNoCS, WriteOnlyDataCommand},
    size::DisplaySize,
    Ssd1306,
};

use crate::{
    hw_notify_cb,
    i2c::{i2c_impl::create_i2c_bus, I2cAddress},
    process_err, send_error,
    spi::spi_impl::{i8_to_slave_select, i8_to_spi_bus},
};

use super::{hw_display_ssd1306, DisplayI2cArgs, DisplaySpiArgs, Reply, Request, Ssd1306Worker};

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
        font_map.insert("FONT6X10", &FONT_6X10);
        font_map.insert("FONT_6X12", &FONT_6X12);
        font_map.insert("FONT_6X13", &FONT_6X13);
        font_map.insert("FONT_7X13", &FONT_7X13);

        let mut text_style = MonoTextStyleBuilder::new()
            .font(&FONT_6X10)
            .text_color(BinaryColor::On)
            .build();

        while let Ok(req) = rx.recv() {
            debug!("{req:?}");

            match req {
                Request::Clear(flush) => {
                    display.clear_buffer();

                    if flush {
                        display.flush().unwrap_or_else(|err| {
                            process_err(format!("{err:?}"), tx, notify);
                        });
                    }
                }
                Request::Flush => {
                    display.flush().unwrap_or_else(|err| {
                        process_err(format!("{err:?}"), tx, notify);
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
                    .map_err(|err| {
                        process_err(format!("{err:?}"), tx, notify);
                    })
                    .unwrap_or_default();
                }
                Request::SetFont(font) => {
                    let font = font.to_string_lossy();
                    match font_map.get(font.to_uppercase().as_str()) {
                        Some(ft) => {
                            text_style.font = *ft;
                        }
                        None => {
                            let keys = font_map
                                .keys()
                                .map(|k| k.to_string())
                                .collect::<Vec<_>>()
                                .join(", ");

                            send_error(
                                tx,
                                notify,
                                format!("font not found: {font:?}. supported fonts are: {keys}")
                                    .as_str(),
                            );
                        }
                    }
                }
                Request::Invert(state) => {
                    display.set_invert(state).unwrap_or_else(|err| {
                        process_err(format!("{err:?}"), tx, notify);
                    });
                }
                Request::Mirror(state) => {
                    display.set_mirror(state).unwrap_or_else(|err| {
                        process_err(format!("{err:?}"), tx, notify);
                    });
                }
                Request::SwitchOn(state) => {
                    display.set_display_on(state).unwrap_or_else(|err| {
                        process_err(format!("{err:?}"), tx, notify);
                    });
                }
                Request::SetPixel(x, y, value) => {
                    display.set_pixel(x as u32, y as u32, value);
                }
                Request::SetData(data) => {
                    display.draw(data.as_slice()).unwrap_or_else(|err| {
                        process_err(format!("{err:?}"), tx, notify);
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
                    image.draw(display).unwrap_or_else(|err| {
                        process_err(format!("{err:?}"), tx, notify);
                    });
                }
                Request::SetRotation(rotate) => {
                    display
                        .set_rotation(match rotate {
                            crate::display::hw_display_rotation::ROTATE_0 => {
                                DisplayRotation::Rotate0
                            }
                            crate::display::hw_display_rotation::ROTATE_90 => {
                                DisplayRotation::Rotate90
                            }
                            crate::display::hw_display_rotation::ROTATE_180 => {
                                DisplayRotation::Rotate180
                            }
                            crate::display::hw_display_rotation::ROTATE_270 => {
                                DisplayRotation::Rotate270
                            }
                        })
                        .unwrap_or_else(|err| {
                            process_err(format!("{err:?}"), tx, notify);
                        });
                }
                Request::SetBrightness(level) => {
                    display
                        .set_brightness(match level {
                            0 => Brightness::DIMMEST,
                            1 => Brightness::DIM,
                            2 => Brightness::NORMAL,
                            3 => Brightness::BRIGHT,
                            4 => Brightness::BRIGHTEST,
                            _ => Brightness::NORMAL,
                        })
                        .unwrap_or_else(|err| {
                            process_err(format!("{err:?}"), tx, notify);
                        });
                }
            }
        }
    }

    pub fn new_i2c<SIZE: DisplaySize + Send + 'static>(
        args: DisplayI2cArgs,
        size: SIZE,
    ) -> Result<Self, CString> {
        let (worker, rx, tx) = Ssd1306Worker::new(args.on_err);

        worker.spawn(tx.clone(), args.notify, move || -> Result<(), String> {
            let i2c = create_i2c_bus(args.i2c_bus, &tx, args.notify)?;
            debug!("I2C init: {i2c:?}");

            let i2c_iface = match args.i2c_addr {
                I2cAddress::Default => ssd1306::I2CDisplayInterface::new(i2c),
                I2cAddress::Alt => ssd1306::I2CDisplayInterface::new_alternate_address(i2c),
                I2cAddress::Invalid(addr) => {
                    return Err(format!("invalid i2c address: {addr}"));
                }
                I2cAddress::Addr(addr) => {
                    ssd1306::I2CDisplayInterface::new_custom_address(i2c, addr)
                }
            };

            let mut display = Ssd1306::new(i2c_iface, size, DisplayRotation::Rotate0)
                .into_buffered_graphics_mode();

            display
                .init()
                .map_err(|err| process_err(format!("{err:?}"), &tx, args.notify))?;
            display.clear_buffer();
            display.flush().unwrap_or_default();

            Self::process_loop(&mut display, &tx, &rx, args.notify);
            Ok(())
        });

        Ok(Self { worker })
    }

    pub fn new_spi<SIZE: DisplaySize + Send + 'static>(
        args: DisplaySpiArgs,
        size: SIZE,
    ) -> Result<Self, CString> {
        let (worker, rx, tx) = Ssd1306Worker::new(args.on_err);

        worker.spawn(tx.clone(), args.notify, move || -> Result<(), String> {
            let gpio = Gpio::new().map_err(|err| process_err(err, &tx, args.notify))?;
            let dc = gpio
                .get(args.dc_pin)
                .map_err(|err| process_err(err, &tx, args.notify))?
                .into_output_low();

            let mut rst = gpio
                .get(args.rs_pin)
                .map_err(|err| process_err(err, &tx, args.notify))?
                .into_output_low();

            debug!("GPIO init");

            let bus = i8_to_spi_bus(args.spi_bus);
            let cs = i8_to_slave_select(args.cs_pin)?;

            let spi = Spi::new(bus, cs, args.freq, rppal::spi::Mode::Mode0).map_err(|err| {
                process_err(
                    format!(
                        "SPI init error: {err}, bus={bus}, cs={cs}, freq={}",
                        args.freq
                    ),
                    &tx,
                    args.notify,
                )
            })?;

            debug!("SPI init: {spi:?} freq={} cs={cs}", args.freq);

            let spi_iface = SPIInterfaceNoCS::new(spi, dc);

            let mut display = Ssd1306::new(spi_iface, size, DisplayRotation::Rotate0)
                .into_buffered_graphics_mode();

            debug!(
                "display init: DC=GPIO_{:02} RST=GPIO_{:02} size={:?}",
                args.dc_pin,
                args.rs_pin,
                display.dimensions()
            );

            display
                .reset(&mut rst, &mut rppal::hal::Delay::default())
                .map_err(|err| process_err(format!("display error: {err:?}"), &tx, args.notify))?;

            display
                .init()
                .map_err(|err| process_err(format!("display error: {err:?}"), &tx, args.notify))?;

            display.clear_buffer();

            Self::process_loop(&mut display, &tx, &rx, args.notify);

            Ok(())
        });

        Ok(Self { worker })
    }

    pub fn process_reply(display: *const Self) -> bool {
        if display.is_null() {
            error!("NULL display pointer");
            false
        } else {
            let display = unsafe { &*display };

            display.worker.process_reply(&|rep| match rep {
                super::Reply::Message(level, cstr) => display.worker.pd_message(&cstr),
            })
        }
    }

    pub fn send_request(display: *const Self, req: Request) -> bool {
        if display.is_null() {
            error!("NULL display pointer");
            false
        } else {
            let display = unsafe { &*display };
            display.worker.send_request(req)
        }
    }
}
