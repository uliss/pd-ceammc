use core::slice;
use std::{
    collections::HashMap,
    ffi::{CStr, CString, OsStr},
    os::raw::{c_char, c_void},
    path::Path,
    ptr::null_mut,
    time::Duration,
};

use regex::Regex;
use reqwest::{RequestBuilder, Response};
use tokio::{fs::File, io::AsyncWriteExt};
use tokio_util::codec::{BytesCodec, FramedRead};

use crate::service::Service;
use crate::service::{callback_msg, callback_notify, callback_progress, Error, ServiceCallback};

type HttpSenderTx = tokio::sync::mpsc::Sender<Result<HttpReply, Error>>;
type HttpService = Service<HttpRequest, HttpReply>;

fn parse_client_options(params: Option<&http_client_param>, len: usize) -> HttpClientParams {
    let mut result = HttpClientParams::new();

    if let Some(params) = params {
        let params = unsafe { slice::from_raw_parts(params, len) };
        for p in params {
            let name = if p.name.is_null() {
                String::default()
            } else {
                unsafe { CStr::from_ptr(p.name) }
                    .to_str()
                    .unwrap_or_default()
                    .to_owned()
            };

            let value = if p.value.is_null() {
                String::default()
            } else {
                unsafe { CStr::from_ptr(p.value) }
                    .to_str()
                    .unwrap_or_default()
                    .to_owned()
            };

            let param = match p.param_type {
                http_client_param_type::Header => HttpClientParam::Header(name, value),
                http_client_param_type::Form => HttpClientParam::Form(name, value),
                http_client_param_type::MultiPart => HttpClientParam::MultiPart(name, value),
                http_client_param_type::BasicAuth => HttpClientParam::BasicAuth(name, value),
                http_client_param_type::StripWhiteSpaces => {
                    result.strip_ws = true;
                    continue;
                }
                http_client_param_type::Selector => {
                    result.selector = Some(CssSelector {
                        name,
                        content: match value.as_str() {
                            "inner" => http_client_select_type::InnerHtml,
                            "text" => http_client_select_type::Text,
                            "html" => http_client_select_type::Html,
                            "none" => http_client_select_type::None,
                            _ => http_client_select_type::Html,
                        },
                    });
                    continue;
                }
                http_client_param_type::Mime => {
                    result.mime = Some(name);
                    continue;
                }
            };

            result.params.push(param);
        }
    }

    result
}

#[derive(Debug)]
struct UploadRequest {
    url: String,
    file: String,
    file_key: String,
}

#[derive(Debug)]
struct DownloadRequest {
    url: String,
    filename: String,
    base_dir: String,
}

#[derive(Debug)]
enum HttpRequestMethod {
    Get(String),
    Post(String),
    UploadFile(UploadRequest),
    Download(DownloadRequest),
}

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Debug)]
pub enum http_client_select_type {
    /// returns empty string
    None,
    /// returns html content of selected element (trimmed)
    Html,
    /// returns inner html content of selected element (trimmed)
    InnerHtml,
    /// returns trimmed text of selected element
    Text,
}

#[derive(Debug)]
struct CssSelector {
    name: String,
    content: http_client_select_type,
}

#[derive(Debug, Default)]
pub struct HttpResponse {
    body: String,
    status: u16,
}

#[derive(Debug)]
pub enum HttpReply {
    Get(HttpResponse),
    Ok(u16),
}

#[allow(non_camel_case_types)]
pub struct http_client {
    service: HttpService,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct http_client_init {
    _dummy: *const c_void,
}

impl http_client {
    fn cstr2string(&self, cstr: Option<&c_char>, name: &str) -> Option<&str> {
        if cstr.is_none() {
            self.service
                .on_error(format!("{name}: null string pointer").as_str());
            return None;
        }

        match unsafe { CStr::from_ptr(cstr.unwrap()) }.to_str() {
            Ok(s) => Some(s),
            Err(err) => {
                self.service.on_error(format!("{name}: {err}").as_str());
                return None;
            }
        }
    }
}

#[derive(Debug)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub enum http_client_param_type {
    /// header key/value
    Header,
    /// form key/value
    Form,
    /// multipart key/value
    MultiPart,
    /// CSS selector/output type
    Selector,
    /// mime type (for uploading)
    Mime,
    /// username/password
    BasicAuth,
    /// strip whitespaces
    StripWhiteSpaces,
}

