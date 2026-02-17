use crate::{rust_atom, vlc::Vlc};

use log::error;
use serde::Deserialize;
use std::ffi::{c_char, c_void, CStr};
use strum_macros::{EnumIter, EnumMessage, EnumString};

#[allow(non_camel_case_types)]
/// vlc control handle
pub struct vlc {
    imp: Vlc,
}

#[allow(non_camel_case_types)]
#[derive(Debug, PartialEq, EnumString, EnumIter, EnumMessage)]
#[repr(C)]
pub enum vlc_sort {
    duration,
    genre,
    artist,
    random,
    title,
    albom,
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

#[allow(non_camel_case_types)]
#[derive(Debug, Default, PartialEq)]
#[repr(u8)]
/// vlc playing state
pub enum vlc_state {
    #[default]
    Unknown,
    Stopped,
    Paused,
    Playing,
}

impl vlc_state {
    pub fn do_pause(self: &Self, pause: bool) -> bool {
        pause && self == &vlc_state::Playing
    }

    pub fn do_resume(self: &Self, pause: bool) -> bool {
        !pause && self != &vlc_state::Playing
    }
}

#[derive(Debug, Default, Deserialize)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub struct vlc_status {
    #[serde(default)]
    pub position: f64,
    #[serde(default)]
    pub volume: f64,
    #[serde(default)]
    pub time: f64,
    #[serde(default)]
    pub length: f64,
    #[serde(default)]
    pub rate: f64,
    #[serde(deserialize_with = "deserialize_state")]
    pub state: vlc_state,
    #[serde(default)]
    pub apiversion: u8,
    #[serde(default, rename = "loop")]
    pub has_loop: bool,
    #[serde(default)]
    pub repeat: bool,
    #[serde(default)]
    pub random: bool,
    #[serde(deserialize_with = "deserialize_bool_or_false")]
    pub fullscreen: bool,
    #[serde(default)]
    currentplid: u64,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct vlc_playlist_item {
    pub name: *const c_char,
    pub uri: *const c_char,
    pub id: u64,
    pub duration: u64,
    pub current: bool,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct vlc_playlist_item_cb {
    /// user data
    user: *mut c_void,
    /// callback function
    cb: Option<extern "C" fn(user: *mut c_void, item: &vlc_playlist_item)>,
}

impl vlc_playlist_item_cb {
    pub fn exec(&self, item: &vlc_playlist_item) {
        self.cb.map(|cb| cb(self.user, item));
    }
}

#[no_mangle]
/// iterate all playlist items with given callback
/// @param items - pointer to items
/// @param size - playlist item count
/// @param cb - callback called for each item
pub extern "C" fn ceammc_vlc_playlist_iter(
    items: *const vlc_playlist_item,
    len: usize,
    cb: vlc_playlist_item_cb,
) {
    if items.is_null() {
        return;
    }

    let items = unsafe { std::slice::from_raw_parts(items, len) };
    for x in items.iter() {
        cb.exec(x);
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct vlc_playlist {
    pub size: usize,
    pub items: *const vlc_playlist_item,
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

fn deserialize_state<'de, D>(deserializer: D) -> Result<vlc_state, D::Error>
where
    D: serde::Deserializer<'de>,
{
    Ok(if let Ok(state) = String::deserialize(deserializer) {
        match state.as_str() {
            "stopped" => vlc_state::Stopped,
            "paused" => vlc_state::Paused,
            "playing" => vlc_state::Playing,
            _ => vlc_state::default(),
        }
    } else {
        vlc_state::default()
    })
}

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Clone, Copy)]
pub struct vlc_status_cb {
    /// user data
    user: *mut c_void,
    /// callback function
    cb: Option<extern "C" fn(user: *mut c_void, status: &vlc_status)>,
}

impl vlc_status_cb {
    pub fn exec(&self, status: &vlc_status) {
        self.cb.map(|cb| cb(self.user, status));
    }
}

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Clone, Copy)]
pub struct vlc_playlist_cb {
    /// user data
    user: *mut c_void,
    /// callback function
    cb: Option<extern "C" fn(user: *mut c_void, playlist: &vlc_playlist)>,
}

impl vlc_playlist_cb {
    pub fn exec(&self, playlist: &vlc_playlist) {
        self.cb.map(|cb| cb(self.user, playlist));
    }
}

#[no_mangle]
/// create vlc control handle
/// @param host - vlc host name or ip address, if NULL use localhost
/// @param port - vlc http remote port
/// @param pass - vlc http password
/// @param notify - notification callback
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
/// free vlc handle
/// @param vlc - vlc control handle
pub extern "C" fn ceammc_vlc_free(vlc: *mut vlc) {
    if !vlc.is_null() {
        drop(unsafe { Box::from_raw(vlc) });
    }
}

#[no_mangle]
/// get incoming messages from vlc
/// @param vlc - vlc control handle
/// @param on_msg - error message callback from worker thread
/// @param on_stat - vlc status callback
/// @param on_playlist - vlc playlist callback
pub extern "C" fn ceammc_vlc_poll(
    vlc: Option<&mut vlc>,
    on_msg: crate::common_ffi::callback_msg,
    on_stat: vlc_status_cb,
    on_playlist: vlc_playlist_cb,
    on_current: vlc_playlist_item_cb,
) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap()
        .imp
        .poll(on_msg, on_stat, on_playlist, on_current)
}

#[no_mangle]
/// go to next item in the playlist and play it
/// @param vlc - vlc control handle
pub extern "C" fn ceammc_vlc_next(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.playlist_next()
}

