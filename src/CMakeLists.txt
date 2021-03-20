add_definitions(-DPD)

if(UNIX)
    add_definitions("-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"")
endif()

add_definitions(-include ${PROJECT_BINARY_DIR}/config.h)

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
    g_ceammc_draw.c
    g_clone.c
    g_editor.c
    g_editor_extras.c
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
    g_style.c
    g_template.c
    g_text.c
    g_toggle.c
    g_traversal.c
    g_undo.c
    g_vdial.c
    g_vslider.c
    g_vumeter.c)

set(S_SRC
    s_audio.c
#    s_audio_audiounit.c
#    s_audio_esd.c
    s_audio_paring.c
#    s_entry.c
    s_file.c
    s_inter.c
    s_loader.c
    s_main.c
    s_midi.c
#    s_midi_dummy.c
    s_net.c
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

set(MISC_H
    g_style.h
    g_ceammc_draw.h
)

set(PD_NEEDED_LIBS)

if(WITH_FFTW)
    include_directories(${FFTW_INCLUDE_DIRECTORIES})
    list(APPEND D_SRC d_fft_fftw.c)
    list(APPEND PD_NEEDED_LIBS ${FFTW_LIBRARIES})
else()
    list(APPEND D_SRC d_fft_fftsg.c)
endif()

# ALSA
if(WITH_ALSA_AUDIO)
    list(APPEND S_SRC s_audio_alsa.c s_audio_alsamm.c)
    list(APPEND PD_NEEDED_LIBS ${ALSA_LIBRARIES})
endif()

if(WITH_ALSA_MIDI)
    list(APPEND S_SRC s_midi_alsa.c)
    list(APPEND PD_NEEDED_LIBS ${ALSA_LIBRARIES})
endif()

# JACK
if(WITH_JACK)
    list(APPEND S_SRC s_audio_jack.c)
    list(APPEND PD_NEEDED_LIBS ${JACK_LIBRARIES})
endif()

if(WITH_PORTAUDIO)
    list(APPEND S_SRC s_audio_pa.c)
    list(APPEND PD_NEEDED_LIBS ${PORTAUDIO_LIBRARIES})
    if(MINGW)
        # this is a link fix when portaudio was build with ASIO support.
        # and it requires C++ linking
        # we are making empty C++ file, so cmake automatically uses c++ linker
        set(_asio_link_fix ${CMAKE_CURRENT_BINARY_DIR}/asio_link_fix.cpp)
        if(NOT EXISTS ${_asio_link_fix})
            file(WRITE ${_asio_link_fix} "")
        endif()
        list(APPEND S_SRC ${_asio_link_fix})
    endif()
endif()

if(WITH_MMIO)
    add_definitions(-DUSEAPI_MMIO)
    list(APPEND S_SRC s_audio_mmio.c)
endif()

if(WITH_PORTMIDI)
    include_directories(${PROJECT_SOURCE_DIR}/portmidi/portmidi/pm_common)
    include_directories(${PROJECT_SOURCE_DIR}/portmidi/portmidi/porttime)
    list(APPEND S_SRC s_midi_pm.c)
    list(APPEND PD_NEEDED_LIBS portmidi-static)
endif()

if(WITH_COREMIDI)
    list(APPEND S_SRC s_midi_coremidi.c)
endif()

if(WISHAPP)
    add_definitions(-DWISHAPP="${WISHAPP}")
endif()

if(WITH_OSS)
    list(APPEND S_SRC s_audio_oss.c)
    list(APPEND S_SRC s_midi_oss.c)
    list(APPEND PD_NEEDED_LIBS ${OSSLIB})
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

if(WIN32 OR MSYS)
    set(RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/pd.rc")
    configure_file("pd.rc" ${RESOURCE})

    add_library(puredata-core SHARED ${D_SRC} ${G_SRC} ${M_SRC} ${S_SRC} ${X_SRC})
    set_target_properties(puredata-core PROPERTIES PREFIX "" OUTPUT_NAME pd)
    target_link_options(puredata-core PRIVATE "-Wl,--export-all-symbols")
    add_executable(pd WIN32 s_entry.c ${RESOURCE})
else()
    add_library(puredata-core STATIC ${D_SRC} ${G_SRC} ${M_SRC} ${S_SRC} ${X_SRC} ${MISC_H})
    add_executable(pd s_entry.c)
    set_target_properties(puredata-core PROPERTIES COMPILE_FLAGS "-fPIC")

    if(APPLE)
        target_link_libraries(puredata-core PRIVATE "-framework CoreMIDI")
        target_link_libraries(puredata-core PRIVATE "-framework CoreMedia")
        target_link_libraries(puredata-core PRIVATE "-framework CoreFoundation")
    endif()
endif()

target_link_libraries(puredata-core PUBLIC ${PD_NEEDED_LIBS} ${PLATFORM_LINK_LIBRARIES})
target_link_libraries(pd PUBLIC puredata-core ${PD_NEEDED_LIBS})

if(UNIX AND NOT APPLE)
    add_executable(pd-watchdog s_watchdog.c)
    target_link_libraries(pd-watchdog ${PLATFORM_LINK_LIBRARIES})
endif()

add_executable(pdsend u_pdsend.c s_net.c)
target_link_libraries(pdsend ${PLATFORM_LINK_LIBRARIES})
add_executable(pdreceive u_pdreceive.c s_net.c)
target_link_libraries(pdreceive ${PLATFORM_LINK_LIBRARIES})

# exe install
install(TARGETS pd         RUNTIME DESTINATION ${PD_EXE_INSTALL_PATH})
install(TARGETS pdsend     RUNTIME DESTINATION ${PD_SEND_INSTALL_PATH})
install(TARGETS pdreceive  RUNTIME DESTINATION ${PD_RECEIVE_INSTALL_PATH})

if(UNIX AND NOT APPLE)
    install(TARGETS pd-watchdog RUNTIME DESTINATION ${PD_WATCHDOG_INSTALL_PATH})

    # create symlinks in bin
    #install_symlink("${CMAKE_INSTALL_PREFIX}/${PD_EXE_INSTALL_PATH}/pd" "${CMAKE_INSTALL_PREFIX}/bin/pd-ceammc")
    #install_symlink("${CMAKE_INSTALL_PREFIX}/${PD_SEND_INSTALL_PATH}/pdsend" "${CMAKE_INSTALL_PREFIX}/bin/pdsend-ceammc")
    #install_symlink("${CMAKE_INSTALL_PREFIX}/${PD_RECEIVE_INSTALL_PATH}/pdreceive" "${CMAKE_INSTALL_PREFIX}/bin/pdreceive-ceammc")
endif()

# install libs
install(TARGETS puredata-core
    RUNTIME DESTINATION ${PD_EXE_INSTALL_PATH}
    LIBRARY DESTINATION ${PDCORE_LIB_INSTALL_PATH}
    ARCHIVE DESTINATION ${PDCORE_LIB_INSTALL_PATH})


# install headers
install(FILES m_pd.h DESTINATION ${PD_INCLUDE_INSTALL_PATH})
