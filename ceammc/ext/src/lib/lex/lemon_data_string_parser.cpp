#line 1 "lex/lemon_data_string_parser.rl"
# include "lemon_data_string_parser.h"
# include "lemon_data_string.h"
# include "parser_numeric.h"

# include <cstdint>
# include <cstdlib>
# include <boost/static_string.hpp>

# include "lemon_data_parser_impl.h"


#line 235 "lex/lemon_data_string_parser.rl"


# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 23 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
		static const int lemon_data_string_lexer_start = 49;
		static const int lemon_data_string_lexer_first_final = 49;
		static const int lemon_data_string_lexer_error = 0;
		
		static const int lemon_data_string_lexer_en_scan_sqstring = 50;
		static const int lemon_data_string_lexer_en_scan_dqstring = 52;
		static const int lemon_data_string_lexer_en_scan_matrix = 54;
		static const int lemon_data_string_lexer_en_scan_dict = 65;
		static const int lemon_data_string_lexer_en_scan_token = 109;
		static const int lemon_data_string_lexer_en_main = 49;
		
		
#line 242 "lex/lemon_data_string_parser.rl"
		
		
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
				
				
#line 141 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					cs = (int)lemon_data_string_lexer_start;
					top = 0;
					ts = 0;
					te = 0;
					act = 0;
				}
				
#line 345 "lex/lemon_data_string_parser.rl"
				
				
#line 153 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					if ( p == pe )
						goto _test_eof;
					goto _resume;
					
					_again: {}
					switch ( cs ) {
						case 49: goto _st49;
						case 50: goto _st50;
						case 51: goto _st51;
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
						case 52: goto _st52;
						case 53: goto _st53;
						case 18: goto _st18;
						case 19: goto _st19;
						case 20: goto _st20;
						case 21: goto _st21;
						case 22: goto _st22;
						case 23: goto _st23;
						case 24: goto _st24;
						case 25: goto _st25;
						case 26: goto _st26;
						case 27: goto _st27;
						case 28: goto _st28;
						case 29: goto _st29;
						case 30: goto _st30;
						case 31: goto _st31;
						case 32: goto _st32;
						case 33: goto _st33;
						case 34: goto _st34;
						case 54: goto _st54;
						case 55: goto _st55;
						case 56: goto _st56;
						case 35: goto _st35;
						case 57: goto _st57;
						case 36: goto _st36;
						case 58: goto _st58;
						case 37: goto _st37;
						case 59: goto _st59;
						case 60: goto _st60;
						case 61: goto _st61;
						case 62: goto _st62;
						case 38: goto _st38;
						case 63: goto _st63;
						case 39: goto _st39;
						case 64: goto _st64;
						case 65: goto _st65;
						case 66: goto _st66;
						case 67: goto _st67;
						case 68: goto _st68;
						case 69: goto _st69;
						case 70: goto _st70;
						case 40: goto _st40;
						case 41: goto _st41;
						case 42: goto _st42;
						case 71: goto _st71;
						case 43: goto _st43;
						case 44: goto _st44;
						case 45: goto _st45;
						case 72: goto _st72;
						case 73: goto _st73;
						case 74: goto _st74;
						case 75: goto _st75;
						case 46: goto _st46;
						case 47: goto _st47;
						case 48: goto _st48;
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
						case 139: goto _st139;
						case 140: goto _st140;
						case 141: goto _st141;
						case 142: goto _st142;
						case 143: goto _st143;
						case 144: goto _st144;
						case 145: goto _st145;
						case 146: goto _st146;
						case 147: goto _st147;
						case 148: goto _st148;
						case 149: goto _st149;
						case 150: goto _st150;
						case 151: goto _st151;
						case 152: goto _st152;
					}
					
					_resume: {}
					switch ( cs ) {
						case 49:
						goto st_case_49;
						case 50:
						goto st_case_50;
						case 51:
						goto st_case_51;
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
						case 52:
						goto st_case_52;
						case 53:
						goto st_case_53;
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
						case 23:
						goto st_case_23;
						case 24:
						goto st_case_24;
						case 25:
						goto st_case_25;
						case 26:
						goto st_case_26;
						case 27:
						goto st_case_27;
						case 28:
						goto st_case_28;
						case 29:
						goto st_case_29;
						case 30:
						goto st_case_30;
						case 31:
						goto st_case_31;
						case 32:
						goto st_case_32;
						case 33:
						goto st_case_33;
						case 34:
						goto st_case_34;
						case 54:
						goto st_case_54;
						case 55:
						goto st_case_55;
						case 56:
						goto st_case_56;
						case 35:
						goto st_case_35;
						case 57:
						goto st_case_57;
						case 36:
						goto st_case_36;
						case 58:
						goto st_case_58;
						case 37:
						goto st_case_37;
						case 59:
						goto st_case_59;
						case 60:
						goto st_case_60;
						case 61:
						goto st_case_61;
						case 62:
						goto st_case_62;
						case 38:
						goto st_case_38;
						case 63:
						goto st_case_63;
						case 39:
						goto st_case_39;
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
						case 40:
						goto st_case_40;
						case 41:
						goto st_case_41;
						case 42:
						goto st_case_42;
						case 71:
						goto st_case_71;
						case 43:
						goto st_case_43;
						case 44:
						goto st_case_44;
						case 45:
						goto st_case_45;
						case 72:
						goto st_case_72;
						case 73:
						goto st_case_73;
						case 74:
						goto st_case_74;
						case 75:
						goto st_case_75;
						case 46:
						goto st_case_46;
						case 47:
						goto st_case_47;
						case 48:
						goto st_case_48;
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
						case 139:
						goto st_case_139;
						case 140:
						goto st_case_140;
						case 141:
						goto st_case_141;
						case 142:
						goto st_case_142;
						case 143:
						goto st_case_143;
						case 144:
						goto st_case_144;
						case 145:
						goto st_case_145;
						case 146:
						goto st_case_146;
						case 147:
						goto st_case_147;
						case 148:
						goto st_case_148;
						case 149:
						goto st_case_149;
						case 150:
						goto st_case_150;
						case 151:
						goto st_case_151;
						case 152:
						goto st_case_152;
					}
					goto st_out;
					_ctr68:
					{
#line 234 "lex/lemon_data_string_parser.rl"
						{p = p - 1; } {stack[top] = 49; top+= 1; goto _st109;}}
					
#line 631 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st49;
					_st49:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 645 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof49;
					st_case_49:
					{
						goto _ctr68;
					}
					_ctr1:
					{
#line 143 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 143 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 663 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr2:
					{
#line 59 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 59 "lex/lemon_data_string_parser.rl"
								
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
					
#line 698 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr19:
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 145 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 709 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr20:
					{
#line 144 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 144 "lex/lemon_data_string_parser.rl"
								ragel_string += '\''; }
						}}
					
