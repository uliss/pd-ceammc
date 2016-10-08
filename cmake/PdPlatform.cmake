set(PD_EXTERNAL_EXTENSION)
set(PD_EXTERNAL_CFLAGS)
set(PD_EXTERNAL_LDFLAGS)

if(APPLE)
    set(PD_EXTERNAL_EXTENSION d_fat)
    set(PD_EXTERNAL_CFLAGS "-fPIC")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3 -funroll-loops -fomit-frame-pointer")
    set(PD_EXTERNAL_LDFLAGS "-bundle -undefined dynamic_lookup")

    add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/gen_build_mac.sh
        COMMAND cmake ${CMAKE_COMMAND}
            -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
            -DPROJECT_BINARY_DIR="${PROJECT_BINARY_DIR}"
            -P ${PROJECT_SOURCE_DIR}/mac/cmake-build-mac.cmake)
    add_custom_target(app DEPENDS gen_build_mac.sh)
endif()
