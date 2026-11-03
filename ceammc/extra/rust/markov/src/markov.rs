use std::ffi::c_void;

use ceammc_rs_msg::{msg_cb, msg_notify, TokioClient, WorkerMessage};
use num_enum::{IntoPrimitive, TryFromPrimitive};
use serde::{Deserialize, Serialize};

use crate::{
    chain::{markov_sample_config, MarkovChain},
    errors::markov_error,
    markov::markov_order::{Order0, Order1, Order2},
    pitch_quantize::{out_of_range, OctaveRange},
};

/// Quantized note duration used as the second component of a Markov state.
///
/// Each variant corresponds to a musical duration (whole, half, quarter, ...,
/// optionally dotted). The numeric discriminants are part of the ABI: they are
/// used as `dur_bin` in [`markov_note`] and must not be renumbered without
/// updating `COUNT`.
#[repr(u8)]
#[derive(Debug, Clone, PartialEq, IntoPrimitive, TryFromPrimitive)]
#[allow(non_camel_case_types)]
pub enum markov_duration {
    /// Whole note.
    D1 = 0,
    /// Dotted whole note.
    D1dotted = 1,
    /// Double-dotted whole note.
    D1dotted2 = 2,
    /// Half note.
    D2 = 3,
    /// Dotted half note.
    D2dotted = 4,
    /// Double-dotted half note.
    D2dotted2 = 5,
    /// Quarter note.
    D4 = 6,
    /// Dotted quarter note.
    D4dotted = 7,
    /// Double-dotted quarter note.
    D4dotted2 = 8,
    /// Eighth note.
    D8 = 9,
    /// Dotted eighth note.
    D8dotted = 10,
    /// Double-dotted eighth note.
    D8dotted2 = 11,
    /// Sixteenth note.
    D16 = 12,
    /// Dotted sixteenth note.
    D16dotted = 13,
    /// Double-dotted sixteenth note.
    D16dotted2 = 14,
    /// Thirty-second note.
    D32 = 15,
    /// Double-dotted thirty-second note.
    D32dotted2 = 16,
    /// Sixty-fourth note.
    D64 = 17,
    /// Number of duration variants. Must be updated when a new variant is added.
    COUNT = 18,
}

/// Markov chain order (memory depth), as exposed over FFI.
///
/// - `Order0`: next note is independent of history (global distribution).
/// - `Order1`: next note depends on the previous note.
/// - `Order2`: next note depends on the two previous notes.
#[repr(u8)]
#[derive(Clone, Copy, PartialEq, Serialize, Deserialize)]
#[allow(non_camel_case_types)]
pub enum markov_order {
    /// No history; sample from the global distribution.
    Order0,
    /// One previous note of context.
    Order1,
    /// Two previous notes of context.
    Order2,
}

impl markov_order {
    /// Convert to a `usize` for use as a chain parameter.
    pub fn into_usize(&self) -> usize {
        self.clone() as usize
    }
}

impl Default for markov_order {
    fn default() -> Self {
        Self::Order1
    }
}

impl TryFrom<u8> for markov_order {
    type Error = markov_error;

    fn try_from(value: u8) -> Result<Self, Self::Error> {
        match value {
            0 => Ok(Order0),
            1 => Ok(Order1),
            2 => Ok(Order2),
            _ => Err(markov_error::InvalidOrder {
                order: value.into(),
            }),
        }
    }
}

/// A single Markov state: a MIDI pitch plus a quantized duration.
#[repr(C)]
#[derive(Debug, Clone, PartialEq)]
pub struct markov_note {
    /// MIDI pitch, 0..=127.
    pub pitch: u8,
    /// Quantized duration bin.
    pub dur_bin: markov_duration,
}

/// Commands sent from the control thread to the worker thread.
#[derive(Clone)]
enum Request {
    /// Append a note to the training data.
    Train(markov_note),
    /// Set the one-note history (for `Order1` chains).
    SetHistory1(markov_note),
    /// Set the two-note history (for `Order2` chains).
    SetHistory2(markov_note, markov_note),
    /// Generate the next note.
    Bang,
    /// Generate the next note with config
    BangWithConfig(markov_sample_config),
    /// Clear all training data and history.
    Clear,
    /// Reset history but keep training data.
    Reset,
    /// Set octave range
    SetOctaveRange { min: i8, max: i8 },
    /// Set out of range mode
    SetOutOfRangeMode { mode: out_of_range },
    /// Set random seed
    SetSeed(u32),
}

