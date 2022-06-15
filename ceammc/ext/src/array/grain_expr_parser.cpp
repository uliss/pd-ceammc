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



#line 26 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
static const int grain_expr_start = 117;
static const int grain_expr_first_final = 117;
static const int grain_expr_error = 0;

static const int grain_expr_en_symbol = 140;
static const int grain_expr_en_main = 117;


#line 209 "grain_expr.rl"


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
			cs = (int)grain_expr_start;
			ts = 0;
			te = 0;
		}
		
#line 438 "grain_expr.rl"
		
		
#line 276 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
		{
			if ( p == pe )
				goto _test_eof;
			switch ( cs ) {
				case 117:
				goto st_case_117;
				case 0:
				goto st_case_0;
				case 118:
				goto st_case_118;
				case 1:
				goto st_case_1;
				case 2:
				goto st_case_2;
				case 3:
				goto st_case_3;
				case 4:
				goto st_case_4;
				case 119:
				goto st_case_119;
				case 5:
				goto st_case_5;
				case 120:
				goto st_case_120;
				case 6:
				goto st_case_6;
				case 7:
				goto st_case_7;
				case 121:
				goto st_case_121;
				case 8:
				goto st_case_8;
				case 122:
				goto st_case_122;
				case 9:
				goto st_case_9;
				case 10:
				goto st_case_10;
				case 11:
				goto st_case_11;
				case 12:
				goto st_case_12;
				case 123:
				goto st_case_123;
				case 124:
				goto st_case_124;
				case 125:
				goto st_case_125;
				case 13:
				goto st_case_13;
				case 126:
				goto st_case_126;
				case 14:
				goto st_case_14;
				case 127:
				goto st_case_127;
				case 15:
				goto st_case_15;
				case 16:
				goto st_case_16;
				case 17:
				goto st_case_17;
				case 18:
				goto st_case_18;
				case 19:
				goto st_case_19;
				case 20:
				goto st_case_20;
				case 21:
				goto st_case_21;
				case 22:
				goto st_case_22;
				case 128:
				goto st_case_128;
				case 23:
				goto st_case_23;
				case 129:
				goto st_case_129;
				case 24:
				goto st_case_24;
				case 25:
				goto st_case_25;
				case 130:
				goto st_case_130;
				case 26:
				goto st_case_26;
				case 131:
				goto st_case_131;
				case 27:
				goto st_case_27;
				case 28:
				goto st_case_28;
				case 29:
				goto st_case_29;
				case 30:
				goto st_case_30;
				case 132:
				goto st_case_132;
				case 133:
				goto st_case_133;
				case 31:
				goto st_case_31;
				case 134:
				goto st_case_134;
				case 32:
				goto st_case_32;
				case 33:
				goto st_case_33;
				case 34:
				goto st_case_34;
				case 35:
				goto st_case_35;
				case 36:
				goto st_case_36;
				case 37:
				goto st_case_37;
				case 38:
				goto st_case_38;
				case 39:
				goto st_case_39;
				case 40:
				goto st_case_40;
				case 41:
				goto st_case_41;
				case 42:
				goto st_case_42;
				case 43:
				goto st_case_43;
				case 44:
				goto st_case_44;
				case 45:
				goto st_case_45;
				case 46:
				goto st_case_46;
				case 47:
				goto st_case_47;
				case 48:
				goto st_case_48;
				case 49:
				goto st_case_49;
				case 50:
				goto st_case_50;
				case 51:
				goto st_case_51;
				case 52:
				goto st_case_52;
				case 53:
				goto st_case_53;
				case 54:
				goto st_case_54;
				case 55:
				goto st_case_55;
				case 56:
				goto st_case_56;
				case 57:
				goto st_case_57;
				case 58:
				goto st_case_58;
				case 59:
				goto st_case_59;
				case 60:
				goto st_case_60;
				case 61:
				goto st_case_61;
				case 62:
				goto st_case_62;
				case 63:
				goto st_case_63;
				case 64:
				goto st_case_64;
				case 65:
				goto st_case_65;
				case 66:
				goto st_case_66;
				case 67:
				goto st_case_67;
				case 68:
				goto st_case_68;
				case 69:
				goto st_case_69;
				case 70:
				goto st_case_70;
				case 71:
				goto st_case_71;
				case 72:
				goto st_case_72;
				case 73:
				goto st_case_73;
				case 74:
				goto st_case_74;
				case 135:
				goto st_case_135;
				case 75:
				goto st_case_75;
				case 76:
				goto st_case_76;
				case 77:
				goto st_case_77;
				case 78:
				goto st_case_78;
				case 136:
				goto st_case_136;
				case 137:
				goto st_case_137;
				case 79:
				goto st_case_79;
				case 80:
				goto st_case_80;
				case 81:
				goto st_case_81;
				case 82:
				goto st_case_82;
				case 83:
				goto st_case_83;
				case 84:
				goto st_case_84;
				case 85:
				goto st_case_85;
				case 86:
				goto st_case_86;
				case 87:
				goto st_case_87;
				case 88:
				goto st_case_88;
				case 89:
				goto st_case_89;
				case 90:
				goto st_case_90;
				case 91:
				goto st_case_91;
				case 92:
				goto st_case_92;
				case 93:
				goto st_case_93;
				case 94:
				goto st_case_94;
				case 95:
				goto st_case_95;
				case 96:
				goto st_case_96;
				case 97:
				goto st_case_97;
				case 98:
				goto st_case_98;
				case 99:
				goto st_case_99;
				case 100:
				goto st_case_100;
				case 101:
				goto st_case_101;
				case 138:
				goto st_case_138;
				case 102:
				goto st_case_102;
				case 103:
				goto st_case_103;
				case 104:
				goto st_case_104;
				case 105:
				goto st_case_105;
				case 106:
				goto st_case_106;
				case 139:
				goto st_case_139;
				case 107:
				goto st_case_107;
				case 108:
				goto st_case_108;
				case 109:
				goto st_case_109;
				case 110:
				goto st_case_110;
				case 111:
				goto st_case_111;
				case 112:
				goto st_case_112;
				case 113:
				goto st_case_113;
				case 114:
				goto st_case_114;
				case 115:
				goto st_case_115;
				case 116:
				goto st_case_116;
				case 140:
				goto st_case_140;
				case 141:
				goto st_case_141;
				case 142:
				goto st_case_142;
			}
			goto st_out;
			_ctr2:
			{
#line 145 "grain_expr.rl"
				{te = p+1;{
#line 145 "grain_expr.rl"
						pushToken(TK_CONST_ASIZE);  }
				}}
			
#line 577 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr3:
			{
#line 144 "grain_expr.rl"
				{te = p+1;{
#line 144 "grain_expr.rl"
						pushToken(TK_CONST_NSAMP);  }
				}}
			
#line 588 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr5:
			{
#line 143 "grain_expr.rl"
				{te = p+1;{
#line 143 "grain_expr.rl"
						pushToken(TK_CONST_NBLOCK); }
				}}
			
#line 599 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr8:
			{
#line 142 "grain_expr.rl"
				{te = p+1;{
#line 142 "grain_expr.rl"
						pushToken(TK_CONST_NITER);  }
				}}
			
#line 610 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr13:
			{
#line 134 "grain_expr.rl"
				{te = p+1;{
#line 134 "grain_expr.rl"
						pushToken(TK_CONST_AT);          }
				}}
			
#line 621 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr14:
			{
#line 133 "grain_expr.rl"
				{te = p+1;{
#line 133 "grain_expr.rl"
						pushToken(TK_CONST_AMP);         }
				}}
			
#line 632 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr15:
			{
#line 146 "grain_expr.rl"
				{te = p+1;{
#line 146 "grain_expr.rl"
						pushToken(TK_CONST_GIDX);   }
				}}
			
#line 643 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr16:
			{
#line 137 "grain_expr.rl"
				{p = ((te))-1;
					{
#line 137 "grain_expr.rl"
						pushToken(TK_CONST_LEN);         }
				}}
			
#line 655 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr19:
			{
#line 137 "grain_expr.rl"
				{te = p+1;{
#line 137 "grain_expr.rl"
						pushToken(TK_CONST_LEN);         }
				}}
			
#line 666 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr20:
			{
#line 135 "grain_expr.rl"
				{p = ((te))-1;
					{
#line 135 "grain_expr.rl"
						pushToken(TK_CONST_PAN);         }
				}}
			
#line 678 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr21:
			{
#line 135 "grain_expr.rl"
				{te = p+1;{
#line 135 "grain_expr.rl"
						pushToken(TK_CONST_PAN);         }
				}}
			
#line 689 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr22:
			{
#line 136 "grain_expr.rl"
				{p = ((te))-1;
					{
#line 136 "grain_expr.rl"
						pushToken(TK_CONST_SPEED);       }
				}}
			
#line 701 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr25:
			{
#line 136 "grain_expr.rl"
				{te = p+1;{
#line 136 "grain_expr.rl"
						pushToken(TK_CONST_SPEED);       }
				}}
			
#line 712 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr26:
			{
#line 139 "grain_expr.rl"
				{te = p+1;{
#line 139 "grain_expr.rl"
						pushToken(TK_CONST_TIME_AFTER);  }
				}}
			
#line 723 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr27:
			{
#line 138 "grain_expr.rl"
				{te = p+1;{
#line 138 "grain_expr.rl"
						pushToken(TK_CONST_TIME_BEFORE); }
				}}
			
#line 734 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr29:
			{
#line 202 "grain_expr.rl"
				{te = p+1;{
#line 202 "grain_expr.rl"
						pushToken(TK_DOTS); }
				}}
			
#line 745 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr30:
			{
#line 23 "grain_expr.rl"
				{p = ((te))-1;
					{
#line 23 "grain_expr.rl"
						
						dval = dsign * (double(dintp) + double(dfracnum) / double(dfracden));
						pushDouble(dval);
					}
				}}
			
#line 760 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr37:
			{
#line 181 "grain_expr.rl"
				{te = p+1;{
#line 181 "grain_expr.rl"
						pushToken(TK_PROP_REPEATS);     }
				}}
			
#line 771 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr42:
			{
#line 175 "grain_expr.rl"
				{te = p+1;{
#line 175 "grain_expr.rl"
						pushToken(TK_PROP_AT);    }
				}}
			
#line 782 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr43:
			{
#line 174 "grain_expr.rl"
				{te = p+1;{
#line 174 "grain_expr.rl"
						pushToken(TK_PROP_AMP);   }
				}}
			
#line 793 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr48:
			{
#line 182 "grain_expr.rl"
				{te = p+1;{
#line 182 "grain_expr.rl"
						pushToken(TK_PROP_INTERP);      }
				}}
			
#line 804 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr49:
			{
#line 176 "grain_expr.rl"
				{p = ((te))-1;
					{
#line 176 "grain_expr.rl"
						pushToken(TK_PROP_LEN);   }
				}}
			
#line 816 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr52:
			{
#line 176 "grain_expr.rl"
				{te = p+1;{
#line 176 "grain_expr.rl"
						pushToken(TK_PROP_LEN);   }
				}}
			
#line 827 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr53:
			{
#line 177 "grain_expr.rl"
				{p = ((te))-1;
					{
#line 177 "grain_expr.rl"
						pushToken(TK_PROP_PAN);   }
				}}
			
#line 839 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr54:
			{
#line 177 "grain_expr.rl"
				{te = p+1;{
#line 177 "grain_expr.rl"
						pushToken(TK_PROP_PAN);   }
				}}
			
#line 850 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr55:
			{
#line 178 "grain_expr.rl"
				{p = ((te))-1;
					{
#line 178 "grain_expr.rl"
						pushToken(TK_PROP_SPEED); }
				}}
			
#line 862 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr58:
			{
#line 178 "grain_expr.rl"
				{te = p+1;{
#line 178 "grain_expr.rl"
						pushToken(TK_PROP_SPEED); }
				}}
			
#line 873 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr60:
			{
#line 179 "grain_expr.rl"
				{te = p+1;{
#line 179 "grain_expr.rl"
						pushToken(TK_PROP_TIME_BEFORE); }
				}}
			
#line 884 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr61:
			{
#line 183 "grain_expr.rl"
				{p = ((te))-1;
					{
#line 183 "grain_expr.rl"
						pushToken(TK_PROP_WINDOW); }
				}}
			
#line 896 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr64:
			{
#line 183 "grain_expr.rl"
				{te = p+1;{
#line 183 "grain_expr.rl"
						pushToken(TK_PROP_WINDOW); }
				}}
			
#line 907 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr70:
			{
#line 29 "grain_expr.rl"
				{te = p+1;{
#line 29 "grain_expr.rl"
						pushToken(TK_FN_ABS); }
				}}
			
#line 918 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr72:
			{
#line 34 "grain_expr.rl"
				{te = p+1;{
#line 34 "grain_expr.rl"
						pushToken(TK_FN_ACOS); }
				}}
			
#line 929 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr74:
			{
#line 35 "grain_expr.rl"
				{te = p+1;{
#line 35 "grain_expr.rl"
						pushToken(TK_FN_ASIN); }
				}}
			
#line 940 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr76:
			{
#line 36 "grain_expr.rl"
				{te = p+1;{
#line 36 "grain_expr.rl"
						pushToken(TK_FN_ATAN); }
				}}
			
#line 951 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr84:
			{
#line 38 "grain_expr.rl"
				{te = p+1;{
#line 38 "grain_expr.rl"
						pushToken(TK_FN_CEIL); }
				}}
			
#line 962 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr88:
			{
#line 168 "grain_expr.rl"
				{te = p+1;{
#line 168 "grain_expr.rl"
						pushToken(TK_CHOICE); }
				}}
			
#line 973 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr90:
			{
#line 164 "grain_expr.rl"
				{te = p+1;{
#line 164 "grain_expr.rl"
						pushToken(TK_CLIP); }
				}}
			
#line 984 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr91:
			{
#line 32 "grain_expr.rl"
				{te = p+1;{
#line 32 "grain_expr.rl"
						pushToken(TK_FN_COS); }
				}}
			
#line 995 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr94:
			{
#line 197 "grain_expr.rl"
				{te = p+1;{
#line 197 "grain_expr.rl"
						pushToken(TK_CUBIC); }
				}}
			
#line 1006 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr98:
			{
#line 200 "grain_expr.rl"
				{te = p+1;{
#line 200 "grain_expr.rl"
						pushToken(TK_DSAW); }
				}}
			
#line 1017 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr105:
			{
#line 39 "grain_expr.rl"
				{te = p+1;{
#line 39 "grain_expr.rl"
						pushToken(TK_FN_FLOOR); }
				}}
			
#line 1028 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr107:
			{
#line 166 "grain_expr.rl"
				{te = p+1;{
#line 166 "grain_expr.rl"
						pushToken(TK_FOLD); }
				}}
			
#line 1039 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr109:
			{
#line 46 "grain_expr.rl"
				{te = p+1;{
#line 46 "grain_expr.rl"
						pushToken(TK_FN_FRAC); }
				}}
			
#line 1050 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr113:
			{
#line 186 "grain_expr.rl"
				{te = p+1;{
#line 186 "grain_expr.rl"
						pushToken(TK_HANN); }
				}}
			
#line 1061 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr121:
			{
#line 191 "grain_expr.rl"
				{te = p+1;{
#line 191 "grain_expr.rl"
						pushToken(TK_LINDOWN); }
				}}
			
#line 1072 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr122:
			{
#line 196 "grain_expr.rl"
				{te = p+1;{
#line 196 "grain_expr.rl"
						pushToken(TK_LINEAR); }
				}}
			
#line 1083 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr124:
			{
#line 42 "grain_expr.rl"
				{p = ((te))-1;
					{
#line 42 "grain_expr.rl"
						pushToken(TK_FN_LOG); }
				}}
			
#line 1095 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr125:
			{
#line 43 "grain_expr.rl"
				{te = p+1;{
#line 43 "grain_expr.rl"
						pushToken(TK_FN_LOG10); }
				}}
			
#line 1106 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr126:
			{
#line 190 "grain_expr.rl"
				{te = p+1;{
#line 190 "grain_expr.rl"
						pushToken(TK_LINUP); }
				}}
			
#line 1117 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr131:
			{
#line 170 "grain_expr.rl"
				{p = ((te))-1;
					{
#line 170 "grain_expr.rl"
						pushToken(TK_MSEC); }
				}}
			
#line 1129 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr132:
			{
#line 170 "grain_expr.rl"
				{te = p+1;{
#line 170 "grain_expr.rl"
						pushToken(TK_MSEC); }
				}}
			
#line 1140 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr136:
			{
#line 195 "grain_expr.rl"
				{te = p+1;{
#line 195 "grain_expr.rl"
						pushToken(TK_NONE); }
				}}
			
#line 1151 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr143:
			{
#line 192 "grain_expr.rl"
				{te = p+1;{
#line 192 "grain_expr.rl"
						pushToken(TK_ONDONE); }
				}}
			
#line 1162 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr149:
			{
#line 193 "grain_expr.rl"
				{te = p+1;{
#line 193 "grain_expr.rl"
						pushToken(TK_OVERFLOW); }
				}}
			
#line 1173 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr155:
			{
#line 167 "grain_expr.rl"
				{te = p+1;{
#line 167 "grain_expr.rl"
						pushToken(TK_FN_URAND); }
				}}
			
#line 1184 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr157:
			{
#line 187 "grain_expr.rl"
				{te = p+1;{
#line 187 "grain_expr.rl"
						pushToken(TK_RECT); }
				}}
			
#line 1195 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr160:
			{
#line 40 "grain_expr.rl"
				{te = p+1;{
#line 40 "grain_expr.rl"
						pushToken(TK_FN_ROUND); }
				}}
			
#line 1206 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr161:
			{
#line 171 "grain_expr.rl"
				{p = ((te))-1;
					{
#line 171 "grain_expr.rl"
						pushToken(TK_SEC);  }
				}}
			
#line 1218 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr163:
			{
#line 199 "grain_expr.rl"
				{te = p+1;{
#line 199 "grain_expr.rl"
						pushToken(TK_USAW); }
				}}
			
#line 1229 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr164:
			{
#line 172 "grain_expr.rl"
				{te = p+1;{
#line 172 "grain_expr.rl"
						pushToken(TK_SAMP); }
				}}
			
#line 1240 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr165:
			{
#line 171 "grain_expr.rl"
				{te = p+1;{
#line 171 "grain_expr.rl"
						pushToken(TK_SEC);  }
				}}
			
#line 1251 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr166:
			{
#line 152 "grain_expr.rl"
				{te = p+1;{
#line 152 "grain_expr.rl"
						pushToken(TK_SIN); }
				}}
			
#line 1262 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr171:
			{
#line 33 "grain_expr.rl"
				{te = p+1;{
#line 33 "grain_expr.rl"
						pushToken(TK_FN_TAN); }
				}}
			
#line 1273 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr172:
			{
#line 188 "grain_expr.rl"
				{te = p+1;{
#line 188 "grain_expr.rl"
						pushToken(TK_TRI); }
				}}
			
#line 1284 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr174:
			{
#line 189 "grain_expr.rl"
				{te = p+1;{
#line 189 "grain_expr.rl"
						pushToken(TK_TRPZ); }
				}}
			
#line 1295 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr181:
			{
#line 165 "grain_expr.rl"
				{te = p+1;{
#line 165 "grain_expr.rl"
						pushToken(TK_WRAP); }
				}}
			
#line 1306 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr184:
			{
#line 123 "grain_expr.rl"
				{te = p+1;{
#line 123 "grain_expr.rl"
						pushToken(TK_MOD); }
				}}
			
#line 1317 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr185:
			{
#line 124 "grain_expr.rl"
				{te = p+1;{
#line 124 "grain_expr.rl"
						pushToken(TK_OPENP); }
				}}
			
#line 1328 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr186:
			{
#line 125 "grain_expr.rl"
				{te = p+1;{
#line 125 "grain_expr.rl"
						pushToken(TK_CLOSEP); }
				}}
			
#line 1339 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr189:
			{
#line 131 "grain_expr.rl"
				{te = p+1;{
#line 131 "grain_expr.rl"
						pushToken(TK_COMMA);}
				}}
			
#line 1350 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr191:
			{
#line 121 "grain_expr.rl"
				{te = p+1;{
#line 121 "grain_expr.rl"
						pushToken(TK_DIVIDE); }
				}}
			
#line 1361 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr193:
			{
#line 130 "grain_expr.rl"
				{te = p+1;{
#line 130 "grain_expr.rl"
						pushToken(TK_ASSIGN_SET);}
				}}
			
#line 1372 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr194:
			{
#line 122 "grain_expr.rl"
				{te = p+1;{
#line 122 "grain_expr.rl"
						pushToken(TK_POW); }
				}}
			
#line 1383 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr196:
			{
#line 204 "grain_expr.rl"
				{te = p;p = p - 1;}}
			
#line 1391 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr197:
			{
#line 137 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 137 "grain_expr.rl"
						pushToken(TK_CONST_LEN);         }
				}}
			
