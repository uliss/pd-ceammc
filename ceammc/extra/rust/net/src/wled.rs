use std::{
    ffi::{c_char, CStr},
    ptr::null_mut,
    time::Duration,
};

use ceammc_rs_msg::{msg_cb, msg_notify, NumThreads, RequestMessage, TokioClient, TokioRtShutdown};
use log::debug;
use tokio::net::UdpSocket;

pub const WLED_UDP_DEFAULT_PORT: u16 = 21324;

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Debug, Clone)]
pub struct wled_color {
    red: u8,
    green: u8,
    blue: u8,
}

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Debug, Clone)]
pub struct wled_pixel_color {
    color: wled_color,
    index: u8,
}

#[derive(Debug)]
enum Request {
    WARLS(u8, Vec<wled_pixel_color>),
    DRGB(u8, Vec<wled_color>),
    DNRGB(u8, Vec<wled_color>),
    Fill {
        timeout: u8,
        color: wled_color,
        from: u16,
        len: u16,
    },
}

impl Request {
    fn get_msec(&self) -> u64 {
        (*match self {
            Request::WARLS(t, _) => t,
            Request::DRGB(t, _) => t,
            Request::DNRGB(t, _) => t,
            Request::Fill {
                timeout,
                color: _,
                from: _,
                len: _,
            } => timeout,
        })
        .into()
    }
}

#[derive(Debug, Clone)]
struct Reply {}

#[allow(non_camel_case_types)]
pub struct wled_udp {
    obj: ceammc_rs_msg::TokioClient<Request, Reply>,
    on_msg: msg_cb,
}

struct Packet {
    data: Vec<u8>,
}

impl Packet {
    fn new() -> Self {
        Self { data: vec![2; 0] }
    }

    fn set(&mut self, req: &Request) {
        match req {
            Request::WARLS(timeout, wled_pixel_colors) => {
                self.data.reserve(wled_pixel_colors.len() + 2);
                self.data.resize(2, 0);
                self.data[0] = 1;
                self.data[1] = *timeout;
                for x in wled_pixel_colors.iter() {
                    self.data.push(x.index);
                    self.data.push(x.color.red);
                    self.data.push(x.color.green);
                    self.data.push(x.color.blue);
                }
            }
            Request::DRGB(_, _wled_colors) => todo!(),
            Request::DNRGB(_, _wled_colors) => todo!(),
            Request::Fill {
                timeout,
                color,
                from,
                len,
            } => {
                let len = *len;
                self.data.reserve(len as usize + 4);
                self.data.resize(4, 0);
                self.data[0] = 4;
                self.data[1] = *timeout;
                let [high, low] = from.to_be_bytes();
                self.data[2] = high;
                self.data[3] = low;
                for _ in 0..len {
                    self.data.push(color.red);
                    self.data.push(color.green);
                    self.data.push(color.blue);
                }
                debug!("{:?}", self.data);
            }
        }
    }
}

impl wled_udp {
    fn new(addr: &CStr, port: u16, notify: msg_notify, on_msg: msg_cb) -> Self {
        let addr = addr.to_string_lossy().to_string();
        let obj = TokioClient::<Request, Reply>::start_worker(
            NumThreads::Num1,
            TokioRtShutdown::NoWait,
            32,
            async move |mut channel, _cancel| {
                let wled_address = format!("{addr}:{port}");
                // let wled_address = "192.168.1.100:21324";
                let socket = UdpSocket::bind("0.0.0.0:0")
                    .await
                    .map_err(|err| err.to_string())?;

                let mut packet = Packet::new();

                let mut timeout_msec = 1000;

                loop {
                    match tokio::time::timeout(
                        Duration::from_millis(timeout_msec),
                        channel.from_client.recv(),
                    )
                    .await
                    {
                        Ok(Some(req)) => match req {
                            RequestMessage::Message(req) => {
                                //
                                packet.set(&req);
                                timeout_msec = req.get_msec();
                            }
                            RequestMessage::Quit => {
                                log::debug!("quit");
                                break;
                            }
                        },
                        Ok(None) => break,
                        Err(_err) => {
                            socket
                                .send_to(&packet.data, &wled_address)
                                .await
                                .map_err(|err| err.to_string())?;
                        }
                    }
                }

                Ok(())
            },
            notify,
            on_msg,
        );

        Self { obj, on_msg }
    }

    fn send_request(cli: *mut Self, req: Request) -> bool {
        if cli.is_null() {
            false
        } else {
            let cli = unsafe { &*cli };
            cli.obj.send_checked(req, &cli.on_msg)
        }
    }
}

#[no_mangle]
/// create new wled UDP client
/// @param addr - wled device ip address or server name
/// @param port - wled device port
/// @param notify - client notification from worker
/// @param on_msg - client callback for messages from worker
/// @param on_data - client callback for data from worker
/// @return pointer to handle or nullptr on error
pub extern "C" fn ceammc_wled_udp_new(
    addr: *const c_char,
    port: u16,
    notify: msg_notify,
    on_msg: msg_cb,
) -> *mut wled_udp {
    if addr.is_null() {
        on_msg.error_str("NULL address pointer");
        return null_mut();
    }

    let addr = unsafe { CStr::from_ptr(addr) };
    return Box::into_raw(Box::new(wled_udp::new(addr, port, notify, on_msg)));
}

#[no_mangle]
/// close and free wled device
/// @param cli - wled device handle (nullable)
pub extern "C" fn ceammc_wled_udp_free(cli: *mut wled_udp) {
    if !cli.is_null() {
        let _ = unsafe { Box::from_raw(cli) };
    }
}

#[no_mangle]
/// send wled pixels
/// @param cli - esphome device handle
/// @param data - pointer to pixel data (nullable)
/// @param len - pixels count
/// @param timeout - wled timeout
/// @return true on sucess, false on error (if device is disconnected etc.)
pub extern "C" fn ceammc_wled_udp_set_pixels(
    cli: *mut wled_udp,
    data: *const wled_pixel_color,
    len: usize,
    timeout: u8,
) -> bool {
    if data.is_null() || len == 0 {
        return false;
    }

    let pixels = unsafe { std::slice::from_raw_parts(data, len) };
    wled_udp::send_request(cli, Request::WARLS(timeout, pixels.to_vec()))
}

#[no_mangle]
/// fill wled pixels
/// @param cli - esphome device handle
/// @return true on sucess, false on error (if device is disconnected etc.)
pub extern "C" fn ceammc_wled_udp_fill(
    cli: *mut wled_udp,
    color: wled_color,
    from: u16,
    len: u16,
    timeout: u8,
) -> bool {
    wled_udp::send_request(
        cli,
        Request::Fill {
            timeout: timeout,
            color: color,
            from: from,
            len: len,
        },
    )
}