#line 720 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr21:
					{
#line 146 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 146 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 731 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr22:
					{
#line 147 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 147 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 742 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr23:
					{
#line 148 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 148 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 753 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr70:
					{
#line 143 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 143 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 764 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr72:
					{
#line 139 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 139 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 775 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr73:
					{
#line 143 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 143 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 786 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_st50:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 800 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof50;
					st_case_50:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 810 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr71;
						}
						case 39: {
							goto _ctr72;
						}
						case 96: {
							goto _st17;
						}
					}
					{
						goto _ctr70;
					}
					_ctr71:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 831 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st51;
					_st51:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof51;
					st_case_51:
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
						goto _ctr73;
					}
					_st1:
					if ( p == eof ) {
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
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
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof17;
					st_case_17:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr19;
						}
						case 39: {
							goto _ctr20;
						}
						case 46: {
							goto _ctr21;
						}
						case 58: {
							goto _ctr22;
						}
						case 96: {
							goto _ctr23;
						}
					}
					{
						goto _st0;
					}
					st_case_0:
					_st0:
					cs = 0;
					goto _pop;
					_ctr24:
					{
#line 155 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 155 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 1354 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr25:
					{
#line 59 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 59 "lex/lemon_data_string_parser.rl"
								
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
					
#line 1389 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr42:
					{
#line 157 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 157 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 1400 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr43:
					{
#line 156 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 156 "lex/lemon_data_string_parser.rl"
								ragel_string += '"'; }
						}}
					
#line 1411 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr44:
					{
#line 158 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 158 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 1422 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr45:
					{
#line 159 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 159 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 1433 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr46:
					{
#line 160 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 160 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 1444 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr76:
					{
#line 155 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 155 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 1455 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr77:
					{
#line 139 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 139 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 1466 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr79:
					{
#line 155 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 155 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 1477 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_st52:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 1491 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof52;
					st_case_52:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 1501 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 34: {
							goto _ctr77;
						}
						case 37: {
							goto _ctr78;
						}
						case 96: {
							goto _st34;
						}
					}
					{
						goto _ctr76;
					}
					_ctr78:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1522 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st53;
					_st53:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof53;
					st_case_53:
					if ( ( (*( p))) == 95 ) {
						goto _st18;
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st18;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st18;
					}
					{
						goto _ctr79;
					}
					_st18:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof18;
					st_case_18:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st19;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st19;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st19;
					}
					{
						goto _ctr24;
					}
					_st19:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof19;
					st_case_19:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st20;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st20;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st20;
					}
					{
						goto _ctr24;
					}
					_st20:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof20;
					st_case_20:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st21;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st21;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st21;
					}
					{
						goto _ctr24;
					}
					_st21:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof21;
					st_case_21:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st22;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st22;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st22;
					}
					{
						goto _ctr24;
					}
					_st22:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof22;
					st_case_22:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st23;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st23;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st23;
					}
					{
						goto _ctr24;
					}
					_st23:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof23;
					st_case_23:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st24;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st24;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st24;
					}
					{
						goto _ctr24;
					}
					_st24:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof24;
					st_case_24:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st25;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st25;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st25;
					}
					{
						goto _ctr24;
					}
					_st25:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof25;
					st_case_25:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st26;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st26;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st26;
					}
					{
						goto _ctr24;
					}
					_st26:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof26;
					st_case_26:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st27;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st27;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st27;
					}
					{
						goto _ctr24;
					}
					_st27:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof27;
					st_case_27:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st28;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st28;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st28;
					}
					{
						goto _ctr24;
					}
					_st28:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof28;
					st_case_28:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st29;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st29;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st29;
					}
					{
						goto _ctr24;
					}
					_st29:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof29;
					st_case_29:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st30;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st30;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st30;
					}
					{
						goto _ctr24;
					}
					_st30:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof30;
					st_case_30:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st31;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st31;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st31;
					}
					{
						goto _ctr24;
					}
					_st31:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof31;
					st_case_31:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st32;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st32;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st32;
					}
					{
						goto _ctr24;
					}
					_st32:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof32;
					st_case_32:
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr25;
						}
						case 95: {
							goto _st33;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 65 <= ( (*( p))) && ( (*( p))) <= 90 ) {
							goto _st33;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st33;
					}
					{
						goto _ctr24;
					}
					_st33:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof33;
					st_case_33:
					if ( ( (*( p))) == 37 ) {
						goto _ctr25;
					}
					{
						goto _ctr24;
					}
					_st34:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof34;
					st_case_34:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr42;
						}
						case 34: {
							goto _ctr43;
						}
						case 46: {
							goto _ctr44;
						}
						case 58: {
							goto _ctr45;
						}
						case 96: {
							goto _ctr46;
						}
					}
					{
						goto _st0;
					}
					_ctr50:
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2046 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr84:
					{
#line 21 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 21 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 2057 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr88:
					{
#line 177 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 177 "lex/lemon_data_string_parser.rl"
								pushToken(TK_MATRIX_CLOSE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 2068 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr89:
					{
#line 176 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;}}
					
#line 2076 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr90:
					{
#line 20 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 20 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 2087 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr91:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2099 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2113 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr94:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2126 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2140 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr95:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2152 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2166 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr98:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2177 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2191 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr99:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2202 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2216 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_st54:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 2230 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof54;
					st_case_54:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 2240 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _st55;
						}
						case 40: {
							goto _st56;
						}
						case 41: {
							goto _ctr84;
						}
						case 43: {
							goto _ctr85;
						}
						case 45: {
							goto _ctr85;
						}
						case 48: {
							goto _ctr86;
						}
						case 93: {
							goto _ctr88;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _st55;
					}
					{
						goto _st0;
					}
					_st55:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof55;
					st_case_55:
					switch( ( (*( p))) ) {
						case 32: {
							goto _st55;
						}
						case 41: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr88;
						}
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st55;
					}
					{
						goto _ctr89;
					}
					_st56:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof56;
					st_case_56:
					if ( ( (*( p))) == 32 ) {
						goto _st56;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st56;
					}
					{
						goto _ctr90;
					}
					_ctr85:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 2328 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 2334 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_st35:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof35;
					st_case_35:
					if ( ( (*( p))) == 48 ) {
						goto _ctr48;
					}
					if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr49;
					}
					{
						goto _st0;
					}
					_ctr48:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2362 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2368 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2374 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st57;
					_st57:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof57;
					st_case_57:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr92;
						}
						case 47: {
							goto _st37;
						}
					}
					{
						goto _ctr91;
					}
					_ctr92:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2408 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 2417 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st36;
					_st36:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof36;
					st_case_36:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr51;
					}
					{
						goto _ctr50;
					}
					_ctr51:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 2445 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st58;
					_st58:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof58;
					st_case_58:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr51;
					}
					{
						goto _ctr94;
					}
					_st37:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof37;
					st_case_37:
					if ( ( (*( p))) == 48 ) {
						goto _ctr52;
					}
					if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr53;
					}
					{
						goto _ctr50;
					}
					_ctr52:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 2490 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st59;
					_st59:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof59;
					st_case_59:
					{
						goto _ctr95;
					}
					_ctr53:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 2512 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st60;
					_st60:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof60;
					st_case_60:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr53;
					}
					{
						goto _ctr95;
					}
					_ctr49:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2537 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2543 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2549 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st61;
					_ctr87:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2557 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 2563 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2569 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2575 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st61;
					_st61:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof61;
					st_case_61:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr92;
						}
						case 47: {
							goto _st37;
						}
					}
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr49;
					}
					{
						goto _ctr91;
					}
					_ctr86:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2608 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 2614 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2620 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2626 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st62;
					_st62:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof62;
					st_case_62:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr92;
						}
						case 47: {
							goto _st37;
						}
						case 98: {
							goto _st38;
						}
						case 120: {
							goto _st39;
						}
					}
					{
						goto _ctr91;
					}
					_st38:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof38;
					st_case_38:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr54;
					}
					{
						goto _ctr50;
					}
					_ctr54:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 2679 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st63;
					_st63:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof63;
					st_case_63:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr54;
					}
					{
						goto _ctr98;
					}
					_st39:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof39;
					st_case_39:
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr55;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr55;
						}
					} else {
						goto _ctr55;
					}
					{
						goto _ctr50;
					}
					_ctr55:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 2729 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st64;
					_st64:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof64;
					st_case_64:
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr55;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr55;
						}
					} else {
						goto _ctr55;
					}
					{
						goto _ctr99;
					}
					_ctr56:
					{
#line 204 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 204 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 2766 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr58:
					{
#line 1 "NONE"
						{switch( act ) {
								case 22:  {
									p = ((te))-1;
									{
#line 16 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 23:  {
									p = ((te))-1;
									{
#line 17 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 24:  {
									p = ((te))-1;
									{
#line 18 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 28:  {
									p = ((te))-1;
									{
#line 28 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st52;}}
									break; 
								}
								case 29:  {
									p = ((te))-1;
									{
#line 29 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st50;}}
									break; 
								}
								case 38:  {
									p = ((te))-1;
									{
#line 204 "lex/lemon_data_string_parser.rl"
										pushToken(TK_SPACE); }
									break; 
								}
								case 39:  {
									p = ((te))-1;
									{
#line 88 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 2825 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr63:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 88 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 2837 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr66:
					{
#line 165 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 165 "lex/lemon_data_string_parser.rl"
								
								pushToken(TK_MATRIX);
								pushFloat(ragel_mtx_rows);
								pushFloat(ragel_mtx_cols);
								{stack[top] = 65; top+= 1; goto _st54;}}
						}}
					
#line 2852 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr107:
					{
#line 21 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 21 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 2863 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr115:
					{
#line 208 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 208 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_CLOSE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 2874 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr117:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 88 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 2885 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr122:
					{
#line 204 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 204 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 2896 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr125:
					{
#line 50 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 50 "lex/lemon_data_string_parser.rl"
								
								// skip starting whitespaces
								auto ts0 = ts;
								while(*ts0 == ' ') ++ts0;
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DICT_KEY, ts0, te0);
							}
						}}
					
#line 2915 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr126:
					{
#line 28 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 28 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st52;}}
						}}
					
#line 2926 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr143:
					{
#line 29 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 29 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st50;}}
						}}
					
#line 2937 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr144:
					{
#line 20 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 20 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 2948 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr147:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2960 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2974 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr154:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2987 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 3001 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr160:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3013 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 3027 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr169:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3038 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 3052 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr175:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3063 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 3077 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr182:
					{
#line 44 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 44 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 65; top+= 1; goto _st65;}}
						}}
					
#line 3091 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr183:
					{
#line 37 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 37 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DATA_NAME, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 3108 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr184:
					{
#line 86 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 86 "lex/lemon_data_string_parser.rl"
								pushToken(TK_STRING); ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st52;}}
						}}
					
#line 3119 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr185:
					{
#line 85 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 85 "lex/lemon_data_string_parser.rl"
								pushToken(TK_STRING); ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st50;}}
						}}
					
#line 3130 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr186:
					{
#line 49 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 49 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); {stack[top] = 65; top+= 1; goto _st65;}}
						}}
					
#line 3141 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr188:
					{
#line 30 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 30 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 3158 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_st65:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 3172 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof65;
					st_case_65:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 3182 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr102;
						}
						case 34: {
							goto _ctr103;
						}
						case 35: {
							goto _st73;
						}
						case 39: {
							goto _ctr105;
						}
						case 40: {
							goto _st87;
						}
						case 41: {
							goto _ctr107;
						}
						case 43: {
							goto _ctr108;
						}
						case 45: {
							goto _ctr108;
						}
						case 48: {
							goto _ctr109;
						}
						case 83: {
							goto _st105;
						}
						case 91: {
							goto _st106;
						}
						case 93: {
							goto _ctr115;
						}
						case 95: {
							goto _st97;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _ctr110;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr102;
						}
					} else if ( ( (*( p))) > 64 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st107;
							}
						} else {
							goto _st103;
						}
					} else {
						goto _st97;
					}
					{
						goto _st66;
					}
					_ctr148:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3256 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_ctr155:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3269 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_ctr161:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3281 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_ctr170:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3292 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_ctr176:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3303 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_st66:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_ctr149:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3368 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_ctr156:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3381 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_ctr162:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3393 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_ctr171:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3404 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_ctr177:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3415 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_st67:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 48: {
							goto _ctr119;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr120;
					}
					{
						goto _st66;
					}
					_ctr119:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3479 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 39;}}
					
