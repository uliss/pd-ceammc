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
use url::Url;

use crate::common_ffi::callback_notify;
use crate::rust_atom;
use crate::vlc_ffi::vlc_sort;
use crate::vlc_ffi::vlc_sort_order;
use crate::vlc_ffi::vlc_status;
use crate::vlc_ffi::vlc_status_cb;
use crate::RustAtom;

#[derive(Debug)]
enum VlcVolume {
    RelativePlus(u16),
    RelativeMinus(u16),
    Absolute(u16),
}

pub struct Vlc {
    tx: std::sync::mpsc::Sender<VlcRequest>,
    rx: std::sync::mpsc::Receiver<VlcReply>,
    cb: crate::common_ffi::callback_msg,
}

async fn send_get_request(
    cli: &reqwest::Client,
    url: &Url,
    pass: &String,
) -> anyhow::Result<vlc_status> {
    let response = cli
        .get(url.as_str())
        .basic_auth("", Some(pass))
        .send()
        .await?;

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

fn make_status_url(
    host: &str,
    port: u16,
    cmd: Option<&str>,
    id: Option<String>,
    value: Option<String>,
) -> anyhow::Result<Url> {
    let mut url = Url::parse(&format!("http://{host}:{port}/requests/status.json"))?;

    if let Some(cmd) = cmd {
        url.query_pairs_mut().append_pair("command", cmd);
    }

    if let Some(id) = id {
        url.query_pairs_mut().append_pair("id", &id);
    }

    if let Some(val) = value {
        url.query_pairs_mut().append_pair("val", &val);
    }

    return Ok(url);
}

fn parse_volume(v0: &RustAtom, v1: &RustAtom) -> Result<VlcVolume, String> {
    if v0 == &RustAtom::from_str("+") && v1.is_float() {
        if let Some(f) = v1.to_float() {
            return Ok(VlcVolume::RelativePlus(f as u16));
        }
    } else if v0 == &RustAtom::from_str("-") && v1.is_float() {
        if let Some(f) = v1.to_float() {
            return Ok(VlcVolume::RelativeMinus(f as u16));
        }
    } else if v0.is_float() && v1.is_null() {
        if let Some(f) = v0.to_float() {
            return Ok(VlcVolume::Absolute(f as u16));
        }
    }

    Err(format!("volume parse error"))
}

// full vlc command list is here:
// https://github.com/videolan/vlc/blob/master/share/lua/http/requests/README.txt
async fn send2vlc(
    cli: &reqwest::Client,
    host: &String,
    pass: &String,
    port: u16,
    req: VlcRequest,
    tx: &Sender<VlcReply>,
    notify: callback_notify,
) -> anyhow::Result<bool> {
    let url: Url = match &req {
        VlcRequest::Next => make_status_url(host, port, Some("pl_next"), None, None)?,
        VlcRequest::Prev => make_status_url(host, port, Some("pl_previous"), None, None)?,
        VlcRequest::Play(id) => make_status_url(
            host,
            port,
            Some("pl_previous"),
            id.map(|id| id.to_string()),
            None,
        )?,
        VlcRequest::Pause(value) => {
            if let Some(pause) = value {
                let url = make_status_url(host, port, None, None, None)?;
                let stat = send_get_request(cli, &url, pass).await?;
                if stat.state.do_pause(*pause) {
                    make_status_url(host, port, Some("pl_forcepause"), None, None)?
                } else if stat.state.do_resume(*pause) {
                    make_status_url(host, port, Some("pl_forceresume"), None, None)?
                } else {
                    return Ok(true);
                }
            } else {
                make_status_url(host, port, Some("pl_pause"), None, None)?
            }
        }
        VlcRequest::Stop => make_status_url(host, port, Some("pl_stop"), None, None)?,
        VlcRequest::Sort(sort, vlc_sort_order) => match vlc_sort::from_str(sort.as_str()) {
            Ok(sort) => make_status_url(
                host,
                port,
                Some("pl_sort"),
                Some(vlc_sort_order.get_serializations()[0].to_string()),
                Some(sort.get_serializations()[0].to_string()),
            )?,
            Err(err) => {
                let usage = vlc_sort::iter()
                    .map(|x| format!("'{}'", x.get_serializations()[0]))
                    .collect::<Vec<_>>()
                    .join(", ");

                bail!("{err:?}. Valid variants are: {usage}")
            }
        },
        VlcRequest::Empty => make_status_url(host, port, Some("pl_empty"), None, None)?,
        VlcRequest::GetStatus => make_status_url(host, port, None, None, None)?,
        VlcRequest::FullScreen(value) => match value {
            Some(value) => {
                let url = make_status_url(host, port, None, None, None)?;
                let stat = send_get_request(cli, &url, pass).await?;
                let is_fullscreen: bool = stat.fullscreen.into();
                if is_fullscreen != *value {
                    make_status_url(host, port, Some("fullscreen"), None, None)?
                } else {
                    return Ok(true);
                }
            }
            None => make_status_url(host, port, Some("fullscreen"), None, None)?,
        },
        VlcRequest::Loop(value) => {
            if let Some(value) = value {
                let url = make_status_url(host, port, None, None, None)?;
                let stat = send_get_request(cli, &url, pass).await?;
                if stat.has_loop != *value {
                    make_status_url(host, port, Some("pl_loop"), None, None)?
                } else {
                    return Ok(true);
                }
            } else {
                make_status_url(host, port, Some("pl_loop"), None, None)?
            }
        }
        VlcRequest::Repeat(value) => {
            if let Some(value) = value {
                let url = make_status_url(host, port, None, None, None)?;
                let stat = send_get_request(cli, &url, pass).await?;
                if stat.repeat != *value {
                    make_status_url(host, port, Some("pl_repeat"), None, None)?
                } else {
                    return Ok(true);
                }
            } else {
                make_status_url(host, port, Some("pl_repeat"), None, None)?
            }
        }
        VlcRequest::Volume(v0, v1) => match parse_volume(v0, v1) {
            Ok(volume) => match volume {
                VlcVolume::RelativePlus(v) => {
                    make_status_url(host, port, Some("volume"), None, Some(format!("+{v}")))?
                }
                VlcVolume::RelativeMinus(v) => {
                    make_status_url(host, port, Some("volume"), None, Some(format!("-{v}")))?
                }
                VlcVolume::Absolute(v) => {
                    make_status_url(host, port, Some("volume"), None, Some(format!("{v}")))?
                }
            },
            Err(err) => bail!("{err}"),
        },
        VlcRequest::PlaybackRate(rate) => {
            const MIN_RATE: f32 = 0.25;
            const MAX_RATE: f32 = 4.0;
            if *rate < MIN_RATE || *rate > MAX_RATE {
                bail!("invalid playback rate: {rate}, should be in [{MIN_RATE}, {MAX_RATE}] range")
            } else {
                make_status_url(host, port, Some("rate"), None, Some(format!("{rate}")))?
            }
        }
        VlcRequest::PlaylistAdd(uri, play) => {
            if *play {
                url::Url::parse(&format!(
                    "http://{host}:{port}/requests/status.json?command=in_play&input={uri}"
                ))?
            } else {
                url::Url::parse(&format!(
                    "http://{host}:{port}/requests/status.json?command=in_enqueue&input={uri}"
                ))?
            }
        }
        VlcRequest::Seek(rust_atom) => match rust_atom {
            RustAtom::Str(smol_str) => {
                make_status_url(host, port, Some("seek"), None, Some(smol_str.to_string()))?
            }
            RustAtom::Float(t_sec) => {
                make_status_url(host, port, Some("seek"), None, Some(format!("{t_sec}")))?
            }
            RustAtom::Null => bail!("seek is not specified"),
        },
    };

    info!("url: {}", url);

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

        let pass = pass.to_string();

        std::thread::spawn(move || {
            debug!("[worker] start worker thread");

            let cli = ClientBuilder::new()
                .timeout(Duration::from_millis(1000))
                .pool_max_idle_per_host(0) // do not cache vlc connections
                .tcp_keepalive(None)
                .tcp_nodelay(true)
                .build();

            debug!("[worker] http client init");

            match cli {
                Ok(cli) => {
                    debug!("[worker] start worker thread");

                    match tokio::runtime::Builder::new_current_thread()
                        .enable_all()
                        .build()
                    {
                        Ok(rt) => {
                            rt.block_on(async {
                                while let Ok(req) = req_rx.recv() {
                                    debug!("[worker] <- {req:?}");

                                    if let Err(err) = send2vlc(
                                        &cli,
                                        &host.to_string(),
                                        &pass,
                                        port,
                                        req,
                                        &rep_tx,
                                        notify,
                                    )
                                    .await
                                    {
                                        Self::worker_err(format!("{err}"), &rep_tx, notify);
                                    }
                                }
                            });
                        }
                        Err(err) => {
                            Self::worker_err(
                                format!("tokio runtime error: {err}"),
                                &rep_tx,
                                notify,
                            );
                        }
                    }
                }
                Err(err) => {
                    Self::worker_err(format!("http client error: {err}"), &rep_tx, notify);
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

    pub fn play(self: &Self, id: Option<i16>) -> bool {
        self.send(VlcRequest::Play(id))
    }

    pub fn pause(self: &Self, value: Option<bool>) -> bool {
        self.send(VlcRequest::Pause(value))
    }

    pub fn stop(self: &Self) -> bool {
        self.send(VlcRequest::Stop)
    }

    pub fn send_sort(self: &Self, sort: String, mode: vlc_sort_order) -> bool {
        self.send(VlcRequest::Sort(sort, mode))
    }

    pub fn playlisy_prev(self: &Self) -> bool {
        self.send(VlcRequest::Prev)
    }

    pub fn playlist_next(self: &Self) -> bool {
        self.send(VlcRequest::Next)
    }

    pub fn clear_playlist(self: &Self) -> bool {
        self.send(VlcRequest::Empty)
    }

    pub fn set_fullscreen(self: &Self, value: Option<bool>) -> bool {
        self.send(VlcRequest::FullScreen(value))
    }

    pub fn set_loop(self: &Self, value: Option<bool>) -> bool {
        self.send(VlcRequest::Loop(value))
    }

    pub fn set_repeat(self: &Self, value: Option<bool>) -> bool {
        self.send(VlcRequest::Repeat(value))
    }

    pub fn set_volume(self: &Self, v0: rust_atom, v1: rust_atom) -> bool {
        self.send(VlcRequest::Volume(v0.as_safe_value(), v1.as_safe_value()))
    }

    pub fn set_playback_rate(self: &Self, rate: f32) -> bool {
        self.send(VlcRequest::PlaybackRate(rate))
    }

    pub fn get_status(self: &Self) -> bool {
        self.send(VlcRequest::GetStatus)
    }

    pub fn add_uri(self: &Self, uri: Option<&c_char>, play: bool) -> bool {
        let uri = uri
            .map(|x| unsafe { CStr::from_ptr(x) }.to_string_lossy().to_string())
            .unwrap_or_default();

        self.send(VlcRequest::PlaylistAdd(uri, play))
    }

    pub fn seek(self: &Self, seek: RustAtom) -> bool {
        self.send(VlcRequest::Seek(seek))
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
    Volume(RustAtom, RustAtom),
    PlaybackRate(f32),
    PlaylistAdd(String, bool),
    Seek(RustAtom),
}

#[derive(Debug)]
enum VlcReply {
    Error(String),
    Status(vlc_status),
}
