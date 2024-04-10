use std::{
    ffi::CString,
    os::raw::{c_char, c_void},
    ptr::null_mut,
    usize,
};

use log::{debug, error, info};

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

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Clone, Copy)]
pub struct callback_progress {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, msg: u8)>,
}

impl callback_progress {
    pub fn exec(&self, val: u8) {
        self.cb.map(|cb| cb(self.user, val));
    }
}

impl Default for callback_progress {
    fn default() -> Self {
        callback_progress {
            user: null_mut(),
            cb: None,
        }
    }
}

pub trait ServiceCallback<T> {
    fn exec(&self, data: &T);
}

pub enum Error {
    Error(String),
    Post(String),
    Debug(String),
    Log(String),
    Progress(u8),
}

pub struct Service<Request: Send + Sized, Reply: Sized> {
    cb_err: callback_msg,
    cb_post: callback_msg,
    cb_debug: callback_msg,
    cb_log: callback_msg,
    cb_progress: callback_progress,
    cb_reply: Box<dyn ServiceCallback<Reply>>,
    tx: tokio::sync::mpsc::Sender<Request>,
    rx: tokio::sync::mpsc::Receiver<Result<Reply, Error>>,
}

pub type CallbackSyncOne<From, To, E> = fn(req: From) -> Result<To, E>;
pub type CallbackSyncMany<From, To, E> = fn(req: From) -> Result<Vec<To>, E>;

pub type CallbackType<From, CallbackProgress, CallbackNotify, To> =
    fn(From, CallbackProgress, CallbackNotify) -> To;

pub type CallbackTypeInst<Inst, From, CallbackProgress, CallbackNotify, To> =
    fn(Inst, From, CallbackProgress, CallbackNotify) -> To;
// pub type CallbackAsyncOne<From, To, E> = fn(req: From) -> impl Future<Output = Result<Vec<To>, E>>;