#line 3485 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr121:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3493 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 109 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 3499 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 39;}}
					
#line 3505 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr136:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3513 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 17 "lex/lemon_data_string_parser.rl"
						{act = 23;}}
					
#line 3519 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr139:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3527 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 18 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 3533 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr142:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3541 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 16 "lex/lemon_data_string_parser.rl"
						{act = 22;}}
					
#line 3547 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr152:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3555 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3565 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 39;}}
					
#line 3571 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr157:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3579 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3590 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 39;}}
					
#line 3596 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr163:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3604 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3614 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 39;}}
					
#line 3620 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_st68:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr58;
						}
						case 34: {
							goto _ctr58;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr58;
						}
						case 93: {
							goto _ctr58;
						}
						case 95: {
							goto _ctr119;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr58;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_ctr120:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 3684 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st69;
					_st69:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr117;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr121;
					}
					{
						goto _st66;
					}
					_ctr102:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3748 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 204 "lex/lemon_data_string_parser.rl"
						{act = 38;}}
					
#line 3754 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st70;
					_st70:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr102;
						}
						case 34: {
							goto _st40;
						}
						case 39: {
							goto _st43;
						}
						case 41: {
							goto _ctr107;
						}
						case 93: {
							goto _ctr115;
						}
						case 95: {
							goto _st45;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr102;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st45;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _st45;
						}
					} else {
						goto _st45;
					}
					{
						goto _ctr122;
					}
					_st40:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof40;
					st_case_40:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st41;
						}
						case 95: {
							goto _st41;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st41;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st41;
					}
					{
						goto _ctr56;
					}
					_st41:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof41;
					st_case_41:
					switch( ( (*( p))) ) {
						case 34: {
							goto _st42;
						}
						case 63: {
							goto _st41;
						}
						case 95: {
							goto _st41;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st41;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st41;
					}
					{
						goto _ctr58;
					}
					_st42:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof42;
					st_case_42:
					if ( ( (*( p))) == 58 ) {
						goto _st71;
					}
					{
						goto _ctr58;
					}
					_ctr165:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3893 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st71;
					_ctr173:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3904 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st71;
					_ctr178:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3915 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st71;
					_st71:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof71;
					st_case_71:
					if ( ( (*( p))) == 32 ) {
						goto _st71;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st71;
					}
					{
						goto _ctr125;
					}
					_st43:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof43;
					st_case_43:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st44;
						}
						case 95: {
							goto _st44;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st44;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st44;
					}
					{
						goto _ctr56;
					}
					_st44:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof44;
					st_case_44:
					switch( ( (*( p))) ) {
						case 39: {
							goto _st42;
						}
						case 63: {
							goto _st44;
						}
						case 95: {
							goto _st44;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st44;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st44;
					}
					{
						goto _ctr58;
					}
					_st45:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof45;
					st_case_45:
					switch( ( (*( p))) ) {
						case 58: {
							goto _st71;
						}
						case 95: {
							goto _st45;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _st45;
						}
					} else if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st45;
						}
					} else {
						goto _st45;
					}
					{
						goto _ctr56;
					}
					_ctr103:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4037 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 28 "lex/lemon_data_string_parser.rl"
						{act = 28;}}
					
