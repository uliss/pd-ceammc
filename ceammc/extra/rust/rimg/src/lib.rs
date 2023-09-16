pub use image::flat::SampleLayout;
use image::io::Reader as ImageReader;
use std::ffi::c_char;
use std::ffi::CStr;
use std::io::{self, Write};  
use std::mem;

#[repr(C)]
pub enum ResultCode { 
    Ok,
    FileNotFound,
    UnknownImageFormat,
    InvalidFileName,
    InvalidString,
    InvalidArguments,
} 

#[repr(C)]
pub struct Result {
    data: *const u8,
    len: usize,
}

#[no_mangle]
pub unsafe extern "C" fn rimg_free_result(res: Option<&mut Result>) {
    if res.is_none() {
        return;
    }

    let res = res.unwrap();
    if res.data.is_null() {
        return;
    }

    drop(Vec::from_raw_parts(res.data.cast_mut(), res.len, res.len));
    res.data = std::ptr::null();
    res.len = 0;
}

#[no_mangle]
pub extern "C" fn rimg_decode_image(
    fname: *const c_char,
    layout: Option<&mut SampleLayout>,
    result: Option<&mut Result>,
) -> ResultCode { 
    if fname.is_null() {
        return ResultCode::InvalidFileName;
    }

    if layout.is_none() {
        return ResultCode::InvalidArguments;
    }

    if result.is_none() {
        return ResultCode::InvalidArguments;
    }

    let layout = layout.unwrap();
    let result = result.unwrap();

    let fname = unsafe { CStr::from_ptr(fname) };
    let fname = match fname.to_str() {
        Ok(x) => x,
        Err(_err) => {
            // println!("Error: {}", _err);
            io::stdout().flush().unwrap();
            return ResultCode::InvalidString;
        }
    }; 

    let img = ImageReader::open(fname);

    let img = match img {
        Ok(file) => file,
        Err(_err) => {
            // println!("Error: {}", _err);
            io::stdout().flush().unwrap();
            return ResultCode::FileNotFound;
        }
    };

    let img = match img.decode() {
        Ok(file) => file,
        Err(_err) => {
            // println!("Error: {}", _err);
            io::stdout().flush().unwrap();
            return ResultCode::UnknownImageFormat;
        }
    };

    let img = img.into_rgba8();
    let samples = img.as_flat_samples();
    *layout = samples.layout;

    let boxed_slice = samples.samples.to_vec();
    let array = boxed_slice.as_ptr();

    result.len = boxed_slice.len();
    result.data = array;

    // Prevent the slice from being destroyed (Leak the memory).
    mem::forget(boxed_slice);

    return ResultCode::Ok;
}
