#[derive(Debug, PartialEq)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub enum ws_rc {
    Ok = 0,
    InvalidClient,
    InvalidServer,
    InvalidMessage,
    InvalidData,
    InvalidClientId,
    SendError,
    NoData,
    CloseError,
    ConnectionClosed,
    NonBlockingError,
    SocketAcceptError,
    SocketReadError,
    SocketDeferClose,
    RunloopExit,
}

use std::{
    ffi::{CStr, CString},
    net::{TcpListener, TcpStream},
    os::raw::{c_char, c_void},
    sync::atomic::{AtomicUsize, Ordering},
};
use tungstenite::{accept_hdr, Message, WebSocket};

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct ws_conn_info {
    addr: *const c_char,
    id: usize,
}

const CSTR_EMPTY: &'static CStr = unsafe { CStr::from_bytes_with_nul_unchecked(b"\0") };

#[derive(Debug)]
struct Token(usize);
impl Token {
    fn new() -> Self {
        static COUNTER: AtomicUsize = AtomicUsize::new(1);
        let inner = COUNTER.fetch_add(1, Ordering::Relaxed);
        Token(inner)
    }
}

impl Default for ws_conn_info {
    fn default() -> Self {
        ws_conn_info {
            addr: CSTR_EMPTY.as_ptr(),
            id: 0,
        }
    }
}

fn defi() -> ws_conn_info {
    ws_conn_info::default()
}

#[allow(non_camel_case_types)]
struct client_info {
    ws: WebSocket<TcpStream>,
    pub addr: CString,
    pub id: usize,
}

impl client_info {
    fn info(&self) -> ws_conn_info {
        ws_conn_info {
            addr: self.addr.as_ptr(),
            id: self.id,
        }
    }
}

#[repr(C)]
#[allow(non_camel_case_types)]
pub struct ws_srv_on_cli {
    pub user: *mut c_void,
    pub cb: Option<extern "C" fn(user: *mut c_void, info: &ws_conn_info)>,
}

impl ws_srv_on_cli {
    pub fn exec(&self, info: &ws_conn_info) {
        self.cb.map(|f| {
            f(self.user, info);
        });
    }
}

#[repr(C)]
#[allow(non_camel_case_types)]
pub struct ws_srv_on_text {
    pub user: *mut c_void,
    pub cb: Option<extern "C" fn(user: *mut c_void, msg: *const c_char, info: &ws_conn_info)>,
}

impl ws_srv_on_text {
    pub fn exec(&self, msg: &str, info: &ws_conn_info) {
        self.cb.map(|f| {
            let msg = CString::new(msg);
            msg.map(|str| {
                f(self.user, str.as_ptr(), info);
            })
        });
    }
}

#[repr(C)]
#[allow(non_camel_case_types)]
pub struct ws_srv_on_data {
    pub user: *mut c_void,
    pub cb:
        Option<extern "C" fn(user: *mut c_void, msg: *const u8, len: usize, info: &ws_conn_info)>,
}

impl ws_srv_on_data {
    pub fn exec(&self, data: &Vec<u8>, info: &ws_conn_info) {
        self.cb.map(|f| {
            f(self.user, data.as_ptr(), data.len(), info);
        });
    }
}

#[allow(non_camel_case_types)]
pub struct ws_server {
    sock: TcpListener,
    on_err: ws_srv_on_text,
    on_txt: ws_srv_on_text,
    on_bin: ws_srv_on_data,
    on_ping: ws_srv_on_data,
    on_conn: ws_srv_on_cli,
    on_disc: ws_srv_on_cli,
    clients: Vec<client_info>,
}

impl ws_server {
    pub fn err(&self, rc: ws_rc, msg: &str, info: &ws_conn_info) -> ws_rc {
        self.on_err.cb.map(|f| {
            let msg = CString::new(msg);
            msg.map(|str| {
                f(self.on_err.user, str.as_ptr(), info);
            })
        });
        rc
    }

    fn add_client(&mut self, ws: WebSocket<TcpStream>, addr: String) -> ws_conn_info {
        let addr = CString::new(addr).unwrap();
        let ci = client_info {
            ws,
            addr,
            id: Token::new().0,
        };
        self.clients.push(ci);
        self.clients.last().unwrap().info()
    }
}

