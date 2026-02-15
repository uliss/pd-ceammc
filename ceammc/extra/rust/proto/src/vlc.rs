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
    url: &str,
    pass: &str,
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

async fn send2vlc(
    cli: &reqwest::Client,
    host: &String,
    pass: &String,
    port: u16,
    req: VlcRequest,
    tx: &Sender<VlcReply>,
) -> anyhow::Result<bool> {
    let url = match &req {
        VlcRequest::Next => format!("http//{host}:{port}/requests/status.json?command=pl_next"),
        VlcRequest::Prev => {
            format!("http://{host}:{port}/requests/status.json?command=pl_previous")
        }
        VlcRequest::Play(id) => {
            if let Some(id) = id {
                format!("http://{host}:{port}/requests/status.json?command=pl_play&id={id}")
            } else {
                format!("http://{host}:{port}/requests/status.json?command=pl_play")
            }
        }
        VlcRequest::Pause => format!("http://{host}:{port}/requests/status.json?command=pl_pause"),
        VlcRequest::Stop => format!("http://{host}:{port}/requests/status.json?command=pl_stop"),
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
        VlcRequest::Empty => format!("http://{host}:{port}/requests/status.json?command=pl_empty"),
        VlcRequest::GetStatus => format!("http://{host}:{port}/requests/status.json"),
        VlcRequest::FullScreen(value) => match value {
            Some(value) => {
                let url = format!("http://{host}:{port}/requests/status.json");
                let stat = send_get_request(cli, &url, pass).await?;
                let sx: bool = stat.fullscreen.into();
                if sx != *value {
                    format!("http://{host}:{port}/requests/status.json?command=fullscreen")
                } else {
                    return Ok(true);
                }
            }
            None => {
                format!("http://{host}:{port}/requests/status.json?command=fullscreen")
            }
        },
    };

    info!("url: {url}");

    send_get_request(cli, &url, pass)
        .await
        .and_then(|stat| match &req {
            VlcRequest::GetStatus => {
                tx.send(VlcReply::Status(stat))?;
                Ok(true)
            }
            _ => Ok(true),
        })
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
                                send2vlc(&cli, &host.to_string(), &pass, port, req, &rep_tx).await
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

    pub fn send_pause(self: &Self) -> bool {
        self.send(VlcRequest::Pause)
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
    Pause,
    Stop,
    Sort(String, vlc_sort_order),
    Empty,
    GetStatus,
    FullScreen(Option<bool>),
}

#[derive(Debug)]
enum VlcReply {
    Error(String),
    Status(vlc_status),
}
