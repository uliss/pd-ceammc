include(faust)
include(ceammc_pd.cmake)
include(cppcheck.cmake)

if(WITH_DOUBLE_PRECISION)
    add_definitions(-DFAUSTFLOAT=double)
endif()

# do no use pd macroses in our code
add_definitions(-DPD_CLASS_DEF)
# needed for math constants in <math.h>: M_PI etc.
add_definitions(-D_USE_MATH_DEFINES)

set(CEAMMC_COMPILER_WARNING_FLAGS "-Wall -Wextra")
if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CEAMMC_COMPILER_WARNING_FLAGS "${CEAMMC_COMPILER_WARNING_FLAGS} -Wno-unused-function -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wduplicate-enum -Wextra-semi -Wfloat-equal -Wlong-long -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo")
endif()

# debug warning flags
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND NOT $ENV{TRAVIS})
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CEAMMC_COMPILER_WARNING_FLAGS}")
endif()

if(WITH_COVERAGE)
    if(APPLE)
        add_compile_options(-fprofile-arcs)
        add_compile_options(-ftest-coverage)
    else()
        add_compile_options(--coverage)
        add_link_options(--coverage)
    endif()
endif()

if(WIN32 OR MSYS OR MINGW)
    list(APPEND CEAMMC_EXT_LINK_LIBRARIES puredata-core)
endif()

if(APPLE)
    # search first for homebrew bison v3.***
    list(APPEND CMAKE_PREFIX_PATH /usr/local/opt/bison /usr/local/opt/flex /usr/local)
endif()

find_package(BISON)
find_package(FLEX)

set(CEAMMC_EXT_LINK_LIBRARIES
    ceammc_analyze
    ceammc_array
    ceammc_base
    ceammc_conv
    ceammc_data
    ceammc_dyn
    ceammc_env
    ceammc_flow
    ceammc_flt
    ceammc_fx
    ceammc_hw
    ceammc_lang
    ceammc_lfo
    ceammc_list
    ceammc_math
    ceammc_midi
    ceammc_misc
    ceammc_music
    ceammc_net
    ceammc_noise
    ceammc_osc
    ceammc_path
    ceammc_pieces
    ceammc_preset
    ceammc_proto
    ceammc_random
    ceammc_samp
    ceammc_spat
    ceammc_string
    ceammc_synth
    ceammc_system
    ceammc_tl
    ceammc_ui
    ceammc_window
    ceammc_core
)

add_subdirectory(lib)
include_directories(lib)
include_directories(${CMAKE_SOURCE_DIR}/src)

remove_definitions(-DPD_INTERNAL)
add_definitions(-DPD)

add_subdirectory(an)
add_subdirectory(array)
add_subdirectory(base)
add_subdirectory(conv)
add_subdirectory(data)
add_subdirectory(dyn)
add_subdirectory(env)
#add_subdirectory(exp)
add_subdirectory(flow)
add_subdirectory(flt)
add_subdirectory(fx)
add_subdirectory(hw)
add_subdirectory(lang)
add_subdirectory(lfo)
add_subdirectory(list)
add_subdirectory(math)
add_subdirectory(midi)
add_subdirectory(misc)
add_subdirectory(music)
add_subdirectory(net)
add_subdirectory(noise)
add_subdirectory(osc)
add_subdirectory(path)
add_subdirectory(pieces)
add_subdirectory(preset)
add_subdirectory(proto)
add_subdirectory(random)
add_subdirectory(samp)
add_subdirectory(spat)
add_subdirectory(string)
add_subdirectory(synth)
add_subdirectory(system)
add_subdirectory(tl)
add_subdirectory(ui)
add_subdirectory(window)

add_custom_target(dev_touch_version
    COMMAND
        ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_SOURCE_DIR}/ceammc_lib_external.cpp"
)

pd_add_external(NAME ceammc
    FILES
        ceammc_lib_external.cpp
        ceammc_lib_listmethods.cpp
        ceammc_lib_listprops.cpp
        ceammc_lib_dumpjson.cpp
        mod_init.cpp
    INTERNAL True
    LIBRARY ceammc
    LINK ${CEAMMC_EXT_LINK_LIBRARIES})

target_include_directories(ceammc
    PRIVATE
        ${PROJECT_BINARY_DIR}/ceammc # for ceammc_config.h
)

if(WIN32)
    target_link_options(ceammc PRIVATE -Wl,--exclude-all-symbols)
    target_link_libraries(ceammc PRIVATE soxr)
endif()

add_custom_target(ceammc_lib_src SOURCES gen_lib.sh)

if(WITH_RUST_CORE)
    target_compile_definitions(ceammc PRIVATE WITH_RUST_CORE)
    target_include_directories(ceammc PRIVATE ${PROJECT_SOURCE_DIR}/ceammc/extra/rust/core)
endif()
