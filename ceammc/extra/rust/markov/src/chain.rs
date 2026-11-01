use num_enum::TryFromPrimitive;
use rand::distr::weighted::WeightedIndex;
use rand::prelude::*;
use std::collections::HashMap;

use crate::{
    errors::markov_error::{self, InvalidIndex, Sampling},
    markov::{
        markov_duration, markov_note,
        markov_order::{self, Order1, Order2},
    },
};

const MAX_DUR: usize = crate::markov::markov_duration::COUNT as usize;
const MAX_PITCH: usize = 127;

/// Index of the note in a flat array.
pub fn note_to_index(note: markov_note) -> usize {
    let pitch = note.pitch;
    let dur_bin: u8 = note.dur_bin.into();
    (pitch as usize) * MAX_DUR + dur_bin as usize
}

/// Decode an index back into a note.
pub fn index_to_note(idx: usize) -> Result<markov_note, markov_error> {
    let pitch = idx / MAX_DUR;
    if pitch > MAX_PITCH {
        return Err(markov_error::InvalidPitch { pitch });
    }

    let dur = idx % MAX_DUR;
    let dur_bin = markov_duration::try_from_primitive(dur as u8).map_err(|err| InvalidIndex {
        index: err.number.into(),
    })?;

    Ok(markov_note {
        pitch: pitch as u8,
        dur_bin,
    })
}

/// Key type for the transition table.
/// - order 0: empty key (global distribution)
/// - order 1: one previous state
/// - order 2: two previous states, packed into u64 (each state fits in 16 bits
///   for realistic DUR_BINS*128 < 65536; use u128 if that assumption breaks)
type HistoryKey = u64;

/// Pack up to two 16-bit state indices into a u64.
/// `len` determines how many slots are meaningful (0, 1, or 2).
fn pack_history(history: &[usize], order: markov_order) -> HistoryKey {
    debug_assert!(history.len() >= order.into_usize());
    let mut key: u64 = 0;
    // Oldest first, so that order 1 and order 2 keys differ naturally.
    for &s in &history[history.len() - (order.into_usize())..] {
        debug_assert!(s < (1 << 16), "state index too large for u64 packing");
        key = (key << 16) | (s as u64 & 0xFFFF);
    }
    key
}

pub struct MarkovChain {
    transitions: HashMap<HistoryKey, Vec<(usize, u32)>>,
    /// Full history of visited states, most recent last. Length <= order.
    history: Vec<usize>,
    order: markov_order,
    rng: SmallRng,
}

impl MarkovChain {
    /// Create a chain with the given order  and seed.
    pub fn new(seed: u64, order: markov_order) -> Self {
        Self {
            transitions: HashMap::new(),
            history: Vec::with_capacity(order.into_usize()),
            order,
            rng: SmallRng::seed_from_u64(seed),
        }
    }

    pub fn train(&mut self, note: markov_note) {
        let idx = note_to_index(note);

        // Build key from the last `order` states; only update if history is full enough.
        if self.history.len() >= self.order.into_usize() {
            let key = pack_history(&self.history, self.order);
            let edges = self.transitions.entry(key).or_default();
            match edges.iter_mut().find(|(to, _)| *to == idx) {
                Some((_, count)) => *count += 1,
                None => edges.push((idx, 1)),
            }
        }

        // Push new state and trim history to order.
        self.history.push(idx);
        if self.history.len() > self.order.into_usize() {
            self.history.remove(0);
        }
    }

    pub fn next(&mut self) -> Result<markov_note, markov_error> {
        if self.history.len() < self.order.into_usize() {
            // Not enough context yet to make a prediction.
            return Err(markov_error::NoCurrentState);
        }

        let key = pack_history(&self.history, self.order);
        let edges = self
            .transitions
            .get(&key)
            .ok_or(markov_error::NoTransitions { state: key })?;

        if edges.is_empty() {
            return Err(markov_error::NoTransitions { state: key });
        }

        let weights: Vec<u32> = edges.iter().map(|&(_, c)| c).collect();
        let dist = WeightedIndex::new(&weights).map_err(|_| Sampling)?;
        let chosen = edges[dist.sample(&mut self.rng)].0;

        self.history.push(chosen);
        if self.history.len() > self.order.into_usize() {
            self.history.remove(0);
        }

        index_to_note(chosen)
    }

