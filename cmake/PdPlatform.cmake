include(TestBigEndian)
include(FindLibDL)
include(windres)

function(ceammc_remove_duplicate_substring stringIn stringOut)
    separate_arguments(stringIn)
    list(REMOVE_DUPLICATES stringIn)
    string(REPLACE ";" " " stringIn "${stringIn}")
    set(${stringOut} "${stringIn}" PARENT_SCOPE)
endfunction()


# big endian check
test_big_endian(IS_BIG_ENDIAN)
if(NOT ${IS_BIG_ENDIAN})
    add_definitions(-DLITTLE_ENDIAN=0x0001 -DBYTE_ORDER=LITTLE_ENDIAN)
endif()

set(CEAMMC_COMPILER_WARNING_FLAGS "-Wall -Wextra")
if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CEAMMC_COMPILER_WARNING_FLAGS "${CEAMMC_COMPILER_WARNING_FLAGS} -Wno-unused-function -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wduplicate-enum -Wextra-semi -Wfloat-equal -Wlong-long -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo")
endif()

# needed for math constants in <math.h>: M_PI etc.
add_definitions(-D_USE_MATH_DEFINES)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads)

set(PLATFORM_LINK_LIBRARIES)

if(UNIX AND NOT APPLE)
    add_definitions(-D_GNU_SOURCE)
    list(APPEND PLATFORM_LINK_LIBRARIES m)
    list(APPEND PLATFORM_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
    set(LINUX True)
endif()

# address sanitizer
if(WITH_ASAN)
    set (CEAMMC_ASAN "-fno-omit-frame-pointer -fsanitize=undefined -fsanitize=address")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CEAMMC_ASAN}")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} ${CEAMMC_ASAN}")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CEAMMC_ASAN}")
    set (CMAKE_LINKER_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CEAMMC_ASAN}")
    set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${CEAMMC_ASAN}")

    ceammc_remove_duplicate_substring(${CMAKE_CXX_FLAGS_DEBUG} CMAKE_CXX_FLAGS_DEBUG)
    ceammc_remove_duplicate_substring(${CMAKE_LINKER_FLAGS_DEBUG} CMAKE_LINKER_FLAGS_DEBUG)
    ceammc_remove_duplicate_substring(${CMAKE_CXX_FLAGS_RELEASE} CMAKE_CXX_FLAGS_RELEASE)
    ceammc_remove_duplicate_substring(${CMAKE_LINKER_FLAGS_RELEASE} CMAKE_LINKER_FLAGS_RELEASE)
    ceammc_remove_duplicate_substring(${CMAKE_LINKER_FLAGS} CMAKE_LINKER_FLAGS)
endif()

