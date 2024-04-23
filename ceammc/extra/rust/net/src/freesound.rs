use std::{
    collections::HashMap,
    ffi::{CStr, CString},
    fmt::Debug,
    os::raw::{c_char, c_void},
    path::{Path, PathBuf},
    ptr::{null_mut, slice_from_raw_parts, slice_from_raw_parts_mut},
    slice,
};

use crate::{
    service::{callback_msg, callback_notify, callback_progress, Error, Service, ServiceCallback},
    utils,
};
use derivative::Derivative;
use log::{debug, error, info};
use oauth2::TokenResponse;
use reqwest::Response;
use symphonia::core::{
    audio::{AudioBuffer, AudioBufferRef, Signal},
    codecs::{DecoderOptions, CODEC_TYPE_NULL},
    conv::{FromSample, IntoSample},
    formats::FormatOptions,
    io::MediaSourceStream,
    meta::MetadataOptions,
    probe::Hint,
    sample::Sample,
};
use tempfile::NamedTempFile;
use tokio::{fs::File, io::AsyncWriteExt};
use url::Url;

#[allow(non_camel_case_types)]
pub type freesound_alloc_fn = extern "C" fn(size: usize) -> *mut t_pd_rust_word;
#[allow(non_camel_case_types)]
pub type freesound_free_fn = extern "C" fn(data: *mut t_pd_rust_word, size: usize);

