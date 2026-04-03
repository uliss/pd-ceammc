find_program(METHOD_GENERATOR "method_args")

function(pd_method_args target name)
  if(METHOD_GENERATOR)
    add_custom_command(
      OUTPUT "${name}_args.hpp"
      DEPENDS "${name}.cpp"
      COMMAND ${METHOD_GENERATOR} --force "${name}.cpp"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    target_sources(${target} PRIVATE "${name}_args.hpp")
  endif()
endfunction()
