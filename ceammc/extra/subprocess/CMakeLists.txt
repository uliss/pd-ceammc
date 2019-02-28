add_subdirectory(tiny-process-library)

if(UNIX)
    set_target_properties(tiny-process-library PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