#[derive(Debug)]
enum HttpClientParam {
    Header(String, String),
    Form(String, String),
    MultiPart(String, String),
    BasicAuth(String, String),
}

#[derive(Debug, Default)]
struct HttpClientParams {
    params: Vec<HttpClientParam>,
    selector: Option<CssSelector>,
    timeout: Option<u16>,
    mime: Option<String>,
    strip_ws: bool,
}

impl HttpClientParams {
    fn new() -> Self {
        HttpClientParams {
            params: vec![],
            selector: None,
            timeout: None,
            mime: None,
            strip_ws: false,
        }
    }
}

#[derive(Debug)]
struct HttpRequest {
    method: HttpRequestMethod,
    params: HttpClientParams,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct http_client_param {
    /// param name or key
    name: *const c_char,
    /// param value
    value: *const c_char,
    /// parameter type
    param_type: http_client_param_type,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct http_client_result {
    /// reply text (can be NULL!)
    data: *const c_char,
    /// HTTP status
    status: u16,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct http_client_result_cb {
    /// user data pointer (can be NULL)
    user: *mut c_void,
    /// callback function (can be NULL)
    cb: Option<extern "C" fn(user: *mut c_void, &http_client_result)>,
}

impl ServiceCallback<HttpReply> for http_client_result_cb {
    fn exec(&self, data: &HttpReply) {
        match data {
            HttpReply::Get(resp) => {
                let msg = CString::new(resp.body.clone()).unwrap_or_default();
                self.cb.map(|cb| {
                    cb(
                        self.user,
                        &http_client_result {
                            data: msg.as_ptr(),
                            status: resp.status,
                        },
                    )
                });
            }
            HttpReply::Ok(status) => {
                self.cb.map(|cb| {
                    cb(
                        self.user,
                        &http_client_result {
                            data: CStr::from_bytes_with_nul(b"\0").unwrap().as_ptr(),
                            status: *status,
                        },
                    )
                });
            }
        }
    }
}

async fn http_response(resp: Response, params: &HttpClientParams) -> Result<Vec<HttpReply>, Error> {
    let status = resp.status().as_u16();
    match resp.text().await {
        Ok(mut body) => {
            if params.strip_ws {
                let re_ws = Regex::new(r"[[:space:]]+").unwrap();
                body = re_ws.replace_all(&body, " ").to_string();
            }
            match apply_selector(&params.selector, &body) {
                Ok(v) => Ok(v
                    .iter()
                    .map(|txt| {
                        HttpReply::Get(HttpResponse {
                            status,
                            body: txt.clone(),
                        })
                    })
                    .collect()),
                Err(err) => Err(Error::Error(err)),
            }
        }
        Err(err) => Err(Error::Error(err.to_string())),
    }
}

fn process_params(mut cli: RequestBuilder, params: &HttpClientParams) -> RequestBuilder {
    let mut form_params = HashMap::new();
    let mut multipart_form = None;

    for p in &params.params {
        match p {
            HttpClientParam::Header(k, v) => {
                cli = cli.header(k, v);
            }
            HttpClientParam::Form(k, v) => {
                form_params.insert(k, v);
            }
            HttpClientParam::BasicAuth(username, password) => {
                cli = cli.basic_auth(username, Some(password));
            }
            HttpClientParam::MultiPart(k, v) => {
                if multipart_form.is_none() {
                    multipart_form = Some(reqwest::multipart::Form::new());
                }

                multipart_form = multipart_form.map(|f| f.text(k.clone(), v.clone()));
            }
        }
    }

    if !form_params.is_empty() {
        cli = cli.form(&form_params);
    }

    if let Some(mp) = multipart_form {
        cli = cli.multipart(mp);
    }

    if let Some(ms) = params.timeout {
        cli = cli.timeout(Duration::from_millis(ms as u64));
    }

    cli
}

async fn http_request(
    req: HttpRequest,
    tx: HttpSenderTx,
    cb: callback_notify,
) -> Result<Vec<HttpReply>, Error> {
    match req.method {
        HttpRequestMethod::Get(url) => {
            let mut client = reqwest::Client::new().get(url);
            client = process_params(client, &req.params);

            match client.send().await {
                Ok(resp) => http_response(resp, &req.params).await,
                Err(err) => Err(Error::Error(err.to_string())),
            }
        }
        HttpRequestMethod::Post(url) => {
            let mut client = reqwest::Client::new().post(url);
            client = process_params(client, &req.params);

            match client.send().await {
                Ok(resp) => http_response(resp, &req.params).await,
                Err(err) => Err(Error::Error(err.to_string())),
            }
        }
        HttpRequestMethod::Download(dreq) => {
            let mut client = reqwest::Client::new().get(dreq.url);
            client = process_params(client, &req.params);

            match client.send().await {
                Ok(mut response) => {
                    if !response.status().is_success() {
                        return Err(Error::Error(format!(
                            "{}: {}",
                            response.status(),
                            response.url()
                        )));
                    }

                    let dir = Path::new(&dreq.base_dir);
                    let fname = if dreq.filename.is_empty() {
                        const DEFAULT_NAME: &str = "tmp.bin";
                        // auto detect
                        Path::new(
                            response
                                .url()
                                .path_segments()
                                .and_then(|segments| segments.last())
                                .and_then(|name| if name.is_empty() { None } else { Some(name) })
                                .unwrap_or(
                                    Path::new(
                                        response
                                            .headers()
                                            .get("Content-Disposition")
                                            .and_then(|x| Some(x.to_str().unwrap_or(DEFAULT_NAME)))
                                            .unwrap_or(DEFAULT_NAME),
                                    )
                                    .file_name()
                                    .unwrap_or_else(|| &OsStr::new(DEFAULT_NAME))
                                    .to_str()
                                    .unwrap_or(DEFAULT_NAME),
                                ),
                        )
                    } else {
                        Path::new(&dreq.filename)
                    };

                    let full_name = if fname.is_relative() {
                        if !dir.exists() {
                            return Err(Error::Error(format!(
                                "base directory not exists: {dir:?}"
                            )));
                        }

                        dir.join(&fname)
                            .as_path()
                            .to_str()
                            .unwrap_or_default()
                            .to_owned()
                    } else {
                        fname.to_str().unwrap_or_default().to_owned()
                    };

                    let status = response.status();
                    let nbytes = response.content_length();

                    match File::create(&full_name).await {
                        Ok(mut file) => {
                            let total: u64 = nbytes.unwrap_or(0);
                            let mut bytes_send: u64 = 0;

                            while let Ok(chunk) = response.chunk().await {
                                if let Some(bytes) = chunk {
                                    if let Err(err) = file.write_all(&bytes).await {
                                        return Err(Error::Error(err.to_string()));
                                    }

                                    if total > 0 {
                                        bytes_send += bytes.len() as u64;
                                        let perc = (100 * bytes_send) / total;
                                        // eprintln!("done: {perc}%");
                                        HttpService::write_progress(&tx, perc as u8, cb).await;
                                    }
                                } else {
                                    break;
                                }
                            }

                            if let Err(err) = file.flush().await {
                                Err(Error::Error(err.to_string()))
                            } else {
                                HttpService::write_debug(
                                    &tx,
                                    format!("file downloaded to '{full_name}'"),
                                    cb,
                                )
                                .await;
                                Ok(vec![HttpReply::Ok(status.into())])
                            }
                        }
                        Err(err) => Err(Error::Error(err.to_string())),
                    }
                }
                Err(err) => Err(Error::Error(err.to_string())),
            }
        }
        HttpRequestMethod::UploadFile(upload) => {
            let filename = std::path::Path::new(upload.file.clone().as_str())
                .file_name()
                .unwrap_or_default()
                .to_str()
                .unwrap_or_default()
                .to_owned();

            match File::open(upload.file).await {
                Ok(file) => {
                    // read file body stream
                    let stream = FramedRead::new(file, BytesCodec::new());
                    let file_body = reqwest::Body::wrap_stream(stream);

                    let mime = req.params.mime.clone().unwrap_or("text/plain".to_owned());
                    // make form part of file
                    match reqwest::multipart::Part::stream(file_body)
                        .file_name(filename)
                        .mime_str(mime.as_str())
                    {
                        Ok(part) => {
                            let mut client = reqwest::Client::new().post(upload.url);
                            client = process_params(client, &req.params);
                            // create the multipart form
                            let form = reqwest::multipart::Form::new().part(upload.file_key, part);

                            //send request
                            match client.multipart(form).send().await {
                                Ok(resp) => http_response(resp, &req.params).await,
                                Err(err) => Err(Error::Error(err.to_string())),
                            }
                        }
                        Err(err) => Err(Error::Error(err.to_string())),
                    }
                }
                Err(err) => Err(Error::Error(err.to_string())),
            }
        }
    }
}

#[must_use]
#[no_mangle]
/// create new http client
/// @param cb_err - called in the main thread on error message
/// @param cb_post - called in the main thread on post message
/// @param cb_debug - called in the main thread on debug message
/// @param cb_log - called in the main thread on log message
/// @param cb_progress - called in the main thread on progress message
/// @param cb_reply - called in the main thread on result reply message
/// @param cb_notify - called in the worker thread (!) to notify main thread
/// @return pointer to new client or NULL on error
pub extern "C" fn ceammc_http_client_new(
    _params: http_client_init,
    cb_err: callback_msg,
    cb_post: callback_msg,
    cb_debug: callback_msg,
    cb_log: callback_msg,
    cb_progress: callback_progress,
    cb_reply: http_client_result_cb,
    cb_notify: callback_notify,
) -> *mut http_client {
    let service = HttpService::new_async_many(
        //
        cb_err,
        cb_post,
        cb_debug,
        cb_log,
        cb_progress,
        Box::new(cb_reply),
        cb_notify,
        http_request,
        16,
    );

    match service {
        Some(service) => Box::into_raw(Box::new(http_client { service })),
        None => {
            cb_err.exec("http client creation error");
            null_mut()
        }
    }
}

/// free http client
/// @param cli - pointer to http client (can be NULL)
#[no_mangle]
pub extern "C" fn ceammc_http_client_free(cli: *mut http_client) {
    if !cli.is_null() {
        drop(unsafe { Box::from_raw(cli) });
    }
}

/// do http get request
/// @param cli - http client pointer
/// @param url - requested URL
/// @param param - pointer to array of request parameters (can be NULL)
/// @param param_len - array size of request parameters
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_http_client_get(
    cli: Option<&mut http_client>,
    url: Option<&c_char>,
    param: Option<&http_client_param>,
    param_len: usize,
) -> bool {
    if cli.is_none() {
        eprintln!("NULL client");
        return false;
    }

    let cli = cli.unwrap();
    if url.is_none() {
        cli.service.on_error("null url string");
        return false;
    }

    let url = unsafe { CStr::from_ptr(url.unwrap()) }
        .to_str()
        .unwrap_or_default();

    let params = parse_client_options(param, param_len);

    let req = HttpRequest {
        method: HttpRequestMethod::Get(url.to_owned()),
        params,
    };

    eprintln!("{req:?}");

    cli.service.send_request(req)
}

/// do http post request
/// @param cli - http client pointer
/// @param url - requested URL
/// @param param - pointer to array of request parameters (can be NULL)
/// @param param_len - array size of request parameters
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_http_client_post(
    cli: Option<&mut http_client>,
    url: Option<&c_char>,
    param: Option<&http_client_param>,
    param_len: usize,
) -> bool {
    if cli.is_none() {
        eprintln!("NULL client");
        return false;
    }

    let cli = cli.unwrap();
    if url.is_none() {
        cli.service.on_error("null url string");
        return false;
    }

    let url = unsafe { CStr::from_ptr(url.unwrap()) }
        .to_str()
        .unwrap_or_default();

    let params = parse_client_options(param, param_len);

    let req = HttpRequest {
        method: HttpRequestMethod::Post(url.to_owned()),
        params,
    };

    eprintln!("post: {req:?}");

    cli.service.send_request(req)
}

/// upload file with POST request
/// @param cli - http client pointer
/// @param url - requested URL
/// @param file - filename
/// @param file_key - file part name in multipart data
/// @param param - pointer to array of request parameters (can be NULL)
/// @param param_len - array size of request parameters
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_http_client_upload(
    cli: Option<&mut http_client>,
    url: Option<&c_char>,
    file: Option<&c_char>,
    file_key: Option<&c_char>,
    params: Option<&http_client_param>,
    params_len: usize,
) -> bool {
    if cli.is_none() {
        eprintln!("NULL client");
        return false;
    }

    let cli = cli.unwrap();
    if url.is_none() {
        cli.service.on_error("null url string");
        return false;
    }
    let url = unsafe { CStr::from_ptr(url.unwrap()) }
        .to_str()
        .unwrap_or_default();

    let file = cli.cstr2string(file, "file");
    let file_key = cli.cstr2string(file_key, "file_key");

    if file.is_none() || file_key.is_none() {
        return false;
    }

    let params = parse_client_options(params, params_len);

    let req = HttpRequest {
        method: HttpRequestMethod::UploadFile(UploadRequest {
            url: url.to_owned(),
            file_key: file_key.unwrap().to_owned(),
            file: file.unwrap().to_owned(),
        }),
        params,
    };

    eprintln!("upload: {req:?}");

    cli.service.send_request(req)
}

/// download file with GET request
/// @param cli - http client pointer
/// @param url - requested URL
/// @param file - output filename (if NULL - try to auto detect)
/// @param dir - base directory to save if filename is not absolute
/// @param param - pointer to array of request parameters (can be NULL)
/// @param param_len - array size of request parameters
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_http_client_download(
    cli: Option<&mut http_client>,
    url: Option<&c_char>,
    file: Option<&c_char>,
    dir: Option<&c_char>,
    params: Option<&http_client_param>,
    params_len: usize,
) -> bool {
    if cli.is_none() {
        eprintln!("NULL client");
        return false;
    }

    let cli = cli.unwrap();
    if url.is_none() {
        cli.service.on_error("null url string");
        return false;
    }
    let url = unsafe { CStr::from_ptr(url.unwrap()) }
        .to_str()
        .unwrap_or_default()
        .to_owned();

    let file = cli.cstr2string(file, "file").unwrap_or_default().to_owned();
    let base_dir = cli.cstr2string(dir, "dir").unwrap_or_default().to_owned();

    let params = parse_client_options(params, params_len);

    let req = HttpRequest {
        method: HttpRequestMethod::Download(DownloadRequest {
            url,
            filename: file,
            base_dir,
        }),
        params,
    };

    eprintln!("download: {req:?}");

    cli.service.send_request(req)
}

/// process all results that are ready
/// @param cli - http client pointer
/// @return true on success, false on error
#[no_mangle]
pub extern "C" fn ceammc_http_client_process(cli: Option<&mut http_client>) -> bool {
    if cli.is_none() {
        eprintln!("NULL client");
        return false;
    }

    let cli = cli.unwrap();
    cli.service.process_results();
    true
}

fn apply_selector(css: &Option<CssSelector>, body: &String) -> Result<Vec<String>, String> {
    match css {
        Some(css) => {
            let sel = scraper::Selector::parse(css.name.as_str());
            match sel {
                Ok(sel) => {
                    eprintln!("CSS selector: {css:?}");
                    let html = scraper::Html::parse_document(body.as_str());

                    let v: Vec<_> = html
                        .select(&sel)
                        .map(|x| match css.content {
                            http_client_select_type::None => String::default(),
                            http_client_select_type::Text => x.text().collect(),
                            http_client_select_type::Html => x.html(),
                            http_client_select_type::InnerHtml => x.inner_html(),
                        })
                        .map(|x| x.trim().to_owned())
                        .collect();

                    return Ok(v);
                }
                Err(err) => {
                    return Err(format!("invalid CSS selector: '{}' ({err})", css.name));
                }
            };
        }
        None => {
            return Ok(vec![body.trim().to_string()]);
        }
    }
}

#[cfg(test)]
mod tests {
    use tokio::runtime::Runtime;

    #[test]
    fn test_ok() {
        let rt = Runtime::new().unwrap();
        rt.block_on(async {
            let res = reqwest::get("http://ya.rusdf/").await;

            println!("result: {res:?}");
        });
    }

    #[test]
    fn test_to_file_path() {
        let url = url::Url::parse("file:///file").unwrap();
        assert_eq!(url.path(), "/file");
        assert_eq!(url.path_segments().unwrap().last().unwrap(), "file");

        let url = url::Url::parse("http://host/path").unwrap();
        assert_eq!(url.path(), "/path");
        assert_eq!(url.path_segments().unwrap().last().unwrap(), "path");

        let url = url::Url::parse("http://host/path/file.bin?key=value#4").unwrap();
        assert_eq!(url.path_segments().unwrap().last().unwrap(), "file.bin");
    }
}
