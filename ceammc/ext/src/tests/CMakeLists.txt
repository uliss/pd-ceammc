include_directories(Catch/single_include)
include_directories(../lib)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(test_atom test_atom.cpp)
target_link_libraries(test_atom ceammc_static)

add_test("ceammc::Atom" test_atom)