/// Replies sent from the worker thread back to the control thread.
#[derive(Clone)]
enum Reply {
    /// A generated note.
    Note(markov_note),
}

/// Opaque handle to a Markov chain generator, owned by the C side.
///
/// The C side holds a `*mut markov` and must call [`ceammc_markov_free`]
/// when done.
#[allow(non_camel_case_types)]
pub struct markov {
    /// Callback invoked for each generated note
    on_data: markov_cb,
    /// Callback invoked for log/error messages
    on_msg: msg_cb,
    /// Worker-thread client used to send requests and receive replies.
    obj: ceammc_rs_msg::TokioClient<Request, Reply>,
}

impl markov {
    fn new(
        order: markov_order,
        on_data: markov_cb,
        on_msg: msg_cb,
        notify: msg_notify,
        seed: u32,
    ) -> Self {
        Self {
            on_data,
            on_msg,
            obj: TokioClient::start_worker(
                ceammc_rs_msg::NumThreads::Auto,
                ceammc_rs_msg::TokioRtShutdown::Wait,
                16,
                async move |mut channel, _cancel| {
                    let mut markov_gen = MarkovChain::new(seed.into(), order);
                    let out_chan = channel.clone_sender();

                    channel
                        .recv_loop(&mut async |req| -> Result<(), String> {
                            match req {
                                Request::Train(markov_note) => {
                                    if let Err(err) = markov_gen.train(markov_note) {
                                        out_chan
                                            .send_msg(WorkerMessage::error(err.to_string()))
                                            .to_worker_result()?
                                    }
                                    Ok(())
                                }
                                Request::Bang => {
                                    match markov_gen.next() {
                                        Ok(note) => {
                                            out_chan
                                                .send_data(Reply::Note(note))
                                                .to_worker_result()?;
                                            notify.exec();
                                        }
                                        Err(err) => out_chan
                                            .send_msg(WorkerMessage::error(err.to_string()))
                                            .to_worker_result()?,
                                    }
                                    Ok(())
                                }
                                Request::Clear => {
                                    markov_gen.clear();
                                    Ok(())
                                }
                                Request::Reset => {
                                    markov_gen.reset();
                                    Ok(())
                                }
                                Request::SetHistory1(note) => {
                                    if let Err(err) = markov_gen.set_history1(note) {
                                        out_chan
                                            .send_msg(WorkerMessage::error(err.to_string()))
                                            .to_worker_result()?;
                                    }
                                    Ok(())
                                }
                                Request::SetHistory2(note1, note2) => {
                                    if let Err(err) = markov_gen.set_history2(note1, note2) {
                                        out_chan
                                            .send_msg(WorkerMessage::error(err.to_string()))
                                            .to_worker_result()?;
                                    }
                                    Ok(())
                                }
                                Request::BangWithConfig(cfg) => {
                                    match markov_gen.next_with_temperature(cfg) {
                                        Ok(note) => {
                                            out_chan
                                                .send_data(Reply::Note(note))
                                                .to_worker_result()?;
                                            notify.exec();
                                        }
                                        Err(err) => out_chan
                                            .send_msg(WorkerMessage::error(err.to_string()))
                                            .to_worker_result()?,
                                    }
                                    Ok(())
                                }
                                Request::SetOctaveRange { min, max } => {
                                    match OctaveRange::new(min, max) {
                                        Ok(rng) => markov_gen.set_octave(rng),
                                        Err(err) => out_chan
                                            .send_msg(WorkerMessage::error(err.to_string()))
                                            .to_worker_result()?,
                                    }

                                    Ok(())
                                }
                                Request::SetOutOfRangeMode { mode } => {
                                    markov_gen.set_out_of_range_mode(mode);
                                    Ok(())
                                }
                                Request::SetSeed(seed) => {
                                    markov_gen.set_seed(seed.into());
                                    Ok(())
                                }
                            }
                        })
                        .await?;

                    Ok(())
                },
                notify,
                on_msg,
            ),
        }
    }

    fn send_request(m: *mut Self, req: Request) -> bool {
        if m.is_null() {
            false
        } else {
            let m = unsafe { &*m };
            m.obj.send_checked(req, &m.on_msg)
        }
    }
}

/// C-compatible callback invoked when a note is generated.
///
/// The `user` pointer is passed through unchanged; it is owned by the C side.
#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Clone, Copy)]
pub struct markov_cb {
    /// Opaque user data, forwarded unchanged to `cb`.
    user: *mut c_void,
    /// Callback function pointer; `None` means "do nothing".
    cb: Option<extern "C" fn(user: *mut c_void, note: markov_note)>,
}

