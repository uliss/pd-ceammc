#line 1 "lex/lemon_data_string_parser.rl"
# include "lemon_data_string_parser.h"
# include "lemon_data_string.h"
# include "parser_numeric.h"

# include <cstdint>
# include <cstdlib>
# include <boost/static_string.hpp>

# ifdef NDEBUG
# undef NDEBUG
# endif
# include "lemon_data_parser_impl.h"


#line 238 "lex/lemon_data_string_parser.rl"


# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 26 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
		static const int lemon_data_string_lexer_start = 49;
		static const int lemon_data_string_lexer_first_final = 49;
		static const int lemon_data_string_lexer_error = 0;
		
		static const int lemon_data_string_lexer_en_scan_sqstring = 50;
		static const int lemon_data_string_lexer_en_scan_dqstring = 52;
		static const int lemon_data_string_lexer_en_scan_matrix = 54;
		static const int lemon_data_string_lexer_en_scan_dict = 65;
		static const int lemon_data_string_lexer_en_scan_token = 109;
		static const int lemon_data_string_lexer_en_main = 49;
		
		
#line 245 "lex/lemon_data_string_parser.rl"
		
		
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
			
			// ragel state
			int cs;
			// ragel action
			int act;
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
				
				
#line 133 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					cs = (int)lemon_data_string_lexer_start;
					top = 0;
					ts = 0;
					te = 0;
					act = 0;
				}
				
#line 337 "lex/lemon_data_string_parser.rl"
				
				
#line 145 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
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
#line 237 "lex/lemon_data_string_parser.rl"
						{p = p - 1; } {stack[top] = 49; top+= 1; goto _st109;}}
					
