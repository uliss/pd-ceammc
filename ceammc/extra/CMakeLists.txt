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
endif()

if(WITH_EXT_FLEXT)
    add_subdirectory(flext)
endif()

if(WITH_EXT_FFTEASE)
    add_subdirectory(fftease)
endif()

if(WITH_EXT_LYONPOTPOURRI)
    add_subdirectory(lyonpotpourri)
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

set(EXTRA_COMPLETION "extra_objects.txt")
add_custom_target(_ceammc_extra_misc SOURCES ${EXTRA_COMPLETION})

if(WITH_SFIZZ)
    cmake_minimum_required(VERSION 3.14)

    include(FetchContent)
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

    FetchContent_Declare(
        sfizz
        URL      https://github.com/sfztools/sfizz/releases/download/1.2.0/sfizz-1.2.0.tar.gz
        URL_HASH MD5=13725a41a811dc81c973f303eaaea86b
    )
    FetchContent_GetProperties(sfizz)
    if(NOT sfizz_POPULATED)
        FetchContent_Populate(sfizz)
        add_subdirectory(${sfizz_SOURCE_DIR} ${sfizz_BINARY_DIR} EXCLUDE_FROM_ALL)

        if(WIN32)
            #install(TARGETS sfizz_shared RUNTIME DESTINATION extra/ceammc)
        endif()
    endif()
endif()

if(WITH_FAUST)
    if(APPLE)
        add_subdirectory(faust)
    else()
        cmake_minimum_required(VERSION 3.14)

        include(FetchContent)
        set(FETCHCONTENT_QUIET FALSE)
        set(INCLUDE_STATIC      ON  CACHE INTERNAL "")
        set(INCLUDE_EXECUTABLE  OFF CACHE INTERNAL "")
        set(INCLUDE_DYNAMIC     OFF CACHE INTERNAL "")
        set(INCLUDE_OSC         OFF CACHE INTERNAL "")
        set(INCLUDE_HTTP        OFF CACHE INTERNAL "")
        set(INCLUDE_EMCC        OFF CACHE INTERNAL "")
        set(INCLUDE_WASM_GLUE   OFF CACHE INTERNAL "")
        set(INCLUDE_LLVM        OFF CACHE INTERNAL "")

        ## Hardcoded backends for faust
        set(ASMJS_BACKEND  OFF            CACHE INTERNAL "")
        set(C_BACKEND      OFF            CACHE INTERNAL "")
        set(CPP_BACKEND    OFF            CACHE INTERNAL "")
        set(FIR_BACKEND    OFF            CACHE INTERNAL "")
        set(INTERP_BACKEND OFF            CACHE INTERNAL "")
        set(JAVA_BACKEND   OFF            CACHE INTERNAL "")
        set(JS_BACKEND     OFF            CACHE INTERNAL "")
        set(LLVM_BACKEND   STATIC DYNAMIC CACHE INTERNAL "")
        set(OLDCPP_BACKEND OFF            CACHE INTERNAL "")
        set(RUST_BACKEND   OFF            CACHE INTERNAL "")
        set(WASM_BACKEND   OFF            CACHE INTERNAL "")

        find_package(Patch REQUIRED)

        FetchContent_Declare(
            faust
            URL      https://github.com/grame-cncm/faust/releases/download/2.60.3/faust-2.60.3.tar.gz
            URL_HASH MD5=d0c436cebafbce0b5affc082bc2f8297
            PATCH_COMMAND ${Patch_EXECUTABLE} -i "${CMAKE_CURRENT_SOURCE_DIR}/faust/cstdint.patch" -l -N -u --verbose ${PROJECT_BINARY_DIR}/_deps/faust-src/compiler/generator/Text.hh
            UPDATE_DISCONNECTED 1
        )
        FetchContent_GetProperties(faust)
        if(NOT faust_POPULATED)
            FetchContent_Populate(faust)
            file(REMOVE ${faust_SOURCE_DIR}/compiler/libcode.cpp)
            add_subdirectory(${faust_SOURCE_DIR}/build ${faust_BINARY_DIR} EXCLUDE_FROM_ALL)
            target_compile_options(staticlib PRIVATE -Wno-unused-variable -Wno-cast-function-type -Wno-attributes)

            set(DOC_INSTALL_PATH "${PD_DOC_INSTALL_PATH}/extra/ceammc")

            install(DIRECTORY ${faust_SOURCE_DIR}/libraries/
                DESTINATION ${DOC_INSTALL_PATH}/faust
                FILES_MATCHING PATTERN "*.lib"
                PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

             message(STATUS "############## ${LLVM_LIB_DIR} ############")

        endif()

        # ceammc binding
        add_library(ceammc_faust STATIC ${PROJECT_SOURCE_DIR}/ceammc/extra/faust/ceammc_llvm.cpp)
        target_compile_definitions(ceammc_faust PRIVATE ${LLVM_DEFINITIONS})
        target_include_directories(ceammc_faust PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/faust/architecture
            ${faust_SOURCE_DIR}/architecture
            ${PROJECT_SOURCE_DIR}/src)
        target_link_libraries(ceammc_faust PUBLIC staticlib)
        target_link_directories(ceammc_faust PUBLIC ${LLVM_LIB_DIR})



        if(COMMAND llvm_map_components_to_libnames)
            set(llvm_libs)
            llvm_map_components_to_libnames(llvm_libs_base
                support core irreader executionengine
                asmprinter mcjit ipo objcarcopts)
    #        target_link_libraries(ceammc_faust PUBLIC ${llvm_libs_base})
            if(APPLE)
                if(CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
                    llvm_map_components_to_libnames(llvm_libs_arm64
                        AArch64AsmParser AArch64Info AArch64Desc AArch64CodeGen)
    #                target_link_libraries(ceammc_faust PUBLIC ${llvm_libs_arm64})
                    message(STATUS "llvm-libs: ${llvm_libs_base} ${llvm_libs_arm64}")
                else (CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64")
                    llvm_map_components_to_libnames(llvm_libs_x86
                        x86asmparser x86info x86desc x86codegen)
                    target_link_libraries(ceammc_faust PRIVATE ${llvm_libs_x86})
                endif()
            elseif(WIN32 OR UNIX)
                llvm_map_components_to_libnames(llvm_libs_x86
                    x86asmparser x86info x86desc x86codegen)
                list(APPEND llvm_libs ${llvm_libs_x86})
            endif()
        endif()
    endif()
endif()

