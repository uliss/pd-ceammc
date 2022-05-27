
#line 1 "grain_expr.rl"
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


#line 207 "grain_expr.rl"



#line 27 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
static const int grain_expr_start = 117;
static const int grain_expr_first_final = 117;
static const int grain_expr_error = 0;

static const int grain_expr_en_symbol = 140;
static const int grain_expr_en_main = 117;


#line 210 "grain_expr.rl"

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

    
#line 266 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	{
	cs = grain_expr_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 439 "grain_expr.rl"
    
#line 276 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr0:
#line 145 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_ASIZE);  }}
	goto st117;
tr2:
#line 144 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_NSAMP);  }}
	goto st117;
tr4:
#line 143 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_NBLOCK); }}
	goto st117;
tr7:
#line 142 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_NITER);  }}
	goto st117;
tr12:
#line 134 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_AT);          }}
	goto st117;
tr13:
#line 133 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_AMP);         }}
	goto st117;
tr14:
#line 146 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_GIDX);   }}
	goto st117;
tr15:
#line 137 "grain_expr.rl"
	{{p = ((te))-1;}{ pushToken(TK_CONST_LEN);         }}
	goto st117;
tr18:
#line 137 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_LEN);         }}
	goto st117;
tr19:
#line 135 "grain_expr.rl"
	{{p = ((te))-1;}{ pushToken(TK_CONST_PAN);         }}
	goto st117;
tr20:
#line 135 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_PAN);         }}
	goto st117;
tr21:
#line 136 "grain_expr.rl"
	{{p = ((te))-1;}{ pushToken(TK_CONST_SPEED);       }}
	goto st117;
tr24:
#line 136 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_SPEED);       }}
	goto st117;
tr25:
#line 139 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_TIME_AFTER);  }}
	goto st117;
tr26:
#line 138 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_TIME_BEFORE); }}
	goto st117;
tr27:
#line 202 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_DOTS); }}
	goto st117;
tr28:
#line 23 "grain_expr.rl"
	{{p = ((te))-1;}{
    dval = dsign * (double(dintp) + double(dfracnum) / double(dfracden));
    pushDouble(dval);
}}
	goto st117;
tr34:
#line 181 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_PROP_REPEATS);     }}
	goto st117;
tr39:
#line 175 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_PROP_AT);    }}
	goto st117;
tr40:
#line 174 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_PROP_AMP);   }}
	goto st117;
tr45:
#line 182 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_PROP_INTERP);      }}
	goto st117;
tr46:
#line 176 "grain_expr.rl"
	{{p = ((te))-1;}{ pushToken(TK_PROP_LEN);   }}
	goto st117;
tr49:
#line 176 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_PROP_LEN);   }}
	goto st117;
tr50:
#line 177 "grain_expr.rl"
	{{p = ((te))-1;}{ pushToken(TK_PROP_PAN);   }}
	goto st117;
tr51:
#line 177 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_PROP_PAN);   }}
	goto st117;
tr52:
#line 178 "grain_expr.rl"
	{{p = ((te))-1;}{ pushToken(TK_PROP_SPEED); }}
	goto st117;
tr55:
#line 178 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_PROP_SPEED); }}
	goto st117;
tr57:
#line 179 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_PROP_TIME_BEFORE); }}
	goto st117;
tr58:
#line 183 "grain_expr.rl"
	{{p = ((te))-1;}{ pushToken(TK_PROP_WINDOW); }}
	goto st117;
tr61:
#line 183 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_PROP_WINDOW); }}
	goto st117;
tr66:
#line 29 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_ABS); }}
	goto st117;
tr68:
#line 34 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_ACOS); }}
	goto st117;
tr70:
#line 35 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_ASIN); }}
	goto st117;
tr72:
#line 36 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_ATAN); }}
	goto st117;
tr79:
#line 38 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_CEIL); }}
	goto st117;
tr83:
#line 168 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CHOICE); }}
	goto st117;
tr85:
#line 164 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CLIP); }}
	goto st117;
