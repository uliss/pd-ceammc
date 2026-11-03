// src/pitch_quantize.rs

use serde::{Deserialize, Serialize};

use crate::{errors::markov_error, markov::markov_note, pitch_quantize::out_of_range::FoldOctave};

/// MIDI pitch range (inclusive).
pub const MIDI_MIN: u8 = 0;
pub const MIDI_MAX: u8 = 127;

/// An octave span, e.g. `OctaveRange::new(3, 5)` covers MIDI 36..=83 (C3..B5).
///
/// Octave numbering follows the scientific pitch notation used by MIDI:
/// octave `n` starts at MIDI `12 * (n + 1)`. Thus C4 = 60, C3 = 48, C5 = 72.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
pub struct OctaveRange {
    /// Lowest octave (inclusive).
    pub min: i8,
    /// Highest octave (inclusive).
    pub max: i8,
}

impl Default for OctaveRange {
    fn default() -> Self {
        Self {
            min: MIDI_MIN as i8,
            max: MIDI_MAX as i8,
        }
    }
}

impl OctaveRange {
    /// Create a range; returns an error if `min > max` or the span is outside
    /// the MIDI pitch range.
    pub fn new(min: i8, max: i8) -> Result<Self, markov_error> {
        if min > max {
            return Err(markov_error::InvalidOctaveRange { min, max });
        }
        let lo = octave_to_midi_min(min);
        let hi = octave_to_midi_max(max);
        if lo > MIDI_MAX || hi < MIDI_MIN {
            return Err(markov_error::InvalidOctaveRange { min, max });
        }
        Ok(Self { min, max })
    }

    /// Lowest MIDI pitch covered by this range (inclusive).
    pub fn midi_min(&self) -> u8 {
        octave_to_midi_min(self.min).max(MIDI_MIN)
    }

    /// Highest MIDI pitch covered by this range (inclusive).
    pub fn midi_max(&self) -> u8 {
        octave_to_midi_max(self.max).min(MIDI_MAX)
    }

    /// Number of distinct MIDI pitches in the range.
    pub fn span(&self) -> usize {
        (self.midi_max() - self.midi_min() + 1) as usize
    }

    pub fn quantize(
        &self,
        note: markov_note,
        mode: out_of_range,
    ) -> Result<markov_note, markov_error> {
        Ok(markov_note {
            pitch: quantize_pitch(note.pitch, &self, mode)?,
            dur_bin: note.dur_bin,
        })
    }
}

/// First MIDI pitch of octave `n` (C of that octave).
fn octave_to_midi_min(n: i8) -> u8 {
    // C0 = 12, C-1 = 0. Clamp to valid MIDI so the arithmetic stays in u8.
    let v = 12i32 * (n as i32 + 1);
    v.clamp(MIDI_MIN as i32, MIDI_MAX as i32) as u8
}

/// Last MIDI pitch of octave `n` (B of that octave).
fn octave_to_midi_max(n: i8) -> u8 {
    let v = 12i32 * (n as i32 + 1) + 11;
    v.clamp(MIDI_MIN as i32, MIDI_MAX as i32) as u8
}

/// Strategy for handling pitches outside the target range.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
#[repr(u8)]
#[allow(non_camel_case_types)]
pub enum out_of_range {
    /// Reject the pitch (return an error).
    Reject,
    /// Transpose by whole octaves until the pitch lands inside the range.
    /// The pitch class is preserved; only the octave changes.
    FoldOctave,
    /// Clamp to the nearest bound of the range. Loses pitch class.
    Clamp,
}

impl Default for out_of_range {
    fn default() -> Self {
        FoldOctave
    }
}

/// Quantize a MIDI pitch (0..=127) into the given octave range.
///
/// Returns a pitch in `range.midi_min()..=range.midi_max()` on success, or an
/// error if `pitch` is outside the range and `mode == OutOfRange::Reject`.
///
/// # Examples
/// ```
/// use markov_notes::pitch_quantize::{OctaveRange, OutOfRange, quantize_pitch};
/// let range = OctaveRange::new(3, 5).unwrap();   // C3..B5 = 48..=83
/// assert_eq!(quantize_pitch(60, range, OutOfRange::Reject), Ok(60)); // C4
/// assert_eq!(quantize_pitch(36, range, OutOfRange::FoldOctave), Ok(48)); // C2 -> C3
/// assert_eq!(quantize_pitch(100, range, OutOfRange::Clamp), Ok(83));   // B5
/// ```
pub fn quantize_pitch(
    pitch: u8,
    range: &OctaveRange,
    mode: out_of_range,
) -> Result<u8, markov_error> {
    if pitch > MIDI_MAX {
        return Err(markov_error::InvalidPitch {
            pitch: pitch as usize,
        });
    }

    let lo = range.midi_min();
    let hi = range.midi_max();
    if (lo..=hi).contains(&pitch) {
        return Ok(pitch);
    }

    match mode {
        out_of_range::Reject => Err(markov_error::PitchOutOfRange {
            pitch: pitch as usize,
            min: lo,
            max: hi,
        }),
        out_of_range::FoldOctave => {
            // Shift by whole octaves until inside. Because the range spans at
            // least one full octave (we enforce this in OctaveRange::new),
            // one of at most 11 shifts lands inside.
            let mut p = pitch as i32;
            while p < lo as i32 {
                p += 12;
            }
            while p > hi as i32 {
                p -= 12;
            }
            if (lo..=hi).contains(&(p as u8)) {
                Ok(p as u8)
            } else {
                // Degenerate range (fewer than 12 semitones) — fall back to clamp.
                Ok(p.clamp(lo as i32, hi as i32) as u8)
            }
        }
        out_of_range::Clamp => Ok(pitch.clamp(lo, hi)),
    }
}

