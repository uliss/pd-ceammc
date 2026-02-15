use crate::vlc::Vlc;

use log::error;
use serde::Deserialize;
use std::ffi::{c_char, c_void, CStr};
use strum_macros::{EnumIter, EnumMessage, EnumString};

#[allow(non_camel_case_types)]
pub struct vlc {
    imp: Vlc,
}

#[allow(non_camel_case_types)]
#[derive(Debug, PartialEq, EnumString, EnumIter, EnumMessage)]
#[repr(C)]
pub enum vlc_sort {
    #[strum(serialize = "Id", serialize = "0", ascii_case_insensitive)]
    Id,
    #[strum(serialize = "Name", serialize = "1", ascii_case_insensitive)]
    Name,
    #[strum(serialize = "Author", serialize = "3", ascii_case_insensitive)]
    Author,
    #[strum(serialize = "Random", serialize = "5", ascii_case_insensitive)]
    Random,
    #[strum(serialize = "Track", serialize = "7", ascii_case_insensitive)]
    Track,
}

#[allow(non_camel_case_types)]
#[derive(Debug, EnumMessage)]
#[repr(C)]
pub enum vlc_sort_order {
    #[strum(serialize = "0")]
    Normal,
    #[strum(serialize = "1")]
    Reversed,
}

#[derive(Debug, Default, Deserialize)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub struct vlc_status {
    #[serde(default)]
    pub apiversion: u8,
    #[serde(default)]
    pub has_loop: bool,
    #[serde(default)]
    pub repeat: bool,
    #[serde(default)]
    pub random: bool,
    #[serde(deserialize_with = "deserialize_bool_or_false")]
    pub fullscreen: bool,
}

fn deserialize_bool_or_false<'de, D>(deserializer: D) -> Result<bool, D::Error>
where
    D: serde::Deserializer<'de>,
{
    match serde_json::Value::deserialize(deserializer) {
        Ok(value) => match value {
            serde_json::Value::Null => Ok(false),
            serde_json::Value::Bool(value) => Ok(value),
            serde_json::Value::Number(number) => Ok(number.as_u64().unwrap_or(0) != 0),
            _ => Ok(false),
        },
        Err(_) => Ok(false),
    }
}

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Clone, Copy)]
pub struct vlc_status_cb {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, status: &vlc_status)>,
}

impl vlc_status_cb {
    pub fn exec(&self, status: &vlc_status) {
        self.cb.map(|cb| cb(self.user, status));
    }
}

#[no_mangle]
pub extern "C" fn ceammc_vlc_create(
    host: Option<&std::ffi::c_char>,
    port: u16,
    pass: Option<&std::ffi::c_char>,
    cb: crate::common_ffi::callback_msg,
    notify: crate::common_ffi::callback_notify,
) -> *mut vlc {
    return Box::into_raw(Box::new(vlc {
        imp: Vlc::new(host, port, pass, cb, notify),
    }));
}

#[no_mangle]
pub extern "C" fn ceammc_vlc_free(vlc: *mut vlc) {
    if !vlc.is_null() {
        drop(unsafe { Box::from_raw(vlc) });
    }
}

#[no_mangle]
pub extern "C" fn ceammc_vlc_poll(
    vlc: Option<&mut vlc>,
    on_msg: crate::common_ffi::callback_msg,
    on_stat: vlc_status_cb,
) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    let vlc = &mut vlc.unwrap().imp;
    vlc.poll(on_msg, on_stat);
    return true;
}

#[no_mangle]
pub extern "C" fn ceammc_vlc_next(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    let vlc = &mut vlc.unwrap().imp;
    return vlc.send_next();
}

#[no_mangle]
pub extern "C" fn ceammc_vlc_prev(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    let vlc = &mut vlc.unwrap().imp;
    return vlc.send_prev();
}

#[no_mangle]
// @param vlc - vlc control handle
// @param id - pointer to track index, can be NULL
pub extern "C" fn ceammc_vlc_play(vlc: Option<&mut vlc>, id: *const i16) -> bool {
    let id = if id.is_null() {
        None
    } else {
        Some(unsafe { *id })
    };

    vlc.and_then(|vlc| Some(vlc.imp.send_play(id)))
        .or_else(|| {
            error!("NULL vlc pointer");
            Some(false)
        })
        .unwrap_or_default()
}

#[no_mangle]
pub extern "C" fn ceammc_vlc_stop(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    let vlc = &mut vlc.unwrap().imp;
    return vlc.send_stop();
}

#[no_mangle]
/// clear current playlist
/// @param vlc - vlc control handle
pub extern "C" fn ceammc_vlc_clear(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    let vlc = &mut vlc.unwrap().imp;
    return vlc.send_clear();
}

#[no_mangle]
/// control vlc fullscreen mode
/// @param vlc - vlc control handle
/// @param value - fullscreen mode, if NULL toggles
pub extern "C" fn ceammc_vlc_fullscreen(vlc: Option<&mut vlc>, value: *const bool) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    let vlc = &mut vlc.unwrap().imp;
    return vlc.send_fullscreen(if value.is_null() {
        None
    } else {
        Some(unsafe { *value })
    });
}

#[no_mangle]
/// clear current playlist
/// @param vlc - vlc control handle
pub extern "C" fn ceammc_vlc_get_status(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    let vlc = &mut vlc.unwrap().imp;
    return vlc.get_status();
}

#[no_mangle]
/// sort playlist
/// @param vlc - vlc control handle
/// @param sort - sort field number, not NULL!
/// @param mode - sort mode: normal or reversed
pub extern "C" fn ceammc_vlc_sort(
    vlc: Option<&mut vlc>,
    sort: &c_char,
    mode: vlc_sort_order,
) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    let vlc = &mut vlc.unwrap().imp;
    return vlc.send_sort(
        unsafe { CStr::from_ptr(sort) }
            .to_string_lossy()
            .to_string(),
        mode,
    );
}