#line 4043 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_st72:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof72;
					st_case_72:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st41;
						}
						case 95: {
							goto _st41;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st41;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st41;
					}
					{
						goto _ctr126;
					}
					_st73:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof73;
					st_case_73:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 48: {
							goto _ctr119;
						}
						case 91: {
							goto _ctr66;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
						case 102: {
							goto _st76;
						}
						case 110: {
							goto _st80;
						}
						case 116: {
							goto _st83;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr127;
					}
					{
						goto _st66;
					}
					_ctr127:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4145 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 4151 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st74;
					_st74:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _st46;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr131;
					}
					{
						goto _st66;
					}
					_ctr131:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4215 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 109 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 4221 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st75;
					_st75:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _st46;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_st46:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof46;
					st_case_46:
					if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr64;
					}
					{
						goto _ctr63;
					}
					_ctr64:
					{
#line 108 "lex/lemon_data_string_parser.rl"
						ragel_mtx_cols = (( (*( p)))) - '0'; }
					
#line 4302 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_st47:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof47;
					st_case_47:
					if ( ( (*( p))) == 91 ) {
						goto _ctr66;
					}
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr65;
					}
					{
						goto _ctr63;
					}
					_ctr65:
					{
#line 108 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_cols *= 10) += ((( (*( p)))) - '0'); }
					
#line 4330 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_st48:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof48;
					st_case_48:
					if ( ( (*( p))) == 91 ) {
						goto _ctr66;
					}
					{
						goto _ctr63;
					}
					_st76:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr117;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
						case 97: {
							goto _st77;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_st77:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr117;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
						case 108: {
							goto _st78;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_st78:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr117;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
						case 115: {
							goto _st79;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_st79:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr117;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
						case 101: {
							goto _ctr136;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_st80:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr117;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
						case 117: {
							goto _st81;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_st81:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr117;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
						case 108: {
							goto _st82;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_st82:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr117;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
						case 108: {
							goto _ctr139;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_st83:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr117;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
						case 114: {
							goto _st84;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_st84:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr117;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
						case 117: {
							goto _st85;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_st85:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr117;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
						case 101: {
							goto _ctr142;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_ctr105:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4945 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/lemon_data_string_parser.rl"
						{act = 29;}}
					
#line 4951 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st86;
					_st86:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof86;
					st_case_86:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st44;
						}
						case 95: {
							goto _st44;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st44;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st44;
					}
					{
						goto _ctr143;
					}
					_st87:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof87;
					st_case_87:
					if ( ( (*( p))) == 32 ) {
						goto _st87;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st87;
					}
					{
						goto _ctr144;
					}
					_ctr108:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 5008 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 5014 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_st88:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 48: {
							goto _ctr145;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr146;
					}
					{
						goto _st66;
					}
					_ctr145:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5078 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5084 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_st89:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr147;
						}
						case 34: {
							goto _ctr147;
						}
						case 35: {
							goto _ctr149;
						}
						case 46: {
							goto _ctr150;
						}
						case 47: {
							goto _st92;
						}
						case 58: {
							goto _ctr147;
						}
						case 91: {
							goto _ctr147;
						}
						case 93: {
							goto _ctr147;
						}
						case 95: {
							goto _ctr152;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr147;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr147;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr152;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr152;
						}
					} else {
						goto _ctr152;
					}
					{
						goto _ctr148;
					}
					_ctr150:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5158 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 5167 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st90;
					_st90:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof90;
					st_case_90:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr153;
					}
					{
						goto _st66;
					}
					_ctr153:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 5231 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st91;
					_st91:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr154;
						}
						case 34: {
							goto _ctr154;
						}
						case 35: {
							goto _ctr156;
						}
						case 58: {
							goto _ctr154;
						}
						case 91: {
							goto _ctr154;
						}
						case 93: {
							goto _ctr154;
						}
						case 95: {
							goto _ctr157;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr154;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr154;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr157;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr157;
						}
					} else {
						goto _ctr153;
					}
					{
						goto _ctr155;
					}
					_st92:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof92;
					st_case_92:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 48: {
							goto _ctr158;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _ctr119;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr119;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr119;
						}
					} else {
						goto _ctr159;
					}
					{
						goto _st66;
					}
					_ctr158:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 5351 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st93;
					_st93:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr160;
						}
						case 34: {
							goto _ctr160;
						}
						case 35: {
							goto _ctr162;
						}
						case 58: {
							goto _ctr160;
						}
						case 91: {
							goto _ctr160;
						}
						case 93: {
							goto _ctr160;
						}
						case 95: {
							goto _ctr163;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr160;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr160;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr163;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr163;
						}
					} else {
						goto _ctr163;
					}
					{
						goto _ctr161;
					}
					_ctr159:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 5415 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st94;
					_st94:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof94;
					st_case_94:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr160;
						}
						case 34: {
							goto _ctr160;
						}
						case 35: {
							goto _ctr162;
						}
						case 58: {
							goto _ctr160;
						}
						case 91: {
							goto _ctr160;
						}
						case 93: {
							goto _ctr160;
						}
						case 95: {
							goto _ctr163;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr160;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr160;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr163;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr163;
						}
					} else {
						goto _ctr159;
					}
					{
						goto _ctr161;
					}
					_ctr146:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5479 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5485 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_st95:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof95;
					st_case_95:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr147;
						}
						case 34: {
							goto _ctr147;
						}
						case 35: {
							goto _ctr149;
						}
						case 46: {
							goto _ctr150;
						}
						case 47: {
							goto _st92;
						}
						case 58: {
							goto _ctr147;
						}
						case 91: {
							goto _ctr147;
						}
						case 93: {
							goto _ctr147;
						}
						case 95: {
							goto _ctr152;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr147;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr147;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr152;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr152;
						}
					} else {
						goto _ctr146;
					}
					{
						goto _ctr148;
					}
					_ctr109:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 5555 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5561 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5567 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_st96:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr147;
						}
						case 34: {
							goto _ctr147;
						}
						case 35: {
							goto _ctr149;
						}
						case 46: {
							goto _ctr150;
						}
						case 47: {
							goto _st92;
						}
						case 58: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr147;
						}
						case 93: {
							goto _ctr147;
						}
						case 95: {
							goto _ctr164;
						}
						case 98: {
							goto _st98;
						}
						case 120: {
							goto _st100;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr147;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr147;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr164;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr164;
						}
					} else {
						goto _ctr164;
					}
					{
						goto _ctr148;
					}
					_ctr164:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5647 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st97;
					_ctr172:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5658 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st97;
					_ctr179:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5669 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st97;
					_st97:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _st71;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _st97;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st97;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _st97;
						}
					} else {
						goto _st97;
					}
					{
						goto _st66;
					}
					_st98:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _st71;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _st97;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 49 ) {
						if ( ( (*( p))) < 63 ) {
							if ( ( (*( p))) <= 57 ) {
								goto _st97;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st97;
							}
						} else {
							goto _st97;
						}
					} else {
						goto _ctr168;
					}
					{
						goto _st66;
					}
					_ctr168:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 5793 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st99;
					_st99:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 35: {
							goto _ctr171;
						}
						case 58: {
							goto _ctr173;
						}
						case 91: {
							goto _ctr169;
						}
						case 93: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr169;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 49 ) {
						if ( ( (*( p))) < 63 ) {
							if ( ( (*( p))) <= 57 ) {
								goto _ctr172;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr172;
							}
						} else {
							goto _ctr172;
						}
					} else {
						goto _ctr168;
					}
					{
						goto _ctr170;
					}
					_st100:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _st71;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _st97;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) < 39 ) {
							if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) > 41 ) {
							if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _ctr174;
							}
						} else {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 64 ) {
						if ( ( (*( p))) < 71 ) {
							{
								goto _ctr174;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( ( (*( p))) > 102 ) {
								if ( ( (*( p))) <= 122 ) {
									goto _st97;
								}
							} else if ( ( (*( p))) >= 97 ) {
								goto _ctr174;
							}
						} else {
							goto _st97;
						}
					} else {
						goto _st97;
					}
					{
						goto _st66;
					}
					_ctr174:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 5929 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st101;
					_st101:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr175;
						}
						case 34: {
							goto _ctr175;
						}
						case 35: {
							goto _ctr177;
						}
						case 58: {
							goto _ctr178;
						}
						case 91: {
							goto _ctr175;
						}
						case 93: {
							goto _ctr175;
						}
						case 95: {
							goto _ctr179;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) < 39 ) {
							if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
								goto _ctr175;
							}
						} else if ( ( (*( p))) > 41 ) {
							if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _ctr174;
							}
						} else {
							goto _ctr175;
						}
					} else if ( ( (*( p))) > 64 ) {
						if ( ( (*( p))) < 71 ) {
							{
								goto _ctr174;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( ( (*( p))) > 102 ) {
								if ( ( (*( p))) <= 122 ) {
									goto _ctr179;
								}
							} else if ( ( (*( p))) >= 97 ) {
								goto _ctr174;
							}
						} else {
							goto _ctr179;
						}
					} else {
						goto _ctr179;
					}
					{
						goto _ctr176;
					}
					_ctr110:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 6005 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 6011 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 6017 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st102;
					_ctr180:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 6025 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 6031 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st102;
					_st102:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr147;
						}
						case 34: {
							goto _ctr147;
						}
						case 35: {
							goto _ctr149;
						}
						case 46: {
							goto _ctr150;
						}
						case 47: {
							goto _st92;
						}
						case 58: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr147;
						}
						case 93: {
							goto _ctr147;
						}
						case 95: {
							goto _ctr164;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr147;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr147;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr164;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr164;
						}
					} else {
						goto _ctr180;
					}
					{
						goto _ctr148;
					}
					_st103:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 40: {
							goto _st104;
						}
						case 58: {
							goto _st71;
						}
						case 91: {
							goto _ctr182;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _st97;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) < 65 ) {
							if ( 63 <= ( (*( p))) ) {
								goto _st97;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st103;
							}
						} else {
							goto _st103;
						}
					} else {
						goto _st97;
					}
					{
						goto _st66;
					}
					_st104:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof104;
					st_case_104:
					if ( ( (*( p))) == 32 ) {
						goto _st104;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st104;
					}
					{
						goto _ctr183;
					}
					_st105:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr184;
						}
						case 35: {
							goto _st67;
						}
						case 39: {
							goto _ctr185;
						}
						case 40: {
							goto _st104;
						}
						case 41: {
							goto _ctr117;
						}
						case 58: {
							goto _st71;
						}
						case 91: {
							goto _ctr182;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _st97;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _st97;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 64 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st103;
							}
						} else {
							goto _st103;
						}
					} else {
						goto _st97;
					}
					{
						goto _st66;
					}
					_st106:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof106;
					st_case_106:
					if ( ( (*( p))) == 32 ) {
						goto _st106;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st106;
					}
					{
						goto _ctr186;
					}
					_st107:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr117;
						}
						case 34: {
							goto _ctr117;
						}
						case 35: {
							goto _st67;
						}
						case 40: {
							goto _st108;
						}
						case 58: {
							goto _st71;
						}
						case 91: {
							goto _ctr117;
						}
						case 93: {
							goto _ctr117;
						}
						case 95: {
							goto _st107;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr117;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr117;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st107;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _st97;
						}
					} else {
						goto _st107;
					}
					{
						goto _st66;
					}
					_st108:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof108;
					st_case_108:
					if ( ( (*( p))) == 32 ) {
						goto _st108;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st108;
					}
					{
						goto _ctr188;
					}
					_ctr192:
					{
#line 28 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 28 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 109; top+= 1; goto _st52;}}
						}}
					
