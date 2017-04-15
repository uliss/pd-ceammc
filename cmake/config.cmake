include(CheckIncludeFiles)
include(CheckFunctionExists)
include(FindLibDL)

if(LIBDL_FOUND)
    set(HAVE_LIBDL 1)
endif()

check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(alloca.h HAVE_ALLOCA_H)
check_include_files(bstring.h HAVE_BSTRING_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(libgen.h HAVE_LIBGEN_H)
check_function_exists(nanosleep HAVE_NANOSLEEP)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)
add_definitions(-include ${PROJECT_BINARY_DIR}/config.h)
