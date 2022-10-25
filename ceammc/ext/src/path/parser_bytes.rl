# include "parser_bytes.h"
# include "fmt/core.h"
# include "ceammc_format.h"

%%{
    machine bytes;

    prefix = '0x' >{ rl_val = 0; };
    xdig = [0-9]  @{ (rl_val <<= 4) |= (fc - '0'); } |
           [a-f]  @{ (rl_val <<= 4) |= (fc - 'a' + 10); } |
           [A-F]  @{ (rl_val <<= 4) |= (fc - 'A' + 10); };

    byte = xdig{2};

    main := prefix byte 0 @{ fbreak; };

    write data;
}%%

namespace ceammc {

int parse_byte_string(const char* str)
{
    int cs = 0;
    int rl_val = 0;

    const char* p = str;

    %% write init;
    %% write exec noend;

    if (cs >= %%{ write first_final; }%%)
        return rl_val;

    return BYTE_PARSE_RESULT_INVALID_STRING;
}

std::pair<int, size_t> parse_bytes(const AtomListView& lv, std::vector<char>& out)
{
    out.clear();
    out.reserve(lv.size());

    for (size_t i = 0; i < lv.size(); i++) {
        auto& a = lv[i];
        if (a.isInteger()) {
            auto val = a.asT<int>();
            if (val >= 0 && val < 255) {
                out.push_back(static_cast<char>(val));
            } else {
                return { BYTE_PARSE_RESULT_INVALID_RANGE, i };
            }
        } else if (a.isSymbol()) {
            auto val = parse_byte_string(a.asT<t_symbol*>()->s_name);
            if (val < 0)
                return { val, i };

            out.push_back(static_cast<char>(val));
        } else {
            return { BYTE_PARSE_RESULT_INVALID_ATOM, i };
        }
    }

    return { 0, 0 };
}

std::string parser_byte_error(const AtomListView& lv, const std::pair<int, size_t>& res)
{
    switch(res.first) {
    case BYTE_PARSE_RESULT_INVALID_RANGE:
        return fmt::format("byte value out of range at [{}]: {}", res.second, to_string(lv.at(res.second)));
    case BYTE_PARSE_RESULT_INVALID_STRING:
        return fmt::format("invalid string at [{}]: {}", res.second, to_string(lv.at(res.second)));
    case BYTE_PARSE_RESULT_INVALID_ATOM:
        return fmt::format("invalid atom at [{}]: {}", res.second, to_string(lv.at(res.second)));
    default:
        return {};
    }
}

%%{
    machine open_mode;

    mode = 'r' @{ rl_mode = std::ios::in; } |
           'w' @{ rl_mode = std::ios::out | std::ios::trunc; } |
           'a' @{ rl_mode = std::ios::out | std::ios::app; };


    main := mode ('+'? @{ rl_mode |= std::ios::in; }) 0 @{ fbreak; };

    write data;
}%%

std::ios::openmode parse_mode(const char* str)
{
    int cs = 0;
    std::ios::openmode rl_mode = 0;
    const char* p = str;

    if (!p || p[0] == '\0')
        return std::ios::in | std::ios::out;

    %% write init;
    %% write exec noend;

    if (cs >= %%{ write first_final; }%%)
        return rl_mode;

    return 0;
}

}
