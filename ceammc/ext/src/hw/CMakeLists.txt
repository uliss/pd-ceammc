add_subdirectory(arduino)

set(HW_SOURCES)

macro(ceammc_hw_extension name)
    list(APPEND HW_SOURCES "hw_${name}.cpp")
endmacro()

ceammc_hw_extension(arduino)

add_library(ceammc_hw STATIC mod_hw.cpp ${HW_SOURCES})
target_link_libraries(ceammc_hw hw_arduino)

if(UNIX)
    set_target_properties(ceammc_hw PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
