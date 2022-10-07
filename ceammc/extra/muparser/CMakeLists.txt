# Bump versions on release
set(MUPARSER_VERSION_MAJOR 2)
set(MUPARSER_VERSION_MINOR 3)
set(MUPARSER_VERSION_PATCH 2)
set(MUPARSER_VERSION ${MUPARSER_VERSION_MAJOR}.${MUPARSER_VERSION_MINOR}.${MUPARSER_VERSION_PATCH})

add_library(muparser STATIC
    muparser/src/muParserBase.cpp
    muparser/src/muParserBytecode.cpp
    muparser/src/muParserCallback.cpp
    muparser/src/muParser.cpp
#    muparser/src/muParserDLL.cpp
    muparser/src/muParserError.cpp
    muparser/src/muParserInt.cpp
    muparser/src/muParserTokenReader.cpp
)

target_include_directories(muparser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/muparser/include)

#target_compile_definitions(muparser PRIVATE MUPARSER_DLL)
target_compile_definitions(muparser PUBLIC MUPARSER_STATIC)


