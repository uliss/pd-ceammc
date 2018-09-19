add_subdirectory(arduino)
include_directories(${PROJECT_BINARY_DIR}/ceammc/extra/libusb)
include_directories(${CMAKE_SOURCE_DIR}/ceammc/extra/serial/serial/include)

macro(ceammc_hw_external name)
    list(APPEND HW_SOURCES "hw_${name}.cpp")
endmacro()

ceammc_hw_external(serial)
ceammc_hw_external(arduino)


if(LIBUSB_1_FOUND)
    ceammc_hw_external(usb)
    add_definitions(-DWITH_LIBUSB_1)
    include_directories(${LIBUSB_1_INCLUDE_DIRS})
    add_compile_options(${LIBUSB_1_DEFINITIONS})
    list(APPEND HW_SOURCES usb/libusb_wrapper.cpp)
endif()

add_library(ceammc_hw STATIC mod_hw.cpp ${HW_SOURCES})
target_link_libraries(ceammc_hw hw_arduino)

if(WITH_EXPERIMENTAL)
    add_definitions(-DWITH_LIBUSB_01)
    set(HW_SOURCES udmx_handle.cpp)
    ceammc_hw_external(udmx)
    target_link_libraries(ceammc_hw libusb_01)
endif()

if(LIBUSB_1_FOUND)
    target_link_libraries(ceammc_hw ${LIBUSB_1_LIBRARIES})
endif()

if(UNIX)
    set_target_properties(ceammc_hw PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
