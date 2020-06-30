# Bump versions on release
set(MUPARSER_VERSION_MAJOR 2)
set(MUPARSER_VERSION_MINOR 3)
set(MUPARSER_VERSION_PATCH 2)
set(MUPARSER_VERSION ${MUPARSER_VERSION_MAJOR}.${MUPARSER_VERSION_MINOR}.${MUPARSER_VERSION_PATCH})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/muparser/include")
add_library(muparser
    muparser/src/muParserBase.cpp
    muparser/src/muParserBytecode.cpp
    muparser/src/muParserCallback.cpp
    muparser/src/muParser.cpp
    muparser/src/muParserDLL.cpp
    muparser/src/muParserError.cpp
    muparser/src/muParserInt.cpp
#    muparser/src/muParserTest.cpp
    muparser/src/muParserTokenReader.cpp
)

# this compiles the "DLL" interface (C API)
target_compile_definitions(muparser PRIVATE MUPARSER_DLL)

#if (BUILD_SHARED_LIBS)
#  target_compile_definitions(muparser PRIVATE MUPARSERLIB_EXPORTS)
#else ()
  target_compile_definitions(muparser PUBLIC MUPARSER_STATIC)
#endif()

set_target_properties(muparser PROPERTIES
    VERSION ${MUPARSER_VERSION}
    SOVERSION ${MUPARSER_VERSION_MAJOR}
)

#install(TARGETS muparser
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT RuntimeLibraries
#    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT RuntimeLibraries
#)

