use std::{
    ffi::CString,
    os::raw::{c_char, c_void},
    ptr::null_mut,
    usize,
};

pub struct ServiceCallbackMsg {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, msg: *const c_char)>,
}

impl ServiceCallbackMsg {
    fn exec(&self, msg: &str) {
        let msg = CString::new(msg).unwrap_or_default();
        self.cb.map(|cb| cb(self.user, msg.as_ptr()));
    }
}

pub struct ServiceCallback<T> {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, data: &T)>,
}

impl<T> ServiceCallback<T> {
    fn exec(&self, data: &T) {
        self.cb.map(|cb| cb(self.user, &data));
    }
}

#[derive(Clone, Copy)]
pub struct HasReplyCallback {
    id: usize,
    cb: Option<extern "C" fn(id: usize)>,
}

impl HasReplyCallback {
    fn exec(&self) {
        self.cb.map(|f| f(self.id));
    }
}

enum Error {
    Error(String),
    Post(String),
    Debug(String),
    Log(String),
}

pub struct Service<Request: Send, Reply> {
    cb_err: ServiceCallbackMsg,
    cb_post: ServiceCallbackMsg,
    cb_debug: ServiceCallbackMsg,
    cb_log: ServiceCallbackMsg,
    cb_reply: ServiceCallback<Reply>,
    tx: tokio::sync::mpsc::Sender<Request>,
    rx: tokio::sync::mpsc::Receiver<Result<Reply, Error>>,
}

impl<Request, Reply> Service<Request, Reply>
where
    Request: Send + 'static,
    Reply: Send + 'static,
{
    fn on_error(&self, msg: &str) {
        self.cb_err.exec(msg);
    }

    fn on_post(&self, msg: &str) {
        self.cb_post.exec(msg);
    }

    fn on_debug(&self, msg: &str) {
        self.cb_debug.exec(msg);
    }

    fn on_log(&self, msg: &str) {
        self.cb_log.exec(msg);
    }

    fn on_reply(&self, reply: &Reply) {
        self.cb_reply.exec(reply);
    }

    fn send_request(&self, req: Request) -> bool {
        match self.tx.try_send(req) {
            Ok(_) => true,
            Err(_err) => false,
        }
    }

    fn process_results(&mut self) {
        while let Ok(x) = self.rx.try_recv() {
            match x {
                Ok(x) => self.on_reply(&x),
                Err(err) => match err {
                    Error::Error(msg) => self.on_error(msg.as_str()),
                    Error::Post(msg) => self.on_post(msg.as_str()),
                    Error::Debug(msg) => self.on_debug(msg.as_str()),
                    Error::Log(msg) => self.on_log(msg.as_str()),
                },
            }
        }
    }

    fn new(
        cb_err: ServiceCallbackMsg,
        cb_post: ServiceCallbackMsg,
        cb_debug: ServiceCallbackMsg,
        cb_log: ServiceCallbackMsg,
        cb_reply: ServiceCallback<Reply>,
        cb_has_reply: HasReplyCallback,
        proc_request: fn(req: Request) -> Result<Reply, Error>,
        chan_size: usize,
    ) -> Option<Self> {
        let rt = tokio::runtime::Runtime::new();
        // let rt = tokio::runtime::Builder::new_current_thread()
        //     .enable_all()
        //     .build();

        match rt {
            Ok(rt) => {
                let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<Request>(chan_size);
                let (rep_tx, rep_rx) =
                    tokio::sync::mpsc::channel::<Result<Reply, Error>>(chan_size);

                std::thread::spawn(move || {
                    rt.block_on(async move {
                        // worker thread
                        tokio::spawn(async move {
                            while let Some(task) = req_rx.recv().await {
                                println!("get task...");
                                let rep_tx = rep_tx.clone();
                                tokio::spawn(async move {
                                    match proc_request(task) {
                                        Ok(reply) => {
                                            Self::write_ok(&rep_tx, reply, cb_has_reply).await;
                                        }
                                        Err(err) => {
                                            Self::write_error(&rep_tx, err, cb_has_reply).await;
                                        }
                                    };
                                });
                            }

                            // Once all senders have gone out of scope,
                            // the `.recv()` call returns None and it will
                            // exit from the while loop and shut down the
                            // thread.
                        });

                        ()
                    });
                });

                let srv = Service::<Request, Reply> {
                    cb_err,
                    cb_post,
                    cb_debug,
                    cb_log,
                    cb_reply,
                    tx: req_tx,
                    rx: rep_rx,
                };

                Some(srv)
            }
            Err(err) => {
                cb_err.exec(format!("runtime creation error: {err}").as_str());
                None
            }
        }
    }

    async fn write_ok(
        out: &tokio::sync::mpsc::Sender<Result<Reply, Error>>,
        reply: Reply,
        reply_cb: HasReplyCallback,
    ) {
        if let Err(err) = out.send(Ok(reply)).await {
            println!(" <- send: error: {err}");
        } else {
            reply_cb.exec();
        }
    }

    async fn write_error(
        out: &tokio::sync::mpsc::Sender<Result<Reply, Error>>,
        msg: Error,
        reply_cb: HasReplyCallback,
    ) {
        if let Err(err) = out.send(Err(msg)).await {
            println!(" <- send: error: {err}");
        } else {
            reply_cb.exec();
        }
    }

    async fn write_debug(
        out: &tokio::sync::mpsc::Sender<Result<Reply, Error>>,
        msg: String,
        reply_cb: HasReplyCallback,
    ) {
        if let Err(err) = out.send(Err(Error::Debug(str))).await {
            println!(" <- send: error: {err}");
        } else {
            reply_cb.exec();
        }
    }
}

#[cfg(test)]
mod tests {
    use std::{
        ffi::CStr,
        os::raw::{c_char, c_void},
        ptr::null_mut,
        thread::sleep,
        time::Duration,
    };

    use super::Service;

    extern "C" fn err_cb(user: *mut c_void, msg: *const c_char) {
        let msg = unsafe { CStr::from_ptr(msg) }.to_str().unwrap_or_default();
        println!("error: {msg}");
    }

    #[test]
    fn test() {
        let cli = Service::<u8, String>::new(
            super::ServiceCallbackMsg {
                user: null_mut(),
                cb: Some(err_cb),
            },
            super::ServiceCallbackMsg {
                user: null_mut(),
                cb: Some(err_cb),
            },
            super::ServiceCallbackMsg {
                user: null_mut(),
                cb: Some(err_cb),
            },
            super::ServiceCallbackMsg {
                user: null_mut(),
                cb: Some(err_cb),
            },
            super::ServiceCallback {
                user: null_mut(),
                cb: None,
            },
            super::HasReplyCallback { id: 123, cb: None },
            |x| Ok(format!("str = {x}")),
            16,
        );

        assert!(cli.is_some());
        let cli = cli.unwrap();
        assert!(cli.send_request(8));
        sleep(Duration::from_secs(1));
    }
}
