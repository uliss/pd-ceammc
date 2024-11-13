use function_name::named;
use log::{debug, error, warn};
use std::ffi::{c_char, c_void, CStr, CString};
use std::ops::Index;
use std::ptr::{null, null_mut};
// use crate::service::{callback_msg, callback_notify, callback_progress, Service, ServiceCallback};

// use obws::requests::scene_items::SetEnabled;
use obws::Client;

use crate::fn_error;

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Clone, Copy)]
pub struct callback_msg {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, msg: *const c_char)>,
}

impl callback_msg {
    pub fn exec(&self, msg: &str) {
        let msg = CString::new(msg).unwrap_or_default();
        self.cb.map(|cb| cb(self.user, msg.as_ptr()));
    }
}

#[derive(Clone, Copy)]
#[repr(C)]
#[allow(non_camel_case_types)]
pub struct callback_notify {
    id: usize,
    cb: Option<extern "C" fn(id: usize)>,
}

impl callback_notify {
    pub fn exec(&self) {
        self.cb.map(|f| f(self.id));
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct obs_init {
    host: *const c_char,
    password: *const c_char,
    port: u16,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct obs_version {
    obs_major: u64,
    obs_minor: u64,
    obs_patch: u64,
    ws_major: u64,
    ws_minor: u64,
    ws_patch: u64,
    rpc_version: u32,
    platform: *const c_char,
    platform_desc: *const c_char,
    image_formats: *const *const c_char,
    image_formats_len: usize,
    available_rpc: *const *const c_char,
    available_rpc_len: usize,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct obs_data_scene {
    name: *const c_char,
    uuid: *const c_char,
}

// impl obs_data_scene {
//     fn new(id: obws::responses::scenes::CurrentProgramSceneId) -> Self {
//         return obs_data_scene {

//         }
//     }
// }

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct obs_data_scenes {
    current_scene: *const obs_data_scene,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct obs_data_monitor {
    index: u32,
    w: u16,
    h: u16,
    x: u16,
    y: u16,
    name: *const c_char,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct obs_result_cb {
    /// user data pointer (can be NULL)
    user: *mut c_void,
    /// version data callback function (can be NULL)
    cb_version: Option<extern "C" fn(user: *mut c_void, ver: &obs_version)>,
    /// scenes data callback function (can be NULL)
    cb_scenes: Option<extern "C" fn(user: *mut c_void, ver: &obs_data_scenes)>,
    /// monitors data callback function (can be NULL)
    cb_monitors:
        Option<extern "C" fn(user: *mut c_void, mons: *const obs_data_monitor, len: usize)>,
    cb_current_scene: Option<extern "C" fn(user: *mut c_void, name: *const c_char)>,
    /// connected/disconnected callback function (can be NULL)
    cb_connected: Option<extern "C" fn(user: *mut c_void, state: bool)>,
}

impl obs_result_cb {
    fn on_connect(&self) {
        match self.cb_connected {
            Some(cb) => cb(self.user, true),
            None => warn!("cb_connected callback is not set"),
        }
    }

    fn on_current_scene(&self, name: &CString) {
        if let Some(cb) = self.cb_current_scene {
            cb(self.user, name.as_ptr());
        }
    }

    fn on_monitors(&self, mons: &Vec<obws::responses::ui::Monitor>) {
        match self.cb_monitors {
            Some(cb) => {
                let names = mons
                    .iter()
                    .map(|m| CString::new(m.name.as_str()).unwrap())
                    .collect::<Vec<_>>();

                let mut mons = mons
                    .iter()
                    .map(|m| obs_data_monitor {
                        index: m.index,
                        w: m.size.width,
                        h: m.size.height,
                        x: m.position.x,
                        y: m.position.y,
                        name: null(),
                    })
                    .collect::<Vec<_>>();

                for (name, mon) in names.iter().zip(mons.iter_mut()) {
                    mon.name = name.as_ptr()
                }

                cb(self.user, mons.as_ptr(), mons.len());
            }
            None => warn!("cb_monitors callback is not set"),
        }
    }

    fn on_scenes(&self, _: &obws::responses::scenes::Scenes) {
        match self.cb_scenes {
            Some(_) => {
                // let cur_sc = scenes.current_program_scene.map(|x|
                //     obs_data_scene::new(x)
                // );

                // let sc = obs_data_scenes {
                //     current_scene: cur_sc
                // };
                // cb(self.user, &sc);
            }
            None => warn!("cb_scenes callback is not set"),
        }
    }

    fn on_version(&self, version: &obws::responses::general::Version) {
        match self.cb_version {
            Some(cb) => {
                let platform = CString::new(version.platform.as_str()).unwrap();
                let platform_desc = CString::new(version.platform_description.as_str()).unwrap();
                let sup_img = version
                    .supported_image_formats
                    .iter()
                    .map(|f| CString::new(f.as_str()).unwrap())
                    .collect::<Vec<_>>();

                let sup_img_ptr = sup_img.iter().map(|f| f.as_ptr()).collect::<Vec<_>>();

                let sup_rpc = version
                    .available_requests
                    .iter()
                    .map(|f| CString::new(f.as_str()).unwrap())
                    .collect::<Vec<CString>>();
                let sup_rpc_ptr = sup_rpc.iter().map(|f| f.as_ptr()).collect::<Vec<_>>();

                cb(
                    self.user,
                    &obs_version {
                        obs_major: version.obs_version.major,
                        obs_minor: version.obs_version.minor,
                        obs_patch: version.obs_version.patch,
                        ws_major: version.obs_web_socket_version.major,
                        ws_minor: version.obs_web_socket_version.minor,
                        ws_patch: version.obs_web_socket_version.patch,
                        rpc_version: version.rpc_version,
                        platform: platform.as_ptr(),
                        platform_desc: platform_desc.as_ptr(),
                        image_formats: sup_img_ptr.as_ptr(),
                        image_formats_len: sup_img_ptr.len(),
                        available_rpc: sup_rpc_ptr.as_ptr(),
                        available_rpc_len: sup_rpc_ptr.len(),
                    },
                );
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

#[derive(Debug)]
enum OBSRequest {
    GetVersion,
    ListScenes,
    ListMonitors,
    GetCurrentScene,
    SetCurrentScene(String),
    NextScene,
    Close,
}

#[derive(Debug)]
enum OBSReply {
    Version(obws::responses::general::Version),
    ListScenes(obws::responses::scenes::Scenes),
    ListMonitors(Vec<obws::responses::ui::Monitor>),
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

#[derive(PartialEq)]
enum ProcessMode {
    Continue,
    Break,
}

async fn process_request(
    req: Option<OBSRequest>,
    cli: &mut Client,
    cb_notify: &callback_notify,
    rep_tx: &tokio::sync::mpsc::Sender<Result<OBSReply, Error>>,
) -> ProcessMode {
    match req {
        Some(req) => {
            match req {
                OBSRequest::GetVersion => {
                    let ver = cli.general().version().await;
                    match ver {
                        Ok(ver) => {
                            // debug!("{ver:?}");
                            reply_send(cb_notify, rep_tx, OBSReply::Version(ver)).await;
                        }
                        Err(err) => {
                            reply_error(cb_notify, rep_tx, err.to_string()).await;
                        }
                    }
                    ProcessMode::Continue
                }
                OBSRequest::Close => {
                    cli.disconnect().await;
                    ProcessMode::Break
                }
                OBSRequest::ListScenes => {
                    match cli.scenes().list().await {
                        Ok(sc) => {
                            debug!("{sc:?}");
                        }
                        Err(err) => reply_error(cb_notify, rep_tx, err.to_string()).await,
                    }
                    ProcessMode::Continue
                }
                OBSRequest::GetCurrentScene => {
                    match cli.scenes().current_program_scene().await {
                        Ok(scene) => {
                            reply_send(
                                cb_notify,
                                rep_tx,
                                OBSReply::CurrentScene(
                                    CString::new(scene.id.name.as_str()).unwrap(),
                                ),
                            )
                            .await;
                        }
                        Err(err) => {
                            reply_error(cb_notify, rep_tx, err.to_string()).await;
                        }
                    }

                    ProcessMode::Continue
                }
                OBSRequest::SetCurrentScene(name) => {
                    if let Err(err) = cli.scenes().set_current_program_scene(name.as_str()).await {
                        reply_error(cb_notify, rep_tx, err.to_string()).await;
                    }

                    ProcessMode::Continue
                }
                OBSRequest::ListMonitors => {
                    match cli.ui().list_monitors().await {
                        Ok(mons) => {
                            reply_send(cb_notify, rep_tx, OBSReply::ListMonitors(mons)).await;
                        }
                        Err(err) => {
                            reply_error(cb_notify, rep_tx, err.to_string()).await;
                        }
                    }

                    ProcessMode::Continue
                }
                OBSRequest::NextScene => {
                    match cli.scenes().list().await {
                        Ok(cur) => {
                            let name = cur.current_program_scene.unwrap().name;
                            let mut iter = cur.scenes.iter();
                            let idx = iter.find(|x| x.id.name == name).take().unwrap().index;
                            debug!("current scene: {name}");

                            match cur.scenes.iter().find(|f| (f.index + 1) == idx) {
                                Some(scene) => match cli
                                    .scenes()
                                    .set_current_program_scene(scene.id.name.as_str())
                                    .await
                                {
                                    Ok(_) => {
                                        debug!("move to scene: '{sname}'", sname = scene.id.name)
                                    }
                                    Err(err) => {
                                        reply_error(cb_notify, rep_tx, err.to_string()).await;
                                    }
                                },
                                None => {
                                    reply_error(cb_notify, rep_tx, String::from("last scene"))
                                        .await;
                                }
                            }
                        }
                        Err(err) => {
                            reply_error(cb_notify, rep_tx, err.to_string()).await;
                        }
                    }

                    ProcessMode::Continue
                }
            }
        }
        None => ProcessMode::Break,
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
                                if process_request(req, &mut cli, &cb_notify, &rep_tx).await
                                    == ProcessMode::Break
                                {
                                    debug!("exit runloop ...");
                                    return ();
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

/// send version request to OBS studio
/// @param cli - pointer to obs client
#[no_mangle]
pub extern "C" fn ceammc_obs_get_version(cli: *mut obs_client) {
    if !cli.is_null() {
        let cli = unsafe { &mut *cli };
        match cli.send.blocking_send(OBSRequest::GetVersion) {
            Ok(_) => {}
            Err(err) => {
                error!("ceammc_obs_get_version() send error: {err}")
            }
        }
    }
}

/// send version request to OBS studio
/// @param cli - pointer to obs client
#[no_mangle]
pub extern "C" fn ceammc_obs_list_scenes(cli: *mut obs_client) {
    if !cli.is_null() {
        let cli = unsafe { &mut *cli };
        match cli.send.blocking_send(OBSRequest::ListScenes) {
            Ok(_) => {}
            Err(err) => {
                error!("ceammc_obs_list_scenes() send error: {err}")
            }
        }
    }
}

/// request list of OBS studio monitors
/// @param cli - pointer to obs client
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_list_monitors(cli: *mut obs_client) {
    if !cli.is_null() {
        let cli = unsafe { &mut *cli };
        match cli.send.blocking_send(OBSRequest::ListMonitors) {
            Ok(_) => {}
            Err(err) => {
                fn_error!("send error: {}", err);
            }
        }
    }
}

/// set current scene
/// @param cli - pointer to obs client
/// @param name - scene name
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_set_current_scene(cli: *mut obs_client, name: *const c_char) -> bool {
    if !cli.is_null() {
        let cli = unsafe { &mut *cli };
        let name = unsafe { CStr::from_ptr(name) }.to_str();
        if let Err(err) = name {
            fn_error!("name error: {}", err);
            return false;
        }

        match cli
            .send
            .blocking_send(OBSRequest::SetCurrentScene(name.unwrap().to_owned()))
        {
            Ok(_) => return true,
            Err(err) => {
                fn_error!("send error: {}", err);
            }
        }
    }

    return false;
}

/// get current OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_get_current_scene(cli: *mut obs_client) -> bool {
    if !cli.is_null() {
        let cli = unsafe { &mut *cli };
        match cli.send.blocking_send(OBSRequest::GetCurrentScene) {
            Ok(_) => return true,
            Err(err) => {
                fn_error!("send error: {}", err);
            }
        }
    }

    return false;
}

/// move to next OBS scene
/// @param cli - pointer to obs client
/// @return true on success, false on error
#[no_mangle]
#[named]
pub extern "C" fn ceammc_obs_next_scene(cli: *mut obs_client) -> bool {
    if !cli.is_null() {
        let cli = unsafe { &mut *cli };
        match cli.send.blocking_send(OBSRequest::NextScene) {
            Ok(_) => return true,
            Err(err) => {
                fn_error!("send error: {}", err);
            }
        }
    }

    return false;
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
        match &rec {
            Ok(reply) => match reply {
                OBSReply::Version(version) => cli.cb_reply.on_version(version),
                OBSReply::Connected => cli.cb_reply.on_connect(),
                OBSReply::ListScenes(scenes) => cli.cb_reply.on_scenes(scenes),
                OBSReply::ListMonitors(vec) => cli.cb_reply.on_monitors(vec),
                OBSReply::CurrentScene(cstr) => cli.cb_reply.on_current_scene(cstr),
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
