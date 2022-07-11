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


#line 232 "lex/lemon_data_string_parser.rl"


# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 58 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
		static const int lemon_data_string_lexer_start = 41;
		static const int lemon_data_string_lexer_first_final = 41;
		static const int lemon_data_string_lexer_error = 0;
		
		static const int lemon_data_string_lexer_en_scan_sqstring = 42;
		static const int lemon_data_string_lexer_en_scan_dqstring = 44;
		static const int lemon_data_string_lexer_en_scan_dict = 46;
		static const int lemon_data_string_lexer_en_scan_token = 88;
		static const int lemon_data_string_lexer_en_main = 41;
		
		
#line 239 "lex/lemon_data_string_parser.rl"
		
		
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
			parse_ok_ = true;
			
			DECLARE_RAGEL_COMMON_VARS;
			DECLARE_RAGEL_NUMERIC_VARS;
			
			// for quoted string parser
			boost::static_string<512> ragel_string;
			
			try {
				
				
#line 150 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					cs = (int)lemon_data_string_lexer_start;
					top = 0;
					ts = 0;
					te = 0;
					act = 0;
				}
				
#line 317 "lex/lemon_data_string_parser.rl"
				
				
#line 162 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					if ( p == pe )
						goto _test_eof;
					goto _resume;
					
					_again: {}
					switch ( cs ) {
						case 41: goto _st41;
						case 42: goto _st42;
						case 43: goto _st43;
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
						case 44: goto _st44;
						case 45: goto _st45;
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
						case 46: goto _st46;
						case 47: goto _st47;
						case 48: goto _st48;
						case 49: goto _st49;
						case 35: goto _st35;
						case 36: goto _st36;
						case 37: goto _st37;
						case 50: goto _st50;
						case 38: goto _st38;
						case 39: goto _st39;
						case 40: goto _st40;
						case 51: goto _st51;
						case 52: goto _st52;
						case 53: goto _st53;
						case 54: goto _st54;
						case 55: goto _st55;
						case 56: goto _st56;
						case 57: goto _st57;
						case 58: goto _st58;
						case 59: goto _st59;
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
					}
					
					_resume: {}
					switch ( cs ) {
						case 41:
						goto st_case_41;
						case 42:
						goto st_case_42;
						case 43:
						goto st_case_43;
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
						case 44:
						goto st_case_44;
						case 45:
						goto st_case_45;
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
						case 46:
						goto st_case_46;
						case 47:
						goto st_case_47;
						case 48:
						goto st_case_48;
						case 49:
						goto st_case_49;
						case 35:
						goto st_case_35;
						case 36:
						goto st_case_36;
						case 37:
						goto st_case_37;
						case 50:
						goto st_case_50;
						case 38:
						goto st_case_38;
						case 39:
						goto st_case_39;
						case 40:
						goto st_case_40;
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
					}
					goto st_out;
					_ctr55:
					{
#line 231 "lex/lemon_data_string_parser.rl"
						{p = p - 1; } {stack[top] = 41; top+= 1; goto _st88;}}
					
#line 544 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st41;
					_st41:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 558 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof41;
					st_case_41:
					if ( ( (*( p))) == 32 ) {
						goto _st41;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st41;
					}
					{
						goto _ctr55;
					}
					_ctr1:
					{
#line 161 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 161 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 582 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_ctr2:
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 91 "lex/lemon_data_string_parser.rl"
								
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
					
#line 617 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_ctr19:
					{
#line 163 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 163 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 628 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_ctr20:
					{
#line 162 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 162 "lex/lemon_data_string_parser.rl"
								ragel_string += '\''; }
						}}
					
