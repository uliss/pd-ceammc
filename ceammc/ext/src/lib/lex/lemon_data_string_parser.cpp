#line 1 "lex/lemon_data_string_parser.rl"
# include "lemon_data_string_parser.h"
# include "ceammc_log.h"
# include "lemon_data_string.h"
# include "parser_numeric.h"
# include "fmt/format.h"

# include <cstdint>
# include <boost/static_string.hpp>

# include "lemon_data_parser_impl.h"


#line 206 "lex/lemon_data_string_parser.rl"


# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 24 "lex/lemon_data_string_parser.cpp"
		static const int lemon_data_string_lexer_start = 32;
		static const int lemon_data_string_lexer_first_final = 32;
		static const int lemon_data_string_lexer_error = 0;
		
		static const int lemon_data_string_lexer_en_scan_dqstring = 33;
		static const int lemon_data_string_lexer_en_scan_matrix = 35;
		static const int lemon_data_string_lexer_en_scan_dict = 46;
		static const int lemon_data_string_lexer_en_scan_token = 95;
		static const int lemon_data_string_lexer_en_main = 32;
		
		
#line 213 "lex/lemon_data_string_parser.rl"
		
		
		LemonDataStringParser::LemonDataStringParser()
		: res_(nullptr)
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
			lemon_data_string_parser_token(parser(), token, this);
		}
		
		void LemonDataStringParser::pushFloat(double val)
		{
# ifndef NDEBUG
			std::cerr << __FUNCTION__ << ' ' << val << std::endl;
# endif
			
			lemon_data_string_parser_float(parser(), TK_FLOAT, val, this);
		}
		
		void LemonDataStringParser::pushSymbolToken(int token, const char* begin, const char* end)
		{
			const auto N = std::min<int>(sizeof(parser_buf_) - 1, end - begin);
			
			for (int i = 0; i < N; i++)
			parser_buf_[i] = begin[i];
			
			parser_buf_[N] = 0;
			lemon_data_string_parser_str(parser(), token, parser_buf_, this);
		}
		
		bool LemonDataStringParser::parse(const char* data) noexcept
		{
# ifndef NDEBUG
			LIB_LOG << fmt::format("[data] parse: {}", data);
# endif
			
			reset();
			
			// null string pointer
			if (data == nullptr) {
				error_ = "[data] parse: null string pointer given";
				return false;
			}
			
			// empty string
			if (data[0] == '\0')
				return true;
			
			// ragel vars
			int cs;
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
			
			DECLARE_RAGEL_COMMON_VARS;
			DECLARE_RAGEL_NUMERIC_VARS;
			
			// for quoted string parser
			boost::static_string<512> ragel_string;
			// for matrices
			int ragel_mtx_cols = 0;
			int ragel_mtx_rows = 0;
			
			try {
				
				
#line 129 "lex/lemon_data_string_parser.cpp"
				{
					cs = (int)lemon_data_string_lexer_start;
					top = 0;
					ts = 0;
					te = 0;
					act = 0;
				}
				
#line 304 "lex/lemon_data_string_parser.rl"
				
				
#line 141 "lex/lemon_data_string_parser.cpp"
				{
					if ( p == pe )
						goto _test_eof;
					goto _resume;
					
					_again: {}
					switch ( cs ) {
						case 32: goto _st32;
						case 33: goto _st33;
						case 34: goto _st34;
						case 1: goto _st1;
						case 2: goto _st2;
						case 3: goto _st3;
						case 4: goto _st4;
						case 5: goto _st5;
						case 6: goto _st6;
						case 7: goto _st7;
						case 8: goto _st8;
						case 9: goto _st9;
						case 10: goto _st10;
						case 11: goto _st11;
						case 12: goto _st12;
						case 13: goto _st13;
						case 14: goto _st14;
						case 15: goto _st15;
						case 16: goto _st16;
						case 17: goto _st17;
						case 0: goto _st0;
						case 35: goto _st35;
						case 36: goto _st36;
						case 37: goto _st37;
						case 18: goto _st18;
						case 38: goto _st38;
						case 19: goto _st19;
						case 39: goto _st39;
						case 20: goto _st20;
						case 40: goto _st40;
						case 41: goto _st41;
						case 42: goto _st42;
						case 43: goto _st43;
						case 21: goto _st21;
						case 44: goto _st44;
						case 22: goto _st22;
						case 45: goto _st45;
						case 46: goto _st46;
						case 47: goto _st47;
						case 48: goto _st48;
						case 49: goto _st49;
						case 50: goto _st50;
						case 51: goto _st51;
						case 52: goto _st52;
						case 53: goto _st53;
						case 54: goto _st54;
						case 23: goto _st23;
						case 24: goto _st24;
						case 25: goto _st25;
						case 55: goto _st55;
						case 26: goto _st26;
						case 27: goto _st27;
						case 28: goto _st28;
						case 56: goto _st56;
						case 57: goto _st57;
						case 58: goto _st58;
						case 59: goto _st59;
						case 29: goto _st29;
						case 30: goto _st30;
						case 31: goto _st31;
						case 60: goto _st60;
						case 61: goto _st61;
						case 62: goto _st62;
						case 63: goto _st63;
						case 64: goto _st64;
						case 65: goto _st65;
						case 66: goto _st66;
						case 67: goto _st67;
						case 68: goto _st68;
						case 69: goto _st69;
						case 70: goto _st70;
						case 71: goto _st71;
						case 72: goto _st72;
						case 73: goto _st73;
						case 74: goto _st74;
						case 75: goto _st75;
						case 76: goto _st76;
						case 77: goto _st77;
						case 78: goto _st78;
						case 79: goto _st79;
						case 80: goto _st80;
						case 81: goto _st81;
						case 82: goto _st82;
						case 83: goto _st83;
						case 84: goto _st84;
						case 85: goto _st85;
						case 86: goto _st86;
						case 87: goto _st87;
						case 88: goto _st88;
						case 89: goto _st89;
						case 90: goto _st90;
						case 91: goto _st91;
						case 92: goto _st92;
						case 93: goto _st93;
						case 94: goto _st94;
						case 95: goto _st95;
						case 96: goto _st96;
						case 97: goto _st97;
						case 98: goto _st98;
						case 99: goto _st99;
						case 100: goto _st100;
						case 101: goto _st101;
						case 102: goto _st102;
						case 103: goto _st103;
						case 104: goto _st104;
						case 105: goto _st105;
						case 106: goto _st106;
						case 107: goto _st107;
						case 108: goto _st108;
						case 109: goto _st109;
						case 110: goto _st110;
						case 111: goto _st111;
						case 112: goto _st112;
						case 113: goto _st113;
						case 114: goto _st114;
						case 115: goto _st115;
						case 116: goto _st116;
						case 117: goto _st117;
						case 118: goto _st118;
						case 119: goto _st119;
						case 120: goto _st120;
						case 121: goto _st121;
						case 122: goto _st122;
						case 123: goto _st123;
						case 124: goto _st124;
						case 125: goto _st125;
						case 126: goto _st126;
						case 127: goto _st127;
						case 128: goto _st128;
						case 129: goto _st129;
						case 130: goto _st130;
						case 131: goto _st131;
						case 132: goto _st132;
						case 133: goto _st133;
						case 134: goto _st134;
						case 135: goto _st135;
						case 136: goto _st136;
						case 137: goto _st137;
						case 138: goto _st138;
					}
					
					_resume: {}
					switch ( cs ) {
						case 32:
						goto st_case_32;
						case 33:
						goto st_case_33;
						case 34:
						goto st_case_34;
						case 1:
						goto st_case_1;
						case 2:
						goto st_case_2;
						case 3:
						goto st_case_3;
						case 4:
						goto st_case_4;
						case 5:
						goto st_case_5;
						case 6:
						goto st_case_6;
						case 7:
						goto st_case_7;
						case 8:
						goto st_case_8;
						case 9:
						goto st_case_9;
						case 10:
						goto st_case_10;
						case 11:
						goto st_case_11;
						case 12:
						goto st_case_12;
						case 13:
						goto st_case_13;
						case 14:
						goto st_case_14;
						case 15:
						goto st_case_15;
						case 16:
						goto st_case_16;
						case 17:
						goto st_case_17;
						case 0:
						goto st_case_0;
						case 35:
						goto st_case_35;
						case 36:
						goto st_case_36;
						case 37:
						goto st_case_37;
						case 18:
						goto st_case_18;
						case 38:
						goto st_case_38;
						case 19:
						goto st_case_19;
						case 39:
						goto st_case_39;
						case 20:
						goto st_case_20;
						case 40:
						goto st_case_40;
						case 41:
						goto st_case_41;
						case 42:
						goto st_case_42;
						case 43:
						goto st_case_43;
						case 21:
						goto st_case_21;
						case 44:
						goto st_case_44;
						case 22:
						goto st_case_22;
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
						case 23:
						goto st_case_23;
						case 24:
						goto st_case_24;
						case 25:
						goto st_case_25;
						case 55:
						goto st_case_55;
						case 26:
						goto st_case_26;
						case 27:
						goto st_case_27;
						case 28:
						goto st_case_28;
						case 56:
						goto st_case_56;
						case 57:
						goto st_case_57;
						case 58:
						goto st_case_58;
						case 59:
						goto st_case_59;
						case 29:
						goto st_case_29;
						case 30:
						goto st_case_30;
						case 31:
						goto st_case_31;
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
						case 75:
						goto st_case_75;
						case 76:
						goto st_case_76;
						case 77:
						goto st_case_77;
						case 78:
						goto st_case_78;
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
						case 117:
						goto st_case_117;
						case 118:
						goto st_case_118;
						case 119:
						goto st_case_119;
						case 120:
						goto st_case_120;
						case 121:
						goto st_case_121;
						case 122:
						goto st_case_122;
						case 123:
						goto st_case_123;
						case 124:
						goto st_case_124;
						case 125:
						goto st_case_125;
						case 126:
						goto st_case_126;
						case 127:
						goto st_case_127;
						case 128:
						goto st_case_128;
						case 129:
						goto st_case_129;
						case 130:
						goto st_case_130;
						case 131:
						goto st_case_131;
						case 132:
						goto st_case_132;
						case 133:
						goto st_case_133;
						case 134:
						goto st_case_134;
						case 135:
						goto st_case_135;
						case 136:
						goto st_case_136;
						case 137:
						goto st_case_137;
						case 138:
						goto st_case_138;
					}
					goto st_out;
					_ctr44:
					{
#line 205 "lex/lemon_data_string_parser.rl"
						{p = p - 1; } {stack[top] = 32; top+= 1; goto _st95;}}
					
#line 577 "lex/lemon_data_string_parser.cpp"
					
					goto _st32;
					_st32:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 591 "lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof32;
					st_case_32:
					{
						goto _ctr44;
					}
					_ctr1:
					{
#line 131 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 131 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 609 "lex/lemon_data_string_parser.cpp"
					
					goto _st33;
					_ctr2:
					{
#line 60 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 60 "lex/lemon_data_string_parser.rl"
								
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
						}}
					
#line 644 "lex/lemon_data_string_parser.cpp"
					
					goto _st33;
					_ctr19:
					{
#line 132 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 132 "lex/lemon_data_string_parser.rl"
								ragel_string += '"'; }
						}}
					
#line 655 "lex/lemon_data_string_parser.cpp"
					
					goto _st33;
					_ctr20:
					{
#line 134 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 134 "lex/lemon_data_string_parser.rl"
								ragel_string += '{'; }
							}}
						
