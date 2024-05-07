use std::{
    ffi::CString,
    os::raw::{c_char, c_void},
    time::Duration,
};

use sysinfo::Components;

use crate::hw_notify_cb;

enum CpuTempReply {
    Temp(CString, f32),
}

enum CpuTempRequest {
    GetTemp,
    Quit,
}

#[allow(non_camel_case_types)]
pub struct hw_cputemp {
    rx: std::sync::mpsc::Receiver<CpuTempReply>,
    tx: std::sync::mpsc::Sender<CpuTempRequest>,
    user: *mut c_void,
    cb_temp: Option<extern "C" fn(user: *mut c_void, label: *const c_char, cpu_temp: f32)>,
}

impl Drop for hw_cputemp {
    fn drop(&mut self) {
        if let Err(err) = self.tx.send(CpuTempRequest::Quit) {
            log::error!("send error: {err}");
        }
    }
}

#[no_mangle]
pub extern "C" fn ceammc_hw_cputemp_create(
    user: *mut c_void,
    cb_temp: Option<extern "C" fn(user: *mut c_void, label: *const c_char, cpu_temp: f32)>,
    cb_notify: hw_notify_cb,
) -> *mut hw_cputemp {
    let (reply_tx, reply_rx) = std::sync::mpsc::channel::<CpuTempReply>();
    let (req_tx, req_rx) = std::sync::mpsc::channel::<CpuTempRequest>();

    std::thread::spawn(move || {
        let mut components = Components::new_with_refreshed_list();
        loop {
            match req_rx.recv_timeout(Duration::from_millis(100)) {
                Ok(data) => match data {
                    CpuTempRequest::Quit => {
                        log::debug!("[worker] quit");
                        break;
                    }
                    CpuTempRequest::GetTemp => {
                        for component in &mut components.iter_mut() {
                            component.refresh();

                            let str = component.label();

                            #[cfg(target_os = "macos")]
                            {
                                if !str.starts_with("GPU") && !str.starts_with("pACC") && !str.starts_with("eACC")   {
                                    continue;
                                }
                            }
                           
                            let lbl = CString::new(str).unwrap_or_default();

                            let temp = component.temperature();
                            if temp.is_nan() { // skip
                                continue;
                            }
                            if let Err(err) = reply_tx.send(CpuTempReply::Temp(lbl, temp)) {
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

    Box::into_raw(Box::new(hw_cputemp {
        rx: reply_rx,
        tx: req_tx,
        user,
        cb_temp,
    }))
}

#[no_mangle]
pub extern "C" fn ceammc_hw_cputemp_get(cpu_temp: *mut hw_cputemp) -> bool {
    if cpu_temp.is_null() {
        log::error!("NULL cpu temp pointer");
        return false;
    }

    let cpu_temp = unsafe { &mut *cpu_temp };
    match cpu_temp.tx.send(CpuTempRequest::GetTemp) {
        Ok(_) => true,
        Err(err) => {
            log::error!("[owner] send error: {err}");
            false
        }
    }
}

#[no_mangle]
pub extern "C" fn ceammc_hw_cputemp_process(cpu_temp: *mut hw_cputemp) -> bool {
    if cpu_temp.is_null() {
        log::error!("NULL cpu temp pointer");
        return false;
    }

    let cpu_temp = unsafe { &mut *cpu_temp };
    loop {
        match cpu_temp.rx.try_recv() {
            Ok(reply) => match reply {
                CpuTempReply::Temp(label, value) => cpu_temp
                    .cb_temp
                    .map(|f| f(cpu_temp.user, label.as_ptr(), value)),
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
pub extern "C" fn ceammc_hw_cputemp_free(cpu_temp: *mut hw_cputemp) {
    if !cpu_temp.is_null() {
        drop(unsafe { Box::from_raw(cpu_temp) });
    }
}
