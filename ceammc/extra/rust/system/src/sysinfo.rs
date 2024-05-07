use std::{
    ffi::CString,
    os::raw::{c_char, c_void},
    time::Duration,
};

use sysinfo::Components;

use crate::system_notify_cb;

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct sysinfo_temp_cb {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, label: *const c_char, cpu_temp: f32)>,
}

impl sysinfo_temp_cb {
    fn exec(&self, label: &CString, value: f32) {
        self.cb.map(|f| f(self.user, label.as_ptr(), value));
    }
}

enum SysInfoReply {
    Temperature(CString, f32),
}

enum SysInfoRequest {
    GetTemp,
    Quit,
}

#[allow(non_camel_case_types)]
pub struct system_info {
    rx: std::sync::mpsc::Receiver<SysInfoReply>,
    tx: std::sync::mpsc::Sender<SysInfoRequest>,
    cb_temp: sysinfo_temp_cb,
}

impl system_info {
    fn request(&self, req: SysInfoRequest) -> bool {
        match self.tx.send(req) {
            Ok(_) => true,
            Err(err) => {
                log::error!("[owner] send error: {err}");
                false
            }
        }
    }

    fn ptr_request(sysinfo: *mut Self, req: SysInfoRequest) -> bool {
        if sysinfo.is_null() {
            log::error!("NULL cpu temp pointer");
            return false;
        }
    
        let sysinfo = unsafe { &mut *sysinfo };
        sysinfo.request(req)
    }
}

impl Drop for system_info {
    fn drop(&mut self) {
        if let Err(err) = self.tx.send(SysInfoRequest::Quit) {
            log::error!("send error: {err}");
        }
    }
}

#[no_mangle]
pub extern "C" fn ceammc_sysinfo_create(
    cb_temp: sysinfo_temp_cb,
    cb_notify: system_notify_cb,
) -> *mut system_info {
    let (reply_tx, reply_rx) = std::sync::mpsc::channel::<SysInfoReply>();
    let (req_tx, req_rx) = std::sync::mpsc::channel::<SysInfoRequest>();

    std::thread::spawn(move || {
        let mut components = Components::new_with_refreshed_list();
        loop {
            match req_rx.recv_timeout(Duration::from_millis(100)) {
                Ok(data) => match data {
                    SysInfoRequest::Quit => {
                        log::debug!("[worker] quit");
                        break;
                    }
                    SysInfoRequest::GetTemp => {
                        log::debug!("[worker] get temperature");
                        for component in &mut components.iter_mut() {
                            component.refresh();

                            let str = component.label();
                            let lbl = CString::new(str).unwrap_or_default();

                            let temp = component.temperature();
                            if temp.is_nan() {
                                // skip
                                continue;
                            }
                            if let Err(err) = reply_tx.send(SysInfoReply::Temperature(lbl, temp)) {
                                log::error!("[worker] send error: {err}");
                                break;
                            }

                            cb_notify.notify();
                        }
                    }
                },
                Err(err) => match err {
                    std::sync::mpsc::RecvTimeoutError::Timeout => continue,
                    std::sync::mpsc::RecvTimeoutError::Disconnected => break,
                },
            }
        }
    });

    Box::into_raw(Box::new(system_info {
        rx: reply_rx,
        tx: req_tx,
        cb_temp,
    }))
}

#[no_mangle]
pub extern "C" fn ceammc_sysinfo_get_temperature(sysinfo: *mut system_info) -> bool {
    system_info::ptr_request(sysinfo, SysInfoRequest::GetTemp)
}

#[no_mangle]
pub extern "C" fn ceammc_sysinfo_process(sysinfo: *mut system_info) -> bool {
    if sysinfo.is_null() {
        log::error!("NULL cpu temp pointer");
        return false;
    }

    let sysinfo = unsafe { &mut *sysinfo };
    loop {
        match sysinfo.rx.try_recv() {
            Ok(reply) => match reply {
                SysInfoReply::Temperature(label, value) => sysinfo.cb_temp.exec(&label, value),
            },
            Err(err) => match err {
                std::sync::mpsc::TryRecvError::Empty => return true,
                std::sync::mpsc::TryRecvError::Disconnected => {
                    log::error!("[owner] process results: {err}");
                    return false;
                }
            },
        };
    }
}

#[no_mangle]
pub extern "C" fn ceammc_sysinfo_free(sysinfo: *mut system_info) {
    if !sysinfo.is_null() {
        drop(unsafe { Box::from_raw(sysinfo) });
    }
}