#line 666 "lex/lemon_data_string_parser.cpp"
						
						goto _st33;
						_ctr21:
						{
#line 135 "lex/lemon_data_string_parser.rl"
							{te = p+1;{
#line 135 "lex/lemon_data_string_parser.rl"
									ragel_string += '}'; }
						}}
					
#line 677 "lex/lemon_data_string_parser.cpp"
					
					goto _st33;
					_ctr22:
					{
#line 133 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 133 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 688 "lex/lemon_data_string_parser.cpp"
					
					goto _st33;
					_ctr46:
					{
#line 131 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 131 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 699 "lex/lemon_data_string_parser.cpp"
					
					goto _st33;
					_ctr47:
					{
#line 128 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 128 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 710 "lex/lemon_data_string_parser.cpp"
					
					goto _st33;
					_ctr49:
					{
#line 131 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 131 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 721 "lex/lemon_data_string_parser.cpp"
					
					goto _st33;
					_st33:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 735 "lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof33;
					st_case_33:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 745 "lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 34: {
							goto _ctr47;
						}
						case 37: {
							goto _ctr48;
						}
						case 96: {
							goto _st17;
						}
					}
					{
						goto _ctr46;
					}
					_ctr48:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 766 "lex/lemon_data_string_parser.cpp"
					
					goto _st34;
					_st34:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof34;
					st_case_34:
					if ( ( (*( p))) == 95 ) {
						goto _st1;
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st1;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st1;
					}
					{
						goto _ctr49;
					}
					_st1:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof1;
					st_case_1:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st2;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st2;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st2;
					}
					{
						goto _ctr1;
					}
					_st2:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof2;
					st_case_2:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st3;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st3;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st3;
					}
					{
						goto _ctr1;
					}
					_st3:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof3;
					st_case_3:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st4;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st4;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st4;
					}
					{
						goto _ctr1;
					}
					_st4:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof4;
					st_case_4:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st5;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st5;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st5;
					}
					{
						goto _ctr1;
					}
					_st5:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof5;
					st_case_5:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st6;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st6;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st6;
					}
					{
						goto _ctr1;
					}
					_st6:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof6;
					st_case_6:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st7;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st7;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st7;
					}
					{
						goto _ctr1;
					}
					_st7:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof7;
					st_case_7:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st8;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st8;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st8;
					}
					{
						goto _ctr1;
					}
					_st8:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof8;
					st_case_8:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st9;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st9;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st9;
					}
					{
						goto _ctr1;
					}
					_st9:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof9;
					st_case_9:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st10;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st10;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st10;
					}
					{
						goto _ctr1;
					}
					_st10:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof10;
					st_case_10:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st11;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st11;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st11;
					}
					{
						goto _ctr1;
					}
					_st11:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof11;
					st_case_11:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st12;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st12;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st12;
					}
					{
						goto _ctr1;
					}
					_st12:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof12;
					st_case_12:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st13;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st13;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st13;
					}
					{
						goto _ctr1;
					}
					_st13:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof13;
					st_case_13:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st14;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st14;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st14;
					}
					{
						goto _ctr1;
					}
					_st14:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof14;
					st_case_14:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st15;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st15;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st15;
					}
					{
						goto _ctr1;
					}
					_st15:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof15;
					st_case_15:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr2;
						}
						case 95: {
							goto _st16;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st16;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st16;
					}
					{
						goto _ctr1;
					}
					_st16:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof16;
					st_case_16:
					if ( ( (*( p))) == 37 ) {
						goto _ctr2;
					}
					{
						goto _ctr1;
					}
					_st17:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof17;
					st_case_17:
					switch( ( (*( p))) ) {
						case 34: {
							goto _ctr19;
						}
						case 40: {
							goto _ctr20;
						}
						case 41: {
							goto _ctr21;
						}
						case 96: {
							goto _ctr22;
						}
					}
					{
						goto _st0;
					}
					st_case_0:
					_st0:
					cs = 0;
					goto _pop;
					_ctr26:
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1291 "lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_ctr54:
					{
#line 23 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 23 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 1302 "lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_ctr58:
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 152 "lex/lemon_data_string_parser.rl"
								pushToken(TK_MATRIX_CLOSE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 1313 "lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_ctr59:
					{
#line 151 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 151 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 1324 "lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_ctr60:
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 1335 "lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_ctr61:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1347 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1361 "lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_ctr64:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1374 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1388 "lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_ctr65:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1400 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1414 "lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_ctr68:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1425 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1439 "lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_ctr69:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1450 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1464 "lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_st35:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 1478 "lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof35;
					st_case_35:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 1488 "lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _st36;
						}
						case 40: {
							goto _st37;
						}
						case 41: {
							goto _ctr54;
						}
						case 43: {
							goto _ctr55;
						}
						case 45: {
							goto _ctr55;
						}
						case 48: {
							goto _ctr56;
						}
						case 93: {
							goto _ctr58;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr57;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _st36;
					}
					{
						goto _st0;
					}
					_st36:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof36;
					st_case_36:
					switch( ( (*( p))) ) {
						case 32: {
							goto _st36;
						}
						case 41: {
							goto _ctr54;
						}
						case 93: {
							goto _ctr58;
						}
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st36;
					}
					{
						goto _ctr59;
					}
					_st37:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof37;
					st_case_37:
					if ( ( (*( p))) == 32 ) {
						goto _st37;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st37;
					}
					{
						goto _ctr60;
					}
					_ctr55:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 1576 "lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 1582 "lex/lemon_data_string_parser.cpp"
					
					goto _st18;
					_st18:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof18;
					st_case_18:
					if ( ( (*( p))) == 48 ) {
						goto _ctr24;
					}
					if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr25;
					}
					{
						goto _st0;
					}
					_ctr24:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1610 "lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 1616 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 1622 "lex/lemon_data_string_parser.cpp"
					
					goto _st38;
					_st38:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof38;
					st_case_38:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr62;
						}
						case 47: {
							goto _st20;
						}
					}
					{
						goto _ctr61;
					}
					_ctr62:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1656 "lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 1665 "lex/lemon_data_string_parser.cpp"
					
					goto _st19;
					_st19:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof19;
					st_case_19:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr27;
					}
					{
						goto _ctr26;
					}
					_ctr27:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 1693 "lex/lemon_data_string_parser.cpp"
					
					goto _st39;
					_st39:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof39;
					st_case_39:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr27;
					}
					{
						goto _ctr64;
					}
					_st20:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof20;
					st_case_20:
					if ( ( (*( p))) == 48 ) {
						goto _ctr28;
					}
					if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr29;
					}
					{
						goto _ctr26;
					}
					_ctr28:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 1738 "lex/lemon_data_string_parser.cpp"
					
					goto _st40;
					_st40:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof40;
					st_case_40:
					{
						goto _ctr65;
					}
					_ctr29:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 1760 "lex/lemon_data_string_parser.cpp"
					
					goto _st41;
					_st41:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof41;
					st_case_41:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr29;
					}
					{
						goto _ctr65;
					}
					_ctr25:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1785 "lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 1791 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 1797 "lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_ctr57:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1805 "lex/lemon_data_string_parser.cpp"
					
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 1811 "lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 1817 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 1823 "lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_st42:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof42;
					st_case_42:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr62;
						}
						case 47: {
							goto _st20;
						}
					}
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr25;
					}
					{
						goto _ctr61;
					}
					_ctr56:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1856 "lex/lemon_data_string_parser.cpp"
					
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 1862 "lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 1868 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 1874 "lex/lemon_data_string_parser.cpp"
					
					goto _st43;
					_st43:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof43;
					st_case_43:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr62;
						}
						case 47: {
							goto _st20;
						}
						case 98: {
							goto _st21;
						}
						case 120: {
							goto _st22;
						}
					}
					{
						goto _ctr61;
					}
					_st21:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof21;
					st_case_21:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr30;
					}
					{
						goto _ctr26;
					}
					_ctr30:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 1927 "lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_st44:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof44;
					st_case_44:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr30;
					}
					{
						goto _ctr68;
					}
					_st22:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof22;
					st_case_22:
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr31;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr31;
						}
					} else {
						goto _ctr31;
					}
					{
						goto _ctr26;
					}
					_ctr31:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 1977 "lex/lemon_data_string_parser.cpp"
					
					goto _st45;
					_st45:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof45;
					st_case_45:
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr31;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr31;
						}
					} else {
						goto _ctr31;
					}
					{
						goto _ctr69;
					}
					_ctr32:
					{
#line 177 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 177 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 2014 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr34:
					{
#line 1 "NONE"
						{switch( act ) {
								case 13:  {
									p = ((te))-1;
									{
#line 18 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 14:  {
									p = ((te))-1;
									{
#line 19 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 15:  {
									p = ((te))-1;
									{
#line 20 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 19:  {
									p = ((te))-1;
									{
#line 30 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 46; top+= 1; goto _st33;}}
									break; 
								}
								case 27:  {
									p = ((te))-1;
									{
#line 177 "lex/lemon_data_string_parser.rl"
										pushToken(TK_SPACE); }
									break; 
								}
								case 28:  {
									p = ((te))-1;
									{
#line 88 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 2066 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr39:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 88 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 2078 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr42:
					{
#line 140 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 140 "lex/lemon_data_string_parser.rl"
								
								pushToken(TK_MATRIX);
								pushFloat(ragel_mtx_rows);
								pushFloat(ragel_mtx_cols);
								{stack[top] = 46; top+= 1; goto _st35;}}
						}}
					
#line 2093 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr77:
					{
#line 23 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 23 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 2104 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr85:
					{
#line 181 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 181 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_CLOSE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 2115 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr87:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 88 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 2126 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr95:
					{
#line 177 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 177 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 2137 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr98:
					{
#line 51 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 51 "lex/lemon_data_string_parser.rl"
								
								// skip starting whitespaces
								auto ts0 = ts;
								while(*ts0 == ' ') ++ts0;
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DICT_KEY, ts0, te0);
							}
						}}
					
#line 2156 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr99:
					{
#line 30 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 30 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 46; top+= 1; goto _st33;}}
						}}
					
#line 2167 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr118:
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 2178 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr121:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2190 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2204 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr129:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2217 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2231 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr136:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2243 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2257 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr146:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2268 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2282 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr153:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2293 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2307 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr161:
					{
#line 45 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 45 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 46; top+= 1; goto _st46;}}
						}}
					
#line 2321 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr162:
					{
#line 38 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 38 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DATA_NAME, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 2338 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr163:
					{
#line 86 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 86 "lex/lemon_data_string_parser.rl"
								pushToken(TK_STRING); ragel_string.clear(); {stack[top] = 46; top+= 1; goto _st33;}}
						}}
					
#line 2349 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr164:
					{
#line 50 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 50 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); {stack[top] = 46; top+= 1; goto _st46;}}
						}}
					
#line 2360 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr166:
					{
#line 31 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 31 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 2377 "lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_st46:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 2391 "lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof46;
					st_case_46:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 2401 "lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr72;
						}
						case 34: {
							goto _ctr73;
						}
						case 35: {
							goto _st57;
						}
						case 39: {
							goto _st70;
						}
						case 40: {
							goto _st73;
						}
						case 41: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr78;
						}
						case 45: {
							goto _ctr78;
						}
						case 48: {
							goto _ctr79;
						}
						case 83: {
							goto _st91;
						}
						case 91: {
							goto _st92;
						}
						case 93: {
							goto _ctr85;
						}
						case 95: {
							goto _st83;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _ctr80;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr72;
						}
					} else if ( ( (*( p))) > 64 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st93;
							}
						} else {
							goto _st89;
						}
					} else {
						goto _st83;
					}
					{
						goto _st47;
					}
					_ctr122:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2475 "lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_ctr130:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2488 "lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_ctr137:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2500 "lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_ctr147:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2511 "lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_ctr154:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2522 "lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_st47:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof47;
					st_case_47:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_ctr123:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2590 "lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_ctr131:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2603 "lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_ctr138:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2615 "lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_ctr148:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2626 "lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_ctr155:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2637 "lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_st48:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof48;
					st_case_48:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 48: {
							goto _ctr90;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr91;
					}
					{
						goto _st47;
					}
					_ctr124:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2708 "lex/lemon_data_string_parser.cpp"
					
					goto _st49;
					_ctr132:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2721 "lex/lemon_data_string_parser.cpp"
					
					goto _st49;
					_ctr139:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2733 "lex/lemon_data_string_parser.cpp"
					
					goto _st49;
					_ctr149:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2744 "lex/lemon_data_string_parser.cpp"
					
					goto _st49;
					_ctr156:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2755 "lex/lemon_data_string_parser.cpp"
					
					goto _st49;
					_st49:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof49;
					st_case_49:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 63: {
							goto _st50;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st50;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st50;
							}
						} else if ( ( (*( p))) >= 64 ) {
							goto _ctr90;
						}
					} else {
						goto _st50;
					}
					{
						goto _st47;
					}
					_st50:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof50;
					st_case_50:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st51;
						}
						case 58: {
							goto _ctr87;
						}
						case 63: {
							goto _st50;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st50;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st50;
							}
						} else if ( ( (*( p))) >= 64 ) {
							goto _ctr90;
						}
					} else {
						goto _st50;
					}
					{
						goto _st47;
					}
					_st51:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof51;
					st_case_51:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 63: {
							goto _st50;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st50;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st50;
							}
						} else if ( ( (*( p))) >= 64 ) {
							goto _ctr90;
						}
					} else {
						goto _st50;
					}
					{
						goto _st47;
					}
					_ctr90:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2946 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 28;}}
					