#line 623 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 637 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof49;
					st_case_49:
					{
						goto _ctr68;
					}
					_ctr1:
					{
#line 146 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 146 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 655 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr2:
					{
#line 62 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 62 "lex/lemon_data_string_parser.rl"
								
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
					
#line 690 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr19:
					{
#line 148 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 148 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 701 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr20:
					{
#line 147 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 147 "lex/lemon_data_string_parser.rl"
								ragel_string += '\''; }
						}}
					
#line 712 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr21:
					{
#line 149 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 149 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 723 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr22:
					{
#line 150 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 150 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 734 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr23:
					{
#line 151 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 151 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 745 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr70:
					{
#line 146 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 146 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 756 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr72:
					{
#line 142 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 142 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 767 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr73:
					{
#line 146 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 146 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 778 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 792 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof50;
					st_case_50:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 802 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 823 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
#line 158 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 158 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 1346 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr25:
					{
#line 62 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 62 "lex/lemon_data_string_parser.rl"
								
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
					
#line 1381 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr42:
					{
#line 160 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 160 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 1392 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr43:
					{
#line 159 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 159 "lex/lemon_data_string_parser.rl"
								ragel_string += '"'; }
						}}
					
#line 1403 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr44:
					{
#line 161 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 161 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 1414 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr45:
					{
#line 162 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 162 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 1425 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr46:
					{
#line 163 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 163 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 1436 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr76:
					{
#line 158 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 158 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 1447 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr77:
					{
#line 142 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 142 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 1458 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_ctr79:
					{
#line 158 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 158 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 1469 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 1483 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof52;
					st_case_52:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 1493 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 1514 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
#line 25 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2038 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr84:
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 24 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 2049 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr87:
					{
#line 180 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 180 "lex/lemon_data_string_parser.rl"
								pushToken(TK_MATRIX_CLOSE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 2060 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr88:
					{
#line 179 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;}}
					
#line 2068 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr89:
					{
#line 23 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 23 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 2079 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr90:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2091 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2105 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr93:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2118 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2132 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr94:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2144 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2158 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr97:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2169 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2183 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr98:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2194 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2208 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 2222 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof54;
					st_case_54:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 2232 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr87;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr49;
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
							goto _ctr87;
						}
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st55;
					}
					{
						goto _ctr88;
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
						goto _ctr89;
					}
					_ctr85:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 2320 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 2348 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2354 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2360 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr91;
						}
						case 47: {
							goto _st37;
						}
					}
					{
						goto _ctr90;
					}
					_ctr91:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2394 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 2403 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 2431 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
						goto _ctr93;
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
					
#line 2476 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
						goto _ctr94;
					}
					_ctr53:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 2498 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
						goto _ctr94;
					}
					_ctr49:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2523 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2529 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2535 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr91;
						}
						case 47: {
							goto _st37;
						}
					}
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr49;
					}
					{
						goto _ctr90;
					}
					_ctr86:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2568 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2574 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2580 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr91;
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
						goto _ctr90;
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
					
#line 2633 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
						goto _ctr97;
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
					
#line 2683 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
						goto _ctr98;
					}
					_ctr56:
					{
#line 207 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 207 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 2720 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr58:
					{
#line 1 "NONE"
						{switch( act ) {
								case 22:  {
									p = ((te))-1;
									{
#line 19 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 23:  {
									p = ((te))-1;
									{
#line 20 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 24:  {
									p = ((te))-1;
									{
#line 21 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 28:  {
									p = ((te))-1;
									{
#line 31 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st52;}}
									break; 
								}
								case 29:  {
									p = ((te))-1;
									{
#line 32 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st50;}}
									break; 
								}
								case 38:  {
									p = ((te))-1;
									{
#line 207 "lex/lemon_data_string_parser.rl"
										pushToken(TK_SPACE); }
									break; 
								}
								case 39:  {
									p = ((te))-1;
									{
#line 91 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 2779 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr63:
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 91 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 2791 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr66:
					{
#line 168 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 168 "lex/lemon_data_string_parser.rl"
								
								pushToken(TK_MATRIX);
								pushFloat(ragel_mtx_rows);
								pushFloat(ragel_mtx_cols);
								{stack[top] = 65; top+= 1; goto _st54;}}
						}}
					
#line 2806 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr106:
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 24 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 2817 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr114:
					{
#line 211 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 211 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_CLOSE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 2828 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr116:
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 91 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 2839 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr121:
					{
#line 207 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 207 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 2850 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr124:
					{
#line 53 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 53 "lex/lemon_data_string_parser.rl"
								
								// skip starting whitespaces
								auto ts0 = ts;
								while(*ts0 == ' ') ++ts0;
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DICT_KEY, ts0, te0);
							}
						}}
					
#line 2869 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr125:
					{
#line 31 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 31 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st52;}}
						}}
					
#line 2880 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr142:
					{
#line 32 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 32 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st50;}}
						}}
					
#line 2891 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr143:
					{
#line 23 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 23 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 2902 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr146:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2914 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2928 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr153:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2941 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2955 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr159:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2967 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2981 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr168:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2992 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 3006 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr174:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3017 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 3031 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr180:
					{
#line 47 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 47 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 65; top+= 1; goto _st65;}}
						}}
					
#line 3045 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr181:
					{
#line 40 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 40 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DATA_NAME, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 3062 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr182:
					{
#line 89 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 89 "lex/lemon_data_string_parser.rl"
								pushToken(TK_STRING); ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st52;}}
						}}
					
#line 3073 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr183:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 88 "lex/lemon_data_string_parser.rl"
								pushToken(TK_STRING); ragel_string.clear(); {stack[top] = 65; top+= 1; goto _st50;}}
						}}
					
#line 3084 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr184:
					{
#line 52 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 52 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); {stack[top] = 65; top+= 1; goto _st65;}}
						}}
					
#line 3095 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr186:
					{
#line 33 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 33 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 3112 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 3126 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof65;
					st_case_65:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 3136 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr101;
						}
						case 34: {
							goto _ctr102;
						}
						case 35: {
							goto _st73;
						}
						case 39: {
							goto _ctr104;
						}
						case 40: {
							goto _st87;
						}
						case 41: {
							goto _ctr106;
						}
						case 43: {
							goto _ctr107;
						}
						case 45: {
							goto _ctr107;
						}
						case 48: {
							goto _ctr108;
						}
						case 83: {
							goto _st105;
						}
						case 91: {
							goto _st106;
						}
						case 93: {
							goto _ctr114;
						}
						case 95: {
							goto _st97;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _ctr109;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr101;
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
					_ctr147:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3210 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_ctr154:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3223 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_ctr160:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3235 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_ctr169:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3246 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_ctr175:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3257 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
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
					
#line 3322 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_ctr155:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3335 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_ctr161:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3347 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_ctr170:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3358 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_ctr176:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3369 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 48: {
							goto _ctr118;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _st66;
					}
					_ctr118:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3433 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 39;}}
					
#line 3439 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr120:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3447 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 112 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 3453 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 39;}}
					
#line 3459 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr135:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3467 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 20 "lex/lemon_data_string_parser.rl"
						{act = 23;}}
					
#line 3473 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr138:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3481 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 3487 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr141:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3495 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 19 "lex/lemon_data_string_parser.rl"
						{act = 22;}}
					
#line 3501 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr151:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3509 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3519 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 39;}}
					
#line 3525 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr156:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3533 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3544 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 39;}}
					
#line 3550 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr162:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3558 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3568 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 39;}}
					
#line 3574 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr118;
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
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
					}
					{
						goto _st66;
					}
					_ctr119:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 3638 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr120;
					}
					{
						goto _st66;
					}
					_ctr101:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3702 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 207 "lex/lemon_data_string_parser.rl"
						{act = 38;}}
					
