include(TestBigEndian)
include(FindLibDL)
include(windres)
test_big_endian(IS_BIG_ENDIAN)
if(NOT ${IS_BIG_ENDIAN})
    add_definitions(-DLITTLE_ENDIAN=0x0001 -DBYTE_ORDER=LITTLE_ENDIAN)
endif()

set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads)

set(PLATFORM_LINK_LIBRARIES)


if(UNIX AND NOT APPLE)
    add_definitions(-D_GNU_SOURCE)
    list(APPEND PLATFORM_LINK_LIBRARIES m)
    list(APPEND PLATFORM_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
    set(LINUX True)
endif()


if(WIN32)
    include(FindWindowsSDK)

    if (WINDOWSSDK_FOUND)
        get_windowssdk_library_dirs(${WINDOWSSDK_PREFERRED_DIR} WINSDK_LIB_DIRS)
        get_windowssdk_include_dirs(${WINDOWSSDK_PREFERRED_DIR} WINSDK_INCLUDE_DIRS)

        link_directories(${WINSDK_LIB_DIRS})
    endif()

    find_program(WISH_PATH
        NAMES wish86.exe wish85.exe wish.exe wish86t.exe
        PATHS C:/Tcl/bin)

    if(NOT WISH_PATH)
        message(FATAL_ERROR "wish.exe not found")
    else()
        message(STATUS "wish found: ${WISH_PATH}")
        get_filename_component(WISHAPP ${WISH_PATH} NAME)
        get_filename_component(WISH_BINDIR ${WISH_PATH} DIRECTORY)
    endif()
    
    # install wish.exe to bin directory
    install(PROGRAMS ${WISH_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    include(InstallRequiredSystemLibraries)

    # install tcl.dll
    find_file(TCLDLL_PATH NAMES tcl86.dll tcl85.dll PATHS ${WISH_BINDIR})
    if(TCLDLL_PATH)
        install(PROGRAMS ${TCLDLL_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # install tk.dll
    find_file(TKDLL_PATH NAMES tk86.dll tk85.dll PATHS ${WISH_BINDIR})
    if(TKDLL_PATH)
        install(PROGRAMS ${TKDLL_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # install zlib.dll
    find_file(ZLIBDLL_PATH NAMES zlib1.dll PATHS ${WISH_BINDIR})
    if(ZLIBDLL_PATH)
        install(PROGRAMS ${ZLIBDLL_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # install tcl libs
    install(DIRECTORY "${WISH_BINDIR}/../lib/tcl8" DESTINATION "${PD_LIBTCL_INSTALL_PATH}")
    install(DIRECTORY "${WISH_BINDIR}/../lib/tcl8.6" DESTINATION "${PD_LIBTCL_INSTALL_PATH}")
    install(DIRECTORY "${WISH_BINDIR}/../lib/tk8.6" DESTINATION "${PD_LIBTCL_INSTALL_PATH}")
    install(DIRECTORY "${WISH_BINDIR}/../lib/dde1.4" DESTINATION "${PD_LIBTCL_INSTALL_PATH}")
    install(DIRECTORY "${WISH_BINDIR}/../lib/tcllib1.18" DESTINATION "${PD_LIBTCL_INSTALL_PATH}")
    install(DIRECTORY "${WISH_BINDIR}/../lib/tklib0.5" DESTINATION "${PD_LIBTCL_INSTALL_PATH}")
    install(DIRECTORY "${WISH_BINDIR}/../lib/reg1.3" DESTINATION "${PD_LIBTCL_INSTALL_PATH}")
    
    # pthreadGC-3.dll
    find_file(PTHREADGC_DLL
        NAMES pthreadGC-3.dll pthreadGC-2.dll winpthread-1.dll libwinpthread-1.dll
        PATHS /mingw/bin /mingw/i686-w64-mingw32/bin)
    if(PTHREADGC_DLL)
        message(STATUS "found thread lib: ${PTHREADGC_DLL}")
        install(FILES ${PTHREADGC_DLL} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # install libsndfile-1.dll
    find_file(SNDFILEDLL_PATH NAMES libsndfile-1.dll PATHS ${WISH_BINDIR})
    if(SNDFILEDLL_PATH)
        install(PROGRAMS ${SNDFILEDLL_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # install libFLAC-8.dll
    find_file(FLACDLL_PATH NAMES libFLAC-8.dll PATHS ${WISH_BINDIR})
    if(FLACDLL_PATH)
        install(PROGRAMS ${FLACDLL_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # install libogg-0.dll
    find_file(OGGDLL_PATH NAMES libogg-0.dll PATHS ${WISH_BINDIR})
    if(OGGDLL_PATH)
        install(PROGRAMS ${OGGDLL_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # install libvorbis-0.dll
    find_file(VORBISDLL_PATH NAMES libvorbis-0.dll PATHS ${WISH_BINDIR})
    if(VORBISDLL_PATH)
        install(PROGRAMS ${VORBISDLL_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # install libvorbisenc-2.dll
    find_file(VORBISENCDLL_PATH NAMES libvorbisenc-2.dll PATHS ${WISH_BINDIR})
    if(VORBISENCDLL_PATH)
        install(PROGRAMS ${VORBISENCDLL_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # mingw runtime libs
    get_filename_component(_MINGW_PATH ${CMAKE_CXX_COMPILER} PATH)
    set(_MINGW_BIN ${_MINGW_PATH}/../i686-w64-mingw32/bin)
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${_MINGW_BIN}/libgcc_s_dw2-1.dll ${_MINGW_BIN}/libstdc++-6.dll )
    include(InstallRequiredSystemLibraries)
    
    # libportaudio-2.dll
    find_file(LIBPORTAUDIO_DLL NAMES libportaudio-2.dll PATHS /usr/bin /usr/local/bin)
    if(LIBPORTAUDIO_DLL)
        install(FILES ${LIBPORTAUDIO_DLL} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()
    
    # install README, LICENSE.txt to root 
    install(FILES 
            ${CMAKE_SOURCE_DIR}/README.txt
            ${CMAKE_SOURCE_DIR}/LICENSE.txt
        DESTINATION
            ${CMAKE_INSTALL_PREFIX})
    
    if(${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore")
        message("Building for UWP")
        set(FIPS_UWP 1)
    else()
        set(FIPS_UWP 0)
    endif()

    if (FIPS_UWP)
        set(CMAKE_CXX_STANDARD_LIBRARIES "WindowsApp.lib")
        set(CMAKE_C_STANDARD_LIBRARIES "WindowsApp.lib")
    else()
        set(CMAKE_CXX_STANDARD_LIBRARIES "-lkernel32 -lgdi32 -lole32 -lwinmm -luuid -lwsock32 -lws2_32")
        set(CMAKE_C_STANDARD_LIBRARIES ${CMAKE_CXX_STANDARD_LIBRARIES})
    endif()

    add_definitions(-DPD_INTERNAL -DWINVER=0x0502 -D_WIN32_WINNT=0x0502)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mms-bitfields -O2 -funroll-loops -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS "-mms-bitfields -O2 -funroll-loops -fomit-frame-pointer")
    list(APPEND PLATFORM_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols -lpthread")
    set(CMAKE_EXE_LINKER_FLAGS "-shared-libgcc -lpthread")
endif()

if(APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -funroll-loops -fomit-frame-pointer")

    set(BUNDLE "Pd-${PD_MACOSX_BUNDLE_SUFFIX}.app")
    set(BUNDLE_FULL_PATH "${PROJECT_BINARY_DIR}/dist/${BUNDLE}")
    set(MAKE_BUNDLE_SCRIPT ${PROJECT_BINARY_DIR}/dist/build_mac.sh)

    # copy and substitute variables to Info.plist
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/dist)
    configure_file(${PROJECT_SOURCE_DIR}/ceammc/gui/Info.plist ${PROJECT_BINARY_DIR}/dist/Info.plist)

    set(CUSTOM_TCL "/Library/Frameworks/Tcl.framework")
    set(CUSTOM_TK  "/Library/Frameworks/Tk.framework")
    set(CUSTOM_WISH "${CUSTOM_TK}/Versions/8.6/Resources/Wish.app")

    set(SYSTEM_TCL     "/System/Library/Frameworks/Tcl.framework")
    set(SYSTEM_TK      "/System/Library/Frameworks/Tk.framework")
    set(SYSTEM_WISH_V5 "${SYSTEM_TK}/Versions/8.5/Resources/Wish.app")
    set(SYSTEM_WISH_V4 "${SYSTEM_TK}/Versions/8.4/Resources/Wish.app")

    set(IS_SYSTEM_TK 0)
    set(TK_VERSION "")

    if(EXISTS ${CUSTOM_TCL} AND EXISTS ${CUSTOM_TK} AND EXISTS ${CUSTOM_WISH})
        set(TCL_PATH "${CUSTOM_TCL}")
        set(TK_PATH  "${CUSTOM_TK}")
        set(WISH_APP "${CUSTOM_WISH}")
        set(TK_VERSION "8.6")
    elseif(EXISTS ${SYSTEM_TCL} AND EXISTS ${SYSTEM_TK})
        set(TCL_PATH "${SYSTEM_TCL}")
        set(TK_PATH  "${SYSTEM_TK}")

        if(EXISTS ${SYSTEM_WISH_V5})
            set(WISH_APP "${SYSTEM_WISH_V5}")
            set(TK_VERSION "8.5")
        elseif(EXISTS ${SYSTEM_WISH_V4})
            set(WISH_APP "${SYSTEM_WISH_V4}")
            set(TK_VERSION "8.4")
        else()
            message(FATAL_ERROR "Wish.app not found in \"${SYSTEM_TK}\"")
        endif()
        set(IS_SYSTEM_TK 1)
    else()
        message(FATAL_ERROR "TCL not found")
    endif()

    message(STATUS "found Tcl: ${TCL_PATH}")
    message(STATUS "found Tk:  ${TK_PATH}")
    message(STATUS "found Wish.app: ${WISH_APP}")
    message(STATUS "found Tcl/Tk version: ${TK_VERSION}")

    add_custom_command(
        OUTPUT ${MAKE_BUNDLE_SCRIPT}
        COMMAND ${CMAKE_COMMAND}
            -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
            -DPROJECT_BINARY_DIR="${PROJECT_BINARY_DIR}"
            -DBUNDLE=${BUNDLE_FULL_PATH}
            -DWISH_APP=${WISH_APP}
            -DTCL_PATH=${TCL_PATH}
            -DDYLIBBUNDLER="${CMAKE_BINARY_DIR}/ceammc/distrib/mac/dylibbundler"
            -DTK_PATH=${TK_PATH}
            -DIS_SYSTEM_TK=${IS_SYSTEM_TK}
            -DTK_VERSION=${TK_VERSION}
            -P ${PROJECT_SOURCE_DIR}/cmake/cmake-build-mac.cmake)

    add_custom_command(
        OUTPUT ${BUNDLE_FULL_PATH}
        COMMAND sh ${PROJECT_BINARY_DIR}/dist/build_mac.sh
        COMMAND ${CMAKE_COMMAND}
            -DBUNDLE=${BUNDLE_FULL_PATH}
            -P ${PROJECT_SOURCE_DIR}/cmake/bundle.cmake
        DEPENDS pd)

    # app target
    # `make app` creates MacOSX bundle
    add_custom_target(app DEPENDS ${MAKE_BUNDLE_SCRIPT} ${BUNDLE_FULL_PATH})

    add_custom_target(codesign
        COMMAND sh ${PROJECT_SOURCE_DIR}/mac/codesign.sh ${BUNDLE_FULL_PATH}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/mac)

    add_custom_target(dmg
        COMMAND
            sh ${PROJECT_SOURCE_DIR}/mac/dmg.sh
            ${BUNDLE_FULL_PATH}
            "${PD_MACOSX_BUNDLE_SUFFIX}-macosx-${MACOSX_VERSION}"
            ${PROJECT_SOURCE_DIR}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS app)
endif()