impl markov_cb {
    /// Invoke the callback, if any.
    pub fn exec(&self, note: markov_note) {
        self.cb.map(|cb| cb(self.user, note));
    }
}

/// Create a new Markov chain generator.
///
/// # Parameters
/// - `order`: chain order (`Order0`, `Order1`, or `Order2`).
/// - `seed`: RNG seed; same seed + same training data yields the same output.
/// - `notify`: notification used by the worker to wake the control thread.
/// - `on_msg`: callback for log/error messages
/// - `on_data`: callback invoked for every generated note.
///
/// # Returns
/// An owned `*mut markov` handle, or a null pointer is never returned for
/// valid arguments. The caller must eventually call [`ceammc_markov_free`].
#[no_mangle]
pub extern "C" fn ceammc_markov_new(
    order: markov_order,
    seed: u32,
    notify: msg_notify,
    on_msg: msg_cb,
    on_data: markov_cb,
) -> *mut markov {
    Box::into_raw(Box::new(markov::new(order, on_data, on_msg, notify, seed)))
}

/// Free a Markov chain generator previously created by [`ceammc_markov_new`].
///
/// # Parameters
/// - `m`: handle returned by [`ceammc_markov_new`]. A null pointer is ignored.
///
/// # Safety
/// `m` must be a pointer previously returned by [`ceammc_markov_new`] and not
/// already freed.
#[no_mangle]
pub extern "C" fn ceammc_markov_free(m: *mut markov) {
    if !m.is_null() {
        let _ = unsafe { Box::from_raw(m) };
    }
}

/// Drain pending replies from the worker thread and dispatch them to `on_data`.
///
/// Call this periodically from the control thread (e.g. from Pd's clock).
///
/// # Parameters
/// - `m`: generator handle.
///
/// # Returns
/// `true` on success, `false` if `m` is null.
///
/// # Safety
/// `m` must be a valid handle from [`ceammc_markov_new`].
#[no_mangle]
pub extern "C" fn ceammc_markov_process(m: *mut markov) -> bool {
    if m.is_null() {
        false
    } else {
        let m = unsafe { &mut *m };
        m.obj.recv_loop(|reply| match reply {
            Reply::Note(note) => m.on_data.exec(note),
        });
        true
    }
}

/// Request generation of the next note.
///
/// The generated note is delivered asynchronously via the `on_data` callback
/// after a subsequent call to [`ceammc_markov_process`].
///
/// # Parameters
/// - `m`: generator handle.
///
/// # Returns
/// `true` if the request was enqueued, `false` if `m` is null or the worker
/// channel is full.
///
/// # Safety
/// `m` must be a valid handle from [`ceammc_markov_new`].
#[no_mangle]
pub extern "C" fn ceammc_markov_bang(m: *mut markov) -> bool {
    markov::send_request(m, Request::Bang)
}

#[no_mangle]
pub extern "C" fn ceammc_markov_bang_with_config(
    m: *mut markov,
    cfg: markov_sample_config,
) -> bool {
    markov::send_request(m, Request::BangWithConfig(cfg))
}

/// Reset the current chain state (history) without discarding training data.
///
/// # Parameters
/// - `m`: generator handle.
///
/// # Returns
/// `true` if the request was enqueued, `false` if `m` is null or the worker
/// channel is full.
///
/// # Safety
/// `m` must be a valid handle from [`ceammc_markov_new`].
#[no_mangle]
pub extern "C" fn ceammc_markov_reset(m: *mut markov) -> bool {
    markov::send_request(m, Request::Reset)
}

/// Clear all training data and reset the chain state.
///
/// # Parameters
/// - `m`: generator handle.
///
/// # Returns
/// `true` if the request was enqueued, `false` if `m` is null or the worker
/// channel is full.
///
/// # Safety
/// `m` must be a valid handle from [`ceammc_markov_new`].
#[no_mangle]
pub extern "C" fn ceammc_markov_clear(m: *mut markov) -> bool {
    markov::send_request(m, Request::Clear)
}

/// Append a note to the training data.
///
/// # Parameters
/// - `m`: generator handle.
/// - `note`: pitch + duration to train on.
///
/// # Returns
/// `true` if the request was enqueued, `false` if `m` is null or the worker
/// channel is full.
///
/// # Safety
/// `m` must be a valid handle from [`ceammc_markov_new`].
#[no_mangle]
pub extern "C" fn ceammc_markov_train(m: *mut markov, note: markov_note) -> bool {
    markov::send_request(m, Request::Train(note))
}

