use function_name::named;
use log::{debug, error, warn};
use std::ffi::{c_char, c_void, CStr, CString};
use std::ptr::{null, null_mut};

use obws::Client;

use crate::common_ffi::{callback_msg, callback_notify};
use crate::fn_error;

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct obs_init {
    host: *const c_char,
    password: *const c_char,
    port: u16,
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
    recv: tokio::sync::mpsc::Receiver<Result<OBSReply, Error>>,
    cb_err: callback_msg,
    cb_post: callback_msg,
    cb_debug: callback_msg,
    cb_log: callback_msg,
    cb_reply: obs_result_cb,
}

impl obs_client {
    fn blocking_send(&self, request: OBSRequest) -> Result<bool, String> {
        self.send
            .blocking_send(request)
            .map(|_| true)
            .map_err(|err| err.to_string())
    }

    fn from_ptr<'a>(cli: *const obs_client) -> Result<&'a obs_client, &'static str> {
        if cli.is_null() {
            Err("null pointer")
        } else {
            Ok(unsafe { &*cli })
        }
    }
}

fn str_from_cstr(str: *const c_char) -> Result<String, String> {
    if str.is_null() {
        Err("null string pointer".to_owned())
    } else {
        Ok(unsafe { CStr::from_ptr(str).to_str() }
            .map_err(|err| err.to_string())?
            .to_owned())
    }
}

#[derive(Debug)]
enum OBSRequest {
    GetVersion,
    ListScenes,
    ListMonitors,
    GetCurrentScene,
    SetCurrentScene(String),
    MoveScene(i32),
    CreateScene(String),
    RemoveScene(String),
    FirstScene,
    LastScene,
    Close,
}

#[derive(Debug)]
enum OBSReply {
    Version(obs_version),
    ListScenes(obs_scene_list),
    ListMonitors(obs_monitor_list),
    CurrentScene(CString),
    Connected,
}

#[derive(Debug)]
pub enum Error {
    Error(String),
    Post(String),
    Debug(String),
    Log(String),
}

async fn reply_send(
    cb: &callback_notify,
    rep_tx: &tokio::sync::mpsc::Sender<Result<OBSReply, Error>>,
    msg: OBSReply,
) {
    if let Err(err) = rep_tx.send(Ok(msg)).await {
        warn!("reply send error: {err}");
    } else {
        cb.exec();
    }
}

async fn reply_error(
    cb: &callback_notify,
    rep_tx: &tokio::sync::mpsc::Sender<Result<OBSReply, Error>>,
    msg: String,
) {
    debug!("error: {msg}");
    if let Err(err) = rep_tx.send(Err(Error::Error(msg))).await {
        warn!("error send error: {err}");
    } else {
        cb.exec();
    }
}

enum RequestResult {
    Reply(OBSReply),
    ReplyOk,
    Break,
}

