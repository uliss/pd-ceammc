use anyhow::bail;
use log::debug;
use log::error;
use log::info;
use reqwest::ClientBuilder;
use std::borrow::Cow;
use std::ffi::c_char;
use std::ffi::CStr;
use std::str::FromStr;
use std::sync::mpsc::Sender;
use std::time::Duration;
use strum::EnumMessage;
use strum::IntoEnumIterator;

use crate::common_ffi::callback_notify;
use crate::vlc_ffi::vlc_sort;
use crate::vlc_ffi::vlc_sort_order;
use crate::vlc_ffi::vlc_status;
use crate::vlc_ffi::vlc_status_cb;

pub struct Vlc {
    tx: std::sync::mpsc::Sender<VlcRequest>,
    rx: std::sync::mpsc::Receiver<VlcReply>,
    cb: crate::common_ffi::callback_msg,
}

async fn send_get_request(
    cli: &reqwest::Client,
    url: &String,
    pass: &String,
) -> anyhow::Result<vlc_status> {
    let response = cli.get(url).basic_auth("", Some(pass)).send().await?;

    if response.status().is_success() {
        let data = response.text().await?;
        // debug!("status: {data}");
        let stat: vlc_status =
            serde_json::from_str(&data).or_else(|err| bail!("vlc json: {err}"))?;
        Ok(stat)
    } else {
        bail!("Status: {}", response.status())
    }
}

fn make_status_url(host: &str, port: u16, cmd: Option<&str>, id: Option<String>) -> String {
    let mut url = format!("http://{host}:{port}/requests/status.json");
    if let Some(cmd) = cmd {
        url += &format!("?command={cmd}");
    }

    if let Some(id) = id {
        url += &format!("&id={id}");
    }

    return url;
}

async fn send2vlc(
    cli: &reqwest::Client,
    host: &String,
    pass: &String,
    port: u16,
    req: VlcRequest,
    tx: &Sender<VlcReply>,
    notify: callback_notify,
) -> anyhow::Result<bool> {
    let url = match &req {
        VlcRequest::Next => make_status_url(host, port, Some("pl_next"), None),
        VlcRequest::Prev => make_status_url(host, port, Some("pl_previous"), None),
        VlcRequest::Play(id) => {
            make_status_url(host, port, Some("pl_previous"), id.map(|id| id.to_string()))
        }
        VlcRequest::Pause(value) => {
            if let Some(pause) = value {
                let url = make_status_url(host, port, None, None);
                let stat = send_get_request(cli, &url, pass).await?;
                if stat.state.do_pause(*pause) {
                    make_status_url(host, port, Some("pl_forcepause"), None)
                } else if stat.state.do_resume(*pause) {
                    make_status_url(host, port, Some("pl_forceresume"), None)
                } else {
                    return Ok(true);
                }
            } else {
                make_status_url(host, port, Some("pl_pause"), None)
            }
        }
        VlcRequest::Stop => make_status_url(host, port, Some("pl_stop"), None),
        VlcRequest::Sort(sort, vlc_sort_order) => match vlc_sort::from_str(sort.as_str()) {
            Ok(sort) => format!(
                "http://{host}:{port}/requests/status.json?command=pl_sort&id={}&val={}",
                vlc_sort_order.get_serializations()[0],
                sort.get_serializations()[0],
            ),
            Err(err) => {
                let usage = vlc_sort::iter()
                    .map(|x| format!("'{}'", x.get_serializations()[0]))
                    .collect::<Vec<_>>()
                    .join(", ");

                bail!("{err:?}. Valid variants are: {usage}")
            }
        },
        VlcRequest::Empty => make_status_url(host, port, Some("pl_empty"), None),
        VlcRequest::GetStatus => make_status_url(host, port, None, None),
        VlcRequest::FullScreen(value) => match value {
            Some(value) => {
                let url = make_status_url(host, port, None, None);
                let stat = send_get_request(cli, &url, pass).await?;
                let is_fullscreen: bool = stat.fullscreen.into();
                if is_fullscreen != *value {
                    make_status_url(host, port, Some("fullscreen"), None)
                } else {
                    return Ok(true);
                }
            }
            None => make_status_url(host, port, Some("fullscreen"), None),
        },
        VlcRequest::Loop(value) => {
            if let Some(value) = value {
                let url = make_status_url(host, port, None, None);
                let stat = send_get_request(cli, &url, pass).await?;
                if stat.has_loop != *value {
                    make_status_url(host, port, Some("pl_loop"), None)
                } else {
                    return Ok(true);
                }
            } else {
                make_status_url(host, port, Some("pl_loop"), None)
            }
        }
        VlcRequest::Repeat(value) => {
            if let Some(value) = value {
                let url = make_status_url(host, port, None, None);
                let stat = send_get_request(cli, &url, pass).await?;
                if stat.repeat != *value {
                    make_status_url(host, port, Some("pl_repeat"), None)
                } else {
                    return Ok(true);
                }
            } else {
                make_status_url(host, port, Some("pl_repeat"), None)
            }
        }
    };

    info!("url: {url}");

    let stat = send_get_request(cli, &url, pass).await?;
    match &req {
        VlcRequest::GetStatus => {
            tx.send(VlcReply::Status(stat))?;
            notify.exec();
            Ok(true)
        }
        _ => Ok(true),
    }
}

