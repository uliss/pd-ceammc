add_library(filesentry STATIC
    FileWatcher.cpp
)

target_include_directories(filesentry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

if(APPLE)
    target_sources(filesentry PRIVATE FileWatcherOSX.cpp)
endif()

if(WIN32)
    target_sources(filesentry PRIVATE FileWatcherWin32.cpp)
endif()

if(UNIX AND NOT APPLE)
    target_sources(filesentry PRIVATE FileWatcherLinux.cpp)
endif()
