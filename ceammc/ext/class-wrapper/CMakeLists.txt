add_subdirectory(scripts)
add_subdirectory(modules)

set(CLASS_WRAPPER_SOURCES
    src/class_constructor.h
    src/class_function.h
    src/class_method.h
    src/class_static_method.h
    src/data_iface.h
    src/wrapper.h
    src/wrapper_converters.h
    src/wrapper_datatype.cpp
    src/wrapper_datatype.h
    src/wrapper_fn_traits.h
    src/wrapper_invocations.h
    src/wrapper_macros.h
    src/wrapper_output.h
    src/wrapper_sequence.h
    src/wrapper_tuple.h
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/ext/src/lib)

add_library(wrapper_lib SHARED ${CLASS_WRAPPER_SOURCES})
target_link_libraries(wrapper_lib ceammc_core ceammc_data)

function(wrapper_add NAME)
    add_custom_target(
        gen_${NAME}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modules/${NAME}/gen_${NAME}.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modules/${NAME}/iface_${NAME}.h
        COMMAND ./generate_iface.sh ${CMAKE_CURRENT_SOURCE_DIR}/modules/${NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts/
    )

    add_custom_target(
        gen_${NAME}_doc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modules/${NAME}/gen_${NAME}.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/modules/${NAME}/iface_${NAME}.h
        COMMAND ./generate_doc.sh ${CMAKE_CURRENT_SOURCE_DIR}/modules/${NAME} ${CMAKE_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts/
    )

    file(GLOB _help_files modules/${NAME}/help/*.pd)

    pd_add_external(NAME ${NAME}
        INTERNAL
        FILES modules/${NAME}/gen_${NAME}.cpp
        LINK ceammc_core wrapper_lib
        HELP_FILES ${_help_files})


    set(WRAPPER_COMPLETION "modules/${NAME}/doc/completion_${NAME}.txt")
    add_custom_target(_completion_${NAME} SOURCES ${WRAPPER_COMPLETION})
    install(FILES ${WRAPPER_COMPLETION} DESTINATION ${PD_GUI_PLUGINS_PATH}/ceammc/user_objects)
endfunction()

wrapper_add(numeric)

find_package(Armadillo)
if(ARMADILLO_FOUND)
    include_directories(${ARMADILLO_INCLUDE_DIRS})

    wrapper_add(matrix)
    target_compile_definitions(matrix PUBLIC -DARMA_DONT_USE_WRAPPER -DARMA_DONT_USE_HDF5)
endif()

