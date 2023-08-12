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
#    flite/src/audio/auclient.c
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
#    flite/src/utils/cst_socket.c
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
    flite/lang/cmulex/cmu_lex.c
    flite/lang/cmulex/cmu_lex_data.c
    flite/lang/cmulex/cmu_lex_entries.c
    flite/lang/cmulex/cmu_lts_model.c
    flite/lang/cmulex/cmu_lts_rules.c
    flite/lang/cmulex/cmu_postlex.c
    flite/lang/usenglish/us_aswd.c
    flite/lang/usenglish/us_dur_stats.c
    flite/lang/usenglish/us_durz_cart.c
    flite/lang/usenglish/us_expand.c
    flite/lang/usenglish/us_f0_model.c
    flite/lang/usenglish/us_f0lr.c
    flite/lang/usenglish/us_ffeatures.c
    flite/lang/usenglish/us_gpos.c
    flite/lang/usenglish/us_int_accent_cart.c
    flite/lang/usenglish/us_int_tone_cart.c
    flite/lang/usenglish/us_nums_cart.c
    flite/lang/usenglish/us_phoneset.c
    flite/lang/usenglish/us_phrasing_cart.c
    flite/lang/usenglish/us_pos_cart.c
    flite/lang/usenglish/us_text.c
    flite/lang/usenglish/usenglish.c

    # lang kal16
    flite/lang/cmu_us_kal16/cmu_us_kal16.c
    flite/lang/cmu_us_kal16/cmu_us_kal16_diphone.c
    flite/lang/cmu_us_kal16/cmu_us_kal16_lpc.c
    flite/lang/cmu_us_kal16/cmu_us_kal16_res.c
    flite/lang/cmu_us_kal16/cmu_us_kal16_residx.c

    # lang rms
    flite/lang/cmu_us_rms/cmu_us_rms.c
    flite/lang/cmu_us_rms/cmu_us_rms_cg.c
    flite/lang/cmu_us_rms/cmu_us_rms_cg_durmodel.c
    flite/lang/cmu_us_rms/cmu_us_rms_cg_f0_trees.c
    flite/lang/cmu_us_rms/cmu_us_rms_cg_phonestate.c
    flite/lang/cmu_us_rms/cmu_us_rms_cg_single_mcep_trees.c
    flite/lang/cmu_us_rms/cmu_us_rms_cg_single_params.c
    flite/lang/cmu_us_rms/cmu_us_rms_spamf0_accent.c
    flite/lang/cmu_us_rms/cmu_us_rms_spamf0_accent_params.c
    flite/lang/cmu_us_rms/cmu_us_rms_spamf0_phrase.c

    # lang slt
    flite/lang/cmu_us_slt/cmu_us_slt.c
    flite/lang/cmu_us_slt/cmu_us_slt_cg.c
    flite/lang/cmu_us_slt/cmu_us_slt_cg_durmodel.c
    flite/lang/cmu_us_slt/cmu_us_slt_cg_f0_trees.c
    flite/lang/cmu_us_slt/cmu_us_slt_cg_phonestate.c
    flite/lang/cmu_us_slt/cmu_us_slt_cg_single_mcep_trees.c
    flite/lang/cmu_us_slt/cmu_us_slt_cg_single_params.c
    flite/lang/cmu_us_slt/cmu_us_slt_spamf0_accent.c
    flite/lang/cmu_us_slt/cmu_us_slt_spamf0_accent_params.c
    flite/lang/cmu_us_slt/cmu_us_slt_spamf0_phrase.c

    # lang awb
    flite/lang/cmu_us_awb/cmu_us_awb.c
    flite/lang/cmu_us_awb/cmu_us_awb_cg.c
    flite/lang/cmu_us_awb/cmu_us_awb_cg_durmodel.c
    flite/lang/cmu_us_awb/cmu_us_awb_cg_f0_trees.c
    flite/lang/cmu_us_awb/cmu_us_awb_cg_phonestate.c
    flite/lang/cmu_us_awb/cmu_us_awb_cg_single_mcep_trees.c
    flite/lang/cmu_us_awb/cmu_us_awb_cg_single_params.c
    flite/lang/cmu_us_awb/cmu_us_awb_spamf0_accent.c
    flite/lang/cmu_us_awb/cmu_us_awb_spamf0_accent_params.c
    flite/lang/cmu_us_awb/cmu_us_awb_spamf0_phrase.c

    # english lang
    flite/lang/usenglish/us_aswd.c
    flite/lang/usenglish/us_dur_stats.c
    flite/lang/usenglish/us_durz_cart.c
    flite/lang/usenglish/us_expand.c
    flite/lang/usenglish/us_f0_model.c
    flite/lang/usenglish/us_f0lr.c
    flite/lang/usenglish/us_ffeatures.c
    flite/lang/usenglish/us_gpos.c
    flite/lang/usenglish/us_int_accent_cart.c
    flite/lang/usenglish/us_int_tone_cart.c
    flite/lang/usenglish/us_nums_cart.c
    flite/lang/usenglish/us_phoneset.c
    flite/lang/usenglish/us_phrasing_cart.c
    flite/lang/usenglish/us_pos_cart.c
    flite/lang/usenglish/us_text.c
    flite/lang/usenglish/usenglish.c
)

add_library(flite STATIC ${FLITE_SRC})

if(UNIX)
    target_sources(flite PRIVATE flite/src/utils/cst_mmap_posix.c)
elseif(WIN32)
    target_sources(flite PRIVATE flite/src/utils/cst_mmap_win32.c)
endif()

target_compile_definitions(flite PRIVATE -DCST_NO_SOCKETS)
target_include_directories(flite PRIVATE flite/include)
target_include_directories(flite PRIVATE flite/lang/cmulex)
target_include_directories(flite PRIVATE flite/lang/usenglish)

if(UNIX)
    set_target_properties(flite PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
