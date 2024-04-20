use std::{
    collections::HashMap,
    ffi::{CStr, CString},
    fmt::Debug,
    os::raw::{c_char, c_void},
    path::{Path, PathBuf},
    ptr::{null, null_mut, slice_from_raw_parts_mut},
    slice,
};

use log::{debug, error};
use oauth2::TokenResponse;
use reqwest::Response;
use symphonia::core::{
    audio::{AudioBufferRef, Signal},
    codecs::{DecoderOptions, CODEC_TYPE_NULL},
    conv::IntoSample,
    formats::FormatOptions,
    io::MediaSourceStream,
    meta::MetadataOptions,
    probe::Hint,
};
use tempfile::NamedTempFile;
use tokio::{fs::File, io::AsyncWriteExt};
use url::Url;

#[repr(C)]
#[derive(Copy, Clone)]
pub union t_pd_rust_word {
    pub w_float: f32,
    pub w_double: f64,
    pub w_index: ::std::os::raw::c_int,
}

impl Debug for t_pd_rust_word {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "t_pd_rust_word union")
    }
}

use crate::{
    service::{callback_msg, callback_notify, callback_progress, Error, Service, ServiceCallback},
    utils,
};

const ACCESS_TOKEN_FNAME: &str = ".freesound.key";

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_search_params {
    query: *const c_char,
    filter: *const c_char,
    sort: *const c_char,
    fields: *const *const c_char,
    num_fields: usize,
    descriptors: *const *const c_char,
    num_descriptors: usize,
    page: i16,
    page_size: i8,
    normalized: bool,
    group_by_pack: bool,
}

#[derive(Debug, Default)]
pub struct SearchParams {
    query: String,
    filter: String,
    sort: String,
    group_by_pack: bool,
    page: Option<u16>,
    page_size: Option<u8>,
    fields: Vec<String>,
    descriptors: Vec<String>,
    normalized: bool,
}

impl From<freesound_search_params> for SearchParams {
    fn from(params: freesound_search_params) -> Self {
        SearchParams {
            query: unsafe { CStr::from_ptr(params.query) }
                .to_str()
                .unwrap_or_default()
                .to_owned(),
            filter: String::new(),
            sort: String::new(),
            group_by_pack: params.group_by_pack,
            page: if params.page > 0 {
                Some(params.page as u16)
            } else {
                None
            },
            page_size: if params.page_size > 0 {
                Some(params.page_size as u8)
            } else {
                None
            },
            fields: {
                let mut fields = vec![];
                if !params.fields.is_null() {
                    for f in unsafe { slice::from_raw_parts(params.fields, params.num_fields) } {
                        if f.is_null() {
                            continue;
                        }
                        fields.push(unsafe { CStr::from_ptr(*f) }.to_string_lossy().to_string());
                    }
                }
                fields
            },
            descriptors: {
                let mut descriptors = vec![];
                if !params.descriptors.is_null() {
                    for f in
                        unsafe { slice::from_raw_parts(params.descriptors, params.num_descriptors) }
                    {
                        if f.is_null() {
                            continue;
                        }
                        descriptors
                            .push(unsafe { CStr::from_ptr(*f) }.to_string_lossy().to_string());
                    }
                }
                descriptors
            },
            normalized: params.normalized,
        }
    }
}

#[derive(Debug)]
struct LoadToArray {
    id: u64,
    channel: usize,
    normalize: bool,
    access: String,
    array: String,
    alloc: Option<extern "C" fn(size: usize) -> *mut t_pd_rust_word>,
    free: Option<extern "C" fn(data: *mut t_pd_rust_word, size: usize)>,
}

#[derive(Debug)]
enum FreeSoundRequest {
    Search(SearchParams, String),
    OAuthGetCode(String, String),
    OAuthGetAccess(String, String, String),
    Me(String),
    StoreAccessToken(String, Option<String>, bool),
    LoadAccessToken(Option<String>),
    Download(u64, String, Option<String>),
    LoadToArray(LoadToArray),
}

#[derive(Debug)]
struct Access {
    token: CString,
    expires: u64,
}