tr86:
#line 32 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_COS); }}
	goto st117;
tr89:
#line 197 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CUBIC); }}
	goto st117;
tr92:
#line 200 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_DSAW); }}
	goto st117;
tr98:
#line 39 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_FLOOR); }}
	goto st117;
tr100:
#line 166 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FOLD); }}
	goto st117;
tr102:
#line 46 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_FRAC); }}
	goto st117;
tr105:
#line 186 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_HANN); }}
	goto st117;
tr112:
#line 191 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_LINDOWN); }}
	goto st117;
tr113:
#line 196 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_LINEAR); }}
	goto st117;
tr115:
#line 42 "grain_expr.rl"
	{{p = ((te))-1;}{ pushToken(TK_FN_LOG); }}
	goto st117;
tr116:
#line 43 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_LOG10); }}
	goto st117;
tr117:
#line 190 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_LINUP); }}
	goto st117;
tr121:
#line 170 "grain_expr.rl"
	{{p = ((te))-1;}{ pushToken(TK_MSEC); }}
	goto st117;
tr122:
#line 170 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_MSEC); }}
	goto st117;
tr125:
#line 195 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_NONE); }}
	goto st117;
tr131:
#line 192 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_ONDONE); }}
	goto st117;
tr137:
#line 193 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_OVERFLOW); }}
	goto st117;
tr142:
#line 167 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_URAND); }}
	goto st117;
tr144:
#line 187 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_RECT); }}
	goto st117;
tr147:
#line 40 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_ROUND); }}
	goto st117;
tr148:
#line 171 "grain_expr.rl"
	{{p = ((te))-1;}{ pushToken(TK_SEC);  }}
	goto st117;
tr150:
#line 199 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_USAW); }}
	goto st117;
tr151:
#line 172 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_SAMP); }}
	goto st117;
tr152:
#line 171 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_SEC);  }}
	goto st117;
tr153:
#line 152 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_SIN); }}
	goto st117;
tr157:
#line 33 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_TAN); }}
	goto st117;
tr158:
#line 188 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_TRI); }}
	goto st117;
tr160:
#line 189 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_TRPZ); }}
	goto st117;
tr165:
#line 165 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_WRAP); }}
	goto st117;
tr168:
#line 123 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_MOD); }}
	goto st117;
tr169:
#line 124 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_OPENP); }}
	goto st117;
tr170:
#line 125 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CLOSEP); }}
	goto st117;
tr173:
#line 131 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_COMMA);}}
	goto st117;
tr176:
#line 121 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_DIVIDE); }}
	goto st117;
tr178:
#line 130 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_ASSIGN_SET);}}
	goto st117;
tr180:
#line 122 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_POW); }}
	goto st117;
tr195:
#line 204 "grain_expr.rl"
	{te = p;p--;}
	goto st117;
tr196:
#line 137 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_CONST_LEN);         }}
	goto st117;
tr199:
#line 135 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_CONST_PAN);         }}
	goto st117;
tr201:
#line 147 "grain_expr.rl"
	{te = p+1;{ pushDouble(std::acos(-1));  }}
	goto st117;
tr202:
#line 136 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_CONST_SPEED);       }}
	goto st117;
tr204:
#line 141 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_CONST_SR);     }}
	goto st117;
tr205:
#line 120 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_TIMES); }}
	goto st117;
tr206:
#line 129 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_ASSIGN_MUL);}}
	goto st117;
tr207:
#line 118 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_PLUS); }}
	goto st117;
tr208:
#line 127 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_ASSIGN_ADD);}}
	goto st117;
tr209:
#line 119 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_MINUS); }}
	goto st117;
tr210:
#line 128 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_ASSIGN_SUB);}}
	goto st117;
tr211:
#line 23 "grain_expr.rl"
	{te = p;p--;{
    dval = dsign * (double(dintp) + double(dfracnum) / double(dfracden));
    pushDouble(dval);
}}
	goto st117;
