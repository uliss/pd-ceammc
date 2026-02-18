use anyhow::bail;
use anyhow::Context;
use globset::GlobBuilder;
use log::debug;
use log::error;
use log::info;
use path_slash::PathBufExt as _;
use reqwest::ClientBuilder;
use reqwest::Response;
use serde::Deserialize;
use smol_str::SmolStr;
use std::borrow::Cow;
use std::ffi::c_char;
use std::ffi::CStr;
use std::ffi::CString;
use std::path::PathBuf;
use std::str::FromStr;
use std::sync::mpsc::Sender;
use std::time::Duration;
use strum::EnumMessage;
use strum::IntoEnumIterator;
use url::Url;

use crate::common_ffi::callback_notify;
use crate::rust_atom;
use crate::vlc_ffi::vlc_fileinfo;
use crate::vlc_ffi::vlc_filelist;
use crate::vlc_ffi::vlc_filelist_cb;
use crate::vlc_ffi::vlc_playlist;
use crate::vlc_ffi::vlc_playlist_cb;
use crate::vlc_ffi::vlc_playlist_item;
use crate::vlc_ffi::vlc_playlist_item_cb;
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

#[derive(Debug)]
struct PlaylistItem {
    id: u64,
    duration: u64,
    name: CString,
    uri: CString,
    current: bool,
}

impl From<&PlaylistItem> for vlc_playlist_item {
    fn from(it: &PlaylistItem) -> Self {
        Self {
            name: it.name.as_ptr(),
            uri: it.uri.as_ptr(),
            id: it.id,
            duration: it.duration,
            current: it.current,
        }
    }
}

#[derive(Debug, Default, Deserialize)]
struct JsonPlaylist {
    // ro: String,
    #[serde(rename = "type")]
    node_type: String,
    name: String,
    id: String,
    #[serde(default)]
    uri: String,
    #[serde(default)]
    duration: u64,
    #[serde(default)]
    children: Vec<JsonPlaylist>,
    #[serde(default)]
    current: String,
}

#[derive(Debug, Default, Deserialize, Clone)]
struct JsonFileInfo {
    #[serde(rename = "type")]
    file_type: String,
    #[serde(default)]
    path: String,
    #[serde(default)]
    name: String,
    #[serde(default)]
    uri: String,
    #[serde(default)]
    size: u64,
}

#[derive(Debug, Default, Deserialize, Clone)]
struct JsonFileList {
    #[serde(default)]
    element: Vec<JsonFileInfo>,
}

impl JsonFileList {
    fn filter(self: &Self, file_type: Option<&str>, glob: Option<&str>) -> Vec<FileInfo> {
        let glob = glob.and_then(|str| {
            if let Ok(glob) = GlobBuilder::new(str)
                .backslash_escape(false)
                .literal_separator(true)
                .build()
            {
                Some(glob.compile_matcher())
            } else {
                None
            }
        });

        let res = self
            .element
            .iter()
            .filter(|x| x.name != "..")
            .filter(|x| match file_type {
                Some(ft) => x.file_type == *ft,
                None => true,
            })
            .filter(|x| match &glob {
                Some(glob) => glob.is_match(x.name.clone()),
                None => true,
            })
            .map(|x| x.into())
            .collect::<Vec<FileInfo>>();

        res
    }
}

#[derive(Debug)]
struct FileInfo {
    file_type: CString,
    path: CString,
    name: CString,
    uri: CString,
    size: u64,
}

impl From<&JsonFileInfo> for FileInfo {
    fn from(fi: &JsonFileInfo) -> Self {
        Self {
            file_type: CString::new(fi.file_type.as_str()).unwrap_or_default(),
            path: CString::new(fi.path.as_str()).unwrap_or_default(),
            name: CString::new(fi.name.as_str()).unwrap_or_default(),
            uri: CString::new(fi.uri.as_str()).unwrap_or_default(),
            size: fi.size,
        }
    }
}

impl From<&FileInfo> for vlc_fileinfo {
    fn from(value: &FileInfo) -> Self {
        Self {
            uri: value.uri.as_ptr(),
            path: value.path.as_ptr(),
            name: value.name.as_ptr(),
            size: value.size,
            type_: value.file_type.to_string_lossy().as_ref().into(),
        }
    }
}

impl JsonPlaylist {
    fn find_by_name(self: &Self, name: &str) -> Vec<&JsonPlaylist> {
        let mut res = Vec::new();

        if self.name == name {
            res.push(self);
        } else {
            for x in self.children.iter() {
                for item in x.find_by_name(name) {
                    res.push(item);
                }
            }
        }

        return res;
    }