#line 1402 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr200:
			{
#line 135 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 135 "grain_expr.rl"
						pushToken(TK_CONST_PAN);         }
				}}
			
#line 1413 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr202:
			{
#line 147 "grain_expr.rl"
				{te = p+1;{
#line 147 "grain_expr.rl"
						pushDouble(std::acos(-1));  }
				}}
			
#line 1424 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr203:
			{
#line 136 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 136 "grain_expr.rl"
						pushToken(TK_CONST_SPEED);       }
				}}
			
#line 1435 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr205:
			{
#line 141 "grain_expr.rl"
				{te = p+1;{
#line 141 "grain_expr.rl"
						pushToken(TK_CONST_SR);     }
				}}
			
#line 1446 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr206:
			{
#line 120 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 120 "grain_expr.rl"
						pushToken(TK_TIMES); }
				}}
			
#line 1457 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr207:
			{
#line 129 "grain_expr.rl"
				{te = p+1;{
#line 129 "grain_expr.rl"
						pushToken(TK_ASSIGN_MUL);}
				}}
			
#line 1468 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr208:
			{
#line 118 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 118 "grain_expr.rl"
						pushToken(TK_PLUS); }
				}}
			
#line 1479 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr209:
			{
#line 127 "grain_expr.rl"
				{te = p+1;{
#line 127 "grain_expr.rl"
						pushToken(TK_ASSIGN_ADD);}
				}}
			