#line 639 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_ctr21:
					{
#line 164 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 164 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 650 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_ctr22:
					{
#line 165 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 165 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 661 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_ctr23:
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 166 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 672 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_ctr57:
					{
#line 161 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 161 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 683 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_ctr59:
					{
#line 157 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 157 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 694 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_ctr60:
					{
#line 161 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 161 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 705 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_st42:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 719 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof42;
					st_case_42:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 729 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 37: {
							goto _ctr58;
						}
						case 39: {
							goto _ctr59;
						}
						case 96: {
							goto _st17;
						}
					}
					{
						goto _ctr57;
					}
					_ctr58:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 750 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st43;
					_st43:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof43;
					st_case_43:
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
						goto _ctr60;
					}
					_st1:
					if ( p == eof ) {
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
#line 173 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 173 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 1273 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_ctr25:
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 91 "lex/lemon_data_string_parser.rl"
								
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
					
#line 1308 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_ctr42:
					{
#line 175 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 175 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 1319 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_ctr43:
					{
#line 174 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 174 "lex/lemon_data_string_parser.rl"
								ragel_string += '"'; }
						}}
					
#line 1330 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_ctr44:
					{
#line 176 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 176 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 1341 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_ctr45:
					{
#line 177 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 177 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 1352 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_ctr46:
					{
#line 178 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 178 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 1363 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_ctr63:
					{
#line 173 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 173 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 1374 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_ctr64:
					{
#line 157 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 157 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 1385 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_ctr66:
					{
#line 173 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 173 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 1396 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_st44:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 1410 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof44;
					st_case_44:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 1420 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 34: {
							goto _ctr64;
						}
						case 37: {
							goto _ctr65;
						}
						case 96: {
							goto _st34;
						}
					}
					{
						goto _ctr63;
					}
					_ctr65:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1441 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st45;
					_st45:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof45;
					st_case_45:
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
						goto _ctr66;
					}
					_st18:
					if ( p == eof ) {
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
						if ( cs >= 41 )
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
					_ctr47:
					{
#line 204 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 204 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 1960 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr49:
					{
#line 1 "NONE"
						{switch( act ) {
								case 17:  {
									p = ((te))-1;
									{
#line 51 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 18:  {
									p = ((te))-1;
									{
#line 52 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 19:  {
									p = ((te))-1;
									{
#line 53 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 23:  {
									p = ((te))-1;
									{
#line 63 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 46; top+= 1; goto _st44;}}
									break; 
								}
								case 24:  {
									p = ((te))-1;
									{
#line 64 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 46; top+= 1; goto _st42;}}
									break; 
								}
								case 30:  {
									p = ((te))-1;
									{
#line 204 "lex/lemon_data_string_parser.rl"
										pushToken(TK_SPACE); }
									break; 
								}
								case 31:  {
									p = ((te))-1;
									{
#line 117 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 2019 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr75:
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 56 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 2030 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr82:
					{
#line 208 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 208 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_CLOSE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 2041 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr84:
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 117 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 2052 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr86:
					{
#line 204 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 204 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 2063 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr89:
					{
#line 85 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 85 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DICT_KEY, ts, te0);
							}
						}}
					
#line 2079 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr90:
					{
#line 63 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 63 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 46; top+= 1; goto _st44;}}
						}}
					
#line 2090 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr104:
					{
#line 64 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 64 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 46; top+= 1; goto _st42;}}
						}}
					
#line 2101 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr105:
					{
#line 55 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 55 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 2112 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr108:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2124 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 57 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 57 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2138 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr114:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2151 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 57 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 57 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2165 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr119:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2177 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 57 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 57 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2191 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr127:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2202 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 57 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 57 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2216 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr133:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2227 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 57 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 57 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 2241 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr140:
					{
#line 79 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 79 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 46; top+= 1; goto _st46;}}
						}}
					
#line 2255 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr142:
					{
#line 72 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 72 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DATA_NAME, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 2272 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr143:
					{
#line 84 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 84 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); }
						}}
					
#line 2283 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_ctr145:
					{
#line 65 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 65 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 2300 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_st46:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 2314 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof46;
					st_case_46:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 2324 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr70;
						}
						case 34: {
							goto _ctr71;
						}
						case 35: {
							goto _st52;
						}
						case 39: {
							goto _ctr73;
						}
						case 40: {
							goto _st64;
						}
						case 41: {
							goto _ctr75;
						}
						case 43: {
							goto _ctr76;
						}
						case 45: {
							goto _ctr76;
						}
						case 48: {
							goto _ctr77;
						}
						case 63: {
							goto _st74;
						}
						case 91: {
							goto _st85;
						}
						case 93: {
							goto _ctr82;
						}
						case 95: {
							goto _st74;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr70;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st86;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st82;
						}
					} else {
						goto _ctr78;
					}
					{
						goto _st47;
					}
					_ctr109:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2394 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_ctr115:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2407 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_ctr120:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2419 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_ctr128:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2430 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_ctr134:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2441 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_st47:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_ctr85:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2498 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 31;}}
					
#line 2504 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_ctr97:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2512 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 52 "lex/lemon_data_string_parser.rl"
						{act = 18;}}
					
#line 2518 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_ctr100:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2526 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 53 "lex/lemon_data_string_parser.rl"
						{act = 19;}}
					
#line 2532 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_ctr103:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2540 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 51 "lex/lemon_data_string_parser.rl"
						{act = 17;}}
					
#line 2546 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_ctr112:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2554 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2564 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 31;}}
					
#line 2570 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_ctr116:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2578 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2589 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 31;}}
					
#line 2595 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_ctr121:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2603 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2613 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 31;}}
					
#line 2619 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_ctr138:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2627 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2636 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 31;}}
					
#line 2642 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_st48:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr49;
						}
						case 34: {
							goto _ctr49;
						}
						case 58: {
							goto _ctr49;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr49;
						}
						case 93: {
							goto _ctr49;
						}
						case 95: {
							goto _ctr85;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr49;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr49;
					}
					{
						goto _st47;
					}
					_ctr70:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2702 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 204 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 2708 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st49;
					_st49:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr70;
						}
						case 34: {
							goto _st35;
						}
						case 39: {
							goto _st38;
						}
						case 41: {
							goto _ctr75;
						}
						case 63: {
							goto _st40;
						}
						case 93: {
							goto _ctr82;
						}
						case 95: {
							goto _st40;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr70;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st40;
						}
					} else {
						goto _st40;
					}
					{
						goto _ctr86;
					}
					_st35:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof35;
					st_case_35:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st36;
						}
						case 95: {
							goto _st36;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st36;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st36;
					}
					{
						goto _ctr47;
					}
					_st36:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof36;
					st_case_36:
					switch( ( (*( p))) ) {
						case 34: {
							goto _st37;
						}
						case 63: {
							goto _st36;
						}
						case 95: {
							goto _st36;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st36;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st36;
					}
					{
						goto _ctr49;
					}
					_st37:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof37;
					st_case_37:
					if ( ( (*( p))) == 58 ) {
						goto _st50;
					}
					{
						goto _ctr49;
					}
					_ctr123:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2846 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr130:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2857 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_ctr135:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2868 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_st50:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof50;
					st_case_50:
					if ( ( (*( p))) == 32 ) {
						goto _st50;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st50;
					}
					{
						goto _ctr89;
					}
					_st38:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof38;
					st_case_38:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st39;
						}
						case 95: {
							goto _st39;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st39;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st39;
					}
					{
						goto _ctr47;
					}
					_st39:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof39;
					st_case_39:
					switch( ( (*( p))) ) {
						case 39: {
							goto _st37;
						}
						case 63: {
							goto _st39;
						}
						case 95: {
							goto _st39;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st39;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st39;
					}
					{
						goto _ctr49;
					}
					_st40:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof40;
					st_case_40:
					switch( ( (*( p))) ) {
						case 58: {
							goto _st50;
						}
						case 63: {
							goto _st40;
						}
						case 95: {
							goto _st40;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st40;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st40;
					}
					{
						goto _ctr47;
					}
					_ctr71:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2989 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 63 "lex/lemon_data_string_parser.rl"
						{act = 23;}}
					
#line 2995 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st51;
					_st51:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof51;
					st_case_51:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st36;
						}
						case 95: {
							goto _st36;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st36;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st36;
					}
					{
						goto _ctr90;
					}
					_st52:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
						case 102: {
							goto _st53;
						}
						case 110: {
							goto _st57;
						}
						case 116: {
							goto _st60;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st53:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
						case 97: {
							goto _st54;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st54:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
						case 108: {
							goto _st55;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st55:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
						case 115: {
							goto _st56;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st56:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof56;
					st_case_56:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
						case 101: {
							goto _ctr97;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st57:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
						case 117: {
							goto _st58;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st58:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
						case 108: {
							goto _st59;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st59:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
						case 108: {
							goto _ctr100;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st60:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
						case 114: {
							goto _st61;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st61:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
						case 117: {
							goto _st62;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st62:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
						case 101: {
							goto _ctr103;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr85;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_ctr73:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3640 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 64 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 3646 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st63;
					_st63:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof63;
					st_case_63:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st39;
						}
						case 95: {
							goto _st39;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st39;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st39;
					}
					{
						goto _ctr104;
					}
					_st64:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof64;
					st_case_64:
					if ( ( (*( p))) == 32 ) {
						goto _st64;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st64;
					}
					{
						goto _ctr105;
					}
					_ctr76:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 3703 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_st65:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 48: {
							goto _ctr106;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 49 ) {
							goto _ctr107;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_ctr106:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 3763 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 3769 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_st66:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr108;
						}
						case 34: {
							goto _ctr108;
						}
						case 46: {
							goto _ctr110;
						}
						case 47: {
							goto _st69;
						}
						case 58: {
							goto _ctr108;
						}
						case 63: {
							goto _ctr112;
						}
						case 91: {
							goto _ctr108;
						}
						case 93: {
							goto _ctr108;
						}
						case 95: {
							goto _ctr112;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr108;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr112;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr112;
						}
					} else {
						goto _ctr108;
					}
					{
						goto _ctr109;
					}
					_ctr110:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3839 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 3848 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_st67:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr113;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_ctr113:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 3908 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_st68:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr114;
						}
						case 34: {
							goto _ctr114;
						}
						case 58: {
							goto _ctr114;
						}
						case 63: {
							goto _ctr116;
						}
						case 91: {
							goto _ctr114;
						}
						case 93: {
							goto _ctr114;
						}
						case 95: {
							goto _ctr116;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr114;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr116;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr113;
						}
					} else {
						goto _ctr114;
					}
					{
						goto _ctr115;
					}
					_st69:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 48: {
							goto _ctr117;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr85;
							}
						} else if ( ( (*( p))) >= 49 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_ctr117:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 4020 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st70;
					_st70:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr119;
						}
						case 34: {
							goto _ctr119;
						}
						case 58: {
							goto _ctr119;
						}
						case 63: {
							goto _ctr121;
						}
						case 91: {
							goto _ctr119;
						}
						case 93: {
							goto _ctr119;
						}
						case 95: {
							goto _ctr121;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr119;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr121;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr121;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _ctr120;
					}
					_ctr118:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 4080 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st71;
					_st71:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr119;
						}
						case 34: {
							goto _ctr119;
						}
						case 58: {
							goto _ctr119;
						}
						case 63: {
							goto _ctr121;
						}
						case 91: {
							goto _ctr119;
						}
						case 93: {
							goto _ctr119;
						}
						case 95: {
							goto _ctr121;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr119;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr121;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr118;
						}
					} else {
						goto _ctr119;
					}
					{
						goto _ctr120;
					}
					_ctr107:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 4140 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 4146 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_st72:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr108;
						}
						case 34: {
							goto _ctr108;
						}
						case 46: {
							goto _ctr110;
						}
						case 47: {
							goto _st69;
						}
						case 58: {
							goto _ctr108;
						}
						case 63: {
							goto _ctr112;
						}
						case 91: {
							goto _ctr108;
						}
						case 93: {
							goto _ctr108;
						}
						case 95: {
							goto _ctr112;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr108;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr112;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr107;
						}
					} else {
						goto _ctr108;
					}
					{
						goto _ctr109;
					}
					_ctr77:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 4212 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 4218 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st73;
					_st73:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr108;
						}
						case 34: {
							goto _ctr108;
						}
						case 46: {
							goto _ctr110;
						}
						case 47: {
							goto _st69;
						}
						case 58: {
							goto _ctr123;
						}
						case 63: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr108;
						}
						case 93: {
							goto _ctr108;
						}
						case 95: {
							goto _ctr122;
						}
						case 98: {
							goto _st75;
						}
						case 120: {
							goto _st77;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr108;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr122;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr122;
						}
					} else {
						goto _ctr108;
					}
					{
						goto _ctr109;
					}
					_ctr122:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4294 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st74;
					_ctr129:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4305 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st74;
					_ctr136:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4316 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st74;
					_st74:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _st50;
						}
						case 63: {
							goto _st74;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _st74;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st74;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st74;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st75:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _st50;
						}
						case 63: {
							goto _st74;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _st74;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr84;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 49 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st74;
							}
						} else {
							goto _st74;
						}
					} else {
						goto _ctr126;
					}
					{
						goto _st47;
					}
					_ctr126:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 4432 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st76;
					_st76:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr127;
						}
						case 34: {
							goto _ctr127;
						}
						case 58: {
							goto _ctr130;
						}
						case 63: {
							goto _ctr129;
						}
						case 91: {
							goto _ctr127;
						}
						case 93: {
							goto _ctr127;
						}
						case 95: {
							goto _ctr129;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr127;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr127;
						}
					} else if ( ( (*( p))) > 49 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr129;
							}
						} else {
							goto _ctr129;
						}
					} else {
						goto _ctr126;
					}
					{
						goto _ctr128;
					}
					_st77:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 58: {
							goto _st50;
						}
						case 63: {
							goto _st74;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _st74;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr84;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) < 97 ) {
							if ( 65 <= ( (*( p))) && ( (*( p))) <= 70 ) {
								goto _ctr132;
							}
						} else if ( ( (*( p))) > 102 ) {
							if ( ( (*( p))) <= 122 ) {
								goto _st74;
							}
						} else {
							goto _ctr131;
						}
					} else {
						goto _ctr131;
					}
					{
						goto _st47;
					}
					_ctr131:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 4556 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st78;
					_st78:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr133;
						}
						case 34: {
							goto _ctr133;
						}
						case 58: {
							goto _ctr135;
						}
						case 63: {
							goto _ctr136;
						}
						case 91: {
							goto _ctr133;
						}
						case 93: {
							goto _ctr133;
						}
						case 95: {
							goto _ctr136;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr133;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) < 97 ) {
							if ( 65 <= ( (*( p))) && ( (*( p))) <= 70 ) {
								goto _ctr132;
							}
						} else if ( ( (*( p))) > 102 ) {
							if ( ( (*( p))) <= 122 ) {
								goto _ctr136;
							}
						} else {
							goto _ctr131;
						}
					} else {
						goto _ctr131;
					}
					{
						goto _ctr134;
					}
					_ctr132:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 4624 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st79;
					_st79:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr133;
						}
						case 34: {
							goto _ctr133;
						}
						case 63: {
							goto _ctr138;
						}
						case 91: {
							goto _ctr133;
						}
						case 93: {
							goto _ctr133;
						}
						case 95: {
							goto _ctr138;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr133;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) < 97 ) {
							if ( 65 <= ( (*( p))) && ( (*( p))) <= 70 ) {
								goto _ctr132;
							}
						} else if ( ( (*( p))) > 102 ) {
							if ( ( (*( p))) <= 122 ) {
								goto _ctr138;
							}
						} else {
							goto _ctr137;
						}
					} else {
						goto _ctr137;
					}
					{
						goto _ctr134;
					}
					_ctr137:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 4689 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st80;
					_st80:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr133;
						}
						case 34: {
							goto _ctr133;
						}
						case 58: {
							goto _ctr133;
						}
						case 63: {
							goto _ctr138;
						}
						case 91: {
							goto _ctr133;
						}
						case 93: {
							goto _ctr133;
						}
						case 95: {
							goto _ctr138;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr133;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) < 97 ) {
							if ( 65 <= ( (*( p))) && ( (*( p))) <= 70 ) {
								goto _ctr132;
							}
						} else if ( ( (*( p))) > 102 ) {
							if ( ( (*( p))) <= 122 ) {
								goto _ctr138;
							}
						} else {
							goto _ctr137;
						}
					} else {
						goto _ctr137;
					}
					{
						goto _ctr134;
					}
					_ctr78:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 4757 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 4763 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st81;
					_st81:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr108;
						}
						case 34: {
							goto _ctr108;
						}
						case 46: {
							goto _ctr110;
						}
						case 47: {
							goto _st69;
						}
						case 58: {
							goto _ctr123;
						}
						case 63: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr108;
						}
						case 93: {
							goto _ctr108;
						}
						case 95: {
							goto _ctr122;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr108;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr122;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr78;
						}
					} else {
						goto _ctr108;
					}
					{
						goto _ctr109;
					}
					_st82:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 40: {
							goto _st83;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr140;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr84;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st84;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st82;
						}
					} else {
						goto _ctr85;
					}
					{
						goto _st47;
					}
					_st83:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof83;
					st_case_83:
					if ( ( (*( p))) == 32 ) {
						goto _st83;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st83;
					}
					{
						goto _ctr142;
					}
					_st84:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 40: {
							goto _st83;
						}
						case 58: {
							goto _ctr84;
						}
						case 63: {
							goto _ctr85;
						}
						case 91: {
							goto _ctr140;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _ctr85;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr84;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st84;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st82;
						}
					} else {
						goto _ctr85;
					}
					{
						goto _st47;
					}
					_st85:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof85;
					st_case_85:
					if ( ( (*( p))) == 32 ) {
						goto _st85;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st85;
					}
					{
						goto _ctr143;
					}
					_st86:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr84;
						}
						case 34: {
							goto _ctr84;
						}
						case 40: {
							goto _st87;
						}
						case 58: {
							goto _st50;
						}
						case 63: {
							goto _st74;
						}
						case 91: {
							goto _ctr84;
						}
						case 93: {
							goto _ctr84;
						}
						case 95: {
							goto _st86;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr84;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st86;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st86;
						}
					} else {
						goto _ctr84;
					}
					{
						goto _st47;
					}
					_st87:
					if ( p == eof ) {
						if ( cs >= 41 )
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
						goto _ctr145;
					}
					_ctr149:
					{
#line 63 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 63 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 88; top+= 1; goto _st44;}}
						}}
					