tr214:
#line 176 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_PROP_LEN);   }}
	goto st117;
tr217:
#line 177 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_PROP_PAN);   }}
	goto st117;
tr219:
#line 178 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_PROP_SPEED); }}
	goto st117;
tr221:
#line 180 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_PROP_TIME_AFTER);  }}
	goto st117;
tr222:
#line 184 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_PROP_TAG); curtok_ = TK_PROP_TAG; {goto st140;} }}
	goto st117;
tr223:
#line 183 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_PROP_WINDOW); }}
	goto st117;
tr226:
#line 42 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_FN_LOG); }}
	goto st117;
tr228:
#line 44 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_LOG2); }}
	goto st117;
tr229:
#line 198 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_MODULATE); }}
	goto st117;
tr230:
#line 194 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_MODE); }}
	goto st117;
tr231:
#line 170 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_MSEC); }}
	goto st117;
tr233:
#line 171 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_SEC);  }}
	goto st117;
tr238:
#line 201 "grain_expr.rl"
	{te = p;p--;{ pushToken(TK_SQR); }}
	goto st117;
tr239:
#line 30 "grain_expr.rl"
	{te = p+1;{ pushToken(TK_FN_SQRT); }}
	goto st117;
st117:
#line 1 "NONE"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof117;
case 117:
#line 1 "NONE"
	{ts = p;}
#line 696 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	switch( (*p) ) {
		case 32: goto st118;
		case 36: goto st1;
		case 37: goto tr168;
		case 40: goto tr169;
		case 41: goto tr170;
		case 42: goto st123;
		case 43: goto st124;
		case 44: goto tr173;
		case 45: goto st125;
		case 46: goto st13;
		case 47: goto tr176;
		case 61: goto tr178;
		case 64: goto st15;
		case 94: goto tr180;
		case 97: goto st34;
		case 99: goto st42;
		case 100: goto st55;
		case 102: goto st58;
		case 104: goto st66;
		case 108: goto st69;
		case 109: goto st77;
		case 110: goto st80;
		case 111: goto st83;
		case 114: goto st94;
		case 115: goto tr191;
		case 116: goto st107;
		case 117: goto st111;
		case 119: goto st114;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr177;
	goto st0;
st0:
cs = 0;
	goto _out;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	if ( (*p) == 32 )
		goto st118;
	goto tr195;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 65: goto tr0;
		case 95: goto tr2;
		case 97: goto st2;
		case 98: goto tr4;
		case 103: goto st4;
		case 108: goto tr6;
		case 110: goto tr7;
		case 112: goto tr8;
		case 115: goto tr9;
		case 116: goto st12;
	}
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 109: goto st3;
		case 116: goto tr12;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 112 )
		goto tr13;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 105 )
		goto tr14;
	goto st0;
tr6:
#line 1 "NONE"
	{te = p+1;}
	goto st119;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
#line 788 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	if ( (*p) == 101 )
		goto st5;
	goto tr196;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 110 )
		goto tr16;
	goto tr15;
tr16:
#line 1 "NONE"
	{te = p+1;}
	goto st120;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
#line 807 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	if ( (*p) == 103 )
		goto st6;
	goto tr196;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 116 )
		goto st7;
	goto tr15;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 104 )
		goto tr18;
	goto tr15;
tr8:
#line 1 "NONE"
	{te = p+1;}
	goto st121;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
#line 833 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	switch( (*p) ) {
		case 97: goto st8;
		case 105: goto tr201;
	}
	goto tr199;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 110 )
		goto tr20;
	goto tr19;
tr9:
#line 1 "NONE"
	{te = p+1;}
	goto st122;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
#line 854 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	switch( (*p) ) {
		case 112: goto st9;
		case 114: goto tr204;
	}
	goto tr202;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 101 )
		goto st10;
	goto tr21;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 101 )
		goto st11;
	goto tr21;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 100 )
		goto tr24;
	goto tr21;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 97: goto tr25;
		case 98: goto tr26;
	}
	goto st0;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	if ( (*p) == 61 )
		goto tr206;
	goto tr205;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	if ( (*p) == 61 )
		goto tr208;
	goto tr207;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	if ( (*p) == 61 )
		goto tr210;
	goto tr209;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 46 )
		goto tr27;
	goto st0;