#[derive(Debug)]
struct InfoMe {
    id: u64,
    str_props: HashMap<CString, CString>,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_info_me {
    /// The sound’s unique identifier.
    id: u64,
    /// str props
    str_props: *const freesound_prop_str,
    /// number of str props
    str_props_len: usize,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_prop_f64 {
    name: *const c_char,
    value: f64,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_prop_str {
    name: *const c_char,
    value: *const c_char,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_search_result {
    /// The sound’s unique identifier.
    id: u64,
    /// tags list (can be NULL)
    tags: *const *const c_char,
    /// tag list length
    tags_len: usize,
    /// numeric props
    num_props: *const freesound_prop_f64,
    /// number of numeric props
    num_props_len: usize,
    /// str props
    str_props: *const freesound_prop_str,
    /// number of str props
    str_props_len: usize,
}

#[derive(Debug)]
struct SearchResult {
    // The sound’s unique identifier.
    id: u64,
    str_map: HashMap<CString, CString>,
    num_map: HashMap<CString, f64>,
    obj_map: HashMap<CString, HashMap<CString, f64>>,
    tags: Vec<CString>,
}

impl SearchResult {
    fn from(value: &serde_json::Value) -> Result<Self, String> {
        let mut res = SearchResult {
            id: json_get_u64(value, "id")?,
            str_map: HashMap::new(),
            num_map: HashMap::new(),
            obj_map: HashMap::new(),
            tags: vec![],
        };

        for k in ["filesize", "bitdepth", "bitrate", "duration", "samplerate"] {
            if let Some((k, v)) = json_get_f64(value, k) {
                res.num_map.insert(k, v);
            }
        }

        for k in [
            "name",
            "description",
            "license",
            "type",
            "geotag",
            "pack",
            "similar_sounds",
            "analysis_stats",
            "analysis_frames",
        ] {
            if let Some((k, v)) = json_get_cstr(value, k) {
                res.str_map.insert(k, v);
            }
        }

        for k in ["analysis", "ac_analysis"] {
            value.get(k).map(|x| {
                x.as_object().map(|x| {
                    let mut data = HashMap::new();
                    for (k, v) in x {
                        v.as_f64().map(|f| {
                            data.insert(CString::new(k.clone()).unwrap_or_default(), f);
                        });
                    }
                    res.obj_map
                        .insert(CString::new(k).unwrap_or_default(), data);
                })
            });
        }

        res.tags = value
            .get("tags")
            .and_then(|x| x.as_array())
            .unwrap_or(&Vec::new())
            .iter()
            .map(|x| {
                x.as_str()
                    .map(|x| CString::new(x).unwrap_or_default())
                    .unwrap_or_default()
            })
            .collect::<Vec<CString>>();

        Ok(res)
    }
}

#[derive(Debug)]
struct SearchResults {
    count: u64,
    next: u32,
    prev: u32,
    results: Vec<SearchResult>,
}

#[derive(Debug)]
enum FreeSoundReply {
    OAuth2Url(String),
    OAuthAccess(Access),
    Info(InfoMe),
    SearchResults(SearchResults),
    Downloaded(CString, Option<CString>),
    LoadToArray(&'static mut [t_pd_rust_word], usize, CString),
}

type FreeSoundTx = tokio::sync::mpsc::Sender<Result<FreeSoundReply, Error>>;
type FreeSoundService = Service<FreeSoundRequest, FreeSoundReply>;

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_init {
    token: *const c_char,
}

#[allow(non_camel_case_types)]
pub struct freesound_client {
    service: FreeSoundService,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_result_cb {
    user: *mut c_void,
    cb_oauth_url: Option<extern "C" fn(user: *mut c_void, url: *const c_char)>,
    cb_oauth_access: Option<extern "C" fn(user: *mut c_void, token: *const c_char, expires: u64)>,
    cb_info_me: Option<extern "C" fn(user: *mut c_void, data: &freesound_info_me)>,
    cb_search_info: Option<extern "C" fn(user: *mut c_void, count: u64, prev: u32, next: u32)>,
    cb_search_result:
        Option<extern "C" fn(user: *mut c_void, i: usize, res: &freesound_search_result)>,
    cb_download:
        Option<extern "C" fn(user: *mut c_void, filename: *const c_char, array: *const c_char)>,
    cb_load: Option<
        extern "C" fn(
            user: *mut c_void,
            data: *const t_pd_rust_word,
            size: usize,
            array: *const c_char,
        ),
    >,
}

impl ServiceCallback<FreeSoundReply> for freesound_result_cb {
    fn exec(&self, data: &FreeSoundReply) {
        match data {
            FreeSoundReply::OAuth2Url(url) => {
                self.cb_oauth_url.map(|f| {
                    let url = CString::new(url.clone()).unwrap_or_default();
                    f(self.user, url.as_ptr());
                });
            }
            FreeSoundReply::OAuthAccess(acc) => {
                self.cb_oauth_access.map(|f| {
                    let token = CString::new(acc.token.clone()).unwrap_or_default();
                    f(self.user, token.as_ptr(), acc.expires);
                });
            }
            FreeSoundReply::Info(info) => {
                self.cb_info_me.map(|f| {
                    let str_props = info
                        .str_props
                        .iter()
                        .map(|(k, v)| freesound_prop_str {
                            name: k.as_ptr(),
                            value: v.as_ptr(),
                        })
                        .collect::<Vec<freesound_prop_str>>();

                    let data = freesound_info_me {
                        id: info.id,
                        str_props: str_props.as_ptr(),
                        str_props_len: str_props.len(),
                    };

                    f(self.user, &data);
                });
            }
            FreeSoundReply::SearchResults(res) => {
                self.cb_search_info.map(|f| {
                    f(self.user, res.count, res.prev, res.next);

                    self.cb_search_result.map(|f| {
                        for (i, res) in res.results.iter().enumerate() {
                            debug!("result: {res:?}");

                            let ctags = res
                                .tags
                                .iter()
                                .map(|x| x.as_ptr())
                                .collect::<Vec<*const c_char>>();

                            let num_props = res
                                .num_map
                                .iter()
                                .map(|(k, v)| freesound_prop_f64 {
                                    name: k.as_ptr(),
                                    value: *v,
                                })
                                .collect::<Vec<freesound_prop_f64>>();

                            let str_props = res
                                .str_map
                                .iter()
                                .map(|(k, v)| freesound_prop_str {
                                    name: k.as_ptr(),
                                    value: v.as_ptr(),
                                })
                                .collect::<Vec<freesound_prop_str>>();

                            let res = freesound_search_result {
                                id: res.id,
                                tags: ctags.as_ptr(),
                                tags_len: ctags.len(),
                                num_props: num_props.as_ptr(),
                                num_props_len: num_props.len(),
                                str_props: str_props.as_ptr(),
                                str_props_len: str_props.len(),
                            };

                            f(self.user, i, &res);
                        }
                    });
                });
            }
            FreeSoundReply::Downloaded(path, array) => {
                self.cb_download.map(|f| {
                    f(
                        self.user,
                        path.as_ptr(),
                        array.clone().map(|x| x.as_ptr()).unwrap_or(null_mut()),
                    );
                });
            }
            FreeSoundReply::LoadToArray(data, size, array) => {
                self.cb_load.map(|f| {
                    f(self.user, data.as_ptr(), *size, array.as_ptr());
                });
            }
        }
    }
}

async fn freesound_get(
    path: &str,
    access_token: &str,
    params: Option<SearchParams>,
) -> Result<String, String> {
    let mut url = Url::parse("https://freesound.org")
        .map_err(|e| e.to_string())?
        .join(path)
        .map_err(|e| e.to_string())?;

    if let Some(params) = params {
        url.query_pairs_mut()
            .append_pair("query", params.query.as_str())
            .finish();

        if params.normalized {
            url.query_pairs_mut()
                .append_pair("normalized", params.query.as_str())
                .finish();
        }

        if let Some(page) = params.page {
            url.query_pairs_mut()
                .append_pair("page", format!("{page}").as_str())
                .finish();
        }

        if let Some(page_size) = params.page_size {
            url.query_pairs_mut()
                .append_pair("page_size", format!("{page_size}").as_str())
                .finish();
        }

        // fields are comma separated: fields=name,score,avg_rating,license,
        if params.fields.len() > 0 {
            let mut fields = params.fields.clone();
            if !fields.contains(&"id".to_owned()) {
                // id field is required!
                fields.push("id".to_owned());
            }

            url.query_pairs_mut()
                .append_pair(
                    "fields",
                    fields
                        .iter()
                        .map(|s| s.as_str())
                        .collect::<Vec<&str>>()
                        .join(",")
                        .as_str(),
                )
                .finish();
        }

        if params.group_by_pack {
            url.query_pairs_mut()
                .append_pair("group_by_pack", "1")
                .finish();
        }

        // sound content-based descriptors are comma separated: descriptors=lowlevel.spectral_centroid,lowlevel.barkbands.mean
        if params.descriptors.len() > 0 {
            url.query_pairs_mut()
                .append_pair(
                    "descriptors",
                    params
                        .descriptors
                        .iter()
                        .map(|s| s.as_str())
                        .collect::<Vec<&str>>()
                        .join(",")
                        .as_str(),
                )
                .finish();
        }
    }

    debug!("url: {url}");

    match reqwest::Client::builder()
        .build()
        .map_err(|err| err.to_string())?
        .get(url)
        .header("Authorization", format!("Bearer {access_token}"))
        .send()
        .await
    {
        Ok(resp) => {
            if resp.status().is_success() {
                match resp.text().await {
                    Ok(body) => Ok(body),
                    Err(err) => Err(err.to_string()),
                }
            } else {
                Err(resp.status().to_string())
            }
        }
        Err(err) => Err(err.to_string()),
    }
}

fn make_oauth_client(id: &String, secret: &String) -> Result<oauth2::basic::BasicClient, String> {
    use oauth2::{basic::*, AuthUrl, ClientId, ClientSecret, RedirectUrl, TokenUrl};

    let id = ClientId::new(id.clone());
    let secret = Some(ClientSecret::new(secret.clone()));

    let auth_url = AuthUrl::new("https://freesound.org/apiv2/oauth2/authorize/".to_string())
        .map_err(|err| err.to_string())?;

    let token_url = TokenUrl::new("https://freesound.org/apiv2/oauth2/access_token/".to_string())
        .map_err(|err| err.to_string())?;

    let redirect_url = RedirectUrl::new(
        "https://freesound.org/home/app_permissions/permission_granted/".to_string(),
    )
    .map_err(|err| err.to_string())?;

    Ok(BasicClient::new(id, secret, auth_url, Some(token_url)).set_redirect_uri(redirect_url))
}

fn json_get_u64(value: &serde_json::Value, k: &str) -> Result<u64, String> {
    Ok(value
        .get(k)
        .ok_or(format!("{k} not found"))?
        .as_u64()
        .ok_or(format!("{k}: not a u64"))?)
}

fn json_get_f64(value: &serde_json::Value, k: &str) -> Option<(CString, f64)> {
    Some((CString::new(k).unwrap_or_default(), value.get(k)?.as_f64()?))
}

fn json_get_cstr(value: &serde_json::Value, k: &str) -> Option<(CString, CString)> {
    Some((
        CString::new(k).ok()?,
        CString::new(value.get(k)?.as_str()?).ok()?,
    ))
}

async fn download_response(id: u64, access: &String) -> Result<Response, String> {
    let cli = reqwest::ClientBuilder::new()
        .build()
        .map_err(|e| e.to_string())?;
    let url = format!("https://freesound.org/apiv2/sounds/{id}/download/");

    let response = cli
        .get(url)
        .header("Authorization", format!("Bearer {access}"))
        .send()
        .await
        .map_err(|e| e.to_string())?;

    if !response.status().is_success() {
        return Err(format!("{}: {}", response.status(), response.url()));
    }

    Ok(response)
}

fn make_download_path(response: &Response, base_dir: Option<String>) -> Result<PathBuf, String> {
    let mut path = PathBuf::from(
        base_dir
            .or(homedir::get_my_home()
                .map_err(|e| e.to_string())?
                .map(|x| x.to_str().unwrap_or_default().to_owned()))
            .ok_or("can't get home directory")?,
    );

    let fname = PathBuf::from(
        response
            .headers()
            .get(reqwest::header::CONTENT_DISPOSITION)
            .and_then(|x| Some(x.to_str().map_err(|e| e.to_string()).ok()?))
            .and_then(|x| utils::parse_content_disposition_filename(x))
            .ok_or("can't determine filename from content disposition")?,
    )
    .file_name()
    .and_then(|x| x.to_str())
    .ok_or("can't determine filename")?
    .to_string();

    path.push(fname);
    Ok(path)
}

fn decode_file(path: &Path, channel: usize, normalize: bool) -> Result<Vec<f32>, String> {
    let file = std::fs::File::open(&path).map_err(|e| e.to_string())?;

    // Create the media source stream.
    let mss = MediaSourceStream::new(Box::new(file), Default::default());

    // Create a probe hint using the file's extension. [Optional]
    let mut hint = Hint::new();
    if let Some(ext) = path.extension() {
        hint.with_extension(ext.to_string_lossy().to_string().as_str());
    }

    // Use the default options for metadata and format readers.
    let meta_opts: MetadataOptions = Default::default();
    let fmt_opts: FormatOptions = Default::default();

    // Probe the media source.
    let mut info = symphonia::default::get_probe()
        .format(&hint, mss, &fmt_opts, &meta_opts)
        .map_err(|e| e.to_string())?;

    eprintln!("{:?}", info.format.tracks());
    let track = info
        .format
        .tracks()
        .iter()
        .find(|t| t.codec_params.codec != CODEC_TYPE_NULL)
        .ok_or_else(|| format!("no supported codec found for {path:?}"))?;

    if let Some(ch) = track.codec_params.channels {
        if channel >= ch.count() {
            return Err(format!("invalid requested channel: {}", channel));
        }
    }

    // Use the default options for the decoder.
    let dec_opts: DecoderOptions = Default::default();

    // Create a decoder for the track.
    let mut decoder = symphonia::default::get_codecs()
        .make(&track.codec_params, &dec_opts)
        .map_err(|e| e.to_string())?;

    // Store the track identifier, it will be used to filter packets.
    let track_id = track.id;

    let mut data: Vec<f32> = vec![];

    // The decode loop.
    loop {
        // Get the next packet from the media format.
        let packet = match info.format.next_packet() {
            Ok(packet) => packet,
            Err(err) => match err {
                Error::IoError(err) => {
                    if err.kind() == std::io::ErrorKind::UnexpectedEof {
                        break;
                    } else {
                        return Err(format!("some error: {err}"));
                    }
                }
                Error::DecodeError(_) => continue,
                err => {
                    // An unrecoverable error occured, halt decoding.
                    return Err(format!("some error: {err}"));
                }
            },
        };

        // If the packet does not belong to the selected track, skip over it.
        if packet.track_id() != track_id {
            continue;
        }

        use symphonia::core::errors::Error;

        // Decode the packet into audio samples.
        match decoder.decode(&packet) {
            Ok(decoded) => {
                if channel >= decoded.spec().channels.count() {
                    return Err(format!("invalid requested channel: {}", channel));
                }

                match decoded {
                    AudioBufferRef::F32(buf) => {
                        buf.as_ref()
                            .chan(channel)
                            .iter()
                            .for_each(|s| data.push(*s));
                    }
                    AudioBufferRef::F64(buf) => {
                        buf.as_ref()
                            .chan(channel)
                            .iter()
                            .for_each(|s| data.push(IntoSample::<f32>::into_sample(*s)));
                    }
                    AudioBufferRef::U8(buf) => {
                        buf.as_ref()
                            .chan(channel)
                            .iter()
                            .for_each(|s| data.push(IntoSample::<f32>::into_sample(*s)));
                    }
                    AudioBufferRef::U16(buf) => {
                        buf.as_ref()
                            .chan(channel)
                            .iter()
                            .for_each(|s| data.push(IntoSample::<f32>::into_sample(*s)));
                    }
                    AudioBufferRef::U24(buf) => {
                        buf.as_ref()
                            .chan(channel)
                            .iter()
                            .for_each(|s| data.push(IntoSample::<f32>::into_sample(*s)));
                    }
                    AudioBufferRef::U32(buf) => {
                        buf.as_ref()
                            .chan(channel)
                            .iter()
                            .for_each(|s| data.push(IntoSample::<f32>::into_sample(*s)));
                    }
                    AudioBufferRef::S8(buf) => {
                        buf.as_ref()
                            .chan(channel)
                            .iter()
                            .for_each(|s| data.push(IntoSample::<f32>::into_sample(*s)));
                    }
                    AudioBufferRef::S16(buf) => {
                        buf.as_ref()
                            .chan(channel)
                            .iter()
                            .for_each(|s| data.push(IntoSample::<f32>::into_sample(*s)));
                    }
                    AudioBufferRef::S24(buf) => {
                        buf.as_ref()
                            .chan(channel)
                            .iter()
                            .for_each(|s| data.push(IntoSample::<f32>::into_sample(*s)));
                    }
                    AudioBufferRef::S32(buf) => {
                        buf.as_ref()
                            .chan(channel)
                            .iter()
                            .for_each(|s| data.push(IntoSample::<f32>::into_sample(*s)));
                    }
                }
            }
            // Consume the decoded audio samples (see below).
            Err(Error::IoError(err)) => {
                if err.kind() == std::io::ErrorKind::UnexpectedEof {
                    break;
                } else {
                    return Err(format!("io error: {err}"));
                }
            }
            Err(Error::DecodeError(_)) => {
                // The packet failed to decode due to invalid data, skip the packet.
                continue;
            }
            Err(err) => {
                return Err(format!("error: {err}"));
            }
        }
    }

    if data.is_empty() {
        return Err(format!("empty result"));
    }

    if normalize {
        let max = (*data.iter().max_by(|a, b| a.total_cmp(b)).unwrap()).abs();
        if max > 0.0 {
            for x in data.iter_mut() {
                *x /= max;
            }
        }
    }

    Ok(data)
}

async fn download_file(
    response: &mut Response,
    path: &Path,
    tx: &FreeSoundTx,
    cb_notify: &callback_notify,
) -> Result<(), String> {
    FreeSoundService::write_debug(&tx, format!("file: {path:?}"), *cb_notify).await;

    match File::create(&path).await {
        Ok(mut file) => {
            let nbytes = response.content_length();
            let total: u64 = nbytes.unwrap_or(0);
            let mut bytes_send: u64 = 0;

            while let Ok(chunk) = response.chunk().await {
                if let Some(bytes) = chunk {
                    file.write_all(&bytes).await.map_err(|e| e.to_string())?;

                    if total > 0 {
                        bytes_send += bytes.len() as u64;
                        let perc = (100 * bytes_send) / total;
                        debug!("done: {perc}%");
                        if !FreeSoundService::write_progress(&tx, perc as u8, *cb_notify).await {
                            break;
                        }
                    }
                } else {
                    break;
                }
            }

            file.flush().await.map_err(|e| e.to_string())?;
            Ok(())
        }
        Err(err) => Err(err.to_string()),
    }
}

async fn process_request(
    req: FreeSoundRequest,
    tx: &FreeSoundTx,
    cb_notify: &callback_notify,
) -> Result<(), String> {
    match req {
        FreeSoundRequest::Me(access_token) => {
            let info = freesound_get("/apiv2/me/", access_token.as_str(), None)
                .await
                .and_then(|res| {
                    serde_json::from_str::<serde_json::Value>(res.as_str())
                        .map_err(|err| err.to_string())
                })
                .and_then(|json| {
                    debug!("{json}");
                    let mut info = InfoMe {
                        id: json_get_u64(&json, "unique_id")?,
                        str_props: HashMap::new(),
                    };

                    for k in ["username", "email", "home_page", "url", "sounds", "packs"] {
                        json.get(k).map(|x| {
                            x.as_str().map(|v| {
                                info.str_props.insert(
                                    CString::new(k).unwrap_or_default(),
                                    CString::new(v).unwrap_or_default(),
                                );
                            })
                        });
                    }

                    Ok(info)
                })?;

            FreeSoundService::write_ok(&tx, FreeSoundReply::Info(info), *cb_notify).await;
            Ok(())
        }
        FreeSoundRequest::Search(params, access_token) => {
            let info = freesound_get("/apiv2/search/text/", access_token.as_str(), Some(params))
                .await
                .and_then(|res| {
                    serde_json::from_str::<serde_json::Value>(res.as_str())
                        .map_err(|err| err.to_string())
                })
                .and_then(|json| {
                    let count = json.get("count").and_then(|x| x.as_u64()).unwrap_or(0);
                    let prev = json
                        .get("previous")
                        .and_then(|x| x.as_str())
                        .and_then(|x| {
                            Url::parse(x)
                                .ok()?
                                .query_pairs()
                                .find(|(k, _)| k == "page")
                                .map(|kv| kv.1.to_string())
                                .map(|n| u32::from_str_radix(n.as_str(), 10).ok())?
                        })
                        .unwrap_or(0);

                    let next = json
                        .get("next")
                        .and_then(|x| x.as_str())
                        .and_then(|x| {
                            Url::parse(x)
                                .ok()?
                                .query_pairs()
                                .find(|(k, _)| k == "page")
                                .map(|kv| kv.1.to_string())
                                .map(|n| u32::from_str_radix(n.as_str(), 10).ok())?
                        })
                        .unwrap_or(0);

                    let mut res = SearchResults {
                        count,
                        next,
                        prev,
                        results: vec![],
                    };

                    json.get("results").and_then(|x| x.as_array()).map(|x| {
                        for value in x.iter().filter(|x| x.is_object()) {
                            debug!("{value}");
                            if let Ok(x) = SearchResult::from(value) {
                                res.results.push(x);
                            }
                        }
                    });

                    Ok(res)
                })?;

            FreeSoundService::write_ok(&tx, FreeSoundReply::SearchResults(info), *cb_notify).await;
            Ok(())
        }
        FreeSoundRequest::OAuthGetCode(id, secret) => {
            let url = make_oauth_client(&id, &secret)
                .and_then(|cli| {
                    use oauth2::{CsrfToken, Scope};

                    debug!("{cli:?}");

                    Ok(cli
                        .authorize_url(CsrfToken::new_random)
                        .add_scope(Scope::new("read".to_string()))
                        .add_scope(Scope::new("write".to_string()))
                        .url()
                        .0)
                })?
                .to_string();

            FreeSoundService::write_ok(&tx, FreeSoundReply::OAuth2Url(url), *cb_notify).await;
            Ok(())
        }
        FreeSoundRequest::OAuthGetAccess(id, secret, code) => {
            use oauth2::AuthorizationCode;

            match make_oauth_client(&id, &secret) {
                Ok(cli) => {
                    let acc = cli
                        .exchange_code(AuthorizationCode::new(code))
                        .request_async(oauth2::reqwest::async_http_client)
                        .await
                        .map(|res| Access {
                            token: CString::new(res.access_token().secret().trim())
                                .unwrap_or_default(),
                            expires: res.expires_in().unwrap_or_default().as_secs(),
                        })
                        .map_err(|err| err.to_string())?;

                    FreeSoundService::write_ok(&tx, FreeSoundReply::OAuthAccess(acc), *cb_notify)
                        .await;
                    Ok(())
                }
                Err(err) => Err(err),
            }
        }
        FreeSoundRequest::StoreAccessToken(key, base_dir, overwrite) => {
            let mut path = PathBuf::from(
                &base_dir
                    .or(homedir::get_my_home()
                        .map_err(|e| e.to_string())?
                        .map(|x| x.to_str().unwrap_or_default().to_owned()))
                    .ok_or("can't get home directory")?,
            );

            path.push(ACCESS_TOKEN_FNAME);
            if path.exists() && !overwrite {
                return Err(format!(
                    "file already exists: '{}', use overwrite flag to force overwriting",
                    path.to_string_lossy()
                ));
            }

            tokio::fs::write(path.as_path(), key)
                .await
                .map_err(|e| e.to_string())?;
            Ok(FreeSoundService::write_debug(
                &tx,
                format!("access token saved to: {}", path.to_string_lossy()),
                *cb_notify,
            )
            .await)
        }
        FreeSoundRequest::LoadAccessToken(base_dir) => {
            let mut path = PathBuf::from(
                &base_dir
                    .or(homedir::get_my_home()
                        .map_err(|e| e.to_string())?
                        .map(|x| x.to_str().unwrap_or_default().to_owned()))
                    .ok_or("can't get home directory")?,
            );

            path.push(ACCESS_TOKEN_FNAME);

            let access_token = tokio::fs::read_to_string(path.as_path())
                .await
                .map_err(|e| e.to_string())?;

            FreeSoundService::write_ok(
                &tx,
                FreeSoundReply::OAuthAccess(Access {
                    token: CString::new(access_token.trim_end()).map_err(|e| e.to_string())?,
                    expires: 0,
                }),
                *cb_notify,
            )
            .await;

            Ok(FreeSoundService::write_debug(
                &tx,
                format!("access token loaded from: {}", path.to_string_lossy()),
                *cb_notify,
            )
            .await)
        }
        FreeSoundRequest::Download(id, access, base_dir) => {
            let mut response = download_response(id, &access).await?;
            let path = make_download_path(&response, base_dir)?;
            download_file(&mut response, &path, tx, cb_notify).await?;

            FreeSoundService::write_ok(
                &tx,
                FreeSoundReply::Downloaded(
                    CString::new(path.to_string_lossy().to_string()).unwrap_or_default(),
                    None,
                ),
                *cb_notify,
            )
            .await;
            Ok(())
        }
        FreeSoundRequest::LoadToArray(load) => {
            let mut response = download_response(load.id, &load.access).await?;

            let path = NamedTempFile::new()
                .map_err(|e| e.to_string())?
                .path()
                .to_path_buf();

            download_file(&mut response, &path, tx, cb_notify).await?;

            let data = decode_file(&path, load.channel, load.normalize)?;
            debug!("loaded {} samples", data.len());

            let alloc = load
                .alloc
                .ok_or_else(|| "NULL alloc fn pointer".to_owned())?;
            let free = load.free.ok_or_else(|| "NULL free fn pointer".to_owned())?;
            let pd_data = alloc(data.len());
            if pd_data.is_null() {
                return Err(format!("can't allocate memory: {}", data.len()));
            }

            let slice = unsafe { &mut *slice_from_raw_parts_mut(pd_data, data.len()) };
            // copy data to allocated memory
            for (src, dest) in data.iter().zip(slice.into_iter()) {
                dest.w_float = *src;
            }

            if !FreeSoundService::write_ok(
                &tx,
                FreeSoundReply::LoadToArray(
                    slice,
                    slice.len(),
                    CString::new(load.array).unwrap_or_default(),
                ),
                *cb_notify,
            )
            .await
            {
                free(pd_data, data.len());
            }

            Ok(())
        }
    }
}

#[must_use]
#[no_mangle]
/// create new freesound client
/// @param params - connection params
/// @param cb_err - called in the main thread on error message
/// @param cb_post - called in the main thread on post message
/// @param cb_debug - called in the main thread on debug message
/// @param cb_log - called in the main thread on log message
/// @param cb_progress - called in the main thread on progress message
/// @param cb_reply - called in the main thread on result reply message
/// @param cb_notify - called in the worker thread (!) to notify main thread
/// @return pointer to new client or NULL on error
pub extern "C" fn ceammc_freesound_new(
    params: freesound_init,
    cb_err: callback_msg,
    cb_post: callback_msg,
    cb_debug: callback_msg,
    cb_log: callback_msg,
    cb_progress: callback_progress,
    cb_reply: freesound_result_cb,
    cb_notify: callback_notify,
) -> *mut freesound_client {
    if params.token.is_null() {
        cb_err.exec("NULL API token");
        return null_mut();
    }
    let token = unsafe { CStr::from_ptr(params.token) }
        .to_str()
        .unwrap_or_default()
        .to_owned();

    if token.is_empty() {
        cb_err.exec("empty API token");
        return null_mut();
    }

    let rt = tokio::runtime::Runtime::new();

    match rt {
        Ok(rt) => {
            debug!("creating tokio runtime ...");

            let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<FreeSoundRequest>(32);
            let (rep_tx, rep_rx) =
                tokio::sync::mpsc::channel::<Result<FreeSoundReply, crate::service::Error>>(32);

            std::thread::spawn(move || {
                debug!("starting worker thread ...");
                rt.block_on(async move {
                    debug!("starting runloop ...");
                    loop {
                        tokio::select! {
                            // process requests
                            request = req_rx.recv() => {
                                debug!("new request: {request:?}");
                                match request {
                                    Some(request) => {
                                        if let Err(err) = process_request(request, &rep_tx, &cb_notify).await {
                                            FreeSoundService::write_error(&rep_tx, Error::Error(err), cb_notify).await;
                                        }
                                    }
                                    None => {
                                        debug!("break runloop");
                                        break;
                                    },
                                }
                            },
                        }
                    }
                });
                debug!("exit worker thread ...");
            });

            let cli = FreeSoundService::new(
                cb_err,
                cb_post,
                cb_debug,
                cb_log,
                cb_progress,
                Box::new(cb_reply),
                req_tx,
                rep_rx,
            );

            return Box::into_raw(Box::new(freesound_client { service: cli }));
        }
        Err(err) => {
            cb_err.exec(err.to_string().as_str());
            null_mut()
        }
    }
}

/// free freesound client
/// @param fs - pointer to freesound client (can be NULL)
#[no_mangle]
pub extern "C" fn ceammc_freesound_free(fs: *mut freesound_client) {
    if !fs.is_null() {
        drop(unsafe { Box::from_raw(fs) });
    }
}

/// process all results that are ready
/// @param cli - freesound client pointer
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_freesound_process(cli: Option<&mut freesound_client>) -> bool {
    if cli.is_none() {
        error!("NULL client");
        return false;
    }

    let cli = cli.unwrap();
    cli.service.process_results();
    true
}

#[no_mangle]
pub extern "C" fn ceammc_freesound_me(
    cli: Option<&freesound_client>,
    access: *const c_char,
) -> bool {
    if cli.is_none() {
        error!("NULL client");
        return false;
    }

    let cli = cli.unwrap();

    if access.is_null() {
        cli.service.on_error("NULL access token");
        return false;
    }
    let access = unsafe { CStr::from_ptr(access) }
        .to_str()
        .unwrap_or_default()
        .to_owned();
    if access.is_empty() {
        cli.service.on_error("empty access token");
        return false;
    }

    cli.service.send_request(FreeSoundRequest::Me(access))
}

#[no_mangle]
pub extern "C" fn ceammc_freesound_search(
    cli: Option<&freesound_client>,
    access: *const c_char,
    params: freesound_search_params,
) -> bool {
    if cli.is_none() {
        error!("NULL client");
        return false;
    }

    let cli = cli.unwrap();

    if access.is_null() {
        cli.service.on_error("NULL access token");
        return false;
    }
    let access = unsafe { CStr::from_ptr(access) }
        .to_str()
        .unwrap_or_default()
        .to_owned();
    if access.is_empty() {
        cli.service.on_error("empty access token");
        return false;
    }

    cli.service
        .send_request(FreeSoundRequest::Search(SearchParams::from(params), access))
}

#[no_mangle]
pub extern "C" fn ceammc_freesound_oauth_get_code(
    cli: Option<&freesound_client>,
    id: *const c_char,
    secret: *const c_char,
) -> bool {
    if cli.is_none() {
        error!("NULL client");
        return false;
    }

    let cli = cli.unwrap();

    if id.is_null() {
        cli.service.on_error("NULL id");
        return false;
    }
    let id = unsafe { CStr::from_ptr(id) }
        .to_str()
        .unwrap_or_default()
        .to_owned();

    if secret.is_null() {
        cli.service.on_error("NULL secret");
        return false;
    }
    let secret = unsafe { CStr::from_ptr(secret) }
        .to_string_lossy()
        .to_string();

    cli.service
        .send_request(FreeSoundRequest::OAuthGetCode(id, secret))
}

#[no_mangle]
pub extern "C" fn ceammc_freesound_oauth_get_access(
    cli: Option<&freesound_client>,
    id: *const c_char,
    secret: *const c_char,
    auth_code: *const c_char,
) -> bool {
    if cli.is_none() {
        error!("NULL client");
        return false;
    }

    let cli = cli.unwrap();

    if auth_code.is_null() {
        cli.service.on_error("NULL auth_code");
        return false;
    }
    let auth_code = unsafe { CStr::from_ptr(auth_code) }
        .to_string_lossy()
        .to_string();

    if id.is_null() {
        cli.service.on_error("NULL id");
        return false;
    }
    let id = unsafe { CStr::from_ptr(id) }.to_string_lossy().to_string();

    if secret.is_null() {
        cli.service.on_error("NULL secret");
        return false;
    }
    let secret = unsafe { CStr::from_ptr(secret) }
        .to_string_lossy()
        .to_string();

    cli.service
        .send_request(FreeSoundRequest::OAuthGetAccess(id, secret, auth_code))
}

#[no_mangle]
pub extern "C" fn ceammc_freesound_oauth_store_access_token(
    cli: Option<&freesound_client>,
    auth_token: *const c_char,
    base_dir: *const c_char,
    overwrite: bool,
) -> bool {
    if cli.is_none() {
        error!("NULL client");
        return false;
    }

    let cli = cli.unwrap();

    if auth_token.is_null() {
        cli.service.on_error("NULL auth_token");
        return false;
    }
    let auth_token = unsafe { CStr::from_ptr(auth_token) }
        .to_string_lossy()
        .to_string();

    let base_dir = if base_dir.is_null() {
        None
    } else {
        Some(
            unsafe { CStr::from_ptr(base_dir) }
                .to_string_lossy()
                .to_string(),
        )
    };

    cli.service.send_request(FreeSoundRequest::StoreAccessToken(
        auth_token, base_dir, overwrite,
    ))
}

#[no_mangle]
pub extern "C" fn ceammc_freesound_oauth_load_access_token(
    cli: Option<&freesound_client>,
    base_dir: *const c_char,
) -> bool {
    if cli.is_none() {
        error!("NULL client");
        return false;
    }

    let cli = cli.unwrap();

    let base_dir = if base_dir.is_null() {
        None
    } else {
        Some(
            unsafe { CStr::from_ptr(base_dir) }
                .to_string_lossy()
                .to_string(),
        )
    };

    cli.service
        .send_request(FreeSoundRequest::LoadAccessToken(base_dir))
}

#[no_mangle]
pub extern "C" fn ceammc_freesound_download_file(
    cli: Option<&freesound_client>,
    id: u64,
    auth_token: *const c_char,
    base_dir: *const c_char,
) -> bool {
    if cli.is_none() {
        error!("NULL client");
        return false;
    }

    let cli = cli.unwrap();

    if auth_token.is_null() {
        cli.service.on_error("NULL auth_token");
        return false;
    }
    let auth_token = unsafe { CStr::from_ptr(auth_token) }
        .to_string_lossy()
        .to_string();

    let base_dir = if base_dir.is_null() {
        None
    } else {
        Some(
            unsafe { CStr::from_ptr(base_dir) }
                .to_string_lossy()
                .to_string(),
        )
    };

    cli.service
        .send_request(FreeSoundRequest::Download(id, auth_token, base_dir))
}

#[no_mangle]
pub extern "C" fn ceammc_freesound_load_array(
    cli: Option<&freesound_client>,
    id: u64,
    channel: usize,
    normalize: bool,
    auth_token: *const c_char,
    array: *const c_char,
    alloc: Option<extern "C" fn(size: usize) -> *mut t_pd_rust_word>,
    free: Option<extern "C" fn(data: *mut t_pd_rust_word, size: usize)>,
) -> bool {
    if cli.is_none() {
        error!("NULL client");
        return false;
    }

    let cli = cli.unwrap();

    if auth_token.is_null() {
        cli.service.on_error("NULL auth_token");
        return false;
    }
    let auth_token = unsafe { CStr::from_ptr(auth_token) }
        .to_string_lossy()
        .to_string();

    if array.is_null() {
        cli.service.on_error("NULL array name");
        return false;
    }
    let array = unsafe { CStr::from_ptr(array) }
        .to_string_lossy()
        .to_string();

    cli.service
        .send_request(FreeSoundRequest::LoadToArray(LoadToArray {
            id,
            channel,
            normalize,
            access: auth_token,
            array,
            alloc,
            free,
        }))
}

#[cfg(test)]
mod tests {}
