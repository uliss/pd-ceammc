use core::slice;
use std::{
    collections::HashMap,
    ffi::{CStr, CString, OsStr, OsString},
    os::raw::{c_char, c_void},
    path::Path,
    ptr::{null, null_mut},
    time::Duration,
};

use reqwest::{RequestBuilder, Response};
use tokio::{
    fs::{metadata, File},
    io::AsyncWriteExt,
};
use tokio_util::codec::{BytesCodec, FramedRead};

use crate::service::Service;
use crate::service::{callback_msg, callback_notify, Error, ServiceCallback};

fn parse_client_options(
    params: Option<&http_client_param>,
    len: usize,
) -> (Vec<HttpClientParam>, Option<CssSelector>, Option<String>) {
    let mut result: Vec<HttpClientParam> = vec![];
    let mut selector = None;
    let mut mime = None;

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
                http_client_param_type::Selector => {
                    selector = Some(CssSelector {
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
                    mime = Some(name);
                    continue;
                }
            };

            result.push(param);
        }
    }

    (result, selector, mime)
}

#[derive(Debug)]
struct UploadRequest {
    url: String,
    file: String,
    file_key: String,
    mime: Option<String>,
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
    None,
    Html,
    InnerHtml,
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
    service: Service<HttpRequest, HttpReply>,
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
    Header,
    Form,
    MultiPart,
    Selector,
    Mime,
    BasicAuth,
}

#[derive(Debug)]
enum HttpClientParam {
    Header(String, String),
    Form(String, String),
    MultiPart(String, String),
    BasicAuth(String, String),
}

#[derive(Debug)]
struct HttpRequest {
    method: HttpRequestMethod,
    params: Vec<HttpClientParam>,
    timeout: Option<u16>,
    selector: Option<CssSelector>,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct http_client_param {
    name: *const c_char,
    value: *const c_char,
    param_type: http_client_param_type,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct http_client_result {
    body: *const c_char,
    status: u16,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct http_client_result_cb {
    user: *mut c_void,
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
                            body: msg.as_ptr(),
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
                            body: null(),
                            status: *status,
                        },
                    )
                });
            }
        }
    }
}

async fn http_response(resp: Response, css: Option<CssSelector>) -> Result<Vec<HttpReply>, Error> {
    let status = resp.status().as_u16();
    match resp.text().await {
        Ok(body) => match apply_selector(css, body) {
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
        },
        Err(err) => Err(Error::Error(err.to_string())),
    }
}

fn process_params(
    mut cli: RequestBuilder,
    params: Vec<HttpClientParam>,
    timeout: Option<u16>,
) -> RequestBuilder {
    let mut form_params = HashMap::new();
    let mut multipart_form = None;

    for p in params {
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

                multipart_form = multipart_form.map(|f| f.text(k, v));
            }
        }
    }

    if !form_params.is_empty() {
        cli = cli.form(&form_params);
    }

    if let Some(mp) = multipart_form {
        cli = cli.multipart(mp);
    }

    if let Some(ms) = timeout {
        cli = cli.timeout(Duration::from_millis(ms as u64));
    }

    cli
}

async fn http_request(req: HttpRequest) -> Result<Vec<HttpReply>, Error> {
    match req.method {
        HttpRequestMethod::Get(url) => {
            let mut client = reqwest::Client::new().get(url);
            client = process_params(client, req.params, req.timeout);

            match client.send().await {
                Ok(resp) => http_response(resp, req.selector).await,
                Err(err) => Err(Error::Error(err.to_string())),
            }
        }
        HttpRequestMethod::Post(url) => {
            let mut client = reqwest::Client::new().post(url);
            client = process_params(client, req.params, req.timeout);

            match client.send().await {
                Ok(resp) => http_response(resp, req.selector).await,
                Err(err) => Err(Error::Error(err.to_string())),
            }
        }
        HttpRequestMethod::Download(dreq) => {
            let mut client = reqwest::Client::new().get(dreq.url);
            client = process_params(client, req.params, req.timeout);

            match client.send().await {
                Ok(mut response) => {
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
                            let mut total: u64 = 0;
                            while let Ok(chunk) = response.chunk().await {
                                if let Some(bytes) = chunk {
                                    if let Err(err) = file.write_all(&bytes).await {
                                        return Err(Error::Error(err.to_string()));
                                    }

                                    total += bytes.len() as u64;
                                    let perc = (100 * total) / nbytes.unwrap();
                                    // eprintln!("download: {perc}% to {full_name}");
                                }
                            }

                            let _ = file.flush().await;

                            Ok(vec![HttpReply::Ok(status.into())])
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

            let mime = upload.mime.unwrap_or("text/plain".to_owned());
            match File::open(upload.file).await {
                Ok(file) => {
                    // read file body stream
                    let stream = FramedRead::new(file, BytesCodec::new());
                    let file_body = reqwest::Body::wrap_stream(stream);

                    // make form part of file
                    match reqwest::multipart::Part::stream(file_body)
                        .file_name(filename)
                        .mime_str(mime.as_str())
                    {
                        Ok(part) => {
                            let mut client = reqwest::Client::new().post(upload.url);
                            client = process_params(client, req.params, req.timeout);
                            // create the multipart form
                            let form = reqwest::multipart::Form::new().part(upload.file_key, part);

                            //send request
                            match client.multipart(form).send().await {
                                Ok(resp) => http_response(resp, req.selector).await,
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
pub extern "C" fn ceammc_http_client_new(
    cb_err: callback_msg,
    cb_post: callback_msg,
    cb_debug: callback_msg,
    cb_log: callback_msg,
    cb_reply: http_client_result_cb,
    cb_notify: callback_notify,
) -> *mut http_client {
    let service = Service::<HttpRequest, HttpReply>::new_async_many(
        //
        cb_err,
        cb_post,
        cb_debug,
        cb_log,
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

#[no_mangle]
pub extern "C" fn ceammc_http_client_free(cli: *mut http_client) {
    if !cli.is_null() {
        drop(unsafe { Box::from_raw(cli) });
    }
}

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

    let (params, selector, _) = parse_client_options(param, param_len);

    let req = HttpRequest {
        method: HttpRequestMethod::Get(url.to_owned()),
        params,
        timeout: None,
        selector,
    };

    eprintln!("{req:?}");

    cli.service.send_request(req)
}

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

    let (params, selector, _) = parse_client_options(param, param_len);

    let req = HttpRequest {
        method: HttpRequestMethod::Post(url.to_owned()),
        params,
        timeout: None,
        selector,
    };

    eprintln!("post: {req:?}");

    cli.service.send_request(req)
}

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

    let (params, selector, mime) = parse_client_options(params, params_len);

    let req = HttpRequest {
        method: HttpRequestMethod::UploadFile(UploadRequest {
            url: url.to_owned(),
            file_key: file_key.unwrap().to_owned(),
            file: file.unwrap().to_owned(),
            mime,
        }),
        params,
        selector,
        timeout: None,
    };

    eprintln!("upload: {req:?}");

    cli.service.send_request(req)
}

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

    let (params, selector, _) = parse_client_options(params, params_len);

    let req = HttpRequest {
        method: HttpRequestMethod::Download(DownloadRequest {
            url,
            filename: file,
            base_dir,
        }),
        params,
        selector,
        timeout: None,
    };

    eprintln!("download: {req:?}");

    cli.service.send_request(req)
}

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

fn apply_selector(css: Option<CssSelector>, body: String) -> Result<Vec<String>, String> {
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
