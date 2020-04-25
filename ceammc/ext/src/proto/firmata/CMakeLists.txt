set(FIRMATA_SOURCES
    firmata_proto.h firmata_proto.cpp
    firmata_parser.cpp firmata_parser.h
    firmata_lexer.cpp firmata_lexer.h
    firmata_pin.cpp firmata_pin.h
    firmata_bison.tab.c firmata_bison.tab.h firmata_bison.y firmata_bison.h
    firmata_proto_request.cpp)

add_library(proto_firmata STATIC ${FIRMATA_SOURCES})
