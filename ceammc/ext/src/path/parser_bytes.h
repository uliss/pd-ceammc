#ifndef PARSER_BYTES_H
#define PARSER_BYTES_H

#include <cstdint>
#include <ios>
#include <string>
#include <vector>

#include "ceammc_atomlist_view.h"

namespace ceammc {

enum ByteParserResult {
    BYTE_PARSE_RESULT_INVALID_RANGE = -1,
    BYTE_PARSE_RESULT_INVALID_STRING = -2,
    BYTE_PARSE_RESULT_INVALID_ATOM = -3,
};

int parse_byte_string(const char* str);

std::pair<int, size_t> parse_bytes(const AtomListView& lv, std::vector<char>& out);

std::string parser_byte_error(const AtomListView& lv, const std::pair<int, size_t>& res);

std::ios::openmode parse_mode(const char* str);

}

#endif // PARSER_BYTES_H