#line 6351 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr194:
					{
#line 29 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 29 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 109; top+= 1; goto _st50;}}
						}}
					
#line 6362 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr196:
					{
#line 21 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 21 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 6373 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr204:
					{
#line 1 "NONE"
						{switch( act ) {
								case 41:  {
									p = ((te))-1;
									{
#line 16 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 42:  {
									p = ((te))-1;
									{
#line 17 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 43:  {
									p = ((te))-1;
									{
#line 18 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 56:  {
									p = ((te))-1;
									{
#line 88 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 6411 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr206:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 88 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 6422 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr212:
					{
#line 231 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 231 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 6433 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr214:
					{
#line 165 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 165 "lex/lemon_data_string_parser.rl"
								
								pushToken(TK_MATRIX);
								pushFloat(ragel_mtx_rows);
								pushFloat(ragel_mtx_cols);
								{stack[top] = 109; top+= 1; goto _st54;}}
						}}
					
#line 6448 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr232:
					{
#line 20 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 20 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 6459 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr235:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6471 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6485 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr241:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6498 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6512 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr246:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6524 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6538 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr252:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6549 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6563 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr256:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6574 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 22 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 22 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6588 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr260:
					{
#line 44 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 44 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 109; top+= 1; goto _st65;}}
						}}
					
#line 6602 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr261:
					{
#line 37 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 37 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DATA_NAME, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 6619 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr262:
					{
#line 86 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 86 "lex/lemon_data_string_parser.rl"
								pushToken(TK_STRING); ragel_string.clear(); {stack[top] = 109; top+= 1; goto _st52;}}
						}}
					
#line 6630 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr263:
					{
#line 85 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 85 "lex/lemon_data_string_parser.rl"
								pushToken(TK_STRING); ragel_string.clear(); {stack[top] = 109; top+= 1; goto _st50;}}
						}}
					
#line 6641 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr264:
					{
#line 217 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 217 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); {stack[top] = 109; top+= 1; goto _st65;}}
						}}
					
#line 6652 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr266:
					{
#line 30 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 30 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 6669 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_st109:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 6683 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof109;
					st_case_109:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 6693 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _st116;
						}
						case 34: {
							goto _ctr192;
						}
						case 35: {
							goto _st117;
						}
						case 39: {
							goto _ctr194;
						}
						case 40: {
							goto _st133;
						}
						case 41: {
							goto _ctr196;
						}
						case 43: {
							goto _ctr197;
						}
						case 45: {
							goto _ctr197;
						}
						case 48: {
							goto _ctr198;
						}
						case 83: {
							goto _st149;
						}
						case 91: {
							goto _st150;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _st116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st151;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st147;
						}
					} else {
						goto _ctr199;
					}
					{
						goto _ctr190;
					}
					_ctr190:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6753 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6759 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr211:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6767 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 108 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_cols *= 10) += ((( (*( p)))) - '0'); }
					
#line 6773 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6779 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr225:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6787 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 17 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 6793 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr228:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6801 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 18 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 6807 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr231:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6815 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 16 "lex/lemon_data_string_parser.rl"
						{act = 41;}}
					
