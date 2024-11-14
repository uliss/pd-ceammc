use function_name::named;
use log::{debug, error, warn};
use std::ffi::{c_char, CStr, CString};
use std::ptr::null_mut;

use obws::Client;

use crate::common_ffi::{callback_msg, callback_notify, Error};
use crate::fn_error;
use crate::obs_ffi::{obs_client, obs_init, obs_result_cb, OBSReply, OBSRequest};

fn str_from_cstr(str: *const c_char) -> Result<String, String> {
    if str.is_null() {
        Err("null string pointer".to_owned())
    } else {
        Ok(unsafe { CStr::from_ptr(str).to_str() }
            .map_err(|err| err.to_string())?
            .to_owned())
    }
}

async fn reply_send(
    cb: &callback_notify,
    rep_tx: &tokio::sync::mpsc::Sender<Result<OBSReply, crate::common_ffi::Error>>,
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

            return Box::into_raw(Box::new(obs_client::new(
                req_tx, rep_rx, cb_err, cb_post, cb_debug, cb_log, cb_reply,
            )));
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
        match cli.blocking_send(OBSRequest::Close) {
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
    cli.process_events();
    true
}
