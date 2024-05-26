use futures_util::{SinkExt, StreamExt};
use log::{debug, info, warn};
use tokio::{net::TcpListener, sync::mpsc::UnboundedSender};
use tungstenite::Message;

use std::{
    collections::HashMap,
    ffi::{CStr, CString},
    net::SocketAddr,
    os::raw::{c_char, c_void},
    sync::{
        atomic::{AtomicUsize, Ordering},
        Arc, Mutex,
    }, time::Duration,
};

use crate::service::{callback_msg, callback_notify, callback_progress};

type WsClientService = crate::service::Service<WsServerRequest, WsServerReply>;

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct ws_server_init {
    /// interface address: 0.0.0.0
    addr: *const c_char,
    /// listen port
    port: u16,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct ws_server_result_cb {
    /// user data pointer (can be NULL)
    user: *mut c_void,
    /// text data callback function (can be NULL)
    cb_text: Option<extern "C" fn(user: *mut c_void, txt: *const c_char, peer: &ws_peer_info)>,
    /// binary data callback function (can be NULL)
    cb_binary: Option<
        extern "C" fn(user: *mut c_void, data: *const u8, data_len: usize, peer: &ws_peer_info),
    >,
    /// ping callback function (can be NULL)
    cb_ping: Option<
        extern "C" fn(user: *mut c_void, data: *const u8, data_len: usize, peer: &ws_peer_info),
    >,
    /// connected/disconnected callback function (can be NULL)
    cb_connected: Option<extern "C" fn(user: *mut c_void, state: bool, peer: &ws_peer_info)>,
}

impl crate::service::ServiceCallback<WsServerReply> for ws_server_result_cb {
    fn exec(&self, data: &mut WsServerReply) {
        debug!("in: {data:?}");

        match data {
            WsServerReply::Text(msg, peer) => {
                self.cb_text
                    .map(|f| f(self.user, msg.as_ptr(), &peer.ffi_info()));
            }
            WsServerReply::Connected(peer) => {
                self.cb_connected
                    .map(|f| f(self.user, true, &peer.ffi_info()));
            }
            WsServerReply::Disconnected(peer) => {
                self.cb_connected
                    .map(|f| f(self.user, false, &peer.ffi_info()));
            }
            WsServerReply::Binary(data, peer) => {
                self.cb_binary
                    .map(|f| f(self.user, data.as_ptr(), data.len(), &peer.ffi_info()));
            }
            WsServerReply::Ping(data, peer) => {
                self.cb_ping
                    .map(|f| f(self.user, data.as_ptr(), data.len(), &peer.ffi_info()));
            }
        }
    }
}

#[derive(Debug)]
enum WsServerRequest {
    Quit,
}

#[derive(Debug)]
enum WsServerReply {
    Text(CString, PeerInfo),
    Binary(Vec<u8>, PeerInfo),
    Ping(Vec<u8>, PeerInfo),
    Connected(PeerInfo),
    Disconnected(PeerInfo),
}

async fn reply_message(
    msg: WsServerReply,
    cb: &crate::service::callback_notify,
    rep_tx: &tokio::sync::mpsc::Sender<Result<WsServerReply, crate::service::Error>>,
) {
    if let Err(err) = rep_tx.send(Ok(msg)).await {
        warn!("send error: {err}");
    } else {
        cb.exec();
    }
}

async fn reply_error(
    cb: &crate::service::callback_notify,
    rep_tx: &tokio::sync::mpsc::Sender<Result<WsServerReply, crate::service::Error>>,
    msg: String,
) {
    debug!("error: {msg}");
    if let Err(err) = rep_tx.send(Err(crate::service::Error::Error(msg))).await {
        warn!("send error: {err}");
    } else {
        cb.exec();
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct ws_peer_info {
    addr: *const c_char,
    id: usize,
}

#[derive(Debug, Clone)]
struct PeerInfo {
    addr: CString,
    id: usize,
}

impl PeerInfo {
    fn ffi_info(&self) -> ws_peer_info {
        ws_peer_info {
            addr: self.addr.as_ptr(),
            id: self.id,
        }
    }
}

#[derive(Debug)]
struct Token(usize);
impl Token {
    fn new() -> Self {
        static COUNTER: AtomicUsize = AtomicUsize::new(1);
        let inner = COUNTER.fetch_add(1, Ordering::Relaxed);
        Token(inner)
    }
}

#[derive(Debug, Clone)]
enum ClientRequest {
    SendText(String),
    SendBinary(Vec<u8>),
    SendPing(Vec<u8>),
    Close,
    Shutdown,
}

type Tx = UnboundedSender<ClientRequest>;
struct ClientInfo {
    peer: PeerInfo,
    tx: Tx,
}
type PeerMap = Arc<Mutex<HashMap<SocketAddr, ClientInfo>>>;

#[allow(non_camel_case_types)]
pub struct ws_server {
    srv: WsClientService,
    peer_map: PeerMap,
}

impl ws_server {
    pub fn err(&self, msg: &str) {
        self.srv.on_error(msg)
    }
}

impl Drop for ws_server {
    fn drop(&mut self) {
        self.srv.blocking_send(WsServerRequest::Quit);
    }
}

fn msg2msg(msg: Message, peer: PeerInfo) -> Option<WsServerReply> {
    match msg {
        Message::Text(txt) => Some(WsServerReply::Text(
            CString::new(txt).unwrap_or_default(),
            peer.clone(),
        )),
        Message::Binary(data) => Some(WsServerReply::Binary(data, peer)),
        Message::Ping(data) => Some(WsServerReply::Ping(data, peer)),
        Message::Close(_) => Some(WsServerReply::Disconnected(peer)),
        _ => None,
    }
}

async fn handle_server_to_client(
    req: ClientRequest,
    cli_tx: &mut futures_util::stream::SplitSink<
        tokio_tungstenite::WebSocketStream<tokio::net::TcpStream>,
        tungstenite::Message,
    >,
    cb_notify: &callback_notify,
    rep_tx: &tokio::sync::mpsc::Sender<Result<WsServerReply, crate::service::Error>>,
) {
    match req {
        ClientRequest::SendText(txt) => {
            if let Err(err) = cli_tx.send(Message::Text(txt)).await {
                reply_error(cb_notify, rep_tx, err.to_string()).await;
            }
        }
        ClientRequest::SendBinary(data) => {
            if let Err(err) = cli_tx.send(Message::Binary(data)).await {
                reply_error(cb_notify, rep_tx, err.to_string()).await;
            }
        }
        ClientRequest::SendPing(data) => {
            if let Err(err) = cli_tx.send(Message::Ping(data)).await {
                reply_error(cb_notify, rep_tx, err.to_string()).await;
            }
        }
        ClientRequest::Close => {
            if let Err(err) = cli_tx.send(Message::Close(None)).await {
                reply_error(cb_notify, rep_tx, err.to_string()).await;
            }
        },
        ClientRequest::Shutdown => {
            if let Err(err) = cli_tx.close().await {
                reply_error(cb_notify, rep_tx, err.to_string()).await;
            }
        },
    }
}

async fn accept_connection(
    peer_map: PeerMap,
    conn: Result<(tokio::net::TcpStream, std::net::SocketAddr), std::io::Error>,
    cb_notify: callback_notify,
    rep_tx: tokio::sync::mpsc::Sender<Result<WsServerReply, crate::service::Error>>,
) {
    match conn {
        Ok((stream, addr)) => {
            debug!("Peer address: {}", addr);

            let ws_stream = match tokio_tungstenite::accept_async(stream).await {
                Ok(stream) => stream,
                Err(err) => {
                    reply_error(
                        &cb_notify,
                        &rep_tx,
                        format!(
                            "Error during the websocket handshake occurred: {}",
                            err.to_string()
                        ),
                    )
                    .await;
                    return;
                }
            };

            info!("new connection: {}", addr);
            // Insert the write part of this peer to the peer map.
            let (tx, mut rx) = tokio::sync::mpsc::unbounded_channel();
            let cli_id = Token::new().0;
            let peer = PeerInfo {
                addr: CString::new(addr.to_string()).unwrap_or_default(),
                id: cli_id,
            };
            let cli_info = ClientInfo {
                peer: peer.clone(),
                tx,
            };
            peer_map.lock().unwrap().insert(addr, cli_info);
            reply_message(WsServerReply::Connected(peer.clone()), &cb_notify, &rep_tx).await;

            let (mut write, mut read) = ws_stream.split();

            loop {
                tokio::select! {
                    ws_msg = read.next() => {
                        match ws_msg {
                            Some(ws_msg) => {
                                match ws_msg {
                                    Ok(ws_msg) =>  {
                                        if let Some(msg) = msg2msg(ws_msg, peer.clone()) {
                                            reply_message(msg, &cb_notify, &rep_tx).await;
                                        };
                                    },
                                    Err(err) => {
                                        reply_error(
                                            &cb_notify,
                                            &rep_tx,
                                            err.to_string(),
                                        )
                                        .await;
                                    },
                                };
                            }
                            None => break,
                        }
                    }
                    req = rx.recv() => {
                        match req {
                            Some(req) => handle_server_to_client(req,&mut write, &cb_notify, &rep_tx).await,
                            None => break,
                        }
                    }
                }
            }

            peer_map.lock().unwrap().remove(&addr);

            info!("disconnected: {addr}");
        }
        Err(err) => reply_error(&cb_notify, &rep_tx, err.to_string()).await,
    }
}

#[no_mangle]
pub extern "C" fn ceammc_ws_server_create(
    params: ws_server_init,
    cb_err: callback_msg,
    cb_post: callback_msg,
    cb_debug: callback_msg,
    cb_log: callback_msg,
    _cb_progress: callback_progress,
    cb_reply: ws_server_result_cb,
    cb_notify: callback_notify,
) -> *mut ws_server {
    if params.addr.is_null() {
        cb_err.exec("null address pointer");
        return std::ptr::null_mut();
    }

    let addr = unsafe { CStr::from_ptr(params.addr) }.to_str();
    match addr {
        Ok(addr) => {
            let addr = format!("{addr}:{}", params.port);
            // let rt = tokio::runtime::Runtime::new();
            let rt = tokio::runtime::Builder::new_multi_thread()
                .thread_keep_alive(Duration::from_millis(1000))
                .worker_threads(2)
                .build();

            match rt {
                Ok(rt) => {
                    debug!("creating tokio runtime ...");

                    let connected_clients = PeerMap::new(Mutex::new(HashMap::new()));
                    let connected_clients_copy = connected_clients.clone();

                    let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<WsServerRequest>(32);
                    let (rep_tx, rep_rx) = tokio::sync::mpsc::channel::<
                        Result<WsServerReply, crate::service::Error>,
                    >(32);

                    std::thread::spawn(move || { 
                        debug!("starting worker thread ...");

                        rt.block_on(async move { 
                            match TcpListener::bind(&addr).await {
                                Ok(sock) => {
                                    info!("listening on: {}", addr);

                                    loop {
                                        tokio::select! {
                                            req = req_rx.recv() => {
                                                match req {
                                                    Some(req) => match req {
                                                        WsServerRequest::Quit => {
                                                            debug!("[worker] quit");
                                                            break
                                                        },
                                                    },
                                                    None => {
                                                        debug!("[worker] recv None");
                                                        break;
                                                    },
                                                }
                                            },
                                            sock = sock.accept() => {
                                                tokio::spawn(accept_connection(connected_clients_copy.clone(), sock, cb_notify, rep_tx.clone()));
                                            }
                                        }
                                    }
                                    
                                    info!("stop listening: {}", addr);
                                }
                                Err(err) => reply_error(&cb_notify, &rep_tx, err.to_string()).await,
                            }
                        });
                    });

                    Box::into_raw(Box::new(ws_server {
                        srv: WsClientService::new(
                            cb_err,
                            cb_post,
                            cb_debug,
                            cb_log,
                            callback_progress::default(),
                            Box::new(cb_reply),
                            req_tx,
                            rep_rx,
                        ),
                        peer_map: connected_clients,
                    }))
                }
                Err(err) => {
                    cb_err.exec(format!("tokio runtime creation error: {err}").as_str());
                    return std::ptr::null_mut();
                }
            }
        }
        Err(err) => {
            cb_err.exec(format!("utf8 error: {err}").as_str());
            std::ptr::null_mut()
        }
    }
}

/// free websocket server
/// @param src - pointer to server
#[no_mangle]
pub extern "C" fn ceammc_ws_server_free(srv: *mut ws_server) {
    if !srv.is_null() { 
        let _ = unsafe { Box::from_raw(srv) };
    }
}

/// process server events (non-blocking) and execute callbacks
/// @param srv - pointer to websocket server
#[no_mangle]
pub extern "C" fn ceammc_ws_server_process_events(srv: *mut ws_server) -> bool {
    if srv.is_null() {
        return false;
    }

    let srv = unsafe { &mut *srv };

    srv.srv.process_results();
    true
}

#[allow(non_camel_case_types)]
#[allow(dead_code)]
#[repr(C)]
pub enum ws_client_selector {
    ALL,    // send to all connected clients
    FIRST,  // send to first connected client
    LAST,   // send to last connected client
    ID,     // send to client with specified id
    EXCEPT, // send to all clients except specified one
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct ws_client_target {
    sel: ws_client_selector, // client selector
    id: usize,               // client id
}

fn send_message(srv: &mut ws_server, msg: ClientRequest, target: ws_client_target) -> bool {
    match target.sel {
        ws_client_selector::ALL => {
            let mut has_errors = false;
            for (_addr, info) in srv.peer_map.lock().unwrap().iter() {
                debug!("send to ALL: [{}]", info.peer.id);
                if let Err(err) = info.tx.send(msg.clone()) {
                    srv.srv.on_error(err.to_string().as_str());
                    has_errors = true;
                }
            }
            return if has_errors { false } else { true };
        }
        ws_client_selector::FIRST => match srv
            .peer_map
            .lock()
            .unwrap()
            .iter()
            .min_by(|x, y| x.1.peer.id.cmp(&y.1.peer.id))
        {
            Some((_addr, info)) => {
                debug!("send to FIRST: [{}]", info.peer.id);
                if let Err(err) = info.tx.send(msg) {
                    srv.srv.on_error(err.to_string().as_str());
                    return false;
                } else {
                    return true;
                }
            }
            None => {
                srv.srv.on_error("no connected clients");
                return false;
            }
        },
        ws_client_selector::LAST => match srv
            .peer_map
            .lock()
            .unwrap()
            .iter()
            .max_by(|x, y| x.1.peer.id.cmp(&y.1.peer.id))
        {
            Some((_addr, info)) => {
                debug!("send to LAST: [{}]", info.peer.id);
                if let Err(err) = info.tx.send(msg) {
                    srv.srv.on_error(err.to_string().as_str());
                    return false;
                } else {
                    return true;
                }
            }
            None => {
                srv.srv.on_error("no connected clients");
                return false;
            }
        },
        ws_client_selector::ID => {
            for (_addr, info) in srv.peer_map.lock().unwrap().iter() {
                if info.peer.id == target.id {
                    debug!("send to ID: [{}]", info.peer.id);
                    if let Err(err) = info.tx.send(msg) {
                        srv.srv.on_error(err.to_string().as_str());
                        return false;
                    } else {
                        return true;
                    }
                }
            }
        }
        ws_client_selector::EXCEPT => {
            for (_addr, info) in srv.peer_map.lock().unwrap().iter() {
                if info.peer.id != target.id {
                    debug!("send EXCEPT {}: [{}]", target.id, info.peer.id);
                    if let Err(err) = info.tx.send(msg.clone()) {
                        srv.srv.on_error(err.to_string().as_str());
                        return false;
                    }
                }
            }
        }
    }

    true
}

/// send text message to connected clients
/// @param srv - pointer to websocket server
/// @param msg - text message
/// @param target - specify target clients
#[no_mangle]
pub extern "C" fn ceammc_ws_server_send_text(
    srv: *mut ws_server,
    msg: *const c_char,
    target: ws_client_target,
) -> bool {
    if srv.is_null() {
        return false;
    }

    let srv = unsafe { &mut *srv };

    if msg.is_null() {
        return false;
    }

    let msg = unsafe { CStr::from_ptr(msg) }.to_string_lossy().to_string();
    send_message(srv, ClientRequest::SendText(msg), target)
}

/// send binary message to connected clients
/// @param srv - pointer to websocket server
/// @param data - pointer to data
/// @param len - data length
/// @param target - specify target clients
#[no_mangle]
pub extern "C" fn ceammc_ws_server_send_binary(
    srv: *mut ws_server,
    data: *const u8,
    len: usize,
    target: ws_client_target,
) -> bool {
    if srv.is_null() {
        return false;
    }

    let srv = unsafe { &mut *srv };

    let msg = if data.is_null() || len == 0 {
        ClientRequest::SendBinary(vec![])
    } else {
        ClientRequest::SendBinary(unsafe { std::slice::from_raw_parts(data, len) }.to_vec())
    };

    send_message(srv, msg, target)
}

/// send ping message to connected clients
/// @param srv - pointer to websocket server
/// @param data - pointer to data
/// @param len - data length
/// @param target - specify target clients
#[no_mangle]
pub extern "C" fn ceammc_ws_server_send_ping(
    srv: *mut ws_server,
    data: *const u8,
    len: usize,
    target: ws_client_target,
) -> bool {
    if srv.is_null() {
        return false;
    }

    let srv = unsafe { &mut *srv };

    let msg = if data.is_null() || len == 0 {
        ClientRequest::SendPing(vec![])
    } else {
        ClientRequest::SendPing(unsafe { std::slice::from_raw_parts(data, len) }.to_vec())
    };

    send_message(srv, msg, target)
}

/// close websocket server client connections by sending them close handshake
/// @param srv - pointer to websocket server
/// @param target - specify target clients
#[no_mangle]
pub extern "C" fn ceammc_ws_server_close_clients(
    srv: *mut ws_server,
    target: ws_client_target,
) -> bool {
    if srv.is_null() {
        return false;
    }

    let srv = unsafe { &mut *srv };

    return send_message(srv, ClientRequest::Close, target);
}

/// abort all client connections without handshake
/// @param srv - pointer to websocket server
/// @param target - specify target clients
#[no_mangle]
pub extern "C" fn ceammc_ws_server_shutdown_clients(
    srv: *mut ws_server,
    target: ws_client_target,
) -> bool {
    if srv.is_null() {
        return false;
    }

    let srv = unsafe { &mut *srv };
    send_message(srv, ClientRequest::Shutdown, target)
}

/// request connected client id
/// @param srv - pointer to server
/// @param user - user data pointer to callback
/// @param cb - request callback
#[no_mangle]
pub extern "C" fn ceammc_ws_server_connected_clients(
    srv: *mut ws_server,
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, msg: *const ws_peer_info, len: usize)>,
) -> bool {
    if srv.is_null() {
        return false;
    }

    let srv = unsafe { &mut *srv };

    let data = srv
        .peer_map
        .lock()
        .unwrap()
        .iter()
        .map(|(_, info)| info.peer.clone())
        .collect::<Vec<_>>();

    let data: Vec<ws_peer_info> = data
        .iter()
        .map(|x| ws_peer_info {
            addr: x.addr.as_ptr(),
            id: x.id,
        })
        .collect();

    cb.map(|f| {
        f(user, data.as_ptr(), data.len());
    });

    true
}
