use std::{
    ffi::{c_char, c_void, CStr, CString},
    sync::mpsc::SyncSender,
    thread::JoinHandle,
};

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
    worker_handle: JoinHandle<()>,
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
            worker_handle,
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
            worker_handle,
            on_msg,
        }
    }

    pub fn stop_worker(self) {
        match self.channel.quit() {
            SendState::Ok => {}
            SendState::NoSpace => self.on_msg.error_str("no space in worker channel"),
            SendState::Disconnected => self.on_msg.error_str("worker disconnected"),
        }

        if let Err(err) = self.worker_handle.join() {
            self.on_msg.error_str(format!("{err:?}"));
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

pub fn cstr_to_string(str: *const c_char) -> Option<String> {
    if str.is_null() {
        None
    } else {
        Some(String::from(
            unsafe { CStr::from_ptr(str) }.to_string_lossy(),
        ))
    }
}

pub fn ptr_to_array<T: Clone>(data: *const T, len: usize) -> Vec<T> {
    if data.is_null() || len == 0 {
        vec![]
    } else {
        unsafe { std::slice::from_raw_parts(data, len) }.to_vec()
    }
}