#line 3708 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr101;
						}
						case 34: {
							goto _st40;
						}
						case 39: {
							goto _st43;
						}
						case 41: {
							goto _ctr106;
						}
						case 93: {
							goto _ctr114;
						}
						case 95: {
							goto _st45;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr101;
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
						goto _ctr121;
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
					_ctr164:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3847 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st71;
					_ctr172:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3858 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st71;
					_ctr177:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3869 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
						goto _ctr124;
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
					_ctr102:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3991 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 31 "lex/lemon_data_string_parser.rl"
						{act = 28;}}
					
#line 3997 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
						goto _ctr125;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 48: {
							goto _ctr118;
						}
						case 91: {
							goto _ctr66;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
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
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr126;
					}
					{
						goto _st66;
					}
					_ctr126:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4099 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 4105 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _st46;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr130;
					}
					{
						goto _st66;
					}
					_ctr130:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4169 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 112 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 4175 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _st46;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
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
#line 111 "lex/lemon_data_string_parser.rl"
						ragel_mtx_cols = (( (*( p)))) - '0'; }
					
#line 4256 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
#line 111 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_cols *= 10) += ((( (*( p)))) - '0'); }
					
#line 4284 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
						case 97: {
							goto _st77;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
						case 108: {
							goto _st78;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
						case 115: {
							goto _st79;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
						case 101: {
							goto _ctr135;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
						case 117: {
							goto _st81;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
						case 108: {
							goto _st82;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
						case 108: {
							goto _ctr138;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
						case 114: {
							goto _st84;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
						case 117: {
							goto _st85;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
						case 101: {
							goto _ctr141;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr118;
					}
					{
						goto _st66;
					}
					_ctr104:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4899 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 32 "lex/lemon_data_string_parser.rl"
						{act = 29;}}
					
#line 4905 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
						goto _ctr142;
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
						goto _ctr143;
					}
					_ctr107:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 4962 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 48: {
							goto _ctr144;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr145;
					}
					{
						goto _st66;
					}
					_ctr144:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5026 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5032 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr146;
						}
						case 34: {
							goto _ctr146;
						}
						case 35: {
							goto _ctr148;
						}
						case 46: {
							goto _ctr149;
						}
						case 47: {
							goto _st92;
						}
						case 58: {
							goto _ctr146;
						}
						case 91: {
							goto _ctr146;
						}
						case 93: {
							goto _ctr146;
						}
						case 95: {
							goto _ctr151;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr146;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr146;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr151;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr151;
						}
					} else {
						goto _ctr151;
					}
					{
						goto _ctr147;
					}
					_ctr149:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5106 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 5115 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr152;
					}
					{
						goto _st66;
					}
					_ctr152:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 5179 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr153;
						}
						case 34: {
							goto _ctr153;
						}
						case 35: {
							goto _ctr155;
						}
						case 58: {
							goto _ctr153;
						}
						case 91: {
							goto _ctr153;
						}
						case 93: {
							goto _ctr153;
						}
						case 95: {
							goto _ctr156;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr153;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr153;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr156;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr156;
						}
					} else {
						goto _ctr152;
					}
					{
						goto _ctr154;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 48: {
							goto _ctr157;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _ctr118;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr118;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr158;
					}
					{
						goto _st66;
					}
					_ctr157:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 5299 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr159;
						}
						case 34: {
							goto _ctr159;
						}
						case 35: {
							goto _ctr161;
						}
						case 58: {
							goto _ctr159;
						}
						case 91: {
							goto _ctr159;
						}
						case 93: {
							goto _ctr159;
						}
						case 95: {
							goto _ctr162;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr159;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr159;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr162;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr162;
						}
					} else {
						goto _ctr162;
					}
					{
						goto _ctr160;
					}
					_ctr158:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 5363 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr159;
						}
						case 34: {
							goto _ctr159;
						}
						case 35: {
							goto _ctr161;
						}
						case 58: {
							goto _ctr159;
						}
						case 91: {
							goto _ctr159;
						}
						case 93: {
							goto _ctr159;
						}
						case 95: {
							goto _ctr162;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr159;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr159;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr162;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr162;
						}
					} else {
						goto _ctr158;
					}
					{
						goto _ctr160;
					}
					_ctr145:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5427 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5433 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr146;
						}
						case 34: {
							goto _ctr146;
						}
						case 35: {
							goto _ctr148;
						}
						case 46: {
							goto _ctr149;
						}
						case 47: {
							goto _st92;
						}
						case 58: {
							goto _ctr146;
						}
						case 91: {
							goto _ctr146;
						}
						case 93: {
							goto _ctr146;
						}
						case 95: {
							goto _ctr151;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr146;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr146;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr151;
							}
						} else if ( ( (*( p))) >= 63 ) {
							goto _ctr151;
						}
					} else {
						goto _ctr145;
					}
					{
						goto _ctr147;
					}
					_ctr108:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5503 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5509 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr146;
						}
						case 34: {
							goto _ctr146;
						}
						case 35: {
							goto _ctr148;
						}
						case 46: {
							goto _ctr149;
						}
						case 47: {
							goto _st92;
						}
						case 58: {
							goto _ctr164;
						}
						case 91: {
							goto _ctr146;
						}
						case 93: {
							goto _ctr146;
						}
						case 95: {
							goto _ctr163;
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
								goto _ctr146;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr146;
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
						goto _ctr147;
					}
					_ctr163:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5589 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st97;
					_ctr171:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5600 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st97;
					_ctr178:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5611 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _st71;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _st97;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _st71;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _st97;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
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
						goto _ctr167;
					}
					{
						goto _st66;
					}
					_ctr167:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 5735 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr168;
						}
						case 34: {
							goto _ctr168;
						}
						case 35: {
							goto _ctr170;
						}
						case 58: {
							goto _ctr172;
						}
						case 91: {
							goto _ctr168;
						}
						case 93: {
							goto _ctr168;
						}
						case 95: {
							goto _ctr171;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr168;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr168;
						}
					} else if ( ( (*( p))) > 49 ) {
						if ( ( (*( p))) < 63 ) {
							if ( ( (*( p))) <= 57 ) {
								goto _ctr171;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr171;
							}
						} else {
							goto _ctr171;
						}
					} else {
						goto _ctr167;
					}
					{
						goto _ctr169;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
						}
						case 35: {
							goto _st67;
						}
						case 58: {
							goto _st71;
						}
						case 91: {
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _st97;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) < 39 ) {
							if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) > 41 ) {
							if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _ctr173;
							}
						} else {
							goto _ctr116;
						}
					} else if ( ( (*( p))) > 64 ) {
						if ( ( (*( p))) < 71 ) {
							{
								goto _ctr173;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( ( (*( p))) > 102 ) {
								if ( ( (*( p))) <= 122 ) {
									goto _st97;
								}
							} else if ( ( (*( p))) >= 97 ) {
								goto _ctr173;
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
					_ctr173:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 5871 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr174;
						}
						case 34: {
							goto _ctr174;
						}
						case 35: {
							goto _ctr176;
						}
						case 58: {
							goto _ctr177;
						}
						case 91: {
							goto _ctr174;
						}
						case 93: {
							goto _ctr174;
						}
						case 95: {
							goto _ctr178;
						}
					}
					if ( ( (*( p))) < 63 ) {
						if ( ( (*( p))) < 39 ) {
							if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
								goto _ctr174;
							}
						} else if ( ( (*( p))) > 41 ) {
							if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
								goto _ctr173;
							}
						} else {
							goto _ctr174;
						}
					} else if ( ( (*( p))) > 64 ) {
						if ( ( (*( p))) < 71 ) {
							{
								goto _ctr173;
							}
						} else if ( ( (*( p))) > 90 ) {
							if ( ( (*( p))) > 102 ) {
								if ( ( (*( p))) <= 122 ) {
									goto _ctr178;
								}
							} else if ( ( (*( p))) >= 97 ) {
								goto _ctr173;
							}
						} else {
							goto _ctr178;
						}
					} else {
						goto _ctr178;
					}
					{
						goto _ctr175;
					}
					_ctr109:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5947 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5953 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr146;
						}
						case 34: {
							goto _ctr146;
						}
						case 35: {
							goto _ctr148;
						}
						case 46: {
							goto _ctr149;
						}
						case 47: {
							goto _st92;
						}
						case 58: {
							goto _ctr164;
						}
						case 91: {
							goto _ctr146;
						}
						case 93: {
							goto _ctr146;
						}
						case 95: {
							goto _ctr163;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr146;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr146;
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
						goto _ctr109;
					}
					{
						goto _ctr147;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
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
							goto _ctr180;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _st97;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
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
						goto _ctr181;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr182;
						}
						case 35: {
							goto _st67;
						}
						case 39: {
							goto _ctr183;
						}
						case 40: {
							goto _st104;
						}
						case 41: {
							goto _ctr116;
						}
						case 58: {
							goto _st71;
						}
						case 91: {
							goto _ctr180;
						}
						case 93: {
							goto _ctr116;
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
							goto _ctr116;
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
						goto _ctr184;
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
							goto _ctr116;
						}
						case 34: {
							goto _ctr116;
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
							goto _ctr116;
						}
						case 93: {
							goto _ctr116;
						}
						case 95: {
							goto _st107;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr116;
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
						goto _ctr186;
					}
					_ctr190:
					{
#line 31 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 31 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 109; top+= 1; goto _st52;}}
						}}
					
