include(TestBigEndian)
include(FindLibDL)
include(windres)
test_big_endian(IS_BIG_ENDIAN)
if(NOT ${IS_BIG_ENDIAN})
    add_definitions(-DLITTLE_ENDIAN=0x0001 -DBYTE_ORDER=LITTLE_ENDIAN)
endif()

find_package(Threads)

set(PLATFORM_LINK_LIBRARIES)


if(UNIX AND NOT APPLE)
    add_definitions(-D_GNU_SOURCE)
    list(APPEND PLATFORM_LINK_LIBRARIES m)
    list(APPEND PLATFORM_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
    set(LINUX True)
endif()


if(MSYS)
    add_definitions(-DPD_INTERNAL -DWINVER=0x0502 -D_WIN32_WINNT=0x0502)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mms-bitfields -g -O3 -funroll-loops -fomit-frame-pointer")
    list(APPEND PLATFORM_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
    list(APPEND PLATFORM_LINK_LIBRARIES "m" "wsock32" "ole32" "winmm")
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")
    set(CMAKE_EXE_LINKER_FLAGS "-lOle32 -lWinmm -lpthread -static-libgcc")	
endif()

if(APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3 -funroll-loops -fomit-frame-pointer")

    set(BUNDLE "Pd-${PD_MACOSX_BUNDLE_SUFFIX}.app")
    set(BUNDLE_FULL_PATH "${PROJECT_BINARY_DIR}/dist/${BUNDLE}")
    set(MAKE_BUNDLE_SCRIPT ${PROJECT_BINARY_DIR}/dist/build_mac.sh)

    # copy and substitute variables to Info.plist
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/dist)
    configure_file(${PROJECT_SOURCE_DIR}/ceammc/gui/Info.plist ${PROJECT_BINARY_DIR}/dist/Info.plist)

    add_custom_command(
        OUTPUT ${MAKE_BUNDLE_SCRIPT}
        COMMAND ${CMAKE_COMMAND}
            -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
            -DPROJECT_BINARY_DIR="${PROJECT_BINARY_DIR}"
            -DBUNDLE=${BUNDLE_FULL_PATH}
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
        COMMAND sh ${PROJECT_SOURCE_DIR}/mac/dmg.sh ${BUNDLE_FULL_PATH}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS app)
endif()
