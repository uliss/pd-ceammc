use std::{
    ffi::{c_char, c_void, CString},
    ptr::null,
};

use log::warn;

use crate::common_ffi::callback_msg;

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct obs_init {
    pub host: *const c_char,
    pub password: *const c_char,
    pub port: u16,
}

#[derive(Debug)]
#[allow(non_camel_case_types)]
pub struct obs_version {
    obs_major: u64,
    obs_minor: u64,
    obs_patch: u64,
    ws_major: u64,
    ws_minor: u64,
    ws_patch: u64,
    rpc_version: u32,
    platform: CString,
    platform_desc: CString,
    image_formats: Vec<CString>,
}

/// get OBS version
/// @param v - pointer to version struct
/// @param major - pointer to store major version data
/// @param minor - pointer to store minor version data
/// @param patch - pointer to store patch version data
#[no_mangle]
pub extern "C" fn ceammc_obs_version_server(
    v: &obs_version,
    major: &mut u64,
    minor: &mut u64,
    patch: &mut u64,
) {
    *major = v.obs_major;
    *minor = v.obs_minor;
    *patch = v.obs_patch;
}

/// get OBS Web Socket version
/// @param v - pointer to version struct
/// @param major - pointer to store major version data
/// @param minor - pointer to store minor version data
/// @param patch - pointer to store patch version data
#[no_mangle]
pub extern "C" fn ceammc_obs_version_websocket(
    v: &obs_version,
    major: &mut u64,
    minor: &mut u64,
    patch: &mut u64,
) {
    *major = v.ws_major;
    *minor = v.ws_minor;
    *patch = v.ws_patch;
}

/// get RPC OBS version
/// @param v - pointer to version struct
#[no_mangle]
pub extern "C" fn ceammc_obs_version_rpc(v: &obs_version) -> u32 {
    v.rpc_version
}

/// get OBS platform
/// @param v - pointer to version struct
#[no_mangle]
pub extern "C" fn ceammc_obs_version_platform(v: &obs_version) -> *const c_char {
    v.platform.as_ptr()
}

/// get OBS platform description
/// @param v - pointer to version struct
#[no_mangle]
pub extern "C" fn ceammc_obs_version_platform_desc(v: &obs_version) -> *const c_char {
    v.platform_desc.as_ptr()
}

/// get OBS image format count
/// @param v - pointer to version struct
#[no_mangle]
pub extern "C" fn ceammc_obs_version_image_fmt_num(v: &obs_version) -> usize {
    v.image_formats.len()
}

/// get OBS image format at
/// @param v - pointer to version struct
/// @param idx - image format index
#[no_mangle]
pub extern "C" fn ceammc_obs_version_image_fmt_at(v: &obs_version, idx: usize) -> *const c_char {
    v.image_formats
        .get(idx)
        .unwrap_or(&CString::default())
        .as_ptr()
}

impl From<obws::responses::general::Version> for obs_version {
    fn from(version: obws::responses::general::Version) -> Self {
        let platform = CString::new(version.platform.as_str()).unwrap();
        let platform_desc = CString::new(version.platform_description.as_str()).unwrap();
        let sup_img = version
            .supported_image_formats
            .iter()
            .map(|f| CString::new(f.as_str()).unwrap())
            .collect::<Vec<_>>();

        return Self {
            obs_major: version.obs_version.major,
            obs_minor: version.obs_version.minor,
            obs_patch: version.obs_version.patch,
            ws_major: version.obs_web_socket_version.major,
            ws_minor: version.obs_web_socket_version.minor,
            ws_patch: version.obs_web_socket_version.patch,
            rpc_version: version.rpc_version,
            platform,
            platform_desc,
            image_formats: sup_img,
        };
    }
}

#[allow(non_camel_case_types)]
#[derive(Debug)]
pub struct obs_scene {
    name: CString,
    uuid: CString,
}

impl obs_scene {
    fn new(name: &String, uuid: &String) -> Self {
        let name = CString::new(name.as_str()).unwrap_or_default();
        let uuid = CString::new(uuid.as_str()).unwrap_or_default();
        Self { name, uuid }
    }
}

/// get scene name as C-string
/// @param scene - pointer to scene (not NULL!)
#[no_mangle]
pub extern "C" fn ceammc_obs_scene_name(scene: &obs_scene) -> *const c_char {
    scene.name.as_ptr()
}

/// get scene UUID as C-string
/// @param scene - pointer to scene (not NULL!)
#[no_mangle]
pub extern "C" fn ceammc_obs_scene_uuid(scene: &obs_scene) -> *const c_char {
    scene.uuid.as_ptr()
}