tr177:
#line 1 "NONE"
	{te = p+1;}
#line 56 "grain_expr.rl"
	{ dsign = 1; dintp = 0; dfracnum = 0; dfracden = 1; }
#line 51 "grain_expr.rl"
	{ (dintp *= 10) += ((*p)-'0'); }
	goto st126;
tr213:
#line 1 "NONE"
	{te = p+1;}
#line 51 "grain_expr.rl"
	{ (dintp *= 10) += ((*p)-'0'); }
	goto st126;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
#line 936 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	if ( (*p) == 46 )
		goto st14;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr213;
	goto tr211;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto tr28;
tr29:
#line 53 "grain_expr.rl"
	{ (dfracnum *= 10) += ((*p)-'0'); dfracden*=10; }
	goto st127;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
#line 957 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto tr211;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 97: goto st16;
		case 105: goto st18;
		case 108: goto tr32;
		case 112: goto tr33;
		case 114: goto tr34;
		case 115: goto tr35;
		case 116: goto st30;
		case 119: goto tr37;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 109: goto st17;
		case 116: goto tr39;
	}
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 112 )
		goto tr40;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 110 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 116 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 101 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 114 )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 112 )
		goto tr45;
	goto st0;
tr32:
#line 1 "NONE"
	{te = p+1;}
	goto st128;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
#line 1035 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	if ( (*p) == 101 )
		goto st23;
	goto tr214;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 110 )
		goto tr47;
	goto tr46;
tr47:
#line 1 "NONE"
	{te = p+1;}
	goto st129;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
#line 1054 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	if ( (*p) == 103 )
		goto st24;
	goto tr214;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 116 )
		goto st25;
	goto tr46;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 104 )
		goto tr49;
	goto tr46;
tr33:
#line 1 "NONE"
	{te = p+1;}
	goto st130;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
#line 1080 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	if ( (*p) == 97 )
		goto st26;
	goto tr217;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 110 )
		goto tr51;
	goto tr50;
tr35:
#line 1 "NONE"
	{te = p+1;}
	goto st131;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
#line 1099 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	if ( (*p) == 112 )
		goto st27;
	goto tr219;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 101 )
		goto st28;
	goto tr52;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 101 )
		goto st29;
	goto tr52;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 100 )
		goto tr55;
	goto tr52;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 97: goto st132;
		case 98: goto tr57;
	}
	goto st0;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
	if ( (*p) == 103 )
		goto tr222;
	goto tr221;
tr37:
#line 1 "NONE"
	{te = p+1;}
	goto st133;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
#line 1148 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	if ( (*p) == 105 )
		goto st31;
	goto tr223;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 110 )
		goto tr59;
	goto tr58;
tr59:
#line 1 "NONE"
	{te = p+1;}
	goto st134;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
#line 1167 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	if ( (*p) == 100 )
		goto st32;
	goto tr223;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 111 )
		goto st33;
	goto tr58;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 119 )
		goto tr61;
	goto tr58;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 98: goto st35;
		case 99: goto st36;
		case 115: goto st38;
		case 116: goto st40;
	}
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 115 )
		goto tr66;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 111 )
		goto st37;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 115 )
		goto tr68;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 105 )
		goto st39;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 110 )
		goto tr70;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 97 )
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 110 )
		goto tr72;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 101: goto st43;
		case 104: goto st45;
		case 108: goto st49;
		case 111: goto st51;
		case 117: goto st52;
	}
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 105 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 108 )
		goto tr79;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 111 )
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 105 )
		goto st47;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 99 )
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 101 )
		goto tr83;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) == 105 )
		goto st50;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 112 )
		goto tr85;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 115 )
		goto tr86;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 98 )
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 105 )
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 99 )
		goto tr89;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 115 )
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 97 )
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 119 )
		goto tr92;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 108: goto st59;
		case 111: goto st62;
		case 114: goto st64;
	}
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 111 )
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 111 )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 114 )
		goto tr98;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 108 )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) == 100 )
		goto tr100;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 97 )
		goto st65;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 99 )
		goto tr102;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 97 )
		goto st67;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 110 )
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 110 )
		goto tr105;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 100: goto st70;
		case 105: goto st73;
		case 111: goto st74;
		case 117: goto st76;
	}
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 111 )
		goto st71;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( (*p) == 119 )
		goto st72;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) == 110 )
		goto tr112;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( (*p) == 110 )
		goto tr113;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	if ( (*p) == 103 )
		goto tr114;
	goto st0;
