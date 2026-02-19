use std::{
    ffi::{c_char, CStr},
    str::FromStr,
};

use log::{error, info, warn};
use serde::{Deserialize, Serialize};

pub const CONFIG_APP_NAME: &str = "org.ceam.puredata.external";

#[derive(Debug, Default, Serialize, Deserialize)]
#[repr(C)]
#[allow(non_camel_case_types)]
pub enum config_lang {
    #[default]
    Default,
    English,
    Russian,
}

impl ToString for config_lang {
    fn to_string(&self) -> String {
        match self {
            config_lang::Default => "default",
            config_lang::English => "en",
            config_lang::Russian => "ru",
        }
        .to_owned()
    }
}

#[derive(Debug, PartialEq, Eq)]
pub enum ConfigErr {
    InvalidLang(String),
}

impl FromStr for config_lang {
    type Err = ConfigErr;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "default" => Ok(config_lang::Default),
            "ru" => Ok(config_lang::Russian),
            "en" => Ok(config_lang::English),
            _ => Err(ConfigErr::InvalidLang(format!("invalid lang code: '{s}'"))),
        }
    }
}

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Default, Debug, Serialize, Deserialize)]
pub struct config {
    pub doc_lang: config_lang,
}

#[no_mangle]
/// load config from filesystem
/// @return pointer to config or NULL on error
pub extern "C" fn ceammc_config_load() -> *mut config {
    match config_load() {
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
pub extern "C" fn ceammc_config_store(config: &config) -> bool {
    if let Err(err) = config_store(config) {
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

    match config_lang::from_str(
        unsafe { CStr::from_ptr(str_lang) }
            .to_string_lossy()
            .as_ref(),
    ) {
        Ok(l) => {
            *lang = l;
            true
        }
        Err(err) => {
            warn!("{err:?}");
            false
        }
    }
}

// for outer modules
#[allow(unused)]
pub fn config_load() -> Result<config, String> {
    confy::load(CONFIG_APP_NAME, None).map_err(|err| err.to_string())
}

// for outer modules
#[allow(unused)]
pub fn config_store(conf: &config) -> Result<(), String> {
    confy::store(CONFIG_APP_NAME, None, &conf).map_err(|err| err.to_string())
}
