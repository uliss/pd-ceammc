use ceammc_rs_msg::{msg_cb, msg_notify, TokioClient, WorkerMessage};
use num_enum::{IntoPrimitive, TryFromPrimitive};

use crate::{
    chain::MarkovChain,
    errors::markov_error,
    markov::markov_order::{Order0, Order1, Order2},
};

#[repr(u8)]
#[derive(Debug, Clone, PartialEq, IntoPrimitive, TryFromPrimitive)]
#[allow(non_camel_case_types)]
pub enum markov_duration {
    D1 = 0,
    D1dotted = 1,
    D1dotted2 = 2,
    D2 = 3,
    D2dotted = 4,
    D2dotted2 = 5,
    D4 = 6,
    D4dotted = 7,
    D4dotted2 = 8,
    D8 = 9,
    D8dotted = 10,
    D8dotted2 = 11,
    D16 = 12,
    D16dotted = 13,
    D16dotted2 = 14,
    D32 = 15,
    D32dotted2 = 16,
    D64 = 17,
    COUNT = 18, // shoud be updated
}

#[repr(u8)]
#[derive(Clone, Copy, PartialEq)]
#[allow(non_camel_case_types)]
pub enum markov_order {
    Order0,
    Order1,
    Order2,
}

impl markov_order {
    pub fn into_usize(&self) -> usize {
        self.clone() as usize
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

#[repr(C)]
#[derive(Debug, Clone, PartialEq)]
pub struct markov_note {
    pub pitch: u8,
    pub dur_bin: markov_duration,
}

/// Команды от control-потока к audio/worker-потоку
#[derive(Clone)]
enum Request {
    Train(markov_note),
    SetHistory1(markov_note),
    SetHistory2(markov_note, markov_note),
    Bang,
    Clear,
    Reset,
}

#[derive(Clone)]
enum Reply {
    Note(markov_note),
}

#[allow(non_camel_case_types)]
pub struct markov {
    on_data: markov_cb,
    on_msg: msg_cb,
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
                                    markov_gen.train(markov_note);
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

#[allow(non_camel_case_types)]
pub type markov_cb = extern "C" fn(note: markov_note);

#[allow(non_camel_case_types)]
pub type markov_err_cb = extern "C" fn(err: markov_error);

#[no_mangle]
/// create markov chain generator
pub extern "C" fn ceammc_markov_new(
    order: u8,
    seed: u32,
    notify: msg_notify,
    on_msg: msg_cb,
    on_data: markov_cb,
    on_err: markov_err_cb,
) -> *mut markov {
    let order = order.try_into();
    match order {
        Ok(order) => Box::into_raw(Box::new(markov::new(order, on_data, on_msg, notify, seed))),
        Err(err) => {
            on_err(err);
            std::ptr::null_mut()
        }
    }
}

#[no_mangle]
/// free markov chain generator
pub extern "C" fn ceammc_markov_free(m: *mut markov) {
    if !m.is_null() {
        let _ = unsafe { Box::from_raw(m) };
    }
}

#[no_mangle]
/// process replies markov chain generator
pub extern "C" fn ceammc_esphome_client_process(m: *mut markov) -> bool {
    if m.is_null() {
        false
    } else {
        let m = unsafe { &mut *m };
        m.obj.recv_loop(|reply| match reply {
            Reply::Note(note) => (m.on_data)(note),
        });
        true
    }
}

#[no_mangle]
/// generate new note
pub extern "C" fn ceammc_markov_bang(m: *mut markov) -> bool {
    markov::send_request(m, Request::Bang)
}

#[no_mangle]
/// reset current state
pub extern "C" fn ceammc_markov_reset(m: *mut markov) -> bool {
    markov::send_request(m, Request::Reset)
}

#[no_mangle]
/// clear all training data
pub extern "C" fn ceammc_markov_clear(m: *mut markov) -> bool {
    markov::send_request(m, Request::Clear)
}

#[no_mangle]
/// add train data
pub extern "C" fn ceammc_markov_train(m: *mut markov, note: markov_note) -> bool {
    markov::send_request(m, Request::Train(note))
}

#[no_mangle]
/// set history for first order
pub extern "C" fn ceammc_markov_set_history1(m: *mut markov, note: markov_note) -> bool {
    markov::send_request(m, Request::SetHistory1(note))
}

#[no_mangle]
/// set history for second order
pub extern "C" fn ceammc_markov_set_history2(
    m: *mut markov,
    note1: markov_note,
    note2: markov_note,
) -> bool {
    markov::send_request(m, Request::SetHistory2(note1, note2))
}