#line 1490 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr210:
			{
#line 119 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 119 "grain_expr.rl"
						pushToken(TK_MINUS); }
				}}
			
#line 1501 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr211:
			{
#line 128 "grain_expr.rl"
				{te = p+1;{
#line 128 "grain_expr.rl"
						pushToken(TK_ASSIGN_SUB);}
				}}
			
#line 1512 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr212:
			{
#line 23 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 23 "grain_expr.rl"
						
						dval = dsign * (double(dintp) + double(dfracnum) / double(dfracden));
						pushDouble(dval);
					}
				}}
			
#line 1526 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr215:
			{
#line 176 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 176 "grain_expr.rl"
						pushToken(TK_PROP_LEN);   }
				}}
			
#line 1537 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr218:
			{
#line 177 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 177 "grain_expr.rl"
						pushToken(TK_PROP_PAN);   }
				}}
			
#line 1548 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr220:
			{
#line 178 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 178 "grain_expr.rl"
						pushToken(TK_PROP_SPEED); }
				}}
			
#line 1559 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr222:
			{
#line 180 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 180 "grain_expr.rl"
						pushToken(TK_PROP_TIME_AFTER);  }
				}}
			
#line 1570 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr223:
			{
#line 184 "grain_expr.rl"
				{te = p+1;{
#line 184 "grain_expr.rl"
						pushToken(TK_PROP_TAG); curtok_ = TK_PROP_TAG; {goto _st140;}}
				}}
			
#line 1581 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr224:
			{
#line 183 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 183 "grain_expr.rl"
						pushToken(TK_PROP_WINDOW); }
				}}
			
#line 1592 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr227:
			{
#line 42 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 42 "grain_expr.rl"
						pushToken(TK_FN_LOG); }
				}}
			
