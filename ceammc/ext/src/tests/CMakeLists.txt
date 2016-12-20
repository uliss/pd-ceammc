include_directories(Catch/single_include)
include_directories(../lib)
include_directories(${PROJECT_SOURCE_DIR}/src)
include(tests.cmake)

ceammc_add_test("ceammc::Atom" test_atom)
ceammc_add_test("ceammc::AtomList" test_atomlist)
ceammc_add_test("ceammc::ControlValue" test_controlvalue)
