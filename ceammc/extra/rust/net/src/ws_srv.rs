mod ws_srv {
    use crate::ws_cli::ws_cli::ws_rc;

    use std::{
        ffi::{CStr, CString},
        net::{TcpListener, TcpStream},
        os::raw::{c_char, c_void},
        sync::atomic::{AtomicUsize, Ordering},
    };
    use tungstenite::{accept, Message, WebSocket};

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
        id: usize,
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
        pub cb: Option<
            extern "C" fn(user: *mut c_void, msg: *const u8, len: usize, info: &ws_conn_info),
        >,
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

        fn add_client(&mut self, sock: WebSocket<TcpStream>, addr: String) {
            let addr = CString::new(addr).unwrap();
            self.clients.push(client_info {
                ws: sock,
                addr,
                id: Token::new().0,
            });
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
                        let _ = cli.ws.send(Message::Ping(data));
                    }
                    // Message::Pong(_) => todo!(),
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

    #[no_mangle]
    pub extern "C" fn ceammc_ws_server_runloop(srv: *mut ws_server) -> ws_rc {
        if srv.is_null() {
            return ws_rc::InvalidServer;
        }

        let srv = unsafe { &mut *srv };

        match srv.sock.accept() {
            Ok((stream, addr)) => {
                let addr = addr.to_string();
                println!("new client: {stream:?} {addr}");
                match accept(stream) {
                    Ok(ws) => {
                        srv.add_client(ws, addr);
                        println!("client id: {}", &srv.clients.last().unwrap().id);
                        srv.on_conn.exec(&srv.clients.last().unwrap().info());
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

        ws_rc::Ok
    }

    #[allow(non_camel_case_types)]
    #[repr(C)]
    pub enum ws_client_target {
        ALL,
        FIRST,
        LAST,
    }

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
            Ok(str) => match target {
                ws_client_target::ALL => {
                    for ci in srv.clients.iter_mut() {
                        println!("send all: {str}");
                        let _ = ci.ws.send(Message::Text(str.to_string())).map_err(|err| {
                            println!("send error: {err}");
                        });
                    }

                    ws_rc::Ok
                }
                ws_client_target::FIRST => {
                    srv.clients.get_mut(0).map(|x| {
                        let _ = x.ws.send(Message::Text(str.to_string()));
                    });
                    ws_rc::Ok
                }
                ws_client_target::LAST => {
                    srv.clients.last_mut().map(|x| {
                        let _ = x.ws.send(Message::Text(str.to_string()));
                    });
                    ws_rc::Ok
                }
            },
            Err(_) => ws_rc::InvalidMessage,
        }
    }

    #[no_mangle]
    pub extern "C" fn ceammc_ws_server_close(srv: *mut ws_server) -> ws_rc {
        if srv.is_null() {
            return ws_rc::InvalidServer;
        }

        let srv = unsafe { &mut *srv };
        srv.clients.clear();

        ws_rc::Ok
    }
}