#line 1603 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr229:
			{
#line 44 "grain_expr.rl"
				{te = p+1;{
#line 44 "grain_expr.rl"
						pushToken(TK_FN_LOG2); }
				}}
			
#line 1614 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr230:
			{
#line 198 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 198 "grain_expr.rl"
						pushToken(TK_MODULATE); }
				}}
			
#line 1625 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr231:
			{
#line 194 "grain_expr.rl"
				{te = p+1;{
#line 194 "grain_expr.rl"
						pushToken(TK_MODE); }
				}}
			
#line 1636 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr232:
			{
#line 170 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 170 "grain_expr.rl"
						pushToken(TK_MSEC); }
				}}
			
#line 1647 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr234:
			{
#line 171 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 171 "grain_expr.rl"
						pushToken(TK_SEC);  }
				}}
			
#line 1658 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr239:
			{
#line 201 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 201 "grain_expr.rl"
						pushToken(TK_SQR); }
				}}
			
#line 1669 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_ctr240:
			{
#line 30 "grain_expr.rl"
				{te = p+1;{
#line 30 "grain_expr.rl"
						pushToken(TK_FN_SQRT); }
				}}
			
#line 1680 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st117;
			_st117:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			{
#line 1 "NONE"
				{ts = 0;}}
			
#line 1694 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			p+= 1;
			if ( p == pe )
				goto _test_eof117;
			st_case_117:
			{
#line 1 "NONE"
				{ts = p;}}
			
#line 1704 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			switch( ( (*( p))) ) {
				case 32: {
					goto _st118;
				}
				case 36: {
					goto _st1;
				}
				case 37: {
					goto _ctr184;
				}
				case 40: {
					goto _ctr185;
				}
				case 41: {
					goto _ctr186;
				}
				case 42: {
					goto _st123;
				}
				case 43: {
					goto _st124;
				}
				case 44: {
					goto _ctr189;
				}
				case 45: {
					goto _st125;
				}
				case 46: {
					goto _st13;
				}
				case 47: {
					goto _ctr191;
				}
				case 61: {
					goto _ctr193;
				}
				case 64: {
					goto _st15;
				}
				case 94: {
					goto _ctr194;
				}
				case 97: {
					goto _st34;
				}
				case 99: {
					goto _st42;
				}
				case 100: {
					goto _st55;
				}
				case 102: {
					goto _st58;
				}
				case 104: {
					goto _st66;
				}
				case 108: {
					goto _st69;
				}
				case 109: {
					goto _st77;
				}
				case 110: {
					goto _st80;
				}
				case 111: {
					goto _st83;
				}
				case 114: {
					goto _st94;
				}
				case 115: {
					goto _ctr195;
				}
				case 116: {
					goto _st107;
				}
				case 117: {
					goto _st111;
				}
				case 119: {
					goto _st114;
				}
			}
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _ctr192;
			}
			{
				goto _st0;
			}
			st_case_0:
			_st0:
			cs = 0;
			goto _pop;
			_st118:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof118;
			st_case_118:
			if ( ( (*( p))) == 32 ) {
				goto _st118;
			}
			{
				goto _ctr196;
			}
			_st1:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof1;
			st_case_1:
			switch( ( (*( p))) ) {
				case 65: {
					goto _ctr2;
				}
				case 95: {
					goto _ctr3;
				}
				case 97: {
					goto _st2;
				}
				case 98: {
					goto _ctr5;
				}
				case 103: {
					goto _st4;
				}
				case 108: {
					goto _ctr7;
				}
				case 110: {
					goto _ctr8;
				}
				case 112: {
					goto _ctr9;
				}
				case 115: {
					goto _ctr10;
				}
				case 116: {
					goto _st12;
				}
			}
			{
				goto _st0;
			}
			_st2:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof2;
			st_case_2:
			switch( ( (*( p))) ) {
				case 109: {
					goto _st3;
				}
				case 116: {
					goto _ctr13;
				}
			}
			{
				goto _st0;
			}
			_st3:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof3;
			st_case_3:
			if ( ( (*( p))) == 112 ) {
				goto _ctr14;
			}
			{
				goto _st0;
			}
			_st4:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof4;
			st_case_4:
			if ( ( (*( p))) == 105 ) {
				goto _ctr15;
			}
			{
				goto _st0;
			}
			_ctr7:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 1926 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st119;
			_st119:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof119;
			st_case_119:
			if ( ( (*( p))) == 101 ) {
				goto _st5;
			}
			{
				goto _ctr197;
			}
			_st5:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof5;
			st_case_5:
			if ( ( (*( p))) == 110 ) {
				goto _ctr17;
			}
			{
				goto _ctr16;
			}
			_ctr17:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 1968 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st120;
			_st120:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof120;
			st_case_120:
			if ( ( (*( p))) == 103 ) {
				goto _st6;
			}
			{
				goto _ctr197;
			}
			_st6:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof6;
			st_case_6:
			if ( ( (*( p))) == 116 ) {
				goto _st7;
			}
			{
				goto _ctr16;
			}
			_st7:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof7;
			st_case_7:
			if ( ( (*( p))) == 104 ) {
				goto _ctr19;
			}
			{
				goto _ctr16;
			}
			_ctr9:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2027 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st121;
			_st121:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof121;
			st_case_121:
			switch( ( (*( p))) ) {
				case 97: {
					goto _st8;
				}
				case 105: {
					goto _ctr202;
				}
			}
			{
				goto _ctr200;
			}
			_st8:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof8;
			st_case_8:
			if ( ( (*( p))) == 110 ) {
				goto _ctr21;
			}
			{
				goto _ctr20;
			}
			_ctr10:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2074 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st122;
			_st122:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof122;
			st_case_122:
			switch( ( (*( p))) ) {
				case 112: {
					goto _st9;
				}
				case 114: {
					goto _ctr205;
				}
			}
			{
				goto _ctr203;
			}
			_st9:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof9;
			st_case_9:
			if ( ( (*( p))) == 101 ) {
				goto _st10;
			}
			{
				goto _ctr22;
			}
			_st10:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof10;
			st_case_10:
			if ( ( (*( p))) == 101 ) {
				goto _st11;
			}
			{
				goto _ctr22;
			}
			_st11:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof11;
			st_case_11:
			if ( ( (*( p))) == 100 ) {
				goto _ctr25;
			}
			{
				goto _ctr22;
			}
			_st12:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof12;
			st_case_12:
			switch( ( (*( p))) ) {
				case 97: {
					goto _ctr26;
				}
				case 98: {
					goto _ctr27;
				}
			}
			{
				goto _st0;
			}
			_st123:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof123;
			st_case_123:
			if ( ( (*( p))) == 61 ) {
				goto _ctr207;
			}
			{
				goto _ctr206;
			}
			_st124:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof124;
			st_case_124:
			if ( ( (*( p))) == 61 ) {
				goto _ctr209;
			}
			{
				goto _ctr208;
			}
			_st125:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof125;
			st_case_125:
			if ( ( (*( p))) == 61 ) {
				goto _ctr211;
			}
			{
				goto _ctr210;
			}
			_st13:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof13;
			st_case_13:
			if ( ( (*( p))) == 46 ) {
				goto _ctr29;
			}
			{
				goto _st0;
			}
			_ctr192:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2245 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			{
#line 56 "grain_expr.rl"
				dsign = 1; dintp = 0; dfracnum = 0; dfracden = 1; }
			
#line 2251 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			{
#line 51 "grain_expr.rl"
				(dintp *= 10) += ((( (*( p))))-'0'); }
			
#line 2257 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st126;
			_ctr214:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2265 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			{
#line 51 "grain_expr.rl"
				(dintp *= 10) += ((( (*( p))))-'0'); }
			
#line 2271 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st126;
			_st126:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof126;
			st_case_126:
			if ( ( (*( p))) == 46 ) {
				goto _st14;
			}
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _ctr214;
			}
			{
				goto _ctr212;
			}
			_st14:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof14;
			st_case_14:
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _ctr31;
			}
			{
				goto _ctr30;
			}
			_ctr31:
			{
#line 53 "grain_expr.rl"
				(dfracnum *= 10) += ((( (*( p))))-'0'); dfracden*=10; }
			