# address sanitizer
if(WITH_PROFILE)
    if(APPLE)
        set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fprofile-instr-generate -fcoverage-mapping")
        set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} -fprofile-instr-generate")
        set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -fprofile-instr-generate -fcoverage-mapping")
        set (CMAKE_LINKER_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-instr-generate")
        set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fprofile-instr-generate")

        ceammc_remove_duplicate_substring(${CMAKE_CXX_FLAGS_DEBUG} CMAKE_CXX_FLAGS_DEBUG)
        ceammc_remove_duplicate_substring(${CMAKE_LINKER_FLAGS_DEBUG} CMAKE_LINKER_FLAGS_DEBUG)
        ceammc_remove_duplicate_substring(${CMAKE_CXX_FLAGS_RELEASE} CMAKE_CXX_FLAGS_RELEASE)
        ceammc_remove_duplicate_substring(${CMAKE_LINKER_FLAGS_RELEASE} CMAKE_LINKER_FLAGS_RELEASE)
        ceammc_remove_duplicate_substring(${CMAKE_LINKER_FLAGS} CMAKE_LINKER_FLAGS)
    endif()
endif()

function(find_and_install_dll mask dir)
    file(GLOB _dll "${dir}/${mask}")
    if(_dll)
        message(STATUS "Found DLL: ${_dll}")
        install(PROGRAMS ${_dll} DESTINATION ${PD_EXE_INSTALL_PATH})
    else()
        message(STATUS "file is not found: ${dir}/${mask}")
    endif()
endfunction()

if(WIN32)
    find_program(WISH_PATH
        NAMES wish86.exe wish.exe
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
    find_file(TCLDLL_PATH NAMES tcl86.dll PATHS ${WISH_BINDIR})
    if(TCLDLL_PATH)
        install(PROGRAMS ${TCLDLL_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # install tk.dll
    find_file(TKDLL_PATH NAMES tk86.dll PATHS ${WISH_BINDIR})
    if(TKDLL_PATH)
        install(PROGRAMS ${TKDLL_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # install zlib.dll
    find_file(ZLIBDLL_PATH NAMES zlib1.dll PATHS ${WISH_BINDIR})
    if(ZLIBDLL_PATH)
        install(PROGRAMS ${ZLIBDLL_PATH} DESTINATION ${PD_EXE_INSTALL_PATH})
    endif()

    # install tcl/tk libs
    function(install_tcl_dir name)
        # get normalized path
        get_filename_component(_path "${WISH_BINDIR}/../lib/${name}" ABSOLUTE)
        if(EXISTS ${_path})
            message(STATUS "Found TCL package: ${name} at ${_path}")
            install(DIRECTORY "${_path}" DESTINATION "${PD_LIBTCL_INSTALL_PATH}")
        else()
            message(STATUS "TCL package not found: ${name}")
        endif()
    endfunction()

    install_tcl_dir(tcl8)
    install_tcl_dir(tcl8.6)
    install_tcl_dir(tk8.6)
    install_tcl_dir(dde1.4)
    install_tcl_dir(tcllib1.18)
    install_tcl_dir(tcllib1.19)
    install_tcl_dir(tcllib1.20)
    install_tcl_dir(tcllib1.21)
    # try different tooltip location
    install_tcl_dir(tklib0.6)
    install_tcl_dir(tklib0.6/tooltip)
    install_tcl_dir(tooltip)
    install_tcl_dir(reg1.3)

    include(InstallRequiredSystemLibraries)
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
        DESTINATION bin
        COMPONENT Libraries)

    # install README, LICENSE.txt to root
    install(FILES
            ${CMAKE_SOURCE_DIR}/README.txt
            ${CMAKE_SOURCE_DIR}/LICENSE.txt
        DESTINATION
            ${CMAKE_INSTALL_PREFIX})

    set(CMAKE_CXX_STANDARD_LIBRARIES "-lkernel32 -lgdi32 -lole32 -lwinmm -luuid -lwsock32 -lws2_32")
    set(CMAKE_C_STANDARD_LIBRARIES ${CMAKE_CXX_STANDARD_LIBRARIES})

    add_definitions(-DPD_INTERNAL -DWINVER=0x0502 -D_WIN32_WINNT=0x0502 -D_USE_MATH_DEFINES -D_WINDOWS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mms-bitfields")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -mms-bitfields")

    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mms-bitfields -O2 -funroll-loops -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -mms-bitfields -O2 -funroll-loops -fomit-frame-pointer")

    set(CMAKE_SHARED_LINKER_FLAGS "-lpthread")
    # set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols -lpthread")
    set(CMAKE_EXE_LINKER_FLAGS "-shared-libgcc -lpthread")

    list(APPEND PLATFORM_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

    # MSYS64 clang flags:
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-incompatible-ms-struct")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-incompatible-ms-struct")
    endif()

    add_custom_target(prepare_win_tests
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:puredata-core>" "${PROJECT_BINARY_DIR}/ceammc/ext/tests"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:ceammc_core>" "${PROJECT_BINARY_DIR}/ceammc/ext/tests"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:wrapper_lib>" "${PROJECT_BINARY_DIR}/ceammc/ext/tests"
    )
endif()

if(APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -funroll-loops")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -funroll-loops")

    set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG -ffast-math -funroll-loops -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -O2 -DNDEBUG -ffast-math -funroll-loops -fomit-frame-pointer")

    if(NOT WITH_ASAN AND NOT WITH_PROFILE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fomit-frame-pointer")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fomit-frame-pointer")
    endif()

    set(BUNDLE ${PD_MACOSX_APP})
    set(BUNDLE_FULL_PATH "${PROJECT_BINARY_DIR}/dist/${BUNDLE}")
    set(DMG_FULL_PATH "${PROJECT_BINARY_DIR}/dist/${PD_MACOSX_DMG}")
    set(MAKE_BUNDLE_SCRIPT ${PROJECT_BINARY_DIR}/dist/make_bundle.sh)

    # copy and substitute variables to Info.plist
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/dist)
    configure_file(${PROJECT_SOURCE_DIR}/ceammc/gui/Info.plist ${PROJECT_BINARY_DIR}/dist/Info.plist)

    # tk versions later then 8.6.8 have problems with russian keyboard shortcuts
    set(TK_VERSION "8.6.12")
    set(EMBEDDED_WISH "${PROJECT_SOURCE_DIR}/mac/Wish-${TK_VERSION}.app")

    if(EXISTS ${EMBEDDED_WISH})
        set(WISH_APP "${EMBEDDED_WISH}")
    else()
        message(FATAL_ERROR "TCL (${TK_VERSION}) not found. You should build embedded Wish.app with build scripts in ${PROJECT_SOURCE_DIR}/mac directory. Run:")
        message(FATAL_ERROR "   ./tcltk-wish.sh --download ${TK_VERSION}")
        message(FATAL_ERROR "   ./tcltk-wish.sh --build --leave ${TK_VERSION}")
    endif()

    message(STATUS "found Wish.app: ${WISH_APP}")
    message(STATUS "found Tcl/Tk version: ${TK_VERSION}")

    add_custom_command(
        OUTPUT ${BUNDLE_FULL_PATH}
        COMMAND sh ${MAKE_BUNDLE_SCRIPT}
        COMMAND ${CMAKE_COMMAND}
            -DBUNDLE=${BUNDLE_FULL_PATH}
            -P ${PROJECT_SOURCE_DIR}/cmake/bundle.cmake
        DEPENDS pd
        USES_TERMINAL
    )

    # app target
    # `make app` creates MacOSX bundle
    add_custom_target(app)

    add_custom_command(TARGET app PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${BUNDLE_FULL_PATH}
        COMMAND sh ${MAKE_BUNDLE_SCRIPT}
        COMMAND ${CMAKE_COMMAND}
            -DBUNDLE=${BUNDLE_FULL_PATH}
            -P ${PROJECT_SOURCE_DIR}/cmake/bundle.cmake
        USES_TERMINAL
        )

    add_custom_target(app-zip
        COMMAND ${CMAKE_COMMAND} -E tar "cf" ${PD_MACOSX_ZIP} --format=zip
           "${BUNDLE_FULL_PATH}"
           WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dist)

    add_custom_target(dev_ceammc
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ceammc> ${BUNDLE_FULL_PATH}/Contents/Resources/extra/ceammc/)

    add_custom_target(codesign
        COMMAND sh ${PROJECT_SOURCE_DIR}/mac/codesign.sh ${BUNDLE_FULL_PATH}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/mac)

    add_custom_target(dmg
        COMMAND
            sh ${PROJECT_SOURCE_DIR}/mac/dmg.sh
            ${BUNDLE_FULL_PATH}
            ${DMG_FULL_PATH}
            ${PROJECT_SOURCE_DIR}
            ${PD_MACOSX_DMG_APP}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS app)
endif()

if(UNIX AND NOT APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -funroll-loops -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -funroll-loops -fomit-frame-pointer")

    set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG -ffast-math -funroll-loops -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -O2 -DNDEBUG -ffast-math -funroll-loops -fomit-frame-pointer")
endif()
