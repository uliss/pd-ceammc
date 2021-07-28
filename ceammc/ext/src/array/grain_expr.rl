# include <cstddef>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <cassert>
# include <iostream>
# include <cmath>

# include "grain_expr_parser.h"
# include "grain.h"
# include "byte_code.h"
# include "ceammc_log.h"

namespace {
# define YYCOVERAGE
# include "grain_expr_lemon.h"
}

%%{

machine grain_expr;

action number_tok {
    dval = dsign * (double(dintp) + double(dfracnum) / double(dfracden));
    pushDouble(dval);
}

# function actions
action fn_abs_tok  { pushToken(TK_FN_ABS); }
action fn_sqrt_tok { pushToken(TK_FN_SQRT); }

action fn_cos_tok  { pushToken(TK_FN_COS); }
action fn_tan_tok  { pushToken(TK_FN_TAN); }
action fn_acos_tok { pushToken(TK_FN_ACOS); }
action fn_asin_tok { pushToken(TK_FN_ASIN); }
action fn_atan_tok { pushToken(TK_FN_ATAN); }

action fn_ceil_tok { pushToken(TK_FN_CEIL); }
action fn_floor_tok{ pushToken(TK_FN_FLOOR); }
action fn_round_tok{ pushToken(TK_FN_ROUND); }

action fn_log_tok  { pushToken(TK_FN_LOG); }
action fn_log10_tok{ pushToken(TK_FN_LOG10); }
action fn_log2_tok { pushToken(TK_FN_LOG2); }

action fn_frac_tok { pushToken(TK_FN_FRAC); }

# float
number_sign = '-' @{ dsign = -1; };
number_intp  = [0-9]+
    ${ (dintp *= 10) += (fc-'0'); };
number_fracp = '.' ([0-9]+
    ${ (dfracnum *= 10) += (fc-'0'); dfracden*=10; });

number = (number_intp number_fracp?)
    >{ dsign = 1; dintp = 0; dfracnum = 0; dfracden = 1; };

# math tokens
plus   = '+';
minus  = '-';
openp  = '(';
closep = ')';
times  = '*';
divide = '/';
power  = '^';
mod    = '%';

# unit tokens
unit_ms = 'ms' | 'msec';
unit_sec = 's' | 'sec';
unit_samp = 'samp';

# prop tokens
prop_amp     = '@amp';
prop_at      = '@at'; # rename!!!
prop_interp  = '@interp';
prop_len     = '@l' | '@len' | '@length';
prop_pan     = '@p' | '@pan';
prop_speed   = '@s' | '@speed';
prop_window  = '@w' | '@win' | '@window';
prop_tafter  = '@ta';
prop_tbefore = '@tb';
prop_tag     = '@tag';
prop_repeats = '@r';

# const tokens
const_amp    = '$amp';
const_at     = '$at';
const_len    = '$l' | '$len' | '$length';
const_pan    = '$p' | '$pan';
const_speed  = '$s' | '$speed';
const_sr     = '$sr';
const_niter  = '$n';
const_nblock = '$b';
const_nsamp  = '$_';
const_pi     = '$pi';
const_asize  = '$A';
const_gidx   = '$gi';

# symbol machine
symbol := |*
    ' '+ => {};
    [a-z0-9_]+ => {
        char buf[32];
        char* pbuf = buf;

        int n = 0;
        while(ts != te && n < sizeof(buf)-1) { *(pbuf++) = *(ts++); n++; }
        *pbuf = '\0';
        pushSymbol(gensym(buf));
        fgoto main;
    };
*|;

# matcher
main := |*
  number       => number_tok;
  plus         => { pushToken(TK_PLUS); };
  minus        => { pushToken(TK_MINUS); };
  times        => { pushToken(TK_TIMES); };
  divide       => { pushToken(TK_DIVIDE); };
  power        => { pushToken(TK_POW); };
  mod          => { pushToken(TK_MOD); };
  openp        => { pushToken(TK_OPENP); };
  closep       => { pushToken(TK_CLOSEP); };
  # assign
  '+='         => { pushToken(TK_ASSIGN_ADD);};
  '-='         => { pushToken(TK_ASSIGN_SUB);};
  '*='         => { pushToken(TK_ASSIGN_MUL);};
  '='          => { pushToken(TK_ASSIGN_SET);};
  ','          => { pushToken(TK_COMMA);};
  # const
  const_amp    => { pushToken(TK_CONST_AMP);         };
  const_at     => { pushToken(TK_CONST_AT);          };
  const_pan    => { pushToken(TK_CONST_PAN);         };
  const_speed  => { pushToken(TK_CONST_SPEED);       };
  const_len    => { pushToken(TK_CONST_LEN);         };
  '$tb'        => { pushToken(TK_CONST_TIME_BEFORE); };
  '$ta'        => { pushToken(TK_CONST_TIME_AFTER);  };
  # extra const
  const_sr     => { pushToken(TK_CONST_SR);     };
  const_niter  => { pushToken(TK_CONST_NITER);  };
  const_nblock => { pushToken(TK_CONST_NBLOCK); };
  const_nsamp  => { pushToken(TK_CONST_NSAMP);  };
  const_asize  => { pushToken(TK_CONST_ASIZE);  };
  const_gidx   => { pushToken(TK_CONST_GIDX);   };
  const_pi     => { pushDouble(std::acos(-1));  };
  # functions
  'abs'     => fn_abs_tok;
  'sqrt'    => fn_sqrt_tok;
  'cos'     => fn_cos_tok;
  'sin'     => { pushToken(TK_SIN); };
  'tan'     => fn_tan_tok;
  'acos'    => fn_acos_tok;
  'asin'    => fn_asin_tok;
  'atan'    => fn_atan_tok;
  'ceil'    => fn_ceil_tok;
  'floor'   => fn_floor_tok;
  'round'   => fn_round_tok;
  'log'     => fn_log_tok;
  'log10'   => fn_log10_tok;
  'log2'    => fn_log2_tok;
  'frac'    => fn_frac_tok;
  'clip'    => { pushToken(TK_CLIP); };
  'wrap'    => { pushToken(TK_WRAP); };
  'fold'    => { pushToken(TK_FOLD); };
  'rand'    => { pushToken(TK_FN_URAND); };
  'choice'  => { pushToken(TK_CHOICE); };
  # units
  unit_ms   => { pushToken(TK_MSEC); };
  unit_sec  => { pushToken(TK_SEC);  };
  unit_samp => { pushToken(TK_SAMP); };
  # props
  prop_amp     => { pushToken(TK_PROP_AMP);   };
  prop_at      => { pushToken(TK_PROP_AT);    };
  prop_len     => { pushToken(TK_PROP_LEN);   };
  prop_pan     => { pushToken(TK_PROP_PAN);   };
  prop_speed   => { pushToken(TK_PROP_SPEED); };
  prop_tbefore => { pushToken(TK_PROP_TIME_BEFORE); };
  prop_tafter  => { pushToken(TK_PROP_TIME_AFTER);  };
  prop_repeats => { pushToken(TK_PROP_REPEATS);     };
  prop_interp  => { pushToken(TK_PROP_INTERP);      };
  prop_window  => { pushToken(TK_PROP_WINDOW); };
  prop_tag     => { pushToken(TK_PROP_TAG); curtok_ = TK_PROP_TAG; fgoto symbol; };
  # misc keywords
  'hann'      =>  { pushToken(TK_HANN); };
  'rect'      =>  { pushToken(TK_RECT); };
  'tri'       =>  { pushToken(TK_TRI); };
  'trpz'      =>  { pushToken(TK_TRPZ); };
  'lup'       =>  { pushToken(TK_LINUP); };
  'ldown'     =>  { pushToken(TK_LINDOWN); };
  'ondone'    =>  { pushToken(TK_ONDONE); };
  'overflow'  =>  { pushToken(TK_OVERFLOW); };
  'mode'      =>  { pushToken(TK_MODE); };
  'none'      =>  { pushToken(TK_NONE); };
  'lin'       =>  { pushToken(TK_LINEAR); };
  'cubic'     =>  { pushToken(TK_CUBIC); };
  'mod'       =>  { pushToken(TK_MODULATE); };
  'u'?'saw'   =>  { pushToken(TK_USAW); };
  'dsaw'      =>  { pushToken(TK_DSAW); };
  'sqr'       =>  { pushToken(TK_SQR); };
  '..'        =>  { pushToken(TK_DOTS); };
  # spaces
  ' '+ => {};
*|;

}%%