#line 2316 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st127;
			_st127:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof127;
			st_case_127:
			if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
				goto _ctr31;
			}
			{
				goto _ctr212;
			}
			_st15:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof15;
			st_case_15:
			switch( ( (*( p))) ) {
				case 97: {
					goto _st16;
				}
				case 105: {
					goto _st18;
				}
				case 108: {
					goto _ctr35;
				}
				case 112: {
					goto _ctr36;
				}
				case 114: {
					goto _ctr37;
				}
				case 115: {
					goto _ctr38;
				}
				case 116: {
					goto _st30;
				}
				case 119: {
					goto _ctr40;
				}
			}
			{
				goto _st0;
			}
			_st16:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof16;
			st_case_16:
			switch( ( (*( p))) ) {
				case 109: {
					goto _st17;
				}
				case 116: {
					goto _ctr42;
				}
			}
			{
				goto _st0;
			}
			_st17:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof17;
			st_case_17:
			if ( ( (*( p))) == 112 ) {
				goto _ctr43;
			}
			{
				goto _st0;
			}
			_st18:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof18;
			st_case_18:
			if ( ( (*( p))) == 110 ) {
				goto _st19;
			}
			{
				goto _st0;
			}
			_st19:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof19;
			st_case_19:
			if ( ( (*( p))) == 116 ) {
				goto _st20;
			}
			{
				goto _st0;
			}
			_st20:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof20;
			st_case_20:
			if ( ( (*( p))) == 101 ) {
				goto _st21;
			}
			{
				goto _st0;
			}
			_st21:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof21;
			st_case_21:
			if ( ( (*( p))) == 114 ) {
				goto _st22;
			}
			{
				goto _st0;
			}
			_st22:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof22;
			st_case_22:
			if ( ( (*( p))) == 112 ) {
				goto _ctr48;
			}
			{
				goto _st0;
			}
			_ctr35:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2505 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st128;
			_st128:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof128;
			st_case_128:
			if ( ( (*( p))) == 101 ) {
				goto _st23;
			}
			{
				goto _ctr215;
			}
			_st23:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof23;
			st_case_23:
			if ( ( (*( p))) == 110 ) {
				goto _ctr50;
			}
			{
				goto _ctr49;
			}
			_ctr50:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2547 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st129;
			_st129:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof129;
			st_case_129:
			if ( ( (*( p))) == 103 ) {
				goto _st24;
			}
			{
				goto _ctr215;
			}
			_st24:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof24;
			st_case_24:
			if ( ( (*( p))) == 116 ) {
				goto _st25;
			}
			{
				goto _ctr49;
			}
			_st25:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof25;
			st_case_25:
			if ( ( (*( p))) == 104 ) {
				goto _ctr52;
			}
			{
				goto _ctr49;
			}
			_ctr36:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2606 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st130;
			_st130:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof130;
			st_case_130:
			if ( ( (*( p))) == 97 ) {
				goto _st26;
			}
			{
				goto _ctr218;
			}
			_st26:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof26;
			st_case_26:
			if ( ( (*( p))) == 110 ) {
				goto _ctr54;
			}
			{
				goto _ctr53;
			}
			_ctr38:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2648 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st131;
			_st131:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof131;
			st_case_131:
			if ( ( (*( p))) == 112 ) {
				goto _st27;
			}
			{
				goto _ctr220;
			}
			_st27:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof27;
			st_case_27:
			if ( ( (*( p))) == 101 ) {
				goto _st28;
			}
			{
				goto _ctr55;
			}
			_st28:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof28;
			st_case_28:
			if ( ( (*( p))) == 101 ) {
				goto _st29;
			}
			{
				goto _ctr55;
			}
			_st29:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof29;
			st_case_29:
			if ( ( (*( p))) == 100 ) {
				goto _ctr58;
			}
			{
				goto _ctr55;
			}
			_st30:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof30;
			st_case_30:
			switch( ( (*( p))) ) {
				case 97: {
					goto _st132;
				}
				case 98: {
					goto _ctr60;
				}
			}
			{
				goto _st0;
			}
			_st132:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof132;
			st_case_132:
			if ( ( (*( p))) == 103 ) {
				goto _ctr223;
			}
			{
				goto _ctr222;
			}
			_ctr40:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2763 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st133;
			_st133:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof133;
			st_case_133:
			if ( ( (*( p))) == 105 ) {
				goto _st31;
			}
			{
				goto _ctr224;
			}
			_st31:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof31;
			st_case_31:
			if ( ( (*( p))) == 110 ) {
				goto _ctr62;
			}
			{
				goto _ctr61;
			}
			_ctr62:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 2805 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st134;
			_st134:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof134;
			st_case_134:
			if ( ( (*( p))) == 100 ) {
				goto _st32;
			}
			{
				goto _ctr224;
			}
			_st32:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof32;
			st_case_32:
			if ( ( (*( p))) == 111 ) {
				goto _st33;
			}
			{
				goto _ctr61;
			}
			_st33:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof33;
			st_case_33:
			if ( ( (*( p))) == 119 ) {
				goto _ctr64;
			}
			{
				goto _ctr61;
			}
			_st34:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof34;
			st_case_34:
			switch( ( (*( p))) ) {
				case 98: {
					goto _st35;
				}
				case 99: {
					goto _st36;
				}
				case 115: {
					goto _st38;
				}
				case 116: {
					goto _st40;
				}
			}
			{
				goto _st0;
			}
			_st35:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof35;
			st_case_35:
			if ( ( (*( p))) == 115 ) {
				goto _ctr70;
			}
			{
				goto _st0;
			}
			_st36:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof36;
			st_case_36:
			if ( ( (*( p))) == 111 ) {
				goto _st37;
			}
			{
				goto _st0;
			}
			_st37:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof37;
			st_case_37:
			if ( ( (*( p))) == 115 ) {
				goto _ctr72;
			}
			{
				goto _st0;
			}
			_st38:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof38;
			st_case_38:
			if ( ( (*( p))) == 105 ) {
				goto _st39;
			}
			{
				goto _st0;
			}
			_st39:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof39;
			st_case_39:
			if ( ( (*( p))) == 110 ) {
				goto _ctr74;
			}
			{
				goto _st0;
			}
			_st40:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof40;
			st_case_40:
			if ( ( (*( p))) == 97 ) {
				goto _st41;
			}
			{
				goto _st0;
			}
			_st41:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof41;
			st_case_41:
			if ( ( (*( p))) == 110 ) {
				goto _ctr76;
			}
			{
				goto _st0;
			}
			_st42:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof42;
			st_case_42:
			switch( ( (*( p))) ) {
				case 101: {
					goto _st43;
				}
				case 104: {
					goto _st45;
				}
				case 108: {
					goto _st49;
				}
				case 111: {
					goto _st51;
				}
				case 117: {
					goto _st52;
				}
			}
			{
				goto _st0;
			}
			_st43:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof43;
			st_case_43:
			if ( ( (*( p))) == 105 ) {
				goto _st44;
			}
			{
				goto _st0;
			}
			_st44:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof44;
			st_case_44:
			if ( ( (*( p))) == 108 ) {
				goto _ctr84;
			}
			{
				goto _st0;
			}
			_st45:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof45;
			st_case_45:
			if ( ( (*( p))) == 111 ) {
				goto _st46;
			}
			{
				goto _st0;
			}
			_st46:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof46;
			st_case_46:
			if ( ( (*( p))) == 105 ) {
				goto _st47;
			}
			{
				goto _st0;
			}
			_st47:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof47;
			st_case_47:
			if ( ( (*( p))) == 99 ) {
				goto _st48;
			}
			{
				goto _st0;
			}
			_st48:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof48;
			st_case_48:
			if ( ( (*( p))) == 101 ) {
				goto _ctr88;
			}
			{
				goto _st0;
			}
			_st49:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof49;
			st_case_49:
			if ( ( (*( p))) == 105 ) {
				goto _st50;
			}
			{
				goto _st0;
			}
			_st50:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof50;
			st_case_50:
			if ( ( (*( p))) == 112 ) {
				goto _ctr90;
			}
			{
				goto _st0;
			}
			_st51:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof51;
			st_case_51:
			if ( ( (*( p))) == 115 ) {
				goto _ctr91;
			}
			{
				goto _st0;
			}
			_st52:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof52;
			st_case_52:
			if ( ( (*( p))) == 98 ) {
				goto _st53;
			}
			{
				goto _st0;
			}
			_st53:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof53;
			st_case_53:
			if ( ( (*( p))) == 105 ) {
				goto _st54;
			}
			{
				goto _st0;
			}
			_st54:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof54;
			st_case_54:
			if ( ( (*( p))) == 99 ) {
				goto _ctr94;
			}
			{
				goto _st0;
			}
			_st55:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof55;
			st_case_55:
			if ( ( (*( p))) == 115 ) {
				goto _st56;
			}
			{
				goto _st0;
			}
			_st56:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof56;
			st_case_56:
			if ( ( (*( p))) == 97 ) {
				goto _st57;
			}
			{
				goto _st0;
			}
			_st57:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof57;
			st_case_57:
			if ( ( (*( p))) == 119 ) {
				goto _ctr98;
			}
			{
				goto _st0;
			}
			_st58:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof58;
			st_case_58:
			switch( ( (*( p))) ) {
				case 108: {
					goto _st59;
				}
				case 111: {
					goto _st62;
				}
				case 114: {
					goto _st64;
				}
			}
			{
				goto _st0;
			}
			_st59:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof59;
			st_case_59:
			if ( ( (*( p))) == 111 ) {
				goto _st60;
			}
			{
				goto _st0;
			}
			_st60:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof60;
			st_case_60:
			if ( ( (*( p))) == 111 ) {
				goto _st61;
			}
			{
				goto _st0;
			}
			_st61:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof61;
			st_case_61:
			if ( ( (*( p))) == 114 ) {
				goto _ctr105;
			}
			{
				goto _st0;
			}
			_st62:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof62;
			st_case_62:
			if ( ( (*( p))) == 108 ) {
				goto _st63;
			}
			{
				goto _st0;
			}
			_st63:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof63;
			st_case_63:
			if ( ( (*( p))) == 100 ) {
				goto _ctr107;
			}
			{
				goto _st0;
			}
			_st64:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof64;
			st_case_64:
			if ( ( (*( p))) == 97 ) {
				goto _st65;
			}
			{
				goto _st0;
			}
			_st65:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof65;
			st_case_65:
			if ( ( (*( p))) == 99 ) {
				goto _ctr109;
			}
			{
				goto _st0;
			}
			_st66:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof66;
			st_case_66:
			if ( ( (*( p))) == 97 ) {
				goto _st67;
			}
			{
				goto _st0;
			}
			_st67:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof67;
			st_case_67:
			if ( ( (*( p))) == 110 ) {
				goto _st68;
			}
			{
				goto _st0;
			}
			_st68:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof68;
			st_case_68:
			if ( ( (*( p))) == 110 ) {
				goto _ctr113;
			}
			{
				goto _st0;
			}
			_st69:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof69;
			st_case_69:
			switch( ( (*( p))) ) {
				case 100: {
					goto _st70;
				}
				case 105: {
					goto _st73;
				}
				case 111: {
					goto _st74;
				}
				case 117: {
					goto _st76;
				}
			}
			{
				goto _st0;
			}
			_st70:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof70;
			st_case_70:
			if ( ( (*( p))) == 111 ) {
				goto _st71;
			}
			{
				goto _st0;
			}
			_st71:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof71;
			st_case_71:
			if ( ( (*( p))) == 119 ) {
				goto _st72;
			}
			{
				goto _st0;
			}
			_st72:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof72;
			st_case_72:
			if ( ( (*( p))) == 110 ) {
				goto _ctr121;
			}
			{
				goto _st0;
			}
			_st73:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof73;
			st_case_73:
			if ( ( (*( p))) == 110 ) {
				goto _ctr122;
			}
			{
				goto _st0;
			}
			_st74:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof74;
			st_case_74:
			if ( ( (*( p))) == 103 ) {
				goto _ctr123;
			}
			{
				goto _st0;
			}
			_ctr123:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 3605 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st135;
			_st135:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof135;
			st_case_135:
			switch( ( (*( p))) ) {
				case 49: {
					goto _st75;
				}
				case 50: {
					goto _ctr229;
				}
			}
			{
				goto _ctr227;
			}
			_st75:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof75;
			st_case_75:
			if ( ( (*( p))) == 48 ) {
				goto _ctr125;
			}
			{
				goto _ctr124;
			}
			_st76:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof76;
			st_case_76:
			if ( ( (*( p))) == 112 ) {
				goto _ctr126;
			}
			{
				goto _st0;
			}
			_st77:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof77;
			st_case_77:
			switch( ( (*( p))) ) {
				case 111: {
					goto _st78;
				}
				case 115: {
					goto _ctr129;
				}
			}
			{
				goto _st0;
			}
			_st78:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof78;
			st_case_78:
			if ( ( (*( p))) == 100 ) {
				goto _st136;
			}
			{
				goto _st0;
			}
			_st136:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof136;
			st_case_136:
			if ( ( (*( p))) == 101 ) {
				goto _ctr231;
			}
			{
				goto _ctr230;
			}
			_ctr129:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 3725 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st137;
			_st137:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof137;
			st_case_137:
			if ( ( (*( p))) == 101 ) {
				goto _st79;
			}
			{
				goto _ctr232;
			}
			_st79:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof79;
			st_case_79:
			if ( ( (*( p))) == 99 ) {
				goto _ctr132;
			}
			{
				goto _ctr131;
			}
			_st80:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof80;
			st_case_80:
			if ( ( (*( p))) == 111 ) {
				goto _st81;
			}
			{
				goto _st0;
			}
			_st81:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof81;
			st_case_81:
			if ( ( (*( p))) == 110 ) {
				goto _st82;
			}
			{
				goto _st0;
			}
			_st82:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof82;
			st_case_82:
			if ( ( (*( p))) == 101 ) {
				goto _ctr136;
			}
			{
				goto _st0;
			}
			_st83:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof83;
			st_case_83:
			switch( ( (*( p))) ) {
				case 110: {
					goto _st84;
				}
				case 118: {
					goto _st88;
				}
			}
			{
				goto _st0;
			}
			_st84:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof84;
			st_case_84:
			if ( ( (*( p))) == 100 ) {
				goto _st85;
			}
			{
				goto _st0;
			}
			_st85:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof85;
			st_case_85:
			if ( ( (*( p))) == 111 ) {
				goto _st86;
			}
			{
				goto _st0;
			}
			_st86:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof86;
			st_case_86:
			if ( ( (*( p))) == 110 ) {
				goto _st87;
			}
			{
				goto _st0;
			}
			_st87:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof87;
			st_case_87:
			if ( ( (*( p))) == 101 ) {
				goto _ctr143;
			}
			{
				goto _st0;
			}
			_st88:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof88;
			st_case_88:
			if ( ( (*( p))) == 101 ) {
				goto _st89;
			}
			{
				goto _st0;
			}
			_st89:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof89;
			st_case_89:
			if ( ( (*( p))) == 114 ) {
				goto _st90;
			}
			{
				goto _st0;
			}
			_st90:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof90;
			st_case_90:
			if ( ( (*( p))) == 102 ) {
				goto _st91;
			}
			{
				goto _st0;
			}
			_st91:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof91;
			st_case_91:
			if ( ( (*( p))) == 108 ) {
				goto _st92;
			}
			{
				goto _st0;
			}
			_st92:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof92;
			st_case_92:
			if ( ( (*( p))) == 111 ) {
				goto _st93;
			}
			{
				goto _st0;
			}
			_st93:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof93;
			st_case_93:
			if ( ( (*( p))) == 119 ) {
				goto _ctr149;
			}
			{
				goto _st0;
			}
			_st94:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof94;
			st_case_94:
			switch( ( (*( p))) ) {
				case 97: {
					goto _st95;
				}
				case 101: {
					goto _st97;
				}
				case 111: {
					goto _st99;
				}
			}
			{
				goto _st0;
			}
			_st95:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof95;
			st_case_95:
			if ( ( (*( p))) == 110 ) {
				goto _st96;
			}
			{
				goto _st0;
			}
			_st96:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof96;
			st_case_96:
			if ( ( (*( p))) == 100 ) {
				goto _ctr155;
			}
			{
				goto _st0;
			}
			_st97:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof97;
			st_case_97:
			if ( ( (*( p))) == 99 ) {
				goto _st98;
			}
			{
				goto _st0;
			}
			_st98:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof98;
			st_case_98:
			if ( ( (*( p))) == 116 ) {
				goto _ctr157;
			}
			{
				goto _st0;
			}
			_st99:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof99;
			st_case_99:
			if ( ( (*( p))) == 117 ) {
				goto _st100;
			}
			{
				goto _st0;
			}
			_st100:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof100;
			st_case_100:
			if ( ( (*( p))) == 110 ) {
				goto _st101;
			}
			{
				goto _st0;
			}
			_st101:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof101;
			st_case_101:
			if ( ( (*( p))) == 100 ) {
				goto _ctr160;
			}
			{
				goto _st0;
			}
			_ctr195:
			{
#line 1 "NONE"
				{te = p+1;}}
			