#line 6273 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr192:
					{
#line 32 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 32 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 109; top+= 1; goto _st50;}}
						}}
					
#line 6284 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr194:
					{
#line 24 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 24 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 6295 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr202:
					{
#line 1 "NONE"
						{switch( act ) {
								case 41:  {
									p = ((te))-1;
									{
#line 19 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 42:  {
									p = ((te))-1;
									{
#line 20 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 43:  {
									p = ((te))-1;
									{
#line 21 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 56:  {
									p = ((te))-1;
									{
#line 91 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 6333 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr204:
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 91 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 6344 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr210:
					{
#line 234 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 234 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 6355 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr212:
					{
#line 168 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 168 "lex/lemon_data_string_parser.rl"
								
								pushToken(TK_MATRIX);
								pushFloat(ragel_mtx_rows);
								pushFloat(ragel_mtx_cols);
								{stack[top] = 109; top+= 1; goto _st54;}}
						}}
					
#line 6370 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr230:
					{
#line 23 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 23 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 6381 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr232:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6393 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6407 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr238:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6420 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6434 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr243:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6446 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6460 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr249:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6471 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6485 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr253:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6496 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 25 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 25 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6510 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr257:
					{
#line 47 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 47 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 109; top+= 1; goto _st65;}}
						}}
					
#line 6524 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr258:
					{
#line 40 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 40 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DATA_NAME, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 6541 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr259:
					{
#line 89 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 89 "lex/lemon_data_string_parser.rl"
								pushToken(TK_STRING); ragel_string.clear(); {stack[top] = 109; top+= 1; goto _st52;}}
						}}
					
#line 6552 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr260:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 88 "lex/lemon_data_string_parser.rl"
								pushToken(TK_STRING); ragel_string.clear(); {stack[top] = 109; top+= 1; goto _st50;}}
						}}
					
#line 6563 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr261:
					{
#line 220 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 220 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); {stack[top] = 109; top+= 1; goto _st65;}}
						}}
					
#line 6574 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_ctr263:
					{
#line 33 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 33 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 6591 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 6605 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof109;
					st_case_109:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 6615 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _st116;
						}
						case 34: {
							goto _ctr190;
						}
						case 35: {
							goto _st117;
						}
						case 39: {
							goto _ctr192;
						}
						case 40: {
							goto _st133;
						}
						case 41: {
							goto _ctr194;
						}
						case 43: {
							goto _ctr195;
						}
						case 45: {
							goto _ctr195;
						}
						case 48: {
							goto _ctr196;
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
						goto _ctr197;
					}
					{
						goto _ctr188;
					}
					_ctr188:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6675 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6681 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr209:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6689 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 111 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_cols *= 10) += ((( (*( p)))) - '0'); }
					
#line 6695 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6701 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr223:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6709 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 20 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 6715 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr226:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6723 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 6729 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr229:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6737 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 19 "lex/lemon_data_string_parser.rl"
						{act = 41;}}
					
