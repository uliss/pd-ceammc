find_program(METHOD_GENERATOR "method_args")

function(pd_method_args target name)
  if(METHOD_GENERATOR)
    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${name}_args.hpp"
      DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp"
      COMMAND ${METHOD_GENERATOR} --force "${name}.cpp"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    target_sources(${target}
                   PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${name}_args.hpp")
  endif()
endfunction()