impl Vlc {
    pub fn new(
        host: Option<&c_char>,
        port: u16,
        pass: Option<&c_char>,
        cb: crate::common_ffi::callback_msg,
        notify: crate::common_ffi::callback_notify,
    ) -> Self {
        let (req_tx, req_rx) = std::sync::mpsc::channel::<VlcRequest>();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel::<VlcReply>();

        let host = host.map_or_else(
            || Cow::Borrowed("localhost"),
            |x| unsafe { CStr::from_ptr(x) }.to_string_lossy(),
        );

        let pass = pass.map_or_else(
            || Cow::Borrowed(""),
            |x| unsafe { CStr::from_ptr(x) }.to_string_lossy(),
        );

        debug!("pass: {pass}");
        let pass = pass.to_string();

        std::thread::spawn(move || {
            debug!("[worker] start worker thread");

            let cli = ClientBuilder::new()
                .timeout(Duration::from_millis(1000))
                .tcp_keepalive(None)
                .tcp_nodelay(true)
                .build();

            debug!("[worker] http client init");

            match cli {
                Ok(cli) => {
                    debug!("[worker] start worker thread");

                    let rt = tokio::runtime::Runtime::new().unwrap();
                    rt.block_on(async {
                        while let Ok(req) = req_rx.recv() {
                            debug!("[worker] {req:?}");

                            if let Err(err) =
                                send2vlc(&cli, &host.to_string(), &pass, port, req, &rep_tx, notify)
                                    .await
                            {
                                Self::worker_err(format!("{err}"), &rep_tx, notify);
                            }
                        }
                    });
                }
                Err(err) => {
                    notify.exec();
                    error!("{err}");
                }
            }

            debug!("[worker] done");
        });

        Vlc {
            tx: req_tx,
            rx: rep_rx,
            cb,
        }
    }

    pub fn send_play(self: &Self, id: Option<i16>) -> bool {
        self.send(VlcRequest::Play(id))
    }

    pub fn send_pause(self: &Self, value: Option<bool>) -> bool {
        self.send(VlcRequest::Pause(value))
    }

    pub fn send_stop(self: &Self) -> bool {
        self.send(VlcRequest::Stop)
    }

    pub fn send_sort(self: &Self, sort: String, mode: vlc_sort_order) -> bool {
        self.send(VlcRequest::Sort(sort, mode))
    }

    pub fn send_prev(self: &Self) -> bool {
        self.send(VlcRequest::Prev)
    }

    pub fn send_next(self: &Self) -> bool {
        self.send(VlcRequest::Next)
    }

    pub fn send_clear(self: &Self) -> bool {
        self.send(VlcRequest::Empty)
    }

    pub fn send_fullscreen(self: &Self, value: Option<bool>) -> bool {
        self.send(VlcRequest::FullScreen(value))
    }

    pub fn send_loop(self: &Self, value: Option<bool>) -> bool {
        self.send(VlcRequest::Loop(value))
    }

    pub fn send_repeat(self: &Self, value: Option<bool>) -> bool {
        self.send(VlcRequest::Repeat(value))
    }

    pub fn get_status(self: &Self) -> bool {
        self.send(VlcRequest::GetStatus)
    }

    fn send(self: &Self, req: VlcRequest) -> bool {
        debug!("send: {req:?}");

        match self.tx.send(req) {
            Ok(_) => {
                return true;
            }
            Err(err) => {
                self.client_err(err.to_string());
                return false;
            }
        }
    }

    fn client_err(self: &Self, str: String) {
        self.cb.exec(str.as_str());
    }

    fn worker_err(str: String, tx: &Sender<VlcReply>, notify: crate::common_ffi::callback_notify) {
        let str = format!("[worker] {str}");
        error!("[worker] error: {str}");
        if let Err(_err) = tx.send(VlcReply::Error(str.clone())) {
            error!("[worker] send error: {str}");
        } else {
            notify.exec();
        }
    }

    pub fn poll(self: &Self, on_msg: crate::common_ffi::callback_msg, on_stat: vlc_status_cb) {
        while let Ok(msg) = self.rx.try_recv() {
            debug!("[client] {msg:?}");

            match msg {
                VlcReply::Error(err) => {
                    on_msg.exec(err.as_str());
                }
                VlcReply::Status(vlc_status) => {
                    on_stat.exec(&vlc_status);
                }
            }
        }
    }
}

#[derive(Debug)]
enum VlcRequest {
    Next,
    Prev,
    Play(Option<i16>),
    Pause(Option<bool>),
    Stop,
    Sort(String, vlc_sort_order),
    Empty,
    GetStatus,
    FullScreen(Option<bool>),
    Loop(Option<bool>),
    Repeat(Option<bool>),
}

#[derive(Debug)]
enum VlcReply {
    Error(String),
    Status(vlc_status),
}