tr114:
#line 1 "NONE"
	{te = p+1;}
	goto st135;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
#line 1496 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	switch( (*p) ) {
		case 49: goto st75;
		case 50: goto tr228;
	}
	goto tr226;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) == 48 )
		goto tr116;
	goto tr115;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	if ( (*p) == 112 )
		goto tr117;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 111: goto st78;
		case 115: goto tr119;
	}
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 100 )
		goto st136;
	goto st0;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	if ( (*p) == 101 )
		goto tr230;
	goto tr229;
tr119:
#line 1 "NONE"
	{te = p+1;}
	goto st137;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
#line 1547 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	if ( (*p) == 101 )
		goto st79;
	goto tr231;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( (*p) == 99 )
		goto tr122;
	goto tr121;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	if ( (*p) == 111 )
		goto st81;
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	if ( (*p) == 110 )
		goto st82;
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( (*p) == 101 )
		goto tr125;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 110: goto st84;
		case 118: goto st88;
	}
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 100 )
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 111 )
		goto st86;
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) == 110 )
		goto st87;
	goto st0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	if ( (*p) == 101 )
		goto tr131;
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	if ( (*p) == 101 )
		goto st89;
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	if ( (*p) == 114 )
		goto st90;
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	if ( (*p) == 102 )
		goto st91;
	goto st0;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	if ( (*p) == 108 )
		goto st92;
	goto st0;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	if ( (*p) == 111 )
		goto st93;
	goto st0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) == 119 )
		goto tr137;
	goto st0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	switch( (*p) ) {
		case 97: goto st95;
		case 101: goto st97;
		case 111: goto st99;
	}
	goto st0;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	if ( (*p) == 110 )
		goto st96;
	goto st0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	if ( (*p) == 100 )
		goto tr142;
	goto st0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	if ( (*p) == 99 )
		goto st98;
	goto st0;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	if ( (*p) == 116 )
		goto tr144;
	goto st0;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	if ( (*p) == 117 )
		goto st100;
	goto st0;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	if ( (*p) == 110 )
		goto st101;
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	if ( (*p) == 100 )
		goto tr147;
	goto st0;
tr191:
#line 1 "NONE"
	{te = p+1;}
	goto st138;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
#line 1725 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	switch( (*p) ) {
		case 97: goto st102;
		case 101: goto st104;
		case 105: goto st105;
		case 113: goto st106;
	}
	goto tr233;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	switch( (*p) ) {
		case 109: goto st103;
		case 119: goto tr150;
	}
	goto tr148;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	if ( (*p) == 112 )
		goto tr151;
	goto tr148;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	if ( (*p) == 99 )
		goto tr152;
	goto tr148;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	if ( (*p) == 110 )
		goto tr153;
	goto tr148;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	if ( (*p) == 114 )
		goto st139;
	goto tr148;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	if ( (*p) == 116 )
		goto tr239;
	goto tr238;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	switch( (*p) ) {
		case 97: goto st108;
		case 114: goto st109;
	}
	goto st0;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	if ( (*p) == 110 )
		goto tr157;
	goto st0;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	switch( (*p) ) {
		case 105: goto tr158;
		case 112: goto st110;
	}
	goto st0;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	if ( (*p) == 122 )
		goto tr160;
	goto st0;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	if ( (*p) == 115 )
		goto st112;
	goto st0;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	if ( (*p) == 97 )
		goto st113;
	goto st0;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	if ( (*p) == 119 )
		goto tr150;
	goto st0;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	if ( (*p) == 114 )
		goto st115;
	goto st0;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	if ( (*p) == 97 )
		goto st116;
	goto st0;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	if ( (*p) == 112 )
		goto tr165;
	goto st0;