/// Quantize a sequence of pitches in one call (convenience wrapper).
pub fn quantize_pitches(
    pitches: &[u8],
    range: &OctaveRange,
    mode: out_of_range,
) -> Result<Vec<u8>, markov_error> {
    pitches
        .iter()
        .map(|&p| quantize_pitch(p, range, mode))
        .collect()
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn octave_bounds() {
        // C4 = 60, B4 = 71.
        let r = OctaveRange::new(4, 4).unwrap();
        assert_eq!(r.midi_min(), 60);
        assert_eq!(r.midi_max(), 71);
        assert_eq!(r.span(), 12);

        // C3..B5 = 48..=83, 3 octaves = 36 semitones.
        let r = OctaveRange::new(3, 5).unwrap();
        assert_eq!(r.midi_min(), 48);
        assert_eq!(r.midi_max(), 83);
        assert_eq!(r.span(), 36);
    }

    #[test]
    fn invalid_range_rejected() {
        assert!(OctaveRange::new(5, 3).is_err());
        // Octave 11 would start at MIDI 144, outside the MIDI range.
        assert!(OctaveRange::new(11, 11).is_err());
        // Octave -2 would end at MIDI -13, outside the MIDI range.
        assert!(OctaveRange::new(-2, -2).is_err());
    }

    #[test]
    fn in_range_passes_through() {
        let r = OctaveRange::new(3, 5).unwrap();
        for p in 48..=83 {
            assert_eq!(quantize_pitch(p, &r, out_of_range::Reject), Ok(p));
        }
    }

    #[test]
    fn reject_mode_errors_outside_range() {
        let r = OctaveRange::new(4, 4).unwrap(); // 60..=71
        assert!(quantize_pitch(59, &r, out_of_range::Reject).is_err());
        assert!(quantize_pitch(72, &r, out_of_range::Reject).is_err());
        assert_eq!(quantize_pitch(60, &r, out_of_range::Reject), Ok(60));
        assert_eq!(quantize_pitch(71, &r, out_of_range::Reject), Ok(71));
    }

    #[test]
    fn fold_octave_preserves_pitch_class() {
        let r = OctaveRange::new(4, 4).unwrap(); // 60..=71
                                                 // C2 (36) -> C4 (60): +24 semitones = +2 octaves.
        assert_eq!(quantize_pitch(36, &r, out_of_range::FoldOctave), Ok(60));
        // C6 (84) -> C4 (60): -24 semitones = -2 octaves.
        assert_eq!(quantize_pitch(84, &r, out_of_range::FoldOctave), Ok(60));
        // F#2 (42) -> F#4 (66): pitch class preserved.
        assert_eq!(quantize_pitch(42, &r, out_of_range::FoldOctave), Ok(66));
        // B6 (95) -> B4 (71): +? Let's verify: 95 - 24 = 71. Yes.
        assert_eq!(quantize_pitch(95, &r, out_of_range::FoldOctave), Ok(71));
    }

    #[test]
    fn fold_octave_for_wide_range() {
        let r = OctaveRange::new(3, 5).unwrap(); // 48..=83
                                                 // C1 (24) -> C3 (48): +2 octaves.
        assert_eq!(quantize_pitch(24, &r, out_of_range::FoldOctave), Ok(48));
        // C7 (96) -> C5 (72): -2 octaves.
        assert_eq!(quantize_pitch(96, &r, out_of_range::FoldOctave), Ok(72));
    }

    #[test]
    fn clamp_mode() {
        let r = OctaveRange::new(4, 4).unwrap(); // 60..=71
        assert_eq!(quantize_pitch(0, &r, out_of_range::Clamp), Ok(60));
        assert_eq!(quantize_pitch(127, &r, out_of_range::Clamp), Ok(71));
        assert_eq!(quantize_pitch(65, &r, out_of_range::Clamp), Ok(65));
    }

    #[test]
    fn sequence_quantization() {
        let r = OctaveRange::new(4, 4).unwrap();
        let input = [36, 60, 84, 65];
        let folded = quantize_pitches(&input, &r, out_of_range::FoldOctave).unwrap();
        assert_eq!(folded, vec![60, 60, 60, 65]);

        let clamped = quantize_pitches(&input, &r, out_of_range::Clamp).unwrap();
        assert_eq!(clamped, vec![60, 60, 71, 65]);
    }

    #[test]
    fn midi_boundary_pitch() {
        let r = OctaveRange::new(0, 9).unwrap();
        // MIDI 0 is C-1, which is below octave 0. Fold should bring it up.
        assert_eq!(quantize_pitch(0, &r, out_of_range::FoldOctave), Ok(12));
        // MIDI 127 is G9, inside 0..=9, so passes through.
        assert_eq!(quantize_pitch(127, &r, out_of_range::Reject), Ok(127));
    }
}
