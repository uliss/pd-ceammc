find_program(FAUST_BIN faust
    PATHS /usr/bin  /usr/local/bin)

if(FAUST_BIN)
    message(STATUS "Faust DSP compiler found: ${FAUST_BIN}")
else()
    message(STATUS "Faust DSP compiler not found")
endif()
