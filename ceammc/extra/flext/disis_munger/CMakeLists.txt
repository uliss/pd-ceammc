include(PdExtension)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../stk/stk/include)

pd_add_extension(NAME disis_munger~
    FILES disis_munger.cpp
    INTERNAL True
    LIBRARY disis_munger
    LINK stk flext_static
    HELP_FILES disis_munger~-help.pd
)