#line 5062 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr151:
					{
#line 64 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 64 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 88; top+= 1; goto _st42;}}
						}}
					
#line 5073 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr153:
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 56 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 5084 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr160:
					{
#line 1 "NONE"
						{switch( act ) {
								case 33:  {
									p = ((te))-1;
									{
#line 51 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 34:  {
									p = ((te))-1;
									{
#line 52 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 35:  {
									p = ((te))-1;
									{
#line 53 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 45:  {
									p = ((te))-1;
									{
#line 117 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 5122 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr161:
					{
#line 228 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 228 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 5133 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr162:
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 117 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 5144 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr176:
					{
#line 55 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 55 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 5155 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr178:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5167 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 57 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 57 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 5181 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr183:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5194 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 57 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 57 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 5208 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr187:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5220 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 57 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 57 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 5234 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr192:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5245 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 57 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 57 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 5259 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr195:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5270 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 57 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 57 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 5284 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr198:
					{
#line 79 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 79 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 88; top+= 1; goto _st46;}}
						}}
					
#line 5298 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr199:
					{
#line 72 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 72 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DATA_NAME, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 5315 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr200:
					{
#line 217 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 217 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); {stack[top] = 88; top+= 1; goto _st46;}}
						}}
					
#line 5326 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr202:
					{
#line 65 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 65 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 5343 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_st88:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 5357 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof88;
					st_case_88:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 5367 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _st90;
						}
						case 34: {
							goto _ctr149;
						}
						case 35: {
							goto _st91;
						}
						case 39: {
							goto _ctr151;
						}
						case 40: {
							goto _st102;
						}
						case 41: {
							goto _ctr153;
						}
						case 43: {
							goto _ctr154;
						}
						case 45: {
							goto _ctr154;
						}
						case 48: {
							goto _ctr155;
						}
						case 91: {
							goto _st118;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _st90;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st119;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st116;
						}
					} else {
						goto _ctr156;
					}
					{
						goto _ctr147;
					}
					_ctr147:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5424 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 5430 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_ctr169:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5438 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 52 "lex/lemon_data_string_parser.rl"
						{act = 34;}}
					
#line 5444 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_ctr172:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5452 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 53 "lex/lemon_data_string_parser.rl"
						{act = 35;}}
					
#line 5458 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_ctr175:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5466 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 51 "lex/lemon_data_string_parser.rl"
						{act = 33;}}
					
#line 5472 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_ctr179:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5480 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5490 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 5496 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_ctr184:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5504 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5515 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 5521 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_ctr188:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5529 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5539 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 5545 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_ctr193:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5553 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5562 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 5568 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_ctr196:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5576 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5585 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 5591 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_st89:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr160;
						}
						case 34: {
							goto _ctr160;
						}
						case 91: {
							goto _ctr160;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr160;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr160;
					}
					{
						goto _ctr147;
					}
					_st90:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _st90;
						}
						case 41: {
							goto _ctr153;
						}
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st90;
					}
					{
						goto _ctr161;
					}
					_st91:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
						case 102: {
							goto _st92;
						}
						case 110: {
							goto _st96;
						}
						case 116: {
							goto _st99;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st92:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
						case 97: {
							goto _st93;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st93:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
						case 108: {
							goto _st94;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st94:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
						case 115: {
							goto _st95;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st95:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
						case 101: {
							goto _ctr169;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st96:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
						case 117: {
							goto _st97;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st97:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
						case 108: {
							goto _st98;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st98:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
						case 108: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st99:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
						case 114: {
							goto _st100;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st100:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
						case 117: {
							goto _st101;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st101:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
						case 101: {
							goto _ctr175;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st102:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof102;
					st_case_102:
					if ( ( (*( p))) == 32 ) {
						goto _st102;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st102;
					}
					{
						goto _ctr176;
					}
					_ctr154:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 6067 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st103;
					_st103:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 48: {
							goto _ctr177;
						}
						case 91: {
							goto _ctr162;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr156;
						}
					} else {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_ctr177:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 6114 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 6120 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st104;
					_st104:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr178;
						}
						case 34: {
							goto _ctr178;
						}
						case 46: {
							goto _ctr180;
						}
						case 47: {
							goto _st107;
						}
						case 91: {
							goto _ctr178;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr178;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr178;
					}
					{
						goto _ctr179;
					}
					_ctr180:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6170 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 6179 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st105;
					_st105:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr182;
						}
					} else {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_ctr182:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 6226 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st106;
					_st106:
					if ( p == eof ) {
						if ( cs >= 41 )
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
						case 91: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr183;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr182;
						}
					} else {
						goto _ctr183;
					}
					{
						goto _ctr184;
					}
					_st107:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 48: {
							goto _ctr185;
						}
						case 91: {
							goto _ctr162;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr186;
						}
					} else {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_ctr185:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 6309 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st108;
					_st108:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr187;
						}
						case 34: {
							goto _ctr187;
						}
						case 91: {
							goto _ctr187;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr187;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr187;
					}
					{
						goto _ctr188;
					}
					_ctr186:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 6349 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_st109:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr187;
						}
						case 34: {
							goto _ctr187;
						}
						case 91: {
							goto _ctr187;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr187;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr186;
						}
					} else {
						goto _ctr187;
					}
					{
						goto _ctr188;
					}
					_ctr156:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 6393 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 6399 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_st110:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr178;
						}
						case 34: {
							goto _ctr178;
						}
						case 46: {
							goto _ctr180;
						}
						case 47: {
							goto _st107;
						}
						case 91: {
							goto _ctr178;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr178;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr156;
						}
					} else {
						goto _ctr178;
					}
					{
						goto _ctr179;
					}
					_ctr155:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 6449 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 6455 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_st111:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr178;
						}
						case 34: {
							goto _ctr178;
						}
						case 46: {
							goto _ctr180;
						}
						case 47: {
							goto _st107;
						}
						case 91: {
							goto _ctr178;
						}
						case 98: {
							goto _st112;
						}
						case 120: {
							goto _st114;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr178;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr178;
					}
					{
						goto _ctr179;
					}
					_st112:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
							goto _ctr191;
						}
					} else {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_ctr191:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 6543 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st113;
					_st113:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr192;
						}
						case 34: {
							goto _ctr192;
						}
						case 91: {
							goto _ctr192;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr192;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
							goto _ctr191;
						}
					} else {
						goto _ctr192;
					}
					{
						goto _ctr193;
					}
					_st114:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 91: {
							goto _ctr162;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr162;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 70 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
								goto _ctr194;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _ctr194;
						}
					} else {
						goto _ctr194;
					}
					{
						goto _ctr147;
					}
					_ctr194:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 6631 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st115;
					_st115:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr195;
						}
						case 34: {
							goto _ctr195;
						}
						case 91: {
							goto _ctr195;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr195;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr195;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 70 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
								goto _ctr194;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _ctr194;
						}
					} else {
						goto _ctr194;
					}
					{
						goto _ctr196;
					}
					_st116:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 40: {
							goto _st117;
						}
						case 91: {
							goto _ctr198;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st116;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st116;
						}
					} else {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st117:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof117;
					st_case_117:
					if ( ( (*( p))) == 32 ) {
						goto _st117;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st117;
					}
					{
						goto _ctr199;
					}
					_st118:
					if ( p == eof ) {
						if ( cs >= 41 )
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
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st118;
					}
					{
						goto _ctr200;
					}
					_st119:
					if ( p == eof ) {
						if ( cs >= 41 )
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
							goto _ctr162;
						}
						case 34: {
							goto _ctr162;
						}
						case 40: {
							goto _st120;
						}
						case 91: {
							goto _ctr162;
						}
						case 95: {
							goto _st119;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr162;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st119;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st119;
						}
					} else {
						goto _ctr162;
					}
					{
						goto _ctr147;
					}
					_st120:
					if ( p == eof ) {
						if ( cs >= 41 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof120;
					st_case_120:
					if ( ( (*( p))) == 32 ) {
						goto _st120;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st120;
					}
					{
						goto _ctr202;
					}
					st_out:
					_test_eof41: cs = 41; goto _test_eof; 
					_test_eof42: cs = 42; goto _test_eof; 
					_test_eof43: cs = 43; goto _test_eof; 
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
					_test_eof44: cs = 44; goto _test_eof; 
					_test_eof45: cs = 45; goto _test_eof; 
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
					_test_eof46: cs = 46; goto _test_eof; 
					_test_eof47: cs = 47; goto _test_eof; 
					_test_eof48: cs = 48; goto _test_eof; 
					_test_eof49: cs = 49; goto _test_eof; 
					_test_eof35: cs = 35; goto _test_eof; 
					_test_eof36: cs = 36; goto _test_eof; 
					_test_eof37: cs = 37; goto _test_eof; 
					_test_eof50: cs = 50; goto _test_eof; 
					_test_eof38: cs = 38; goto _test_eof; 
					_test_eof39: cs = 39; goto _test_eof; 
					_test_eof40: cs = 40; goto _test_eof; 
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
					
					_test_eof: {}
					if ( p == eof ) {
						switch ( cs ) {
							case 41: {
								break;
							}
							case 42: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 6960 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 43: {
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
							case 44: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 7026 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 45: {
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
							case 46: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 7089 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
							case 35: {
								break;
							}
							case 36: {
								break;
							}
							case 37: {
								break;
							}
							case 50: {
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
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 7239 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
						}
						switch ( cs ) {
						}
						switch ( cs ) {
							case 41:
							goto _st41;case 42:
							goto _st42;case 43:
							goto _ctr60;case 1:
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
							goto _st0;case 44:
							goto _st44;case 45:
							goto _ctr66;case 18:
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
							goto _st34;case 46:
							goto _st46;case 47:
							goto _ctr84;case 48:
							goto _ctr49;case 49:
							goto _ctr86;case 35:
							goto _ctr47;case 36:
							goto _ctr49;case 37:
							goto _ctr49;case 50:
							goto _ctr89;case 38:
							goto _ctr47;case 39:
							goto _ctr49;case 40:
							goto _ctr47;case 51:
							goto _ctr90;case 52:
							goto _ctr84;case 53:
							goto _ctr84;case 54:
							goto _ctr84;case 55:
							goto _ctr84;case 56:
							goto _ctr84;case 57:
							goto _ctr84;case 58:
							goto _ctr84;case 59:
							goto _ctr84;case 60:
							goto _ctr84;case 61:
							goto _ctr84;case 62:
							goto _ctr84;case 63:
							goto _ctr104;case 64:
							goto _ctr105;case 65:
							goto _ctr84;case 66:
							goto _ctr108;case 67:
							goto _ctr84;case 68:
							goto _ctr114;case 69:
							goto _ctr84;case 70:
							goto _ctr119;case 71:
							goto _ctr119;case 72:
							goto _ctr108;case 73:
							goto _ctr108;case 74:
							goto _ctr84;case 75:
							goto _ctr84;case 76:
							goto _ctr127;case 77:
							goto _ctr84;case 78:
							goto _ctr133;case 79:
							goto _ctr133;case 80:
							goto _ctr133;case 81:
							goto _ctr108;case 82:
							goto _ctr84;case 83:
							goto _ctr142;case 84:
							goto _ctr84;case 85:
							goto _ctr143;case 86:
							goto _ctr84;case 87:
							goto _ctr145;case 88:
							goto _st88;case 89:
							goto _ctr160;case 90:
							goto _ctr161;case 91:
							goto _ctr162;case 92:
							goto _ctr162;case 93:
							goto _ctr162;case 94:
							goto _ctr162;case 95:
							goto _ctr162;case 96:
							goto _ctr162;case 97:
							goto _ctr162;case 98:
							goto _ctr162;case 99:
							goto _ctr162;case 100:
							goto _ctr162;case 101:
							goto _ctr162;case 102:
							goto _ctr176;case 103:
							goto _ctr162;case 104:
							goto _ctr178;case 105:
							goto _ctr162;case 106:
							goto _ctr183;case 107:
							goto _ctr162;case 108:
							goto _ctr187;case 109:
							goto _ctr187;case 110:
							goto _ctr178;case 111:
							goto _ctr178;case 112:
							goto _ctr162;case 113:
							goto _ctr192;case 114:
							goto _ctr162;case 115:
							goto _ctr195;case 116:
							goto _ctr162;case 117:
							goto _ctr199;case 118:
							goto _ctr200;case 119:
							goto _ctr162;case 120:
							goto _ctr202;	}
					}
					
					if ( cs >= 41 )
						goto _out; _pop: {}
					_out: {}
				}
				
#line 318 "lex/lemon_data_string_parser.rl"
				
				
			} catch(std::exception& e) {
				LIB_ERR << e.what();
				return false;
			}
			
			if (cs < 
#line 7481 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
			41
#line 325 "lex/lemon_data_string_parser.rl"
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