#line 2952 "lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr94:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2960 "lex/lemon_data_string_parser.cpp"
					
					{
#line 106 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 2966 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 28;}}
					
#line 2972 "lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr109:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2980 "lex/lemon_data_string_parser.cpp"
					
					{
#line 19 "lex/lemon_data_string_parser.rl"
						{act = 14;}}
					
#line 2986 "lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr112:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2994 "lex/lemon_data_string_parser.cpp"
					
					{
#line 20 "lex/lemon_data_string_parser.rl"
						{act = 15;}}
					
#line 3000 "lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr115:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3008 "lex/lemon_data_string_parser.cpp"
					
					{
#line 18 "lex/lemon_data_string_parser.rl"
						{act = 13;}}
					
#line 3014 "lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr127:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3022 "lex/lemon_data_string_parser.cpp"
					
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3032 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 28;}}
					
#line 3038 "lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr133:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3046 "lex/lemon_data_string_parser.cpp"
					
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3057 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 28;}}
					
#line 3063 "lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr140:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3071 "lex/lemon_data_string_parser.cpp"
					
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3081 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 28;}}
					
#line 3087 "lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_st52:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof52;
					st_case_52:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr34;
						}
						case 34: {
							goto _ctr34;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr34;
						}
						case 91: {
							goto _ctr34;
						}
						case 93: {
							goto _ctr34;
						}
						case 95: {
							goto _ctr90;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr34;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr34;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_ctr91:
					{
#line 106 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 3154 "lex/lemon_data_string_parser.cpp"
					
					goto _st53;
					_st53:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof53;
					st_case_53:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr94;
					}
					{
						goto _st47;
					}
					_ctr72:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3221 "lex/lemon_data_string_parser.cpp"
					
					{
#line 177 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3227 "lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_st54:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof54;
					st_case_54:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr72;
						}
						case 34: {
							goto _st23;
						}
						case 39: {
							goto _st26;
						}
						case 41: {
							goto _ctr77;
						}
						case 93: {
							goto _ctr85;
						}
						case 95: {
							goto _st28;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr72;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st28;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _st28;
						}
					} else {
						goto _st28;
					}
					{
						goto _ctr95;
					}
					_st23:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof23;
					st_case_23:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st24;
						}
						case 95: {
							goto _st24;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st24;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st24;
					}
					{
						goto _ctr32;
					}
					_st24:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof24;
					st_case_24:
					switch( ( (*( p))) ) {
						case 34: {
							goto _st25;
						}
						case 63: {
							goto _st24;
						}
						case 95: {
							goto _st24;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st24;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st24;
					}
					{
						goto _ctr34;
					}
					_st25:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof25;
					st_case_25:
					if ( ( (*( p))) == 58 ) {
						goto _st55;
					}
					{
						goto _ctr34;
					}
					_ctr142:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3366 "lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_ctr151:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3377 "lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_ctr157:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3388 "lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_st55:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof55;
					st_case_55:
					if ( ( (*( p))) == 32 ) {
						goto _st55;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st55;
					}
					{
						goto _ctr98;
					}
					_st26:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof26;
					st_case_26:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st27;
						}
						case 95: {
							goto _st27;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st27;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st27;
					}
					{
						goto _ctr32;
					}
					_st27:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof27;
					st_case_27:
					switch( ( (*( p))) ) {
						case 39: {
							goto _st25;
						}
						case 63: {
							goto _st27;
						}
						case 95: {
							goto _st27;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st27;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st27;
					}
					{
						goto _ctr32;
					}
					_st28:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof28;
					st_case_28:
					switch( ( (*( p))) ) {
						case 58: {
							goto _st55;
						}
						case 95: {
							goto _st28;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _st28;
						}
					} else if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st28;
						}
					} else {
						goto _st28;
					}
					{
						goto _ctr32;
					}
					_ctr73:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3510 "lex/lemon_data_string_parser.cpp"
					
					{
#line 30 "lex/lemon_data_string_parser.rl"
						{act = 19;}}
					
