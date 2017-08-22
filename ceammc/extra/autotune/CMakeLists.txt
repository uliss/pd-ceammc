include(PdExtension)
include_directories(${PROJECT_SOURCE_DIR}/src)

pd_add_extension(NAME autotune~
    FILES autotune~.c
    INTERNAL True
    LIBRARY autotune
    HELP_FILES autotune~-help.pd
)
