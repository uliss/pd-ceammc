use std::{
    ffi::CString,
    os::raw::{c_char, c_void},
    usize,
};

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Clone)]
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

pub struct ServiceCallback<T> {
    user: *mut c_void,
    cb: Option<fn(user: *mut c_void, data: &T)>,
}

impl<T> ServiceCallback<T> {
    pub fn exec(&self, data: &T) {
        self.cb.map(|cb| cb(self.user, &data));
    }

    pub fn new(user: *mut c_void, cb: fn(user: *mut c_void, data: &T)) -> Self {
        ServiceCallback::<T> { user, cb: Some(cb) }
    }
}

pub enum Error {
    Error(String),
    Post(String),
    Debug(String),
    Log(String),
}

pub struct Service<Request: Send + Sized, Reply: Sized> {
    cb_err: callback_msg,
    cb_post: callback_msg,
    cb_debug: callback_msg,
    cb_log: callback_msg,
    cb_reply: ServiceCallback<Reply>,
    tx: tokio::sync::mpsc::Sender<Request>,
    rx: tokio::sync::mpsc::Receiver<Result<Reply, Error>>,
}

pub type CallbackSyncOne<From, To, E> = fn(req: From) -> Result<To, E>;
pub type CallbackSyncMany<From, To, E> = fn(req: From) -> Result<Vec<To>, E>;

pub type CallbackType<From, To> = fn(From) -> To;
// pub type CallbackAsyncOne<From, To, E> = fn(req: From) -> impl Future<Output = Result<Vec<To>, E>>;

impl<Request, Reply> Service<Request, Reply>
where
    Request: Send + Sized + 'static,
    Reply: Send + Sized + 'static,
{
    pub fn on_error(&self, msg: &str) {
        self.cb_err.exec(msg);
    }

    pub fn on_post(&self, msg: &str) {
        self.cb_post.exec(msg);
    }

    pub fn on_debug(&self, msg: &str) {
        self.cb_debug.exec(msg);
    }

    pub fn on_log(&self, msg: &str) {
        self.cb_log.exec(msg);
    }

    fn on_reply(&self, reply: &Reply) {
        self.cb_reply.exec(reply);
    }

    pub fn send_request(&self, req: Request) -> bool {
        match self.tx.try_send(req) {
            Ok(_) => true,
            Err(_err) => false,
        }
    }

    pub fn process_results(&mut self) {
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

    pub fn new_sync(
        cb_err: callback_msg,
        cb_post: callback_msg,
        cb_debug: callback_msg,
        cb_log: callback_msg,
        cb_reply: ServiceCallback<Reply>,
        cb_notify: callback_notify,
        proc_request: CallbackSyncOne<Request, Reply, Error>,
        chan_size: usize,
    ) -> Option<Self> {
        let rt = tokio::runtime::Runtime::new();

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
                                            Self::write_ok(&rep_tx, reply, cb_notify).await;
                                        }
                                        Err(err) => {
                                            Self::write_error(&rep_tx, err, cb_notify).await;
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

    pub fn new_sync_many(
        cb_err: callback_msg,
        cb_post: callback_msg,
        cb_debug: callback_msg,
        cb_log: callback_msg,
        cb_reply: ServiceCallback<Reply>,
        cb_notify: callback_notify,
        proc_request: CallbackSyncMany<Request, Reply, Error>,
        chan_size: usize,
    ) -> Option<Self> {
        let rt = tokio::runtime::Runtime::new();

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
                                            for x in reply {
                                                Self::write_ok(&rep_tx, x, cb_notify).await;
                                            }
                                        }
                                        Err(err) => {
                                            Self::write_error(&rep_tx, err, cb_notify).await;
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

    pub fn new_async(
        cb_err: callback_msg,
        cb_post: callback_msg,
        cb_debug: callback_msg,
        cb_log: callback_msg,
        cb_reply: ServiceCallback<Reply>,
        cb_notify: callback_notify,
        proc_request: CallbackType<
            Request,
            impl std::future::Future<Output = Result<Reply, Error>> + std::marker::Send + 'static,
        >,
        chan_size: usize,
    ) -> Option<Self> {
        let rt = tokio::runtime::Runtime::new();

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
                                    match proc_request(task).await {
                                        Ok(reply) => {
                                            Self::write_ok(&rep_tx, reply, cb_notify).await;
                                        }
                                        Err(err) => {
                                            Self::write_error(&rep_tx, err, cb_notify).await;
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

    pub fn new_async_many(
        cb_err: callback_msg,
        cb_post: callback_msg,
        cb_debug: callback_msg,
        cb_log: callback_msg,
        cb_reply: ServiceCallback<Reply>,
        cb_notify: callback_notify,
        proc_request: CallbackType<
            Request,
            impl std::future::Future<Output = Result<Vec<Reply>, Error>> + std::marker::Send + 'static,
        >,
        chan_size: usize,
    ) -> Option<Self> {
        let rt = tokio::runtime::Runtime::new();

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
                                    match proc_request(task).await {
                                        Ok(reply) => {
                                            for x in reply {
                                                Self::write_ok(&rep_tx, x, cb_notify).await;
                                            }
                                        }
                                        Err(err) => {
                                            Self::write_error(&rep_tx, err, cb_notify).await;
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

    pub async fn write_ok(
        out: &tokio::sync::mpsc::Sender<Result<Reply, Error>>,
        reply: Reply,
        reply_cb: callback_notify,
    ) {
        if let Err(err) = out.send(Ok(reply)).await {
            println!(" <- send: error: {err}");
        } else {
            reply_cb.exec();
        }
    }

    pub async fn write_error(
        out: &tokio::sync::mpsc::Sender<Result<Reply, Error>>,
        msg: Error,
        reply_cb: callback_notify,
    ) {
        if let Err(err) = out.send(Err(msg)).await {
            println!(" <- send: error: {err}");
        } else {
            reply_cb.exec();
        }
    }

    pub async fn write_debug(
        out: &tokio::sync::mpsc::Sender<Result<Reply, Error>>,
        msg: String,
        reply_cb: callback_notify,
    ) {
        if let Err(err) = out.send(Err(Error::Debug(msg))).await {
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

    extern "C" fn err_cb(_user: *mut c_void, msg: *const c_char) {
        let msg = unsafe { CStr::from_ptr(msg) }.to_str().unwrap_or_default();
        println!("error: {msg}");
    }

    #[test]
    fn test() {
        let cli = Service::<u8, String>::new_sync(
            super::callback_msg {
                user: null_mut(),
                cb: Some(err_cb),
            },
            super::callback_msg {
                user: null_mut(),
                cb: Some(err_cb),
            },
            super::callback_msg {
                user: null_mut(),
                cb: Some(err_cb),
            },
            super::callback_msg {
                user: null_mut(),
                cb: Some(err_cb),
            },
            super::ServiceCallback {
                user: null_mut(),
                cb: None,
            },
            super::callback_notify { id: 123, cb: None },
            |x| Ok(format!("str = {x}")),
            16,
        );

        assert!(cli.is_some());
        let cli = cli.unwrap();
        assert!(cli.send_request(8));
        sleep(Duration::from_secs(1));
    }
}
