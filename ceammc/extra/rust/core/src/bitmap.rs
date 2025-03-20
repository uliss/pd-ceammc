#![allow(non_camel_case_types)]
use std::collections::HashMap;
use std::ffi::{c_char, c_void};
use std::fs::File;
use std::io::BufWriter;
use std::path::Path;
use std::{ffi::CString, ptr::null_mut};

use base64::Engine;
use embedded_graphics::mono_font::iso_8859_5::{FONT_4X6, FONT_5X7, FONT_5X8, FONT_6X10, FONT_6X9};
use embedded_graphics::mono_font::MonoTextStyle;
use embedded_graphics::prelude::Angle;
use embedded_graphics::primitives::{
    Arc, Circle, Ellipse, Line, PrimitiveStyle, Rectangle, Sector, StrokeAlignment, StyledDrawable,
    Triangle,
};
use embedded_graphics::text::Text;
use embedded_graphics::Drawable;
use embedded_graphics::{
    pixelcolor::BinaryColor,
    prelude::{DrawTarget, OriginDimensions, Point, Size},
    Pixel,
};
use image::ImageReader;
use log::{debug, error};
use ndarray::{arr2, Array2, Axis};

use crate::{core_log_level, core_notify, core_on_msg, cstr_to_string, data_to_vec};

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
    DrawArc(i16, i16, u16, f32, f32, bool),
    DrawSector(i16, i16, u16, f32, f32, bool),
    DrawTriangle(i16, i16, i16, i16, i16, i16),
    DrawRow(u16, i16, i16),
    DrawColumn(u16, i16, i16),
    VShift(i16),
    HShift(i16),
    SetFont(String),
    SetStrokeWidth(u8),
    SetStrokeColor(Option<bool>),
    SetFillColor(Option<bool>),
    SetTextColor(Option<bool>),
    GetData,
    GetMatrix,
    GetSubMatrix(u16, u16, u16, u16),
    SetData(Vec<u8>),
    SetMatrix(Vec<u8>, u16, u16, u16, u16),
    InvertAxis(core_bitmap_axis),
    Save(String),
    Load(String, f32),
    View,
}

#[derive(Debug)]
pub enum Reply {
    Data(core_bitmap_output_format, u16, u16, Vec<u8>),
    ViewData(CString),
    Message(core_log_level, CString),
}

pub struct core_async_bitmap {
    tx: std::sync::mpsc::Sender<Request>,
    rx: std::sync::mpsc::Receiver<Reply>,
    on_data: core_bitmap_on_data,
    on_view: core_bitmap_on_view,
    on_msg: core_on_msg,
}

#[derive(Debug)]
#[repr(C)]
pub enum core_bitmap_output_format {
    List,
    Matrix,
}

#[derive(Debug)]
#[repr(C)]
pub enum core_bitmap_axis {
    X,
    Y,
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
    cb: extern "C" fn(
        user: *mut c_void,
        rows: u16,
        cols: u16,
        format: core_bitmap_output_format,
        data: *const u8,
        len: usize,
    ),
}

impl core_bitmap_on_data {
    fn exec(
        &self,
        rows: u16,
        cols: u16,
        format: core_bitmap_output_format,
        data: *const u8,
        len: usize,
    ) {
        (self.cb)(self.user, rows, cols, format, data, len);
    }
}

#[repr(C)]
pub struct core_bitmap_on_view {
    // nullable
    user: *mut c_void,
    // not NULL
    cb: extern "C" fn(user: *mut c_void, base64_str: *const c_char),
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

    fn to_submatrix(&self, row: u16, col: u16, num_rows: u16, num_cols: u16) -> Array2<u8> {
        let r0 = row as usize;
        let c0 = col as usize;
        if r0 >= self.buf.dim().0 || c0 >= self.buf.dim().1 {
            return arr2(&[[]]);
        }

        let r1 = (r0 + num_rows as usize).min(self.buf.dim().0);
        let c1 = (c0 + num_cols as usize).min(self.buf.dim().1);

        use ndarray::s;
        self.buf.slice(s![r0..r1, c0..c1]).to_owned()
    }

