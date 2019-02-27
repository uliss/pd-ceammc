if(WINE_CROSSCOMPILE)
    set(INSTALLED_BIN_DIR "Z:/${CMAKE_INSTALL_PREFIX}")
else()
    set(INSTALLED_BIN_DIR "${CMAKE_INSTALL_PREFIX}")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pd.iss ${CMAKE_BINARY_DIR})

if(WIN32 OR MSYS OR MINGW)
    install(FILES fonts/DejaVuSansMono.ttf
        DESTINATION ${PD_FONT_INSTALL_PATH})
endif()

find_program(ISCC_PATH
    NAMES ISCC.exe
    PATHS "$ENV{PROGRAMFILES}/Inno Setup 5"
        "$ENV{PROGRAMFILES(X86)}/Inno Setup 5"
          "$ENV{HOME}/.wine/drive_c/Program Files/Inno Setup 5")


if(ISCC_PATH)
    message(STATUS "Inno Setup Installed found: ${ISCC_PATH}")
    message(STATUS "     You could type `make inno` after `make install` to build installer package")

    if(WINE_CROSSCOMPILE)
        add_custom_target(inno
            COMMAND wine ${ISCC_PATH} "${CMAKE_BINARY_DIR}/pd.iss"
        )
    else()
        add_custom_target(inno
            COMMAND ${ISCC_PATH} "${CMAKE_BINARY_DIR}/pd.iss"
        )
    endif()
endif()

add_subdirectory(homebrew)

if(WIN32)
    install(DIRECTORY fonts DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

add_custom_target(window_installer SOURCES pd.iss)