async fn process_request(req: OBSRequest, cli: &mut Client) -> Result<RequestResult, String> {
    match req {
        OBSRequest::GetVersion => {
            return cli
                .general()
                .version()
                .await
                .map(|x| RequestResult::Reply(OBSReply::Version(x.into())))
                .map_err(|err| err.to_string());
        }
        OBSRequest::Close => {
            cli.disconnect().await;
            return Ok(RequestResult::Break);
        }
        OBSRequest::ListScenes => {
            return cli
                .scenes()
                .list()
                .await
                .map(|sc| RequestResult::Reply(OBSReply::ListScenes(sc.into())))
                .map_err(|err| err.to_string());
        }
        OBSRequest::GetCurrentScene => {
            return cli
                .scenes()
                .current_program_scene()
                .await
                .map(|scene| {
                    RequestResult::Reply(OBSReply::CurrentScene(
                        CString::new(scene.id.name.as_str()).unwrap(),
                    ))
                })
                .map_err(|err| err.to_string());
        }
        OBSRequest::SetCurrentScene(name) => {
            return cli
                .scenes()
                .set_current_program_scene(name.as_str())
                .await
                .map(|_| RequestResult::ReplyOk)
                .map_err(|err| err.to_string());
        }
        OBSRequest::ListMonitors => {
            return cli
                .ui()
                .list_monitors()
                .await
                .map(|x| RequestResult::Reply(OBSReply::ListMonitors(x.into())))
                .map_err(|err| err.to_string());
        }
        OBSRequest::MoveScene(offset) => {
            let cur = cli.scenes().list().await.map_err(|err| err.to_string())?;
            let name = cur.current_program_scene.unwrap().name;
            let mut iter = cur.scenes.iter();
            let idx = iter
                .find(|x| x.id.name == name)
                .take()
                .map_or(-1, |x| x.index as i32);

            if idx < 0 {
                return Err(String::from("no current scene is set"));
            }

            debug!("current scene: {name}");
            let move_idx = idx + offset;
            if move_idx < 0 && offset != 0 {
                return Err(String::from("first scene is reached"));
            } else if move_idx >= (cur.scenes.len() as i32) && offset != 0 {
                return Err(String::from("last scene is reached"));
            }

            let move_idx = move_idx.clamp(0, cur.scenes.len() as i32) as usize;
            let scene = cur
                .scenes
                .iter()
                .find(|f| f.index == move_idx)
                .ok_or(format!("index not found: {move_idx}"))?
                .id
                .name
                .as_str();

            cli.scenes()
                .set_current_program_scene(scene)
                .await
                .map(|_| RequestResult::ReplyOk)
                .map_err(|err| err.to_string())?;

            let new_scene = CString::new(scene).map_err(|e| e.to_string())?;
            return Ok(RequestResult::Reply(OBSReply::CurrentScene(new_scene)));
        }
        OBSRequest::CreateScene(name) => {
            return cli
                .scenes()
                .create(name.as_str().into())
                .await
                .map(|_| RequestResult::ReplyOk)
                .map_err(|e| e.to_string());
        }
        OBSRequest::RemoveScene(name) => {
            return cli
                .scenes()
                .remove(name.as_str().into())
                .await
                .map(|_| RequestResult::ReplyOk)
                .map_err(|e| e.to_string());
        }
        OBSRequest::FirstScene => {
            let first = cli
                .scenes()
                .list()
                .await
                .map_err(|e| e.to_string())?
                .scenes
                .first()
                .map(|x| x.id.clone())
                .ok_or("scene is empty")?;

            cli.scenes()
                .set_current_program_scene(first.clone())
                .await
                .map_err(|e| e.to_string())?;

            let new_scene = CString::new(first.name.as_str()).map_err(|e| e.to_string())?;
            return Ok(RequestResult::Reply(OBSReply::CurrentScene(new_scene)));
        }
        OBSRequest::LastScene => {
            let first = cli
                .scenes()
                .list()
                .await
                .map_err(|e| e.to_string())?
                .scenes
                .last()
                .map(|x| x.id.clone())
                .ok_or("scene is empty")?;

            cli.scenes()
                .set_current_program_scene(first.clone())
                .await
                .map_err(|e| e.to_string())?;

            let new_scene = CString::new(first.name.as_str()).map_err(|e| e.to_string())?;
            return Ok(RequestResult::Reply(OBSReply::CurrentScene(new_scene)));
        }
    }
}

