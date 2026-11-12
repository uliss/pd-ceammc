use thiserror::Error;

/// Errors of the Markov chain core.
#[derive(Debug, Error, PartialEq)]
#[repr(C)]
pub enum markov_error {
    /// The chain has not seen any (pitch, dur) pair yet, so
    /// there is nothing for `next()` to sample from.
    #[error("no current state: chain has not been trained or was reset")]
    NoCurrentState,

    /// There is no outgoing transition from the current state.
    /// This is a normal situation for a sparse chain — the caller
    /// must decide what to do (fallback, stop, reset).
    #[error("no outgoing transitions from state {state}")]
    NoTransitions { state: u64 },

    /// Failed to build sampling weights (e.g. all zeros
    /// or `WeightedIndex` overflow).
    #[error("failed to build weighted distribution")]
    Sampling,

    /// Invalid input parameter: pitch outside 0..=127 or
    /// dur_bin outside 0..DUR_BINS.
    #[error("invalid pitch {pitch}: must be in 0..=127")]
    InvalidPitch { pitch: usize },

    #[error("invalid index {index}")]
    InvalidIndex { index: usize },

    #[error("invalid order: {order}")]
    InvalidOrder { order: usize },

    #[error("unexpected order, got: {order}, expected: {expected}")]
    UnexpectedOrder { order: usize, expected: usize },

    #[error("serialization error: {msg}")]
    SerializationError { msg: String },

    #[error("deserialization error: {msg}")]
    DeserializationError { msg: String },

    #[error("error '{msg}' while writing to file: '{path}'")]
    FSWriteError { path: String, msg: String },

    #[error("error '{msg}' while reading from file: '{path}'")]
    FSReadError { path: String, msg: String },
}