impl Default for obs_scene {
    fn default() -> Self {
        Self::new(&Default::default(), &Default::default())
    }
}

#[allow(non_camel_case_types)]
#[derive(Debug)]
pub struct obs_scene_list {
    current: obs_scene,
    scenes: Vec<obs_scene>,
}

impl From<obws::responses::scenes::Scenes> for obs_scene_list {
    fn from(sc: obws::responses::scenes::Scenes) -> Self {
        let current = sc
            .current_program_scene
            .map(|x| obs_scene::new(&x.name, &x.uuid.to_string()))
            .unwrap_or_default();

        let scenes = sc
            .scenes
            .iter()
            .map(|x| obs_scene::new(&x.id.name, &x.id.uuid.to_string()))
            .collect::<Vec<_>>();

        obs_scene_list { current, scenes }
    }
}

/// get current scene
/// @param scenes - pointer to scenes (not NULL!)
#[no_mangle]
pub extern "C" fn ceammc_obs_scene_current(scl: &obs_scene_list) -> &obs_scene {
    &scl.current
}

/// get scene list length
/// @param scenes - pointer to scenes (not NULL!)
#[no_mangle]
pub extern "C" fn ceammc_obs_scene_list_length(scl: &obs_scene_list) -> usize {
    scl.scenes.len()
}

/// get scene list data
/// @param scenes - pointer to scenes (not NULL!)
#[no_mangle]
pub extern "C" fn ceammc_obs_scene_list_at(scl: &obs_scene_list, idx: usize) -> *const obs_scene {
    scl.scenes
        .get(idx)
        .map(|x| x as *const obs_scene)
        .unwrap_or(null())
}

#[derive(Debug)]
#[allow(non_camel_case_types)]
pub struct obs_monitor {
    index: u32,
    w: u16,
    h: u16,
    x: u16,
    y: u16,
    name: CString,
}

#[no_mangle]
/// get monitor name
/// @param m - pointer to monitor (not NULL!)
pub extern "C" fn ceammc_obs_monitor_name(m: &obs_monitor) -> *const c_char {
    m.name.as_ptr()
}

#[no_mangle]
/// get monitor index
/// @param m - pointer to monitor (not NULL!)
pub extern "C" fn ceammc_obs_monitor_index(m: &obs_monitor) -> u32 {
    m.index
}

#[no_mangle]
/// get monitor geometry
/// @param m - pointer to monitor (not NULL!)
/// @param x - pointer to store x coord
/// @param y - pointer to store y coord
/// @param w - pointer to store monitor width
/// @param h - pointer to store monitor height
pub extern "C" fn ceammc_obs_monitor_geom(
    m: &obs_monitor,
    x: &mut u16,
    y: &mut u16,
    w: &mut u16,
    h: &mut u16,
) {
    *x = m.x;
    *y = m.y;
    *w = m.w;
    *h = m.h;
}

#[no_mangle]
/// get monitor at specified position
/// @param ml - pointer to monitor list (not NULL!)
/// @return pointer to monitor or nullptr if not found
pub extern "C" fn ceammc_obs_monitor_at(ml: &obs_monitor_list, idx: usize) -> *const obs_monitor {
    ml.mons
        .get(idx)
        .map(|x| x as *const obs_monitor)
        .unwrap_or(null())
}

#[no_mangle]
/// get monitor list length
/// @param ml - pointer to monitor list (not NULL!)
pub extern "C" fn ceammc_obs_monitor_count(ml: &obs_monitor_list) -> usize {
    ml.mons.len()
}

#[derive(Debug)]
#[allow(non_camel_case_types)]
pub struct obs_monitor_list {
    mons: Vec<obs_monitor>,
}

