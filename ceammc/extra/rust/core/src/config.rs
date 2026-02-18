use log::error;
use serde::{Deserialize, Serialize};

const CONFIG_APP_NAME: &str = "org.ceam.puredata.external";

#[derive(Debug, Default, Serialize, Deserialize)]
#[repr(C)]
#[allow(non_camel_case_types)]
pub enum ceammc_config_lang {
    #[default]
    Default,
    English,
    Russian,
}

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Default, Debug, Serialize, Deserialize)]
pub struct ceammc_config {
    doc_lang: ceammc_config_lang,
}

#[no_mangle]
/// load config from filesystem
/// @return pointer to config or NULL on error
pub extern "C" fn ceammc_config_load() -> *mut ceammc_config {
    let config: Result<ceammc_config, _> = confy::load(CONFIG_APP_NAME, None);
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
pub extern "C" fn ceammc_config_free(config: Option<&mut ceammc_config>) -> bool {
    match config {
        Some(config) => {
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
pub extern "C" fn ceammc_config_store(config: &mut ceammc_config) -> bool {
    if let Err(err) = confy::store(CONFIG_APP_NAME, None, &config) {
        error!("{err}");
        false
    } else {
        true
    }
}
