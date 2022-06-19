add_subdirectory(arduino)

function(ceammc_hw_external name)
    list(APPEND HW_SOURCES "hw_${name}.cpp")
endfunction()

# Sudden motion sensor on macbook's
if(APPLE)
    add_compile_definitions(WITH_SMS)
    add_compile_definitions(WITH_SMC)
    add_compile_definitions(WITH_IODISPLAY)
    list(APPEND HW_SOURCES apple/suddenmotionsensor.cpp)
    list(APPEND HW_SOURCES apple/smc.cpp)
    list(APPEND HW_SOURCES apple/fn_strings.cpp)
    list(APPEND HW_SOURCES apple/iodisplay.cpp)
elseif(UNIX AND NOT APPLE)
#    find_package(XCB)
#    if(XCB_FOUND AND XCB_RANDR_FOUND AND XCB_UTIL_FOUND)
#        add_definitions(-DWITH_X11DISPLAY)
#        list(APPEND HW_SOURCES linux/x11display.cpp)
#        list(APPEND HW_LINK_LIBRARIES ${XCB_LIBRARIES} ${XCB_RANDR_LIBRARIES} ${XCB_UTIL_LIBRARIES})
#    endif()
endif()


ceammc_hw_external(apple_smc)
ceammc_hw_external(apple_sms)
ceammc_hw_external(arduino)
ceammc_hw_external(cpu_temp)
ceammc_hw_external(display)
ceammc_hw_external(kbd_light)
ceammc_hw_external(serial)

#if(LIBUSB_1_FOUND)
#    ceammc_hw_external(usb)
#    add_definitions(-DWITH_LIBUSB_1)
#    include_directories(${LIBUSB_1_INCLUDE_DIRS})
#    add_compile_options(${LIBUSB_1_DEFINITIONS})
#    list(APPEND HW_SOURCES usb/libusb_wrapper.cpp)
#endif()

add_library(ceammc_hw STATIC mod_hw.cpp ${HW_SOURCES})
target_include_directories(ceammc_hw
    PRIVATE
        ${PROJECT_BINARY_DIR} # for config.h
        ${PROJECT_SOURCE_DIR}/ceammc/extra/SMSLib
)

target_link_libraries(ceammc_hw PRIVATE serial readerwriterqueue hw_arduino)

#if(WITH_EXPERIMENTAL)
#    add_definitions(-DWITH_LIBUSB_01)
#    set(HW_SOURCES udmx_handle.cpp)
#    ceammc_hw_external(udmx)
#    target_link_libraries(ceammc_hw libusb_01)
#endif()

#if(LIBUSB_1_FOUND)
#    target_link_libraries(ceammc_hw ${LIBUSB_1_LIBRARIES})
#endif()
