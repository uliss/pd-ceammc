add_subdirectory(arduino)
include_directories(${PROJECT_BINARY_DIR}/ceammc/extra/libusb)

set(HW_SOURCES udmx_handle.cpp)

macro(ceammc_hw_extension name)
    list(APPEND HW_SOURCES "hw_${name}.cpp")
endmacro()

ceammc_hw_extension(arduino)
ceammc_hw_extension(udmx)

if(LIBUSB_1_FOUND)
    ceammc_hw_extension(usb)
    add_definitions(-DWITH_LIBUSB_1)
    include_directories(${LIBUSB_1_INCLUDE_DIRS})
    add_compile_options(${LIBUSB_1_DEFINITIONS})
    list(APPEND HW_SOURCES usb/libusb_wrapper.cpp)
endif()

add_library(ceammc_hw STATIC mod_hw.cpp ${HW_SOURCES})
target_link_libraries(ceammc_hw hw_arduino libusb_01)

if(LIBUSB_1_FOUND)
    target_link_libraries(ceammc_hw ${LIBUSB_1_LIBRARIES})
endif()

if(UNIX)
    set_target_properties(ceammc_hw PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
