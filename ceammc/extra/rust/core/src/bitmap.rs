#![allow(non_camel_case_types)]
use std::collections::HashMap;
use std::ffi::{c_char, c_void};
use std::{ffi::CString, ptr::null_mut};

use embedded_graphics::mono_font::iso_8859_5::{FONT_4X6, FONT_5X7, FONT_5X8, FONT_6X10, FONT_6X9};
use embedded_graphics::mono_font::MonoTextStyle;
use embedded_graphics::primitives::{
    Circle, Ellipse, Line, PrimitiveStyle, Rectangle, StrokeAlignment, StyledDrawable,
};
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
    SetPixel(u16, u16, bool),
    DrawText(String, i16, i16),
    DrawLine(i16, i16, i16, i16),
    DrawRect(i16, i16, u16, u16, bool),
    DrawCircle(i16, i16, u16, bool),
    DrawEllipse(i16, i16, u16, u16, bool),
    VShift(i16),
    HShift(i16),
    SetFont(String),
    SetStrokeWidth(u8),
    SetStrokeColor(Option<bool>),
    SetFillColor(Option<bool>),
    GetData,
}

#[derive(Debug)]
pub enum Reply {
    Data(Vec<u8>),
    Error(CString),
}

pub struct core_async_bitmap {
    tx: std::sync::mpsc::Sender<Request>,
    rx: std::sync::mpsc::Receiver<Reply>,
    on_data: core_bitmap_on_data,
    on_err: core_on_msg,
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

fn i2_into_color(x: i8) -> Option<bool> {
    if x > 0 {
        Some(true)
    } else if x == 0 {
        Some(false)
    } else {
        None
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

    fn rotate_up(&mut self, dy: i16) {
        use ndarray::s;

        let ydim = self.buf.dim().0;
        let dy = if dy >= 0 {
            (dy as usize) % ydim
        } else {
            (ydim - ((dy.abs() as usize) % ydim)) % ydim
        };

        if dy == 0 {
            return;
        }

        let copy = self.buf.clone();
        let l0 = dy;
        let l1 = ydim - dy;

        self.buf
            .slice_mut(s![..l1, ..])
            .assign(&copy.slice(s![l0.., ..]));

        self.buf
            .slice_mut(s![l1.., ..])
            .assign(&copy.slice(s![..l0, ..]));
    }

    fn rotate_right(&mut self, dx: i16) {
        use ndarray::s;

        let xdim = self.buf.dim().1;
        let dx = if dx >= 0 {
            (dx as usize) % xdim
        } else {
            (xdim - ((dx.abs() as usize) % xdim)) % xdim
        };

        if dx == 0 {
            return;
        }

        let copy = self.buf.clone();
        let l0 = dx;
        let l1 = xdim - dx;

        self.buf
            .slice_mut(s![.., ..l1])
            .assign(&copy.slice(s![.., l0..]));

        self.buf
            .slice_mut(s![.., l1..])
            .assign(&copy.slice(s![.., ..l0]));
    }

    fn set_pixel(&mut self, x: u16, y: u16, value: bool) {
        let x = x as usize;
        let y = y as usize;

        if x < self.buf.dim().1 && y < self.buf.dim().0 {
            self.buf[(y, x)] = if value { 1 } else { 0 };
        } else {
            error!("invalid pixel value: {x} {y}");
        }
    }

    fn send_error(&self, tx: &std::sync::mpsc::Sender<Reply>, msg: &str, notify: core_notify) {
        error!("{msg}");

        tx.send(Reply::Error(CString::new(msg).unwrap_or_default()))
            .map(|_| notify.notify())
            .unwrap_or_else(|err| error!("{err}"));
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
        on_err: core_on_msg,
    ) -> Result<Self, CString> {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("worker thread start");

            let mut display = BitmapDisplay::new(w, h);
            debug!("create bitmap: {w}x{h}");

            let to_pt = |x: i16, y: i16| Point::new(x as i32, y as i32);
            let to_size = |x: u16, y: u16| Size::new(x as u32, y as u32);
            let to_color = |color: bool| {
                if color {
                    BinaryColor::On
                } else {
                    BinaryColor::Off
                }
            };

            let mut draw_style = PrimitiveStyle::new();
            draw_style.stroke_color = Some(BinaryColor::On);
            draw_style.stroke_alignment = StrokeAlignment::Inside;
            draw_style.stroke_width = 1;
            let mut text_style = MonoTextStyle::new(&FONT_5X8, BinaryColor::On);

            let mut font_map = HashMap::new();

            font_map.insert("FONT_4X6", &FONT_4X6);
            font_map.insert("FONT_5X7", &FONT_5X7);
            font_map.insert("FONT_5X8", &FONT_5X8);
            font_map.insert("FONT_6X9", &FONT_6X9);
            font_map.insert("FONT_6X10", &FONT_6X10);

            while let Ok(req) = req_rx.recv() {
                debug!("{req:?}");

                match req {
                    Request::Fill(value) => display.buf.fill(if value { 1 } else { 0 }),
                    Request::SetPixel(x, y, value) => display.set_pixel(x, y, value),
                    Request::DrawText(str, x, y) => {
                        Text::new(str.as_str(), to_pt(x, y), text_style)
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
                    Request::Clear => display.buf.fill(0),
                    Request::Invert => {
                        display.buf.iter_mut().for_each(|x| {
                            *x ^= 1;
                        });
                    }
                    Request::DrawLine(x0, y0, x1, y1) => {
                        Line::new(to_pt(x0, y0), to_pt(x1, y1))
                            .draw_styled(&draw_style, &mut display)
                            .unwrap();
                    }
                    Request::VShift(dy) => display.rotate_up(dy),
                    Request::HShift(dx) => display.rotate_right(dx),
                    Request::SetFont(font) => {
                        let font = font.to_uppercase();
                        match font_map.get(font.as_str()) {
                            Some(ft) => {
                                text_style.font = ft;
                            }
                            None => {
                                display.send_error(
                                    &rep_tx,
                                    format!("unknown font: {font}").as_str(),
                                    notify,
                                );
                            }
                        }
                    }
                    Request::DrawRect(x, y, w, h, center) => {
                        if center {
                            Rectangle::with_center(to_pt(x, y), to_size(w, h))
                                .draw_styled(&draw_style, &mut display)
                                .unwrap();
                        } else {
                            Rectangle::new(to_pt(x, y), to_size(w, h))
                                .draw_styled(&draw_style, &mut display)
                                .unwrap();
                        }
                    }
                    Request::SetStrokeWidth(wd) => draw_style.stroke_width = wd as u32,
                    Request::SetStrokeColor(color) => {
                        draw_style.stroke_color = color.map(|c| to_color(c))
                    }
                    Request::SetFillColor(color) => {
                        draw_style.fill_color = color.map(|c| to_color(c))
                    }
                    Request::DrawCircle(x, y, diam, center) => {
                        if center {
                            Circle::with_center(to_pt(x, y), diam as u32)
                                .draw_styled(&draw_style, &mut display)
                                .unwrap();
                        } else {
                            Circle::new(to_pt(x, y), diam as u32)
                                .draw_styled(&draw_style, &mut display)
                                .unwrap();
                        }
                    }
                    Request::DrawEllipse(x, y, w, h, center) => {
                        if center {
                            Ellipse::with_center(to_pt(x, y), to_size(w, h))
                                .draw_styled(&draw_style, &mut display)
                                .unwrap();
                        } else {
                            Ellipse::new(to_pt(x, y), to_size(w, h))
                                .draw_styled(&draw_style, &mut display)
                                .unwrap();
                        }
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
            on_err,
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
    match core_async_bitmap::new(w, h, notify, on_data, on_err.clone()) {
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
                Reply::Error(str) => {
                    bitmap.on_err.exec_raw(&str);
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
pub extern "C" fn ceammc_bitmap_draw_line(
    bitmap: *mut core_async_bitmap,
    x0: i16,
    y0: i16,
    x1: i16,
    y1: i16,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::DrawLine(x0, y0, x1, y1))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_draw_rect(
    bitmap: *mut core_async_bitmap,
    x: i16,
    y: i16,
    w: u16,
    h: u16,
    center: bool,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::DrawRect(x, y, w, h, center))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_draw_circle(
    bitmap: *mut core_async_bitmap,
    x: i16,
    y: i16,
    diam: u16,
    center: bool,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::DrawCircle(x, y, diam, center))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_draw_ellipse(
    bitmap: *mut core_async_bitmap,
    x: i16,
    y: i16,
    w: u16,
    h: u16,
    center: bool,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::DrawEllipse(x, y, w, h, center))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_draw_pixel(
    bitmap: *mut core_async_bitmap,
    x: u16,
    y: u16,
    value: bool,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::SetPixel(x, y, value))
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

#[no_mangle]
pub extern "C" fn ceammc_bitmap_vshift(bitmap: *mut core_async_bitmap, dy: i16) -> bool {
    core_async_bitmap::send_request(bitmap, Request::VShift(dy))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_hshift(bitmap: *mut core_async_bitmap, dx: i16) -> bool {
    core_async_bitmap::send_request(bitmap, Request::HShift(dx))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_font(bitmap: *mut core_async_bitmap, font: *const c_char) -> bool {
    core_async_bitmap::send_request(bitmap, Request::SetFont(cstr_to_string(font)))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_set_fill_color(bitmap: *mut core_async_bitmap, color: i8) -> bool {
    core_async_bitmap::send_request(bitmap, Request::SetFillColor(i2_into_color(color)))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_set_stroke_color(
    bitmap: *mut core_async_bitmap,
    color: i8,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::SetFillColor(i2_into_color(color)))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_set_stroke_width(
    bitmap: *mut core_async_bitmap,
    width: u8,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::SetStrokeWidth(width))
}

#[cfg(test)]
mod tests {
    use embedded_graphics::{
        pixelcolor::BinaryColor,
        prelude::{DrawTarget, OriginDimensions, Point, Size},
        Pixel,
    };
    use ndarray::arr2;

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

    #[test]
    fn rotate() {
        let mut d = BitmapDisplay::new(3, 2);
        assert_eq!(d.size(), Size::new(3, 2));
        d.buf = arr2(&[[1, 2, 3], [4, 5, 6]]);

        d.rotate_up(1);
        assert_eq!(d.buf, arr2(&[[4, 5, 6], [1, 2, 3]]));
        d.rotate_up(2);
        assert_eq!(d.buf, arr2(&[[4, 5, 6], [1, 2, 3]]));
        d.rotate_up(-1);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 5, 6]]));
        d.rotate_up(-2);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 5, 6]]));

        d.rotate_right(3);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 5, 6]]));
        d.rotate_right(-3);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 5, 6]]));
        d.rotate_right(1);
        assert_eq!(d.buf, arr2(&[[2, 3, 1], [5, 6, 4]]));
        d.rotate_right(-1);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 5, 6]]));
        d.rotate_right(2);
        assert_eq!(d.buf, arr2(&[[3, 1, 2], [6, 4, 5]]));
    }
}