    fn rotate_up(&mut self, dy: i16) {
        use ndarray::s;

        let ydim = self.buf.dim().0;
        let dy = if dy < 0 {
            (dy.abs() as usize) % ydim
        } else {
            (ydim - ((dy as usize) % ydim)) % ydim
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
        let dx = if dx < 0 {
            (dx.abs() as usize) % xdim
        } else {
            (xdim - ((dx as usize) % xdim)) % xdim
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

    fn set_data(&mut self, mut data: Vec<u8>) {
        data.resize(self.buf.len(), 0);
        match Array2::from_shape_vec([self.buf.dim().0, self.buf.dim().1], data) {
            Ok(arr) => {
                self.buf = arr;
            }
            Err(err) => {
                error!("{err}")
            }
        }
    }

    fn set_matrix(&mut self, mut data: Vec<u8>, nrows: u16, ncols: u16, row: u16, col: u16) {
        use ndarray::s;

        let nrows = nrows as usize;
        let ncols = ncols as usize;
        data.resize(nrows * ncols, 0);

        let row = row as usize;
        let col = col as usize;

        let new_rows = self.buf.dim().0.min(nrows + row);
        let new_cols = self.buf.dim().1.min(ncols + col);

        match Array2::from_shape_vec([nrows, ncols], data) {
            Ok(arr) => {
                self.buf
                    .slice_mut(s![row..new_rows, col..new_cols])
                    .assign(&arr.slice(s![..new_rows - row, ..new_cols - col]));
            }
            Err(err) => {
                error!("{err}")
            }
        }
    }

    fn send_reply(&self, tx: &std::sync::mpsc::Sender<Reply>, msg: Reply, notify: core_notify) {
        tx.send(msg)
            .map(|_| notify.notify())
            .unwrap_or_else(|err| error!("{err}"))
    }

    fn send_error(&self, tx: &std::sync::mpsc::Sender<Reply>, msg: &str, notify: core_notify) {
        error!("{msg}");

        self.send_reply(
            tx,
            Reply::Message(core_log_level::Error, CString::new(msg).unwrap_or_default()),
            notify,
        )
    }

    fn send_debug(&self, tx: &std::sync::mpsc::Sender<Reply>, msg: &str, notify: core_notify) {
        debug!("{msg}");

        self.send_reply(
            tx,
            Reply::Message(core_log_level::Debug, CString::new(msg).unwrap_or_default()),
            notify,
        )
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
        on_view: core_bitmap_on_view,
        on_msg: core_on_msg,
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
                        display.send_reply(
                            &rep_tx,
                            Reply::Data(
                                core_bitmap_output_format::List,
                                display.buf.dim().0 as u16,
                                display.buf.dim().1 as u16,
                                display.to_vec(),
                            ),
                            notify,
                        );
                    }
                    Request::GetMatrix => {
                        display.send_reply(
                            &rep_tx,
                            Reply::Data(
                                core_bitmap_output_format::Matrix,
                                display.buf.dim().0 as u16,
                                display.buf.dim().1 as u16,
                                display.to_vec(),
                            ),
                            notify,
                        );
                    }
                    Request::GetSubMatrix(row, col, num_rows, num_cols) => {
                        let sub_mtx = display.to_submatrix(row, col, num_rows, num_cols);

                        display.send_reply(
                            &rep_tx,
                            Reply::Data(
                                core_bitmap_output_format::Matrix,
                                sub_mtx.dim().0 as u16,
                                sub_mtx.dim().1 as u16,
                                sub_mtx.flatten().to_vec(),
                            ),
                            notify,
                        );
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

                                let font_list =
                                    font_map.iter().fold(String::new(), |mut a, (key, _)| {
                                        a += " ";
                                        a += *key;
                                        a
                                    });

                                display.send_error(
                                    &rep_tx,
                                    format!("supported fonts are: {}", font_list.trim_start())
                                        .as_str(),
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
                    Request::DrawArc(x, y, diam, start, length, center) => {
                        if center {
                            Arc::with_center(
                                to_pt(x, y),
                                diam as u32,
                                Angle::from_degrees(start),
                                Angle::from_degrees(length),
                            )
                            .draw_styled(&draw_style, &mut display)
                            .unwrap();
                        } else {
                            Arc::new(
                                to_pt(x, y),
                                diam as u32,
                                Angle::from_degrees(start),
                                Angle::from_degrees(length),
                            )
                            .draw_styled(&draw_style, &mut display)
                            .unwrap();
                        }
                    }
                    Request::DrawSector(x, y, diam, start, length, center) => {
                        if center {
                            Sector::with_center(
                                to_pt(x, y),
                                diam as u32,
                                Angle::from_degrees(start),
                                Angle::from_degrees(length),
                            )
                            .draw_styled(&draw_style, &mut display)
                            .unwrap();
                        } else {
                            Sector::new(
                                to_pt(x, y),
                                diam as u32,
                                Angle::from_degrees(start),
                                Angle::from_degrees(length),
                            )
                            .draw_styled(&draw_style, &mut display)
                            .unwrap();
                        }
                    }
                    Request::DrawTriangle(x0, y0, x1, y1, x2, y2) => {
                        Triangle::new(to_pt(x0, y0), to_pt(x1, y1), to_pt(x2, y2))
                            .draw_styled(&draw_style, &mut display)
                            .unwrap();
                    }
                    Request::DrawRow(row, offset, length) => {
                        if length.abs() > 0 {
                            let start = Point::new(offset as i32, row as i32);
                            let end =
                                Point::new((offset + length - length.signum()) as i32, row as i32);

                            Line::new(start, end)
                                .draw_styled(&draw_style, &mut display)
                                .unwrap();
                        }
                    }
                    Request::DrawColumn(col, offset, length) => {
                        if length.abs() > 0 {
                            let start = Point::new(col as i32, offset as i32);
                            let end =
                                Point::new(col as i32, (offset + length - length.signum()) as i32);

                            Line::new(start, end)
                                .draw_styled(&draw_style, &mut display)
                                .unwrap();
                        }
                    }
                    Request::SetData(data) => display.set_data(data),
                    Request::SetMatrix(data, nrows, ncols, row, col) => {
                        display.set_matrix(data, nrows, ncols, row, col);
                    }
                    Request::InvertAxis(axis) => {
                        display.buf.invert_axis(Axis(match axis {
                            core_bitmap_axis::X => 0,
                            core_bitmap_axis::Y => 1,
                        }));
                    }
                    Request::Save(filename) => {
                        let path = Path::new(&filename);
                        let file = match File::create(path) {
                            Ok(file) => file,
                            Err(err) => {
                                display.send_error(
                                    &rep_tx,
                                    format!("Error creating file '{filename}': {err}").as_str(),
                                    notify,
                                );
                                continue;
                            }
                        };
                        let ref mut w = BufWriter::new(file);

                        let mut encoder =
                            png::Encoder::new(w, display.size().width, display.size().height);
                        encoder.set_color(png::ColorType::Grayscale);
                        encoder.set_depth(png::BitDepth::Eight);
                        let mut writer = match encoder.write_header() {
                            Ok(w) => w,
                            Err(err) => {
                                display.send_error(
                                    &rep_tx,
                                    format!("Encoder error: {err}").as_str(),
                                    notify,
                                );
                                continue;
                            }
                        };

                        let data = display
                            .buf
                            .flatten()
                            .map(|x| match x {
                                0 => 0,
                                _ => 0xff,
                            })
                            .to_vec();
                        writer
                            .write_image_data(data.as_slice())
                            .unwrap_or_else(|err| {
                                display.send_error(
                                    &rep_tx,
                                    format!("Write error: {err}").as_str(),
                                    notify,
                                );
                            });
                    }
                    Request::SetTextColor(color) => {
                        text_style.text_color = color.map(|c| to_color(c))
                    }
                    Request::Load(filename, scale) => {
                        let reader = match ImageReader::open(filename.as_str()) {
                            Ok(reader) => reader,
                            Err(err) => {
                                display.send_error(
                                    &rep_tx,
                                    format!("load error for \"{filename}\": {err}").as_str(),
                                    notify,
                                );
                                continue;
                            }
                        };

                        let img = match reader.decode() {
                            Ok(img) => {
                                let w = (img.width() as f32 * scale).round() as u32;
                                let h = (img.height() as f32 * scale).round() as u32;
                                img.resize(w, h, image::imageops::FilterType::Lanczos3)
                                    .into_luma8()
                            }
                            Err(err) => {
                                display.send_error(
                                    &rep_tx,
                                    format!("image decode error: {err}").as_str(),
                                    notify,
                                );
                                continue;
                            }
                        };

                        let w = if img.width() > std::u16::MAX.into() {
                            display.send_error(
                                &rep_tx,
                                format!("image width is too big: {}", img.width()).as_str(),
                                notify,
                            );
                            continue;
                        } else {
                            img.width() as u16
                        };

                        let h = if img.height() > std::u16::MAX.into() {
                            display.send_error(
                                &rep_tx,
                                format!("image height is too big: {}", img.height()).as_str(),
                                notify,
                            );
                            continue;
                        } else {
                            img.height() as u16
                        };

                        display = BitmapDisplay::new(w, h);

                        for (a, b) in &mut display.buf.iter_mut().zip(img.iter()) {
                            *a = if *b > 127 { 1 } else { 0 };
                        }

                        display.send_debug(
                            &rep_tx,
                            format!("image loaded: {filename} ({w}x{h})").as_str(),
                            notify,
                        );
                    }
                    Request::View => {
                        let mut bytes: Vec<u8> = Vec::new();
                        let mut img =
                            image::GrayImage::new(display.size().width, display.size().height);

                        for (a, b) in img.iter_mut().zip(display.buf.iter()) {
                            *a = if *b > 0 { 255 } else { 0 };
                        }

                        if let Err(err) = img.write_to(
                            &mut std::io::Cursor::new(&mut bytes),
                            image::ImageFormat::Png,
                        ) {
                            display.send_error(
                                &rep_tx,
                                format!("image write error: {err}").as_str(),
                                notify,
                            );
                            continue;
                        }

                        let data = CString::new(base64::prelude::BASE64_STANDARD.encode(bytes))
                            .unwrap_or_default();

                        display.send_reply(&rep_tx, Reply::ViewData(data), notify);
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
            on_view,
            on_msg,
        })
    }
}

/// create bitmap struct and starts separate worker thread(!)
/// @param w - bitmap width in pixels
/// @param h - bitmap height in pixels
/// @param notify - notify callback called when some processing result is ready
/// @param on_data - data callback
/// @param on_err - error callback
/// @return point to bitmap or NULL on error
#[no_mangle]
pub extern "C" fn ceammc_bitmap_new(
    w: u16,
    h: u16,
    notify: core_notify,
    on_data: core_bitmap_on_data,
    on_view: core_bitmap_on_view,
    on_msg: core_on_msg,
) -> *mut core_async_bitmap {
    match core_async_bitmap::new(w, h, notify, on_data, on_view, on_msg.clone()) {
        Ok(dht) => return Box::into_raw(Box::new(dht)),
        Err(err) => {
            on_msg.error_cstr(&err);
            return null_mut();
        }
    }
}

/// free bitmap struct and stops worker thread
/// @param bitmap - bitmap pointer (nullable)
#[no_mangle]
pub extern "C" fn ceammc_bitmap_free(bitmap: *mut core_async_bitmap) {
    if !bitmap.is_null() {
        drop(unsafe { Box::from_raw(bitmap) })
    }
}

/// process ready bitmap data
/// @param bitmap - bitmap pointer (nullable)
#[no_mangle]
pub extern "C" fn ceammc_bitmap_process(bitmap: *mut core_async_bitmap) {
    if !bitmap.is_null() {
        let bitmap = unsafe { &*bitmap };
        while let Ok(rep) = bitmap.rx.try_recv() {
            match rep {
                Reply::Data(format, rows, cols, data) => {
                    bitmap
                        .on_data
                        .exec(rows, cols, format, data.as_ptr(), data.len());
                }
                Reply::Message(level, str) => {
                    bitmap.on_msg.exec_raw(level, &str);
                }
                Reply::ViewData(cstr) => {
                    (bitmap.on_view.cb)(bitmap.on_view.user, cstr.as_ptr());
                }
            }
        }
    }
}

/// draw text on bitmap
/// @param bitmap - bitmap pointer (nullable)
/// @param txt - text C-string
/// @param x - start left coord
/// @param y - start bottom coord
#[no_mangle]
pub extern "C" fn ceammc_bitmap_draw_text(
    bitmap: *mut core_async_bitmap,
    txt: *const c_char,
    x: i16,
    y: i16,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::DrawText(cstr_to_string(txt), x, y))
}

/// draw line
/// @param bitmap - bitmap pointer (nullable)
/// @param x0 - start x coord
/// @param y0 - start y coord
/// @param x1 - end x coord
/// @param y1 - end y coord
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

/// draw rect
/// @param bitmap - bitmap pointer (nullable)
/// @param x - left or center x coord
/// @param y - top or center y coord
/// @param w - rect width
/// @param h - rect height
/// @param center - use center coord as origin
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

/// draw circle
/// @param bitmap - bitmap pointer (nullable)
/// @param x - left or center x coord
/// @param y - top or center y coord
/// @param diam - circle diameter
/// @param center - use center coord as origin
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

/// draw ellipse
/// @param bitmap - bitmap pointer (nullable)
/// @param x - left or center x coord
/// @param y - top or center y coord
/// @param w - ellipse width
/// @param h - ellipse height
/// @param center - use center coord as origin
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

/// draw open arc
/// @param bitmap - bitmap pointer (nullable)
/// @param x - left or center x coord
/// @param y - top or center y coord
/// @param diam - correspoding circle diameter
/// @param angle_start - start angle in degrees, 0º - NORD orientation
/// @param arc_length - length in degrees, 360º - full circle
/// @param center - use center coord as origin
#[no_mangle]
pub extern "C" fn ceammc_bitmap_draw_arc(
    bitmap: *mut core_async_bitmap,
    x: i16,
    y: i16,
    diam: u16,
    angle_start: f32,
    arc_length: f32,
    center: bool,
) -> bool {
    core_async_bitmap::send_request(
        bitmap,
        Request::DrawArc(x, y, diam, angle_start, arc_length, center),
    )
}

/// draw filled sector
/// @param bitmap - bitmap pointer (nullable)
/// @param x - left or center x coord
/// @param y - top or center y coord
/// @param diam - correspoding circle diameter
/// @param angle_start - start angle in degrees, 0º - NORD orientation
/// @param arc_length - length in degrees, 360º - full circle
/// @param center - use center coord as origin
#[no_mangle]
pub extern "C" fn ceammc_bitmap_draw_sector(
    bitmap: *mut core_async_bitmap,
    x: i16,
    y: i16,
    diam: u16,
    angle_start: f32,
    arc_length: f32,
    center: bool,
) -> bool {
    core_async_bitmap::send_request(
        bitmap,
        Request::DrawSector(x, y, diam, angle_start, arc_length, center),
    )
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_draw_triangle(
    bitmap: *mut core_async_bitmap,
    x0: i16,
    y0: i16,
    x1: i16,
    y1: i16,
    x2: i16,
    y2: i16,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::DrawTriangle(x0, y0, x1, y1, x2, y2))
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
pub extern "C" fn ceammc_bitmap_draw_column(
    bitmap: *mut core_async_bitmap,
    col: u16,
    height: i16,
    offset: i16,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::DrawColumn(col, offset, height))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_draw_row(
    bitmap: *mut core_async_bitmap,
    row: u16,
    width: i16,
    offset: i16,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::DrawRow(row, offset, width))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_get_data(bitmap: *mut core_async_bitmap) -> bool {
    core_async_bitmap::send_request(bitmap, Request::GetData)
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_get_matrix(bitmap: *mut core_async_bitmap) -> bool {
    core_async_bitmap::send_request(bitmap, Request::GetMatrix)
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_get_submatrix(
    bitmap: *mut core_async_bitmap,
    row: u16,
    col: u16,
    num_rows: u16,
    num_cols: u16,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::GetSubMatrix(row, col, num_rows, num_cols))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_set_data(
    bitmap: *mut core_async_bitmap,
    data: *const u8,
    len: usize,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::SetData(data_to_vec(data, len)))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_set_matrix(
    bitmap: *mut core_async_bitmap,
    nrows: u16,
    ncols: u16,
    at_row: u16,
    at_col: u16,
    data: *const u8,
    len: usize,
) -> bool {
    core_async_bitmap::send_request(
        bitmap,
        Request::SetMatrix(data_to_vec(data, len), nrows, ncols, at_row, at_col),
    )
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
pub extern "C" fn ceammc_bitmap_save_to_png(
    bitmap: *mut core_async_bitmap,
    path: *const c_char,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::Save(cstr_to_string(path)))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_load(
    bitmap: *mut core_async_bitmap,
    path: *const c_char,
    scale: f32,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::Load(cstr_to_string(path), scale))
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_view(bitmap: *mut core_async_bitmap) -> bool {
    core_async_bitmap::send_request(bitmap, Request::View)
}

#[no_mangle]
pub extern "C" fn ceammc_bitmap_set_text_color(bitmap: *mut core_async_bitmap, color: i8) -> bool {
    core_async_bitmap::send_request(bitmap, Request::SetTextColor(i2_into_color(color)))
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

#[no_mangle]
pub extern "C" fn ceammc_bitmap_invert_axis(
    bitmap: *mut core_async_bitmap,
    axis: core_bitmap_axis,
) -> bool {
    core_async_bitmap::send_request(bitmap, Request::InvertAxis(axis))
}

#[cfg(test)]
mod tests {
    use embedded_graphics::{
        pixelcolor::BinaryColor,
        prelude::{DrawTarget, OriginDimensions, Point, Size},
        Pixel,
    };
    use ndarray::{arr2, Array};

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

        d.rotate_up(-1);
        assert_eq!(d.buf, arr2(&[[4, 5, 6], [1, 2, 3]]));
        d.rotate_up(-2);
        assert_eq!(d.buf, arr2(&[[4, 5, 6], [1, 2, 3]]));
        d.rotate_up(1);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 5, 6]]));
        d.rotate_up(2);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 5, 6]]));

        d.rotate_right(3);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 5, 6]]));
        d.rotate_right(-3);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 5, 6]]));
        d.rotate_right(-1);
        assert_eq!(d.buf, arr2(&[[2, 3, 1], [5, 6, 4]]));
        d.rotate_right(1);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 5, 6]]));
        d.rotate_right(-2);
        assert_eq!(d.buf, arr2(&[[3, 1, 2], [6, 4, 5]]));
    }

    #[test]
    fn submatrix() {
        let mut d = BitmapDisplay::new(4, 4);
        d.buf = Array::range(0., 16., 1.)
            .into_shape_with_order((4, 4))
            .unwrap()
            .map(|x| *x as u8);

        assert_eq!(d.to_submatrix(0, 0, 1, 1), arr2(&[[0]]));
        assert_eq!(d.to_submatrix(3, 3, 1, 1), arr2(&[[15]]));

        assert_eq!(d.to_submatrix(0, 0, 2, 2), arr2(&[[0, 1], [4, 5]]));
        assert_eq!(d.to_submatrix(2, 2, 10, 10), arr2(&[[10, 11], [14, 15]]));
        assert_eq!(d.to_submatrix(10, 10, 10, 10), arr2(&[[]]));
    }

    #[test]
    fn set_data() {
        let mut d = BitmapDisplay::new(3, 2);
        d.set_data(vec![1, 2, 3, 4]);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 0, 0]]));

        d.set_data(vec![1, 2, 3, 4, 5, 6]);
        assert_eq!(d.buf, arr2(&[[1, 2, 3], [4, 5, 6]]));

        d.set_data(vec![2, 3, 4, 5, 6, 7, 8]);
        assert_eq!(d.buf, arr2(&[[2, 3, 4], [5, 6, 7]]));

        d.set_data(vec![1, 2]);
        assert_eq!(d.buf, arr2(&[[1, 2, 0], [0, 0, 0]]));
    }

    #[test]
    fn set_matrix() {
        let mut d = BitmapDisplay::new(4, 3);
        d.set_matrix(vec![1, 2, 3, 4], 2, 2, 0, 0);
        assert_eq!(d.buf, arr2(&[[1, 2, 0, 0], [3, 4, 0, 0], [0, 0, 0, 0]]));

        d.buf.fill(0);
        d.set_matrix(vec![1, 2, 3, 4], 2, 2, 0, 1);
        assert_eq!(d.buf, arr2(&[[0, 1, 2, 0], [0, 3, 4, 0], [0, 0, 0, 0]]));

        d.buf.fill(0);
        d.set_matrix(vec![1, 2, 3, 4], 2, 2, 0, 2);
        assert_eq!(d.buf, arr2(&[[0, 0, 1, 2], [0, 0, 3, 4], [0, 0, 0, 0]]));

        d.buf.fill(0);
        d.set_matrix(vec![1, 2, 3, 4], 2, 2, 0, 3);
        assert_eq!(d.buf, arr2(&[[0, 0, 0, 1], [0, 0, 0, 3], [0, 0, 0, 0]]));

        d.buf.fill(0);
        d.set_matrix(vec![1, 2, 3, 4], 2, 2, 0, 4);
        assert_eq!(d.buf, arr2(&[[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]]));

        d.buf.fill(0);
        d.set_matrix(vec![1, 2, 3, 4], 2, 2, 1, 2);
        assert_eq!(d.buf, arr2(&[[0, 0, 0, 0], [0, 0, 1, 2], [0, 0, 3, 4]]));

        d.buf.fill(0);
        d.set_matrix(vec![1, 2, 3, 4], 2, 2, 2, 2);
        assert_eq!(d.buf, arr2(&[[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 1, 2]]));

        d.buf.fill(0);
        d.set_matrix(vec![1, 2, 3, 4], 2, 2, 2, 3);
        assert_eq!(d.buf, arr2(&[[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 1]]));

        d.buf.fill(0);
        d.set_matrix(vec![1, 2, 3, 4], 2, 2, 3, 4);
        assert_eq!(d.buf, arr2(&[[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]]));
    }
}
