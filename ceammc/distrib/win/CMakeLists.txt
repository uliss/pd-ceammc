configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pd.iss ${CMAKE_BINARY_DIR})

find_program(ISCC_PATH ISCC.exe
    PATHS "$ENV{PROGRAMFILES}/Inno Setup 5")
    
    
if(ISCC_PATH)
    message(STATUS "Inno Setup Installed found: ${ISCC_PATH}")
    message(STATUS "     You could type `make inno` after `make install` to build installer package")
    
    add_custom_target(inno
        COMMAND ${ISCC_PATH} "${CMAKE_BINARY_DIR}/pd.iss"
    )
endif()