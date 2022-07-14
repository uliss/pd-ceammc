#ifndef LEMON_DATA_PARSER_IMPL_H
#define LEMON_DATA_PARSER_IMPL_H

#include <cstdio>

namespace ceammc {
namespace parser {
    class LemonDataStringParser;
}
}

void lemon_data_string_parserInit(void* parser);
void lemon_data_string_parserFinalize(void* parser);
void lemon_data_string_parser_float(void* parser, int token, double value, ceammc::parser::LemonDataStringParser*);
void lemon_data_string_parser_str(void* parser, int token, const char* value, ceammc::parser::LemonDataStringParser*);
void lemon_data_string_parser_token(void* parser, int token, ceammc::parser::LemonDataStringParser*);


#ifndef NDEBUG
void lemon_data_string_parserTrace(FILE*, char* prefix);
#endif

#endif