#[no_mangle]
pub extern "C" fn ceammc_ws_server_create(
    addr: *const c_char,
    on_err: ws_srv_on_text,
    on_txt: ws_srv_on_text,
    on_bin: ws_srv_on_data,
    on_ping: ws_srv_on_data,
    on_conn: ws_srv_on_cli,
    on_disc: ws_srv_on_cli,
) -> *mut ws_server {
    if addr.is_null() {
        on_err.exec("null address pointer", &defi());
        return std::ptr::null_mut();
    }

    let addr = unsafe { CStr::from_ptr(addr) }.to_str();
    match addr {
        Ok(addr) => match TcpListener::bind(addr) {
            Ok(sock) => {
                println!("bind to addr: {addr}");
                sock.set_nonblocking(true).unwrap_or_else(|err| {
                    on_err.exec(format!("can't set non-blocking: {err}").as_str(), &defi());
                });
                println!("set non-blocking");
                Box::into_raw(Box::new(ws_server {
                    sock,
                    on_err,
                    on_txt,
                    on_bin,
                    on_ping,
                    on_conn,
                    on_disc,
                    clients: vec![],
                }))
            }
            Err(err) => {
                on_err.exec(
                    format!("bind error: '{err}' for addr '{addr}'").as_str(),
                    &defi(),
                );
                std::ptr::null_mut()
            }
        },
        Err(err) => {
            on_err.exec(format!("utf8 error: {err}").as_str(), &defi());
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

fn socket_read(
    cli: &mut client_info,
    on_err: &ws_srv_on_text,
    on_txt: &ws_srv_on_text,
    on_bin: &ws_srv_on_data,
    on_ping: &ws_srv_on_data,
    on_disc: &ws_srv_on_cli,
) -> ws_rc {
    use tungstenite::Error;
    match cli.ws.read() {
        Ok(msg) => {
            match msg {
                Message::Text(txt) => {
                    println!("txt: {txt}");
                    on_txt.exec(txt.as_str(), &cli.info());
                }
                Message::Binary(data) => {
                    println!("bin: {data:?}");
                    on_bin.exec(&data, &cli.info());
                }
                Message::Ping(data) => {
                    println!("ping: {:?}", data);
                    on_ping.exec(&data, &cli.info());
                    let _ = cli.ws.send(Message::Pong(data));
                }
                Message::Pong(_) => {}
                Message::Close(_) => {
                    println!("close");
                    on_disc.exec(&cli.info());
                }
                _ => {
                    println!("unhandled message: {msg:?}");
                }
            };
            ws_rc::Ok
        }
        Err(err) => match err {
            Error::ConnectionClosed => ws_rc::SocketDeferClose,
            Error::AlreadyClosed => ws_rc::SocketDeferClose,
            Error::Io(err) => {
                use std::io::ErrorKind::WouldBlock;

                if err.kind() != WouldBlock {
                    on_err.exec(format!("socket read error: {err}").as_str(), &cli.info());
                    ws_rc::SocketReadError
                } else {
                    ws_rc::RunloopExit
                }
            }
            _ => {
                on_err.exec(format!("websocket read error: {err}").as_str(), &cli.info());
                ws_rc::SocketReadError
            }
        },
    }
}

/// process server events (non-blocking) and execute callbacks
/// @param srv - pointer to websocket server
#[no_mangle]
pub extern "C" fn ceammc_ws_server_process_events(srv: *mut ws_server) -> ws_rc {
    if srv.is_null() {
        return ws_rc::InvalidServer;
    }

    let srv = unsafe { &mut *srv };

    use tungstenite::handshake::server::{Request, Response};
    let callback = |req: &Request, response: Response| {
        println!("The request's path is: {}", req.uri().path());
        println!("The request's headers are:");
        for (ref header, _value) in req.headers() {
            println!("* {}: {:?}", header, _value);
        }

        Ok(response)
    };

    match srv.sock.accept() {
        Ok((stream, addr)) => {
            let addr = addr.to_string();
            println!("new client: {stream:?} {addr}");
            match accept_hdr(stream, callback) {
                Ok(ws) => {
                    let cli = srv.add_client(ws, addr);
                    println!("client id: {}", cli.id);
                    srv.on_conn.exec(&cli);
                }
                Err(err) => {
                    return srv.err(
                        ws_rc::SocketAcceptError,
                        format!("websocket accept error: {err}").as_str(),
                        &defi(),
                    )
                }
            }
        }
        Err(e) => {
            use std::io::ErrorKind::WouldBlock;

            if e.kind() != WouldBlock {
                return srv.err(
                    ws_rc::SocketAcceptError,
                    format!("socket accept error: {e}").as_str(),
                    &defi(),
                );
            }
        }
    }

    let mut remove_clients: Vec<usize> = vec![];

    for cli in srv.clients.iter_mut() {
        loop {
            match socket_read(
                cli,
                &srv.on_err,
                &srv.on_txt,
                &srv.on_bin,
                &srv.on_ping,
                &srv.on_disc,
            ) {
                ws_rc::SocketDeferClose => {
                    println!("remove client: {}", cli.id);
                    remove_clients.push(cli.id);
                    break;
                }
                ws_rc::Ok => continue,
                // ws_rc::RunloopExit => break,
                _ => break,
            };
        }
    }

    for id in remove_clients.iter() {
        srv.clients.retain(|ci| ci.id != *id);
    }

    srv.clients
        .retain(|ci| ci.ws.can_write() || ci.ws.can_read());

    ws_rc::Ok
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

fn send_message(srv: &mut ws_server, msg: Message, target: ws_client_target) -> ws_rc {
    match target.sel {
        ws_client_selector::ALL => {
            let mut has_errors = false;
            for ci in srv.clients.iter_mut() {
                println!("send to *: [{}]", ci.id);
                let _ = ci.ws.send(msg.clone()).map_err(|err| {
                    srv.on_err.exec(
                        format!("[{}] send error: {err}", ci.id).as_str(),
                        &ci.info(),
                    );
                    has_errors = true;
                });
            }
            return if has_errors {
                ws_rc::SendError
            } else {
                ws_rc::Ok
            };
        }
        ws_client_selector::FIRST => match srv.clients.first_mut() {
            Some(ci) => {
                println!("send to first: [{}]", ci.id);
                match ci.ws.send(msg) {
                    Err(err) => {
                        srv.on_err.exec(
                            format!("[{}] send error: {err}", ci.id).as_str(),
                            &ci.info(),
                        );
                        ws_rc::SendError
                    }
                    _ => ws_rc::Ok,
                }
            }
            _ => ws_rc::InvalidClientId,
        },
        ws_client_selector::LAST => match srv.clients.last_mut() {
            Some(ci) => {
                println!("send to last: [{}]", ci.id);
                match ci.ws.send(msg) {
                    Err(err) => {
                        srv.on_err.exec(
                            format!("[{}] send error: {err}", ci.id).as_str(),
                            &ci.info(),
                        );
                        ws_rc::SendError
                    }
                    _ => ws_rc::Ok,
                }
            }
            _ => ws_rc::InvalidClientId,
        },
        ws_client_selector::ID => {
            for ci in srv.clients.iter_mut() {
                if ci.id == target.id {
                    println!("send to id == [{}]", ci.id);
                    if let Err(err) = ci.ws.send(msg) {
                        srv.on_err.exec(
                            format!("[{}] send error: {err}", ci.id).as_str(),
                            &ci.info(),
                        );
                        return ws_rc::SendError;
                    }
                    break;
                }
            }
            ws_rc::Ok
        }
        ws_client_selector::EXCEPT => {
            for ci in srv.clients.iter_mut() {
                if ci.id != target.id {
                    println!("send to id != [{}]", ci.id);
                    if let Err(err) = ci.ws.send(msg.clone()) {
                        srv.on_err.exec(
                            format!("[{}] send error: {err}", ci.id).as_str(),
                            &ci.info(),
                        );
                        return ws_rc::SendError;
                    }
                    break;
                }
            }
            ws_rc::Ok
        }
    }
}

/// send text message to connected clients
/// @param srv - pointer to websocket server
/// @param msg - text message
/// @param target - specify target clients
/// @return ceammc_ws_rc
#[no_mangle]
pub extern "C" fn ceammc_ws_server_send_text(
    srv: *mut ws_server,
    msg: *const c_char,
    target: ws_client_target,
) -> ws_rc {
    if srv.is_null() {
        return ws_rc::InvalidServer;
    }

    let srv = unsafe { &mut *srv };

    if msg.is_null() {
        return ws_rc::InvalidMessage;
    }

    let msg = unsafe { CStr::from_ptr(msg) }.to_str();
    match msg {
        Ok(msg) => send_message(srv, Message::text(msg), target),
        Err(_) => ws_rc::InvalidMessage,
    }
}

/// send binary message to connected clients
/// @param srv - pointer to websocket server
/// @param data - pointer to data
/// @param len - data length
/// @param target - specify target clients
/// @return ceammc_ws_rc
#[no_mangle]
pub extern "C" fn ceammc_ws_server_send_binary(
    srv: *mut ws_server,
    data: *const u8,
    len: usize,
    target: ws_client_target,
) -> ws_rc {
    if srv.is_null() {
        return ws_rc::InvalidServer;
    }

    let srv = unsafe { &mut *srv };

    let msg = if data.is_null() || len == 0 {
        Message::Binary(vec![])
    } else {
        Message::Binary(unsafe { std::slice::from_raw_parts(data, len) }.to_vec())
    };

    send_message(srv, msg, target)
}

/// send ping message to connected clients
/// @param srv - pointer to websocket server
/// @param data - pointer to data
/// @param len - data length
/// @param target - specify target clients
/// @return ceammc_ws_rc
#[no_mangle]
pub extern "C" fn ceammc_ws_server_send_ping(
    srv: *mut ws_server,
    data: *const u8,
    len: usize,
    target: ws_client_target,
) -> ws_rc {
    if srv.is_null() {
        return ws_rc::InvalidServer;
    }

    let srv = unsafe { &mut *srv };

    let msg = if data.is_null() || len == 0 {
        Message::Ping(vec![])
    } else {
        Message::Ping(unsafe { std::slice::from_raw_parts(data, len) }.to_vec())
    };

    send_message(srv, msg, target)
}

/// close websocket server client connections by sending them close handshake
/// @param srv - pointer to websocket server
/// @param target - specify target clients
/// @return ceammc_ws_rc
#[no_mangle]
pub extern "C" fn ceammc_ws_server_close_clients(
    srv: *mut ws_server,
    target: ws_client_target,
) -> ws_rc {
    if srv.is_null() {
        return ws_rc::InvalidServer;
    }

    let srv = unsafe { &mut *srv };

    return send_message(srv, Message::Close(None), target);
}

/// abort all client connections without handshake
/// @param srv - pointer to websocket server
/// @param target - specify target clients
/// @return ceammc_ws_rc
#[no_mangle]
pub extern "C" fn ceammc_ws_server_shutdown_clients(
    srv: *mut ws_server,
    target: ws_client_target,
) -> ws_rc {
    if srv.is_null() {
        return ws_rc::InvalidServer;
    }

    let srv = unsafe { &mut *srv };

    match target.sel {
        ws_client_selector::ALL => {
            srv.clients.clear();
        }
        ws_client_selector::FIRST => {
            if srv.clients.len() > 0 {
                srv.clients.remove(0);
            }
        }
        ws_client_selector::LAST => {
            srv.clients.pop();
        }
        ws_client_selector::ID => {
            srv.clients.retain(|x| x.id != target.id);
        }
        ws_client_selector::EXCEPT => {
            srv.clients.retain(|x| x.id == target.id);
        }
    }

    ws_rc::Ok
}

/// request connected client id
/// @param srv - pointer to server
/// @param cb - request callback
/// @return ceammc_ws_rc
#[no_mangle]
pub extern "C" fn ceammc_ws_server_connected_clients(
    srv: *mut ws_server,
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, msg: *const ws_conn_info, len: usize)>,
) -> ws_rc {
    if srv.is_null() {
        return ws_rc::InvalidServer;
    }

    let srv = unsafe { &mut *srv };

    let data: Vec<ws_conn_info> = srv
        .clients
        .iter()
        .map(|x| ws_conn_info {
            addr: x.addr.as_ptr(),
            id: x.id,
        })
        .collect();

    cb.map(|f| {
        f(user, data.as_ptr(), data.len());
    });

    ws_rc::Ok
}
