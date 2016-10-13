add_definitions(-DPD)

set(M_SRC m_atom.c
    m_binbuf.c
    m_class.c
    m_conf.c
    m_glob.c
    m_memory.c
    m_obj.c
    m_pd.c
    m_sched.c)

set(D_SRC d_arithmetic.c
    d_array.c
    d_ctl.c
    d_dac.c
    d_delay.c
    d_fft.c
    d_filter.c
    d_global.c
    d_math.c
    d_misc.c
    d_osc.c
    d_resample.c
    d_soundfile.c
    d_ugen.c)

set(G_SRC
    g_all_guis.c
    g_array.c
    g_bang.c
    g_canvas.c
    g_clone.c
    g_editor.c
    g_graph.c
    g_guiconnect.c
    g_hdial.c
    g_hslider.c
    g_io.c
    g_mycanvas.c
    g_numbox.c
    g_readwrite.c
    g_rtext.c
    g_scalar.c
    g_template.c
    g_text.c
    g_toggle.c
    g_traversal.c
    g_vdial.c
    g_vslider.c
    g_vumeter.c)

set(S_SRC
    s_audio.c
#    s_audio_audiounit.c
#    s_audio_esd.c
#    s_audio_mmio.c
    s_audio_paring.c
#    s_entry.c
    s_file.c
    s_inter.c
    s_loader.c
    s_main.c
    s_midi.c
    s_path.c
    s_print.c
    s_utf8.c)

set(X_SRC
    x_acoustics.c
    x_arithmetic.c
    x_array.c
    x_connective.c
    x_gui.c
    x_interface.c
    x_list.c
    x_midi.c
    x_misc.c
    x_net.c
    x_scalar.c
    x_text.c
    x_time.c
    x_vexp.c
    x_vexp_fun.c
    x_vexp_if.c)

if(WITH_FFTW)
    include_directories(${FFTW_INCLUDE_DIRECTORIES})
    list(APPEND D_SRC d_fft_fftw.c)
    list(APPEND PLATFORM_LINK_LIBRARIES ${FFTW_LIBRARIES})
else()
    list(APPEND D_SRC d_fft_fftsg.c)
endif()

# ALSA
if(WITH_ALSA_AUDIO)
    list(APPEND S_SRC s_audio_alsa.c s_audio_alsamm.c)
    list(APPEND PLATFORM_LINK_LIBRARIES ${ALSA_LIBRARIES})
endif()

if(WITH_ALSA_MIDI)
    list(APPEND S_SRC s_midi_alsa.c)
    list(APPEND PLATFORM_LINK_LIBRARIES ${ALSA_LIBRARIES})
endif()

# JACK
if(WITH_JACK)
    list(APPEND S_SRC s_audio_jack.c)
    list(APPEND PLATFORM_LINK_LIBRARIES ${JACK_LIBRARIES})
endif()

if(WITH_PORTAUDIO)
    list(APPEND S_SRC s_audio_pa.c)
    list(APPEND PLATFORM_LINK_LIBRARIES ${PORTAUDIO_LIBRARIES})
endif()

if(WITH_PORTMIDI)
    include_directories(${PORTMIDI_INCLUDE_DIRECTORIES})
    list(APPEND S_SRC s_midi_pm.c)
    list(APPEND PLATFORM_LINK_LIBRARIES ${PORTMIDI_LIBRARIES})
endif()

# OSS
if(WITH_OSS_AUDIO)
    list(APPEND S_SRC s_audio_oss.c)
endif()

if(WITH_OSS_MIDI)
    list(APPEND S_SRC s_midi_oss.c)
endif()

# Dummy
if(WITH_DUMMY_AUDIO)
    list(APPEND S_SRC s_audio_dummy.c)
endif()

if(WITH_DUMMY_MIDI)
    list(APPEND S_SRC s_midi_dummy.c)
endif()

if(LIBDL_FOUND)
    list(APPEND PLATFORM_LINK_LIBRARIES ${LIBDL_LIBRARIES})
endif()

add_library(puredata-core STATIC ${D_SRC} ${G_SRC} ${M_SRC} ${S_SRC} ${X_SRC})

add_executable(pd s_entry.c)
target_link_libraries(pd puredata-core ${PLATFORM_LINK_LIBRARIES})

add_executable(pd-watchdog s_watchdog.c)
add_executable(pdsend u_pdsend.c)
add_executable(pdreceive u_pdreceive.c)

install(TARGETS pd pdsend pdreceive
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin)

install(FILES m_pd.h DESTINATION include/pd)
install(TARGETS pd-watchdog pd
    RUNTIME DESTINATION lib/puredata/bin
)