    fn flatten(self: &Self) -> Vec<PlaylistItem> {
        let mut res = Vec::new();

        if self.node_type == "leaf" {
            res.push(self.into());
        } else {
            for child in self.children.iter() {
                for x in child.flatten() {
                    res.push(x.into());
                }
            }
        }

        res
    }

    fn current_id(self: &Self) -> Option<PlaylistItem> {
        if self.node_type == "leaf" && self.current == "current" {
            return Some(self.into());
        } else {
            for child in self.children.iter() {
                let cur = child.current_id();
                if cur.is_some() {
                    return cur;
                }
            }
        }

        None
    }
}

impl From<&JsonPlaylist> for PlaylistItem {
    fn from(value: &JsonPlaylist) -> Self {
        Self {
            id: value.id.parse::<u64>().unwrap_or_default(),
            duration: value.duration,
            name: CString::new(value.name.as_str()).unwrap_or_default(),
            uri: CString::new(value.uri.as_str()).unwrap_or_default(),
            current: value.current == "current",
        }
    }
}

async fn get_response(
    cli: &reqwest::Client,
    host: &str,
    port: u16,
    pass: &str,
    json_path: &str,
) -> anyhow::Result<Response> {
    Ok(cli
        .get(format!("http://{host}:{port}/requests/{json_path}.json"))
        .basic_auth("", Some(pass))
        .send()
        .await?)
}

async fn request_playlist(
    cli: &reqwest::Client,
    host: &str,
    port: u16,
    pass: &str,
) -> anyhow::Result<JsonPlaylist> {
    let response = get_response(cli, host, port, pass, "playlist").await?;

    if response.status().is_success() {
        let data = response.text().await?;
        let playlist: JsonPlaylist =
            serde_json::from_str(&data).or_else(|err| bail!("playlist: {err}"))?;
        Ok(playlist)
    } else {
        bail!("Status: {}", response.status())
    }
}

