## Hardcoded targets for faust
#set(MSVC_STATIC         ON  CACHE STRING  "Use static runtimes with MSVC" FORCE)
set(INCLUDE_STATIC      ON  CACHE STRING  "Include static library"        FORCE)
set(INCLUDE_EXECUTABLE  OFF CACHE STRING  "Include runtime executable"    FORCE)
set(INCLUDE_DYNAMIC     OFF CACHE STRING  "Include dynamic library"       FORCE)
set(INCLUDE_OSC         OFF CACHE STRING  "Include Faust OSC library"     FORCE)
set(INCLUDE_HTTP        OFF CACHE STRING  "Include Faust HTTPD library"   FORCE)
set(INCLUDE_EMCC        OFF CACHE STRING  ""   FORCE )
set(INCLUDE_WASM_GLUE   OFF CACHE STRING  ""   FORCE )

## Hardcoded backends for faust
set(ASMJS_BACKEND  OFF                            CACHE STRING  "Include ASMJS backend"     FORCE)
set(C_BACKEND      OFF                            CACHE STRING  "Include C backend"         FORCE)
set(CPP_BACKEND    OFF                            CACHE STRING  "Include CPP backend"       FORCE)
set(FIR_BACKEND    OFF                            CACHE STRING  "Include FIR backend"       FORCE)
set(INTERP_BACKEND OFF                            CACHE STRING  "Include INTERPRETER backend" FORCE)
set(JAVA_BACKEND   OFF                            CACHE STRING  "Include JAVA backend"      FORCE)
set(JS_BACKEND     OFF                            CACHE STRING  "Include JAVASCRIPT backend" FORCE)
set(LLVM_BACKEND   COMPILER STATIC DYNAMIC        CACHE STRING  "Include LLVM backend"      FORCE)
set(OLDCPP_BACKEND OFF                            CACHE STRING  "Include old CPP backend"   FORCE)
set(RUST_BACKEND   OFF                            CACHE STRING  "Include RUST backend"      FORCE)
set(WASM_BACKEND   OFF                            CACHE STRING  "Include WASM backend"      FORCE)

if(APPLE OR UNIX)
    set(LLVM_DIR /usr/local/opt/llvm/lib/cmake/llvm CACHE STRING "" FORCE)
    set(INCLUDE_LLVM OFF CACHE BOOL "" FORCE)
elseif(WIN32)
    set(INCLUDE_LLVM ON CACHE BOOL "" FORCE)
    set(SELF_CONTAINED_LIBRARY ON CACHE BOOL "" FORCE)
endif()

set(INCLUDE_STATIC ON CACHE BOOL "" FORCE)
set(USE_LLVM_CONFIG OFF CACHE BOOL "" FORCE)

if(APPLE)
    find_path(MACPORTS_LLVM
        NAMES llvm
        PATHS /opt/local/libexec
        PATH_SUFFIXES llvm-14/lib/cmake llvm-10/lib/cmake llvm-11/lib/cmake llvm-12/lib/cmake llvm-9.0/lib/cmake
        NO_DEFAULT_PATH
        NO_CMAKE_SYSTEM_PATH
        REQUIRED
        NO_CACHE)

    set(LLVM_DIR ${MACPORTS_LLVM}/llvm)
    find_package(LLVM REQUIRED CONFIG)
else()
    find_package(LLVM REQUIRED CONFIG)
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

## Call the faust cmakelist.txt
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/faust/build EXCLUDE_FROM_ALL)

add_library(ceammc_faust STATIC ceammc_llvm.cpp)
target_link_libraries(ceammc_faust PRIVATE staticlib)
target_compile_definitions(ceammc_faust PRIVATE ${LLVM_DEFINITIONS})
target_include_directories(ceammc_faust PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/faust/architecture
    ${LLVM_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src)

if(COMMAND llvm_map_components_to_libnames)
    set(llvm_libs)
    if(CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
        llvm_map_components_to_libnames(llvm_libs_arm64
            AArch64AsmParser AArch64Info AArch64Desc AArch64CodeGen)
        list(APPEND llvm_libs ${llvm_libs_arm64})
    elseif (CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64")
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
endif()

set(DOC_INSTALL_PATH "${PD_DOC_INSTALL_PATH}/extra/ceammc")

install(DIRECTORY faust/libraries/
    DESTINATION ${DOC_INSTALL_PATH}/faust
    FILES_MATCHING PATTERN "*.lib"
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
