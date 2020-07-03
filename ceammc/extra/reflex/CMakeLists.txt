set(REFLEX_SOURCES
    RE-flex/lib/convert.cpp
    RE-flex/lib/debug.cpp
    RE-flex/lib/error.cpp
    RE-flex/lib/input.cpp
    RE-flex/lib/matcher.cpp
    RE-flex/lib/pattern.cpp
    RE-flex/lib/posix.cpp
    RE-flex/lib/unicode.cpp
    RE-flex/lib/utf8.cpp
    RE-flex/unicode/block_scripts.cpp
    RE-flex/unicode/language_scripts.cpp
    RE-flex/unicode/letter_scripts.cpp
)

add_library(reflex STATIC ${REFLEX_SOURCES})
target_include_directories(reflex
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/RE-flex/include)
add_executable(reflex-bin RE-flex/src/reflex.cpp)
target_link_libraries(reflex-bin reflex)

if(UNIX)
    target_compile_options(reflex PRIVATE "-fPIC")
endif()
