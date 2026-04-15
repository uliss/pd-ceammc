use std::{
    ffi::{c_char, c_void, CStr, CString},
    future::Future,
    sync::mpsc::SyncSender,
    thread::JoinHandle,
};

use log::debug;
use tokio::select;
use tokio_util::sync::CancellationToken;

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Copy)]
/// notify pd caller from other threads
pub struct msg_notify {
    /// pd subscriber id
    id: usize,
    /// non null!
    cb: extern "C" fn(id: usize),
}

impl msg_notify {
    pub fn exec(&self) {
        (self.cb)(self.id)
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
/// notify pd caller from other threads
pub struct msg_notify_int {
    /// pd subscriber id
    id: usize,
    /// non null!
    cb: extern "C" fn(id: usize, value: i32),
}

impl msg_notify_int {
    pub fn exec(&self, value: i32) {
        (self.cb)(self.id, value)
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub enum msg_level {
    Error,
    Post,
    Debug,
    Log,
}

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Copy)]
pub struct msg_cb {
    /// user data pointer
    user: *mut c_void,
    /// pd callback: non null!
    cb: extern "C" fn(user: *mut c_void, level: msg_level, msg: *const c_char),
}

impl msg_cb {
    pub fn exec(&self, msg: &WorkerMessage) {
        self.exec_cstr(&msg.msg, msg.level);
    }

    pub fn exec_cstr<T: AsRef<CStr>>(&self, str: T, level: msg_level) {
        (self.cb)(self.user, level, str.as_ref().as_ptr())
    }

    pub fn exec_str<T: AsRef<str>>(&self, str: T, level: msg_level) {
        let cstr = CString::new(str.as_ref()).unwrap_or_default();
        self.exec_cstr(&cstr, level);
    }

    pub fn error_cstr<T: AsRef<CStr>>(&self, str: T) {
        self.exec_cstr(str, msg_level::Error);
    }

    pub fn debug_cstr<T: AsRef<CStr>>(&self, str: T) {
        self.exec_cstr(str, msg_level::Debug);
    }

    pub fn post_cstr<T: AsRef<CStr>>(&self, str: T) {
        self.exec_cstr(str, msg_level::Post);
    }

    pub fn log_cstr<T: AsRef<CStr>>(&self, str: T) {
        self.exec_cstr(str, msg_level::Log);
    }

    pub fn error_str<T: AsRef<str>>(&self, str: T) {
        self.exec_str(str, msg_level::Error);
    }

    pub fn debug_str<T: AsRef<str>>(&self, str: T) {
        self.exec_str(str, msg_level::Debug);
    }

    pub fn post_str<T: AsRef<str>>(&self, str: T) {
        self.exec_str(str, msg_level::Post);
    }

    pub fn log_str<T: AsRef<str>>(&self, str: T) {
        self.exec_str(str, msg_level::Log);
    }
}

#[derive(Debug)]
pub struct WorkerMessage {
    pub msg: CString,
    pub level: msg_level,
}

impl WorkerMessage {
    pub fn error<T: AsRef<str>>(str: T) -> Self {
        Self {
            msg: CString::new(str.as_ref()).unwrap_or_default(),
            level: msg_level::Error,
        }
    }

    pub fn debug<T: AsRef<str>>(str: T) -> Self {
        Self {
            msg: CString::new(str.as_ref()).unwrap_or_default(),
            level: msg_level::Debug,
        }
    }

    pub fn post<T: AsRef<str>>(str: T) -> Self {
        Self {
            msg: CString::new(str.as_ref()).unwrap_or_default(),
            level: msg_level::Post,
        }
    }

    pub fn log<T: AsRef<str>>(str: T) -> Self {
        Self {
            msg: CString::new(str.as_ref()).unwrap_or_default(),
            level: msg_level::Log,
        }
    }
}

#[must_use]
#[derive(PartialEq)]
pub enum SendState {
    Ok,
    NoSpace,
    Disconnected,
}

impl SendState {
    pub fn to_worker_result(&self) -> Result<(), String> {
        match self {
            SendState::Ok => Ok(()),
            SendState::NoSpace => Err(format!("no in space channel")),
            SendState::Disconnected => Err(format!("client disconnected")),
        }
    }
}

pub enum RequestMessage<T> {
    Message(T),
    Quit,
}

pub enum ReplyMessage<T> {
    Message(WorkerMessage),
    Data(T),
}

pub struct WorkerChannelBounded<Request, Reply>
where
    Request: Send + 'static,
    Reply: Send + 'static,
{
    to_worker: std::sync::mpsc::SyncSender<RequestMessage<Request>>,
    to_client: std::sync::mpsc::Receiver<ReplyMessage<Reply>>,
}

impl<Request, Reply> WorkerChannelBounded<Request, Reply>
where
    Request: Send + 'static,
    Reply: Send + 'static,
{
    pub fn send(&self, req: Request) -> SendState {
        if let Err(err) = self.to_worker.try_send(RequestMessage::Message(req)) {
            match err {
                std::sync::mpsc::TrySendError::Full(_) => SendState::NoSpace,
                std::sync::mpsc::TrySendError::Disconnected(_) => SendState::Disconnected,
            }
        } else {
            SendState::Ok
        }
    }

    pub fn quit(&self) -> SendState {
        if let Err(err) = self.to_worker.try_send(RequestMessage::Quit) {
            match err {
                std::sync::mpsc::TrySendError::Full(_) => SendState::NoSpace,
                std::sync::mpsc::TrySendError::Disconnected(_) => SendState::Disconnected,
            }
        } else {
            SendState::Ok
        }
    }

    pub fn clone_sender(&self) -> std::sync::mpsc::SyncSender<RequestMessage<Request>> {
        self.to_worker.clone()
    }
}

pub struct ClientChannelBounded<Request, Reply>
where
    Request: Send + 'static,
    Reply: Send + 'static,
{
    to_client: std::sync::mpsc::SyncSender<ReplyMessage<Reply>>,
    from_worker: std::sync::mpsc::Receiver<RequestMessage<Request>>,
    notify: msg_notify,
}

impl<Request, Reply> ClientChannelBounded<Request, Reply>
where
    Request: Send + 'static,
    Reply: Send + 'static,
{
    pub fn send_data(&self, reply: Reply) -> SendState {
        self.send(ReplyMessage::Data(reply))
    }

    pub fn send_msg(&self, msg: WorkerMessage) -> SendState {
        self.send(ReplyMessage::Message(msg))
    }

    pub fn send_debug<T: AsRef<str>>(&self, msg: T) -> Result<(), String> {
        self.send_msg(WorkerMessage::debug(msg)).to_worker_result()
    }

    pub fn send_error<T: AsRef<str>>(&self, msg: T) -> Result<(), String> {
        self.send_msg(WorkerMessage::error(msg)).to_worker_result()
    }

    pub fn send_post<T: AsRef<str>>(&self, msg: T) -> Result<(), String> {
        self.send_msg(WorkerMessage::post(msg)).to_worker_result()
    }

    pub fn send(&self, msg: ReplyMessage<Reply>) -> SendState {
        if let Err(err) = self.to_client.try_send(msg) {
            log::error!("worker send error: {err}");
            match err {
                std::sync::mpsc::TrySendError::Full(_) => {
                    self.notify.exec();
                    SendState::NoSpace
                }
                std::sync::mpsc::TrySendError::Disconnected(_) => SendState::Disconnected,
            }
        } else {
            self.notify.exec();
            SendState::Ok
        }
    }

    pub fn recv_loop<F>(&self, on_request: &mut F) -> Result<(), String>
    where
        F: FnMut(Request) -> Result<(), String>,
    {
        while let Ok(req) = self.from_worker.recv() {
            match req {
                RequestMessage::Message(req) => (on_request)(req)?,
                RequestMessage::Quit => {
                    log::debug!("quit");
                    break;
                }
            }
        }

        Ok(())
    }
}

pub struct Client<Request, Reply>
where
    Request: Send + 'static,
    Reply: Send + 'static,
{
    channel: WorkerChannelBounded<Request, Reply>,
    worker_handle: Option<JoinHandle<()>>,
    on_msg: msg_cb,
}

impl<Request, Reply> Client<Request, Reply>
where
    Request: Send + 'static,
    Reply: Send + 'static,
{
    pub fn make_channel(
        size: usize,
        notify: msg_notify,
    ) -> (
        WorkerChannelBounded<Request, Reply>,
        ClientChannelBounded<Request, Reply>,
    ) {
        let (req_tx, req_rx) = std::sync::mpsc::sync_channel(size);
        let (rep_tx, rep_rx) = std::sync::mpsc::sync_channel(size);

        let to_worker = WorkerChannelBounded {
            to_worker: req_tx,
            to_client: rep_rx,
        };

        let from_worker = ClientChannelBounded {
            to_client: rep_tx,
            from_worker: req_rx,
            notify,
        };

        (to_worker, from_worker)
    }

    pub fn start_worker<F>(cb: F, size: usize, notify: msg_notify, on_msg: msg_cb) -> Self
    where
        F: Fn(&ClientChannelBounded<Request, Reply>) -> Result<(), String> + Send + 'static,
    {
        let (to_worker, to_client) = Self::make_channel(size, notify);

        let worker_handle = std::thread::spawn(move || {
            log::debug!("worker is started");

            if let Err(err) = cb(&to_client) {
                match to_client.send_msg(WorkerMessage::error(&err)) {
                    SendState::Ok => {
                        log::error!("worker error: {err}")
                    }
                    SendState::NoSpace => {
                        log::error!("no space in caller channel");
                        log::error!("worker error: {err}")
                    }
                    SendState::Disconnected => {
                        log::error!("client is disconnected");
                        log::error!("{err}")
                    }
                }
            }

            log::debug!("worker is finished")
        });

        Self {
            channel: to_worker,
            worker_handle: Some(worker_handle),
            on_msg,
        }
    }

    pub fn start_worker2<F>(cb: F, size: usize, notify: msg_notify, on_msg: msg_cb) -> Self
    where
        F: Fn(
                &ClientChannelBounded<Request, Reply>,
                SyncSender<RequestMessage<Request>>,
            ) -> Result<(), String>
            + Send
            + 'static,
    {
        let (to_worker, to_client) = Self::make_channel(size, notify);
        let sender2 = to_worker.clone_sender();

        let worker_handle = std::thread::spawn(move || {
            log::debug!("worker is started");

            if let Err(err) = cb(&to_client, sender2) {
                match to_client.send_msg(WorkerMessage::error(&err)) {
                    SendState::Ok => {
                        log::error!("worker error: {err}")
                    }
                    SendState::NoSpace => {
                        log::error!("no space in caller channel");
                        log::error!("worker error: {err}")
                    }
                    SendState::Disconnected => {
                        log::error!("client is disconnected");
                        log::error!("{err}")
                    }
                }
            }

            log::debug!("worker is finished")
        });

        Self {
            channel: to_worker,
            worker_handle: Some(worker_handle),
            on_msg,
        }
    }

    fn stop_worker(&mut self) {
        match self.channel.quit() {
            SendState::Ok => {}
            SendState::NoSpace => self.on_msg.error_str("no space in worker channel"),
            SendState::Disconnected => self.on_msg.error_str("worker disconnected"),
        }

        if let Some(handle) = self.worker_handle.take() {
            if let Err(err) = handle.join() {
                self.on_msg.error_str(format!("{err:?}"));
            }
        }
    }

    pub fn send(&self, req: Request) -> bool {
        match self.channel.send(req) {
            SendState::Ok => true,
            SendState::NoSpace => {
                self.on_msg.error_str("no space in worker channel");
                false
            }
            SendState::Disconnected => {
                self.on_msg.error_str("worker is disconnected");
                false
            }
        }
    }

    pub fn recv_loop<F>(&self, on_data: F)
    where
        F: Fn(Reply),
    {
        loop {
            match self.channel.to_client.try_recv() {
                Ok(data) => match data {
                    ReplyMessage::Message(worker_message) => self.on_msg.exec(&worker_message),
                    ReplyMessage::Data(data) => (on_data)(data),
                },
                Err(err) => {
                    match err {
                        std::sync::mpsc::TryRecvError::Empty => break,
                        std::sync::mpsc::TryRecvError::Disconnected => {
                            self.on_msg.error_str("worker is disconnected");
                        }
                    }
                    break;
                }
            }
        }
    }
}

pub struct TokioWorkerChannel<Request, Reply>
where
    Request: Send + 'static,
    Reply: Send + 'static,
{
    to_worker: tokio::sync::mpsc::Sender<RequestMessage<Request>>,
    from_worker: tokio::sync::mpsc::Receiver<ReplyMessage<Reply>>,
}

impl<Request, Reply> TokioWorkerChannel<Request, Reply>
where
    Request: Send + 'static,
    Reply: Send + 'static,
{
    pub fn send(&self, req: Request) -> SendState {
        if let Err(err) = self.to_worker.try_send(RequestMessage::Message(req)) {
            match err {
                tokio::sync::mpsc::error::TrySendError::Full(_) => SendState::NoSpace,
                tokio::sync::mpsc::error::TrySendError::Closed(_) => SendState::Disconnected,
            }
        } else {
            SendState::Ok
        }
    }

    pub fn quit(&self) -> SendState {
        if let Err(err) = self.to_worker.try_send(RequestMessage::Quit) {
            match err {
                tokio::sync::mpsc::error::TrySendError::Full(_) => SendState::NoSpace,
                tokio::sync::mpsc::error::TrySendError::Closed(_) => SendState::Disconnected,
            }
        } else {
            SendState::Ok
        }
    }
}

#[derive(Clone)]
pub struct TokioToClient<Reply>
where
    Reply: Send + 'static,
{
    sender: tokio::sync::mpsc::Sender<ReplyMessage<Reply>>,
    notify: msg_notify,
}

impl<Reply> TokioToClient<Reply>
where
    Reply: Send + 'static,
{
    pub fn send(&self, msg: ReplyMessage<Reply>) -> SendState {
        if let Err(err) = self.sender.try_send(msg) {
            log::error!("worker send error: {err}");
            match err {
                tokio::sync::mpsc::error::TrySendError::Full(_) => {
                    self.notify.exec();
                    SendState::NoSpace
                }
                tokio::sync::mpsc::error::TrySendError::Closed(_) => SendState::Disconnected,
            }
        } else {
            self.notify.exec();
            SendState::Ok
        }
    }

    pub fn send_msg(&self, msg: WorkerMessage) -> SendState {
        self.send(ReplyMessage::Message(msg))
    }

    pub fn send_data(&self, data: Reply) -> SendState {
        self.send(ReplyMessage::Data(data))
    }
}

pub struct TokioClientChannel<Request, Reply>
where
    Request: Send + 'static,
    Reply: Clone + Send + 'static,
{
    to_client: TokioToClient<Reply>,
    from_client: tokio::sync::mpsc::Receiver<RequestMessage<Request>>,
}

impl<Request, Reply> TokioClientChannel<Request, Reply>
where
    Request: Send + 'static,
    Reply: Clone + Send + 'static,
{
    pub fn send(&self, msg: ReplyMessage<Reply>) -> SendState {
        self.to_client.send(msg)
    }

    pub fn send_data(&self, data: Reply) -> SendState {
        self.to_client.send_data(data)
    }

    pub fn send_msg(&self, msg: WorkerMessage) -> SendState {
        self.to_client.send(ReplyMessage::Message(msg))
    }

    pub fn send_debug<T: AsRef<str>>(&self, msg: T) -> SendState {
        self.to_client.send_msg(WorkerMessage::debug(msg))
    }

    pub async fn recv_loop<F>(&mut self, on_request: &mut F) -> Result<(), String>
    where
        F: AsyncFnMut(Request) -> Result<(), String>,
    {
        while let Some(req) = self.from_client.recv().await {
            match req {
                RequestMessage::Message(req) => (on_request)(req).await?,
                RequestMessage::Quit => {
                    log::debug!("quit");
                    break;
                }
            }
        }

        Ok(())
    }

    pub fn clone_sender(&self) -> TokioToClient<Reply> {
        self.to_client.clone()
    }

    pub async fn spawn<F>(&self, task: F) -> tokio::task::JoinHandle<F::Output>
    where
        F: Future + Send + 'static,
        F::Output: Send + 'static,
    {
        tokio::spawn(task)
    }
}

pub struct TokioClient<Request, Reply>
where
    Request: Send + 'static,
    Reply: Clone + Send + 'static,
{
    channel: TokioWorkerChannel<Request, Reply>,
    worker_handle: Option<JoinHandle<()>>,
    on_msg: msg_cb,
    cancel_token: CancellationToken,
}

impl<Request, Reply> TokioClient<Request, Reply>
where
    Request: Send + 'static,
    Reply: Clone + Send + 'static,
{
    pub fn make_channel(
        size: usize,
        notify: msg_notify,
    ) -> (
        TokioWorkerChannel<Request, Reply>,
        TokioClientChannel<Request, Reply>,
    ) {
        let (req_tx, req_rx) = tokio::sync::mpsc::channel(size);
        let (rep_tx, rep_rx) = tokio::sync::mpsc::channel(size);

        let worker_channel = TokioWorkerChannel {
            to_worker: req_tx,
            from_worker: rep_rx,
        };

        let client_channel = TokioClientChannel {
            to_client: TokioToClient {
                sender: rep_tx,
                notify,
            },
            from_client: req_rx,
        };

        (worker_channel, client_channel)
    }

    pub fn start_worker<F>(
        num_threads: Option<u8>,
        cb: F,
        size: usize,
        notify: msg_notify,
        on_msg: msg_cb,
    ) -> Self
    where
        F: AsyncFnOnce(TokioClientChannel<Request, Reply>) -> Result<(), String> + Send + 'static,
    {
        let (worker, client) = Self::make_channel(size, notify);
        let cancel_0 = CancellationToken::new();
        let cancel_1 = cancel_0.clone();

        let worker_handle = std::thread::spawn(move || {
            log::debug!("worker is started");

            let mut runtime_builder = if num_threads.is_some() {
                tokio::runtime::Builder::new_multi_thread()
            } else {
                tokio::runtime::Builder::new_current_thread()
            };

            runtime_builder.enable_all();

            if let Some(num) = num_threads {
                if num > 0 {
                    debug!("set tokio multithread runtime with {num} worker threads");
                    runtime_builder.worker_threads(num.into());
                } else {
                    debug!("set tokio multithread runtime with default number of worker threads");
                }
            } else {
                debug!("set tokio current thread runtime");
            }

            match runtime_builder.build() {
                Ok(rt) => {
                    log::debug!("tokio create");
                    rt.block_on(async move {
                        let err_channel = client.clone_sender();
                        log::debug!("tokio start");

                        select! {
                            Err(err) = cb(client) => {
                                log::error!("worker error: {err}");
                                 match err_channel.send_msg(WorkerMessage::error(&err)) {
                                    SendState::Ok => {}
                                    SendState::NoSpace => {
                                        log::error!("no space in caller channel");
                                    }
                                    SendState::Disconnected => {
                                        log::error!("client is disconnected");
                                    }
                                }
                            }
                            _ = cancel_0.cancelled() => {
                                log::debug!("tokio cancelled");
                            }
                        }
                    });
                    log::debug!("tokio done");
                }
                Err(err) => {
                    log::error!("can't create tokio runtime: {err}")
                }
            }

            log::debug!("worker is finished")
        });

        Self {
            channel: worker,
            worker_handle: Some(worker_handle),
            on_msg,
            cancel_token: cancel_1,
        }
    }

    fn stop_worker(&mut self) {
        self.cancel_token.cancel();

        match self.channel.quit() {
            SendState::Ok => {}
            SendState::NoSpace => self.on_msg.error_str("no space in worker channel"),
            SendState::Disconnected => self.on_msg.error_str("worker disconnected"),
        }

        if let Some(handle) = self.worker_handle.take() {
            if let Err(err) = handle.join() {
                self.on_msg.error_str(format!("{err:?}"));
            }
        }
    }

    pub fn recv_loop<F>(&mut self, on_data: F)
    where
        F: Fn(Reply),
    {
        while let Ok(rep) = self.channel.from_worker.try_recv() {
            match rep {
                ReplyMessage::Message(msg) => {
                    self.on_msg.exec(&msg);
                }
                ReplyMessage::Data(reply) => on_data(reply),
            }
        }
    }

    pub fn send(&self, req: Request) -> SendState {
        self.channel.send(req)
    }
}

pub fn cstr_to_string(str: *const c_char) -> Option<String> {
    if str.is_null() {
        None
    } else {
        Some(String::from(
            unsafe { CStr::from_ptr(str) }.to_string_lossy(),
        ))
    }
}

pub fn cstr_from_string<T>(str: T) -> CString
where
    T: AsRef<str>,
{
    CString::new(str.as_ref()).unwrap_or_default()
}

pub fn ptr_to_array<T: Clone>(data: *const T, len: usize) -> Vec<T> {
    if data.is_null() || len == 0 {
        vec![]
    } else {
        unsafe { std::slice::from_raw_parts(data, len) }.to_vec()
    }
}

impl<Request, Reply> Drop for Client<Request, Reply>
where
    Request: Send + 'static,
    Reply: Send + 'static,
{
    fn drop(&mut self) {
        self.stop_worker();
    }
}

impl<Request, Reply> Drop for TokioClient<Request, Reply>
where
    Request: Send + 'static,
    Reply: Clone + Send + 'static,
{
    fn drop(&mut self) {
        self.stop_worker();
    }
}