impl<Request, Reply> Service<Request, Reply>
where
    Request: Send + Sized + 'static + core::fmt::Debug,
    Reply: Send + Sized + 'static + core::fmt::Debug,
{
    pub fn new(
        cb_err: callback_msg,
        cb_post: callback_msg,
        cb_debug: callback_msg,
        cb_log: callback_msg,
        cb_progress: callback_progress,
        cb_reply: Box<dyn ServiceCallback<Reply>>,
        tx: tokio::sync::mpsc::Sender<Request>,
        rx: tokio::sync::mpsc::Receiver<Result<Reply, Error>>,
    ) -> Self {
        Service::<Request, Reply> {
            cb_err,
            cb_post,
            cb_debug,
            cb_log,
            cb_progress,
            cb_reply,
            tx,
            rx,
        }
    }

    pub fn on_error(&self, msg: &str) {
        error!("{msg}");
        self.cb_err.exec(msg);
    }

    pub fn on_post(&self, msg: &str) {
        info!("{msg}");
        self.cb_post.exec(msg);
    }

    pub fn on_debug(&self, msg: &str) {
        debug!("{msg}");
        self.cb_debug.exec(msg);
    }

    pub fn on_log(&self, msg: &str) {
        self.cb_log.exec(msg);
    }

    pub fn on_progress(&self, val: u8) {
        self.cb_progress.exec(val);
    }

    fn on_reply(&self, reply: &Reply) {
        self.cb_reply.exec(reply);
    }

    pub fn send_request(&self, req: Request) -> bool {
        debug!("send_request {req:?}");
        match self.tx.try_send(req) {
            Ok(_) => true,
            Err(_err) => {
                error!("send_request error: {_err}");
                false
            }
        }
    }

    pub fn blocking_send(&self, req: Request) -> bool {
        debug!("blocking_send {req:?}");
        match self.tx.blocking_send(req) {
            Ok(_) => true,
            Err(_err) => {
                error!("send_request error: {_err}");
                false
            }
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
                    Error::Progress(val) => self.on_progress(val),
                },
            }
        }
    }

    pub fn new_sync(
        cb_err: callback_msg,
        cb_post: callback_msg,
        cb_debug: callback_msg,
        cb_log: callback_msg,
        cb_progress: callback_progress,
        cb_reply: Box<dyn ServiceCallback<Reply>>,
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
                                let rep_tx = rep_tx.clone();
                                tokio::task::spawn_blocking(move || {
                                    match proc_request(task) {
                                        Ok(reply) => {
                                            Self::sync_write_ok(&rep_tx, reply, cb_notify);
                                        }
                                        Err(err) => {
                                            Self::sync_write_error(&rep_tx, err, cb_notify);
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

                let srv = Service::<Request, Reply>::new(
                    cb_err,
                    cb_post,
                    cb_debug,
                    cb_log,
                    cb_progress,
                    cb_reply,
                    req_tx,
                    rep_rx,
                );

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
        cb_progress: callback_progress,
        cb_reply: Box<dyn ServiceCallback<Reply>>,
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

                let srv = Service::<Request, Reply>::new(
                    cb_err,
                    cb_post,
                    cb_debug,
                    cb_log,
                    cb_progress,
                    cb_reply,
                    req_tx,
                    rep_rx,
                );

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
        cb_progress: callback_progress,
        cb_reply: Box<dyn ServiceCallback<Reply>>,
        cb_notify: callback_notify,
        proc_request: CallbackType<
            Request,
            tokio::sync::mpsc::Sender<Result<Reply, Error>>,
            callback_notify,
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

                let srv = Service::<Request, Reply>::new(
                    cb_err,
                    cb_post,
                    cb_debug,
                    cb_log,
                    cb_progress,
                    cb_reply,
                    req_tx,
                    rep_rx,
                );

                std::thread::spawn(move || {
                    rt.block_on(async move {
                        // worker thread
                        tokio::spawn(async move {
                            while let Some(task) = req_rx.recv().await {
                                let rep_tx = rep_tx.clone();
                                tokio::spawn(async move {
                                    match proc_request(task, rep_tx.clone(), cb_notify).await {
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
        cb_progress: callback_progress,
        cb_reply: Box<dyn ServiceCallback<Reply>>,
        cb_notify: callback_notify,
        proc_request: CallbackType<
            Request,
            tokio::sync::mpsc::Sender<Result<Reply, Error>>,
            callback_notify,
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

                let srv = Service::<Request, Reply>::new(
                    cb_err,
                    cb_post,
                    cb_debug,
                    cb_log,
                    cb_progress,
                    cb_reply,
                    req_tx,
                    rep_rx,
                );

                std::thread::spawn(move || {
                    rt.block_on(async move {
                        // worker thread
                        let worker = tokio::spawn(async move {
                            while let Some(task) = req_rx.recv().await {
                                let rep_tx = rep_tx.clone();
                                tokio::spawn(async move {
                                    match proc_request(task, rep_tx.clone(), cb_notify).await {
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

                        let _ = worker.await;

                        ()
                    });
                });

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
            error!(" <- send: error: {err}");
        } else {
            reply_cb.exec();
        }
    }

    pub fn sync_write_ok(
        out: &tokio::sync::mpsc::Sender<Result<Reply, Error>>,
        reply: Reply,
        reply_cb: callback_notify,
    ) {
        if let Err(err) = out.blocking_send(Ok(reply)) {
            error!(" <- send: error: {err}");
        } else {
            reply_cb.exec();
        }
    }

    pub fn sync_write_error(
        out: &tokio::sync::mpsc::Sender<Result<Reply, Error>>,
        msg: Error,
        reply_cb: callback_notify,
    ) {
        if let Err(err) = out.blocking_send(Err(msg)) {
            error!(" <- send: error: {err}");
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
            error!(" <- send: error: {err}");
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
            error!(" <- send: error: {err}");
        } else {
            reply_cb.exec();
        }
    }

    pub async fn write_progress(
        out: &tokio::sync::mpsc::Sender<Result<Reply, Error>>,
        value: u8,
        cb_notify: callback_notify,
    ) {
        if let Err(err) = out.send(Err(Error::Progress(value))).await {
            error!(" <- send: error: {err}");
        } else {
            cb_notify.exec();
        }
    }
}

#[cfg(test)]
mod tests {}