#line 6743 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr233:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6751 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6761 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6767 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr239:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6775 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6786 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6792 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr244:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6800 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6810 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6816 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr250:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6824 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6833 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6839 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_ctr254:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6847 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6856 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 56;}}
					
#line 6862 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr202;
						}
						case 34: {
							goto _ctr202;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr202;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr202;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr202;
					}
					{
						goto _ctr188;
					}
					_ctr234:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6909 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_ctr240:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6922 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_ctr245:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6934 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_ctr251:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6945 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_ctr255:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6956 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr205;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
					}
					_ctr205:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 7003 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 58: {
							goto _st114;
						}
						case 91: {
							goto _ctr204;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr206;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
					}
					_ctr206:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 7053 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 58: {
							goto _st114;
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
						goto _ctr188;
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
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr208;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
					}
					_ctr208:
					{
#line 111 "lex/lemon_data_string_parser.rl"
						ragel_mtx_cols = (( (*( p)))) - '0'; }
					
#line 7138 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr209;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
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
							goto _ctr194;
						}
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st116;
					}
					{
						goto _ctr210;
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr212;
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
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr211;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
					}
					_ctr211:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						ragel_mtx_rows = (( (*( p)))) - '0'; }
					
#line 7258 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 58: {
							goto _st120;
						}
						case 91: {
							goto _ctr204;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr216;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
					}
					_ctr216:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_rows *= 10) += ((( (*( p)))) - '0'); }
					