async fn request_browse(
    cli: &reqwest::Client,
    host: &str,
    port: u16,
    pass: &str,
    path: &str,
) -> anyhow::Result<JsonFileList> {
    let mut url = Url::parse(&format!("http://{host}:{port}/requests/browse.json"))?;
    url.query_pairs_mut().append_pair("uri", path);

    let response = cli
        .get(url.as_str())
        .basic_auth("", Some(pass))
        .send()
        .await?;

    if response.status().is_success() {
        let data = response.text().await?;
        let stat: JsonFileList =
            serde_json::from_str(&data).or_else(|err| bail!("browse: {err}"))?;
        Ok(stat)
    } else {
        bail!("Status: {}", response.status())
    }
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

fn process_prefix<'a>(
    uri: (&'a str, Option<String>),
    prefixes: Vec<&str>,
    replace_pattern: &Option<PathBuf>,
) -> Result<String, (&'a str, Option<String>)> {
    if uri.1.is_some() {
        return Err(uri);
    }

    let uri = uri.0;

    match replace_pattern {
        Some(replace_pattern) => {
            for pre in prefixes {
                debug!("check for prefix: '{pre}'");
                if uri.starts_with(pre) {
                    let pattern = replace_pattern
                        .to_slash()
                        .ok_or((uri, Some(format!("{pre} slash replace error"))))?
                        .to_string();
                    let mut uri = uri.to_string();
                    uri.replace_range(0..pre.len(), &pattern);
                    return Ok(uri);
                }
            }

            Err((uri, None))
        }
        None => Err((
            uri,
            Some(format!("{} replace error", prefixes.join(" | ")).into()),
        )),
    }
}
fn process_uri(uri: &String) -> anyhow::Result<String> {
    process_prefix((&uri, None), vec!["~", "%HOME%"], &dirs::home_dir())
        .or_else(|uri| process_prefix(uri, vec!["%MUSIC%", "%AUDIO%"], &dirs::audio_dir()))
        .or_else(|uri| {
            process_prefix(
                uri,
                vec!["%VIDEO%", "%MOVIE%", "%MOVIES%"],
                &dirs::video_dir(),
            )
        })
        .or_else(|uri| {
            process_prefix(
                uri,
                vec!["%IMAGE%", "%PICTURE", "%PICTURES"],
                &dirs::picture_dir(),
            )
        })
        .or_else(|uri| {
            process_prefix(
                uri,
                vec!["%DOWNLOAD%", "%DOWNLOADS%"],
                &dirs::download_dir(),
            )
        })
        .or_else(|uri| process_prefix(uri, vec!["%DESKTOP%"], &dirs::desktop_dir()))
        .or_else(|uri| {
            process_prefix(
                uri,
                vec!["%DOC%", "%DOCS%", "%DOCUMENT%", "%DOCUMENTS%"],
                &dirs::document_dir(),
            )
        })
        .or_else(|err| match err {
            (src_uri, None) => Ok(src_uri.to_string()),
            (src_uri, Some(err)) => bail!("error processing uric'{src_uri}': {err}"),
        })
        .map(|mut uri| {
            if !uri.starts_with("file://") {
                uri.insert_str(0, "file://");
            }
            uri
        })
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
            id.to_str().map(|x| x.to_string()),
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
        VlcRequest::PlaylistAddUri(uri, play) => {
            let uri = process_uri(uri)?;
            if *play {
                let mut url = make_status_url(host, port, Some("in_play"), None, None)?;
                url.query_pairs_mut().append_pair("input", &uri);
                url
            } else {
                let mut url = make_status_url(host, port, Some("in_enqueue"), None, None)?;
                url.query_pairs_mut().append_pair("input", &uri);
                url
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
        VlcRequest::DeleteById(id) => {
            make_status_url(host, port, Some("pl_delete"), Some(id.to_string()), None)?
        }
        VlcRequest::DeleteAtPos(pos) => {
            let playlist = request_playlist(cli, host, port, pass).await?.flatten();
            let count = playlist.len();

            let index = if *pos >= 0 && (*pos as usize) < count {
                *pos as usize
            } else if (pos.abs() as usize) <= count {
                count - (pos.abs() as usize)
            } else {
                bail!("invalid item position: {pos}")
            };

            make_status_url(
                host,
                port,
                Some("pl_delete"),
                Some(
                    playlist
                        .get(index)
                        .context(format!("invalid index: {index}"))?
                        .id
                        .to_string(),
                ),
                None,
            )?
        }
        VlcRequest::DeleteByName(name) => {
            let playlist = request_playlist(cli, host, port, pass).await?;
            let items = playlist.find_by_name(name);
            if items.is_empty() {
                bail!("playlist item '{name}' not found")
            }
            for item in items {
                let url =
                    make_status_url(host, port, Some("pl_delete"), Some(item.id.clone()), None)?;
                send_get_request(cli, &url, pass).await?;
            }
            return Ok(true);
        }
        VlcRequest::GetPlaylist => {
            let playlist = request_playlist(cli, host, port, pass).await?;
            tx.send(VlcReply::Playlist(playlist.flatten()))?;
            notify.exec();
            return Ok(true);
        }
        VlcRequest::GetCurrentItem => {
            let playlist = request_playlist(cli, host, port, pass).await?;
            tx.send(VlcReply::CurrentId(playlist.current_id()))?;
            notify.exec();
            return Ok(true);
        }
        VlcRequest::Browse(opts) => {
            let uri = process_uri(&opts.uri).context("invalid URI: {uri}")?;
            debug!("uri: {uri}");
            let filelist = request_browse(cli, host, port, pass, &uri).await?.filter(
                opts.filter_type.as_ref().map(|x| x.as_str()),
                opts.match_glob.as_ref().map(|x| x.as_str()),
            );

            tx.send(VlcReply::FileList(filelist))?;
            notify.exec();
            return Ok(true);
        }
        VlcRequest::PlaylistAddDir(dir, glob) => {
            let dir = process_uri(dir).context("invalid URI: {uri}")?;
            debug!("add directory content: '{dir}' with glob pattern '{glob}'");

            let filelist = request_browse(cli, host, port, pass, &dir)
                .await?
                .filter(Some("file"), Some(glob));

            // debug!("files to add: {filelist:?}");

            for f in filelist.iter() {
                if let Ok(mut url) = make_status_url(host, port, Some("in_enqueue"), None, None) {
                    url.query_pairs_mut()
                        .append_pair("input", &f.uri.to_string_lossy());

                    debug!("add: {:?}", f.path);
                    let _ = send_get_request(cli, &url, pass).await?;
                }
            }

            tx.send(VlcReply::FileList(filelist))?;
            notify.exec();

            return Ok(true);
        }
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

    pub fn play(self: &Self, id: RustAtom) -> bool {
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

    pub fn get_playlist(self: &Self) -> bool {
        self.send(VlcRequest::GetPlaylist)
    }

    pub fn get_current(self: &Self) -> bool {
        self.send(VlcRequest::GetCurrentItem)
    }

    pub fn browse(
        self: &Self,
        uri: Option<&c_char>,
        filter_type: Option<&c_char>,
        match_glob: Option<&c_char>,
    ) -> bool {
        let uri = uri
            .map(|x| unsafe { CStr::from_ptr(x) }.to_string_lossy().to_string())
            .unwrap_or_default();

        let filter_type =
            filter_type.map(|x| unsafe { CStr::from_ptr(x) }.to_string_lossy().to_string());

        let match_glob =
            match_glob.map(|x| unsafe { CStr::from_ptr(x) }.to_string_lossy().to_string());

        self.send(VlcRequest::Browse(BrowseParams {
            uri,
            filter_type,
            match_glob,
        }))
    }

    pub fn add_uri(self: &Self, uri: Option<&c_char>, play: bool) -> bool {
        let uri = uri
            .map(|x| unsafe { CStr::from_ptr(x) }.to_string_lossy().to_string())
            .unwrap_or_default();

        self.send(VlcRequest::PlaylistAddUri(uri, play))
    }

    pub fn add_dir_files(self: &Self, dir: &c_char, glob: &c_char) -> bool {
        let dir = unsafe { CStr::from_ptr(dir) }.to_string_lossy().to_string();
        let glob = unsafe { CStr::from_ptr(glob) }
            .to_string_lossy()
            .to_string();

        self.send(VlcRequest::PlaylistAddDir(dir, glob))
    }

    pub fn seek(self: &Self, seek: RustAtom) -> bool {
        self.send(VlcRequest::Seek(seek))
    }

    pub fn delete_by_name(self: &Self, name: RustAtom) -> bool {
        match name {
            RustAtom::Str(name) => self.send(VlcRequest::DeleteByName(name)),
            _ => {
                self.client_err(format!("playlist item name expected, got: {name:?}"));
                false
            }
        }
    }

    pub fn delete_at_pos(self: &Self, pos: i32) -> bool {
        self.send(VlcRequest::DeleteAtPos(pos))
    }

    pub fn delete_by_id(self: &Self, id: u64) -> bool {
        self.send(VlcRequest::DeleteById(id))
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

    pub fn poll(
        self: &Self,
        on_msg: crate::common_ffi::callback_msg,
        on_stat: vlc_status_cb,
        on_playlist: vlc_playlist_cb,
        on_current_id: vlc_playlist_item_cb,
        on_filelist: vlc_filelist_cb,
    ) -> bool {
        while let Ok(msg) = self.rx.try_recv() {
            debug!("[client] {msg:?}");

            match msg {
                VlcReply::Error(err) => {
                    on_msg.exec(err.as_str());
                }
                VlcReply::Status(vlc_status) => {
                    on_stat.exec(&vlc_status);
                }
                VlcReply::Playlist(playlist) => {
                    let items = playlist
                        .iter()
                        .map(|x| vlc_playlist_item {
                            name: x.name.as_ptr(),
                            uri: x.uri.as_ptr(),
                            id: x.id,
                            duration: x.duration,
                            current: x.current,
                        })
                        .collect::<Vec<_>>();

                    let pl = vlc_playlist {
                        size: items.len(),
                        items: items.as_ptr(),
                    };

                    on_playlist.exec(&pl);
                }
                VlcReply::CurrentId(id) => {
                    if let Some(id) = id {
                        on_current_id.exec(&vlc_playlist_item::from(&id));
                    } else {
                        self.client_err(format!("current item not found"));
                    }
                }
                VlcReply::FileList(info) => {
                    let files = info.iter().map(|x| x.into()).collect::<Vec<vlc_fileinfo>>();

                    let filelist = vlc_filelist {
                        size: files.len(),
                        files: files.as_ptr(),
                    };

                    on_filelist.exec(&filelist)
                }
            }
        }

        true
    }
}

#[derive(Debug)]
struct BrowseParams {
    uri: String,
    filter_type: Option<String>,
    match_glob: Option<String>,
}

#[derive(Debug)]
enum VlcRequest {
    Next,
    Prev,
    Play(RustAtom),
    Pause(Option<bool>),
    Stop,
    Sort(String, vlc_sort_order),
    Empty,
    GetStatus,
    GetPlaylist,
    GetCurrentItem,
    Browse(BrowseParams),
    FullScreen(Option<bool>),
    Loop(Option<bool>),
    Repeat(Option<bool>),
    Volume(RustAtom, RustAtom),
    PlaybackRate(f32),
    PlaylistAddUri(String, bool),
    PlaylistAddDir(String, String),
    DeleteById(u64),
    DeleteAtPos(i32),
    DeleteByName(SmolStr),
    Seek(RustAtom),
}

#[derive(Debug)]
enum VlcReply {
    Error(String),
    Status(vlc_status),
    Playlist(Vec<PlaylistItem>),
    CurrentId(Option<PlaylistItem>),
    FileList(Vec<FileInfo>),
}
