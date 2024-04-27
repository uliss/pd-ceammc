# include "parser_dict_expr.h"
# include "ragel_common.h"
# include "parser_numeric.h"

# include <cstring>

namespace ceammc {
namespace parser {

%%{
    machine dict_expr;
    include numeric_common "ragel_numeric.rl";


    action key_start { key_start = &fc; }
    action key_end   {
        if (key_start) {
            auto key_end = &fc;
            match.key_name.assign(key_start, key_end - key_start);

            printf(" key: %s\n", match.key_name.c_str());
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
        if (match.array_slice_length == ARRAY_SLICE_LENGTH_END)
            printf("array range: %lld:END\n", match.array_slice_begin);
        else if (match.array_slice_length == ARRAY_SLICE_LENGTH_NONE)
            printf("array range: %lld:?\n", match.array_slice_begin);
        else
            printf("array range: %lld:%lld\n", match.array_slice_begin, match.array_slice_length);
    }

    key_expr = '"' ([^"\[\]()]+) >key_start %key_end '"';
    arr_expr = (num_int %arr_begin %{ match.array_slice_length = 1; }
             | (num_int %arr_begin ':' %arr_len_end)
             | (num_int %arr_begin ':' %arr_len_end num_uint %arr_len)) %arr_end;

    obj_expr = arr_expr | key_expr;

    match_any = '.' %any_start;
    match_key = '.' %any_start ([a-zA-Z_0-9]+)
                    >key_start
                    %key_end
                    ;
    match_object = ('.[' %any_start obj_expr? ']');
    match = match_any | match_object | match_key;

    main :=
        (match
            %{
                if (matches) {
                    matches->push_back(match);
                    match = {};
                    key_start = nullptr;
                }
             }
        )+;
    write data;
}%%

bool parse_dict_expr(const char* expr, std::vector<DictExprMatcher>* matches)
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
