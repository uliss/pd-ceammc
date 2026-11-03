use num_enum::TryFromPrimitive;
use rand::distr::weighted::WeightedIndex;
use rand::prelude::*;
use serde::{Deserialize, Serialize};
use std::collections::HashMap;

use crate::{
    errors::markov_error::{self, InvalidIndex, Sampling},
    markov::{
        markov_duration, markov_note,
        markov_order::{self, Order1, Order2},
    },
    pitch_quantize::{out_of_range, OctaveRange},
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

/// Sampling parameters. All fields are cheap `Copy` types,
/// so the struct can be stored directly in the Pd object.
#[derive(Debug, Clone, Copy)]
#[repr(C)]
#[allow(non_camel_case_types)]
pub struct markov_sample_config {
    /// Keep only the `top_k` most frequent outgoing transitions before sampling.
    /// 0 = disabled (use all edges).
    pub top_k: usize,
    /// Temperature for softmax-style smoothing of transition counts.
    /// 1.0 = raw counts, >1.0 = flatter (more surprise), <1.0 = sharper.
    pub temperature: f32,
    /// If no outgoing transition exists for the full context, fall back to
    /// shorter contexts (order-1, order-2, ..., 0). Requires `train` to record
    /// transitions at every level.
    pub backoff: bool,
    /// If even the order-0 distribution is empty, pick a random observed state.
    /// If `false`, `next_with_temperature` returns `NoTransitions`.
    pub restart: bool,
}

impl Default for markov_sample_config {
    fn default() -> Self {
        Self {
            temperature: 1.0,
            top_k: 0,
            backoff: true,
            restart: true,
        }
    }
}

fn default_rng() -> SmallRng {
    SmallRng::seed_from_u64(0)
}

#[derive(Serialize, Deserialize)]
pub struct MarkovChain {
    transitions: HashMap<HistoryKey, Vec<(usize, u32)>>,
    /// Full history of visited states, most recent last. Length <= order.
    history: Vec<usize>,
    order: markov_order,
    #[serde(skip, default = "default_rng")]
    rng: SmallRng,
    octave_range: OctaveRange,
    out_of_range_mode: out_of_range,
}

impl MarkovChain {
    /// Create a chain with the given order  and seed.
    pub fn new(seed: u64, order: markov_order) -> Self {
        Self {
            transitions: HashMap::new(),
            history: Vec::with_capacity(order.into_usize()),
            order,
            rng: SmallRng::seed_from_u64(seed),
            octave_range: Default::default(),
            out_of_range_mode: Default::default(),
        }
    }

    pub fn set_seed(&mut self, seed: u64) {
        self.rng = SmallRng::seed_from_u64(seed)
    }

    pub fn set_octave(&mut self, range: OctaveRange) {
        self.octave_range = range
    }

    pub fn set_out_of_range_mode(&mut self, mode: out_of_range) {
        self.out_of_range_mode = mode
    }

    pub fn train(&mut self, note: markov_note) -> Result<(), markov_error> {
        let note = self.octave_range.quantize(note, self.out_of_range_mode)?;
        let idx = note_to_index(note);
        let max_ctx = self.order.into_usize().min(self.history.len());
        for ctx_len in 0..=max_ctx {
            let key = pack_history(
                &self.history,
                markov_order::try_from(ctx_len as u8).unwrap_or_default(),
            );
            let edges = self.transitions.entry(key).or_default();
            match edges.iter_mut().find(|(to, _)| *to == idx) {
                Some((_, count)) => *count += 1,
                None => edges.push((idx, 1)),
            }
        }
        self.history.push(idx);
        if self.history.len() > self.order.into_usize() {
            self.history.remove(0);
        }

        Ok(())
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

    pub fn save(&self, path: &str) -> Result<(), markov_error> {
        let bytes =
            postcard::to_allocvec(&self).map_err(|err| markov_error::SerializationError {
                msg: err.to_string(),
            })?;

        std::fs::write(path, &bytes).map_err(|err| markov_error::FSWriteError {
            path: path.into(),
            msg: err.to_string(),
        })?;

        Ok(())
    }

    pub fn load(&mut self, path: &str) -> Result<(), markov_error> {
        let bytes = std::fs::read(path).map_err(|err| markov_error::FSReadError {
            path: path.into(),
            msg: err.to_string(),
        })?;

        *self = postcard::from_bytes(&bytes).map_err(|err| markov_error::DeserializationError {
            msg: err.to_string(),
        })?;
        Ok(())
    }

    /// Generate the next note using temperature, top-k, backoff and restart.
    ///
    /// Order of operations:
    /// 1. Try the full context (order). If no edges and `backoff` is on,
    ///    try shorter contexts down to 0.
    /// 2. If still no edges and `restart` is on, pick a random observed state.
    /// 3. Apply top-k (keep `k` most frequent edges) if `top_k > 0`.
    /// 4. Apply temperature: `weight = count^(1/T)`.
    /// 5. Sample with `WeightedIndex`.
    /// 6. Update history (push chosen, trim to `order`).
    pub fn next_with_temperature(
        &mut self,
        cfg: markov_sample_config,
    ) -> Result<markov_note, markov_error> {
        // ---- 1. Resolve context with optional backoff ----
        let mut chosen: Option<usize> = None;

        let max_ctx = self.order.into_usize().min(self.history.len());
        let ctx_range: Box<dyn Iterator<Item = usize>> = if cfg.backoff {
            Box::new((0..=max_ctx).rev())
        } else {
            // Only the full context; if history is shorter, max_ctx < order
            // and we still try exactly max_ctx (which may be 0).
            Box::new(std::iter::once(max_ctx))
        };

        for ctx_len in ctx_range {
            let key = pack_history(
                &self.history,
                (ctx_len as u8).try_into().unwrap_or_default(),
            );
            let Some(edges) = self.transitions.get(&key) else {
                continue;
            };
            if edges.is_empty() {
                continue;
            }
            // Disjoint field borrows: `self.transitions` is immutably borrowed
            // through `edges`, `self.rng` is mutably borrowed separately.
            // `sample_from_edges` is an associated fn, not a method on `&mut self`,
            // so the compiler sees two disjoint borrows.
            chosen = Some(Self::sample_from_edges(edges, &mut self.rng, cfg)?);
            break;
        }

        // ---- 2. Restart fallback ----
        let chosen = match chosen {
            Some(idx) => idx,
            None => {
                if !cfg.restart {
                    // Report the full-context key for diagnostics.
                    let key = pack_history(
                        &self.history,
                        (max_ctx as u8).try_into().unwrap_or_default(),
                    );
                    return Err(markov_error::NoTransitions { state: key });
                }
                // Global (order-0) distribution is the natural "observed states"
                // set when `train` records at all levels. If even that is empty,
                // the chain has never been trained.
                let global = self
                    .transitions
                    .get(&0)
                    .ok_or(markov_error::NoCurrentState)?;
                if global.is_empty() {
                    return Err(markov_error::NoCurrentState);
                }
                // Restart bypasses temperature/top-k: we want a fresh state,
                // not a smoothed one.
                let weights: Vec<u32> = global.iter().map(|&(_, c)| c).collect();
                let dist = WeightedIndex::new(&weights).map_err(|_| markov_error::Sampling)?;
                global[dist.sample(&mut self.rng)].0
            }
        };

        // ---- 3. Update history ----
        self.history.push(chosen);
        if self.history.len() > self.order.into_usize() {
            self.history.remove(0);
        }

        index_to_note(chosen)
    }

    /// Sample one state index from `edges`, applying top-k and temperature.
    ///
    /// This is an *associated* function (no `self`), so the caller can pass
    /// `&self.transitions`-derived `edges` and `&mut self.rng` separately.
    /// That avoids the "immutable borrow later used by call" error.
    fn sample_from_edges(
        edges: &[(usize, u32)],
        rng: &mut SmallRng,
        cfg: markov_sample_config,
    ) -> Result<usize, markov_error> {
        debug_assert!(!edges.is_empty());

        // top-k: keep the k most frequent edges.
        let kept: Vec<(usize, u32)>;
        let slice: &[(usize, u32)] = if cfg.top_k == 0 || cfg.top_k >= edges.len() {
            edges
        } else {
            let mut sorted = edges.to_vec();
            sorted.sort_unstable_by(|a, b| b.1.cmp(&a.1));
            sorted.truncate(cfg.top_k);
            kept = sorted;
            &kept
        };

        // temperature: weight = count^(1/T). Skip powf when T == 1.
        let t = cfg.temperature;
        let inv_t = if t > 0.0 { 1.0 / t } else { 1e6 };
        let weights: Vec<f32> = if (inv_t - 1.0).abs() < f32::EPSILON {
            slice.iter().map(|&(_, c)| c as f32).collect()
        } else {
            slice.iter().map(|&(_, c)| (c as f32).powf(inv_t)).collect()
        };

        let dist = WeightedIndex::new(&weights).map_err(|_| markov_error::Sampling)?;
        Ok(slice[dist.sample(rng)].0)
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
        chain::{
            index_to_note, markov_sample_config, note_to_index, MarkovChain, MAX_DUR, MAX_PITCH,
        },
        errors::markov_error::{NoCurrentState, NoTransitions},
        markov::{
            markov_duration, markov_note,
            markov_order::{self, Order1, Order2},
        },
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
        c.train(note(60, 0)).unwrap();
        c.train(note(62, 0)).unwrap();
        c.train(note(60, 0)).unwrap();

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

        c.train(note(60, 0)).unwrap();
        assert_eq!(
            c.next(),
            Err(NoTransitions {
                state: note_to_index(note(60, 0)) as u64
            })
        ); // still no transition (only one note seen)

        c.train(note(62, 0)).unwrap();
        c.set_history1(note(60, 0)).unwrap();
        assert_eq!(c.next(), Ok(note(62, 0)));
    }

    #[test]
    fn clear_and_reset() {
        let mut c = MarkovChain::new(42, markov_order::Order1);
        c.train(note(60, 0)).unwrap();
        c.train(note(62, 0)).unwrap();
        c.reset();
        assert!(c.next().is_err());
        c.train(note(60, 0)).unwrap();
        c.train(note(62, 0)).unwrap();
        c.clear();
        assert!(c.next().is_err());
    }

    #[test]
    fn order_2_needs_two_previous() {
        let mut c = MarkovChain::new(42, markov_order::Order2);
        c.train(note(60, 0)).unwrap();
        assert!(c.next().is_err());
        c.train(note(62, 0)).unwrap();
        assert!(c.next().is_err()); // still not enough for order 2
        c.train(note(64, 0)).unwrap();

        c.set_history2(note(60, 0), note(62, 0)).unwrap();
        // Now transition (60, 62) -> 64 exists.
        let n = c.next().unwrap();
        assert_eq!(n.pitch, 64);
    }

    #[test]
    fn backoff_falls_to_shorter_context() {
        let mut c = MarkovChain::new(42, Order2);
        // Train on 60 -> 62 -> 64, then force history to [62, 64].
        // Transition (62, 64) -> ? was never observed at order 2,
        // but order 1 has 62 -> 64 and 64 -> ? (nothing).
        // Actually 64 has no outgoing at any level, so backoff finds nothing
        // for 64 alone. Train on 64 -> 60 to give it an order-1 edge.
        c.train(note(60, 0)).unwrap();
        c.train(note(62, 0)).unwrap();
        c.train(note(64, 0)).unwrap();
        c.train(note(60, 0)).unwrap();

        c.set_history2(note(62, 0), note(64, 0)).unwrap();
        let cfg = markov_sample_config {
            backoff: true,
            restart: false,
            ..Default::default()
        };
        // Order-2 context (62, 64) has no edges, backoff to order-1 context (64)
        // which has 64 -> 60.
        let n = c.next_with_temperature(cfg).unwrap();
        assert_eq!(n.pitch, 60);
    }

    #[test]
    fn temperature_and_backoff_compile_and_run() {
        let mut c = MarkovChain::new(42, Order2);
        c.train(note(60, 0)).unwrap();
        c.train(note(62, 0)).unwrap();
        c.train(note(64, 0)).unwrap();
        c.train(note(60, 0)).unwrap();

        // Force order-2 context (62, 64); order-2 has no edges from it,
        // backoff to order-1 context (64), which has 64 -> 60.
        c.set_history2(note(62, 0), note(64, 0)).unwrap();

        let cfg = markov_sample_config {
            backoff: true,
            restart: false,
            temperature: 1.0,
            top_k: 0,
        };
        let n = c.next_with_temperature(cfg).unwrap();
        assert_eq!(n.pitch, 60);
    }

    #[test]
    fn restart_when_no_edges_anywhere() {
        let mut c = MarkovChain::new(42, Order1);
        c.train(note(60, 0)).unwrap();
        c.train(note(62, 0)).unwrap();
        // Current history is [62], no edges from 62 at any level except order-0.
        let cfg = markov_sample_config::default();
        let n = c.next_with_temperature(cfg).unwrap();
        assert!(n.pitch == 60 || n.pitch == 62);
    }
}
