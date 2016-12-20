include_directories(Catch/single_include)
include_directories(../lib)
include_directories(${PROJECT_SOURCE_DIR}/src)
include(tests.cmake)

ceammc_add_test("ceammc::Atom" test_atom)