const ACCESS_TOKEN_FNAME: &str = ".freesound.key";

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
                .to_string_lossy()
                .to_string(),
            filter: String::new(),
            sort: unsafe { CStr::from_ptr(params.sort) }
                .to_string_lossy()
                .to_string(),
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

                if params.num_descriptors > 0 {
                    let k = "analysis".to_owned();
                    if !fields.contains(&k) {
                        fields.push(k);
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
struct ArraysParam {
    name: String,
    channel: usize,
}

#[derive(Debug)]
struct LoadToArray {
    file_id: u64,
    arrays: Vec<ArraysParam>,
    normalize: bool,
    access: String,
    alloc: freesound_alloc_fn,
    free: freesound_free_fn,
    float_type: FloatType,
}

#[derive(Debug)]
enum FreeSoundRequest {
    Search(SearchParams, String),
    OAuthGetCode(String, String),
    OAuthGetAccess(String, String, String),
    OAuthReadSecretFile(String),
    Me(String),
    StoreAccessToken(String, Option<String>, bool),
    LoadAccessToken(Option<String>),
    Download(u64, String, Option<String>),
    LoadToArray(LoadToArray),
    Quit,
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
pub struct freesound_prop_array_f64 {
    name: *const c_char,
    data: *const f64,
    size: usize,
}

impl Debug for freesound_prop_array_f64 {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("freesound_prop_array_f64")
            .field(
                "name",
                &unsafe { CStr::from_ptr(self.name) }.to_string_lossy(),
            )
            .field("data", &unsafe {
                &*slice_from_raw_parts(self.data, self.size)
            })
            .field("size", &self.size)
            .finish()
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_prop_str {
    name: *const c_char,
    value: *const c_char,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_prop_obj {
    name: *const c_char,
    data: *const freesound_prop_array_f64,
    len: usize,
}

impl Debug for freesound_prop_obj {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("freesound_prop_obj")
            .field(
                "name",
                &unsafe { CStr::from_ptr(self.name) }.to_string_lossy(),
            )
            .field("data", &unsafe {
                &*slice_from_raw_parts(self.data, self.len)
            })
            .field("len", &self.len)
            .finish()
    }
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
    /// obj props
    obj_props: *const freesound_prop_obj,
    /// number of obj props
    obj_props_len: usize,
}

#[derive(Debug)]
struct SearchResult {
    // The sound’s unique identifier.
    id: u64,
    str_map: HashMap<CString, CString>,
    num_map: HashMap<CString, f64>,
    obj_map: HashMap<CString, HashMap<CString, Vec<f64>>>,
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
                x.as_object().map(|level1| {
                    let mut data = HashMap::new();
                    for (k, v) in level1 {
                        v.as_object().map(|level2| {
                            for (kk, vv) in level2 {
                                let key = CString::new(format!("{k}.{kk}")).unwrap_or_default();

                                match vv {
                                    serde_json::Value::Number(x) => {
                                        x.as_f64().map(|f| {
                                            data.insert(key, vec![f]);
                                        });
                                    }
                                    serde_json::Value::Array(arr) => {
                                        let floats = arr
                                            .iter()
                                            .filter(|x| x.is_f64())
                                            .map(|x| x.as_f64().unwrap_or_default())
                                            .collect::<Vec<_>>();

                                        data.insert(key, floats);
                                    }
                                    _ => {}
                                }
                            }
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

#[allow(non_camel_case_types)]
pub struct freesound_array_data {
    array: freesound_array,
    data: *mut t_pd_rust_word,
    size: usize,
    free: freesound_free_fn,
    owner: bool,
}

impl freesound_array_data {
    fn from(arr: &mut ArrayData) -> Self {
        freesound_array_data {
            array: freesound_array {
                name: arr.name.as_ptr(),
                channel: arr.channel,
            },
            data: arr.data.as_mut_ptr(),
            size: arr.size,
            owner: arr.retain_owner(),
            free: arr.free,
        }
    }
}

impl Drop for freesound_array_data {
    fn drop(&mut self) {
        if self.owner {
            info!(
                "free freesound_array_data: '{}' [{}]",
                unsafe { CStr::from_ptr(self.array.name) }.to_string_lossy(),
                self.array.channel
            );
            (self.free)(self.data, self.size);
        }
    }
}

#[derive(Debug, Clone, Copy)]
enum FloatType {
    Double,
    Float,
}

#[derive(Derivative)]
#[derivative(Debug)]
struct ArrayData {
    name: CString,
    channel: usize,
    #[derivative(Debug = "ignore")]
    data: &'static mut [t_pd_rust_word],
    size: usize,
    alloc: freesound_alloc_fn,
    free: freesound_free_fn,
    owner: bool,
    float_type: FloatType,
}

impl ArrayData {
    fn alloc(
        name: CString,
        channel: usize,
        capacity: usize,
        alloc: freesound_alloc_fn,
        free: freesound_free_fn,
        float_type: FloatType,
    ) -> Option<Self> {
        let data = alloc(capacity);
        if data.is_null() {
            None
        } else {
            let data = unsafe { &mut *slice_from_raw_parts_mut(data, capacity) };

            Some(ArrayData {
                data,
                size: 0,
                alloc,
                free,
                name,
                channel,
                owner: true,
                float_type,
            })
        }
    }

    fn is_empty(&self) -> bool {
        return self.size == 0;
    }

    fn push(&mut self, x: f32) {
        if self.size < self.data.len() {
            match self.float_type {
                FloatType::Double => self.data[self.size].w_double = x as f64,
                FloatType::Float => self.data[self.size].w_float = x,
            }
            self.size += 1;
        }
    }

    fn abs_max(&self) -> f32 {
        match self.float_type {
            FloatType::Double => unsafe {
                (*self
                    .data
                    .iter()
                    .max_by(|a, b| a.w_double.abs().total_cmp(&b.w_double.abs()))
                    .unwrap())
                .w_double
            }
            .abs() as f32,
            FloatType::Float => unsafe {
                (*self
                    .data
                    .iter()
                    .max_by(|a, b| a.w_float.abs().total_cmp(&b.w_float.abs()))
                    .unwrap())
                .w_float
            }
            .abs(),
        }
    }

    fn normalize(&mut self, max: f32) {
        match self.float_type {
            FloatType::Double => {
                let max = max as f64;
                if max > 0.0 {
                    for x in self.data.iter_mut() {
                        unsafe { x.w_double /= max };
                    }
                }
            }
            FloatType::Float => {
                if max > 0.0 {
                    for x in self.data.iter_mut() {
                        unsafe { x.w_float /= max };
                    }
                }
            }
        }
    }

    #[must_use]
    fn retain_owner(&mut self) -> bool {
        if self.owner {
            self.owner = false;
            true
        } else {
            false
        }
    }
}

impl Drop for ArrayData {
    fn drop(&mut self) {
        if self.owner {
            info!("free ArrayData");
            (self.free)(self.data.as_mut_ptr(), self.size);
        }
    }
}

#[derive(Debug)]
enum FreeSoundReply {
    OAuth2Url(CString),
    OAuthAccess(Access),
    OAuthSecrets(CString, CString),
    Info(InfoMe),
    SearchResults(SearchResults),
    Downloaded(CString),
    LoadToArray(Vec<ArrayData>),
}

type FreeSoundTx = tokio::sync::mpsc::Sender<Result<FreeSoundReply, Error>>;
type FreeSoundService = Service<FreeSoundRequest, FreeSoundReply>;

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_init {
    /// can be NULL
    secret_file: *const c_char,
    /// non NULL
    alloc: freesound_alloc_fn,
    /// non NULL
    free: freesound_free_fn,
}

impl freesound_init {
    fn secret_file(&self) -> Option<String> {
        if self.secret_file.is_null() {
            None
        } else {
            let path = unsafe { CStr::from_ptr(self.secret_file) }.to_string_lossy();

            if path.is_empty() {
                None
            } else {
                Some(path.to_string())
            }
        }
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_array {
    name: *const c_char,
    channel: usize,
}

#[allow(non_camel_case_types)]
pub struct freesound_client {
    service: FreeSoundService,
    alloc: freesound_alloc_fn,
    free: freesound_free_fn,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_result_cb {
    user: *mut c_void,
    cb_oauth_url: Option<extern "C" fn(user: *mut c_void, url: *const c_char)>,
    cb_oauth_access: Option<extern "C" fn(user: *mut c_void, token: *const c_char, expires: u64)>,
    cb_oauth_file:
        Option<extern "C" fn(user: *mut c_void, id: *const c_char, secret: *const c_char)>,
    cb_info_me: Option<extern "C" fn(user: *mut c_void, data: &freesound_info_me)>,
    cb_search_info: Option<extern "C" fn(user: *mut c_void, count: u64, prev: u32, next: u32)>,
    cb_search_result:
        Option<extern "C" fn(user: *mut c_void, i: usize, res: &freesound_search_result)>,
    cb_download: Option<extern "C" fn(user: *mut c_void, filename: *const c_char)>,
    cb_load: Option<extern "C" fn(user: *mut c_void, data: *mut freesound_array_data, size: usize)>,
}

impl ServiceCallback<FreeSoundReply> for freesound_result_cb {
    fn exec(&self, data: &mut FreeSoundReply) {
        match data {
            FreeSoundReply::OAuth2Url(url) => {
                self.cb_oauth_url.map(|f| {
                    f(self.user, url.as_ptr());
                });
            }
            FreeSoundReply::OAuthAccess(acc) => {
                self.cb_oauth_access.map(|f| {
                    f(self.user, acc.token.as_ptr(), acc.expires);
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

                            let ctags = res.tags.iter().map(|x| x.as_ptr()).collect::<Vec<_>>();

                            let num_props = res
                                .num_map
                                .iter()
                                .map(|(k, v)| freesound_prop_f64 {
                                    name: k.as_ptr(),
                                    value: *v,
                                })
                                .collect::<Vec<_>>();

                            let str_props = res
                                .str_map
                                .iter()
                                .map(|(k, v)| freesound_prop_str {
                                    name: k.as_ptr(),
                                    value: v.as_ptr(),
                                })
                                .collect::<Vec<_>>();

                            let obj_props = res
                                .obj_map
                                .iter()
                                .map(|(_, v)| {
                                    v.iter()
                                        .map(|(p, data)| freesound_prop_array_f64 {
                                            name: p.as_ptr(),
                                            data: data.as_ptr(),
                                            size: data.len(),
                                        })
                                        .collect::<Vec<_>>()
                                })
                                .collect::<Vec<_>>();

                            let obj_props2 = res
                                .obj_map
                                .iter()
                                .zip(&obj_props)
                                .map(|((k, _), data)| freesound_prop_obj {
                                    name: k.as_ptr(),
                                    data: data.as_ptr(),
                                    len: data.len(),
                                })
                                .collect::<Vec<_>>();

                            let res = freesound_search_result {
                                id: res.id,
                                tags: ctags.as_ptr(),
                                tags_len: ctags.len(),
                                num_props: num_props.as_ptr(),
                                num_props_len: num_props.len(),
                                str_props: str_props.as_ptr(),
                                str_props_len: str_props.len(),
                                obj_props: obj_props2.as_ptr(),
                                obj_props_len: obj_props2.len(),
                            };

                            f(self.user, i, &res);
                        }
                    });
                });
            }
            FreeSoundReply::Downloaded(path) => {
                self.cb_download.map(|f| {
                    f(self.user, path.as_ptr());
                });
            }
            FreeSoundReply::LoadToArray(data) => {
                self.cb_load.map(|f| {
                    let mut data = data
                        .iter_mut()
                        .map(|x| freesound_array_data::from(x))
                        .collect::<Vec<_>>();
                    f(self.user, data.as_mut_ptr(), data.len());
                });
            }
            FreeSoundReply::OAuthSecrets(id, secret) => {
                self.cb_oauth_file.map(|f| {
                    f(self.user, id.as_ptr(), secret.as_ptr());
                });
            }
        }
    }
}

const SORT_VALUES: [&str; 9] = [
    "score",
    "duration_desc",
    "duration_asc",
    "created_desc",
    "created_asc",
    "downloads_desc",
    "downloads_asc",
    "rating_desc",
    "rating_asc",
];

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

        if !params.sort.is_empty() {
            if !SORT_VALUES.contains(&params.sort.as_str()) {
                return Err(format!(
                    "invalid sort value: '{}', expected values are: {}.",
                    params.sort,
                    SORT_VALUES
                        .iter()
                        .map(|x| format!("'{}'", x.to_owned()))
                        .collect::<Vec<_>>()
                        .join(", ")
                ));
            } else {
                url.query_pairs_mut()
                .append_pair("sort", params.sort.as_str())
                .finish();
            }
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

fn read_buffer<T: Sample>(buf: &AudioBuffer<T>, channels: &Vec<usize>, data: &mut Vec<ArrayData>)
where
    f32: FromSample<T>,
{
    assert!(channels.len() == data.len());

    for (i, ch) in channels.iter().enumerate() {
        buf.chan(*ch).iter().for_each(|s| {
            if i < data.len() {
                data[i].push(IntoSample::<f32>::into_sample(*s))
            }
        });
    }
}

fn decode_file(params: LoadToArray, path: &Path) -> Result<Vec<ArrayData>, String> {
    let channels = params.arrays.iter().map(|x| x.channel).collect::<Vec<_>>();
    if channels.is_empty() {
        return Err(format!("no channels requested"));
    }

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

    let num_chan = track
        .codec_params
        .channels
        .ok_or_else(|| "can't get number of channels".to_owned())?
        .count();

    let channels: Vec<usize> = channels
        .iter()
        .filter(|ch| {
            if **ch >= num_chan {
                error!("invalid requested channel: {}", ch);
                return false;
            } else {
                return true;
            }
        })
        .cloned()
        .collect();

    let num_frames = track
        .codec_params
        .n_frames
        .ok_or_else(|| "can't get number of frames".to_owned())? as usize;

    // Use the default options for the decoder.
    let dec_opts: DecoderOptions = Default::default();

    // Create a decoder for the track.
    let mut decoder = symphonia::default::get_codecs()
        .make(&track.codec_params, &dec_opts)
        .map_err(|e| e.to_string())?;

    // Store the track identifier, it will be used to filter packets.
    let track_id = track.id;

    assert!(channels.len() <= params.arrays.len());

    let mut data = vec![];
    for i in 0..channels.len() {
        data.push(
            ArrayData::alloc(
                CString::new(params.arrays[i].name.clone()).unwrap_or_default(),
                channels[i],
                num_frames,
                params.alloc,
                params.free,
                params.float_type,
            )
            .ok_or_else(|| format!("can't allocate {num_frames} bytes"))?,
        );
    }

    assert!(channels.len() == data.len());

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
                    // An unrecoverable error occurred, halt decoding.
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
            Ok(decoded) => match decoded {
                AudioBufferRef::F32(buf) => {
                    read_buffer::<f32>(buf.as_ref(), &channels, &mut data);
                }
                AudioBufferRef::F64(buf) => {
                    read_buffer::<f64>(buf.as_ref(), &channels, &mut data);
                }
                AudioBufferRef::U8(buf) => {
                    read_buffer::<u8>(buf.as_ref(), &channels, &mut data);
                }
                AudioBufferRef::U16(buf) => {
                    read_buffer::<u16>(buf.as_ref(), &channels, &mut data);
                }
                AudioBufferRef::U24(buf) => {
                    read_buffer::<symphonia::core::sample::u24>(buf.as_ref(), &channels, &mut data);
                }
                AudioBufferRef::U32(buf) => {
                    read_buffer::<u32>(buf.as_ref(), &channels, &mut data);
                }
                AudioBufferRef::S8(buf) => {
                    read_buffer::<i8>(buf.as_ref(), &channels, &mut data);
                }
                AudioBufferRef::S16(buf) => {
                    read_buffer::<i16>(buf.as_ref(), &channels, &mut data);
                }
                AudioBufferRef::S24(buf) => {
                    read_buffer::<symphonia::core::sample::i24>(buf.as_ref(), &channels, &mut data);
                }
                AudioBufferRef::S32(buf) => {
                    read_buffer::<i32>(buf.as_ref(), &channels, &mut data);
                }
            },
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

    if data.is_empty() || data[0].is_empty() {
        return Err(format!("empty result"));
    }

    if params.normalize {
        let max = data
            .iter()
            .map(|x| x.abs_max())
            .reduce(f32::max)
            .ok_or_else(|| format!("normalization error"))?;

        data.iter_mut().for_each(|x| x.normalize(max));
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

#[derive(PartialEq)]
enum ProcessAction {
    Continue,
    Break,
}

async fn process_request(
    req: FreeSoundRequest,
    tx: &FreeSoundTx,
    cb_notify: &callback_notify,
) -> Result<ProcessAction, String> {
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
            Ok(ProcessAction::Continue)
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
                            debug!("value: {value}");
                            if let Ok(x) = SearchResult::from(value) {
                                res.results.push(x);
                            }
                        }
                    });

                    Ok(res)
                })?;

            FreeSoundService::write_ok(&tx, FreeSoundReply::SearchResults(info), *cb_notify).await;
            Ok(ProcessAction::Continue)
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

            let url = CString::new(url).unwrap_or_default();
            FreeSoundService::write_ok(&tx, FreeSoundReply::OAuth2Url(url), *cb_notify).await;
            Ok(ProcessAction::Continue)
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
                    Ok(ProcessAction::Continue)
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
            FreeSoundService::write_debug(
                &tx,
                format!("access token saved to: {}", path.to_string_lossy()),
                *cb_notify,
            )
            .await;
            Ok(ProcessAction::Continue)
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

            FreeSoundService::write_debug(
                &tx,
                format!("access token loaded from: {}", path.to_string_lossy()),
                *cb_notify,
            )
            .await;
            Ok(ProcessAction::Continue)
        }
        FreeSoundRequest::Download(id, access, base_dir) => {
            let mut response = download_response(id, &access).await?;
            let path = make_download_path(&response, base_dir)?;
            download_file(&mut response, &path, tx, cb_notify).await?;

            FreeSoundService::write_ok(
                &tx,
                FreeSoundReply::Downloaded(
                    CString::new(path.to_string_lossy().to_string()).unwrap_or_default(),
                ),
                *cb_notify,
            )
            .await;
            Ok(ProcessAction::Continue)
        }
        FreeSoundRequest::LoadToArray(params) => {
            let mut response = download_response(params.file_id, &params.access).await?;

            let path = NamedTempFile::new()
                .map_err(|e| e.to_string())?
                .path()
                .to_path_buf();

            download_file(&mut response, &path, tx, cb_notify).await?;

            let data = decode_file(params, &path)?;
            debug!("loaded {data:?} channels");

            FreeSoundService::write_ok(&tx, FreeSoundReply::LoadToArray(data), *cb_notify).await;

            Ok(ProcessAction::Continue)
        }
        FreeSoundRequest::OAuthReadSecretFile(secret_file) => {
            debug!("OAuthReadSecretFile: {secret_file}");

            let data = tokio::fs::read_to_string(secret_file)
                .await
                .map_err(|e| e.to_string())?;
            let lines: Vec<String> = data
                .split('\n')
                .filter(|ln| {
                    let ln = ln.trim();
                    !(ln.is_empty() || ln.starts_with('#'))
                })
                .map(|ln| ln.to_owned())
                .collect();

            // debug!("{lines:?}");

            if lines.len() < 2 {
                return Err(format!("OAuth ID and OAuth secret expected"));
            } else {
                let id = CString::new(lines[0].trim()).unwrap_or_default();
                let secret = CString::new(lines[1].trim()).unwrap_or_default();

                if id.is_empty() || secret.is_empty() {
                    return Err(format!("empty OAuth ID or secret"));
                }

                FreeSoundService::write_ok(
                    &tx,
                    FreeSoundReply::OAuthSecrets(id, secret),
                    *cb_notify,
                )
                .await;

                return Ok(ProcessAction::Continue);
            }
        }
        FreeSoundRequest::Quit => Ok(ProcessAction::Break),
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
                                        match process_request(request, &rep_tx, &cb_notify).await {
                                            Ok(action) => if action == ProcessAction::Break { break; },
                                            Err(err) => FreeSoundService::write_error(&rep_tx, Error::Error(err), cb_notify).await
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

            if let Some(file) = params.secret_file() {
                cli.send_request(FreeSoundRequest::OAuthReadSecretFile(file));
            }

            return Box::into_raw(Box::new(freesound_client {
                service: cli,
                alloc: params.alloc,
                free: params.free,
            }));
        }
        Err(err) => {
            cb_err.exec(err.to_string().as_str());
            null_mut()
        }
    }
}

/// free freesound client and stop worker thread
/// @param fs - pointer to freesound client (non NULL)
#[no_mangle]
pub extern "C" fn ceammc_freesound_free(fs: *mut freesound_client) {
    if !fs.is_null() {
        let cli = unsafe { Box::from_raw(fs) };
        cli.service.send_request(FreeSoundRequest::Quit);
    }
}

/// process all results that are ready
/// @param cli - freesound client pointer (non NULL)
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

/// get freesound user information
/// @param cli - freesound client pointer (non NULL)
/// @param access - tmp access token (non NULL)
/// @return true on success, false on error
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

/// freesound search
/// @param cli - freesound client pointer (non NULL)
/// @param access - tmp access token (non NULL)
/// @param params - search params
/// @return true on success, false on error
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

/// request for freesound URL to get access code
/// @param cli - freesound client pointer (non NULL)
/// @param id - OAuth2 id (non NULL)
/// @param secret - OAuth2 secret (non NULL)
/// @return true on success, false on error
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

/// request for OAuth temp access token (valid for 24 hours)
/// @param cli - freesound client pointer (non NULL)
/// @param id - OAuth2 id (non NULL)
/// @param secret - OAuth2 secret (non NULL)
/// @param auth_code - auth code from freesound http app page (non NULL)
/// @return true on success, false on error
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

/// async store temp access token to the file
/// @param cli - freesound client (non NULL)
/// @param access - temp access token (non NULL)
/// @param base_dir - base directory for saving token (nullable)
/// @param overwrite - should overwrite existing token
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_freesound_oauth_store_access_token(
    cli: Option<&freesound_client>,
    access: *const c_char,
    base_dir: *const c_char,
    overwrite: bool,
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
        access, base_dir, overwrite,
    ))
}

/// async load temp access token from file
/// @param cli - freesound client pointer (non NULL)
/// @param base_dir - base directory (nullable)
/// @return true on success, false on error
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

/// download freesound file to local directory
/// @param cli - freesound client pointer (non NULL)
/// @param file_id - freesound file id
/// @param access - access token
/// @param base_dir - base directory for saving
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_freesound_download_file(
    cli: Option<&freesound_client>,
    file_id: u64,
    access: *const c_char,
    base_dir: *const c_char,
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
        .send_request(FreeSoundRequest::Download(file_id, access, base_dir))
}

/// load freesound file to specified arrays
/// @param cli - freesound client pointer (non NULL)
/// @param file_id - sound file id
/// @param arrays - pointer to array load params (non NULL)
/// @param num_arrays - number or array params
/// @param normalize - if perform array normalization
/// @param access - temp access token (non NULL)
/// @param double - double precision used
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_freesound_load_to_arrays(
    cli: Option<&freesound_client>,
    file_id: u64,
    arrays: *const freesound_array,
    num_arrays: usize,
    normalize: bool,
    access: *const c_char,
    double: bool,
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
        .to_string_lossy()
        .to_string();

    if arrays.is_null() {
        cli.service.on_error("NULL arrays");
        return false;
    }
    let arrays = unsafe { slice::from_raw_parts(arrays, num_arrays) };
    if arrays.is_empty() {
        cli.service.on_error("empty arrays");
        return false;
    }
    let mut vec_arrays = Vec::with_capacity(arrays.len());
    for arr in arrays {
        vec_arrays.push(ArraysParam {
            name: unsafe { CStr::from_ptr(arr.name) }
                .to_string_lossy()
                .to_string(),
            channel: arr.channel,
        });
    }

    cli.service
        .send_request(FreeSoundRequest::LoadToArray(LoadToArray {
            file_id,
            arrays: vec_arrays,
            normalize,
            access,
            alloc: cli.alloc,
            free: cli.free,
            float_type: if double {
                FloatType::Double
            } else {
                FloatType::Float
            },
        }))
}

/// retain loaded data (caller should free itself)
/// @param data - pointer to array data (non NULL!)
#[no_mangle]
pub extern "C" fn ceammc_freesound_array_data_retain(data: &mut freesound_array_data) {
    data.owner = false;
}

/// get data size
/// @param data - pointer to array data (non NULL!)
#[no_mangle]
pub extern "C" fn ceammc_freesound_array_data_size(data: &mut freesound_array_data) -> usize {
    data.size
}

/// get data size
/// @param data - pointer to array data (non NULL!)
#[no_mangle]
pub extern "C" fn ceammc_freesound_array_data_ptr(
    data: &mut freesound_array_data,
) -> *mut t_pd_rust_word {
    data.data
}

/// get data array name
/// @param array - pointer to array data (non NULL!)
#[no_mangle]
pub extern "C" fn ceammc_freesound_array_data_name(
    data: &mut freesound_array_data,
) -> *const c_char {
    data.array.name
}

/// get data array channel
/// @param array - pointer to array data (non NULL!)
#[no_mangle]
pub extern "C" fn ceammc_freesound_array_data_channel(data: &mut freesound_array_data) -> usize {
    data.array.channel
}

/// get array data
/// @param array - pointer to array data (non NULL!)
#[no_mangle]
pub extern "C" fn ceammc_freesound_array_data_at(
    idx: usize,
    data: *mut freesound_array_data,
    len: usize,
) -> *mut freesound_array_data {
    if idx < len {
        unsafe { data.offset(idx as isize) }
    } else {
        null_mut()
    }
}

#[cfg(test)]
mod tests {}