tr242:
#line 102 "grain_expr.rl"
	{te = p;p--;}
	goto st140;
tr243:
#line 103 "grain_expr.rl"
	{te = p;p--;{
        char buf[32];
        char* pbuf = buf;

        int n = 0;
        while(ts != te && n < sizeof(buf)-1) { *(pbuf++) = *(ts++); n++; }
        *pbuf = '\0';
        pushSymbol(gensym(buf));
        {goto st117;}
    }}
	goto st140;
st140:
#line 1 "NONE"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof140;
case 140:
#line 1 "NONE"
	{ts = p;}
#line 1876 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
	switch( (*p) ) {
		case 32: goto st141;
		case 95: goto st142;
	}
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st142;
	} else if ( (*p) >= 48 )
		goto st142;
	goto st0;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	if ( (*p) == 32 )
		goto st141;
	goto tr242;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	if ( (*p) == 95 )
		goto st142;
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st142;
	} else if ( (*p) >= 48 )
		goto st142;
	goto tr243;
	}
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof120: cs = 120; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof121: cs = 121; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof122: cs = 122; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof123: cs = 123; goto _test_eof; 
	_test_eof124: cs = 124; goto _test_eof; 
	_test_eof125: cs = 125; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof126: cs = 126; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof127: cs = 127; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof128: cs = 128; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof129: cs = 129; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof130: cs = 130; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof131: cs = 131; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof132: cs = 132; goto _test_eof; 
	_test_eof133: cs = 133; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof134: cs = 134; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof135: cs = 135; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof136: cs = 136; goto _test_eof; 
	_test_eof137: cs = 137; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof138: cs = 138; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof139: cs = 139; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof140: cs = 140; goto _test_eof; 
	_test_eof141: cs = 141; goto _test_eof; 
	_test_eof142: cs = 142; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 118: goto tr195;
	case 119: goto tr196;
	case 5: goto tr15;
	case 120: goto tr196;
	case 6: goto tr15;
	case 7: goto tr15;
	case 121: goto tr199;
	case 8: goto tr19;
	case 122: goto tr202;
	case 9: goto tr21;
	case 10: goto tr21;
	case 11: goto tr21;
	case 123: goto tr205;
	case 124: goto tr207;
	case 125: goto tr209;
	case 126: goto tr211;
	case 14: goto tr28;
	case 127: goto tr211;
	case 128: goto tr214;
	case 23: goto tr46;
	case 129: goto tr214;
	case 24: goto tr46;
	case 25: goto tr46;
	case 130: goto tr217;
	case 26: goto tr50;
	case 131: goto tr219;
	case 27: goto tr52;
	case 28: goto tr52;
	case 29: goto tr52;
	case 132: goto tr221;
	case 133: goto tr223;
	case 31: goto tr58;
	case 134: goto tr223;
	case 32: goto tr58;
	case 33: goto tr58;
	case 135: goto tr226;
	case 75: goto tr115;
	case 136: goto tr229;
	case 137: goto tr231;
	case 79: goto tr121;
	case 138: goto tr233;
	case 102: goto tr148;
	case 103: goto tr148;
	case 104: goto tr148;
	case 105: goto tr148;
	case 106: goto tr148;
	case 139: goto tr238;
	case 141: goto tr242;
	case 142: goto tr243;
	}
	}

	_out: {}
	}

#line 440 "grain_expr.rl"

    if (cs < 117) {
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
