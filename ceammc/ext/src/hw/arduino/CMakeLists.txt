set(ARDUINO_SOURCES
    arduino_thread.cpp arduino_thread.h
    arduino.cpp arduino.h
    arduino_connection_storage.cpp arduino_connection_storage.h
    arduino_connection_tracker.cpp arduino_connection_tracker.h)


add_library(hw_arduino STATIC ${ARDUINO_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/ceammc/extra/serial/serial/include)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra) # for readerwriterqueue

target_link_libraries(hw_arduino serial ceammc_core)