#line 6821 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr236:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6829 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6839 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6845 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr242:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6853 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6864 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6870 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr247:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6878 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6888 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6894 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr253:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6902 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6911 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6917 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr257:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6925 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6934 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6940 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_st110:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr204;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr204;
					}
					{
						goto _ctr190;
					}
					_ctr237:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6987 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_ctr243:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 7000 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_ctr248:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 7012 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_ctr254:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 7023 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_ctr258:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 7034 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_st111:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr207;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_ctr207:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 7081 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st112;
					_st112:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 58: {
							goto _st114;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr208;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_ctr208:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 7131 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st113;
					_st113:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 58: {
							goto _st114;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st114:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr210;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_ctr210:
					{
#line 108 "lex/lemon_data_string_parser.rl"
						ragel_mtx_cols = (( (*( p)))) - '0'; }
					
#line 7216 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st115;
					_st115:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr211;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st116:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _st116;
						}
						case 41: {
							goto _ctr196;
						}
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st116;
					}
					{
						goto _ctr212;
					}
					_st117:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr214;
						}
						case 102: {
							goto _st123;
						}
						case 110: {
							goto _st127;
						}
						case 116: {
							goto _st130;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr213;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_ctr213:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 7336 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st118;
					_st118:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 58: {
							goto _st120;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr218;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_ctr218:
					{
#line 109 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 7386 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st119;
					_st119:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof119;
					st_case_119:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 58: {
							goto _st120;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st120:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr220;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_ctr220:
					{
#line 108 "lex/lemon_data_string_parser.rl"
						ragel_mtx_cols = (( (*( p)))) - '0'; }
					
#line 7471 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st121;
					_st121:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr214;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr221;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_ctr221:
					{
#line 108 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_cols *= 10) += ((( (*( p)))) - '0'); }
					
#line 7518 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st122;
					_st122:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr214;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st123:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
						case 97: {
							goto _st124;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st124:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
						case 108: {
							goto _st125;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st125:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
						case 115: {
							goto _st126;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st126:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
						case 101: {
							goto _ctr225;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st127:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
						case 117: {
							goto _st128;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st128:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
						case 108: {
							goto _st129;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st129:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
						case 108: {
							goto _ctr228;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st130:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
						case 114: {
							goto _st131;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st131:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
						case 117: {
							goto _st132;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st132:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
						case 101: {
							goto _ctr231;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st133:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof133;
					st_case_133:
					if ( ( (*( p))) == 32 ) {
						goto _st133;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st133;
					}
					{
						goto _ctr232;
					}
					_ctr197:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 7961 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 7967 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st134;
					_st134:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof134;
					st_case_134:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 48: {
							goto _ctr233;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr234;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_ctr233:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 8017 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 8023 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st135;
					_st135:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr235;
						}
						case 34: {
							goto _ctr235;
						}
						case 35: {
							goto _ctr237;
						}
						case 46: {
							goto _ctr238;
						}
						case 47: {
							goto _st138;
						}
						case 91: {
							goto _ctr235;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr235;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr235;
					}
					{
						goto _ctr236;
					}
					_ctr238:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 8076 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 8085 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st136;
					_st136:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof136;
					st_case_136:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr240;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_ctr240:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 8135 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st137;
					_st137:
					if ( p == eof ) {
						if ( cs >= 49 )
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
							goto _ctr241;
						}
						case 34: {
							goto _ctr241;
						}
						case 35: {
							goto _ctr243;
						}
						case 91: {
							goto _ctr241;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr241;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr240;
						}
					} else {
						goto _ctr241;
					}
					{
						goto _ctr242;
					}
					_st138:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof138;
					st_case_138:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 48: {
							goto _ctr244;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr245;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_ctr244:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 8224 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st139;
					_st139:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof139;
					st_case_139:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr246;
						}
						case 34: {
							goto _ctr246;
						}
						case 35: {
							goto _ctr248;
						}
						case 91: {
							goto _ctr246;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr246;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr246;
					}
					{
						goto _ctr247;
					}
					_ctr245:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 8267 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st140;
					_st140:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof140;
					st_case_140:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr246;
						}
						case 34: {
							goto _ctr246;
						}
						case 35: {
							goto _ctr248;
						}
						case 91: {
							goto _ctr246;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr246;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr245;
						}
					} else {
						goto _ctr246;
					}
					{
						goto _ctr247;
					}
					_ctr199:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 8314 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 8320 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 8326 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st141;
					_ctr234:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 8334 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 8340 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st141;
					_st141:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof141;
					st_case_141:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr235;
						}
						case 34: {
							goto _ctr235;
						}
						case 35: {
							goto _ctr237;
						}
						case 46: {
							goto _ctr238;
						}
						case 47: {
							goto _st138;
						}
						case 91: {
							goto _ctr235;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr235;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr234;
						}
					} else {
						goto _ctr235;
					}
					{
						goto _ctr236;
					}
					_ctr198:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_num = {}; }
					
#line 8393 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 8399 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 8405 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st142;
					_st142:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof142;
					st_case_142:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr235;
						}
						case 34: {
							goto _ctr235;
						}
						case 35: {
							goto _ctr237;
						}
						case 46: {
							goto _ctr238;
						}
						case 47: {
							goto _st138;
						}
						case 91: {
							goto _ctr235;
						}
						case 98: {
							goto _st143;
						}
						case 120: {
							goto _st145;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr235;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr235;
					}
					{
						goto _ctr236;
					}
					_st143:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof143;
					st_case_143:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
							goto _ctr251;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_ctr251:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 8499 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st144;
					_st144:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof144;
					st_case_144:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr252;
						}
						case 34: {
							goto _ctr252;
						}
						case 35: {
							goto _ctr254;
						}
						case 91: {
							goto _ctr252;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr252;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
							goto _ctr251;
						}
					} else {
						goto _ctr252;
					}
					{
						goto _ctr253;
					}
					_st145:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof145;
					st_case_145:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr206;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr206;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 70 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
								goto _ctr255;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _ctr255;
						}
					} else {
						goto _ctr255;
					}
					{
						goto _ctr190;
					}
					_ctr255:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 8593 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st146;
					_st146:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof146;
					st_case_146:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr256;
						}
						case 34: {
							goto _ctr256;
						}
						case 35: {
							goto _ctr258;
						}
						case 91: {
							goto _ctr256;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr256;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr256;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 70 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
								goto _ctr255;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _ctr255;
						}
					} else {
						goto _ctr255;
					}
					{
						goto _ctr257;
					}
					_st147:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof147;
					st_case_147:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 40: {
							goto _st148;
						}
						case 91: {
							goto _ctr260;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st147;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st147;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st148:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof148;
					st_case_148:
					if ( ( (*( p))) == 32 ) {
						goto _st148;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st148;
					}
					{
						goto _ctr261;
					}
					_st149:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof149;
					st_case_149:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr206;
						}
						case 34: {
							goto _ctr262;
						}
						case 35: {
							goto _st111;
						}
						case 39: {
							goto _ctr263;
						}
						case 40: {
							goto _st148;
						}
						case 41: {
							goto _ctr206;
						}
						case 91: {
							goto _ctr260;
						}
					}
					if ( ( (*( p))) < 65 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st147;
						}
					} else {
						goto _st147;
					}
					{
						goto _ctr190;
					}
					_st150:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof150;
					st_case_150:
					if ( ( (*( p))) == 32 ) {
						goto _st150;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st150;
					}
					{
						goto _ctr264;
					}
					_st151:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof151;
					st_case_151:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr206;
						}
						case 34: {
							goto _ctr206;
						}
						case 35: {
							goto _st111;
						}
						case 40: {
							goto _st152;
						}
						case 91: {
							goto _ctr206;
						}
						case 95: {
							goto _st151;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr206;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st151;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st151;
						}
					} else {
						goto _ctr206;
					}
					{
						goto _ctr190;
					}
					_st152:
					if ( p == eof ) {
						if ( cs >= 49 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof152;
					st_case_152:
					if ( ( (*( p))) == 32 ) {
						goto _st152;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st152;
					}
					{
						goto _ctr266;
					}
					st_out:
					_test_eof49: cs = 49; goto _test_eof; 
					_test_eof50: cs = 50; goto _test_eof; 
					_test_eof51: cs = 51; goto _test_eof; 
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
					_test_eof52: cs = 52; goto _test_eof; 
					_test_eof53: cs = 53; goto _test_eof; 
					_test_eof18: cs = 18; goto _test_eof; 
					_test_eof19: cs = 19; goto _test_eof; 
					_test_eof20: cs = 20; goto _test_eof; 
					_test_eof21: cs = 21; goto _test_eof; 
					_test_eof22: cs = 22; goto _test_eof; 
					_test_eof23: cs = 23; goto _test_eof; 
					_test_eof24: cs = 24; goto _test_eof; 
					_test_eof25: cs = 25; goto _test_eof; 
					_test_eof26: cs = 26; goto _test_eof; 
					_test_eof27: cs = 27; goto _test_eof; 
					_test_eof28: cs = 28; goto _test_eof; 
					_test_eof29: cs = 29; goto _test_eof; 
					_test_eof30: cs = 30; goto _test_eof; 
					_test_eof31: cs = 31; goto _test_eof; 
					_test_eof32: cs = 32; goto _test_eof; 
					_test_eof33: cs = 33; goto _test_eof; 
					_test_eof34: cs = 34; goto _test_eof; 
					_test_eof54: cs = 54; goto _test_eof; 
					_test_eof55: cs = 55; goto _test_eof; 
					_test_eof56: cs = 56; goto _test_eof; 
					_test_eof35: cs = 35; goto _test_eof; 
					_test_eof57: cs = 57; goto _test_eof; 
					_test_eof36: cs = 36; goto _test_eof; 
					_test_eof58: cs = 58; goto _test_eof; 
					_test_eof37: cs = 37; goto _test_eof; 
					_test_eof59: cs = 59; goto _test_eof; 
					_test_eof60: cs = 60; goto _test_eof; 
					_test_eof61: cs = 61; goto _test_eof; 
					_test_eof62: cs = 62; goto _test_eof; 
					_test_eof38: cs = 38; goto _test_eof; 
					_test_eof63: cs = 63; goto _test_eof; 
					_test_eof39: cs = 39; goto _test_eof; 
					_test_eof64: cs = 64; goto _test_eof; 
					_test_eof65: cs = 65; goto _test_eof; 
					_test_eof66: cs = 66; goto _test_eof; 
					_test_eof67: cs = 67; goto _test_eof; 
					_test_eof68: cs = 68; goto _test_eof; 
					_test_eof69: cs = 69; goto _test_eof; 
					_test_eof70: cs = 70; goto _test_eof; 
					_test_eof40: cs = 40; goto _test_eof; 
					_test_eof41: cs = 41; goto _test_eof; 
					_test_eof42: cs = 42; goto _test_eof; 
					_test_eof71: cs = 71; goto _test_eof; 
					_test_eof43: cs = 43; goto _test_eof; 
					_test_eof44: cs = 44; goto _test_eof; 
					_test_eof45: cs = 45; goto _test_eof; 
					_test_eof72: cs = 72; goto _test_eof; 
					_test_eof73: cs = 73; goto _test_eof; 
					_test_eof74: cs = 74; goto _test_eof; 
					_test_eof75: cs = 75; goto _test_eof; 
					_test_eof46: cs = 46; goto _test_eof; 
					_test_eof47: cs = 47; goto _test_eof; 
					_test_eof48: cs = 48; goto _test_eof; 
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
					_test_eof139: cs = 139; goto _test_eof; 
					_test_eof140: cs = 140; goto _test_eof; 
					_test_eof141: cs = 141; goto _test_eof; 
					_test_eof142: cs = 142; goto _test_eof; 
					_test_eof143: cs = 143; goto _test_eof; 
					_test_eof144: cs = 144; goto _test_eof; 
					_test_eof145: cs = 145; goto _test_eof; 
					_test_eof146: cs = 146; goto _test_eof; 
					_test_eof147: cs = 147; goto _test_eof; 
					_test_eof148: cs = 148; goto _test_eof; 
					_test_eof149: cs = 149; goto _test_eof; 
					_test_eof150: cs = 150; goto _test_eof; 
					_test_eof151: cs = 151; goto _test_eof; 
					_test_eof152: cs = 152; goto _test_eof; 
					
					_test_eof: {}
					if ( p == eof ) {
						switch ( cs ) {
							case 49: {
								break;
							}
							case 50: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 9011 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 51: {
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
							case 52: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 9077 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 53: {
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
							case 23: {
								break;
							}
							case 24: {
								break;
							}
							case 25: {
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
							case 29: {
								break;
							}
							case 30: {
								break;
							}
							case 31: {
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
							case 54: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 9140 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 55: {
								break;
							}
							case 56: {
								break;
							}
							case 35: {
								break;
							}
							case 57: {
								break;
							}
							case 36: {
								break;
							}
							case 58: {
								break;
							}
							case 37: {
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
							case 38: {
								break;
							}
							case 63: {
								break;
							}
							case 39: {
								break;
							}
							case 64: {
								break;
							}
							case 65: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 9194 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
							case 40: {
								break;
							}
							case 41: {
								break;
							}
							case 42: {
								break;
							}
							case 71: {
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
							case 46: {
								break;
							}
							case 47: {
								break;
							}
							case 48: {
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
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 9359 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
							case 139: {
								break;
							}
							case 140: {
								break;
							}
							case 141: {
								break;
							}
							case 142: {
								break;
							}
							case 143: {
								break;
							}
							case 144: {
								break;
							}
							case 145: {
								break;
							}
							case 146: {
								break;
							}
							case 147: {
								break;
							}
							case 148: {
								break;
							}
							case 149: {
								break;
							}
							case 150: {
								break;
							}
							case 151: {
								break;
							}
							case 152: {
								break;
							}
						}
						switch ( cs ) {
						}
						switch ( cs ) {
							case 49:
							goto _st49;case 50:
							goto _st50;case 51:
							goto _ctr73;case 1:
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
							goto _st0;case 52:
							goto _st52;case 53:
							goto _ctr79;case 18:
							goto _ctr24;case 19:
							goto _ctr24;case 20:
							goto _ctr24;case 21:
							goto _ctr24;case 22:
							goto _ctr24;case 23:
							goto _ctr24;case 24:
							goto _ctr24;case 25:
							goto _ctr24;case 26:
							goto _ctr24;case 27:
							goto _ctr24;case 28:
							goto _ctr24;case 29:
							goto _ctr24;case 30:
							goto _ctr24;case 31:
							goto _ctr24;case 32:
							goto _ctr24;case 33:
							goto _ctr24;case 34:
							goto _st34;case 54:
							goto _st54;case 55:
							goto _ctr89;case 56:
							goto _ctr90;case 35:
							goto _st35;case 57:
							goto _ctr91;case 36:
							goto _ctr50;case 58:
							goto _ctr94;case 37:
							goto _ctr50;case 59:
							goto _ctr95;case 60:
							goto _ctr95;case 61:
							goto _ctr91;case 62:
							goto _ctr91;case 38:
							goto _ctr50;case 63:
							goto _ctr98;case 39:
							goto _ctr50;case 64:
							goto _ctr99;case 65:
							goto _st65;case 66:
							goto _ctr117;case 67:
							goto _ctr117;case 68:
							goto _ctr58;case 69:
							goto _ctr117;case 70:
							goto _ctr122;case 40:
							goto _ctr56;case 41:
							goto _ctr58;case 42:
							goto _ctr58;case 71:
							goto _ctr125;case 43:
							goto _ctr56;case 44:
							goto _ctr58;case 45:
							goto _ctr56;case 72:
							goto _ctr126;case 73:
							goto _ctr117;case 74:
							goto _ctr117;case 75:
							goto _ctr117;case 46:
							goto _ctr63;case 47:
							goto _ctr63;case 48:
							goto _ctr63;case 76:
							goto _ctr117;case 77:
							goto _ctr117;case 78:
							goto _ctr117;case 79:
							goto _ctr117;case 80:
							goto _ctr117;case 81:
							goto _ctr117;case 82:
							goto _ctr117;case 83:
							goto _ctr117;case 84:
							goto _ctr117;case 85:
							goto _ctr117;case 86:
							goto _ctr143;case 87:
							goto _ctr144;case 88:
							goto _ctr117;case 89:
							goto _ctr147;case 90:
							goto _ctr117;case 91:
							goto _ctr154;case 92:
							goto _ctr117;case 93:
							goto _ctr160;case 94:
							goto _ctr160;case 95:
							goto _ctr147;case 96:
							goto _ctr147;case 97:
							goto _ctr117;case 98:
							goto _ctr117;case 99:
							goto _ctr169;case 100:
							goto _ctr117;case 101:
							goto _ctr175;case 102:
							goto _ctr147;case 103:
							goto _ctr117;case 104:
							goto _ctr183;case 105:
							goto _ctr117;case 106:
							goto _ctr186;case 107:
							goto _ctr117;case 108:
							goto _ctr188;case 109:
							goto _st109;case 110:
							goto _ctr204;case 111:
							goto _ctr206;case 112:
							goto _ctr206;case 113:
							goto _ctr206;case 114:
							goto _ctr206;case 115:
							goto _ctr206;case 116:
							goto _ctr212;case 117:
							goto _ctr206;case 118:
							goto _ctr206;case 119:
							goto _ctr206;case 120:
							goto _ctr206;case 121:
							goto _ctr206;case 122:
							goto _ctr206;case 123:
							goto _ctr206;case 124:
							goto _ctr206;case 125:
							goto _ctr206;case 126:
							goto _ctr206;case 127:
							goto _ctr206;case 128:
							goto _ctr206;case 129:
							goto _ctr206;case 130:
							goto _ctr206;case 131:
							goto _ctr206;case 132:
							goto _ctr206;case 133:
							goto _ctr232;case 134:
							goto _ctr206;case 135:
							goto _ctr235;case 136:
							goto _ctr206;case 137:
							goto _ctr241;case 138:
							goto _ctr206;case 139:
							goto _ctr246;case 140:
							goto _ctr246;case 141:
							goto _ctr235;case 142:
							goto _ctr235;case 143:
							goto _ctr206;case 144:
							goto _ctr252;case 145:
							goto _ctr206;case 146:
							goto _ctr256;case 147:
							goto _ctr206;case 148:
							goto _ctr261;case 149:
							goto _ctr206;case 150:
							goto _ctr264;case 151:
							goto _ctr206;case 152:
							goto _ctr266;	}
					}
					
					if ( cs >= 49 )
						goto _out; _pop: {}
					_out: {}
				}
				
#line 346 "lex/lemon_data_string_parser.rl"
				
				
			} catch(std::exception& e) {
				LIB_ERR << e.what();
				return false;
			}
			
			if (cs < 
#line 9666 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
			49
#line 353 "lex/lemon_data_string_parser.rl"
			) {
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
