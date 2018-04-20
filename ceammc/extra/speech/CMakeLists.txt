set(FLITE_SRC
#    flite/src/audio/au_alsa.c
    flite/src/audio/au_command.c
    flite/src/audio/au_none.c
#    flite/src/audio/au_oss.c
#    flite/src/audio/au_palmos.c
#    flite/src/audio/au_pulseaudio.c
    flite/src/audio/au_streaming.c
#    flite/src/audio/au_sun.c
#    flite/src/audio/au_win.c
#    flite/src/audio/au_wince.c
    flite/src/audio/auclient.c
    flite/src/audio/audio.c
    flite/src/audio/auserver.c
    flite/src/utils/cst_alloc.c
    flite/src/utils/cst_args.c
    flite/src/utils/cst_endian.c
    flite/src/utils/cst_error.c
    flite/src/utils/cst_features.c
#    flite/src/utils/cst_file_palmos.c
    flite/src/utils/cst_file_stdio.c
#    flite/src/utils/cst_file_wince.c
    flite/src/utils/cst_mmap_none.c
    flite/src/utils/cst_mmap_posix.c
    flite/src/utils/cst_socket.c
    flite/src/utils/cst_string.c
    flite/src/utils/cst_tokenstream.c
    flite/src/utils/cst_url.c
    flite/src/utils/cst_val.c
    flite/src/utils/cst_val_const.c
    flite/src/utils/cst_val_user.c
    flite/src/utils/cst_wchar.c
    flite/src/cg/cst_cg.c
    flite/src/cg/cst_cg_dump_voice.c
    flite/src/cg/cst_cg_load_voice.c
    flite/src/cg/cst_cg_map.c
    flite/src/cg/cst_mlpg.c
    flite/src/cg/cst_mlsa.c
    flite/src/cg/cst_spamf0.c
    flite/src/cg/cst_vc.c
    flite/src/hrg/cst_ffeature.c
    flite/src/hrg/cst_item.c
    flite/src/hrg/cst_rel_io.c
    flite/src/hrg/cst_relation.c
    flite/src/hrg/cst_utterance.c
    flite/src/lexicon/cst_lexicon.c
    flite/src/lexicon/cst_lts.c
    flite/src/lexicon/cst_lts_rewrites.c
    flite/src/regex/cst_regex.c
    flite/src/regex/regexp.c
    flite/src/regex/regsub.c
    flite/src/speech/cst_lpcres.c
    flite/src/speech/cst_track.c
    flite/src/speech/cst_track_io.c
    flite/src/speech/cst_wave.c
    flite/src/speech/cst_wave_io.c
    flite/src/speech/cst_wave_utils.c
    flite/src/speech/g721.c
    flite/src/speech/g723_24.c
    flite/src/speech/g723_40.c
    flite/src/speech/g72x.c
    flite/src/speech/rateconv.c
    flite/src/stats/cst_cart.c
    flite/src/stats/cst_ss.c
    flite/src/stats/cst_viterbi.c
    flite/src/synth/cst_ffeatures.c
    flite/src/synth/cst_phoneset.c
    flite/src/synth/cst_ssml.c
    flite/src/synth/cst_synth.c
    flite/src/synth/cst_utt_utils.c
    flite/src/synth/cst_voice.c
    flite/src/synth/flite.c
    flite/src/wavesynth/cst_clunits.c
    flite/src/wavesynth/cst_diphone.c
    flite/src/wavesynth/cst_reflpc.c
    flite/src/wavesynth/cst_sigpr.c
    flite/src/wavesynth/cst_sts.c
    flite/src/wavesynth/cst_units.c
)

if(WIN32)
    list(APPEND FLITE_SRC flite/src/utils/cst_mmap_win32.c)
endif()

include_directories(flite/include)
add_library(flite STATIC ${FLITE_SRC})
