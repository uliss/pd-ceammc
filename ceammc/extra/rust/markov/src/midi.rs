use midly::{MetaMessage, MidiMessage, Smf, TrackEventKind};
use std::collections::HashMap;
use std::path::Path;

use crate::errors::markov_error;

/// Note extraction mode
#[derive(Debug, Clone)]
pub enum ExtractMode {
    /// All notes from all tracks, sorted by timeline
    AllTracks,
    /// All notes from the track with the given index, sorted by timeline
    TrackByIndex(usize),
    /// All notes from the track with the given name, sorted by timeline
    TrackByName(String),
}

/// Information about a note
#[derive(Debug, Clone, PartialEq)]
pub struct Note {
    /// Absolute start time in ticks
    pub start_tick: u64,
    /// MIDI pitch (0-127)
    pub pitch: u8,
    /// Velocity
    pub velocity: u8,
    /// Duration in ticks
    pub duration_ticks: u64,
    /// Index of the track the note was extracted from
    pub track_index: usize,
}

/// Reads a MIDI file asynchronously and extracts notes according to the given mode.
pub async fn extract_notes_async<P: AsRef<Path> + std::fmt::Debug>(
    path: P,
    mode: ExtractMode,
) -> Result<Vec<Note>, markov_error> {
    let data = tokio::fs::read(&path)
        .await
        .map_err(|e| markov_error::FSReadError {
            path: format!("{path:?}"),
            msg: e.to_string(),
        })?;

    tokio::task::spawn_blocking(move || extract_notes_from_bytes(&data, mode))
        .await
        .map_err(|e| markov_error::TokioTaskJoinError { msg: e.to_string() })?
}

