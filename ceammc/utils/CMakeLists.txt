set(TCL_LEXER_GEN_FILE ${CMAKE_CURRENT_BINARY_DIR}/tcl2cxx_lexer.cpp)

add_custom_command(
    OUTPUT     tcl2cxx_lexer.cpp
    COMMAND    $<TARGET_FILE:reflex-bin> ${CMAKE_CURRENT_SOURCE_DIR}/tcl2cxx.l
    DEPENDS    tcl2cxx.l
    VERBATIM
)

add_executable(tcl2cxx tcl2cxx.cpp ${TCL_LEXER_GEN_FILE})
target_include_directories(tcl2cxx
    PRIVATE
        $<TARGET_PROPERTY:reflex,INCLUDE_DIRECTORIES>
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_BINARY_DIR}
)

target_link_libraries(tcl2cxx PUBLIC reflex)

