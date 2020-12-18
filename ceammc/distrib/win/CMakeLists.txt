if(WINE_CROSSCOMPILE)
    set(INSTALLED_BIN_DIR "Z:/${CMAKE_INSTALL_PREFIX}")
else()
    set(INSTALLED_BIN_DIR "${CMAKE_INSTALL_PREFIX}")
endif()

if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CEAMMC_DISTRIB_ARCH "64-bit")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(CEAMMC_DISTRIB_ARCH "32-bit")
    endif()

    if(WITH_DOUBLE_PRECISION)
        set(CEAMMC_PRECISION "double")
        set(CEAMMC_APP_SUFFIX " (double)")
    else()
        set(CEAMMC_PRECISION "float")
        set(CEAMMC_APP_SUFFIX "")
    endif()

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pd.iss ${CMAKE_BINARY_DIR})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fix_dll_deps.sh ${CMAKE_BINARY_DIR} @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/win_upload_to_github.sh ${CMAKE_BINARY_DIR} @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/win_fix_pddoc_key_modifiers.sh ${CMAKE_BINARY_DIR} @ONLY)

    add_custom_target(install0
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ceammc_core> "${CMAKE_INSTALL_PREFIX}/${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ceammc> "${CMAKE_INSTALL_PREFIX}/${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc")
    add_dependencies(install0 ceammc ceammc_core)

    add_custom_target(run0 COMMAND "${CMAKE_INSTALL_PREFIX}/${PD_EXE_INSTALL_PATH}/$<TARGET_FILE_NAME:pd>")
    add_dependencies(run0 install0)
    add_custom_target(run1 COMMAND "${CMAKE_INSTALL_PREFIX}/${PD_EXE_INSTALL_PATH}/$<TARGET_FILE_NAME:pd>" -d 1)
    add_dependencies(run1 install0)
    add_custom_target(run2 COMMAND "${CMAKE_INSTALL_PREFIX}/${PD_EXE_INSTALL_PATH}/$<TARGET_FILE_NAME:pd>" -d 2)
    add_dependencies(run2 install0)
    add_custom_target(run3 COMMAND "${CMAKE_INSTALL_PREFIX}/${PD_EXE_INSTALL_PATH}/$<TARGET_FILE_NAME:pd>" -d 3)
    add_dependencies(run3 install0)
endif()


if(WIN32 OR MSYS OR MINGW)
    install(FILES fonts/DejaVuSansMono.ttf
        DESTINATION ${PD_FONT_INSTALL_PATH})
endif()

find_program(ISCC_PATH
    NAMES ISCC.exe
    PATHS "$ENV{PROGRAMFILES}/Inno Setup 5"
          "C:/Program Files (x86)/Inno Setup 5"
          "$ENV{HOME}/.wine/drive_c/Program Files/Inno Setup 5"
          "$ENV{PROGRAMFILES}/Inno Setup 6"
          "C:/Program Files (x86)/Inno Setup 6")


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

add_custom_target(window_installer SOURCES
    fix_dll_deps.sh
    make_lib_win.sh
    win_upload_to_github.sh
    win_fix_pddoc_key_modifiers.sh
    pd.iss
)
