include(library_pddoc.cmake)
include(CheckIncludeFileCXX)


add_subdirectory(SoundTouch)
add_subdirectory(aubio)
add_subdirectory(comport)
add_subdirectory(fftconv)
add_subdirectory(helmholtz)
add_subdirectory(airwindows)
add_subdirectory(colorm)
add_subdirectory(date)

if(WITH_EXT_AUTOTUNE)
    add_subdirectory(autotune)
    install(FILES autotune_objects.txt DESTINATION ${PD_COMPLETION_PATH})
endif()

if(WITH_EXT_FLEXT)
    add_subdirectory(flext)
endif()

if(WITH_EXT_FFTEASE)
    add_subdirectory(fftease)
    install(FILES fftease_objects.txt DESTINATION ${PD_COMPLETION_PATH})
endif()

if(WITH_EXT_LYONPOTPOURRI)
    add_subdirectory(lyonpotpourri)
    install(FILES lyonpotpourri_objects.txt DESTINATION  ${PD_COMPLETION_PATH})
endif()

if(WITH_ARTNET)
    add_subdirectory(artnet)
endif()

add_subdirectory(glitch)
add_subdirectory(httplib)
add_subdirectory(midifile)
add_subdirectory(miniz)
add_subdirectory(muparser)
add_subdirectory(qrcode)
add_subdirectory(readerwriterqueue)
add_subdirectory(reflex)
add_subdirectory(rust)
add_subdirectory(serial)
add_subdirectory(sf2)
add_subdirectory(soxr)
add_subdirectory(stk)
add_subdirectory(subprocess)
add_subdirectory(tcltk)
add_subdirectory(verovio)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_BUILD_TYPE Release)
set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(fmt)
set_target_properties(fmt PROPERTIES DEBUG_POSTFIX "")

option(RE2_BUILD_TESTING "" OFF)
set(RE2_BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(re2 EXCLUDE_FROM_ALL)

option(HOA_BUILD_TESTS "" OFF)
set(HOA_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(HoaLibrary-Light)

if(UNIX)
    set_target_properties(re2 PROPERTIES COMPILE_FLAGS "-fPIC")
    set_target_properties(fmt PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

if(WITH_EXPERIMENTAL)
    add_subdirectory(import)
    add_subdirectory(libdir)
    add_subdirectory(libusb)

    install(
        FILES
            import_objects.txt
            libdir_objects.txt
        DESTINATION
            ${PD_COMPLETION_PATH}
    )
endif()

if(WITH_FLUIDSYNTH)
    find_package(GLIB REQUIRED)
    if(GLIB_FOUND)
        #include paths
        list(APPEND FLUIDSYNTH_INCLUDES
            ${GLIB_INCLUDES}
            ${PROJECT_BINARY_DIR}/ceammc/extra/fluidsynth
            ${PROJECT_SOURCE_DIR}/ceammc/extra/fluidsynth/fluidsynth/include)
        # libs
        list(APPEND FLUIDSYNTH_LIBRARIES fluidsynth ${GLIB_LIBRARIES})
        add_subdirectory(fluidsynth)
    endif()
endif()

if(WITH_TTS_FLITE)
    add_subdirectory(speech)
endif()

add_custom_target(_ceammc_extra_misc
    SOURCES
        autotune_objects.txt
        extra_objects.txt
        fftease_objects.txt
        import_objects.txt
        libdir_objects.txt
)

if(WITH_SFIZZ)
    cmake_minimum_required(VERSION 3.14)

    include(FetchContent)
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
        cmake_policy(SET CMP0135 NEW)
    endif()
    set(FETCHCONTENT_QUIET FALSE)
    set(SFIZZ_AU OFF CACHE INTERNAL "")
    set(SFIZZ_LV2 OFF CACHE INTERNAL "")
    set(SFIZZ_LV2_UI OFF CACHE INTERNAL "")
    set(SFIZZ_VST OFF CACHE INTERNAL "")
    set(SFIZZ_JACK OFF CACHE INTERNAL "")
    set(SFIZZ_RENDER OFF CACHE INTERNAL "")
    set(ABSL_PROPAGATE_CXX_STD ON CACHE INTERNAL "")

    if(WIN32)
        set(SFIZZ_SHARED ON CACHE INTERNAL "")
    else()
        set(SFIZZ_SHARED OFF CACHE INTERNAL "")
    endif()

    set(SFIZZ_USE_SNDFILE OFF CACHE INTERNAL "")
    if (UNIX AND NOT APPLE)
    	FetchContent_Declare(
		sfizz
        	URL      https://github.com/sfztools/sfizz/releases/download/1.2.0/sfizz-1.2.0.tar.gz
        	URL_HASH MD5=13725a41a811dc81c973f303eaaea86b
		)
    else()
    	FetchContent_Declare(
        	sfizz
        	URL      https://github.com/sfztools/sfizz/releases/download/1.2.0/sfizz-1.2.0.tar.gz
        	URL_HASH MD5=13725a41a811dc81c973f303eaaea86b
        	DOWNLOAD_EXTRACT_TIMESTAMP ON
    	)
    endif()
    FetchContent_GetProperties(sfizz)
    if(NOT sfizz_POPULATED)
        FetchContent_Populate(sfizz)
        add_subdirectory(${sfizz_SOURCE_DIR} ${sfizz_BINARY_DIR} EXCLUDE_FROM_ALL)

        if(WIN32)
            install(TARGETS sfizz_shared RUNTIME DESTINATION bin)
        endif()
    endif()
endif()

if(WITH_FAUST)
    add_subdirectory(faust)
endif()