#line 4154 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st138;
			_st138:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof138;
			st_case_138:
			switch( ( (*( p))) ) {
				case 97: {
					goto _st102;
				}
				case 101: {
					goto _st104;
				}
				case 105: {
					goto _st105;
				}
				case 113: {
					goto _st106;
				}
			}
			{
				goto _ctr234;
			}
			_st102:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof102;
			st_case_102:
			switch( ( (*( p))) ) {
				case 109: {
					goto _st103;
				}
				case 119: {
					goto _ctr163;
				}
			}
			{
				goto _ctr161;
			}
			_st103:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof103;
			st_case_103:
			if ( ( (*( p))) == 112 ) {
				goto _ctr164;
			}
			{
				goto _ctr161;
			}
			_st104:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof104;
			st_case_104:
			if ( ( (*( p))) == 99 ) {
				goto _ctr165;
			}
			{
				goto _ctr161;
			}
			_st105:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof105;
			st_case_105:
			if ( ( (*( p))) == 110 ) {
				goto _ctr166;
			}
			{
				goto _ctr161;
			}
			_st106:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof106;
			st_case_106:
			if ( ( (*( p))) == 114 ) {
				goto _st139;
			}
			{
				goto _ctr161;
			}
			_st139:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof139;
			st_case_139:
			if ( ( (*( p))) == 116 ) {
				goto _ctr240;
			}
			{
				goto _ctr239;
			}
			_st107:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof107;
			st_case_107:
			switch( ( (*( p))) ) {
				case 97: {
					goto _st108;
				}
				case 114: {
					goto _st109;
				}
			}
			{
				goto _st0;
			}
			_st108:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof108;
			st_case_108:
			if ( ( (*( p))) == 110 ) {
				goto _ctr171;
			}
			{
				goto _st0;
			}
			_st109:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof109;
			st_case_109:
			switch( ( (*( p))) ) {
				case 105: {
					goto _ctr172;
				}
				case 112: {
					goto _st110;
				}
			}
			{
				goto _st0;
			}
			_st110:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof110;
			st_case_110:
			if ( ( (*( p))) == 122 ) {
				goto _ctr174;
			}
			{
				goto _st0;
			}
			_st111:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof111;
			st_case_111:
			if ( ( (*( p))) == 115 ) {
				goto _st112;
			}
			{
				goto _st0;
			}
			_st112:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof112;
			st_case_112:
			if ( ( (*( p))) == 97 ) {
				goto _st113;
			}
			{
				goto _st0;
			}
			_st113:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof113;
			st_case_113:
			if ( ( (*( p))) == 119 ) {
				goto _ctr163;
			}
			{
				goto _st0;
			}
			_st114:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof114;
			st_case_114:
			if ( ( (*( p))) == 114 ) {
				goto _st115;
			}
			{
				goto _st0;
			}
			_st115:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof115;
			st_case_115:
			if ( ( (*( p))) == 97 ) {
				goto _st116;
			}
			{
				goto _st0;
			}
			_st116:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof116;
			st_case_116:
			if ( ( (*( p))) == 112 ) {
				goto _ctr181;
			}
			{
				goto _st0;
			}
			_ctr244:
			{
#line 102 "grain_expr.rl"
				{te = p;p = p - 1;}}
			
