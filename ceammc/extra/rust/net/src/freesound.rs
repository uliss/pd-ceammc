use std::{
    ffi::{CStr, CString},
    os::raw::{c_char, c_void},
    ptr::null_mut,
    slice,
};

use log::{debug, error};
use oauth2::TokenResponse;
use url::Url;

use crate::service::{
    callback_msg, callback_notify, callback_progress, Error, Service, ServiceCallback,
};

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
            descriptors: vec![],
            normalized: params.normalized,
        }
    }
}

#[derive(Debug)]
pub enum FreeSoundRequest {
    Search(SearchParams, String),
    OAuthGetCode(String, String),
    OAuthGetAccess(String, String, String),
    Me(String),
}

#[derive(Debug)]
struct Access {
    token: CString,
    _refresh: CString,
    expires: u64,
}

#[derive(Debug)]
struct InfoMe {
    id: u64,
    username: CString,
    email: CString,
    home_page: CString,
    url: CString,
    sounds: CString,
    packs: CString,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct freesound_search_result {
    /// The sound’s unique identifier.
    id: u64,
    /// The URI for this sound on the Freesound website.
    url: *const c_char,
    /// The name user gave to the sound.
    name: *const c_char,
    /// The license under which the sound is available to you.
    license: *const c_char,
    /// The type of sound (wav, aif, aiff, mp3, m4a or flac).
    file_type: *const c_char,
    /// The number of channels.
    channels: u8,
    /// The size of the file in bytes.
    file_size: u64,
    /// The bit depth of the sound.
    bit_depth: u8,
    ///  The duration of the sound in seconds.
    duration: f32,
    /// The sample_rate of the sound.
    sample_rate: f32,
}

#[derive(Debug)]
struct SearchResult {
    // The sound’s unique identifier.
    id: u64,
    // The URI for this sound on the Freesound website.
    url: CString,
    // The name user gave to the sound.
    name: CString,
    // The license under which the sound is available to you.
    license: CString,
    // The type of sound (wav, aif, aiff, mp3, m4a or flac).
    file_type: CString,
    // The number of channels.
    channels: u8,
    // The size of the file in bytes.
    file_size: u64,
    // The bit rate of the sound in kbps.
    // bitrate: f32,
    // The bit depth of the sound.
    bit_depth: u8,
    //  The duration of the sound in seconds.
    duration: f32,
    // The sample_rate of the sound.
    sample_rate: f32,
    // The URI for retrieving the original sound.
    // download: CString,
    // description: String,
    // tags 	array[strings] 	An array of tags the user gave to the sound.
    // description 	string 	The description the user gave to the sound.
    // geotag 	string 	Latitude and longitude of the geotag separated by spaces (e.g. “41.0082325664 28.9731252193”, only for sounds that have been geotagged).
    // created 	string 	The date when the sound was uploaded (e.g. “2014-04-16T20:07:11.145”).
    // username 	string 	The username of the uploader of the sound.
    // pack 	URI 	If the sound is part of a pack, this URI points to that pack’s API resource.
    // bookmark 	URI 	The URI for bookmarking the sound.
    // previews 	object 	Dictionary containing the URIs for mp3 and ogg versions of the sound. The dictionary includes the fields preview-hq-mp3 and preview-lq-mp3 (for ~128kbps quality and ~64kbps quality mp3 respectively), and preview-hq-ogg and preview-lq-ogg (for ~192kbps quality and ~80kbps quality ogg respectively).
    // images 	object 	Dictionary including the URIs for spectrogram and waveform visualizations of the sound. The dictionary includes the fields waveform_l and waveform_m (for large and medium waveform images respectively), and spectral_l and spectral_m (for large and medium spectrogram images respectively).
    // num_downloads 	number 	The number of times the sound was downloaded.
    // avg_rating 	number 	The average rating of the sound.
    // num_ratings 	number 	The number of times the sound was rated.
    // rate 	URI 	The URI for rating the sound.
    // comments 	URI 	The URI of a paginated list of the comments of the sound.
    // num_comments 	number 	The number of comments.
    // comment 	URI 	The URI to comment the sound.
    // similar_sounds 	URI 	URI pointing to the similarity resource (to get a list of similar sounds).
    // analysis 	object 	Dictionary containing requested descriptors information according to the descriptors request parameter (see below). This field will be null if no descriptors were specified (or invalid descriptor names specified) or if the analysis data for the sound is not available.
    // analysis_stats 	URI 	URI pointing to the complete analysis results of the sound (see Analysis Descriptor Documentation).
    // analysis_frames 	URI 	The URI for retrieving a JSON file with analysis information for each frame of the sound (see Analysis Descriptor Documentation).
    // ac_analysis 	object 	Dictionary containing the results of the AudioCommons analysis for the given sound.
}

impl SearchResult {
    fn from(value: &serde_json::Value) -> Result<Self, String> {
        Ok(SearchResult {
            id: json_get_u64(value, "id")?,
            url: json_get_cstring(value, "url").unwrap_or_default(),
            name: json_get_cstring(value, "name").unwrap_or_default(),
            license: json_get_cstring(value, "license").unwrap_or_default(),
            file_type: json_get_cstring(value, "type").unwrap_or_default(),
            channels: value
                .get("channels")
                .and_then(|x| x.as_u64())
                .unwrap_or_default() as u8,
            file_size: value
                .get("file_size")
                .and_then(|x| x.as_u64())
                .unwrap_or_default(),
            bit_depth: value
                .get("bit_depth")
                .and_then(|x| x.as_u64())
                .unwrap_or_default() as u8,
            duration: value
                .get("duration")
                .and_then(|x| x.as_f64())
                .unwrap_or_default() as f32,
            sample_rate: value
                .get("sample_rate")
                .and_then(|x| x.as_f64())
                .unwrap_or_default() as f32,
        })
    }