#[no_mangle]
/// go to previous item in the playlist and play it
/// @param vlc - vlc control handle
pub extern "C" fn ceammc_vlc_prev(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.playlisy_prev()
}

#[no_mangle]
/// play playlist item
/// @param vlc - vlc control handle
/// @param id - pointer to track index, can be NULL
pub extern "C" fn ceammc_vlc_play(vlc: Option<&mut vlc>, id: Option<&i16>) -> bool {
    vlc.and_then(|vlc| Some(vlc.imp.play(id.map(|x| *x))))
        .or_else(|| {
            error!("NULL vlc pointer");
            Some(false)
        })
        .unwrap_or_default()
}

#[no_mangle]
/// stop vlc playback
/// @param vlc - vlc control handle
pub extern "C" fn ceammc_vlc_stop(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.stop()
}

#[no_mangle]
/// pause vlc playback
/// @param vlc - vlc control handle
/// @param value - if true, pauses playback, otherwise resume. If NULL toggles pause mode
pub extern "C" fn ceammc_vlc_pause(vlc: Option<&mut vlc>, value: Option<&bool>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.pause(value.map(|x| *x))
}

#[no_mangle]
/// clear current playlist
/// @param vlc - vlc control handle
pub extern "C" fn ceammc_vlc_clear(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.clear_playlist()
}

#[no_mangle]
/// set vlc fullscreen mode
/// @param vlc - vlc control handle
/// @param value - fullscreen mode, if NULL toggles fullscreen
pub extern "C" fn ceammc_vlc_fullscreen(vlc: Option<&mut vlc>, value: Option<&bool>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.set_fullscreen(value.map(|x| *x))
}

#[no_mangle]
/// set vlc loop mode
/// @param vlc - vlc control handle
/// @param value - loop value, if NULL toggles loop mode
pub extern "C" fn ceammc_vlc_loop(vlc: Option<&mut vlc>, value: Option<&bool>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.set_loop(value.map(|x| *x))
}

#[no_mangle]
/// set vlc repeat mode
/// @param vlc - vlc control handle
/// @param value - repeat value, if NULL toggles repeat mode
pub extern "C" fn ceammc_vlc_repeat(vlc: Option<&mut vlc>, value: Option<&bool>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.set_repeat(value.map(|x| *x))
}

#[no_mangle]
/// set vlc volume
/// supported syntax:[+, 10], [-, 10], [100, None] etc.
/// @param vlc - vlc control handle
/// @param v0 - first arg
/// @param v1 - second arg
pub extern "C" fn ceammc_vlc_volume(vlc: Option<&mut vlc>, v0: rust_atom, v1: rust_atom) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.set_volume(v0, v1)
}

#[no_mangle]
/// set vlc playback rate
/// @param vlc - vlc control handle
/// @param rate - vlc playback rate
pub extern "C" fn ceammc_vlc_playback_rate(vlc: Option<&mut vlc>, rate: f32) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.set_playback_rate(rate)
}

#[no_mangle]
/// add URI into vlc playlist
/// @param vlc - vlc control handle
/// @param uri - resource URI
/// @param play - should immidiately play added url
pub extern "C" fn ceammc_vlc_add_uri(
    vlc: Option<&mut vlc>,
    uri: Option<&c_char>,
    play: bool,
) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.add_uri(uri, play)
}

#[no_mangle]
/// seek to specified time
/// @param vlc - vlc control handle
/// @param seek - seek time value
pub extern "C" fn ceammc_vlc_seek(vlc: Option<&mut vlc>, seek: rust_atom) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.seek(seek.as_safe_value())
}

#[no_mangle]
/// remove item from playlist
/// @param vlc - vlc control handle
/// @param name - playlist item name
pub extern "C" fn ceammc_vlc_delete_by_name(vlc: Option<&mut vlc>, name: rust_atom) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.delete_by_name(name.as_safe_value())
}

#[no_mangle]
/// remove item from playlist
/// @param vlc - vlc control handle
/// @param pos - playlist item position
pub extern "C" fn ceammc_vlc_delete_at_pos(vlc: Option<&mut vlc>, pos: i32) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.delete_at_pos(pos)
}

#[no_mangle]
/// remove item from playlist
/// @param vlc - vlc control handle
/// @param id - playlist item ID (not index position!)
pub extern "C" fn ceammc_vlc_delete_by_id(vlc: Option<&mut vlc>, id: u64) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.delete_by_id(id)
}

#[no_mangle]
/// get current vlc status
/// @param vlc - vlc control handle
pub extern "C" fn ceammc_vlc_get_status(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.get_status()
}

#[no_mangle]
/// get vlc playlist
/// @param vlc - vlc control handle
pub extern "C" fn ceammc_vlc_get_playlist(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.get_playlist()
}

#[no_mangle]
/// get current playlist item
/// @param vlc - vlc control handle
pub extern "C" fn ceammc_vlc_get_current(vlc: Option<&mut vlc>) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.get_current()
}

#[no_mangle]
/// browse vlc directories
/// @param vlc - vlc control handle
pub extern "C" fn ceammc_vlc_browse(
    vlc: Option<&mut vlc>,
    uri: Option<&c_char>,
    filter_type: Option<&c_char>,
    match_glob: Option<&c_char>,
) -> bool {
    if vlc.is_none() {
        error!("NULL vlc pointer");
        return false;
    }

    vlc.unwrap().imp.browse(uri, filter_type, match_glob)
}

#[no_mangle]
/// sort playlist
/// @param vlc - vlc control handle
/// @param sort - sort field, not NULL!
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
