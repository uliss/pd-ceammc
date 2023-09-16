add_subdirectory(tiny-process-library)

set(BUILD_SHARED_LIBS OFF)
if(UNIX)
    set_target_properties(tiny-process-library PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
