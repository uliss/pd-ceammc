# - Find Ragel executable and provides macros to generate custom build rules
# The module defines the following variables:
#
#  RAGEL_EXECUTABLE - path to the ragel program
#  RAGEL_VERSION - version of ragel
#  RAGEL_FOUND - true if the program was found
#
# If ragel is found, the module defines the macros:
#
#  RAGEL_TARGET(<Name> <RagelInp> <CodeOutput>
#              [COMPILE_FLAGS <string>])
#
# which will create  a custom rule to generate a state machine. <RagelInp> is
# the path to a Ragel file. <CodeOutput> is the name of the source file
# generated by ragel. If  COMPILE_FLAGS option is specified, the next
# parameter is added in the ragel command line.
#
# The macro defines a set of variables:
#  RAGEL_${Name}_DEFINED       - true is the macro ran successfully
#  RAGEL_${Name}_INPUT         - The input source file, an alias for <RagelInp>
#  RAGEL_${Name}_OUTPUT_SOURCE - The source file generated by ragel
#  RAGEL_${Name}_OUTPUT_HEADER - The header file generated by ragel
#  RAGEL_${Name}_OUTPUTS       - The sources files generated by ragel
#  RAGEL_${Name}_COMPILE_FLAGS - Options used in the ragel command line
#
#  ====================================================================
#  Example:
#
#   find_package(RAGEL) # or e.g.: find_package(RAGEL 6.6 REQUIRED)
#   RAGEL_TARGET(MyMachine machine.rl ${CMAKE_CURRENT_BINARY_DIR}/machine.cc)
#   add_executable(Foo main.cc ${RAGEL_MyMachine_OUTPUTS})
#  ====================================================================

# 2014-02-09, Georg Sauthoff <mail@georg.so>
#
# I don't think that these few lines are even copyrightable material,
# but I am fine with using the BSD/MIT/GPL license on it ...
#
# I've used following references:
# http://www.cmake.org/cmake/help/v2.8.12/cmake.html
# /usr/share/cmake/Modules/FindFLEX.cmake
# /usr/share/cmake/Modules/FindBISON.cmake

# uses some features which are not available in 2.6
cmake_minimum_required(VERSION 3.5)

find_program(RAGEL_EXECUTABLE NAMES ragel DOC "path to the ragel executable")
mark_as_advanced(RAGEL_EXECUTABLE)

if(RAGEL_EXECUTABLE)

  execute_process(COMMAND ${RAGEL_EXECUTABLE} --version
    OUTPUT_VARIABLE RAGEL_version_output
    ERROR_VARIABLE  RAGEL_version_error
    RESULT_VARIABLE RAGEL_version_result
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  if(${RAGEL_version_result} EQUAL 0)
    string(REGEX REPLACE "^Ragel State Machine Compiler version ([^ ]+) .*$"
                         "\\1"
                         RAGEL_VERSION "${RAGEL_version_output}")
  else()
    message(SEND_ERROR
            "Command \"${RAGEL_EXECUTABLE} --version\" failed with output:
${RAGEL_version_error}")
  endif()

  #============================================================
  # RAGEL_TARGET (public macro)
  #============================================================
  #
  macro(RAGEL_TARGET Name Input Output)
    set(RAGEL_TARGET_usage
              "RAGEL_TARGET(<Name> <Input> <Output> [COMPILE_FLAGS <string>]")
    if(${ARGC} GREATER 3)
      if(${ARGC} EQUAL 5)
        if("${ARGV3}" STREQUAL "COMPILE_FLAGS")
          set(RAGEL_EXECUTABLE_opts  "${ARGV4}")
          separate_arguments(RAGEL_EXECUTABLE_opts)
        else()
          message(SEND_ERROR ${RAGEL_TARGET_usage})
        endif()
      else()
        message(SEND_ERROR ${RAGEL_TARGET_usage})
      endif()
    endif()

    add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${Output}"
      COMMAND ${RAGEL_EXECUTABLE}
      ARGS    ${RAGEL_EXECUTABLE_opts} -o${Output} ${Input}
      DEPENDS ${Input}
      COMMENT
          "[RAGEL][${Name}] Compiling state machine with Ragel ${RAGEL_VERSION}"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    set(RAGEL_${Name}_DEFINED       TRUE)
    set(RAGEL_${Name}_OUTPUTS       ${Output})
    set(RAGEL_${Name}_INPUT         ${Input})
    set(RAGEL_${Name}_COMPILE_FLAGS ${RAGEL_EXECUTABLE_opts})
  endmacro()

endif()

# use this include when module file is located under /usr/share/cmake/Modules
#include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
# use this include when module file is located in build tree
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(RAGEL REQUIRED_VARS  RAGEL_EXECUTABLE
                                        VERSION_VAR    RAGEL_VERSION)
