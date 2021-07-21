add_executable(test_ext_list test_list_base.h)
target_sources(test_ext_list
    PRIVATE test_list_base.cpp)

target_compile_definitions(test_ext_list
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_list
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/list"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_list PUBLIC
    tests_main_lib
    ceammc_list
    ceammc_core
    puredata-core)

add_test(NAME test_ext_list
    COMMAND test_ext_list
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

macro(add_list_test src)
    target_sources(test_ext_list PRIVATE "test_list_${src}.cpp")
endmacro()

# test list
add_list_test(all_of)
add_list_test(any_of)
add_list_test(append)
add_list_test(apply_to)
add_list_test(at)
add_list_test(choice)
add_list_test(contains)
add_list_test(count)
add_list_test(count_if)
add_list_test(do)
add_list_test(each)
add_list_test(enumerate)
add_list_test(equal)
add_list_test(gen)
add_list_test(insert)
add_list_test(last)
add_list_test(length)
add_list_test(max)
add_list_test(mean)
add_list_test(min)
add_list_test(none_of)
add_list_test(normalize)
add_list_test(pass_if)
add_list_test(prepend)
add_list_test(product)
add_list_test(range)
add_list_test(reduce)
add_list_test(remove)
add_list_test(remove_if)
add_list_test(repeat)
add_list_test(reverse)
add_list_test(rldecode)
add_list_test(rlencode)
add_list_test(rotate)
add_list_test(route)
add_list_test(rundiff)
add_list_test(runsum)
add_list_test(search)
add_list_test(seq)
add_list_test(set)
add_list_test(shift)
add_list_test(slice)
add_list_test(sort)
add_list_test(sort_with)
add_list_test(split)
add_list_test(stretch)
add_list_test(sum)
add_list_test(unique)
add_list_test(unpack)
add_list_test(walk)
add_list_test(walk2)
add_list_test(xat)
add_list_test(xcontains)
add_list_test(xsearch)
add_list_test(zip)