impl From<Vec<obws::responses::ui::Monitor>> for obs_monitor_list {
    fn from(mons: Vec<obws::responses::ui::Monitor>) -> Self {
        Self {
            mons: mons
                .iter()
                .map(|m| obs_monitor {
                    index: m.index,
                    w: m.size.width,
                    h: m.size.height,
                    x: m.position.x,
                    y: m.position.y,
                    name: CString::new(m.name.as_str()).unwrap_or_default(),
                })
                .collect::<Vec<_>>(),
        }
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct obs_result_cb {
    /// user data pointer (can be NULL)
    user: *mut c_void,
    /// version callback function (can be NULL)
    cb_version: Option<extern "C" fn(user: *mut c_void, ver: &obs_version)>,
    /// scenes list callback function (can be NULL)
    cb_scene_list: Option<extern "C" fn(user: *mut c_void, scl: &obs_scene_list)>,
    /// monitor list callback function (can be NULL)
    cb_monitor_list: Option<extern "C" fn(user: *mut c_void, mons: *const obs_monitor_list)>,
    /// current scene callback function (can be NULL)
    cb_current_scene: Option<extern "C" fn(user: *mut c_void, name: *const c_char)>,
    /// connected/disconnected callback function (can be NULL)
    cb_connected: Option<extern "C" fn(user: *mut c_void, state: bool)>,
}

impl obs_result_cb {
    fn connected(&self) {
        match self.cb_connected {
            Some(cb) => cb(self.user, true),
            None => warn!("cb_connected callback is not set"),
        }
    }

    fn current_scene(&self, name: &CString) {
        if let Some(cb) = self.cb_current_scene {
            cb(self.user, name.as_ptr());
        }
    }

    fn monitor_list(&self, mons: obs_monitor_list) {
        match self.cb_monitor_list {
            Some(cb) => {
                cb(self.user, &mons);
            }
            None => warn!("cb_monitors callback is not set"),
        }
    }

    fn scene_list(&self, scenes: obs_scene_list) {
        match self.cb_scene_list {
            Some(cb) => {
                cb(self.user, &scenes);
            }
            None => warn!("cb_scenes callback is not set"),
        }
    }

    fn version(&self, version: &obs_version) {
        match self.cb_version {
            Some(cb) => {
                cb(self.user, version);
            }
            None => warn!("cb_version callback is not set"),
        }
    }
}

#[allow(non_camel_case_types)]
pub struct obs_client {
    send: tokio::sync::mpsc::Sender<OBSRequest>,
    recv: tokio::sync::mpsc::Receiver<Result<OBSReply, crate::common_ffi::Error>>,
    cb_err: callback_msg,
    cb_post: callback_msg,
    cb_debug: callback_msg,
    cb_log: callback_msg,
    cb_reply: obs_result_cb,
}

impl obs_client {
    pub fn new(
        send: tokio::sync::mpsc::Sender<OBSRequest>,
        recv: tokio::sync::mpsc::Receiver<Result<OBSReply, crate::common_ffi::Error>>,
        cb_err: callback_msg,
        cb_post: callback_msg,
        cb_debug: callback_msg,
        cb_log: callback_msg,
        cb_reply: obs_result_cb,
    ) -> Self {
        Self {
            send,
            recv,
            cb_err,
            cb_post,
            cb_debug,
            cb_log,
            cb_reply,
        }
    }

    pub fn blocking_send(&self, request: OBSRequest) -> Result<bool, String> {
        self.send
            .blocking_send(request)
            .map(|_| true)
            .map_err(|err| err.to_string())
    }

    pub fn process_events(&mut self) {
        use crate::common_ffi::Error;

        while let Ok(rec) = self.recv.try_recv() {
            match rec {
                Ok(reply) => match reply {
                    OBSReply::Version(version) => self.cb_reply.version(&version),
                    OBSReply::Connected => self.cb_reply.connected(),
                    OBSReply::ListScenes(scenes) => self.cb_reply.scene_list(scenes),
                    OBSReply::ListMonitors(vec) => self.cb_reply.monitor_list(vec),
                    OBSReply::CurrentScene(cstr) => self.cb_reply.current_scene(&cstr),
                },
                Err(err) => match err {
                    Error::Error(msg) => self.cb_err.exec(msg.as_str()),
                    Error::Post(msg) => self.cb_post.exec(msg.as_str()),
                    Error::Debug(msg) => self.cb_debug.exec(msg.as_str()),
                    Error::Log(msg) => self.cb_log.exec(msg.as_str()),
                },
            }
        }
    }

    pub fn from_ptr<'a>(cli: *const obs_client) -> Result<&'a obs_client, &'static str> {
        if cli.is_null() {
            Err("null pointer")
        } else {
            Ok(unsafe { &*cli })
        }
    }
}

#[derive(Debug)]
pub enum OBSRequest {
    GetVersion,
    ListScenes,
    ListMonitors,
    GetCurrentScene,
    SetCurrentScene(String),
    ChangeSceneBy(i32),
    CreateScene(String),
    RemoveScene(String),
    FirstScene,
    LastScene,
    Close,
}

#[derive(Debug)]
pub enum OBSReply {
    Version(obs_version),
    ListScenes(obs_scene_list),
    ListMonitors(obs_monitor_list),
    CurrentScene(CString),
    Connected,
}
