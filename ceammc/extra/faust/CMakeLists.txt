if(APPLE)
    if(CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
        find_path(MACPORTS_LLVM
            NAMES llvm
            PATHS /opt/local/libexec
            PATH_SUFFIXES llvm-17/lib/cmake
            NO_DEFAULT_PATH
            NO_CMAKE_SYSTEM_PATH
            REQUIRED
            NO_CACHE)
    else()
        find_path(MACPORTS_LLVM
            NAMES llvm
            PATHS /opt/local/libexec
            PATH_SUFFIXES llvm-14/lib/cmake llvm-11/lib/cmake llvm-12/lib/cmake llvm-9.0/lib/cmake
            NO_DEFAULT_PATH
            NO_CMAKE_SYSTEM_PATH
            REQUIRED
            NO_CACHE)
    endif()

    set(LLVM_DIR ${MACPORTS_LLVM}/llvm)
    find_package(LLVM REQUIRED)
else()
    find_package(LLVM REQUIRED CONFIG)
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# find faust
if(WIN32)
    # faust expected to be installed with ./script/build_faust.sh
    # into this directory
    set(Faust_ROOT $ENV{USERPROFILE}/.local/faust)
else()
    # faust expected to be installed with ./script/build_faust.sh
    # into this directory
    set(Faust_ROOT $ENV{HOME}/.local/faust)
endif()
message(FATAL "Faust root is: ${Faust_ROOT}")

find_package(Faust REQUIRED)
message(STATUS "Faust libraries: ${Faust_LIBRARIES}")
add_library(libfaust SHARED IMPORTED)
set_target_properties(libfaust PROPERTIES
    IMPORTED_LOCATION ${Faust_LIBRARIES}
    IMPORTED_IMPLIB   ${Faust_LIBRARIES}
    INTERFACE_INCLUDE_DIRECTORIES ${Faust_INCLUDE_DIRS}
) 

set(FAUSTLIB_INSTALL_DIR ${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc)
get_filename_component(_FAUST_LIBRARY_DIR ${FAUST_LIBRARY} DIRECTORY)
if(UNIX AND NOT APPLE) # install so-versioned files
    file(REAL_PATH ${FAUST_LIBRARY} _FAUST_LIBRARY_SO)
    install(FILES "${_FAUST_LIBRARY_SO}" DESTINATION ${FAUSTLIB_INSTALL_DIR})
    install(FILES "${FAUST_LIBRARY}" DESTINATION ${FAUSTLIB_INSTALL_DIR})
    get_filename_component(_FAUST_LIBRARY_NAME ${_FAUST_LIBRARY_SO} NAME)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${_FAUST_LIBRARY_NAME} "libfaust.so.2"
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${_FAUST_LIBRARY_NAME} "libfaust.so.2."
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/libfaust.so.2"
        "${CMAKE_CURRENT_BINARY_DIR}/libfaust.so.2."
        DESTINATION ${FAUSTLIB_INSTALL_DIR})
elseif(WIN32) # install dll
    install(FILES "${_FAUST_LIBRARY_DIR}/libfaust.dll" DESTINATION ${PDCORE_LIB_INSTALL_PATH})
endif()

add_library(ceammc_faust STATIC ceammc_llvm.cpp)
target_link_libraries(ceammc_faust 
  PRIVATE 
    libfaust
    ${LLVM_LIBRARIES}
)
target_compile_definitions(ceammc_faust PRIVATE ${LLVM_DEFINITIONS})
target_include_directories(ceammc_faust
  PRIVATE
    ${LLVM_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src
  PUBLIC
    ${Faust_INCLUDE_DIRS})

set(DOC_INSTALL_PATH "${PD_DOC_INSTALL_PATH}/extra/ceammc")

install(DIRECTORY faustlibraries/
    DESTINATION ${DOC_INSTALL_PATH}/faust
    FILES_MATCHING PATTERN "*.lib"
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