    pub fn clear(&mut self) {
        self.transitions.clear();
        self.history.clear();
    }

    pub fn reset(&mut self) {
        self.history.clear();
    }

    pub fn set_history1(&mut self, note: markov_note) -> Result<(), markov_error> {
        if self.order != Order1 {
            return Err(markov_error::UnexpectedOrder {
                order: self.order.into_usize(),
                expected: Order1.into_usize(),
            });
        }
        self.history.clear();
        self.history.push(note_to_index(note));
        Ok(())
    }

    pub fn set_history2(
        &mut self,
        note1: markov_note,
        note2: markov_note,
    ) -> Result<(), markov_error> {
        if self.order != Order2 {
            return Err(markov_error::UnexpectedOrder {
                order: self.order.into_usize(),
                expected: Order2.into_usize(),
            });
        }
        self.history.clear();
        self.history.push(note_to_index(note1));
        self.history.push(note_to_index(note2));
        Ok(())
    }
}

#[cfg(test)]
mod test {
    use num_enum::TryFromPrimitive;

    use crate::{
        chain::{index_to_note, note_to_index, MarkovChain, MAX_DUR, MAX_PITCH},
        errors::markov_error::{NoCurrentState, NoTransitions},
        markov::{markov_duration, markov_note, markov_order},
    };

    fn note(pitch: u8, dur: u8) -> markov_note {
        markov_note {
            pitch,
            dur_bin: markov_duration::try_from_primitive(dur).unwrap(),
        }
    }

    #[test]
    fn index_roundtrip() {
        let mut idx = 0;
        for pitch in 0..=(MAX_PITCH as u8) {
            for dur in 0..MAX_DUR as u8 {
                let dur = crate::markov::markov_duration::try_from_primitive(dur);
                assert!(dur.is_ok());

                let note = markov_note {
                    pitch,
                    dur_bin: dur.unwrap(),
                };
                assert_eq!(note_to_index(note.clone()), idx);
                assert_eq!(index_to_note(idx), Ok(note));
                idx += 1;
            }
        }
    }

    #[test]
    fn order_0_uses_global_distribution() {
        let mut c = MarkovChain::new(42, markov_order::Order0);
        c.train(note(60, 0));
        c.train(note(62, 0));
        c.train(note(60, 0));

        // Order 0: no history requirement, key is always 0.
        for _ in 0..20 {
            let n = c.next().unwrap();
            // Only notes seen during training should be generated.
            assert!(n.pitch == 60 || n.pitch == 62);
        }
    }

    #[test]
    fn order_1_needs_one_previous() {
        let mut c = MarkovChain::new(42, markov_order::Order1);
        assert_eq!(c.next(), Err(NoCurrentState)); // no history yet

        c.train(note(60, 0));
        assert_eq!(
            c.next(),
            Err(NoTransitions {
                state: note_to_index(note(60, 0)) as u64
            })
        ); // still no transition (only one note seen)

        c.train(note(62, 0));
        c.set_history1(note(60, 0)).unwrap();
        assert_eq!(c.next(), Ok(note(62, 0)));
    }

    #[test]
    fn clear_and_reset() {
        let mut c = MarkovChain::new(42, markov_order::Order1);
        c.train(note(60, 0));
        c.train(note(62, 0));
        c.reset();
        assert!(c.next().is_err());
        c.train(note(60, 0));
        c.train(note(62, 0));
        c.clear();
        assert!(c.next().is_err());
    }

    #[test]
    fn order_2_needs_two_previous() {
        let mut c = MarkovChain::new(42, markov_order::Order2);
        c.train(note(60, 0));
        assert!(c.next().is_err());
        c.train(note(62, 0));
        assert!(c.next().is_err()); // still not enough for order 2
        c.train(note(64, 0));

        c.set_history2(note(60, 0), note(62, 0)).unwrap();
        // Now transition (60, 62) -> 64 exists.
        let n = c.next().unwrap();
        assert_eq!(n.pitch, 64);
    }
}
