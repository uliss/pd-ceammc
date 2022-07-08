# include "lemon_data_string_parser.h"
# include "lemon_data_string.h"
# include "parser_numeric.h"

# include <cstdint>

# ifdef NDEBUG
# undef NDEBUG
# endif
# include "lemon_data_parser_impl.h"

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
    machine lemon_data_string_lexer;
    include numeric_common "ragel_numeric.rl";

    action on_fn_list_call_init { ts = fpc; }
    action on_fn_list_call_done {
        pushSymbolToken(TK_FUNC_LIST_CALL, ts, fpc-1);
        pushToken(TK_LIST_OPEN);
    }
    action on_prop_init { ts = fpc; }
    action on_prop_done {
        pushSymbolToken(TK_PROPERTY, ts, fpc);
    }
    action on_float_done {
        switch(ragel_type) {
        case TYPE_FLOAT:
            pushDouble(ragel_num.getFloat());
            break;
        case TYPE_INT:
        case TYPE_BIN:
        case TYPE_HEX:
            pushDouble(ragel_num.getInteger());
            break;
        case TYPE_RATIO:
            pushDouble(ragel_num.getRatioAsFloat());
            break;
        default:
            break;
        }
    }
    action on_token_done {
        token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
    }

    property       = '@' [a-z_0-9?]+;
    func_call_list = [a-z][a-z_0-9]* '(';
    data_call_list = [A-Z][a-zA-Z]*  '(';
    data_call_dict = [A-Z][a-zA-Z]*  '[';
    data_call_str  = [A-Z][a-zA-Z]*  '"';
    dict_key       = [a-z_0-9?]+;
    float          = num_float | num_int | num_bin | num_hex | num_ratio;

    token = (
        space+
        | "true"            % { pushDouble(1); }
        | "false"           % { pushDouble(0); }
        | "null"            % { pushToken(TK_NULL); }
        | "("               % { pushToken(TK_LIST_OPEN);  }
        | ")"               % { pushToken(TK_LIST_CLOSE); }
        | "["               % { pushToken(TK_DICT_OPEN);  }
        | "]"               % { pushToken(TK_DICT_CLOSE); }
        | float
                % on_float_done
        | property
                >on_prop_init
                %on_prop_done
        | func_call_list
                >on_fn_list_call_init
                %on_fn_list_call_done

    ) % on_token_done;

    main := space* token (space+ token)* space*
    0 @{ pushToken(0); fbreak; }
    ;
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
    std::cerr << __FUNCTION__ << ' ' << token << '\n';
    lemon_data_string_parser(parser(), token, {}, this);
}

void LemonDataStringParser::pushDouble(double val)
{
    std::cerr << __FUNCTION__ << " TK_FLOAT " << val << '\n';
    lemon_data_string_parser(parser(), TK_FLOAT, val, this);
}

void LemonDataStringParser::pushSymbolToken(int token, const char* begin, const char* end)
{
    const auto N = std::min<int>(sizeof(parser_buf_) - 1, end - begin);

    for (int i = 0; i < N; i++)
        parser_buf_[i] = begin[i];

    parser_buf_[N] = 0;
    std::cerr << __FUNCTION__ << " token '" << parser_buf_ << "'\n";
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
    if (data == nullptr)
        return false;

    if (data[0] == '\0')
        return true;

    int cs;
    int act;
    const char* ts = 0;
    const char* te = 0;
    const char* p = data;
    parse_ok_ = true;

    t_float token_float = 0;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    %% write init;
    %% write exec noend;

    if (cs < %%{ write first_final; }%%) {
        char buf[32] = "";
        snprintf(buf, sizeof(buf)-1, "unknown token: '%s'", p);
        setErrorMsg(buf);
        return false;
    } else {
        if (!parse_ok_) {
            pushToken(0);
            return false;
        } else
            return true;
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
