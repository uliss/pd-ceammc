#![allow(non_camel_case_types)]

use std::{
    ffi::{c_char, c_int, c_void, CStr, CString},
    num::NonZero,
    path::Path,
    time::Instant,
};

use audioadapter_buffers::direct::InterleavedSlice;
use ceammc_rs_msg::{cstr_to_string, msg_cb, msg_notify, Client, ClientChannelBounded};
use rubato::{Resampler, SincInterpolationParameters};
use whisper_rs::{FullParams, WhisperContext, WhisperContextParameters};

pub struct misc_whisper {
    obj: ceammc_rs_msg::Client<Request, Reply>,
    on_data: whisper_data_cb,
}

#[derive(Debug)]
pub enum Request {
    AppendBuf64([f32; 64]),
    AppendBuf(Vec<f32>),
    ClearBuf,
    SetLanguage(whisper_lang),
    SetNumThreads(u8),
    Recognize(u32),
}

pub enum Reply {
    Text(CString),
}

#[must_use]
#[derive(PartialEq)]
pub enum SendStatus {
    Ok,
    NoSpace,
    Disconnected,
}

#[repr(C)]
#[derive(Debug, PartialEq)]
pub enum whisper_lang {
    Russian,
    English,
    Auto,
}

#[repr(C)]
pub struct whisper_data_cb {
    user: *mut c_void,
    cb: extern "C" fn(user: *mut c_void, str: *const c_char),
}

impl whisper_data_cb {
    fn exec(&self, str: &CStr) {
        (self.cb)(self.user, str.as_ptr())
    }
}

#[no_mangle]
/// create whisper handle
/// @param model_path - full path to model
/// @param notify - nofity caller callback
/// @param on_msg - message from whisper
/// @param on_data - data from whisper
pub extern "C" fn ceammc_misc_whisper_create(
    model_path: *const c_char,
    notify: msg_notify,
    on_msg: msg_cb,
    on_data: whisper_data_cb,
) -> *mut misc_whisper {
    if model_path.is_null() {
        std::ptr::null_mut()
    } else {
        let path = cstr_to_string(model_path).unwrap_or_default();
        Box::into_raw(Box::new(misc_whisper::new(path, notify, on_msg, on_data)))
    }
}

#[no_mangle]
/// free whisper module
/// @param wh - whisper pointer (nullable)
pub extern "C" fn ceammc_misc_whisper_free(wh: *mut misc_whisper) {
    if !wh.is_null() {
        unsafe { Box::from_raw(wh) }.quit()
    }
}

#[no_mangle]
/// process all available data from whisper worker
/// @param wh - whisper pointer (nullable)
pub extern "C" fn ceammc_misc_whisper_process_reply(wh: *mut misc_whisper) -> bool {
    misc_whisper::process_reply_ptr(wh)
}

#[no_mangle]
/// append samples to whisper
/// @param wh - whisper handle (nullable)
/// @param data - pointer to data (nullable)
/// @param len - data length
pub extern "C" fn ceammc_misc_whisper_append_samples(
    wh: *mut misc_whisper,
    data: *const f32,
    len: usize,
) -> bool {
    if wh.is_null() || data.is_null() || len == 0 {
        return false;
    }

    let wh = unsafe { &*wh };
    let data = unsafe { std::slice::from_raw_parts(data, len) };
    let num64_blocks = len / 64;
    for i in 0..num64_blocks {
        if !wh.send(Request::AppendBuf64(*data[i..i + 64].as_array().unwrap())) {
            return false;
        }
    }

    let extra = len % 64;
    if extra > 0 {
        return wh.send(Request::AppendBuf(data[num64_blocks * 64..].to_vec()));
    }

    true
}

#[no_mangle]
/// clear whisper internal buffer
/// @param wh - whisper handle (nullable)
pub extern "C" fn ceammc_misc_whisper_clear(wh: *mut misc_whisper) -> bool {
    if wh.is_null() {
        return false;
    }

    let wh = unsafe { &*wh };
    wh.send(Request::ClearBuf)
}

#[no_mangle]
/// do whisper recognition
/// @param wh - whisper handle (nullable)
/// @param recorded samplerate
pub extern "C" fn ceammc_misc_whisper_recognize(wh: *mut misc_whisper, sr: u32) -> bool {
    if wh.is_null() {
        return false;
    }

    let wh = unsafe { &*wh };
    wh.send(Request::Recognize(sr))
}

#[no_mangle]
/// set whisper recognition language
/// @param wh - whisper handle (nullable)
/// @param lang - language
pub extern "C" fn ceammc_misc_whisper_set_language(
    wh: *mut misc_whisper,
    lang: whisper_lang,
) -> bool {
    if wh.is_null() {
        return false;
    }

    let wh = unsafe { &*wh };
    wh.send(Request::SetLanguage(lang))
}

#[no_mangle]
/// set numbers of processing threads
/// @param wh - whisper handle (nullable)
/// @param num - number of threads
pub extern "C" fn ceammc_misc_whisper_set_num_threads(wh: *mut misc_whisper, num: u8) -> bool {
    if wh.is_null() {
        return false;
    }

    let wh = unsafe { &*wh };
    wh.send(Request::SetNumThreads(num))
}

