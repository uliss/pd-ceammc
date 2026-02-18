use std::ffi::{c_char, CStr};

use log::{error, info, warn};
use serde::{Deserialize, Serialize};

const CONFIG_APP_NAME: &str = "org.ceam.puredata.external";

#[derive(Debug, Default, Serialize, Deserialize)]
#[repr(C)]
#[allow(non_camel_case_types)]
pub enum config_lang {
    #[default]
    Default,
    English,
    Russian,
}

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Default, Debug, Serialize, Deserialize)]
pub struct config {
    doc_lang: config_lang,
}

#[no_mangle]
/// load config from filesystem
/// @return pointer to config or NULL on error
pub extern "C" fn ceammc_config_load() -> *mut config {
    let config: Result<config, _> = confy::load(CONFIG_APP_NAME, None);
    match config {
        Ok(config) => Box::into_raw(Box::new(config)),
        Err(err) => {
            error!("{err}");
            return std::ptr::null_mut();
        }
    }
}

#[no_mangle]
/// free config
/// @return true on success, false on error
pub extern "C" fn ceammc_config_free(config: Option<&mut config>) -> bool {
    match config {
        Some(config) => {
            println!("load: {config:?}");
            drop(unsafe { Box::from_raw(config) });
            true
        }
        None => {
            error!("NULL config pointer");
            false
        }
    }
}

#[no_mangle]
/// store config
/// @param config - not NULL
/// @return true on success, false on error
pub extern "C" fn ceammc_config_store(config: &mut config) -> bool {
    if let Err(err) = confy::store(CONFIG_APP_NAME, None, &config) {
        info!("{err}");
        false
    } else {
        info!("store: {config:?}");
        true
    }
}

#[no_mangle]
/// dump config std output
/// @param config - not NULL
pub extern "C" fn ceammc_config_dump(config: &config) -> bool {
    println!("{:?}", config);
    true
}

#[no_mangle]
/// parse c-string and get language
/// @param str_lang - c-string
/// @param lang - not NULL
/// @return true on success, false on error
pub extern "C" fn ceammc_config_parse_lang(
    str_lang: *const c_char,
    lang: &mut config_lang,
) -> bool {
    if str_lang.is_null() {
        error!("NULL c-string");
        return false;
    }

    let str = unsafe { CStr::from_ptr(str_lang) }
        .to_string_lossy()
        .to_string();

    match str.as_str() {
        "ru" => {
            *lang = config_lang::Russian;
            true
        }
        "en" => {
            *lang = config_lang::English;
            true
        }
        "def" => {
            *lang = config_lang::Default;
            true
        }
        _ => {
            warn!("unknown language code: {str}");
            false
        }
    }
}
