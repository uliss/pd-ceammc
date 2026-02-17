use path_slash::PathBufExt as _;
use std::{
    env,
    ffi::{c_char, c_void, CString},
    path::PathBuf,
};

use dirs;
use log::{error, warn};

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct ceammc_path_str_cb {
    /// nullable
    user: *mut c_void,
    /// not NULL
    /// NOTE: param str is valid only inside of callback call
    /// to future usage do not save str pointer, copy(!) string to elsewhere
    cb: extern "C" fn(user: *mut c_void, str: *const c_char),
}

fn output_path(path: PathBuf, cb: ceammc_path_str_cb) -> bool {
    if let Ok(str) = CString::new(path.to_slash_lossy().as_ref()) {
        (cb.cb)(cb.user, str.as_ptr());
        true
    } else {
        false
    }
}

fn path_to_string(path: Option<PathBuf>, cb: ceammc_path_str_cb) -> bool {
    match path {
        Some(path) => {
            if !path.exists() {
                error!("path not exists: '{}'", path.to_string_lossy());
                return false;
            }

            match path.canonicalize() {
                Ok(new_path) => output_path(new_path, cb),
                Err(err) => {
                    warn!("can't canonicalize path: {err}");
                    output_path(path, cb)
                }
            }
        }
        None => {
            error!("path not found");
            false
        }
    }
}

#[no_mangle]
/// cross-platform home directory path with '/' slashes
pub extern "C" fn ceammc_path_home(cb: ceammc_path_str_cb) -> bool {
    path_to_string(dirs::home_dir(), cb)
}

#[no_mangle]
/// cross-platform pd user directory path with '/' slashes
pub extern "C" fn ceammc_path_pd_user(cb: ceammc_path_str_cb) -> bool {
    let path = dirs::home_dir().map(|mut path| {
        path.push("Documents");
        path.push("Pd");
        path
    });
    path_to_string(path, cb)
}

#[no_mangle]
/// cross-platform puredata doc directory path with '/' slashes
/// using PD_DOC env variable
pub extern "C" fn ceammc_path_pd_doc(cb: ceammc_path_str_cb) -> bool {
    let path = env::var("PD_DOC").ok().map(|var| PathBuf::from(var));
    path_to_string(path, cb)
}

#[no_mangle]
/// cross-platform ceammc external doc directory path with '/' slashes
/// using CEAMMC_DOC env variable
pub extern "C" fn ceammc_path_ceammc_doc(cb: ceammc_path_str_cb) -> bool {
    let path = env::var("CEAMMC_DOC").ok().map(|var| PathBuf::from(var));
    path_to_string(path, cb)
}

#[no_mangle]
/// cross-platform desktop directory path with '/' slashes
pub extern "C" fn ceammc_path_desktop(cb: ceammc_path_str_cb) -> bool {
    path_to_string(dirs::desktop_dir(), cb)
}

#[no_mangle]
/// cross-platform audio directory path with '/' slashes
pub extern "C" fn ceammc_path_audio(cb: ceammc_path_str_cb) -> bool {
    path_to_string(dirs::audio_dir(), cb)
}

#[no_mangle]
/// cross-platform video directory path with '/' slashes
pub extern "C" fn ceammc_path_video(cb: ceammc_path_str_cb) -> bool {
    path_to_string(dirs::video_dir(), cb)
}

#[no_mangle]
/// cross-platform images directory path with '/' slashes
pub extern "C" fn ceammc_path_image(cb: ceammc_path_str_cb) -> bool {
    path_to_string(dirs::picture_dir(), cb)
}

#[no_mangle]
/// cross-platform documents directory path with '/' slashes
pub extern "C" fn ceammc_path_documents(cb: ceammc_path_str_cb) -> bool {
    path_to_string(dirs::document_dir(), cb)
}

#[no_mangle]
pub extern "C" fn ceammc_path_downloads(cb: ceammc_path_str_cb) -> bool {
    path_to_string(dirs::download_dir(), cb)
}

#[no_mangle]
/// cross-platform tmp directory path with '/' slashes
pub extern "C" fn ceammc_path_tmp(cb: ceammc_path_str_cb) -> bool {
    path_to_string(Some(std::env::temp_dir()), cb)
}

#[no_mangle]
/// cross-platform current working directory path with '/' slashes
pub extern "C" fn ceammc_path_cwd(cb: ceammc_path_str_cb) -> bool {
    path_to_string(std::env::current_dir().ok(), cb)
}