    fn into(&self) -> freesound_search_result {
        freesound_search_result {
            id: self.id,
            url: self.url.as_ptr(),
            name: self.name.as_ptr(),
            license: self.license.as_ptr(),
            file_type: self.file_type.as_ptr(),
            channels: self.channels,
            file_size: self.file_size,
            bit_depth: self.bit_depth,
            duration: self.duration,
            sample_rate: self.sample_rate,
        }
    }
}

#[derive(Debug)]
struct SearchResults {
    count: u64,
    next: Option<String>,
    prev: Option<String>,
    results: Vec<SearchResult>,
}

#[derive(Debug)]
enum FreeSoundReply {
    OAuth2Url(String),
    OAuthAccess(Access),
    Info(InfoMe),
    SearchResults(SearchResults),
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
    cb_info_me: Option<
        extern "C" fn(
            user: *mut c_void,
            id: u64,
            username: *const c_char,
            email: *const c_char,
            homepage: *const c_char,
            url: *const c_char,
            sounds: *const c_char,
            packs: *const c_char,
        ),
    >,
    cb_search_info: Option<
        extern "C" fn(user: *mut c_void, count: u64, prev: *const c_char, next: *const c_char),
    >,
    cb_search_result:
        Option<extern "C" fn(user: *mut c_void, i: usize, res: &freesound_search_result)>,
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
                    f(
                        self.user,
                        info.id,
                        info.username.as_ptr(),
                        info.email.as_ptr(),
                        info.home_page.as_ptr(),
                        info.url.as_ptr(),
                        info.sounds.as_ptr(),
                        info.packs.as_ptr(),
                    );
                });
            }
            FreeSoundReply::SearchResults(res) => {
                self.cb_search_info.map(|f| {
                    let prev =
                        CString::new(res.prev.clone().unwrap_or_default()).unwrap_or_default();
                    let next =
                        CString::new(res.next.clone().unwrap_or_default()).unwrap_or_default();

                    f(self.user, res.count, prev.as_ptr(), next.as_ptr());

                    self.cb_search_result.map(|f| {
                        for (i, res) in res.results.iter().enumerate() {
                            debug!("result: {res:?}");
                            f(self.user, i, &res.into());
                        }
                    });
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

        if let Some(page_size) = params.page_size {
            url.query_pairs_mut()
                .append_pair("page_size", format!("{page_size}").as_str())
                .finish();
        }

        if params.fields.len() > 0 {
            url.query_pairs_mut()
                .append_pair(
                    "fields",
                    params
                        .fields
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

fn json_get_cstring(value: &serde_json::Value, k: &str) -> Result<CString, String> {
    Ok(CString::new(
        value
            .get(k)
            .ok_or(format!("{k} not found"))?
            .as_str()
            .ok_or(format!("{k}: not a string"))?
            .to_owned(),
    )
    .unwrap_or_default())
}

fn json_get_string(value: &serde_json::Value, k: &str) -> Result<String, String> {
    Ok(value
        .get(k)
        .ok_or(format!("{k} not found"))?
        .as_str()
        .ok_or(format!("{k}: not a string"))?
        .to_owned())
}

fn json_get_u64(value: &serde_json::Value, k: &str) -> Result<u64, String> {
    Ok(value
        .get(k)
        .ok_or(format!("{k} not found"))?
        .as_u64()
        .ok_or(format!("{k}: not a string"))?
        .to_owned())
}

async fn process_request(req: FreeSoundRequest, tx: &FreeSoundTx, cb_notify: &callback_notify) {
    match req {
        FreeSoundRequest::Me(access_token) => {
            match freesound_get("/apiv2/me/", access_token.as_str(), None).await {
                Ok(res) => match serde_json::from_str::<serde_json::Value>(res.as_str()) {
                    Ok(json) => {
                        debug!("{json}");
                        let get_info = || {
                            Ok(InfoMe {
                                id: json_get_u64(&json, "unique_id")?,
                                username: json_get_cstring(&json, "username")?,
                                email: json_get_cstring(&json, "email")?,
                                home_page: json_get_cstring(&json, "home_page")?,
                                url: json_get_cstring(&json, "url")?,
                                sounds: json_get_cstring(&json, "sounds")?,
                                packs: json_get_cstring(&json, "packs")?,
                            })
                        };

                        match get_info() {
                            Ok(info) => {
                                debug!("{info:?}");
                                FreeSoundService::write_ok(
                                    &tx,
                                    FreeSoundReply::Info(info),
                                    *cb_notify,
                                )
                                .await;
                            }
                            Err(err) => {
                                FreeSoundService::write_error(&tx, Error::Error(err), *cb_notify)
                                    .await;
                            }
                        }
                    }
                    Err(err) => {
                        FreeSoundService::write_error(
                            &tx,
                            Error::Error(err.to_string()),
                            *cb_notify,
                        )
                        .await;
                    }
                },
                Err(err) => {
                    FreeSoundService::write_error(&tx, Error::Error(err), *cb_notify).await;
                }
            }
        }
        FreeSoundRequest::Search(params, access_token) => {
            match freesound_get("/apiv2/search/text/", access_token.as_str(), Some(params)).await {
                Ok(res) => match serde_json::from_str::<serde_json::Value>(res.as_str()) {
                    Ok(json) => {
                        let count = json.get("count").and_then(|x| x.as_u64()).unwrap_or(0);
                        let prev = json
                            .get("previous")
                            .and_then(|x| x.as_str())
                            .map(|x| x.to_owned());
                        let next = json
                            .get("next")
                            .and_then(|x| x.as_str())
                            .map(|x| x.to_owned());

                        let mut res = SearchResults {
                            count,
                            next,
                            prev,
                            results: vec![],
                        };

                        if let Some(results) = json.get("results").and_then(|x| x.as_array()) {
                            for value in results.iter().filter(|x| x.is_object()) {
                                debug!("{value}");
                                if let Ok(x) = SearchResult::from(value) {
                                    res.results.push(x);
                                }
                            }
                        }

                        FreeSoundService::write_ok(
                            &tx,
                            FreeSoundReply::SearchResults(res),
                            *cb_notify,
                        )
                        .await;
                    }
                    Err(err) => {
                        FreeSoundService::write_error(
                            &tx,
                            Error::Error(err.to_string()),
                            *cb_notify,
                        )
                        .await;
                    }
                },
                Err(err) => {
                    FreeSoundService::write_error(&tx, Error::Error(err), *cb_notify).await;
                }
            }
        }
        FreeSoundRequest::OAuthGetCode(id, secret) => {
            use oauth2::{CsrfToken, Scope};

            let url = make_oauth_client(&id, &secret).and_then(|cli| {
                debug!("{cli:?}");

                Ok(cli
                    .authorize_url(CsrfToken::new_random)
                    .add_scope(Scope::new("read".to_string()))
                    .add_scope(Scope::new("write".to_string()))
                    .url()
                    .0)
            });

            match url {
                Ok(url) => {
                    debug!("{}", url.to_string());

                    FreeSoundService::write_ok(
                        &tx,
                        FreeSoundReply::OAuth2Url(url.to_string()),
                        *cb_notify,
                    )
                    .await;
                }
                Err(err) => {
                    FreeSoundService::write_error(&tx, Error::Error(err), *cb_notify).await;
                }
            }
        }
        FreeSoundRequest::OAuthGetAccess(id, secret, code) => {
            use oauth2::AuthorizationCode;

            match make_oauth_client(&id, &secret) {
                Ok(cli) => {
                    match cli
                        .exchange_code(AuthorizationCode::new(code))
                        .request_async(oauth2::reqwest::async_http_client)
                        .await
                        .map(|res| Access {
                            token: CString::new(res.access_token().secret().clone())
                                .unwrap_or_default(),
                            _refresh: res
                                .refresh_token()
                                .clone()
                                .map(|t| CString::new(t.secret().clone()).unwrap_or_default())
                                .unwrap_or_default(),
                            expires: res.expires_in().unwrap_or_default().as_secs(),
                        }) {
                        Ok(acc) => {
                            FreeSoundService::write_ok(
                                &tx,
                                FreeSoundReply::OAuthAccess(acc),
                                *cb_notify,
                            )
                            .await;
                        }
                        Err(err) => {
                            FreeSoundService::write_error(
                                &tx,
                                Error::Error(err.to_string()),
                                *cb_notify,
                            )
                            .await;
                        }
                    }
                }
                Err(err) => {
                    FreeSoundService::write_error(&tx, Error::Error(err), *cb_notify).await;
                }
            }
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
                                        process_request(request, &rep_tx, &cb_notify).await;
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

#[cfg(test)]
mod tests {}
