# include "lexer_data_string.h"
# include "parser_numeric.h"
# include "data_string_parser2.h"
# include "data_string_location2.h"

# include <cstdint>

#define VERBOSE

# ifdef VERBOSE
# define DUMP_TOKEN(token, var) std::cerr << "[token] " #token << ' ' << var << std::endl;
# else
# define DUMP_TOKEN(token)
# endif

namespace {
uint8_t xchar2digit(char c)
{
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return c - '0';
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        return c - 'A' + 10;
    default:
        return c - 'a' + 10;
    }
}

}

%%{
    machine data_string_lexer;
    include numeric_common "ragel_numeric.rl";

    num = (num_float | num_int | num_bin | num_hex | num_ratio) % {
        t_float v = 0;
        switch(ragel_type) {
        case TYPE_HEX:
        case TYPE_BIN:
        case TYPE_INT:
            token_float = ragel_num.getInteger();
        break;
        case TYPE_FLOAT:
            token_float = ragel_num.getFloat();
        break;
        case TYPE_RATIO:
            token_float = ragel_num.getRatioAsFloat();
        break;
        default:
            return parser::make_LEXER_ERROR(tokenRange());
        break;
        }
    };

    property       = '@' [a-z_0-9?]+;
    func_call_list = [a-z][a-z_0-9]* '(';
    data_call_list = [A-Z][a-zA-Z]*  '(';
    data_call_dict = [A-Z][a-zA-Z]*  '[';
    data_call_str  = [A-Z][a-zA-Z]*  '"';
    dict_key       = [a-z_0-9?]+;


    main := |*
        space+    => { DUMP_TOKEN(SPACE, ' '); return parser::make_SPACE(tokenRange()); };
        "true"    => { return parser::make_FLOAT(1, tokenRange()); };
        "false"   => { return parser::make_FLOAT(0, tokenRange()); };
        "null"    => { return parser::make_NULL(tokenRange()); };
        "("       => { return parser::make_LIST_OPEN(tokenRange()); };
        ")"       => { return parser::make_LIST_CLOSE(tokenRange()); };
        "["       => { return parser::make_DICT_OPEN(tokenRange()); };
        "]"       => { return parser::make_DICT_CLOSE(tokenRange()); };

        num       => {
            DUMP_TOKEN(FLOAT, token_float);
            return parser::make_FLOAT(token_float, tokenRange());
        };

        property =>  {
            saveSymbol();
            DUMP_TOKEN(PROPERTY, text_);
            return parser::make_PROPERTY(text_, tokenRange());
        };

        func_call_list => {
            saveFunction();
            DUMP_TOKEN(FUNC_CALL, text_);
            return parser::make_FUNC_CALL(text_, location());
        };

        data_call_list => {
            saveFunction();
            DUMP_TOKEN(DATA_CALL_LIST, text_);
            return parser::make_DATA_CALL_LIST(text_, location());
        };

        data_call_dict => {
            saveFunction();
            DUMP_TOKEN(DATA_CALL_DICT, text_);
            return parser::make_DATA_CALL_DICT(text_, location());
        };

        dict_key => {
            saveFunction();
            DUMP_TOKEN(DICT_KEY, text_);
            return parser::make_DICT_KEY(text_, location());
        };
    *|;

    write data;
}%%

# include <cstring>

namespace ceammc {
namespace parser {

static boost::string_view trim(boost::string_view src)
{
    constexpr auto SPACES = " \t\n";
    const auto first_ns = src.find_first_not_of(SPACES);

    if (first_ns != std::string::npos)
        return src.substr(first_ns, src.find_last_not_of(SPACES) - first_ns + 1);
    else
        return "";
}

DataStringLexer::DataStringLexer(string_view src)
    : source_(trim(src))
    , p(source_.begin())
    , ts(source_.begin())
    , te(source_.begin())
    , pe(source_.end())
    , eof(source_.end())
    , cs(data_string_lexer_start)
    , act(0)
{
    %% write init;
}

void DataStringLexer::saveFunction()
{
    const auto NMAX = std::min<int>(sizeof(text_) - 1, (te - ts) - 1);
    for (int i = 0; i < NMAX; i++)
        text_[i] = ts[i];

    text_[NMAX] = '\0';
}

void DataStringLexer::saveSymbol()
{
    const auto NMAX = std::min<int>(sizeof(text_) - 1, (te - ts));
    for (int i = 0; i < NMAX; i++)
        text_[i] = ts[i];

    text_[NMAX] = '\0';
}

location DataStringLexer::tokenRange() const
{
    int start = ts - source_.begin();
    int end = te - source_.begin();
    //printf("loc: %d..%d\n", start, end);
    return location{ position{&filename_, start}, position{&filename_, end }};
}

DataStringLexer::parser::symbol_type DataStringLexer::exec()
{
    if (p == pe)
        return parser::make_END(tokenRange());

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;
    t_float token_float = 0;

    %% write exec;
    printf("cs=%d\n", cs);
    DUMP_TOKEN(SYMBOL, text_);
    return parser::make_SYMBOL(text_, tokenRange());
}

DataStringLexer::parser::symbol_type DataStringLexer::next()
{
    auto result = this->exec();
    this->p = te;
    return result;
}



}
}
