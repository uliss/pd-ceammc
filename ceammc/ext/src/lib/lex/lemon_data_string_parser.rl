# include "lemon_data_string_parser.h"
# include "lemon_data_string.h"
# include "parser_numeric.h"

# include <cstdint>
# include <boost/static_string.hpp>

# ifdef NDEBUG
# undef NDEBUG
# endif
# include "lemon_data_parser_impl.h"

namespace {

constexpr auto TK_EOF = 0;

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
    machine lemon_data_string_lexer;
    include numeric_common "ragel_numeric.rl";

    true           = "true";
    false          = "false";
    tok_space      = space+;
    tok_null       = "null";
    tok_lpar       = "(";
    tok_rpar       = ")";
    tok_lbr        = "[";
    tok_rbr        = "]";
    tok_squote     = "'";
    tok_dquote     = '"';
    func_call_list = [a-z][a-z_0-9]* '(';
    data_call_list = [A-Z][a-zA-Z]*  '(';
    data_call_dict = [A-Z][a-zA-Z]*  '[';
    # data_call_str  = [A-Z][a-zA-Z]*  '"';
    # dict_key       = [a-z_0-9?]+;
    float          = num_float | num_int | num_bin | num_hex | num_ratio;

    tok_all = true
        | false
        | tok_space
        | tok_null
        | tok_lpar
        | tok_rpar
        | tok_lbr
        | tok_dquote
        | tok_squote
        | func_call_list
        | data_call_list
        | data_call_dict
        | float;

    other = (any+) -- tok_all;

    str_escape = '`';

    # NOTE: changes empty_str
    sqstring := |*
        ^(str_escape | tok_squote) =>   { ragel_string += fc;   };
        str_escape tok_squote      =>   { ragel_string += '\''; };
        str_escape space           =>   { ragel_string += ' '; };
        str_escape str_escape      =>   { ragel_string += '`'; };
        tok_squote                 =>   { pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); fret; };
    *|;

    # NOTE: changes empty_str
    dqstring := |*
        ^(str_escape | tok_dquote) =>   { ragel_string += fc;  };
        str_escape tok_dquote      =>   { ragel_string += '"'; };
        str_escape space           =>   { ragel_string += ' '; };
        str_escape str_escape      =>   { ragel_string += '`'; };
        tok_dquote                 =>   { pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); fret; };
    *|;

    token := |*
        true      => { pushFloat(1); };
        false     => { pushFloat(0); };
        tok_null  => { pushToken(TK_NULL); };
        tok_lpar  => { pushToken(TK_LIST_OPEN); };
        tok_rpar  => { pushToken(TK_LIST_CLOSE); };
        tok_lbr   => { pushToken(TK_DICT_OPEN); };
        tok_rbr   => { pushToken(TK_DICT_CLOSE); };
        tok_dquote => { ragel_string.clear(); fcall dqstring; };
        tok_squote => { ragel_string.clear(); fcall sqstring; };

        float     => {
            switch(ragel_type) {
            case TYPE_FLOAT:
                pushFloat(ragel_num.getFloat());
                break;
            case TYPE_INT:
            case TYPE_BIN:
            case TYPE_HEX:
                pushFloat(ragel_num.getInteger());
                break;
            case TYPE_RATIO:
                pushFloat(ragel_num.getRatioAsFloat());
                break;
            default:
                break;
            }

            ragel_num = {};
            ragel_cat = CAT_UNKNOWN;
            ragel_type = TYPE_UNKNOWN;
        };

        func_call_list => {
            pushSymbolToken(TK_FUNC_LIST_CALL, ts, te-1);
            pushToken(TK_LIST_OPEN);
        };

        data_call_list => {
            pushSymbolToken(TK_DATA_NAME, ts, te-1);
            pushToken(TK_LIST_OPEN);
        };

        data_call_dict => {
            pushSymbolToken(TK_DATA_NAME, ts, te-1);
            pushToken(TK_DICT_OPEN);
        };

        tok_space =>      { pushToken(TK_SPACE); fret; };

        other  => { pushSymbolToken(TK_SYMBOL, ts, te); };
    *|;

    main := space** ((any-space) >{ fhold; fcall token; } space**)*;
}%%

# include <cstring>

namespace ceammc {
namespace parser {

%% write data;

static_assert(LemonDataStringParser::PARSER_SIZE >= sizeof(yyParser), "");

LemonDataStringParser::LemonDataStringParser()
{
    reset();
    lemon_data_string_parserInit(parser_data_);
    lemon_data_string_parserTrace(stderr, (char*)"\t> ");
}

LemonDataStringParser::~LemonDataStringParser()
{
    lemon_data_string_parserFinalize(parser_data_);
}

void LemonDataStringParser::pushToken(int token)
{
    lemon_data_string_parser(parser(), token, {}, this);
}

void LemonDataStringParser::pushFloat(double val)
{
    lemon_data_string_parser(parser(), TK_FLOAT, val, this);
}

void LemonDataStringParser::pushSymbolToken(int token, const char* begin, const char* end)
{
    const auto N = std::min<int>(sizeof(parser_buf_) - 1, end - begin);

    for (int i = 0; i < N; i++)
        parser_buf_[i] = begin[i];

    parser_buf_[N] = 0;
    lemon_data_string_parser(parser(), token, parser_buf_, this);
}

bool LemonDataStringParser::parse(const char* str)
{
    reset();

    std::cerr << "parse: '" << str << "'\n";

    if (!doParse(str)) {
        std::cerr << "parse error: '" << str << "'\n";
        return false;
    }

    return true;
}

bool LemonDataStringParser::doParse(const char* data)
{
    // null string pointer
    if (data == nullptr)
        return false;

    // empty string
    if (data[0] == '\0')
        return true;

    int cs;
    int act;
    const char* ts = 0;
    const char* te = 0;
    const char* p = data;
    const char* pe = data + strlen(data);
    const char* eof = pe;
    parse_ok_ = false;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    // for quoted string parser
    boost::static_string<512> ragel_string;

    try {

    %% write init;
    %% write exec;

    } catch(std::exception& e) {
        setErrorMsg(e.what());
        return false;
    }

    if (cs < %%{ write first_final; }%%) {
        char buf[32] = "";
        snprintf(buf, sizeof(buf)-1, "unknown token: '%s'", ts);
        setErrorMsg(buf);
        return false;
    } else {
        pushToken(0);
        return parse_ok_;
        if (!parse_ok_) {
            pushToken(0);
            return false;
        } else {
            return true;
        }
    }
}

void LemonDataStringParser::reset()
{
    // clean error message
    err_buf_[0] = '\0';
    parse_ok_ = true;
    res_.clear();
    prop_.clear();
}

void LemonDataStringParser::pPushProp(t_symbol* name)
{
    prop_.insert(0, name);
}

void LemonDataStringParser::pPushPropAtom(const t_atom& a)
{
    prop_.append(a);
}

void LemonDataStringParser::pPushListAtom(const t_atom& a)
{
    res_.append(a);
}

void LemonDataStringParser::setErrorMsg(const char* msg)
{
    snprintf(err_buf_, sizeof(err_buf_)-1, "%s", msg);
    std::cerr << msg << "\n";
    parse_ok_ = false;
}

void LemonDataStringParser::stackOverflow()
{
    setErrorMsg("stack overflow");
}

void LemonDataStringParser::parseFailure()
{
    setErrorMsg("parse failure");
}


}
}
