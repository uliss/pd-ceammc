#![allow(non_camel_case_types)]
use std::ffi::{c_char, c_void};
use std::{ffi::CString, ptr::null_mut};

use embedded_graphics::mono_font::ascii::FONT_6X10;
use embedded_graphics::mono_font::MonoTextStyle;
use embedded_graphics::text::Text;
use embedded_graphics::Drawable;
use embedded_graphics::{
    pixelcolor::BinaryColor,
    prelude::{DrawTarget, OriginDimensions, Point, Size},
    Pixel,
};
use log::{debug, error};
use ndarray::Array2;

use crate::{core_notify, core_on_msg, cstr_to_string};

#[derive(Debug)]
pub enum Request {
    Fill(bool),
    Clear,
    Invert,
    DrawPixel(u16, u16),
    DrawText(String, i16, i16),
    GetData,
}

#[derive(Debug)]
pub enum Reply {
    Data(Vec<u8>),
}

pub struct core_async_bitmap {
    tx: std::sync::mpsc::Sender<Request>,
    rx: std::sync::mpsc::Receiver<Reply>,
    on_data: core_bitmap_on_data,
}

impl core_async_bitmap {
    fn send_request(bitmap: *mut core_async_bitmap, req: Request) -> bool {
        if bitmap.is_null() {
            return false;
        }

        let bitmap = unsafe { &*bitmap };
        bitmap
            .tx
            .send(req)
            .map_err(|err| {
                error!("send request error: {err}");
            })
            .is_ok()
    }
}

#[repr(C)]
pub struct core_bitmap_on_data {
    user: *mut c_void,
    cb: extern "C" fn(user: *mut c_void, data: *const u8, len: usize),
}

impl core_bitmap_on_data {
    fn exec(&self, data: *const u8, len: usize) {
        (self.cb)(self.user, data, len);
    }
}

struct BitmapDisplay {
    buf: Array2<u8>,
}

impl BitmapDisplay {
    fn new(w: u16, h: u16) -> Self {
        BitmapDisplay {
            buf: Array2::zeros((h as usize, w as usize)),
        }
    }

    fn to_vec(&self) -> Vec<u8> {
        self.buf.flatten().to_vec()
    }
}

impl DrawTarget for BitmapDisplay {
    type Color = BinaryColor;
    type Error = core::convert::Infallible;

    fn draw_iter<I>(&mut self, pixels: I) -> Result<(), Self::Error>
    where
        I: IntoIterator<Item = Pixel<Self::Color>>,
    {
        for Pixel(coord, color) in pixels.into_iter() {
            let w = self.buf.dim().1 as i32;
            let h = self.buf.dim().0 as i32;
            if coord.x >= 0 && coord.x < w && coord.y >= 0 && coord.y < h {
                self.buf[[coord.y as usize, coord.x as usize]] = if color.is_on() { 1 } else { 0 };
            }
        }

        Ok(())
    }
}

impl OriginDimensions for BitmapDisplay {
    fn size(&self) -> Size {
        Size::new(self.buf.dim().1 as u32, self.buf.dim().0 as u32)
    }
}

impl core_async_bitmap {
    fn new(
        w: u16,
        h: u16,
        notify: core_notify,
        on_data: core_bitmap_on_data,
    ) -> Result<Self, CString> {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("worker thread start");

            let mut display = BitmapDisplay::new(w, h);
            debug!("create bitmap: {w}x{h}");

            while let Ok(req) = req_rx.recv() {
                debug!("{req:?}");

                match req {
                    Request::Fill(value) => display.buf.fill(if value { 1 } else { 0 }),
                    Request::DrawPixel(x, y) => {
                        display.buf[(y as usize, x as usize)] = 1;
                    }
                    Request::DrawText(str, x, y) => {
                        // Create a new character style
                        let style = MonoTextStyle::new(&FONT_6X10, BinaryColor::On);

                        Text::new(str.as_str(), Point::new(x as i32, y as i32), style)
                            .draw(&mut display)
                            .unwrap();
                    }
                    Request::GetData => {
                        rep_tx
                            .send(Reply::Data(display.to_vec()))
                            .unwrap_or_else(|err| {
                                error!("{err}");
                            });
                        notify.notify();
                    }
                    Request::Clear => {
                        display.buf.fill(0);
                    }
                    Request::Invert => {
                        display.buf.iter_mut().for_each(|x| {
                            *x ^= 1;
                        });
                    }
                }
            }

            debug!("worker thread exit");
            Ok(())
        });

        Ok(core_async_bitmap {
            tx: req_tx,
            rx: rep_rx,
            on_data,
        })
    }
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_new(
    w: u16,
    h: u16,
    notify: core_notify,
    on_data: core_bitmap_on_data,
    on_err: core_on_msg,
) -> *mut core_async_bitmap {
    match core_async_bitmap::new(w, h, notify, on_data) {
        Ok(dht) => return Box::into_raw(Box::new(dht)),
        Err(err) => {
            on_err.exec_raw(&err);
            return null_mut();
        }
    }
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_free(bitmap: *mut core_async_bitmap) {
    if !bitmap.is_null() {
        drop(unsafe { Box::from_raw(bitmap) })
    }
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_process(bitmap: *mut core_async_bitmap) {
    if !bitmap.is_null() {
        let bitmap = unsafe { &*bitmap };
        while let Ok(rep) = bitmap.rx.try_recv() {
            match rep {
                Reply::Data(items) => {
                    bitmap.on_data.exec(items.as_ptr(), items.len());
                }
            }
        }
    }
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_draw_text(
    bitmap: *mut core_async_bitmap,
    txt: *const c_char,
    x: i16,
    y: i16,
) {
    if !bitmap.is_null() {
        let bitmap = unsafe { &*bitmap };
        let str = cstr_to_string(txt);

        bitmap
            .tx
            .send(Request::DrawText(str, x, y))
            .unwrap_or_else(|err| {
                error!("{err}");
            });
    }
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_get(bitmap: *mut core_async_bitmap) -> bool {
    core_async_bitmap::send_request(bitmap, Request::GetData)
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_clear(bitmap: *mut core_async_bitmap) -> bool {
    core_async_bitmap::send_request(bitmap, Request::Clear)
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_invert(bitmap: *mut core_async_bitmap) -> bool {
    core_async_bitmap::send_request(bitmap, Request::Invert)
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_fill(bitmap: *mut core_async_bitmap, value: bool) -> bool {
    core_async_bitmap::send_request(bitmap, Request::Fill(value))
}

#[cfg(test)]
mod tests {
    use embedded_graphics::{
        pixelcolor::BinaryColor,
        prelude::{DrawTarget, OriginDimensions, Point, Size},
        Pixel,
    };

    use super::BitmapDisplay;

    #[test]
    fn draw() {
        let mut d = BitmapDisplay::new(3, 2);
        assert_eq!(d.size(), Size::new(3, 2));
        d.draw_iter([
            Pixel(Point::new(0, 0), BinaryColor::On),
            Pixel(Point::new(1, 0), BinaryColor::Off),
            Pixel(Point::new(2, 0), BinaryColor::On),
            Pixel(Point::new(0, 1), BinaryColor::Off),
            Pixel(Point::new(1, 1), BinaryColor::On),
            Pixel(Point::new(2, 1), BinaryColor::Off),
        ])
        .unwrap();
        assert_eq!(d.to_vec(), vec![1, 0, 1, 0, 1, 0]);
    }
}
