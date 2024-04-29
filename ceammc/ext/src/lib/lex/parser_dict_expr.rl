# include "parser_dict_expr.h"
# include "ragel_common.h"
# include "parser_numeric.h"
# include "ceammc_string.h"

# include <cstring>

namespace ceammc {
namespace parser {

%%{
    machine dict_expr;
    include numeric_common "ragel_numeric.rl";


    action key_start { key_start = &fc; }
    action key_end   {
        if (key_start) {
            constexpr int BUF_SIZE = 256;
            char buf[BUF_SIZE+1];

            auto str_len = std::min<int>(&fc - key_start, BUF_SIZE);
            int i;
            for (i = 0; i < str_len; i++) {
                buf[i] = key_start[i];
            }
            buf[i] = '\0'; // set c-string null byte
            match.key_name = gensym(buf);

#ifndef NDEBUG
            printf(" key: %s\n", match.key_name->s_name);
#endif

            match.type = DictExprMatchType::DICT;
        }
    }

    action any_start   { match.type = DictExprMatchType::ANY; }

    action arr_begin   {
        match.array_slice_begin = ragel_num.getInteger();
        match.array_slice_length = ARRAY_SLICE_LENGTH_END;
        ragel_num = {};
    }
    action arr_len     { match.array_slice_length = ragel_num.getInteger(); ragel_num = {}; }
    action arr_len_end { match.array_slice_length = ARRAY_SLICE_LENGTH_END; }
    action arr_end   {
        match.type = DictExprMatchType::LIST;
#ifndef NDEBUG
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
#endif
    }

    action match_add  {
        if (matches) {
            matches->push_back(match);
            match = {};
            key_start = nullptr;
        }
    }

    obj_expr = '"' ([^"\[\]()]+) >key_start %key_end '"';
    arr_expr = (num_int %arr_begin %{ match.array_slice_length = 1; }
             | (num_int %arr_begin ':' %arr_len_end)
             | (num_int %arr_begin ':' %arr_len_end num_uint %arr_len)) %arr_end;

    match_array  = ('/' %any_start arr_expr);
    match_object = ('/' %any_start obj_expr);
    match_any    = '/*' %any_start;
    match_key    = '/' %any_start ([a-zA-Z_] [a-zA-Z_0-9]*)
                    >key_start
                    %key_end;

    match = match_any | match_key | match_array | match_object;

    main := (match %match_add)+;
    write data;
}%%

bool parse_dict_expr(const char* expr, DictExprMatchList* matches)
{
    int cs = 0;
    const char* p = expr;
    const char* pe = p + strlen(expr);
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    const char* key_start = nullptr;
    DictExprMatcher match;

    %% write init;
    %% write exec;

    const auto ok = cs >= %%{ write first_final; }%%;

    return ok;
}

}
}
