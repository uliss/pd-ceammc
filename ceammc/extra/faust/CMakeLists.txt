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
set(Faust_ROOT $ENV{HOME}/.local/faust)
find_package(Faust REQUIRED)
message(STATUS "Faust libraries: ${Faust_LIBRARIES}")
add_library(faust SHARED IMPORTED)
set_target_properties(faust PROPERTIES
    IMPORTED_LOCATION ${Faust_LIBRARIES}
    INTERFACE_INCLUDE_DIRECTORIES ${Faust_INCLUDE_DIRS}
)

set(FAUSTLIB_INSTALL_DIR ${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc)
get_filename_component(_FAUST_LIBRARY_DIR ${FAUST_LIBRARY} DIRECTORY)
if(UNIX AND NOT APPLE)
    file(REAL_PATH ${FAUST_LIBRARY} _FAUST_LIBRARY_SO)
    install(FILES "${_FAUST_LIBRARY_SO}" DESTINATION ${FAUSTLIB_INSTALL_DIR})
    install(FILES "${FAUST_LIBRARY}.2" DESTINATION ${FAUSTLIB_INSTALL_DIR})
    install(FILES "${FAUST_LIBRARY}" DESTINATION ${FAUSTLIB_INSTALL_DIR})
#    install(IMPORTED_RUNTIME_ARTIFACTS faust DESTINATION ${FAUSTLIB_INSTALL_DIR})
endif()

add_library(ceammc_faust STATIC ceammc_llvm.cpp)
target_link_libraries(ceammc_faust PRIVATE faust)
target_compile_definitions(ceammc_faust PRIVATE ${LLVM_DEFINITIONS})
target_include_directories(ceammc_faust
  PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/faust/architecture
    ${LLVM_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src
  PUBLIC
    ${Faust_INCLUDE_DIRS})

if(COMMAND llvm_map_components_to_libnames)
    set(llvm_libs)
    if(APPLE)
        if(CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
            llvm_map_components_to_libnames(llvm_libs_arm64
                AArch64AsmParser AArch64Info AArch64Desc AArch64CodeGen)
            list(APPEND llvm_libs ${llvm_libs_arm64})
        else (CMAKE_OSX_ARCHIDESTINATION ${FAUSTLIB_INSTALL_DIR}TECTURES STREQUAL "x86_64")
            llvm_map_components_to_libnames(llvm_libs_x86
                x86asmparser x86info x86desc x86codegen)
            list(APPEND llvm_libs ${llvm_libs_x86})
        endif()

        llvm_map_components_to_libnames(llvm_libs_base
            support core irreader executionengine
            asmprinter mcjit ipo objcarcopts)
        list(APPEND llvm_libs ${llvm_libs_base})

        message(STATUS "llvm-libs: ${llvm_libs}")
        target_link_libraries(ceammc_faust PRIVATE ${llvm_libs})
    elseif(UNIX)
        llvm_map_components_to_libnames(llvm_libs_base
            support core irreader executionengine
            asmprinter mcjit ipo objcarcopts)
        list(APPEND llvm_libs ${llvm_libs_base})

        message(STATUS "llvm-libs: ${llvm_libs}")
#        target_link_libraries(ceammc_faust PRIVATE ${llvm_libs})
        target_link_libraries(ceammc_faust PRIVATE ${LLVM_LIBRARIES})

    endif()
endif()

set(DOC_INSTALL_PATH "${PD_DOC_INSTALL_PATH}/extra/ceammc")

install(DIRECTORY faustlibraries/
    DESTINATION ${DOC_INSTALL_PATH}/faust
    FILES_MATCHING PATTERN "*.lib"
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