%% write data;

static_assert(ceammc::GrainExprParser::PARSER_SIZE >= sizeof(yyParser), "");

namespace ceammc {

GrainExprParser::GrainExprParser(Grain* g)
    : grain_(g)
    , calc_moment_(GRAIN_CALC_ONCE)
    , res_(0)
{
    parse_grain_exprInit(parser_data_);
}

void GrainExprParser::pushToken(int token)
{
    parse_grain_expr(parser(), token, {}, this);
}

void GrainExprParser::pushDouble(double val)
{
    parse_grain_expr(parser(), TK_DOUBLE, val, this);
}

void GrainExprParser::pushSymbol(t_symbol* sym)
{
    parse_grain_expr(parser(), TK_SYMBOL, sym, this);
}

bool GrainExprParser::grainPropGet(double& val) const
{
    switch (current_prop_) {
    case GRAIN_PROP_TIME_BEFORE:
        val = grainTimeBefore();
    break;
    case GRAIN_PROP_TIME_AFTER:
        val = grainTimeAfter();
    break;
    case GRAIN_PROP_REPEATS:
        val = grainRepeats();
    break;
    case GRAIN_PROP_PAN:
        val = grainPan();
    break;
    case GRAIN_PROP_LENGTH:
        val = grainLength();
    break;
    case GRAIN_PROP_AMP:
        val = grainAmp();
    break;
    case GRAIN_PROP_AT:
        val = grainAt();
    break;
    case GRAIN_PROP_SPEED:
        val = grainSpeed();
    break;
    default:
        LIB_ERR << __FUNCTION__ << " unsupported prop: " << current_prop_;
        return false;
    }

    return true;
}

bool GrainExprParser::grainPropSet(double val)
{
    if (!grain_)
        return true;

    switch (current_prop_) {
    case GRAIN_PROP_TIME_BEFORE:
        grain_->setTimeBefore(std::max<double>(0, val));
    break;
    case GRAIN_PROP_TIME_AFTER:
        grain_->setTimeAfter(std::max<double>(0, val));
    break;
    case GRAIN_PROP_LENGTH:
        if (val <= 0) {
            LIB_ERR << "positive value expected for length property, got: " << val;
            return false;
        }
        grain_->setLengthInSamples(val);
    break;
    case GRAIN_PROP_PAN:
        grain_->setPan(res_);
    break;
    case GRAIN_PROP_AMP:
        grain_->setAmplitude(val);
    break;
    case GRAIN_PROP_REPEATS:
        grain_->setRepeats(val);
    break;
    case GRAIN_PROP_SPEED:
        grain_->setSpeed(val);
    break;
    case GRAIN_PROP_AT:
        grain_->setArrayPosInSamples(val);
    break;
    default:
        LIB_ERR << __FUNCTION__ << " unsupported prop: " << current_prop_;
        return false;
    break;
    }

    return true;
}

bool GrainExprParser::byteCodeEval()
{
    if (!grainPropGet(res_))
        return false;

    if (!bc_.eval(res_))
        return false;

    return grainPropSet(res_);
}

void GrainExprParser::saveGrainTag(t_symbol* tag)
{
    if (!grain_)
        return;

    grain_->setTag(tag);
}

void GrainExprParser::setOnDone()
{
    auto ok = current_prop_ != GRAIN_PROP_NONE && !bc_.empty();
    if (ok && grain_)
        grain_->setOnDone(current_prop_, bc_);
}

void GrainExprParser::startProp(GrainPropId prop)
{
    bc_.reset();
    if (grain_)
        grain_->initByteCodeConst(bc_);

    calc_moment_ = GRAIN_CALC_ONCE;
    current_prop_ = prop;
}

void GrainExprParser::setPanOverflow(GrainPropOverflow of)
{
    if (!grain_) return;
    grain_->setPanOverflow(of);
}

void GrainExprParser::setPanMode(GrainPan m)
{
    if (!grain_) return;
    grain_->setPanMode(m);
}

bool GrainExprParser::parse(const AtomListView& lv)
{
    reset();

    for (auto& a: lv) {
        if (!lexAtom(a)) {
            LIB_ERR << "syntax error, unexpected token: " << a;
            return false;
        }

        if (!parse_ok_) {
            LIB_ERR << "invalid token: " << a;
            return false;
        }
    }

    return parseEof();
}

void GrainExprParser::reset()
{
    // clean error message
    err_buf_[0] = '\0';
    parse_ok_ = true;
    bc_err_ = false;
    current_prop_ = GRAIN_PROP_NONE;
    res_ = 0;
    num_vargs_ = 0;
}

bool GrainExprParser::parse(const char* data)
{
    reset();

    if (!doParse(data)) {
        std::cerr << "parse error: " << data << "\n";
        return false;
    }

    return parseEof();
}

bool GrainExprParser::parseEof()
{
    pushToken(0);

    // check byte code
    if (bc_err_) {
        setErrorMsg("byte code error");
        return false;
    }

    if (!parse_ok_)
        std::cerr << err_buf_ << "\n";

    return parse_ok_;
}

bool GrainExprParser::doParse(const char* data)
{
    int cs;
    int act;
    const char* ts;
    const char* te;
    const size_t len = strlen(data);
    const char* p = data;
    const char* pe = data + len;
    const char* eof = pe;
    int dintp = 0;
    int dfracnum = 0;
    int dfracden = 1;
    double dsign = 1;
    double dval = 0;

    %% write init;
    %% write exec;

    if (cs < %%{ write first_final; }%%) {
        char buf[32] = "";
        snprintf(buf, sizeof(buf)-1, "unknown token: %s", p);
        setErrorMsg(buf);
        return false;
    } else
        return true;
}

bool GrainExprParser::lexAtom(const Atom& a)
{
    if (curtok_ == TK_PROP_TAG && a.isSymbol()) {
        pushSymbol(a.asT<t_symbol*>());
        curtok_ = -1;
        return true;
    } else if (a.isFloat()) {
        pushDouble(a.asFloat());
        return true;
    } else if(a.isSymbol()) {
        const char* str = a.asT<t_symbol*>()->s_name;
        return doParse(str);
    } else
        return false;
}

void GrainExprParser::setErrorMsg(const char* str)
{
    const size_t n = sizeof(err_buf_) - 1;
    for (size_t i = 0; i < n; i++) {
        char c = str[i];
        err_buf_[i] = c;
        if (c == '\0')
            break;
    }

    parse_ok_ = false;
}

void GrainExprParser::bcPushValue(double v)
{
    if (bc_err_)
        return;

    if (!bc_.pushByteValue(v)) {
        setErrorMsg("push byte code value");
        bc_err_ = true;
    }
}

void GrainExprParser::bcPopValue()
{
    if (bc_err_)
        return;

    if (!bc_.popByteValue()) {
        setErrorMsg("pop byte code value");
        bc_err_ = true;
    }
}

void GrainExprParser::bcPushCode(ByteCodeOp op)
{
    if (bc_err_)
        return;

    if (!bc_.pushByteCode(op)) {
        setErrorMsg("push opcode error");
        bc_err_ = true;
    }
}

void GrainExprParser::setAssignType(ByteCodeAssignType type)
{
    bc_.setAssignType(type);
}

void GrainExprParser::setWinType(GrainWindowType wt)
{
    if (grain_)
        grain_->setWinType(wt);
}

void GrainExprParser::setWinParam(double v)
{
    if (grain_)
        grain_->setWinParam(v);
}

void GrainExprParser::setInterp(GrainInterp i)
{
    if (grain_)
        grain_->setPlayInterpolation(i);
}

void GrainExprParser::setModulation(double a, double b, GrainModulation m, float freq)
{
    if (grain_)
        grain_->setModulation(current_prop_, GrainPropModulator(m, freq, a, b));
}

}