/// create OSB Studio client
/// @param params - connection params
/// @param cb_err - callback for error messages
/// @param cb_post - callback for post messages
/// @param cb_debug - callback for debug messages
/// @param cb_log - callback for log messages
/// @param cb_reply - reply callbacks
/// @param cb_notify - notification callback
/// @return pointer to obs client or NULL on error
#[no_mangle]
pub extern "C" fn ceammc_obs_new(
    params: obs_init,
    cb_err: callback_msg,
    cb_post: callback_msg,
    cb_debug: callback_msg,
    cb_log: callback_msg,
    cb_reply: obs_result_cb,
    cb_notify: callback_notify,
) -> *mut obs_client {
    match tokio::runtime::Runtime::new() {
        Ok(rt) => {
            debug!("creating tokio runtime ...");

            let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<OBSRequest>(32);
            let (rep_tx, rep_rx) = tokio::sync::mpsc::channel::<Result<OBSReply, Error>>(32);

            let host = unsafe { CStr::from_ptr(params.host) }
                .to_str()
                .ok()
                .unwrap_or("localhost");
            let passwd = unsafe { CStr::from_ptr(params.password) }.to_str().ok();

            std::thread::spawn(move || {
                debug!("starting worker thread ...");
                rt.block_on(async move {
                    // connect
                    let p = params.port;
                    debug!("connecting to OBS: host={host}:{p}");

                    match Client::connect(host, params.port, passwd).await {
                        Ok(mut cli) => {
                            debug!("connected to OBS Studio ...");
                            reply_send(&cb_notify, &rep_tx, OBSReply::Connected).await;

                            debug!("starting runloop ...");
                            loop {
                                let req = req_rx.recv().await;
                                if req.is_none() {
                                    debug!("exit runloop ...");
                                    return ();
                                }

                                match process_request(req.unwrap(), &mut cli).await {
                                    Ok(res) => match res {
                                        RequestResult::Reply(obsreply) => {
                                            reply_send(&cb_notify, &rep_tx, obsreply).await;
                                        }
                                        RequestResult::ReplyOk => {}
                                        RequestResult::Break => {
                                            debug!("break runloop ...");
                                            return ();
                                        }
                                    },
                                    Err(err) => reply_error(&cb_notify, &rep_tx, err).await,
                                }
                            }
                        }
                        Err(err) => {
                            // connection error
                            reply_error(&cb_notify, &rep_tx, err.to_string()).await;
                        }
                    }
                });
                debug!("exit worker thread ...");
            });

            let cli: obs_client = obs_client {
                send: req_tx,
                recv: rep_rx,
                cb_err,
                cb_post,
                cb_debug,
                cb_log,
                cb_reply,
            };

            return Box::into_raw(Box::new(cli));
        }
        Err(err) => {
            cb_err.exec(format!("tokio runtime creation error: {err}").as_str());
            return null_mut();
        }
    }
}

/// free OBS Studio client
/// @param cli - pointer to obs client
#[no_mangle]
pub extern "C" fn ceammc_obs_free(cli: *mut obs_client) {
    if !cli.is_null() {
        let cli = unsafe { Box::from_raw(cli) };
        match cli.send.blocking_send(OBSRequest::Close) {
            Ok(_) => {}
            Err(err) => {
                error!("send error: {err}")
            }
        }
    }
}

fn obs_get_version(cli: *const obs_client) -> Result<bool, String> {
    let cli = obs_client::from_ptr(cli)?;
    cli.blocking_send(OBSRequest::GetVersion)
}

/// send version request to OBS studio
/// @param cli - pointer to obs client
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_get_version(cli: *const obs_client) -> bool {
    obs_get_version(cli)
        .map_err(|err| fn_error!("{}", err))
        .is_ok()
}

fn osb_list_scenes(cli: *const obs_client) -> Result<bool, String> {
    let cli = obs_client::from_ptr(cli)?;
    cli.blocking_send(OBSRequest::ListScenes)
}

/// send version request to OBS studio
/// @param cli - pointer to obs client
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_list_scenes(cli: *const obs_client) -> bool {
    osb_list_scenes(cli)
        .map_err(|err| fn_error!("{}", err))
        .is_ok()
}

fn obs_list_monitors(cli: *const obs_client) -> Result<bool, String> {
    let cli = obs_client::from_ptr(cli)?;
    cli.blocking_send(OBSRequest::ListMonitors)
}

/// request list of OBS studio monitors
/// @param cli - pointer to obs client
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_list_monitors(cli: *const obs_client) -> bool {
    obs_list_monitors(cli)
        .map_err(|err| fn_error!("{}", err))
        .is_ok()
}

fn obs_set_current_scene(cli: *const obs_client, name: *const c_char) -> Result<bool, String> {
    let cli = obs_client::from_ptr(cli)?;
    let name = str_from_cstr(name)?;
    cli.blocking_send(OBSRequest::SetCurrentScene(name.to_owned()))
}

/// set current scene
/// @param cli - pointer to obs client
/// @param name - scene name
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_set_current_scene(cli: *mut obs_client, name: *const c_char) -> bool {
    obs_set_current_scene(cli, name)
        .map_err(|err| fn_error!("{}", err))
        .is_ok()
}

