mod ws_srv {
    use crate::ws_cli::ws_cli::ws_callback_text;
    use crate::ws_cli::ws_cli::ws_rc;

    use std::{
        ffi::{CStr, CString},
        net::{TcpListener, TcpStream},
        os::raw::c_char,
    };
    use tungstenite::{accept, WebSocket};

    #[allow(non_camel_case_types)]
    pub struct ws_server {
        sock: TcpListener,
        on_err: ws_callback_text,
        clients: Vec<WebSocket<TcpStream>>,
    }

    impl ws_server {
        pub fn err(&self, rc: ws_rc, msg: &str) -> ws_rc {
            self.on_err.cb.map(|f| {
                let msg = CString::new(msg);
                msg.map(|str| {
                    f(self.on_err.user, str.as_ptr());
                })
            });
            rc
        }

        fn add_client(&mut self, sock: WebSocket<TcpStream>) {
            self.clients.push(sock);
        }
    }

    #[no_mangle]
    pub extern "C" fn ceammc_rs_ws_server_create(
        addr: *const c_char,
        on_err: ws_callback_text,
    ) -> *mut ws_server {
        if addr.is_null() {
            on_err.exec("null address pointer");
            return std::ptr::null_mut();
        }

        let addr = unsafe { CStr::from_ptr(addr) }.to_str();
        match addr {
            Ok(addr) => match TcpListener::bind(addr) {
                Ok(sock) => {
                    println!("bind to addr: {addr}");
                    sock.set_nonblocking(true).unwrap_or_else(|err| {
                        on_err.exec(format!("can't set non-blocking: {err}").as_str());
                    });
                    println!("set non-blocking");
                    Box::into_raw(Box::new(ws_server {
                        sock,
                        on_err,
                        clients: vec![],
                    }))
                }
                Err(err) => {
                    on_err.exec(format!("bind error: '{err}' for addr '{addr}'").as_str());
                    std::ptr::null_mut()
                }
            },
            Err(err) => {
                on_err.exec(format!("utf8 error: {err}").as_str());
                std::ptr::null_mut()
            }
        }
    }

    #[no_mangle]
    pub extern "C" fn ceammc_rs_ws_server_free(srv: *mut ws_server) {
        if !srv.is_null() {
            let _ = unsafe { Box::from_raw(srv) };
        }
    }

    fn socket_read(ws: &mut WebSocket<TcpStream>, on_err: &ws_callback_text) -> ws_rc {
        match ws.read() {
            Ok(msg) => {
                use tungstenite::Message;
                match msg {
                    Message::Text(txt) => {
                        println!("txt: {txt}");
                    }
                    // Message::Binary(_) => todo!(),
                    Message::Ping(data) => {
                        let _ = ws.send(Message::Ping(data));
                    }
                    // Message::Pong(_) => todo!(),
                    // Message::Close(_) => return ws_rc::SocketDeferClose,
                    _ => {
                        println!("unhandled message: {msg:?}");
                    }
                };
                ws_rc::Ok
            }
            Err(err) => match err {
                tungstenite::Error::ConnectionClosed => ws_rc::SocketDeferClose,
                tungstenite::Error::Io(err) => {
                    use std::io::ErrorKind::WouldBlock;

                    if err.kind() != WouldBlock {
                        on_err.exec(format!("socket read error: {err}").as_str());
                        ws_rc::SocketReadError
                    } else {
                        ws_rc::Ok
                    }
                }
                _ => {
                    on_err.exec(format!("websocket read error: {err}").as_str());
                    ws_rc::SocketReadError
                }
            },
        }
    }

    #[no_mangle]
    pub extern "C" fn ceammc_rs_ws_server_runloop(srv: *mut ws_server) -> ws_rc {
        if srv.is_null() {
            return ws_rc::InvalidServer;
        }

        let srv = unsafe { &mut *srv };

        match srv.sock.accept() {
            Ok((stream, addr)) => {
                println!("new client: {stream:?} {addr}");
                match accept(stream) {
                    Ok(ws) => srv.add_client(ws),
                    Err(err) => {
                        return srv.err(
                            ws_rc::SocketAcceptError,
                            format!("websocket accept error: {err}").as_str(),
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
                    );
                }
            }
        }

        let mut remove_clients: Vec<usize> = vec![];

        for ws in srv.clients.iter_mut().enumerate() {
            loop {
                match socket_read(ws.1, &srv.on_err) {
                    ws_rc::Ok => {}
                    _ => {
                        remove_clients.push(ws.0);
                        break;
                    }
                };
            }
        }

        for id in remove_clients.iter() {
            if *id < srv.clients.len() {
                srv.clients.remove(*id);
                println!("remove client: {}", *id);
            }
        }

        ws_rc::Ok
    }
}