fn process(model: &Path, channel: &ClientChannelBounded<Request, Reply>) -> Result<(), String> {
    let mut time_start = Instant::now();

    let ctx = WhisperContext::new_with_params(model, WhisperContextParameters::default())
        .map_err(|err| err.to_string())?;

    let mut state = ctx.create_state().map_err(|err| err.to_string())?;

    // let mut params = FullParams::new(whisper_rs::SamplingStrategy::Greedy { best_of: 1 });
    let mut params = FullParams::new(whisper_rs::SamplingStrategy::BeamSearch {
        beam_size: 5,
        patience: -1.0,
    });
    params.set_n_threads(1);
    params.set_print_special(false);
    params.set_print_progress(false);
    params.set_single_segment(true);

    let mut samples = vec![];

    channel.send_debug(format!(
        "model size: {}MB",
        model.metadata().map_err(|e| e.to_string())?.len() / 1000000
    ))?;

    channel.send_debug(format!(
        "model load takes: {}ms",
        time_start.elapsed().as_millis()
    ))?;

    channel.recv(&mut |req| {
        match req {
            Request::AppendBuf64(data) => {
                samples.extend_from_slice(&data);
                Ok(())
            }
            Request::AppendBuf(mut data) => {
                samples.append(&mut data);
                Ok(())
            }
            Request::ClearBuf => {
                samples.clear();
                Ok(())
            }
            Request::Recognize(sr) => {
                channel.send_debug("recognize")?;
                time_start = Instant::now();

                let target_rate = 16000;
                let resample_ratio = target_rate as f64 / sr as f64;

                let resample_params = SincInterpolationParameters {
                    sinc_len: 256,
                    f_cutoff: 0.95,
                    oversampling_factor: 128,
                    interpolation: rubato::SincInterpolationType::Linear,
                    window: rubato::WindowFunction::BlackmanHarris2,
                };

                let mut resampler = rubato::Async::<f32>::new_sinc(
                    resample_ratio,
                    1.0,
                    &resample_params,
                    256,
                    1,
                    rubato::FixedAsync::Input,
                )
                .map_err(|err| err.to_string())?;

                // let max_output_frames = resampler.output_frames_max();
                let input = InterleavedSlice::new(&samples, 1, samples.len())
                    .map_err(|err| err.to_string())?;

                let output_len = resampler.process_all_needed_output_len(samples.len());
                let mut outdata = vec![0.0; output_len];
                let mut output = InterleavedSlice::new_mut(&mut outdata, 1, output_len).unwrap();

                println!("num input samples:  {}", samples.len());
                println!("num output samples: {output_len}");

                let (len_in, len_out) = resampler
                    .process_all_into_buffer(&input, &mut output, samples.len(), None)
                    .map_err(|err| err.to_string())?;

                println!("resampled: in: {len_in}, out: {len_out}");
                channel.send_debug(format!(
                    "resampled in {}ms",
                    time_start.elapsed().as_millis()
                ))?;

                time_start = Instant::now();

                state
                    .full(params.clone(), &outdata)
                    .map_err(|err| err.to_string())?;

                let mut result = String::new();
                for segment in state.as_iter() {
                    result.push_str(
                        &segment
                            .to_str_lossy()
                            .unwrap_or_default()
                            .replace("{", "") // remove unsupported Pd characters
                            .replace("}", ""),
                    );
                    result.push(' ');
                }

                log::info!("result: {result}");

                channel.send_debug(format!(
                    "recognized in {}ms",
                    time_start.elapsed().as_millis()
                ))?;
                let cstr = CString::new(result).unwrap_or_default();
                channel.send_data(Reply::Text(cstr)).to_worker_result()
            }
            Request::SetLanguage(whisper_lang) => {
                channel.send_debug(format!("set_language: {whisper_lang:?}"))?;
                params.set_language(match whisper_lang {
                    whisper_lang::Russian => Some("ru"),
                    whisper_lang::English => Some("en"),
                    whisper_lang::Auto => None,
                });
                Ok(())
            }
            Request::SetNumThreads(u8) => {
                let default =
                    std::thread::available_parallelism().unwrap_or(NonZero::new(1).unwrap());
                let num_threads = NonZero::<usize>::new(u8.into())
                    .unwrap_or(default)
                    .min(default)
                    .get() as c_int;

                channel.send_debug(format!("set_n_threads: {num_threads}"))?;
                params.set_n_threads(num_threads);
                Ok(())
            }
        }
    })
}

impl misc_whisper {
    fn new(path: String, notify: msg_notify, on_msg: msg_cb, on_data: whisper_data_cb) -> Self {
        let obj = Client::<Request, Reply>::start_worker(
            move |channel| -> Result<(), String> {
                let model = Path::new(&path);
                match process(&model, channel) {
                    Err(err) => log::error!("{err}"),
                    _ => {}
                }
                Ok(())
            },
            32,
            notify,
            on_msg,
        );

        Self { obj, on_data }
    }

    fn quit(self) {
        self.obj.stop_worker();
    }

    fn process_reply_ptr(wh: *mut misc_whisper) -> bool {
        if wh.is_null() {
            false
        } else {
            let wh = unsafe { &*wh };
            wh.obj.recv(|rep| match rep {
                Reply::Text(txt) => wh.on_data.exec(&txt),
            });

            true
        }
    }

    fn send(&self, req: Request) -> bool {
        self.obj.send(req)
    }
}
