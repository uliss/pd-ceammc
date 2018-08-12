if(UNIX)
    if(APPLE)
        set(DARWIN_API 1)
        set(LINUX_API 0)
    else()
        set(LINUX_API 1)
        set(DARWIN_API 0)
    endif()
endif()

check_include_file(limits.h HAVE_LIMITS_H)
check_include_file(values.h HAVE_VALUES_H)
check_include_file(unistd.h HAVE_UNISTD_H)
set(LIBUSB_MAJOR_VERSION 0)
set(LIBUSB_MINOR_VERSION 1)
set(LIBUSB_MICRO_VERSION 12)

check_include_file(dlfcn.h HAVE_DLFCN_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(signal.h HAVE_SIGNAL_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)

test_big_endian(BIGENDIAN)
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/libusb_config.h @ONLY)
configure_file(libusb-0.1.12/usb.h.in ${CMAKE_CURRENT_BINARY_DIR}/usb.h)
add_compile_options(-include ${CMAKE_CURRENT_BINARY_DIR}/libusb_config.h)

set(LIBUSB_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/usb.h
    ${CMAKE_CURRENT_BINARY_DIR}/libusb_config.h
    config.h.in
    libusb-0.1.12/usb.h.in
    libusb-0.1.12/usbi.h
    libusb-0.1.12/descriptors.c
    libusb-0.1.12/error.h
    libusb-0.1.12/error.c
    libusb-0.1.12/usb.c
    libusb-0.1.12/usbpp.h
    libusb-0.1.12/usbpp.cpp)


include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(APPLE)
    list(APPEND LIBUSB_SOURCES libusb-0.1.12/darwin.c)

    add_library(libusb_01 STATIC ${LIBUSB_SOURCES})
endif()

if(UNIX)
    set_target_properties(libusb_01 PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