/// Synchronous core: parses MIDI bytes and extracts notes.
/// Kept public so it can be reused or tested without a runtime.
pub fn extract_notes_from_bytes(data: &[u8], mode: ExtractMode) -> Result<Vec<Note>, markov_error> {
    let smf = Smf::parse(data).map_err(|e| markov_error::MidiParseError { msg: e.to_string() })?;

    // Determine which tracks to process
    let track_indices: Vec<usize> = match mode {
        ExtractMode::AllTracks => (0..smf.tracks.len()).collect(),
        ExtractMode::TrackByIndex(idx) => {
            if idx >= smf.tracks.len() {
                return Err(markov_error::TrackIndexNotFound {
                    idx,
                    total: smf.tracks.len(),
                });
            }
            vec![idx]
        }
        ExtractMode::TrackByName(ref name) => {
            let mut found = None;
            for (i, track) in smf.tracks.iter().enumerate() {
                for event in track {
                    if let TrackEventKind::Meta(MetaMessage::TrackName(bytes)) = event.kind {
                        if let Ok(track_name) = std::str::from_utf8(bytes) {
                            if track_name.trim() == name {
                                found = Some(i);
                                break;
                            }
                        }
                    }
                }
                if found.is_some() {
                    break;
                }
            }
            match found {
                Some(i) => vec![i],
                None => return Err(markov_error::TrackNameNotFound { name: name.clone() }),
            }
        }
    };

    let mut notes = Vec::new();

    for &track_idx in &track_indices {
        let track = &smf.tracks[track_idx];
        // Active notes map: pitch -> (start_tick, velocity)
        let mut active_notes: HashMap<u8, (u64, u8)> = HashMap::new();
        let mut abs_tick: u64 = 0;

        for event in track {
            abs_tick += u64::from(event.delta.as_int());

            match event.kind {
                TrackEventKind::Midi { message, .. } => match message {
                    MidiMessage::NoteOn { key, vel } => {
                        if vel > 0 {
                            active_notes.insert(key.as_int(), (abs_tick, vel.as_int()));
                        } else {
                            // NoteOn with velocity 0 is equivalent to NoteOff
                            if let Some((start, velocity)) = active_notes.remove(&key.as_int()) {
                                notes.push(Note {
                                    start_tick: start,
                                    pitch: key.as_int(),
                                    velocity,
                                    duration_ticks: abs_tick - start,
                                    track_index: track_idx,
                                });
                            }
                        }
                    }
                    MidiMessage::NoteOff { key, .. } => {
                        if let Some((start, velocity)) = active_notes.remove(&key.as_int()) {
                            notes.push(Note {
                                start_tick: start,
                                pitch: key.as_int(),
                                velocity,
                                duration_ticks: abs_tick - start,
                                track_index: track_idx,
                            });
                        }
                    }
                    _ => {}
                },
                _ => {}
            }
        }
    }

    // Sort by timeline (start_tick), then by pitch for stability
    notes.sort_by_key(|n| (n.start_tick, n.pitch));

    Ok(notes)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn build_smf(tracks: Vec<Vec<u8>>) -> Vec<u8> {
        let mut out = Vec::new();
        out.extend_from_slice(b"MThd");
        out.extend_from_slice(&6u32.to_be_bytes());
        out.extend_from_slice(&1u16.to_be_bytes());
        out.extend_from_slice(&(tracks.len() as u16).to_be_bytes());
        out.extend_from_slice(&96u16.to_be_bytes());

        for mut track in tracks {
            track.extend_from_slice(&[0x00, 0xFF, 0x2F, 0x00]);
            out.extend_from_slice(b"MTrk");
            out.extend_from_slice(&(track.len() as u32).to_be_bytes());
            out.extend_from_slice(&track);
        }

        out
    }

    fn note_on(delta: u8, ch: u8, key: u8, vel: u8) -> Vec<u8> {
        vec![delta, 0x90 | ch, key, vel]
    }

    fn note_off(delta: u8, ch: u8, key: u8, vel: u8) -> Vec<u8> {
        vec![delta, 0x80 | ch, key, vel]
    }

    fn track_name(delta: u8, name: &str) -> Vec<u8> {
        let bytes = name.as_bytes();
        let mut v = vec![delta, 0xFF, 0x03, bytes.len() as u8];
        v.extend_from_slice(bytes);
        v
    }

    #[test]
    fn test_single_track_single_note() {
        let mut track = Vec::new();
        track.extend(track_name(0, "Piano"));
        track.extend(note_on(0, 0, 60, 100));
        track.extend(note_off(96, 0, 60, 0));

        let data = build_smf(vec![track]);
        let notes = extract_notes_from_bytes(&data, ExtractMode::AllTracks).unwrap();

        assert_eq!(notes.len(), 1);
        assert_eq!(notes[0].pitch, 60);
        assert_eq!(notes[0].velocity, 100);
        assert_eq!(notes[0].start_tick, 0);
        assert_eq!(notes[0].duration_ticks, 96);
        assert_eq!(notes[0].track_index, 0);
    }

    #[test]
    fn test_note_on_with_zero_velocity_is_note_off() {
        let mut track = Vec::new();
        track.extend(note_on(0, 0, 64, 80));
        track.extend(note_on(48, 0, 64, 0));

        let data = build_smf(vec![track]);
        let notes = extract_notes_from_bytes(&data, ExtractMode::AllTracks).unwrap();

        assert_eq!(notes.len(), 1);
        assert_eq!(notes[0].pitch, 64);
        assert_eq!(notes[0].start_tick, 0);
        assert_eq!(notes[0].duration_ticks, 48);
    }

    #[test]
    fn test_multiple_notes_sorted_by_timeline() {
        let mut track = Vec::new();
        track.extend(note_on(0, 0, 60, 100));
        track.extend(note_off(24, 0, 60, 0));
        track.extend(note_on(0, 0, 62, 90));
        track.extend(note_off(24, 0, 62, 0));
        track.extend(note_on(0, 0, 64, 80));
        track.extend(note_off(24, 0, 64, 0));

        let data = build_smf(vec![track]);
        let notes = extract_notes_from_bytes(&data, ExtractMode::AllTracks).unwrap();

        assert_eq!(notes.len(), 3);
        assert_eq!(notes[0].pitch, 60);
        assert_eq!(notes[0].start_tick, 0);
        assert_eq!(notes[1].pitch, 62);
        assert_eq!(notes[1].start_tick, 24);
        assert_eq!(notes[2].pitch, 64);
        assert_eq!(notes[2].start_tick, 48);
    }

    #[test]
    fn test_two_tracks_all_tracks_mode() {
        let mut t0 = Vec::new();
        t0.extend(track_name(0, "Bass"));
        t0.extend(note_on(0, 0, 36, 100));
        t0.extend(note_off(96, 0, 36, 0));

        let mut t1 = Vec::new();
        t1.extend(track_name(0, "Lead"));
        t1.extend(note_on(48, 0, 72, 100));
        t1.extend(note_off(48, 0, 72, 0));

        let data = build_smf(vec![t0, t1]);
        let notes = extract_notes_from_bytes(&data, ExtractMode::AllTracks).unwrap();

        assert_eq!(notes.len(), 2);
        assert_eq!(notes[0].pitch, 36);
        assert_eq!(notes[0].track_index, 0);
        assert_eq!(notes[0].start_tick, 0);
        assert_eq!(notes[1].pitch, 72);
        assert_eq!(notes[1].track_index, 1);
        assert_eq!(notes[1].start_tick, 48);
    }

    #[test]
    fn test_track_by_index() {
        let mut t0 = Vec::new();
        t0.extend(note_on(0, 0, 36, 100));
        t0.extend(note_off(96, 0, 36, 0));

        let mut t1 = Vec::new();
        t1.extend(note_on(0, 0, 72, 100));
        t1.extend(note_off(96, 0, 72, 0));

        let data = build_smf(vec![t0, t1]);
        let notes = extract_notes_from_bytes(&data, ExtractMode::TrackByIndex(1)).unwrap();

        assert_eq!(notes.len(), 1);
        assert_eq!(notes[0].pitch, 72);
        assert_eq!(notes[0].track_index, 1);
    }

    #[test]
    fn test_track_by_index_out_of_bounds() {
        let mut t0 = Vec::new();
        t0.extend(note_on(0, 0, 60, 100));
        t0.extend(note_off(96, 0, 60, 0));

        let data = build_smf(vec![t0]);
        let err = extract_notes_from_bytes(&data, ExtractMode::TrackByIndex(5)).unwrap_err();
        assert_eq!(err, markov_error::TrackIndexNotFound { idx: 5, total: 1 });
    }

    #[test]
    fn test_track_by_name() {
        let mut t0 = Vec::new();
        t0.extend(track_name(0, "Bass"));
        t0.extend(note_on(0, 0, 36, 100));
        t0.extend(note_off(96, 0, 36, 0));

        let mut t1 = Vec::new();
        t1.extend(track_name(0, "Lead"));
        t1.extend(note_on(0, 0, 72, 100));
        t1.extend(note_off(96, 0, 72, 0));

        let data = build_smf(vec![t0, t1]);
        let notes =
            extract_notes_from_bytes(&data, ExtractMode::TrackByName("Lead".to_string())).unwrap();

        assert_eq!(notes.len(), 1);
        assert_eq!(notes[0].pitch, 72);
        assert_eq!(notes[0].track_index, 1);
    }

    #[test]
    fn test_track_by_name_not_found() {
        let mut t0 = Vec::new();
        t0.extend(track_name(0, "Bass"));
        t0.extend(note_on(0, 0, 36, 100));
        t0.extend(note_off(96, 0, 36, 0));

        let data = build_smf(vec![t0]);
        let err =
            extract_notes_from_bytes(&data, ExtractMode::TrackByName("Nonexistent".to_string()))
                .unwrap_err();
        assert_eq!(
            err,
            markov_error::TrackNameNotFound {
                name: "Nonexistent".into()
            }
        );
    }

    #[test]
    fn test_overlapping_notes_same_pitch() {
        let mut track = Vec::new();
        track.extend(note_on(0, 0, 60, 100));
        track.extend(note_on(10, 0, 60, 90));
        track.extend(note_off(20, 0, 60, 0));

        let data = build_smf(vec![track]);
        let notes = extract_notes_from_bytes(&data, ExtractMode::AllTracks).unwrap();

        assert_eq!(notes.len(), 1);
        assert_eq!(notes[0].start_tick, 10);
        assert_eq!(notes[0].velocity, 90);
        assert_eq!(notes[0].duration_ticks, 20);
    }

    #[test]
    fn test_midi_parse_error() {
        let err = extract_notes_from_bytes(b"not a midi file", ExtractMode::AllTracks).unwrap_err();
        assert_eq!(
            err,
            markov_error::MidiParseError {
                msg: "invalid midi: not a midi file".into()
            }
        );
    }

    #[tokio::test]
    async fn test_async_file_not_found() {
        let err = extract_notes_async("/nonexistent/path/to/file.mid", ExtractMode::AllTracks)
            .await
            .unwrap_err();
        assert_eq!(
            err,
            markov_error::FSReadError {
                path: "\"/nonexistent/path/to/file.mid\"".into(),
                msg: "No such file or directory (os error 2)".into()
            }
        );
    }

    #[tokio::test]
    async fn test_async_reads_file() {
        let mut track = Vec::new();
        track.extend(note_on(0, 0, 60, 100));
        track.extend(note_off(96, 0, 60, 0));

        let data = build_smf(vec![track]);

        let mut path = std::env::temp_dir();
        path.push("markov_test_async.mid");
        tokio::fs::write(&path, &data).await.unwrap();

        let notes = extract_notes_async(&path, ExtractMode::AllTracks)
            .await
            .unwrap();
        assert_eq!(notes.len(), 1);
        assert_eq!(notes[0].pitch, 60);

        let _ = tokio::fs::remove_file(&path).await;
    }
}