/// Set the one-note history (for `Order1` chains).
///
/// Used to start generation from a specific note instead of relying on the
/// history accumulated during training.
///
/// # Parameters
/// - `m`: generator handle.
/// - `note`: the previous note to use as context.
///
/// # Returns
/// `true` if the request was enqueued, `false` if `m` is null or the worker
/// channel is full. If the chain order is not `Order1`, the worker reports an
/// error via `on_msg`.
///
/// # Safety
/// `m` must be a valid handle from [`ceammc_markov_new`].
#[no_mangle]
pub extern "C" fn ceammc_markov_set_history1(m: *mut markov, note: markov_note) -> bool {
    markov::send_request(m, Request::SetHistory1(note))
}

/// Set the two-note history (for `Order2` chains).
///
/// `note1` is the older note, `note2` is the more recent one; generation will
/// continue from this context.
///
/// # Parameters
/// - `m`: generator handle.
/// - `note1`: the older note of the two-note context.
/// - `note2`: the more recent note of the two-note context.
///
/// # Returns
/// `true` if the request was enqueued, `false` if `m` is null or the worker
/// channel is full. If the chain order is not `Order2`, the worker reports an
/// error via `on_msg`.
///
/// # Safety
/// `m` must be a valid handle from [`ceammc_markov_new`].
#[no_mangle]
pub extern "C" fn ceammc_markov_set_history2(
    m: *mut markov,
    note1: markov_note,
    note2: markov_note,
) -> bool {
    markov::send_request(m, Request::SetHistory2(note1, note2))
}

/// Set the MIDI octave range used to quantize incoming and outgoing pitches.
///
/// Pitches outside `[min, max]` are handled according to the out-of-range
/// mode (see [`ceammc_markov_set_out_of_range_mode`]). The range is applied to
/// both training and generation: `train` quantizes incoming pitches, and `bang`
/// produces pitches inside the range.
///
/// Octave numbering follows scientific pitch notation: octave `n` starts at
/// MIDI `12 * (n + 1)`, so C4 = 60, C3 = 48, C5 = 72.
///
/// # Parameters
/// - `m`: generator handle.
/// - `min`: lowest octave (inclusive).
/// - `max`: highest octave (inclusive). Must be `>= min`, and the resulting
///   MIDI span must intersect `0..=127`.
///
/// # Returns
/// `true` if the request was enqueued, `false` if `m` is null or the worker
/// channel is full. If the range is invalid, the worker reports an error via
/// `on_msg`.
///
/// # Safety
/// `m` must be a valid handle from [`ceammc_markov_new`].
#[no_mangle]
pub extern "C" fn ceammc_markov_set_octave_range(m: *mut markov, min: i8, max: i8) -> bool {
    markov::send_request(m, Request::SetOctaveRange { min, max })
}

/// Set the strategy for pitches that fall outside the configured octave range.
///
/// The mode applies whenever a pitch is quantized (during `train`) or generated
/// (during `bang`). See [`out_of_range`] for the available strategies.
///
/// # Parameters
/// - `m`: generator handle.
/// - `mode`: out-of-range handling strategy.
///
/// # Returns
/// `true` if the request was enqueued, `false` if `m` is null or the worker
/// channel is full.
///
/// # Safety
/// `m` must be a valid handle from [`ceammc_markov_new`].
#[no_mangle]
pub extern "C" fn ceammc_markov_set_out_of_range_mode(m: *mut markov, mode: out_of_range) -> bool {
    markov::send_request(m, Request::SetOutOfRangeMode { mode })
}

/// Re-seed the random number generator.
///
/// Same seed + same training data yields the same generated sequence, which is
/// useful for reproducible tests and for restoring a saved chain. Re-seeding
/// does **not** reset the training data or the current history; it only affects
/// future sampling.
///
/// # Parameters
/// - `m`: generator handle.
/// - `seed`: new RNG seed.
///
/// # Returns
/// `true` if the request was enqueued, `false` if `m` is null or the worker
/// channel is full.
///
/// # Safety
/// `m` must be a valid handle from [`ceammc_markov_new`].
#[no_mangle]
pub extern "C" fn ceammc_markov_set_seed(m: *mut markov, seed: u32) -> bool {
    markov::send_request(m, Request::SetSeed(seed))
}
