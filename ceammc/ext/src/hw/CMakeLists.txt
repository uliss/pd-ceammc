add_subdirectory(arduino)

add_library(ceammc_hw STATIC mod_hw.cpp)

function(ceammc_hw_external name)
    target_sources(ceammc_hw PRIVATE "hw_${name}.cpp")
endfunction()

# Sudden motion sensor on macbook's
if(APPLE)
    add_compile_definitions(WITH_SMS)
    add_compile_definitions(WITH_SMC)
    add_compile_definitions(WITH_IODISPLAY)
    target_sources(ceammc_hw PRIVATE apple/suddenmotionsensor.cpp)
    target_sources(ceammc_hw PRIVATE apple/smc.cpp)
    target_sources(ceammc_hw PRIVATE apple/fn_strings.cpp)
    target_sources(ceammc_hw PRIVATE apple/iodisplay.cpp)
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
ceammc_hw_external(motu_avb)
ceammc_hw_external(serial)

#if(LIBUSB_1_FOUND)
#    ceammc_hw_external(usb)
#    add_definitions(-DWITH_LIBUSB_1)
#    include_directories(${LIBUSB_1_INCLUDE_DIRS})
#    add_compile_options(${LIBUSB_1_DEFINITIONS})
#    list(APPEND HW_SOURCES usb/libusb_wrapper.cpp)
#endif()

target_include_directories(ceammc_hw
    PRIVATE
        ${PROJECT_BINARY_DIR} # for config.h
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
)

target_link_libraries(ceammc_hw
    PRIVATE
        serial
        readerwriterqueue
        hw_arduino
        http_lib
)

#if(WITH_EXPERIMENTAL)
#    add_definitions(-DWITH_LIBUSB_01)
#    set(HW_SOURCES udmx_handle.cpp)
#    ceammc_hw_external(udmx)
#    target_link_libraries(ceammc_hw libusb_01)
#endif()

#if(LIBUSB_1_FOUND)
#    target_link_libraries(ceammc_hw ${LIBUSB_1_LIBRARIES})
#endif()
