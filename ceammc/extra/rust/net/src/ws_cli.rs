use futures_util::{SinkExt, StreamExt};
use std::{
    borrow::Cow, ffi::{CStr, CString}, os::raw::{c_char, c_void}, ptr::null_mut
};
use tokio::net::TcpStream;
use tokio_tungstenite::{connect_async, MaybeTlsStream, WebSocketStream};
use tungstenite::{error::ProtocolError, protocol::{frame::coding::CloseCode, CloseFrame}, Message};
use url::Url;
use log::{warn, debug};

use crate::service::{callback_msg, callback_notify, callback_progress, Service, ServiceCallback};

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct ws_client_init {
    /// websocket url in format: ws://HOST:PORT/PATH?
    url: *const c_char,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct ws_client_result_cb {
    /// user data pointer (can be NULL)
    user: *mut c_void,
    /// text data callback function (can be NULL)
    cb_text: Option<extern "C" fn(user: *mut c_void, txt: *const c_char)>,
    /// binary data callback function (can be NULL)
    cb_binary: Option<extern "C" fn(user: *mut c_void, data: *const u8, data_len: usize)>,
    /// ping callback function (can be NULL)
    cb_ping: Option<extern "C" fn(user: *mut c_void, data: *const u8, data_len: usize)>,
    /// pong callback function (can be NULL)
    cb_pong: Option<extern "C" fn(user: *mut c_void, data: *const u8, data_len: usize)>,
    /// close callback function (can be NULL)
    cb_close: Option<extern "C" fn(user: *mut c_void)>,
    /// connected/disconnected callback function (can be NULL)
    cb_connected: Option<extern "C" fn(user: *mut c_void, state: bool)>,
}

#[derive(Debug)]
enum WsRequest {
    SendMessage(Message, bool),
    Close,
    Flush,
    Quit,
}

#[derive(Debug)]
enum WsReply {
    Message(Message),
    Connected,
    Disconnected,
}

type WsClientService = Service<WsRequest, WsReply>;

#[allow(non_camel_case_types)]
pub struct ws_client {
    cli: WsClientService,
}

impl ServiceCallback<WsReply> for ws_client_result_cb {
    fn exec(&self, data: &WsReply) {
        match data {
            WsReply::Message(msg) => match msg {
                Message::Text(txt) => {
                    self.cb_text.map(|f| {
                        let txt = CString::new(txt.clone().trim()).unwrap_or_default();
                        f(self.user, txt.as_ptr());
                    });
                }
                Message::Binary(data) => {
                    self.cb_binary.map(|f| {
                        f(self.user, data.as_ptr(), data.len());
                    });
                }
                Message::Ping(data) => {
                    self.cb_ping.map(|f| {
                        f(self.user, data.as_ptr(), data.len());
                    });
                }
                Message::Pong(data) => {
                    self.cb_pong.map(|f| {
                        f(self.user, data.as_ptr(), data.len());
                    });
                }
                Message::Close(_x) => {
                    self.cb_close.map(|f| {
                        f(self.user);
                    });
                }
                Message::Frame(_x) => {}
            },
            WsReply::Connected => {
                self.cb_connected.map(|f| {
                    f(self.user, true);
                });
            },
            WsReply::Disconnected => {
                self.cb_connected.map(|f| {
                    f(self.user, false);
                });
            }
        }
    }
}

async fn reply_error(
    cb: &crate::service::callback_notify,
    rep_tx: &tokio::sync::mpsc::Sender<Result<WsReply, crate::service::Error>>,
    msg: String,
) {
    debug!("error: {msg}");
    if let Err(err) = rep_tx.send(Err(crate::service::Error::Error(msg))).await {
        warn!("send error: {err}");
    } else {
        cb.exec();
    }
}

async fn reply_send(
    cb: &crate::service::callback_notify,
    rep_tx: &tokio::sync::mpsc::Sender<Result<WsReply, crate::service::Error>>,
    msg: WsReply,
) {
    if let Err(err) = rep_tx.send(Ok(msg)).await {
        warn!("send error: {err}");
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
    req: Option<WsRequest>,
    ws: &mut WebSocketStream<MaybeTlsStream<TcpStream>>,
    cb_notify: &crate::service::callback_notify,
    rep_tx: &tokio::sync::mpsc::Sender<Result<WsReply, crate::service::Error>>,
) -> ProcessMode {
    match req {
        Some(req) => match req {
            WsRequest::SendMessage(m, flush) => {
                if flush {
                    if let Err(err) = ws.send(m).await {
                        let _ = reply_error(cb_notify, rep_tx, err.to_string());
                        return ProcessMode::Break;
                    }
                } else {
                    if let Err(err) = ws.feed(m).await {
                        let _ = reply_error(cb_notify, rep_tx, err.to_string());
                        return ProcessMode::Break;
                    }
                }
                ProcessMode::Continue
            }
            WsRequest::Close => {
                if let Err(err) = ws.close(Some(CloseFrame{ code: CloseCode::Normal, reason: Cow::Borrowed("bye") })).await {
                    let _ = reply_error(cb_notify, rep_tx, err.to_string());
                    return ProcessMode::Break;
                }
                ProcessMode::Continue
            }
            WsRequest::Quit => ProcessMode::Break,
            WsRequest::Flush => {
                if let Err(err) = ws.flush().await {
                    let _ = reply_error(cb_notify, rep_tx, err.to_string());
                    return ProcessMode::Break;
                }
                ProcessMode::Continue
            }
        },
        None => ProcessMode::Break,
    }
}

async fn process_incoming(
    res: Result<tungstenite::Message, tungstenite::Error>,
    ws: &mut WebSocketStream<MaybeTlsStream<TcpStream>>,
    cb_notify: &crate::service::callback_notify,
    rep_tx: &tokio::sync::mpsc::Sender<Result<WsReply, crate::service::Error>>,
) {
    match res {
        Ok(msg) => match msg {
            Message::Ping(data) => {
                debug!("pong {data:?}");
                if let Err(err) = ws.send(Message::Pong(data.clone())).await {
                    let _ = reply_error(&cb_notify, &rep_tx, err.to_string());
                } else {
                    reply_send(&cb_notify, &rep_tx, WsReply::Message(Message::Ping(data))).await;
                }
            }
            Message::Close(_x) => {
                reply_send(&cb_notify, &rep_tx, WsReply::Disconnected).await;
            }
            msg => {
                reply_send(&cb_notify, &rep_tx, WsReply::Message(msg)).await;
            }
        },
        Err(err) => {
            match err {
                tungstenite::Error::ConnectionClosed => { 
                    debug!("closed");
                    reply_send(&cb_notify, &rep_tx, WsReply::Disconnected).await;
                }
                tungstenite::Error::Protocol(ProtocolError::ResetWithoutClosingHandshake) => {
                    reply_error(&cb_notify, &rep_tx, err.to_string()).await;
                    reply_send(&cb_notify, &rep_tx, WsReply::Disconnected).await;
                },
                _ => {
                    reply_error(&cb_notify, &rep_tx, err.to_string()).await;
                }
            }
            return ();
        }
    }
}

/// create websocket client
/// @param params - connection params
/// @param cb_err - callback for error messages
/// @param cb_post - callback for post messages
/// @param cb_debug - callback for debug messages
/// @param cb_log - callback for log messages
/// @param _cb_progress - unused
/// @param cb_reply - reply callbacks
/// @param cb_notify - notification callback
/// @return pointer to websocket client or NULL on error
#[no_mangle]
pub extern "C" fn ceammc_ws_client_new(
    params: ws_client_init,
    cb_err: callback_msg,
    cb_post: callback_msg,
    cb_debug: callback_msg,
    cb_log: callback_msg,
    _cb_progress: callback_progress,
    cb_reply: ws_client_result_cb,
    cb_notify: callback_notify,
) -> *mut ws_client {
    if params.url.is_null() {
        cb_err.exec("null url pointer");
        return std::ptr::null_mut();
    }

    let url = unsafe { CStr::from_ptr(params.url) }.to_str();

    let rt = tokio::runtime::Runtime::new();

    match rt {
        Ok(rt) => {
            debug!("creating tokio runtime ...");

            let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<WsRequest>(32);
            let (rep_tx, rep_rx) =
                tokio::sync::mpsc::channel::<Result<WsReply, crate::service::Error>>(32);

            match url {
                Ok(url) => match Url::parse(url) {
                    Ok(url) => {
                        std::thread::spawn(move || {
                            debug!("starting worker thread ...");
                            rt.block_on(async move {
                                match connect_async(url).await {
                                    Ok((mut wss, _response)) => {
                                        debug!("connected to server ...");
                                        reply_send(&cb_notify, &rep_tx, WsReply::Connected).await;
                                        debug!("starting runloop ...");
                                        loop {
                                            tokio::select! {
                                                // process requests
                                                var = req_rx.recv() => {
                                                    debug!("new request: {var:?}");
                                                    if process_request(var, &mut wss, &cb_notify, &rep_tx).await == ProcessMode::Break { 
                                                        debug!("exit runloop ...");
                                                        return (); 
                                                    }
                                                },
                                                var = wss.next() => {
                                                    debug!("message from server: {var:?}");
                                                    match var {
                                                        Some(x) => process_incoming(x, &mut wss, &cb_notify, &rep_tx).await,
                                                        None => {
                                                            debug!("exit runloop ...");
                                                            return ();
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    Err(err) => {
                                        reply_error(&cb_notify, &rep_tx, err.to_string()).await;
                                    }
                                }
                            });
                            debug!("exit worker thread ...");
                        });

                        let cli = WsClientService::new(
                            cb_err,
                            cb_post,
                            cb_debug,
                            cb_log,
                            callback_progress::default(),
                            Box::new(cb_reply),
                            req_tx,
                            rep_rx,
                        );

                        return Box::into_raw(Box::new(ws_client { cli }));
                    }

                    Err(err) => cb_err.exec(err.to_string().as_str()),
                },
                Err(err) => cb_err.exec(err.to_string().as_str()),
            }
        }
        Err(err) => {
            cb_err.exec(format!("tokio runtime creation error: {err}").as_str());
            return null_mut();
        }
    }

    null_mut()
}

/// free websocket client
/// @param cli - pointer to websocket client
#[no_mangle]
pub extern "C" fn ceammc_ws_client_free(cli: *mut ws_client) {
    if !cli.is_null() {
        let cli = unsafe { Box::from_raw(cli) };
        cli.cli.blocking_send(WsRequest::Quit);
    }
}

#[no_mangle]
/// sends text message to WebSocket server
/// @param cli - pointer to ws client
/// @param msg - text message
/// @param flush - if true ensures all messages
///        previously passed to write and automatic queued pong responses are written & flushed into the underlying stream.
/// @return true on success, false on error
pub extern "C" fn ceammc_ws_client_send_text(
    cli: *mut ws_client,
    msg: *const c_char,
    flush: bool,
) -> bool {
    if cli.is_null() {
        return false;
    }
    let cli = unsafe { &mut *cli };

    if msg.is_null() {
        cli.cli.on_error("null message pointer");
        return false;
    }
    let msg = unsafe { CStr::from_ptr(msg) }.to_str();
    match msg {
        Ok(str) => cli
            .cli
            .send_request(WsRequest::SendMessage(Message::Text(str.to_owned()), flush)),
        Err(err) => {
            cli.cli.on_error(err.to_string().as_str());
            false
        }
    }
}

#[no_mangle]
/// sends binary message to WebSocket server
/// @param cli - pointer to ws client
/// @param data - data pointer
/// @param len - data length
/// @param flush - if true ensures all messages
///        previously passed to write and automatic queued pong responses are written & flushed into the underlying stream.
/// @return true on success, false on error
pub extern "C" fn ceammc_ws_client_send_binary(
    cli: *mut ws_client,
    data: *const u8,
    len: usize,
    flush: bool,
) -> bool {
    if cli.is_null() {
        return false;
    }
    let cli = unsafe { &mut *cli };

    if data.is_null() || len == 0 {
        cli.cli.on_error("invalid data");
        return false;
    }

    let data = unsafe { std::slice::from_raw_parts(data, len) }.to_vec();
    cli.cli
        .send_request(WsRequest::SendMessage(Message::Binary(data), flush))
}

/// sends ping to websocket server
/// @param cli - pointer to websocket client
/// @param data - pointer to ping data (can be NULL)
/// @param len - data length
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_ws_client_send_ping(
    cli: *mut ws_client,
    data: *const u8,
    len: usize,
) -> bool {
    if cli.is_null() {
        return false;
    }
    let cli = unsafe { &mut *cli };

    let data = if data.is_null() || len == 0 {
        vec![]
    } else {
        unsafe { std::slice::from_raw_parts(data, len) }.to_vec()
    };

    cli.cli
        .send_request(WsRequest::SendMessage(Message::Ping(data), true))
}

/// process all available results from websocket
/// @param cli - pointer to websocket client
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_ws_client_process_events(cli: *mut ws_client) -> bool {
    if cli.is_null() {
        return false;
    }
    let cli = unsafe { &mut *cli };

    cli.cli.process_results();
    true
}

/// close websocket client connection
/// @param cli - pointer to websocket client
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_ws_client_close(cli: *mut ws_client) -> bool {
    if cli.is_null() {
        return false;
    }
    let cli = unsafe { &mut *cli };

    cli.cli.send_request(WsRequest::Close)
}

/// flush websocket client connection
/// @param cli - pointer to websocket client
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_ws_client_flush(cli: *mut ws_client) -> bool {
    if cli.is_null() {
        return false;
    }
    let cli = unsafe { &mut *cli };

    cli.cli.send_request(WsRequest::Flush)
}
