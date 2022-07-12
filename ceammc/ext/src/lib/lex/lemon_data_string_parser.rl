# include "lemon_data_string_parser.h"
# include "lemon_data_string.h"
# include "parser_numeric.h"

# include <cstdint>
# include <cstdlib>
# include <boost/static_string.hpp>

# include "lemon_data_parser_impl.h"

%%{
    machine lemon_data_string_lexer;
    include numeric_common "ragel_numeric.rl";
    include string_common  "ragel_strings.rl";

    # actions
    action on_true  { pushFloat(1); }
    action on_false { pushFloat(0); }
    action on_null  { pushToken(TK_NULL); }
    action on_space { pushToken(TK_SPACE); }
    action on_lpar  { pushToken(TK_LIST_OPEN); }
    action on_rpar  { pushToken(TK_LIST_CLOSE); }
    action on_float {
        onFloat(ragel_cat, ragel_type, ragel_num);
        ragel_num = {};
        ragel_cat = CAT_UNKNOWN;
        ragel_type = TYPE_UNKNOWN;
    }
    action on_double_quote { ragel_string.clear(); fcall scan_dqstring; }
    action on_single_quote { ragel_string.clear(); fcall scan_sqstring; }
    action on_fn_call {
        // skip trailing whitespaces
        auto te0 = te;
        while(*(--te0) == ' ') ;
        pushSymbolToken(TK_FUNC_LIST_CALL, ts, te0);
        pushToken(TK_LIST_OPEN);
    }
    action on_data_list  {
        // skip trailing whitespaces
        auto te0 = te;
        while(*(--te0) == ' ') ;
        pushSymbolToken(TK_DATA_NAME, ts, te0);
        pushToken(TK_LIST_OPEN);
    }
    action on_data_dict {
        pushSymbolToken(TK_DATA_NAME, ts, te-1);
        pushToken(TK_DICT_OPEN);
        fcall scan_dict;
    }
    action on_dict_start { pushToken(TK_DICT_OPEN); fcall scan_dict; }
    action on_dict_key {
        // skip starting whitespaces
        auto ts0 = ts;
        while(*ts0 == ' ') ++ts0;
        // skip trailing whitespaces
        auto te0 = te;
        while(*(--te0) == ' ') ;
        pushSymbolToken(TK_DICT_KEY, ts0, te0);
    }
    action on_env_variable {
        constexpr int BS = sizeof(parser_buf_) - 1;
        const auto N = std::min<int>(BS, (te - ts) - 2);
        for (int i = 0; i < N; i++)
            parser_buf_[i] = ts[i+1];

        parser_buf_[N] = 0;
        auto env = std::getenv(parser_buf_);

        if (env) {
            for (int i = 0; i < 256; i++) {
                bool break_loop = false;
                auto ch = env[i];
                switch (ch) {
                case '\'': ragel_string += '`'; ragel_string += '\''; break;
                case '"':  ragel_string += '`'; ragel_string += '"'; break;
                case '\0': break_loop = true; break;
                default:   ragel_string += ch; break;
                }

                if (break_loop) break;
            }
        } else
            ragel_string.append(ts, te - ts);
    }

    action on_data_sqstring { pushToken(TK_STRING); ragel_string.clear(); fcall scan_sqstring; }
    action on_data_dqstring { pushToken(TK_STRING); ragel_string.clear(); fcall scan_dqstring; }

    action on_other { pushSymbolToken(TK_SYMBOL, ts, te); }

    true           = "#true";
    false          = "#false";
    tok_space      = space+;
    tok_null       = "#null";
    tok_lpar       = "(" space**;
    tok_rpar       = space** ")";
    tok_lbr        = "[" space**;
    tok_rbr        = space** "]";
    func_call_list = [a-z][a-z_0-9]* '(' space*;
    data_call_list = [A-Z][a-zA-Z]*  '(' space*;
    data_call_dict = [A-Z][a-zA-Z]*  '[';
    data_sqstring  = "S'";
    data_dqstring  = 'S"';
    # matrix syntax:
    # #[(1 ...3) (2...)]
    # #M:N[(1 ...3) (2...)]
    matrix_cols    = [1-9] @{ ragel_mtx_cols = fc - '0'; } [0-9]? @{ (ragel_mtx_cols *= 10) += (fc - '0'); };
    matrix_rows    = [1-9] @{ ragel_mtx_rows = fc - '0'; } [0-9]? @{ (ragel_mtx_rows *= 10) += (fc - '0'); };
    matrix_delim   = ':';
    data_matrix    = '#' (matrix_rows matrix_delim matrix_cols)? '[';
    float          = (num_float | num_int | num_bin | num_hex | num_ratio) >{ ragel_num = {}; };

    tok_all = true
        | false
        | tok_space
        | tok_null
        | tok_lpar
        | tok_rpar
        | tok_lbr
        | str_dquote
        | str_squote
        | data_sqstring
        | data_dqstring
        | data_matrix
        | func_call_list
        | data_call_list
        | data_call_dict
        | float;

    tok_other = ((any+) -- (tok_all - (true|false|tok_null|float)));

    action on_quote_end  { pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); fret; }

    scan_sqstring := |*
        ^(str_escape | str_squote) => { ragel_string += fc;   };
        "`'"                       => { ragel_string += '\''; };
        esc_space                  => { ragel_string += ' '; };
        esc_comma                  => { ragel_string += ','; };
        esc_semicolon              => { ragel_string += ';'; };
        esc_escape                 => { ragel_string += '`'; };
        esc_slash                  => { ragel_string += '\\'; };
        str_envvar                 => on_env_variable;
        str_squote                 => on_quote_end;
    *|;

    scan_dqstring := |*
        ^(str_escape | str_dquote) => { ragel_string += fc;  };
        '`"'                       => { ragel_string += '"'; };
        esc_space                  => { ragel_string += ' '; };
        esc_comma                  => { ragel_string += ','; };
        esc_semicolon              => { ragel_string += ';'; };
        esc_escape                 => { ragel_string += '`'; };
        esc_slash                  => { ragel_string += '\\'; };
        str_envvar                 => on_env_variable;
        str_dquote                 => on_quote_end;
    *|;

    action on_data_matrix {
        pushToken(TK_MATRIX);
        pushFloat(ragel_mtx_rows);
        pushFloat(ragel_mtx_cols);
        fcall scan_matrix;
    }

    scan_matrix := |*
        float     => on_float;
        tok_lpar  => on_lpar;
        tok_rpar  => on_rpar;
        tok_space => {};
        tok_rbr   => { pushToken(TK_MATRIX_CLOSE); fret; };
    *|;

    dict_key_id   = [a-zA-Z_0-9?@]+;
    dict_key0     = dict_key_id | ('"' [a-z_0-9?]+ '"') | ("'" [a-z_0-9?]+ "'");
    dict_key      = space** dict_key0 ':' space**;
    scan_dict_other = tok_other -- (dict_key | tok_rbr | tok_space);

    scan_dict := |*
        true       => on_true;
        false      => on_false;
        tok_null   => on_null;
        tok_lpar   => on_lpar;
        tok_rpar   => on_rpar;
        tok_lbr    => on_dict_start;
        str_dquote => on_double_quote;
        str_squote => on_single_quote;

        float          => on_float;
        func_call_list => on_fn_call;
        data_call_list => on_data_list;
        data_call_dict => on_data_dict;
        data_sqstring  => on_data_sqstring;
        data_dqstring  => on_data_dqstring;
        data_matrix    => on_data_matrix;

        dict_key         => on_dict_key;
        tok_space        => { pushToken(TK_SPACE); };
        scan_dict_other  => on_other;

        # return token
        tok_rbr          => { pushToken(TK_DICT_CLOSE); fret; };
    *|;

    scan_token := |*
        true       => on_true;
        false      => on_false;
        tok_null   => on_null;
        tok_lpar   => on_lpar;
        tok_rpar   => on_rpar;
        tok_lbr    => { pushToken(TK_DICT_OPEN); fcall scan_dict; };
        str_dquote => on_double_quote;
        str_squote => on_single_quote;

        float          => on_float;
        func_call_list => on_fn_call;
        data_call_list => on_data_list;
        data_call_dict => on_data_dict;
        data_sqstring  => on_data_sqstring;
        data_dqstring  => on_data_dqstring;
        data_matrix    => on_data_matrix;

        tok_other      => on_other;
        # return token
        tok_space      => { pushToken(TK_SPACE); fret; };
    *|;

    main := ((any) >{ fhold; fcall scan_token; } space**)*;
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

