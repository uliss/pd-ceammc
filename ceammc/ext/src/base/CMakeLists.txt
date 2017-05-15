include_directories(${CMAKE_BINARY_DIR}) # for config.h

set(BASE_SOURCES
    expand_env.cpp
    function.cpp
    function_call.cpp
    msg.cpp
    prop.get
    prop.set
    replace.cpp)

# predicates
list(APPEND BASE_SOURCES
    is_any.cpp
    is_bang.cpp
    is_even.cpp
    is_float.cpp
    is_file.cpp
    is_list.cpp
    is_odd.cpp
    is_pointer.cpp
    is_symbol.cpp)

# test
list(APPEND BASE_SOURCES
    test_data.cpp
    test_expect.cpp)

if(TARGET ceammc_sound)
    add_definitions(-DWITH_SND_FILE)
    list(APPEND BASE_SOURCES snd_file.cpp)
endif()

add_library(ceammc_base STATIC mod_base.cpp ${BASE_SOURCES})
if(UNIX)
    set_target_properties(ceammc_base PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

# separate extension:
# becaust it use CEAMMC Pd API
ceammc_cxx_extension(debug gensym)
