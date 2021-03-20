set(ARDUINO_SOURCES
    arduino_thread.cpp arduino_thread.h
    arduino.cpp arduino.h
    arduino_connection_storage.cpp arduino_connection_storage.h
    arduino_connection_tracker.cpp arduino_connection_tracker.h)


add_library(hw_arduino STATIC ${ARDUINO_SOURCES})

target_link_libraries(hw_arduino serial ceammc_core)
target_include_directories(hw_arduino
    PRIVATE
        ${CMAKE_SOURCE_DIR}/ceammc/extra/serial/serial/include
        ${PROJECT_SOURCE_DIR}/ceammc/extra # for readerwriterqueue
        ${PROJECT_BINARY_DIR} # for config.h
)
