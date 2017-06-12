include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(utf8rewind STATIC
    utf8rewind.c
    unicodedatabase.c
    internal/casemapping.c
    internal/codepoint.c
    internal/composition.c
    internal/database.c
    internal/decomposition.c
    internal/seeking.c
    internal/streaming.c
)
