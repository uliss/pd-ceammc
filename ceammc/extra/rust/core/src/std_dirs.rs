use path_slash::PathBufExt as _;
use std::{env, ffi::CString, path::PathBuf};

use dirs;
use log::{error, warn};

use crate::rust_str_cb;

fn output_dir(path: PathBuf, cb: rust_str_cb) -> bool {
    if let Ok(str) = CString::new(path.to_slash_lossy().as_ref()) {
        (cb.cb)(cb.user, str.as_ptr());
        true
    } else {
        false
    }
}

fn dir_to_string(path: Option<PathBuf>, cb: rust_str_cb) -> bool {
    match path {
        Some(path) => {
            if !path.exists() {
                error!("directory not exists: '{}'", path.to_string_lossy());
                return false;
            }

            match path.canonicalize() {
                Ok(new_path) => output_dir(new_path, cb),
                Err(err) => {
                    warn!("can't canonicalize directory path: {err}");
                    output_dir(path, cb)
                }
            }
        }
        None => {
            error!("directory not found");
            false
        }
    }
}

#[no_mangle]
/// cross-platform home directory path with '/' slashes
pub extern "C" fn ceammc_home_dir(cb: rust_str_cb) -> bool {
    dir_to_string(dirs::home_dir(), cb)
}

#[no_mangle]
/// cross-platform pd user directory path with '/' slashes
pub extern "C" fn ceammc_pd_user_dir(cb: rust_str_cb) -> bool {
    let path = dirs::home_dir().map(|mut path| {
        path.push("Documents");
        path.push("Pd");
        path
    });
    dir_to_string(path, cb)
}

#[no_mangle]
/// cross-platform puredata doc directory path with '/' slashes
/// using PD_DOC env variable
pub extern "C" fn ceammc_pd_doc_dir(cb: rust_str_cb) -> bool {
    match env::var("PD_DOC") {
        Ok(var) => dir_to_string(Some(PathBuf::from(var)), cb),
        Err(err) => {
            error!("{err}");
            false
        }
    }
}

#[no_mangle]
/// cross-platform ceammc external doc directory path with '/' slashes
/// using CEAMMC_DOC env variable
pub extern "C" fn ceammc_ext_doc_dir(cb: rust_str_cb) -> bool {
    let path = env::var("CEAMMC_DOC").ok().map(|var| PathBuf::from(var));
    dir_to_string(path, cb)
}

#[no_mangle]
/// cross-platform desktop directory path with '/' slashes
pub extern "C" fn ceammc_desktop_dir(cb: rust_str_cb) -> bool {
    dir_to_string(dirs::desktop_dir(), cb)
}

#[no_mangle]
/// cross-platform audio directory path with '/' slashes
pub extern "C" fn ceammc_audio_dir(cb: rust_str_cb) -> bool {
    dir_to_string(dirs::audio_dir(), cb)
}

#[no_mangle]
/// cross-platform video directory path with '/' slashes
pub extern "C" fn ceammc_video_dir(cb: rust_str_cb) -> bool {
    dir_to_string(dirs::video_dir(), cb)
}

#[no_mangle]
/// cross-platform images directory path with '/' slashes
pub extern "C" fn ceammc_image_dir(cb: rust_str_cb) -> bool {
    dir_to_string(dirs::picture_dir(), cb)
}

#[no_mangle]
/// cross-platform documents directory path with '/' slashes
pub extern "C" fn ceammc_document_dir(cb: rust_str_cb) -> bool {
    dir_to_string(dirs::document_dir(), cb)
}

#[no_mangle]
pub extern "C" fn ceammc_download_dir(cb: rust_str_cb) -> bool {
    dir_to_string(dirs::download_dir(), cb)
}

#[no_mangle]
/// cross-platform tmp directory path with '/' slashes
pub extern "C" fn ceammc_tmp_dir(cb: rust_str_cb) -> bool {
    dir_to_string(Some(std::env::temp_dir()), cb)
}

#[no_mangle]
/// cross-platform current working directory path with '/' slashes
pub extern "C" fn ceammc_cwd_dir(cb: rust_str_cb) -> bool {
    dir_to_string(std::env::current_dir().ok(), cb)
}