#line 3516 "lex/lemon_data_string_parser.cpp"
					
					goto _st56;
					_st56:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof56;
					st_case_56:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st24;
						}
						case 95: {
							goto _st24;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st24;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st24;
					}
					{
						goto _ctr99;
					}
					_st57:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof57;
					st_case_57:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 48: {
							goto _ctr90;
						}
						case 91: {
							goto _ctr42;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
						case 102: {
							goto _st60;
						}
						case 110: {
							goto _st64;
						}
						case 116: {
							goto _st67;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr100;
					}
					{
						goto _st47;
					}
					_ctr100:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3621 "lex/lemon_data_string_parser.cpp"
					
					{
#line 106 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 3627 "lex/lemon_data_string_parser.cpp"
					
					goto _st58;
					_st58:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof58;
					st_case_58:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _st29;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr104;
					}
					{
						goto _st47;
					}
					_ctr104:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3694 "lex/lemon_data_string_parser.cpp"
					
					{
#line 106 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 3700 "lex/lemon_data_string_parser.cpp"
					
					goto _st59;
					_st59:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof59;
					st_case_59:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _st29;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_st29:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof29;
					st_case_29:
					if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr40;
					}
					{
						goto _ctr39;
					}
					_ctr40:
					{
#line 105 "lex/lemon_data_string_parser.rl"
						ragel_mtx_cols = (( (*( p)))) - '0'; }
					
#line 3784 "lex/lemon_data_string_parser.cpp"
					
					goto _st30;
					_st30:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof30;
					st_case_30:
					if ( ( (*( p))) == 91 ) {
						goto _ctr42;
					}
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
					{
						goto _ctr39;
					}
					_ctr41:
					{
#line 105 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_cols *= 10) += ((( (*( p)))) - '0'); }
					
#line 3812 "lex/lemon_data_string_parser.cpp"
					
					goto _st31;
					_st31:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof31;
					st_case_31:
					if ( ( (*( p))) == 91 ) {
						goto _ctr42;
					}
					{
						goto _ctr39;
					}
					_st60:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof60;
					st_case_60:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
						case 97: {
							goto _st61;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_st61:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof61;
					st_case_61:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
						case 108: {
							goto _st62;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_st62:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof62;
					st_case_62:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
						case 115: {
							goto _st63;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_st63:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof63;
					st_case_63:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
						case 101: {
							goto _ctr109;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_st64:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof64;
					st_case_64:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
						case 117: {
							goto _st65;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_st65:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof65;
					st_case_65:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
						case 108: {
							goto _st66;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_st66:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof66;
					st_case_66:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
						case 108: {
							goto _ctr112;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_st67:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof67;
					st_case_67:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
						case 114: {
							goto _st68;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_st68:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof68;
					st_case_68:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
						case 117: {
							goto _st69;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_st69:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof69;
					st_case_69:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _ctr87;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
						case 101: {
							goto _ctr115;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr90;
					}
					{
						goto _st47;
					}
					_st70:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof70;
					st_case_70:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 63: {
							goto _st71;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st71;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st71;
							}
						} else if ( ( (*( p))) >= 64 ) {
							goto _ctr90;
						}
					} else {
						goto _st71;
					}
					{
						goto _st47;
					}
					_st71:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof71;
					st_case_71:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st72;
						}
						case 58: {
							goto _ctr87;
						}
						case 63: {
							goto _st71;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st71;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st71;
							}
						} else if ( ( (*( p))) >= 64 ) {
							goto _ctr90;
						}
					} else {
						goto _st71;
					}
					{
						goto _st47;
					}
					_st72:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof72;
					st_case_72:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _st55;
						}
						case 63: {
							goto _st50;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st50;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st50;
							}
						} else if ( ( (*( p))) >= 64 ) {
							goto _ctr90;
						}
					} else {
						goto _st50;
					}
					{
						goto _st47;
					}
					_st73:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof73;
					st_case_73:
					if ( ( (*( p))) == 32 ) {
						goto _st73;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st73;
					}
					{
						goto _ctr118;
					}
					_ctr78:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 4660 "lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 4666 "lex/lemon_data_string_parser.cpp"
					
					goto _st74;
					_st74:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof74;
					st_case_74:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 48: {
							goto _ctr119;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr120;
					}
					{
						goto _st47;
					}
					_ctr119:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 4733 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 4739 "lex/lemon_data_string_parser.cpp"
					
					goto _st75;
					_st75:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof75;
					st_case_75:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr121;
						}
						case 34: {
							goto _ctr121;
						}
						case 35: {
							goto _ctr123;
						}
						case 39: {
							goto _ctr124;
						}
						case 46: {
							goto _ctr125;
						}
						case 47: {
							goto _st78;
						}
						case 58: {
							goto _ctr121;
						}
						case 91: {
							goto _ctr121;
						}
						case 93: {
							goto _ctr121;
						}
						case 95: {
							goto _ctr127;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr121;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr121;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr127;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr127;
						}
					} else {
						goto _ctr127;
					}
					{
						goto _ctr122;
					}
					_ctr125:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4816 "lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 4825 "lex/lemon_data_string_parser.cpp"
					
					goto _st76;
					_st76:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof76;
					st_case_76:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr128;
					}
					{
						goto _st47;
					}
					_ctr128:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 4892 "lex/lemon_data_string_parser.cpp"
					
					goto _st77;
					_st77:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof77;
					st_case_77:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr129;
						}
						case 34: {
							goto _ctr129;
						}
						case 35: {
							goto _ctr131;
						}
						case 39: {
							goto _ctr132;
						}
						case 58: {
							goto _ctr129;
						}
						case 91: {
							goto _ctr129;
						}
						case 93: {
							goto _ctr129;
						}
						case 95: {
							goto _ctr133;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr129;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr129;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr133;
						}
					} else {
						goto _ctr128;
					}
					{
						goto _ctr130;
					}
					_st78:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof78;
					st_case_78:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 48: {
							goto _ctr134;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr90;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr90;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr90;
						}
					} else {
						goto _ctr135;
					}
					{
						goto _st47;
					}
					_ctr134:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 5018 "lex/lemon_data_string_parser.cpp"
					
					goto _st79;
					_st79:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof79;
					st_case_79:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr136;
						}
						case 34: {
							goto _ctr136;
						}
						case 35: {
							goto _ctr138;
						}
						case 39: {
							goto _ctr139;
						}
						case 58: {
							goto _ctr136;
						}
						case 91: {
							goto _ctr136;
						}
						case 93: {
							goto _ctr136;
						}
						case 95: {
							goto _ctr140;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr136;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr136;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr140;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr140;
						}
					} else {
						goto _ctr140;
					}
					{
						goto _ctr137;
					}
					_ctr135:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 5085 "lex/lemon_data_string_parser.cpp"
					
					goto _st80;
					_st80:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof80;
					st_case_80:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr136;
						}
						case 34: {
							goto _ctr136;
						}
						case 35: {
							goto _ctr138;
						}
						case 39: {
							goto _ctr139;
						}
						case 58: {
							goto _ctr136;
						}
						case 91: {
							goto _ctr136;
						}
						case 93: {
							goto _ctr136;
						}
						case 95: {
							goto _ctr140;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr136;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr136;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr140;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr140;
						}
					} else {
						goto _ctr135;
					}
					{
						goto _ctr137;
					}
					_ctr120:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5152 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5158 "lex/lemon_data_string_parser.cpp"
					
					goto _st81;
					_st81:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof81;
					st_case_81:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr121;
						}
						case 34: {
							goto _ctr121;
						}
						case 35: {
							goto _ctr123;
						}
						case 39: {
							goto _ctr124;
						}
						case 46: {
							goto _ctr125;
						}
						case 47: {
							goto _st78;
						}
						case 58: {
							goto _ctr121;
						}
						case 91: {
							goto _ctr121;
						}
						case 93: {
							goto _ctr121;
						}
						case 95: {
							goto _ctr127;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr121;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr121;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr127;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr127;
						}
					} else {
						goto _ctr120;
					}
					{
						goto _ctr122;
					}
					_ctr79:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 5231 "lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5237 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5243 "lex/lemon_data_string_parser.cpp"
					
					goto _st82;
					_st82:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof82;
					st_case_82:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr121;
						}
						case 34: {
							goto _ctr121;
						}
						case 35: {
							goto _ctr123;
						}
						case 39: {
							goto _ctr124;
						}
						case 46: {
							goto _ctr125;
						}
						case 47: {
							goto _st78;
						}
						case 58: {
							goto _ctr142;
						}
						case 91: {
							goto _ctr121;
						}
						case 93: {
							goto _ctr121;
						}
						case 95: {
							goto _ctr141;
						}
						case 98: {
							goto _st84;
						}
						case 120: {
							goto _st86;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr121;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr121;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr141;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr122;
					}
					_ctr141:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5326 "lex/lemon_data_string_parser.cpp"
					
					goto _st83;
					_ctr150:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5337 "lex/lemon_data_string_parser.cpp"
					
					goto _st83;
					_ctr158:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5348 "lex/lemon_data_string_parser.cpp"
					
					goto _st83;
					_st83:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof83;
					st_case_83:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _st55;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st83;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st83;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _st83;
						}
					} else {
						goto _st83;
					}
					{
						goto _st47;
					}
					_st84:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof84;
					st_case_84:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _st55;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st83;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 49 ) {
						if ( ( (*( p))) < 63 ) {
							if ( ( (*( p))) <= 57 ) {
								goto _st83;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st83;
							}
						} else {
							goto _st83;
						}
					} else {
						goto _ctr145;
					}
					{
						goto _st47;
					}
					_ctr145:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 5478 "lex/lemon_data_string_parser.cpp"
					
					goto _st85;
					_st85:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof85;
					st_case_85:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr146;
						}
						case 34: {
							goto _ctr146;
						}
						case 35: {
							goto _ctr148;
						}
						case 39: {
							goto _ctr149;
						}
						case 58: {
							goto _ctr151;
						}
						case 91: {
							goto _ctr146;
						}
						case 93: {
							goto _ctr146;
						}
						case 95: {
							goto _ctr150;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr146;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr146;
						}
					} else if ( ( (*( p))) > 49 ) {
						if ( ( (*( p))) < 63 ) {
							if ( ( (*( p))) <= 57 ) {
								goto _ctr150;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr150;
							}
						} else {
							goto _ctr150;
						}
					} else {
						goto _ctr145;
					}
					{
						goto _ctr147;
					}
					_st86:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof86;
					st_case_86:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 58: {
							goto _st55;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st83;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) < 40 ) {
							if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) > 41 ) {
							if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _ctr152;
							}
						} else {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 64 ) {
						if ( ( (*( p))) < 71 ) {
							{
								goto _ctr152;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( ( (*( p))) > 102 ) {
								if ( ( (*( p))) <= 122 ) {
									goto _st83;
								}
							} else if ( ( (*( p))) >= 97 ) {
								goto _ctr152;
							}
						} else {
							goto _st83;
						}
					} else {
						goto _st83;
					}
					{
						goto _st47;
					}
					_ctr152:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 5620 "lex/lemon_data_string_parser.cpp"
					
					goto _st87;
					_st87:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof87;
					st_case_87:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr153;
						}
						case 34: {
							goto _ctr153;
						}
						case 35: {
							goto _ctr155;
						}
						case 39: {
							goto _ctr156;
						}
						case 58: {
							goto _ctr157;
						}
						case 91: {
							goto _ctr153;
						}
						case 93: {
							goto _ctr153;
						}
						case 95: {
							goto _ctr158;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) < 40 ) {
							if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
								goto _ctr153;
							}
						} else if ( ( (*( p))) > 41 ) {
							if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _ctr152;
							}
						} else {
							goto _ctr153;
						}
					} else if ( ( (*( p))) > 64 ) {
						if ( ( (*( p))) < 71 ) {
							{
								goto _ctr152;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( ( (*( p))) > 102 ) {
								if ( ( (*( p))) <= 122 ) {
									goto _ctr158;
								}
							} else if ( ( (*( p))) >= 97 ) {
								goto _ctr152;
							}
						} else {
							goto _ctr158;
						}
					} else {
						goto _ctr158;
					}
					{
						goto _ctr154;
					}
					_ctr80:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 5699 "lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5705 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5711 "lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr159:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5719 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5725 "lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_st88:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof88;
					st_case_88:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr121;
						}
						case 34: {
							goto _ctr121;
						}
						case 35: {
							goto _ctr123;
						}
						case 39: {
							goto _ctr124;
						}
						case 46: {
							goto _ctr125;
						}
						case 47: {
							goto _st78;
						}
						case 58: {
							goto _ctr142;
						}
						case 91: {
							goto _ctr121;
						}
						case 93: {
							goto _ctr121;
						}
						case 95: {
							goto _ctr141;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr121;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr121;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr141;
						}
					} else {
						goto _ctr159;
					}
					{
						goto _ctr122;
					}
					_st89:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof89;
					st_case_89:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 40: {
							goto _st90;
						}
						case 41: {
							goto _ctr87;
						}
						case 58: {
							goto _st55;
						}
						case 91: {
							goto _ctr161;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st83;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _st83;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 64 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st89;
							}
						} else {
							goto _st89;
						}
					} else {
						goto _st83;
					}
					{
						goto _st47;
					}
					_st90:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof90;
					st_case_90:
					if ( ( (*( p))) == 32 ) {
						goto _st90;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st90;
					}
					{
						goto _ctr162;
					}
					_st91:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof91;
					st_case_91:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr163;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 40: {
							goto _st90;
						}
						case 41: {
							goto _ctr87;
						}
						case 58: {
							goto _st55;
						}
						case 91: {
							goto _ctr161;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st83;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _st83;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 64 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st89;
							}
						} else {
							goto _st89;
						}
					} else {
						goto _st83;
					}
					{
						goto _st47;
					}
					_st92:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof92;
					st_case_92:
					if ( ( (*( p))) == 32 ) {
						goto _st92;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st92;
					}
					{
						goto _ctr164;
					}
					_st93:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof93;
					st_case_93:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 35: {
							goto _st48;
						}
						case 39: {
							goto _st49;
						}
						case 40: {
							goto _st94;
						}
						case 41: {
							goto _ctr87;
						}
						case 58: {
							goto _st55;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st93;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st93;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _st83;
						}
					} else {
						goto _st93;
					}
					{
						goto _st47;
					}
					_st94:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof94;
					st_case_94:
					if ( ( (*( p))) == 32 ) {
						goto _st94;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st94;
					}
					{
						goto _ctr166;
					}
					_ctr170:
					{
#line 30 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 30 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 95; top+= 1; goto _st33;}}
						}}
					