fn obs_get_current_scene(cli: *const obs_client) -> Result<bool, String> {
    let cli = obs_client::from_ptr(cli)?;
    cli.blocking_send(OBSRequest::GetCurrentScene)
}

/// get current OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_get_current_scene(cli: *const obs_client) -> bool {
    obs_get_current_scene(cli)
        .map_err(|err| fn_error!("{}", err))
        .is_ok()
}

fn obs_next_scene(cli: *const obs_client) -> Result<bool, String> {
    let cli = obs_client::from_ptr(cli)?;
    cli.blocking_send(OBSRequest::MoveScene(1))
}

/// move to next OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_next_scene(cli: *const obs_client) -> bool {
    obs_next_scene(cli)
        .map_err(|err| fn_error!("{}", err))
        .is_ok()
}

fn obs_prev_scene(cli: *const obs_client) -> Result<bool, String> {
    let cli = obs_client::from_ptr(cli)?;
    cli.blocking_send(OBSRequest::MoveScene(-1))
}

/// move to previous OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_prev_scene(cli: *const obs_client) -> bool {
    obs_prev_scene(cli)
        .map_err(|err| fn_error!("{}", err))
        .is_ok()
}

fn obs_create_scene(cli: *const obs_client, name: *const c_char) -> Result<bool, String> {
    let cli = obs_client::from_ptr(cli)?;
    let name = str_from_cstr(name)?;
    cli.blocking_send(OBSRequest::CreateScene(name))
}

/// create new empty OBS scene
/// @param cli - pointer to obs client
/// @param name - scene name
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_create_scene(cli: *const obs_client, name: *const c_char) -> bool {
    obs_create_scene(cli, name)
        .map_err(|err| fn_error!("{}", err))
        .is_ok()
}

fn obs_remove_scene(cli: *const obs_client, name: *const c_char) -> Result<bool, String> {
    let cli = obs_client::from_ptr(cli)?;
    let name = str_from_cstr(name)?;
    cli.blocking_send(OBSRequest::RemoveScene(name))
}

/// remove OBS scene
/// @param cli - pointer to obs client
/// @param name - scene name
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_remove_scene(cli: *const obs_client, name: *const c_char) -> bool {
    obs_remove_scene(cli, name)
        .map_err(|err| fn_error!("{}", err))
        .is_ok()
}

fn obs_first_scene(cli: *const obs_client) -> Result<bool, String> {
    let cli = obs_client::from_ptr(cli)?;
    cli.blocking_send(OBSRequest::FirstScene)
}

/// change to first OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_first_scene(cli: *const obs_client) -> bool {
    obs_first_scene(cli)
        .map_err(|err| fn_error!("{}", err))
        .is_ok()
}

fn obs_last_scene(cli: *const obs_client) -> Result<bool, String> {
    let cli = obs_client::from_ptr(cli)?;
    cli.blocking_send(OBSRequest::LastScene)
}

/// change to last OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_last_scene(cli: *const obs_client) -> bool {
    obs_last_scene(cli)
        .map_err(|err| fn_error!("{}", err))
        .is_ok()
}

/// process all available results from OBS studio
/// @param cli - pointer to obs client client
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_obs_process_events(cli: *mut obs_client) -> bool {
    if cli.is_null() {
        return false;
    }
    let cli = unsafe { &mut *cli };

    while let Ok(rec) = cli.recv.try_recv() {
        match rec {
            Ok(reply) => match reply {
                OBSReply::Version(version) => cli.cb_reply.version(&version),
                OBSReply::Connected => cli.cb_reply.connected(),
                OBSReply::ListScenes(scenes) => cli.cb_reply.scene_list(scenes),
                OBSReply::ListMonitors(vec) => cli.cb_reply.monitor_list(vec),
                OBSReply::CurrentScene(cstr) => cli.cb_reply.current_scene(&cstr),
            },
            Err(err) => match err {
                Error::Error(msg) => cli.cb_err.exec(msg.as_str()),
                Error::Post(msg) => cli.cb_post.exec(msg.as_str()),
                Error::Debug(msg) => cli.cb_debug.exec(msg.as_str()),
                Error::Log(msg) => cli.cb_log.exec(msg.as_str()),
            },
        }
    }

    true
}
