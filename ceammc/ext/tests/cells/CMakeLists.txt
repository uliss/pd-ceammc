add_executable(test_cells test_property.h)

find_program(SOX_EXEC sox)
if(SOX_EXEC)
    target_compile_definitions(test_cells PRIVATE SOX_EXEC="${SOX_EXEC}")
endif()

target_compile_definitions(test_cells
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_cells
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "$<TARGET_PROPERTY:reflex,INCLUDE_DIRECTORIES>"
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_cells PUBLIC
    tests_main_lib
    ceammc_core
    puredata-core)

add_test(NAME test_cells
    COMMAND test_cells
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/..")

function(add_cell_test src)
    target_sources(test_cells PRIVATE "test_${src}.cpp")
endfunction()

add_cell_test(array)
add_cell_test(array_saver)
add_cell_test(atom)
add_cell_test(atom2)
add_cell_test(atomlist)
add_cell_test(atomlist2)
add_cell_test(atomlist_view)
add_cell_test(message)

add_cell_test(data)
add_cell_test(datatype_color)
add_cell_test(datatype_dict)
add_cell_test(datatype_mlist)
add_cell_test(datatype_set)
add_cell_test(datatype_string)

add_cell_test(editor_unescape)
add_cell_test(exceptions)
add_cell_test(filesystem)

add_cell_test(parser_array_saver)
add_cell_test(parser_clone)
add_cell_test(parser_color)
add_cell_test(parser_data_string)
add_cell_test(parser_data_string3)
add_cell_test(parser_dict_expr)
add_cell_test(parser_faust)
add_cell_test(parser_midi)
add_cell_test(parser_music)
add_cell_test(parser_numeric)
add_cell_test(parser_score)
add_cell_test(parser_select)
add_cell_test(parser_timeline)
add_cell_test(parser_ui_bind)
add_cell_test(parser_units)

add_cell_test(properties)
add_cell_test(properties2)
add_cell_test(propertyinfo)

add_cell_test(property_alias)
add_cell_test(property_bool)
add_cell_test(property_bpm)
add_cell_test(property_data)
add_cell_test(property_duration)
add_cell_test(property_float)
add_cell_test(property_int)
add_cell_test(property_list)
add_cell_test(property_symbol)
add_cell_test(property_timesig)

add_cell_test(resampler)

add_cell_test(args)
add_cell_test(base64)
add_cell_test(convert)
add_cell_test(dsp)
add_cell_test(fn_list)
add_cell_test(fn_list2)
add_cell_test(format)
add_cell_test(json)
add_cell_test(platform)
add_cell_test(random)
add_cell_test(regexp)
add_cell_test(score)
add_cell_test(string)
add_cell_test(transport)
add_cell_test(units)