# ifndef NDEBUG
    lemon_data_string_parserTrace(stderr, (char*)"\t> ");
# endif
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
# ifndef NDEBUG
    std::cerr << __FUNCTION__ << ' ' << val << std::endl;
# endif

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

# ifndef NDEBUG
    std::cerr << "parse: '" << str << "'\n";
# endif

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

    // ragel state
    int cs;
    // ragel action
    int act;
    int top;
    int stack[16];
    // scanner match begin/end
    const char* ts = 0;
    const char* te = 0;
    // text start
    const char* p = data;
    // remove initial spaces
    while (*p == ' ') p++;
    // text end
    const char* pe = p + strlen(p);
    // remove last spaces
    while (pe != p && *(pe-1) == ' ') --pe;
    // EOF
    const char* eof = pe;

    parse_ok_ = true;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    // for quoted string parser
    boost::static_string<512> ragel_string;
    // for matrices
    int ragel_mtx_cols = 0;
    int ragel_mtx_rows = 0;

    try {

    %% write init;
    %% write exec;

    } catch(std::exception& e) {
        LIB_ERR << e.what();
        return false;
    }

    if (cs < %%{ write first_final; }%%) {
        char buf[32] = "";
        snprintf(buf, sizeof(buf)-1, "unknown token: '%s'", ts);
        setErrorMsg(buf);
        return false;
    } else {
        if (parse_ok_)
            pushToken(0);

        return parse_ok_;
    }
}

void LemonDataStringParser::reset()
{
    // clean error message
    err_buf_[0] = '\0';
    parse_ok_ = true;
    res_.clear();
}

void LemonDataStringParser::pPushListAtom(const t_atom& a)
{
    res_.push_back(a);
}

void LemonDataStringParser::setErrorMsg(const char* msg)
{
    LIB_ERR << msg;
    parse_ok_ = false;
    throw std::runtime_error(msg);
}

void LemonDataStringParser::stackOverflow()
{
    setErrorMsg("stack overflow");
}

void LemonDataStringParser::parseFailure()
{
    setErrorMsg("parse failure");
}

void LemonDataStringParser::onFloat(AtomCategory cat, AtomType type, const fsm::NumericData& num) {
    switch(type) {
    case TYPE_FLOAT:
        return pushFloat(num.getFloat());
    case TYPE_INT:
    case TYPE_BIN:
    case TYPE_HEX:
        return pushFloat(num.getInteger());
    case TYPE_RATIO:
        return pushFloat(num.getRatioAsFloat());
    default:
        break;
    }
}


}
}