#line 7308 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 58: {
							goto _st120;
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
						goto _ctr188;
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
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr218;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
					}
					_ctr218:
					{
#line 111 "lex/lemon_data_string_parser.rl"
						ragel_mtx_cols = (( (*( p)))) - '0'; }
					
#line 7393 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr212;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr219;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
					}
					_ctr219:
					{
#line 111 "lex/lemon_data_string_parser.rl"
						(ragel_mtx_cols *= 10) += ((( (*( p)))) - '0'); }
					
#line 7440 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 91: {
							goto _ctr212;
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
						goto _ctr188;
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
						case 97: {
							goto _st124;
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
						goto _ctr188;
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
						case 108: {
							goto _st125;
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
						goto _ctr188;
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
						case 115: {
							goto _st126;
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
						goto _ctr188;
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
						case 101: {
							goto _ctr223;
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
						goto _ctr188;
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
						case 117: {
							goto _st128;
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
						goto _ctr188;
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
						case 108: {
							goto _st129;
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
						goto _ctr188;
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
						case 108: {
							goto _ctr226;
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
						goto _ctr188;
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
						case 114: {
							goto _st131;
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
						goto _ctr188;
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
						case 117: {
							goto _st132;
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
						goto _ctr188;
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
						case 101: {
							goto _ctr229;
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
						goto _ctr188;
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
						goto _ctr230;
					}
					_ctr195:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 7883 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 48: {
							goto _ctr231;
						}
						case 91: {
							goto _ctr204;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr197;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
					}
					_ctr231:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 7933 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 7939 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr232;
						}
						case 34: {
							goto _ctr232;
						}
						case 35: {
							goto _ctr234;
						}
						case 46: {
							goto _ctr235;
						}
						case 47: {
							goto _st138;
						}
						case 91: {
							goto _ctr232;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr232;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr232;
					}
					{
						goto _ctr233;
					}
					_ctr235:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 7992 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 8001 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr237;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
					}
					_ctr237:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 8051 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr238;
						}
						case 34: {
							goto _ctr238;
						}
						case 35: {
							goto _ctr240;
						}
						case 91: {
							goto _ctr238;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr238;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr237;
						}
					} else {
						goto _ctr238;
					}
					{
						goto _ctr239;
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 48: {
							goto _ctr241;
						}
						case 91: {
							goto _ctr204;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr242;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
					}
					_ctr241:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 8140 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr243;
						}
						case 34: {
							goto _ctr243;
						}
						case 35: {
							goto _ctr245;
						}
						case 91: {
							goto _ctr243;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr243;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr243;
					}
					{
						goto _ctr244;
					}
					_ctr242:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 8183 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr243;
						}
						case 34: {
							goto _ctr243;
						}
						case 35: {
							goto _ctr245;
						}
						case 91: {
							goto _ctr243;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr243;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr242;
						}
					} else {
						goto _ctr243;
					}
					{
						goto _ctr244;
					}
					_ctr197:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 8230 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 8236 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr232;
						}
						case 34: {
							goto _ctr232;
						}
						case 35: {
							goto _ctr234;
						}
						case 46: {
							goto _ctr235;
						}
						case 47: {
							goto _st138;
						}
						case 91: {
							goto _ctr232;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr232;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr197;
						}
					} else {
						goto _ctr232;
					}
					{
						goto _ctr233;
					}
					_ctr196:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 8289 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 8295 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr232;
						}
						case 34: {
							goto _ctr232;
						}
						case 35: {
							goto _ctr234;
						}
						case 46: {
							goto _ctr235;
						}
						case 47: {
							goto _st138;
						}
						case 91: {
							goto _ctr232;
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
							goto _ctr232;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr232;
					}
					{
						goto _ctr233;
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
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
							goto _ctr248;
						}
					} else {
						goto _ctr204;
					}
					{
						goto _ctr188;
					}
					_ctr248:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 8389 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr249;
						}
						case 34: {
							goto _ctr249;
						}
						case 35: {
							goto _ctr251;
						}
						case 91: {
							goto _ctr249;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr249;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
							goto _ctr248;
						}
					} else {
						goto _ctr249;
					}
					{
						goto _ctr250;
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
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr204;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 70 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
								goto _ctr252;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _ctr252;
						}
					} else {
						goto _ctr252;
					}
					{
						goto _ctr188;
					}
					_ctr252:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 8483 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr253;
						}
						case 34: {
							goto _ctr253;
						}
						case 35: {
							goto _ctr255;
						}
						case 91: {
							goto _ctr253;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr253;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr253;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 70 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
								goto _ctr252;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _ctr252;
						}
					} else {
						goto _ctr252;
					}
					{
						goto _ctr254;
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 40: {
							goto _st148;
						}
						case 91: {
							goto _ctr257;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
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
						goto _ctr204;
					}
					{
						goto _ctr188;
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
						goto _ctr258;
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr259;
						}
						case 35: {
							goto _st111;
						}
						case 39: {
							goto _ctr260;
						}
						case 40: {
							goto _st148;
						}
						case 41: {
							goto _ctr204;
						}
						case 91: {
							goto _ctr257;
						}
					}
					if ( ( (*( p))) < 65 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
						}
					} else if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st147;
						}
					} else {
						goto _st147;
					}
					{
						goto _ctr188;
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
						goto _ctr261;
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
							goto _ctr204;
						}
						case 34: {
							goto _ctr204;
						}
						case 35: {
							goto _st111;
						}
						case 40: {
							goto _st152;
						}
						case 91: {
							goto _ctr204;
						}
						case 95: {
							goto _st151;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr204;
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
						goto _ctr204;
					}
					{
						goto _ctr188;
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
						goto _ctr263;
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
								
#line 8901 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
								
#line 8967 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
								
#line 9030 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
								
#line 9084 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
								
#line 9249 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
							goto _ctr88;case 56:
							goto _ctr89;case 35:
							goto _st35;case 57:
							goto _ctr90;case 36:
							goto _ctr50;case 58:
							goto _ctr93;case 37:
							goto _ctr50;case 59:
							goto _ctr94;case 60:
							goto _ctr94;case 61:
							goto _ctr90;case 62:
							goto _ctr90;case 38:
							goto _ctr50;case 63:
							goto _ctr97;case 39:
							goto _ctr50;case 64:
							goto _ctr98;case 65:
							goto _st65;case 66:
							goto _ctr116;case 67:
							goto _ctr116;case 68:
							goto _ctr58;case 69:
							goto _ctr116;case 70:
							goto _ctr121;case 40:
							goto _ctr56;case 41:
							goto _ctr58;case 42:
							goto _ctr58;case 71:
							goto _ctr124;case 43:
							goto _ctr56;case 44:
							goto _ctr58;case 45:
							goto _ctr56;case 72:
							goto _ctr125;case 73:
							goto _ctr116;case 74:
							goto _ctr116;case 75:
							goto _ctr116;case 46:
							goto _ctr63;case 47:
							goto _ctr63;case 48:
							goto _ctr63;case 76:
							goto _ctr116;case 77:
							goto _ctr116;case 78:
							goto _ctr116;case 79:
							goto _ctr116;case 80:
							goto _ctr116;case 81:
							goto _ctr116;case 82:
							goto _ctr116;case 83:
							goto _ctr116;case 84:
							goto _ctr116;case 85:
							goto _ctr116;case 86:
							goto _ctr142;case 87:
							goto _ctr143;case 88:
							goto _ctr116;case 89:
							goto _ctr146;case 90:
							goto _ctr116;case 91:
							goto _ctr153;case 92:
							goto _ctr116;case 93:
							goto _ctr159;case 94:
							goto _ctr159;case 95:
							goto _ctr146;case 96:
							goto _ctr146;case 97:
							goto _ctr116;case 98:
							goto _ctr116;case 99:
							goto _ctr168;case 100:
							goto _ctr116;case 101:
							goto _ctr174;case 102:
							goto _ctr146;case 103:
							goto _ctr116;case 104:
							goto _ctr181;case 105:
							goto _ctr116;case 106:
							goto _ctr184;case 107:
							goto _ctr116;case 108:
							goto _ctr186;case 109:
							goto _st109;case 110:
							goto _ctr202;case 111:
							goto _ctr204;case 112:
							goto _ctr204;case 113:
							goto _ctr204;case 114:
							goto _ctr204;case 115:
							goto _ctr204;case 116:
							goto _ctr210;case 117:
							goto _ctr204;case 118:
							goto _ctr204;case 119:
							goto _ctr204;case 120:
							goto _ctr204;case 121:
							goto _ctr204;case 122:
							goto _ctr204;case 123:
							goto _ctr204;case 124:
							goto _ctr204;case 125:
							goto _ctr204;case 126:
							goto _ctr204;case 127:
							goto _ctr204;case 128:
							goto _ctr204;case 129:
							goto _ctr204;case 130:
							goto _ctr204;case 131:
							goto _ctr204;case 132:
							goto _ctr204;case 133:
							goto _ctr230;case 134:
							goto _ctr204;case 135:
							goto _ctr232;case 136:
							goto _ctr204;case 137:
							goto _ctr238;case 138:
							goto _ctr204;case 139:
							goto _ctr243;case 140:
							goto _ctr243;case 141:
							goto _ctr232;case 142:
							goto _ctr232;case 143:
							goto _ctr204;case 144:
							goto _ctr249;case 145:
							goto _ctr204;case 146:
							goto _ctr253;case 147:
							goto _ctr204;case 148:
							goto _ctr258;case 149:
							goto _ctr204;case 150:
							goto _ctr261;case 151:
							goto _ctr204;case 152:
							goto _ctr263;	}
					}
					
					if ( cs >= 49 )
						goto _out; _pop: {}
					_out: {}
				}
				
#line 338 "lex/lemon_data_string_parser.rl"
				
				
			} catch(std::exception& e) {
				LIB_ERR << e.what();
				return false;
			}
			
			if (cs < 
#line 9556 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
			49
#line 345 "lex/lemon_data_string_parser.rl"
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