#line 6052 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr173:
					{
#line 23 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 23 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 6063 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr181:
					{
#line 1 "NONE"
						{switch( act ) {
								case 30:  {
									p = ((te))-1;
									{
#line 18 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 31:  {
									p = ((te))-1;
									{
#line 19 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 32:  {
									p = ((te))-1;
									{
#line 20 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 43:  {
									p = ((te))-1;
									{
#line 88 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 6101 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr183:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 88 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 6112 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr189:
					{
#line 202 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 202 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 6123 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr191:
					{
#line 140 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 140 "lex/lemon_data_string_parser.rl"
								
								pushToken(TK_MATRIX);
								pushFloat(ragel_mtx_rows);
								pushFloat(ragel_mtx_cols);
								{stack[top] = 95; top+= 1; goto _st35;}}
						}}
					
#line 6138 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr209:
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 6149 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr212:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6161 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6175 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr218:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6188 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6202 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr223:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6214 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6228 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr229:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6239 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6253 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr233:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6264 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 24 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6278 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr237:
					{
#line 45 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 45 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 95; top+= 1; goto _st46;}}
						}}
					
#line 6292 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr238:
					{
#line 38 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 38 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DATA_NAME, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 6309 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr239:
					{
#line 86 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 86 "lex/lemon_data_string_parser.rl"
								pushToken(TK_STRING); ragel_string.clear(); {stack[top] = 95; top+= 1; goto _st33;}}
						}}
					
#line 6320 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr240:
					{
#line 190 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 190 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); {stack[top] = 95; top+= 1; goto _st46;}}
						}}
					
#line 6331 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr242:
					{
#line 31 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 31 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 6348 "lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_st95:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 6362 "lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof95;
					st_case_95:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 6372 "lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _st102;
						}
						case 34: {
							goto _ctr170;
						}
						case 35: {
							goto _st103;
						}
						case 40: {
							goto _st119;
						}
						case 41: {
							goto _ctr173;
						}
						case 43: {
							goto _ctr174;
						}
						case 45: {
							goto _ctr174;
						}
						case 48: {
							goto _ctr175;
						}
						case 83: {
							goto _st135;
						}
						case 91: {
							goto _st136;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _st102;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st137;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st133;
						}
					} else {
						goto _ctr176;
					}
					{
						goto _ctr168;
					}
					_ctr168:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6429 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 6435 "lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_ctr188:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6443 "lex/lemon_data_string_parser.cpp"
					
					{
#line 105 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_cols *= 10) += ((( (*( p)))) - '0'); }
					
#line 6449 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 6455 "lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_ctr202:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6463 "lex/lemon_data_string_parser.cpp"
					
					{
#line 19 "lex/lemon_data_string_parser.rl"
						{act = 31;}}
					
#line 6469 "lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_ctr205:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6477 "lex/lemon_data_string_parser.cpp"
					
					{
#line 20 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 6483 "lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_ctr208:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6491 "lex/lemon_data_string_parser.cpp"
					
					{
#line 18 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 6497 "lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_ctr213:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6505 "lex/lemon_data_string_parser.cpp"
					
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6515 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 6521 "lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_ctr219:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6529 "lex/lemon_data_string_parser.cpp"
					
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6540 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 6546 "lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_ctr224:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6554 "lex/lemon_data_string_parser.cpp"
					
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6564 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 6570 "lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_ctr230:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6578 "lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6587 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 6593 "lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_ctr234:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6601 "lex/lemon_data_string_parser.cpp"
					
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6610 "lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 6616 "lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_st96:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof96;
					st_case_96:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr181;
						}
						case 34: {
							goto _ctr181;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr181;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr181;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr181;
					}
					{
						goto _ctr168;
					}
					_ctr214:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6663 "lex/lemon_data_string_parser.cpp"
					
					goto _st97;
					_ctr220:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6676 "lex/lemon_data_string_parser.cpp"
					
					goto _st97;
					_ctr225:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6688 "lex/lemon_data_string_parser.cpp"
					
					goto _st97;
					_ctr231:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6699 "lex/lemon_data_string_parser.cpp"
					
					goto _st97;
					_ctr235:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6710 "lex/lemon_data_string_parser.cpp"
					
					goto _st97;
					_st97:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof97;
					st_case_97:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr184;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_ctr184:
					{
#line 106 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 6757 "lex/lemon_data_string_parser.cpp"
					
					goto _st98;
					_st98:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof98;
					st_case_98:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 58: {
							goto _st100;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr185;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_ctr185:
					{
#line 106 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 6807 "lex/lemon_data_string_parser.cpp"
					
					goto _st99;
					_st99:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof99;
					st_case_99:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 58: {
							goto _st100;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st100:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof100;
					st_case_100:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr187;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_ctr187:
					{
#line 105 "lex/lemon_data_string_parser.rl"
						ragel_mtx_cols = (( (*( p)))) - '0'; }
					
#line 6892 "lex/lemon_data_string_parser.cpp"
					
					goto _st101;
					_st101:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof101;
					st_case_101:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr188;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st102:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof102;
					st_case_102:
					switch( ( (*( p))) ) {
						case 32: {
							goto _st102;
						}
						case 41: {
							goto _ctr173;
						}
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st102;
					}
					{
						goto _ctr189;
					}
					_st103:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof103;
					st_case_103:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr191;
						}
						case 102: {
							goto _st109;
						}
						case 110: {
							goto _st113;
						}
						case 116: {
							goto _st116;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr190;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_ctr190:
					{
#line 106 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 7012 "lex/lemon_data_string_parser.cpp"
					
					goto _st104;
					_st104:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof104;
					st_case_104:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 58: {
							goto _st106;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr195;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_ctr195:
					{
#line 106 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 7062 "lex/lemon_data_string_parser.cpp"
					
					goto _st105;
					_st105:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof105;
					st_case_105:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 58: {
							goto _st106;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st106:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof106;
					st_case_106:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr197;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_ctr197:
					{
#line 105 "lex/lemon_data_string_parser.rl"
						ragel_mtx_cols = (( (*( p)))) - '0'; }
					
#line 7147 "lex/lemon_data_string_parser.cpp"
					
					goto _st107;
					_st107:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof107;
					st_case_107:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr191;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr198;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_ctr198:
					{
#line 105 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_cols *= 10) += ((( (*( p)))) - '0'); }
					
#line 7194 "lex/lemon_data_string_parser.cpp"
					
					goto _st108;
					_st108:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof108;
					st_case_108:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr191;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st109:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof109;
					st_case_109:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
						case 97: {
							goto _st110;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st110:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof110;
					st_case_110:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
						case 108: {
							goto _st111;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st111:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof111;
					st_case_111:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
						case 115: {
							goto _st112;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st112:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof112;
					st_case_112:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
						case 101: {
							goto _ctr202;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st113:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof113;
					st_case_113:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
						case 117: {
							goto _st114;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st114:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof114;
					st_case_114:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
						case 108: {
							goto _st115;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st115:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof115;
					st_case_115:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
						case 108: {
							goto _ctr205;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st116:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof116;
					st_case_116:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
						case 114: {
							goto _st117;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st117:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof117;
					st_case_117:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
						case 117: {
							goto _st118;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st118:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof118;
					st_case_118:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
						case 101: {
							goto _ctr208;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_st119:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof119;
					st_case_119:
					if ( ( (*( p))) == 32 ) {
						goto _st119;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st119;
					}
					{
						goto _ctr209;
					}
					_ctr174:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 7637 "lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 7643 "lex/lemon_data_string_parser.cpp"
					
					goto _st120;
					_st120:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof120;
					st_case_120:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 48: {
							goto _ctr210;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr211;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_ctr210:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 7693 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 7699 "lex/lemon_data_string_parser.cpp"
					
					goto _st121;
					_st121:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof121;
					st_case_121:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr212;
						}
						case 34: {
							goto _ctr212;
						}
						case 35: {
							goto _ctr214;
						}
						case 46: {
							goto _ctr215;
						}
						case 47: {
							goto _st124;
						}
						case 91: {
							goto _ctr212;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr212;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr212;
					}
					{
						goto _ctr213;
					}
					_ctr215:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 7752 "lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 7761 "lex/lemon_data_string_parser.cpp"
					
					goto _st122;
					_st122:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof122;
					st_case_122:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr217;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_ctr217:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 7811 "lex/lemon_data_string_parser.cpp"
					
					goto _st123;
					_st123:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof123;
					st_case_123:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr218;
						}
						case 34: {
							goto _ctr218;
						}
						case 35: {
							goto _ctr220;
						}
						case 91: {
							goto _ctr218;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr218;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr217;
						}
					} else {
						goto _ctr218;
					}
					{
						goto _ctr219;
					}
					_st124:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof124;
					st_case_124:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 48: {
							goto _ctr221;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr222;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_ctr221:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 7900 "lex/lemon_data_string_parser.cpp"
					
					goto _st125;
					_st125:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof125;
					st_case_125:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr223;
						}
						case 34: {
							goto _ctr223;
						}
						case 35: {
							goto _ctr225;
						}
						case 91: {
							goto _ctr223;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr223;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr223;
					}
					{
						goto _ctr224;
					}
					_ctr222:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 7943 "lex/lemon_data_string_parser.cpp"
					
					goto _st126;
					_st126:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof126;
					st_case_126:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr223;
						}
						case 34: {
							goto _ctr223;
						}
						case 35: {
							goto _ctr225;
						}
						case 91: {
							goto _ctr223;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr223;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr222;
						}
					} else {
						goto _ctr223;
					}
					{
						goto _ctr224;
					}
					_ctr176:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 7990 "lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 7996 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 8002 "lex/lemon_data_string_parser.cpp"
					
					goto _st127;
					_ctr211:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 8010 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 8016 "lex/lemon_data_string_parser.cpp"
					
					goto _st127;
					_st127:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof127;
					st_case_127:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr212;
						}
						case 34: {
							goto _ctr212;
						}
						case 35: {
							goto _ctr214;
						}
						case 46: {
							goto _ctr215;
						}
						case 47: {
							goto _st124;
						}
						case 91: {
							goto _ctr212;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr212;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr211;
						}
					} else {
						goto _ctr212;
					}
					{
						goto _ctr213;
					}
					_ctr175:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 8069 "lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 8075 "lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 8081 "lex/lemon_data_string_parser.cpp"
					
					goto _st128;
					_st128:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof128;
					st_case_128:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr212;
						}
						case 34: {
							goto _ctr212;
						}
						case 35: {
							goto _ctr214;
						}
						case 46: {
							goto _ctr215;
						}
						case 47: {
							goto _st124;
						}
						case 91: {
							goto _ctr212;
						}
						case 98: {
							goto _st129;
						}
						case 120: {
							goto _st131;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr212;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr212;
					}
					{
						goto _ctr213;
					}
					_st129:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof129;
					st_case_129:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
							goto _ctr228;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr168;
					}
					_ctr228:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 8175 "lex/lemon_data_string_parser.cpp"
					
					goto _st130;
					_st130:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof130;
					st_case_130:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr229;
						}
						case 34: {
							goto _ctr229;
						}
						case 35: {
							goto _ctr231;
						}
						case 91: {
							goto _ctr229;
						}
					}
					if ( ( (*( p))) < 40 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr229;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
							goto _ctr228;
						}
					} else {
						goto _ctr229;
					}
					{
						goto _ctr230;
					}
					_st131:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof131;
					st_case_131:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr183;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 70 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
								goto _ctr232;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _ctr232;
						}
					} else {
						goto _ctr232;
					}
					{
						goto _ctr168;
					}
					_ctr232:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 8269 "lex/lemon_data_string_parser.cpp"
					
					goto _st132;
					_st132:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof132;
					st_case_132:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr233;
						}
						case 34: {
							goto _ctr233;
						}
						case 35: {
							goto _ctr235;
						}
						case 91: {
							goto _ctr233;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 40 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr233;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr233;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 70 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
								goto _ctr232;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _ctr232;
						}
					} else {
						goto _ctr232;
					}
					{
						goto _ctr234;
					}
					_st133:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof133;
					st_case_133:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 40: {
							goto _st134;
						}
						case 41: {
							goto _ctr183;
						}
						case 91: {
							goto _ctr237;
						}
					}
					if ( ( (*( p))) < 65 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st133;
						}
					} else {
						goto _st133;
					}
					{
						goto _ctr168;
					}
					_st134:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof134;
					st_case_134:
					if ( ( (*( p))) == 32 ) {
						goto _st134;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st134;
					}
					{
						goto _ctr238;
					}
					_st135:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof135;
					st_case_135:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr239;
						}
						case 35: {
							goto _st97;
						}
						case 40: {
							goto _st134;
						}
						case 41: {
							goto _ctr183;
						}
						case 91: {
							goto _ctr237;
						}
					}
					if ( ( (*( p))) < 65 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st133;
						}
					} else {
						goto _st133;
					}
					{
						goto _ctr168;
					}
					_st136:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof136;
					st_case_136:
					if ( ( (*( p))) == 32 ) {
						goto _st136;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st136;
					}
					{
						goto _ctr240;
					}
					_st137:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof137;
					st_case_137:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr183;
						}
						case 34: {
							goto _ctr183;
						}
						case 35: {
							goto _st97;
						}
						case 40: {
							goto _st138;
						}
						case 41: {
							goto _ctr183;
						}
						case 91: {
							goto _ctr183;
						}
						case 95: {
							goto _st137;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st137;
						}
					} else {
						goto _st137;
					}
					{
						goto _ctr168;
					}
					_st138:
					if ( p == eof ) {
						if ( cs >= 32 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof138;
					st_case_138:
					if ( ( (*( p))) == 32 ) {
						goto _st138;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st138;
					}
					{
						goto _ctr242;
					}
					st_out:
					_test_eof32: cs = 32; goto _test_eof; 
					_test_eof33: cs = 33; goto _test_eof; 
					_test_eof34: cs = 34; goto _test_eof; 
					_test_eof1: cs = 1; goto _test_eof; 
					_test_eof2: cs = 2; goto _test_eof; 
					_test_eof3: cs = 3; goto _test_eof; 
					_test_eof4: cs = 4; goto _test_eof; 
					_test_eof5: cs = 5; goto _test_eof; 
					_test_eof6: cs = 6; goto _test_eof; 
					_test_eof7: cs = 7; goto _test_eof; 
					_test_eof8: cs = 8; goto _test_eof; 
					_test_eof9: cs = 9; goto _test_eof; 
					_test_eof10: cs = 10; goto _test_eof; 
					_test_eof11: cs = 11; goto _test_eof; 
					_test_eof12: cs = 12; goto _test_eof; 
					_test_eof13: cs = 13; goto _test_eof; 
					_test_eof14: cs = 14; goto _test_eof; 
					_test_eof15: cs = 15; goto _test_eof; 
					_test_eof16: cs = 16; goto _test_eof; 
					_test_eof17: cs = 17; goto _test_eof; 
					_test_eof35: cs = 35; goto _test_eof; 
					_test_eof36: cs = 36; goto _test_eof; 
					_test_eof37: cs = 37; goto _test_eof; 
					_test_eof18: cs = 18; goto _test_eof; 
					_test_eof38: cs = 38; goto _test_eof; 
					_test_eof19: cs = 19; goto _test_eof; 
					_test_eof39: cs = 39; goto _test_eof; 
					_test_eof20: cs = 20; goto _test_eof; 
					_test_eof40: cs = 40; goto _test_eof; 
					_test_eof41: cs = 41; goto _test_eof; 
					_test_eof42: cs = 42; goto _test_eof; 
					_test_eof43: cs = 43; goto _test_eof; 
					_test_eof21: cs = 21; goto _test_eof; 
					_test_eof44: cs = 44; goto _test_eof; 
					_test_eof22: cs = 22; goto _test_eof; 
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
					_test_eof23: cs = 23; goto _test_eof; 
					_test_eof24: cs = 24; goto _test_eof; 
					_test_eof25: cs = 25; goto _test_eof; 
					_test_eof55: cs = 55; goto _test_eof; 
					_test_eof26: cs = 26; goto _test_eof; 
					_test_eof27: cs = 27; goto _test_eof; 
					_test_eof28: cs = 28; goto _test_eof; 
					_test_eof56: cs = 56; goto _test_eof; 
					_test_eof57: cs = 57; goto _test_eof; 
					_test_eof58: cs = 58; goto _test_eof; 
					_test_eof59: cs = 59; goto _test_eof; 
					_test_eof29: cs = 29; goto _test_eof; 
					_test_eof30: cs = 30; goto _test_eof; 
					_test_eof31: cs = 31; goto _test_eof; 
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
					_test_eof75: cs = 75; goto _test_eof; 
					_test_eof76: cs = 76; goto _test_eof; 
					_test_eof77: cs = 77; goto _test_eof; 
					_test_eof78: cs = 78; goto _test_eof; 
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
					_test_eof102: cs = 102; goto _test_eof; 
					_test_eof103: cs = 103; goto _test_eof; 
					_test_eof104: cs = 104; goto _test_eof; 
					_test_eof105: cs = 105; goto _test_eof; 
					_test_eof106: cs = 106; goto _test_eof; 
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
					_test_eof117: cs = 117; goto _test_eof; 
					_test_eof118: cs = 118; goto _test_eof; 
					_test_eof119: cs = 119; goto _test_eof; 
					_test_eof120: cs = 120; goto _test_eof; 
					_test_eof121: cs = 121; goto _test_eof; 
					_test_eof122: cs = 122; goto _test_eof; 
					_test_eof123: cs = 123; goto _test_eof; 
					_test_eof124: cs = 124; goto _test_eof; 
					_test_eof125: cs = 125; goto _test_eof; 
					_test_eof126: cs = 126; goto _test_eof; 
					_test_eof127: cs = 127; goto _test_eof; 
					_test_eof128: cs = 128; goto _test_eof; 
					_test_eof129: cs = 129; goto _test_eof; 
					_test_eof130: cs = 130; goto _test_eof; 
					_test_eof131: cs = 131; goto _test_eof; 
					_test_eof132: cs = 132; goto _test_eof; 
					_test_eof133: cs = 133; goto _test_eof; 
					_test_eof134: cs = 134; goto _test_eof; 
					_test_eof135: cs = 135; goto _test_eof; 
					_test_eof136: cs = 136; goto _test_eof; 
					_test_eof137: cs = 137; goto _test_eof; 
					_test_eof138: cs = 138; goto _test_eof; 
					
					_test_eof: {}
					if ( p == eof ) {
						switch ( cs ) {
							case 32: {
								break;
							}
							case 33: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 8668 "lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 34: {
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
							case 5: {
								break;
							}
							case 6: {
								break;
							}
							case 7: {
								break;
							}
							case 8: {
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
							case 13: {
								break;
							}
							case 14: {
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
							case 0: {
								break;
							}
							case 35: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 8734 "lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 36: {
								break;
							}
							case 37: {
								break;
							}
							case 18: {
								break;
							}
							case 38: {
								break;
							}
							case 19: {
								break;
							}
							case 39: {
								break;
							}
							case 20: {
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
							case 21: {
								break;
							}
							case 44: {
								break;
							}
							case 22: {
								break;
							}
							case 45: {
								break;
							}
							case 46: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 8788 "lex/lemon_data_string_parser.cpp"
								
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
							case 23: {
								break;
							}
							case 24: {
								break;
							}
							case 25: {
								break;
							}
							case 55: {
								break;
							}
							case 26: {
								break;
							}
							case 27: {
								break;
							}
							case 28: {
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
							case 29: {
								break;
							}
							case 30: {
								break;
							}
							case 31: {
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
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 8968 "lex/lemon_data_string_parser.cpp"
								
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
							case 117: {
								break;
							}
							case 118: {
								break;
							}
							case 119: {
								break;
							}
							case 120: {
								break;
							}
							case 121: {
								break;
							}
							case 122: {
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
							case 126: {
								break;
							}
							case 127: {
								break;
							}
							case 128: {
								break;
							}
							case 129: {
								break;
							}
							case 130: {
								break;
							}
							case 131: {
								break;
							}
							case 132: {
								break;
							}
							case 133: {
								break;
							}
							case 134: {
								break;
							}
							case 135: {
								break;
							}
							case 136: {
								break;
							}
							case 137: {
								break;
							}
							case 138: {
								break;
							}
						}
						switch ( cs ) {
						}
						switch ( cs ) {
							case 32:
							goto _st32;case 33:
							goto _st33;case 34:
							goto _ctr49;case 1:
							goto _ctr1;case 2:
							goto _ctr1;case 3:
							goto _ctr1;case 4:
							goto _ctr1;case 5:
							goto _ctr1;case 6:
							goto _ctr1;case 7:
							goto _ctr1;case 8:
							goto _ctr1;case 9:
							goto _ctr1;case 10:
							goto _ctr1;case 11:
							goto _ctr1;case 12:
							goto _ctr1;case 13:
							goto _ctr1;case 14:
							goto _ctr1;case 15:
							goto _ctr1;case 16:
							goto _ctr1;case 17:
							goto _st17;case 0:
							goto _st0;case 35:
							goto _st35;case 36:
							goto _ctr59;case 37:
							goto _ctr60;case 18:
							goto _st18;case 38:
							goto _ctr61;case 19:
							goto _ctr26;case 39:
							goto _ctr64;case 20:
							goto _ctr26;case 40:
							goto _ctr65;case 41:
							goto _ctr65;case 42:
							goto _ctr61;case 43:
							goto _ctr61;case 21:
							goto _ctr26;case 44:
							goto _ctr68;case 22:
							goto _ctr26;case 45:
							goto _ctr69;case 46:
							goto _st46;case 47:
							goto _ctr87;case 48:
							goto _ctr87;case 49:
							goto _ctr87;case 50:
							goto _ctr87;case 51:
							goto _ctr87;case 52:
							goto _ctr34;case 53:
							goto _ctr87;case 54:
							goto _ctr95;case 23:
							goto _ctr32;case 24:
							goto _ctr34;case 25:
							goto _ctr34;case 55:
							goto _ctr98;case 26:
							goto _ctr32;case 27:
							goto _ctr32;case 28:
							goto _ctr32;case 56:
							goto _ctr99;case 57:
							goto _ctr87;case 58:
							goto _ctr87;case 59:
							goto _ctr87;case 29:
							goto _ctr39;case 30:
							goto _ctr39;case 31:
							goto _ctr39;case 60:
							goto _ctr87;case 61:
							goto _ctr87;case 62:
							goto _ctr87;case 63:
							goto _ctr87;case 64:
							goto _ctr87;case 65:
							goto _ctr87;case 66:
							goto _ctr87;case 67:
							goto _ctr87;case 68:
							goto _ctr87;case 69:
							goto _ctr87;case 70:
							goto _ctr87;case 71:
							goto _ctr87;case 72:
							goto _ctr87;case 73:
							goto _ctr118;case 74:
							goto _ctr87;case 75:
							goto _ctr121;case 76:
							goto _ctr87;case 77:
							goto _ctr129;case 78:
							goto _ctr87;case 79:
							goto _ctr136;case 80:
							goto _ctr136;case 81:
							goto _ctr121;case 82:
							goto _ctr121;case 83:
							goto _ctr87;case 84:
							goto _ctr87;case 85:
							goto _ctr146;case 86:
							goto _ctr87;case 87:
							goto _ctr153;case 88:
							goto _ctr121;case 89:
							goto _ctr87;case 90:
							goto _ctr162;case 91:
							goto _ctr87;case 92:
							goto _ctr164;case 93:
							goto _ctr87;case 94:
							goto _ctr166;case 95:
							goto _st95;case 96:
							goto _ctr181;case 97:
							goto _ctr183;case 98:
							goto _ctr183;case 99:
							goto _ctr183;case 100:
							goto _ctr183;case 101:
							goto _ctr183;case 102:
							goto _ctr189;case 103:
							goto _ctr183;case 104:
							goto _ctr183;case 105:
							goto _ctr183;case 106:
							goto _ctr183;case 107:
							goto _ctr183;case 108:
							goto _ctr183;case 109:
							goto _ctr183;case 110:
							goto _ctr183;case 111:
							goto _ctr183;case 112:
							goto _ctr183;case 113:
							goto _ctr183;case 114:
							goto _ctr183;case 115:
							goto _ctr183;case 116:
							goto _ctr183;case 117:
							goto _ctr183;case 118:
							goto _ctr183;case 119:
							goto _ctr209;case 120:
							goto _ctr183;case 121:
							goto _ctr212;case 122:
							goto _ctr183;case 123:
							goto _ctr218;case 124:
							goto _ctr183;case 125:
							goto _ctr223;case 126:
							goto _ctr223;case 127:
							goto _ctr212;case 128:
							goto _ctr212;case 129:
							goto _ctr183;case 130:
							goto _ctr229;case 131:
							goto _ctr183;case 132:
							goto _ctr233;case 133:
							goto _ctr183;case 134:
							goto _ctr238;case 135:
							goto _ctr183;case 136:
							goto _ctr240;case 137:
							goto _ctr183;case 138:
							goto _ctr242;	}
					}
					
					if ( cs >= 32 )
						goto _out; _pop: {}
					_out: {}
				}
				
#line 305 "lex/lemon_data_string_parser.rl"
				
				
			} catch(std::exception& e) {
				// can be thrown on static buffer overflows etc.
				fmt::format_to(std::back_inserter(error_), "[data] parse exception: '{}'", e.what());
				return false;
			}
			
			if (cs < 
#line 9262 "lex/lemon_data_string_parser.cpp"
			32
#line 313 "lex/lemon_data_string_parser.rl"
			) {
				std::string tok_name;
				if (ts && te)
					tok_name.append(ts, te);
				
				fmt::format_to(std::back_inserter(error_), "[data] lexer failed on '{}', unknown token: '{}'", data, tok_name);
				return false;
			} else {
				if (parse_ok_)
					pushToken(0);
				
				return parse_ok_;
			}
		}
		
		void LemonDataStringParser::reset()
		{
			parse_ok_ = true;
			if (res_)
				res_->clear();
			error_.clear();
		}
		
		void LemonDataStringParser::onStackOverflow()
		{
			parse_ok_ = false;
			fmt::format_to(std::back_inserter(error_), "[data] parse: stack overflow");
		}
		
		void LemonDataStringParser::onParseFailure()
		{
			parse_ok_ = false;
			fmt::format_to(std::back_inserter(error_), "[data] parse failed");
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
