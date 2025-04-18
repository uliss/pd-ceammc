find_program(FAUST_BIN faust
    PATHS
        $ENV{HOME}/.local/faust/bin
        /usr/bin
    NO_DEFAULT_PATH
)

if(FAUST_BIN)
    message(STATUS "Faust DSP compiler found: ${FAUST_BIN}")
else()
    message(STATUS "Faust DSP compiler not found")
endif()
