use std::{
    ffi::{CStr, CString},
    os::raw::{c_char, c_void},
    ptr::null_mut,
    sync::mpsc::Receiver,
    time::Duration,
};

use tokio::runtime::Runtime;

struct Channel<A> {
    rx: std::sync::mpsc::Receiver<A>,
    tx: std::sync::mpsc::Sender<A>,
}

#[derive(Debug)]
enum HttpRequest {
    Get(String),
}

#[derive(Debug, Default)]
struct HttpResponse {
    body: String,
    status: u16,
}

#[derive(Debug)]
enum HttpReply {
    Get(HttpResponse),
}

#[allow(non_camel_case_types)]
pub struct http_client {
    rt: tokio::runtime::Runtime,
    cb_err: http_client_err_cb,
    cb_result: http_client_result_cb,
    cb_quit: http_client_quit_cb,
    to_worker: Channel<HttpRequest>,
}

impl http_client {
    fn on_err(&self, msg: &str) {
        self.cb_err.exec(msg);
    }

    fn on_result(&self, status: u16, msg: &str) {
        self.cb_result.exec(status, msg);
    }

    fn should_quit(&self) -> bool {
        self.cb_quit.check()
    }

    fn send_request(&self, req: HttpRequest) -> bool {
        match self.to_worker.tx.send(req) {
            Ok(_) => true,
            Err(err) => {
                self.on_err(err.to_string().as_str());
                false
            }
        }
    }

    fn add_reply(&self, rep: HttpReply) {
        match rep {
            HttpReply::Get(resp) => {
                self.on_result(resp.status, resp.body.as_str());
            }
        }
    }
}

#[repr(C)]
#[allow(non_camel_case_types)]
pub struct http_client_err_cb {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, msg: *const c_char)>,
}

impl http_client_err_cb {
    fn exec(&self, msg: &str) {
        let msg = CString::new(msg).unwrap_or_default();
        self.cb.map(|cb| cb(self.user, msg.as_ptr()));
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct http_client_quit_cb {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void) -> bool>,
}

impl http_client_quit_cb {
    fn check(&self) -> bool {
        self.cb.map_or(false, |f| f(self.user))
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct http_client_result_cb {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, status: u16, body: *const c_char)>,
}

impl http_client_result_cb {
    fn exec(&self, status: u16, msg: &str) {
        let msg = CString::new(msg).unwrap_or_default();
        self.cb.map(|cb| cb(self.user, status, msg.as_ptr()));
    }
}

#[must_use]
#[no_mangle]
pub extern "C" fn ceammc_http_client_new(
    cb_err: http_client_err_cb,
    cb_result: http_client_result_cb,
    cb_quit: http_client_quit_cb,
) -> *mut http_client {
    let rt = Runtime::new();
    match rt {
        Ok(rt) => {
            let (tx, rx) = std::sync::mpsc::channel::<HttpRequest>();

            return Box::into_raw(Box::new(http_client {
                rt,
                cb_err,
                cb_result,
                cb_quit,
                to_worker: Channel { rx, tx },
            }));
        }
        Err(err) => {
            cb_err.exec(format!("runtime creation error: {err}").as_str());
            return null_mut();
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
    url: *const c_char,
) -> bool {
    if cli.is_none() {
        eprintln!("NULL client");
        return false;
    }

    let cli = cli.unwrap();
    if url.is_null() {
        cli.on_err("null url string");
        return false;
    }

    let url = unsafe { CStr::from_ptr(url) }.to_str().unwrap_or_default();
    return cli.send_request(HttpRequest::Get(url.to_owned()));
}

#[no_mangle]
pub extern "C" fn ceammc_http_client_runloop(cli: Option<&mut http_client>) -> bool {
    if cli.is_none() {
        eprintln!("NULL client");
        return false;
    }

    let cli = cli.unwrap();

    type ReplyChanMsg = Result<HttpReply, String>;
    type ReplyChannel = tokio::sync::mpsc::Sender<ReplyChanMsg>;

    let res: Result<(), String> = cli.rt.block_on(async {
        let (wtx, mut wrx) = tokio::sync::mpsc::channel::<HttpRequest>(16);
        let (res_tx, mut res_rx) = tokio::sync::mpsc::channel::<ReplyChanMsg>(16);

        async fn write_ok(out: &ReplyChannel, reply: HttpReply) {
            if let Err(err) = out.send(Ok(reply)).await {
                println!(" <- send: error: {err}");
            }
            ()
        }

        async fn write_err(out: &ReplyChannel, err: String) {
            if let Err(err) = out.send(Err(err.to_string())).await {
                println!(" <- send: error: {err}");
            }
            ()
        }

        println!("starting worker thread...");
        let worker = tokio::spawn(async move {
            println!(" -> worker is started");
            while let Some(x) = wrx.recv().await {
                let out_tx = res_tx.clone();

                tokio::spawn(async move {
                    match x {
                        HttpRequest::Get(url) => match reqwest::get(url).await {
                            Ok(resp) => {
                                let status = resp.status().as_u16();
                                match resp.text().await {
                                    Ok(body) => {
                                        write_ok(
                                            &out_tx,
                                            HttpReply::Get(HttpResponse { status, body }),
                                        )
                                        .await
                                    }
                                    Err(err) => write_err(&out_tx, err.to_string()).await,
                                }

                                Ok(())
                            }
                            Err(err) => {
                                write_err(&out_tx, err.to_string()).await;
                                Ok::<(), reqwest::Error>(())
                            }
                        },
                    }
                });
            }
        });

        let (sync_tx, sync_rx) = std::sync::mpsc::channel::<Result<HttpReply, String>>();

        println!("starting result thread...");
        // read and combine results
        let result = tokio::spawn(async move {
            println!(" <- result thread is started");
            loop {
                match res_rx.recv().await {
                    Some(res) => {
                        println!(" <- new result: {:?}", res);
                        let _ = sync_tx.send(res);
                    }
                    None => break,
                }
            }
        });

        println!("starting main loop");
        'main: loop {
            // process all available worker requests
            'worker_in: loop {
                match cli.to_worker.rx.try_recv() {
                    Ok(req) => {
                        if wtx.send(req).await.is_err() {
                            break 'worker_in;
                        }
                    }
                    Err(err) => {
                        // try after sleep
                        break 'worker_in;
                    }
                }

                if cli.should_quit() {
                    break 'main;
                }
            }

            // process all available results
            'worker_out: loop {
                match sync_rx.try_recv() {
                    Ok(reply) => match reply {
                        Ok(reply) => cli.add_reply(reply),
                        Err(err) => {
                            cli.on_err(err.as_str());
                        }
                    },
                    Err(_) => {
                        // try after sleep
                        break 'worker_out;
                    }
                }

                if cli.should_quit() {
                    break 'main;
                }
            }

            tokio::time::sleep(Duration::from_millis(100)).await;

            if cli.should_quit() {
                break 'main;
            }
        }

        worker.abort();
        result.abort();

        Ok(())
    });

    if let Err(e) = res {
        cli.on_err(format!("error: {e}").as_str());
        return false;
    }

    true
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
