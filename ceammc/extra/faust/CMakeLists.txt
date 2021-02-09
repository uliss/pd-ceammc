## Hardcoded targets for faust
#set(MSVC_STATIC         ON  CACHE STRING  "Use static runtimes with MSVC" FORCE)
set(INCLUDE_STATIC      ON  CACHE STRING  "Include static library"        FORCE)
set(INCLUDE_EXECUTABLE  OFF CACHE STRING  "Include runtime executable"    FORCE)
set(INCLUDE_DYNAMIC     OFF CACHE STRING  "Include dynamic library"       FORCE)
set(INCLUDE_OSC         OFF CACHE STRING  "Include Faust OSC library"     FORCE)
set(INCLUDE_HTTP        OFF CACHE STRING  "Include Faust HTTPD library"   FORCE)

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

if(APPLE)
    find_program(LLVM_CONFIG "llvm-config"
        PATHS /usr/local/opt/llvm/bin)
    if (${LLVM_CONFIG} STREQUAL LLVM_CONFIG-NOTFOUND)
        message (FATAL_ERROR "Cannot find program llvm-config")
    endif()

    message(STATUS "llvm: ${LLVM_CONFIG}")
endif()

## Call the faust cmakelist.txt
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/faust/build EXCLUDE_FROM_ALL)

#find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_library(ceammc_faust STATIC ceammc_llvm.cpp)
target_link_libraries(ceammc_faust staticlib)
target_compile_definitions(ceammc_faust PRIVATE ${LLVM_DEFINITIONS})
target_include_directories(ceammc_faust PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/faust
    ${LLVM_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src)
