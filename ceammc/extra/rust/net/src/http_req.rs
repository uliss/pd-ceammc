use std::{
    ffi::{CStr, CString},
    os::raw::{c_char, c_void},
    ptr::null_mut,
};

use crate::service::Service;
use crate::service::{callback_msg, callback_notify, Error, ServiceCallback};

#[derive(Debug)]
enum HttpRequestMethod {
    Get(String),
}

#[derive(Debug)]
struct HttpRequest {
    method: HttpRequestMethod,
    css_selector: String,
}

#[derive(Debug, Default)]
pub struct HttpResponse {
    body: String,
    status: u16,
}

#[derive(Debug)]
pub enum HttpReply {
    Get(HttpResponse),
}

#[allow(non_camel_case_types)]
pub struct http_client {
    service: Service<HttpRequest, HttpReply>,
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
        }
    }
}

async fn http_request(req: HttpRequest) -> Result<Vec<HttpReply>, Error> {
    match req.method {
        HttpRequestMethod::Get(url) => match reqwest::get(url).await {
            Ok(resp) => {
                let status = resp.status().as_u16();
                match resp.text().await {
                    Ok(body) => {
                        let css = req.css_selector;
                        match apply_selector(css, body) {
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
            Err(err) => Err(Error::Error(err.to_string())),
        },
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
    css_sel: Option<&c_char>,
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
    let mut req = HttpRequest {
        method: HttpRequestMethod::Get(url.to_owned()),
        css_selector: String::default(),
    };

    if let Some(css) = css_sel {
        req.css_selector = unsafe { CStr::from_ptr(css) }
            .to_str()
            .unwrap_or_default()
            .to_owned();
    }

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

fn apply_selector(css: String, body: String) -> Result<Vec<String>, String> {
    if !css.is_empty() {
        let sel = scraper::Selector::parse(css.as_str());
        match sel {
            Ok(sel) => {
                println!("CSS selector: {css}");
                let html = scraper::Html::parse_document(body.as_str());

                let v: Vec<_> = html.select(&sel).map(|x| x.html()).collect();

                return Ok(v);
            }
            Err(err) => {
                return Err(format!("invalid CSS selector: '{css}' ({err})"));
            }
        };
    } else {
        return Ok(vec![body]);
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
}