#line 4477 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st140;
			_ctr245:
			{
#line 103 "grain_expr.rl"
				{te = p;p = p - 1;{
#line 103 "grain_expr.rl"
						
						char buf[32];
						char* pbuf = buf;
						
						int n = 0;
						while(ts != te && n < sizeof(buf)-1) { *(pbuf++) = *(ts++); n++; }
						*pbuf = '\0';
						pushSymbol(gensym(buf));
						{goto _st117;}}
				}}
			
#line 4496 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			goto _st140;
			_st140:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			{
#line 1 "NONE"
				{ts = 0;}}
			
#line 4510 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			p+= 1;
			if ( p == pe )
				goto _test_eof140;
			st_case_140:
			{
#line 1 "NONE"
				{ts = p;}}
			
#line 4520 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
			
			switch( ( (*( p))) ) {
				case 32: {
					goto _st141;
				}
				case 95: {
					goto _st142;
				}
			}
			if ( ( (*( p))) > 57 ) {
				if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
					goto _st142;
				}
			} else if ( ( (*( p))) >= 48 ) {
				goto _st142;
			}
			{
				goto _st0;
			}
			_st141:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof141;
			st_case_141:
			if ( ( (*( p))) == 32 ) {
				goto _st141;
			}
			{
				goto _ctr244;
			}
			_st142:
			if ( p == eof ) {
				if ( cs >= 117 )
					goto _out;
				else
					goto _pop;
			}
			p+= 1;
			if ( p == pe )
				goto _test_eof142;
			st_case_142:
			if ( ( (*( p))) == 95 ) {
				goto _st142;
			}
			if ( ( (*( p))) > 57 ) {
				if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
					goto _st142;
				}
			} else if ( ( (*( p))) >= 48 ) {
				goto _st142;
			}
			{
				goto _ctr245;
			}
			st_out:
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
			if ( p == eof ) {
				switch ( cs ) {
					case 117: {
						{
#line 1 "NONE"
							{ts = p;}}
						
#line 4733 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
						
						break;
					}
					case 0: {
						break;
					}
					case 118: {
						break;
					}
					case 1: {
						break;
					}
					case 2: {
						break;
					}
					case 3: {
						break;
					}
					case 4: {
						break;
					}
					case 119: {
						break;
					}
					case 5: {
						break;
					}
					case 120: {
						break;
					}
					case 6: {
						break;
					}
					case 7: {
						break;
					}
					case 121: {
						break;
					}
					case 8: {
						break;
					}
					case 122: {
						break;
					}
					case 9: {
						break;
					}
					case 10: {
						break;
					}
					case 11: {
						break;
					}
					case 12: {
						break;
					}
					case 123: {
						break;
					}
					case 124: {
						break;
					}
					case 125: {
						break;
					}
					case 13: {
						break;
					}
					case 126: {
						break;
					}
					case 14: {
						break;
					}
					case 127: {
						break;
					}
					case 15: {
						break;
					}
					case 16: {
						break;
					}
					case 17: {
						break;
					}
					case 18: {
						break;
					}
					case 19: {
						break;
					}
					case 20: {
						break;
					}
					case 21: {
						break;
					}
					case 22: {
						break;
					}
					case 128: {
						break;
					}
					case 23: {
						break;
					}
					case 129: {
						break;
					}
					case 24: {
						break;
					}
					case 25: {
						break;
					}
					case 130: {
						break;
					}
					case 26: {
						break;
					}
					case 131: {
						break;
					}
					case 27: {
						break;
					}
					case 28: {
						break;
					}
					case 29: {
						break;
					}
					case 30: {
						break;
					}
					case 132: {
						break;
					}
					case 133: {
						break;
					}
					case 31: {
						break;
					}
					case 134: {
						break;
					}
					case 32: {
						break;
					}
					case 33: {
						break;
					}
					case 34: {
						break;
					}
					case 35: {
						break;
					}
					case 36: {
						break;
					}
					case 37: {
						break;
					}
					case 38: {
						break;
					}
					case 39: {
						break;
					}
					case 40: {
						break;
					}
					case 41: {
						break;
					}
					case 42: {
						break;
					}
					case 43: {
						break;
					}
					case 44: {
						break;
					}
					case 45: {
						break;
					}
					case 46: {
						break;
					}
					case 47: {
						break;
					}
					case 48: {
						break;
					}
					case 49: {
						break;
					}
					case 50: {
						break;
					}
					case 51: {
						break;
					}
					case 52: {
						break;
					}
					case 53: {
						break;
					}
					case 54: {
						break;
					}
					case 55: {
						break;
					}
					case 56: {
						break;
					}
					case 57: {
						break;
					}
					case 58: {
						break;
					}
					case 59: {
						break;
					}
					case 60: {
						break;
					}
					case 61: {
						break;
					}
					case 62: {
						break;
					}
					case 63: {
						break;
					}
					case 64: {
						break;
					}
					case 65: {
						break;
					}
					case 66: {
						break;
					}
					case 67: {
						break;
					}
					case 68: {
						break;
					}
					case 69: {
						break;
					}
					case 70: {
						break;
					}
					case 71: {
						break;
					}
					case 72: {
						break;
					}
					case 73: {
						break;
					}
					case 74: {
						break;
					}
					case 135: {
						break;
					}
					case 75: {
						break;
					}
					case 76: {
						break;
					}
					case 77: {
						break;
					}
					case 78: {
						break;
					}
					case 136: {
						break;
					}
					case 137: {
						break;
					}
					case 79: {
						break;
					}
					case 80: {
						break;
					}
					case 81: {
						break;
					}
					case 82: {
						break;
					}
					case 83: {
						break;
					}
					case 84: {
						break;
					}
					case 85: {
						break;
					}
					case 86: {
						break;
					}
					case 87: {
						break;
					}
					case 88: {
						break;
					}
					case 89: {
						break;
					}
					case 90: {
						break;
					}
					case 91: {
						break;
					}
					case 92: {
						break;
					}
					case 93: {
						break;
					}
					case 94: {
						break;
					}
					case 95: {
						break;
					}
					case 96: {
						break;
					}
					case 97: {
						break;
					}
					case 98: {
						break;
					}
					case 99: {
						break;
					}
					case 100: {
						break;
					}
					case 101: {
						break;
					}
					case 138: {
						break;
					}
					case 102: {
						break;
					}
					case 103: {
						break;
					}
					case 104: {
						break;
					}
					case 105: {
						break;
					}
					case 106: {
						break;
					}
					case 139: {
						break;
					}
					case 107: {
						break;
					}
					case 108: {
						break;
					}
					case 109: {
						break;
					}
					case 110: {
						break;
					}
					case 111: {
						break;
					}
					case 112: {
						break;
					}
					case 113: {
						break;
					}
					case 114: {
						break;
					}
					case 115: {
						break;
					}
					case 116: {
						break;
					}
					case 140: {
						{
#line 1 "NONE"
							{ts = p;}}
						
#line 5159 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
						
						break;
					}
					case 141: {
						break;
					}
					case 142: {
						break;
					}
				}
				switch ( cs ) {
				}
				switch ( cs ) {
					case 117:
					goto _st117;case 0:
					goto _st0;case 118:
					goto _ctr196;case 1:
					goto _st1;case 2:
					goto _st2;case 3:
					goto _st3;case 4:
					goto _st4;case 119:
					goto _ctr197;case 5:
					goto _ctr16;case 120:
					goto _ctr197;case 6:
					goto _ctr16;case 7:
					goto _ctr16;case 121:
					goto _ctr200;case 8:
					goto _ctr20;case 122:
					goto _ctr203;case 9:
					goto _ctr22;case 10:
					goto _ctr22;case 11:
					goto _ctr22;case 12:
					goto _st12;case 123:
					goto _ctr206;case 124:
					goto _ctr208;case 125:
					goto _ctr210;case 13:
					goto _st13;case 126:
					goto _ctr212;case 14:
					goto _ctr30;case 127:
					goto _ctr212;case 15:
					goto _st15;case 16:
					goto _st16;case 17:
					goto _st17;case 18:
					goto _st18;case 19:
					goto _st19;case 20:
					goto _st20;case 21:
					goto _st21;case 22:
					goto _st22;case 128:
					goto _ctr215;case 23:
					goto _ctr49;case 129:
					goto _ctr215;case 24:
					goto _ctr49;case 25:
					goto _ctr49;case 130:
					goto _ctr218;case 26:
					goto _ctr53;case 131:
					goto _ctr220;case 27:
					goto _ctr55;case 28:
					goto _ctr55;case 29:
					goto _ctr55;case 30:
					goto _st30;case 132:
					goto _ctr222;case 133:
					goto _ctr224;case 31:
					goto _ctr61;case 134:
					goto _ctr224;case 32:
					goto _ctr61;case 33:
					goto _ctr61;case 34:
					goto _st34;case 35:
					goto _st35;case 36:
					goto _st36;case 37:
					goto _st37;case 38:
					goto _st38;case 39:
					goto _st39;case 40:
					goto _st40;case 41:
					goto _st41;case 42:
					goto _st42;case 43:
					goto _st43;case 44:
					goto _st44;case 45:
					goto _st45;case 46:
					goto _st46;case 47:
					goto _st47;case 48:
					goto _st48;case 49:
					goto _st49;case 50:
					goto _st50;case 51:
					goto _st51;case 52:
					goto _st52;case 53:
					goto _st53;case 54:
					goto _st54;case 55:
					goto _st55;case 56:
					goto _st56;case 57:
					goto _st57;case 58:
					goto _st58;case 59:
					goto _st59;case 60:
					goto _st60;case 61:
					goto _st61;case 62:
					goto _st62;case 63:
					goto _st63;case 64:
					goto _st64;case 65:
					goto _st65;case 66:
					goto _st66;case 67:
					goto _st67;case 68:
					goto _st68;case 69:
					goto _st69;case 70:
					goto _st70;case 71:
					goto _st71;case 72:
					goto _st72;case 73:
					goto _st73;case 74:
					goto _st74;case 135:
					goto _ctr227;case 75:
					goto _ctr124;case 76:
					goto _st76;case 77:
					goto _st77;case 78:
					goto _st78;case 136:
					goto _ctr230;case 137:
					goto _ctr232;case 79:
					goto _ctr131;case 80:
					goto _st80;case 81:
					goto _st81;case 82:
					goto _st82;case 83:
					goto _st83;case 84:
					goto _st84;case 85:
					goto _st85;case 86:
					goto _st86;case 87:
					goto _st87;case 88:
					goto _st88;case 89:
					goto _st89;case 90:
					goto _st90;case 91:
					goto _st91;case 92:
					goto _st92;case 93:
					goto _st93;case 94:
					goto _st94;case 95:
					goto _st95;case 96:
					goto _st96;case 97:
					goto _st97;case 98:
					goto _st98;case 99:
					goto _st99;case 100:
					goto _st100;case 101:
					goto _st101;case 138:
					goto _ctr234;case 102:
					goto _ctr161;case 103:
					goto _ctr161;case 104:
					goto _ctr161;case 105:
					goto _ctr161;case 106:
					goto _ctr161;case 139:
					goto _ctr239;case 107:
					goto _st107;case 108:
					goto _st108;case 109:
					goto _st109;case 110:
					goto _st110;case 111:
					goto _st111;case 112:
					goto _st112;case 113:
					goto _st113;case 114:
					goto _st114;case 115:
					goto _st115;case 116:
					goto _st116;case 140:
					goto _st140;case 141:
					goto _ctr244;case 142:
					goto _ctr245;	}
			}
			
			if ( cs >= 117 )
				goto _out; _pop: {}
			_out: {}
		}
		
#line 439 "grain_expr.rl"
		
		
		if (cs < 
#line 5328 "/Users/serge/work/music/pure-data/ceammc/ext/src/array/grain_expr_parser.cpp"
		117
#line 441 "grain_expr.rl"
		) {
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
