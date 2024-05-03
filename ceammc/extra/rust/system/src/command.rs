use std::os::raw::c_void;
use std::process::ExitStatus;
use std::ptr::slice_from_raw_parts;
use std::{os::raw::c_char, ptr::null_mut};

use std::ffi::{CStr, CString};

use duct::Expression;
use lazy_static::lazy_static;
use regex::bytes::Regex;

lazy_static! {
    static ref FUDI_RE: Regex = Regex::new(r"(?m)((?:(?s).*?)[^\\])\n?;").unwrap();
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct system_process_cmd {
    prog: *const c_char,
    argv: *const *const c_char,
    argc: usize,
}

impl system_process_cmd {
    fn to_cmd(&self) -> Expression {
        let program = unsafe { CStr::from_ptr(self.prog) }
            .to_string_lossy()
            .to_string();

        let mut args = vec![];
        if !self.argv.is_null() {
            for x in unsafe { &*slice_from_raw_parts(self.argv, self.argc) } {
                if x.is_null() {
                    continue;
                }
                args.push(unsafe { CStr::from_ptr(*x) }.to_string_lossy().to_string());
            }
        }

        duct::cmd(program, args)
    }
}

#[allow(non_camel_case_types)]
pub struct system_process {
    cmd: Option<duct::Expression>,
    handle: Option<duct::Handle>,
    mode: system_process_mode,
    capture_stdout: bool,
    capture_stderr: bool,
    user: *mut c_void,
    on_err: Option<extern "C" fn(user: *mut c_void, msg: *const c_char)>,
    on_stdout_data: Option<extern "C" fn(user: *mut c_void, data: *const u8, len: usize)>,
    on_stderr_data: Option<extern "C" fn(user: *mut c_void, data: *const u8, len: usize)>,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub enum system_process_rc {
    Error,
    Running,
    Ready,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub enum system_process_mode {
    Bytes,
    Lines,
    Fudi,
    String,
}

impl system_process {
    fn kill(&mut self) -> bool {
        if let Some(h) = &self.handle {
            match h.kill() {
                Ok(_) => true,
                Err(err) => {
                    self.on_err(format!("can't terminate process: {err}").as_str());
                    false
                }
            }
        } else {
            false
        }
    }

    fn try_wait_or_kill(&mut self) {
        if let Some(h) = &self.handle {
            match h.try_wait() {
                Ok(output) => {
                    if output.is_none() {
                        // still running
                        if self.kill() {
                            self.handle = None;
                        }
                    }
                }
                Err(err) => {
                    self.on_err(format!("process error: {err}").as_str());
                    if self.kill() {
                        self.handle = None;
                    }
                }
            }
        }
    }

    fn is_running(&self) -> bool {
        if let Some(h) = &self.handle {
            if let Ok(None) = h.try_wait() {
                return true;
            } else {
                return false;
            }
        }

        false
    }

    fn on_err(&self, msg: &str) {
        self.on_err.map(|f| {
            f(self.user, CString::new(msg).unwrap_or_default().as_ptr());
        });

        eprintln!("error: {msg}");
    }

    fn exec(&mut self) -> bool {
        match &self.cmd {
            Some(expr) => {
                if self.is_running() {
                    self.on_err("process already running");
                    return false;
                }

                let mut new_expr = expr.clone();
                if self.capture_stderr {
                    new_expr = expr.stderr_capture()
                }

                if self.capture_stdout {
                    new_expr = new_expr.stdout_capture();
                }

                match new_expr.start() {
                    Ok(res) => {
                        self.handle = Some(res);
                        return true;
                    }
                    Err(err) => {
                        self.on_err(format!("exec error: {err}").as_str());
                        return false;
                    }
                }
            }
            None => {
                self.on_err("empty command");
                return false;
            }
        }
    }

    fn process_output(
        &self,
        data: &[u8],
        cb: Option<extern "C" fn(user: *mut c_void, data: *const u8, len: usize)>,
    ) {
        cb.map(|f| match self.mode {
            system_process_mode::Bytes => f(self.user, data.as_ptr(), data.len()),
            system_process_mode::Lines  => {
                data.split(|x| *x == b'\n') // split by \n
                    .map(|str| {
                        if let Some(str) = str.strip_suffix(&[b'\r']) {
                            return str;
                        } else {
                            str
                        }
                    })
                    .filter(|x| !x.is_empty())
                    .for_each(|x| {
                        f(self.user, x.as_ptr(), x.len());
                    });
            }
            system_process_mode::String => {
                data.split(|x| *x == 0x0) // split by \0
                    .filter(|x| x.is_empty())
                    .for_each(|x| f(self.user, x.as_ptr(), x.len()));
            }
            system_process_mode::Fudi => {
                for x in FUDI_RE.captures_iter(data) {
                    if let Some(m) = x.get(1) {
                        f(self.user, m.as_bytes().as_ptr(), m.len());
                    }
                }
            }
        });
    }

    fn process_stdout(&self, data: &[u8]) {
        if self.capture_stdout {
            self.process_output(data, self.on_stdout_data);
        }
    }

    fn process_stderr(&self, data: &[u8]) {
        if self.capture_stderr {
            self.process_output(data, self.on_stderr_data);
        }
    }

    fn process_results(&self) -> (system_process_rc, Option<ExitStatus>) {
        if let Some(handle) = &self.handle {
            match handle.try_wait() {
                Ok(res) => {
                    match res {
                        Some(res) => {
                            self.process_stdout(res.stdout.as_slice());
                            self.process_stderr(res.stderr.as_slice());

                            return (system_process_rc::Ready, Some(res.status));
                        }
                        None => {
                            // still running
                            return (system_process_rc::Running, None);
                        }
                    }
                }
                Err(err) => {
                    self.on_err(err.to_string().as_str());
                    return (system_process_rc::Error, None);
                }
            }
        } else {
            self.on_err("no process handle");
            return (system_process_rc::Error, None);
        }
    }
}

impl Drop for system_process {
    fn drop(&mut self) {
        self.try_wait_or_kill()
    }
}

#[no_mangle]
pub extern "C" fn ceammc_system_process_new(
    cmd: *const system_process_cmd,
    cmd_len: usize,
    mode: system_process_mode,
    capture_stdout: bool,
    capture_stderr: bool,
    pwd: *const c_char,
    stdin: *const c_char,
    user: *mut c_void,
    on_err: Option<extern "C" fn(user: *mut c_void, msg: *const c_char)>,
    on_stdout_data: Option<extern "C" fn(user: *mut c_void, data: *const u8, len: usize)>,
    on_stderr_data: Option<extern "C" fn(user: *mut c_void, data: *const u8, len: usize)>,
) -> *mut system_process {
    if cmd.is_null() {
        on_err.map(|f| {
            f(user, b"NULL program pointer\0".as_ptr() as *const i8);
        });
        return null_mut();
    }

    let cmd = unsafe { &*slice_from_raw_parts(cmd, cmd_len) };
    if cmd.is_empty() {
        on_err.map(|f| {
            f(user, b"empty command\0".as_ptr() as *const i8);
        });
        return null_mut();
    }

    let mut expr = cmd[0].to_cmd();
    if !pwd.is_null() {
        let pwd = unsafe { CStr::from_ptr(pwd) }.to_string_lossy();
        if !pwd.is_empty() {
            expr = expr.dir(pwd.to_string());
        }
    }

    if !stdin.is_null() {
        let stdin = unsafe { CStr::from_ptr(stdin) }.to_string_lossy();
        if !stdin.is_empty() {
            expr = expr.stdin_bytes(stdin.as_bytes());
        }
    }

    for x in cmd[1..].iter() {
        expr = expr.pipe(x.to_cmd());
    }

    Box::into_raw(Box::new(system_process {
        cmd: Some(expr),
        handle: None,
        user,
        on_err,
        mode,
        capture_stdout,
        capture_stderr,
        on_stdout_data,
        on_stderr_data,
    }))
}

#[no_mangle]
pub extern "C" fn ceammc_system_process_free(proc: *mut system_process) {
    if !proc.is_null() {
        drop(unsafe { Box::from_raw(proc) })
    }
}

#[no_mangle]
pub extern "C" fn ceammc_system_process_clear(proc: *mut system_process) -> bool {
    if proc.is_null() {
        false
    } else {
        let prop = unsafe { &mut *proc };
        prop.try_wait_or_kill();
        prop.cmd = None;
        true
    }
}

#[no_mangle]
pub extern "C" fn ceammc_system_process_exec(proc: *mut system_process) -> bool {
    if proc.is_null() {
        eprintln!("NULL pointer");
        false
    } else {
        let prop = unsafe { &mut *proc };
        return prop.exec();
    }
}

#[no_mangle]
pub extern "C" fn ceammc_system_process_results(
    proc: *mut system_process,
    result_code: *mut i32,
) -> system_process_rc {
    if proc.is_null() {
        system_process_rc::Error
    } else {
        let prop = unsafe { &mut *proc };
        let (rc, status) = prop.process_results();

        if let Some(c) = status {
            if !result_code.is_null() {
                let rc = unsafe { &mut *result_code };
                c.code().map(|x| *rc = x);
            }
        }

        return rc;
    }
}

#[no_mangle]
pub extern "C" fn ceammc_system_process_terminate(proc: *mut system_process) -> bool {
    if proc.is_null() {
        false
    } else {
        let prop = unsafe { &mut *proc };
        prop.kill()
    }
}
