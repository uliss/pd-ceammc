#line 1 "lex/lemon_data_string_parser.rl"
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


#line 176 "lex/lemon_data_string_parser.rl"


# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 57 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
		static const int lemon_data_string_lexer_start = 19;
		static const int lemon_data_string_lexer_first_final = 19;
		static const int lemon_data_string_lexer_error = 0;
		
		static const int lemon_data_string_lexer_en_scan_sqstring = 20;
		static const int lemon_data_string_lexer_en_scan_dqstring = 21;
		static const int lemon_data_string_lexer_en_scan_dict = 22;
		static const int lemon_data_string_lexer_en_scan_token = 72;
		static const int lemon_data_string_lexer_en_main = 19;
		
		
#line 183 "lex/lemon_data_string_parser.rl"
		
		
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
				
				
#line 149 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					cs = (int)lemon_data_string_lexer_start;
					top = 0;
					ts = 0;
					te = 0;
					act = 0;
				}
				
#line 261 "lex/lemon_data_string_parser.rl"
				
				
#line 161 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					if ( p == pe )
						goto _test_eof;
					goto _resume;
					
					_again: {}
					switch ( cs ) {
						case 19: goto _st19;
						case 20: goto _st20;
						case 1: goto _st1;
						case 0: goto _st0;
						case 21: goto _st21;
						case 2: goto _st2;
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
						case 3: goto _st3;
						case 4: goto _st4;
						case 5: goto _st5;
						case 35: goto _st35;
						case 6: goto _st6;
						case 7: goto _st7;
						case 8: goto _st8;
						case 36: goto _st36;
						case 37: goto _st37;
						case 38: goto _st38;
						case 39: goto _st39;
						case 40: goto _st40;
						case 9: goto _st9;
						case 41: goto _st41;
						case 10: goto _st10;
						case 42: goto _st42;
						case 43: goto _st43;
						case 44: goto _st44;
						case 45: goto _st45;
						case 11: goto _st11;
						case 46: goto _st46;
						case 12: goto _st12;
						case 47: goto _st47;
						case 48: goto _st48;
						case 49: goto _st49;
						case 50: goto _st50;
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
						case 13: goto _st13;
						case 87: goto _st87;
						case 14: goto _st14;
						case 88: goto _st88;
						case 89: goto _st89;
						case 90: goto _st90;
						case 91: goto _st91;
						case 15: goto _st15;
						case 92: goto _st92;
						case 16: goto _st16;
						case 93: goto _st93;
						case 94: goto _st94;
						case 95: goto _st95;
						case 96: goto _st96;
						case 97: goto _st97;
						case 98: goto _st98;
						case 99: goto _st99;
						case 100: goto _st100;
						case 101: goto _st101;
						case 17: goto _st17;
						case 102: goto _st102;
						case 103: goto _st103;
						case 104: goto _st104;
						case 105: goto _st105;
						case 18: goto _st18;
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
					}
					
					_resume: {}
					switch ( cs ) {
						case 19:
						goto st_case_19;
						case 20:
						goto st_case_20;
						case 1:
						goto st_case_1;
						case 0:
						goto st_case_0;
						case 21:
						goto st_case_21;
						case 2:
						goto st_case_2;
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
						case 3:
						goto st_case_3;
						case 4:
						goto st_case_4;
						case 5:
						goto st_case_5;
						case 35:
						goto st_case_35;
						case 6:
						goto st_case_6;
						case 7:
						goto st_case_7;
						case 8:
						goto st_case_8;
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
						case 9:
						goto st_case_9;
						case 41:
						goto st_case_41;
						case 10:
						goto st_case_10;
						case 42:
						goto st_case_42;
						case 43:
						goto st_case_43;
						case 44:
						goto st_case_44;
						case 45:
						goto st_case_45;
						case 11:
						goto st_case_11;
						case 46:
						goto st_case_46;
						case 12:
						goto st_case_12;
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
						case 13:
						goto st_case_13;
						case 87:
						goto st_case_87;
						case 14:
						goto st_case_14;
						case 88:
						goto st_case_88;
						case 89:
						goto st_case_89;
						case 90:
						goto st_case_90;
						case 91:
						goto st_case_91;
						case 15:
						goto st_case_15;
						case 92:
						goto st_case_92;
						case 16:
						goto st_case_16;
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
						case 17:
						goto st_case_17;
						case 102:
						goto st_case_102;
						case 103:
						goto st_case_103;
						case 104:
						goto st_case_104;
						case 105:
						goto st_case_105;
						case 18:
						goto st_case_18;
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
					}
					goto st_out;
					_ctr41:
					{
#line 175 "lex/lemon_data_string_parser.rl"
						{p = p - 1; } {stack[top] = 19; top+= 1; goto _st72;}}
					
#line 558 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st19;
					_st19:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 572 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof19;
					st_case_19:
					if ( ( (*( p))) == 32 ) {
						goto _st19;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st19;
					}
					{
						goto _ctr41;
					}
					_ctr2:
					{
#line 87 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 87 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 595 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st20;
					_ctr3:
					{
#line 86 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 86 "lex/lemon_data_string_parser.rl"
								ragel_string += '\''; }
						}}
					
#line 606 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st20;
					_ctr4:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 88 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 617 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st20;
					_ctr5:
					{
#line 89 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 89 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 628 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st20;
					_ctr6:
					{
#line 90 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 90 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 639 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st20;
					_ctr43:
					{
#line 85 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 85 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 650 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st20;
					_ctr44:
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 91 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 661 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st20;
					_st20:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 675 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof20;
					st_case_20:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 685 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 39: {
							goto _ctr44;
						}
						case 96: {
							goto _st1;
						}
					}
					{
						goto _ctr43;
					}
					_st1:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof1;
					st_case_1:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr2;
						}
						case 39: {
							goto _ctr3;
						}
						case 46: {
							goto _ctr4;
						}
						case 58: {
							goto _ctr5;
						}
						case 96: {
							goto _ctr6;
						}
					}
					{
						goto _st0;
					}
					st_case_0:
					_st0:
					cs = 0;
					goto _pop;
					_ctr8:
					{
#line 98 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 98 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 741 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr9:
					{
#line 97 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 97 "lex/lemon_data_string_parser.rl"
								ragel_string += '"'; }
						}}
					
#line 752 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr10:
					{
#line 99 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 99 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 763 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr11:
					{
#line 100 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 100 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 774 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr12:
					{
#line 101 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 101 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 785 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr46:
					{
#line 96 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 96 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 796 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr47:
					{
#line 102 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 102 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 807 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_st21:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 821 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof21;
					st_case_21:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 831 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 34: {
							goto _ctr47;
						}
						case 96: {
							goto _st2;
						}
					}
					{
						goto _ctr46;
					}
					_st2:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof2;
					st_case_2:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr8;
						}
						case 34: {
							goto _ctr9;
						}
						case 46: {
							goto _ctr10;
						}
						case 58: {
							goto _ctr11;
						}
						case 96: {
							goto _ctr12;
						}
					}
					{
						goto _st0;
					}
					_ctr13:
					{
#line 128 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 128 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 884 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr15:
					{
#line 1 "NONE"
						{switch( act ) {
								case 15:  {
									p = ((te))-1;
									{
#line 110 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 16:  {
									p = ((te))-1;
									{
#line 111 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 17:  {
									p = ((te))-1;
									{
#line 112 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 21:  {
									p = ((te))-1;
									{
#line 116 "lex/lemon_data_string_parser.rl"
										pushToken(TK_DICT_CLOSE); }
									break; 
								}
								case 22:  {
									p = ((te))-1;
									{
#line 117 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 22; top+= 1; goto _st21;}}
									break; 
								}
								case 23:  {
									p = ((te))-1;
									{
#line 118 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 22; top+= 1; goto _st20;}}
									break; 
								}
								case 24:  {
									p = ((te))-1;
									{
#line 120 "lex/lemon_data_string_parser.rl"
										
										onFloat(ragel_cat, ragel_type, ragel_num);
										ragel_num = {};
										ragel_cat = CAT_UNKNOWN;
										ragel_type = TYPE_UNKNOWN;
									}
									break; 
								}
								case 26:  {
									p = ((te))-1;
									{
#line 128 "lex/lemon_data_string_parser.rl"
										pushToken(TK_SPACE); }
									break; 
								}
								case 27:  {
									p = ((te))-1;
									{
#line 129 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 962 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr20:
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 120 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 979 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr54:
					{
#line 114 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 114 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 990 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr69:
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 129 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 1001 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr76:
					{
#line 128 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 128 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 1012 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr79:
					{
#line 127 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 127 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_KEY); }
						}}
					
#line 1023 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr80:
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 117 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 22; top+= 1; goto _st21;}}
						}}
					
#line 1034 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr81:
					{
#line 118 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 118 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 22; top+= 1; goto _st20;}}
						}}
					
#line 1045 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr82:
					{
#line 113 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 113 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 1056 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr85:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1068 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 120 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1082 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr88:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1095 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 120 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1109 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr89:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1121 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 120 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1135 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr94:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1146 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 120 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1160 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr97:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1171 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 120 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1185 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr109:
					{
#line 115 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 115 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); }
						}}
					
#line 1196 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_st22:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 1210 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof22;
					st_case_22:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 1220 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr50;
						}
						case 34: {
							goto _ctr51;
						}
						case 39: {
							goto _ctr52;
						}
						case 40: {
							goto _st38;
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
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _st60;
						}
						case 93: {
							goto _ctr60;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr61;
						}
						case 110: {
							goto _ctr62;
						}
						case 116: {
							goto _ctr63;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr50;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr58;
						}
					} else {
						goto _ctr57;
					}
					{
						goto _ctr49;
					}
					_ctr49:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1288 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 1294 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st23;
					_ctr60:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1302 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 116 "lex/lemon_data_string_parser.rl"
						{act = 21;}}
					
#line 1308 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st23;
					_ctr64:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1316 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 1322 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 1328 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st23;
					_st23:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof23;
					st_case_23:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr15;
						}
						case 34: {
							goto _ctr15;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr15;
						}
						case 95: {
							goto _st24;
						}
						case 102: {
							goto _st25;
						}
						case 110: {
							goto _st28;
						}
						case 116: {
							goto _st29;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr15;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr15;
						}
					} else {
						goto _ctr15;
					}
					{
						goto _ctr49;
					}
					_st24:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof24;
					st_case_24:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _st24;
						}
						case 102: {
							goto _st25;
						}
						case 110: {
							goto _st28;
						}
						case 116: {
							goto _st29;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr69;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_st25:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof25;
					st_case_25:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _st24;
						}
						case 97: {
							goto _st26;
						}
						case 102: {
							goto _st25;
						}
						case 110: {
							goto _st28;
						}
						case 116: {
							goto _st29;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr69;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_st26:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof26;
					st_case_26:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _st24;
						}
						case 102: {
							goto _st25;
						}
						case 108: {
							goto _st27;
						}
						case 110: {
							goto _st28;
						}
						case 116: {
							goto _st29;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr69;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_st27:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof27;
					st_case_27:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _st24;
						}
						case 102: {
							goto _st25;
						}
						case 110: {
							goto _st28;
						}
						case 115: {
							goto _st31;
						}
						case 116: {
							goto _st29;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr69;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_st28:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof28;
					st_case_28:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _st24;
						}
						case 102: {
							goto _st25;
						}
						case 110: {
							goto _st28;
						}
						case 116: {
							goto _st29;
						}
						case 117: {
							goto _st32;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr69;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_st29:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof29;
					st_case_29:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _st24;
						}
						case 102: {
							goto _st25;
						}
						case 110: {
							goto _st28;
						}
						case 114: {
							goto _st30;
						}
						case 116: {
							goto _st29;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr69;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_st30:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof30;
					st_case_30:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _st24;
						}
						case 102: {
							goto _st25;
						}
						case 110: {
							goto _st28;
						}
						case 116: {
							goto _st29;
						}
						case 117: {
							goto _st31;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr69;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_st31:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof31;
					st_case_31:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _st24;
						}
						case 101: {
							goto _ctr69;
						}
						case 102: {
							goto _st25;
						}
						case 110: {
							goto _st28;
						}
						case 116: {
							goto _st29;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr69;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_st32:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof32;
					st_case_32:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _st24;
						}
						case 102: {
							goto _st25;
						}
						case 108: {
							goto _st33;
						}
						case 110: {
							goto _st28;
						}
						case 116: {
							goto _st29;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr69;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_st33:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof33;
					st_case_33:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _st24;
						}
						case 102: {
							goto _st25;
						}
						case 108: {
							goto _ctr69;
						}
						case 110: {
							goto _st28;
						}
						case 116: {
							goto _st29;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr69;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr50:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2034 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 128 "lex/lemon_data_string_parser.rl"
						{act = 26;}}
					
#line 2040 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st34;
					_st34:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr50;
						}
						case 34: {
							goto _st3;
						}
						case 39: {
							goto _st6;
						}
						case 63: {
							goto _st8;
						}
						case 95: {
							goto _st8;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr50;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st8;
						}
					} else {
						goto _st8;
					}
					{
						goto _ctr76;
					}
					_st3:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof3;
					st_case_3:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st4;
						}
						case 95: {
							goto _st4;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st4;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st4;
					}
					{
						goto _ctr13;
					}
					_st4:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof4;
					st_case_4:
					switch( ( (*( p))) ) {
						case 34: {
							goto _st5;
						}
						case 63: {
							goto _st4;
						}
						case 95: {
							goto _st4;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st4;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st4;
					}
					{
						goto _ctr15;
					}
					_st5:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof5;
					st_case_5:
					if ( ( (*( p))) == 58 ) {
						goto _st35;
					}
					{
						goto _ctr15;
					}
					_ctr91:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2172 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_ctr96:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2183 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_ctr98:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2194 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_st35:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof35;
					st_case_35:
					if ( ( (*( p))) == 32 ) {
						goto _st35;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st35;
					}
					{
						goto _ctr79;
					}
					_st6:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof6;
					st_case_6:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st7;
						}
						case 95: {
							goto _st7;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st7;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st7;
					}
					{
						goto _ctr13;
					}
					_st7:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof7;
					st_case_7:
					switch( ( (*( p))) ) {
						case 39: {
							goto _st5;
						}
						case 63: {
							goto _st7;
						}
						case 95: {
							goto _st7;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st7;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st7;
					}
					{
						goto _ctr15;
					}
					_ctr90:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2287 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st8;
					_ctr95:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2298 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st8;
					_ctr99:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2309 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st8;
					_st8:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof8;
					st_case_8:
					switch( ( (*( p))) ) {
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _st8;
						}
						case 95: {
							goto _st8;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st8;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st8;
					}
					{
						goto _ctr15;
					}
					_ctr51:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2349 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 22;}}
					
#line 2355 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st36;
					_st36:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof36;
					st_case_36:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st4;
						}
						case 95: {
							goto _st4;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st4;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st4;
					}
					{
						goto _ctr80;
					}
					_ctr52:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2392 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 118 "lex/lemon_data_string_parser.rl"
						{act = 23;}}
					
#line 2398 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st37;
					_st37:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof37;
					st_case_37:
					switch( ( (*( p))) ) {
						case 63: {
							goto _st7;
						}
						case 95: {
							goto _st7;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st7;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st7;
					}
					{
						goto _ctr81;
					}
					_st38:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof38;
					st_case_38:
					if ( ( (*( p))) == 32 ) {
						goto _st38;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st38;
					}
					{
						goto _ctr82;
					}
					_ctr55:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 2455 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st39;
					_st39:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof39;
					st_case_39:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 48: {
							goto _ctr83;
						}
						case 63: {
							goto _st24;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _st24;
						}
						case 102: {
							goto _st25;
						}
						case 110: {
							goto _st28;
						}
						case 116: {
							goto _st29;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st24;
							}
						} else if ( ( (*( p))) >= 49 ) {
							goto _ctr84;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr83:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2527 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2533 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2539 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st40;
					_st40:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof40;
					st_case_40:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr86;
						}
						case 47: {
							goto _st10;
						}
					}
					{
						goto _ctr85;
					}
					_ctr86:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2573 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 2582 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st9;
					_st9:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof9;
					st_case_9:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr21;
					}
					{
						goto _ctr20;
					}
					_ctr21:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 2610 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st41;
					_st41:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof41;
					st_case_41:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr21;
					}
					{
						goto _ctr88;
					}
					_st10:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof10;
					st_case_10:
					if ( ( (*( p))) == 48 ) {
						goto _ctr22;
					}
					if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr23;
					}
					{
						goto _ctr20;
					}
					_ctr22:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 2655 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_st42:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof42;
					st_case_42:
					{
						goto _ctr89;
					}
					_ctr23:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 2677 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st43;
					_st43:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof43;
					st_case_43:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr23;
					}
					{
						goto _ctr89;
					}
					_ctr84:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2702 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2708 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2714 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_st44:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof44;
					st_case_44:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr86;
						}
						case 47: {
							goto _st10;
						}
					}
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr84;
					}
					{
						goto _ctr85;
					}
					_ctr56:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2747 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2753 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2759 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 2765 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st45;
					_st45:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof45;
					st_case_45:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr86;
						}
						case 47: {
							goto _st10;
						}
						case 58: {
							goto _ctr91;
						}
						case 63: {
							goto _ctr90;
						}
						case 95: {
							goto _ctr90;
						}
						case 98: {
							goto _st11;
						}
						case 120: {
							goto _st12;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr90;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _ctr90;
					}
					{
						goto _ctr85;
					}
					_st11:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof11;
					st_case_11:
					switch( ( (*( p))) ) {
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _st8;
						}
						case 95: {
							goto _st8;
						}
					}
					if ( ( (*( p))) < 50 ) {
						if ( 48 <= ( (*( p))) ) {
							goto _ctr24;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st8;
						}
					} else {
						goto _st8;
					}
					{
						goto _ctr20;
					}
					_ctr24:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2853 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 2859 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 2865 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_st46:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof46;
					st_case_46:
					switch( ( (*( p))) ) {
						case 58: {
							goto _ctr96;
						}
						case 63: {
							goto _ctr95;
						}
						case 95: {
							goto _ctr95;
						}
					}
					if ( ( (*( p))) < 50 ) {
						if ( 48 <= ( (*( p))) ) {
							goto _ctr24;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr95;
						}
					} else {
						goto _ctr95;
					}
					{
						goto _ctr94;
					}
					_st12:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof12;
					st_case_12:
					switch( ( (*( p))) ) {
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _st8;
						}
						case 95: {
							goto _st8;
						}
					}
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr25;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( ( (*( p))) > 102 ) {
							if ( ( (*( p))) <= 122 ) {
								goto _st8;
							}
						} else if ( ( (*( p))) >= 97 ) {
							goto _ctr25;
						}
					} else {
						goto _ctr26;
					}
					{
						goto _ctr20;
					}
					_ctr25:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2949 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 2955 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 2961 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_st47:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof47;
					st_case_47:
					switch( ( (*( p))) ) {
						case 58: {
							goto _ctr98;
						}
						case 63: {
							goto _ctr99;
						}
						case 95: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr25;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( ( (*( p))) > 102 ) {
							if ( ( (*( p))) <= 122 ) {
								goto _ctr99;
							}
						} else if ( ( (*( p))) >= 97 ) {
							goto _ctr25;
						}
					} else {
						goto _ctr26;
					}
					{
						goto _ctr97;
					}
					_ctr26:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 3009 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_st48:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof48;
					st_case_48:
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr26;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr26;
						}
					} else {
						goto _ctr26;
					}
					{
						goto _ctr97;
					}
					_ctr57:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3042 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 3048 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 3054 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 3060 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st49;
					_st49:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof49;
					st_case_49:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr86;
						}
						case 47: {
							goto _st10;
						}
						case 58: {
							goto _ctr91;
						}
						case 63: {
							goto _ctr90;
						}
						case 95: {
							goto _ctr90;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr90;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _ctr57;
					}
					{
						goto _ctr85;
					}
					_ctr58:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3106 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3112 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_st50:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr100:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3184 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3190 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st51;
					_st51:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 97: {
							goto _ctr103;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr103:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3265 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3271 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_st52:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 108: {
							goto _ctr104;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr104:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3346 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3352 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st53;
					_st53:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 115: {
							goto _ctr105;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr101:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3427 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3433 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_st54:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
						case 117: {
							goto _ctr106;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr102:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3508 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3514 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_st55:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 114: {
							goto _ctr107;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr107:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3589 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3595 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st56;
					_st56:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
						case 117: {
							goto _ctr105;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr105:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3670 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3676 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st57;
					_st57:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 101: {
							goto _st8;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr106:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3751 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3757 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st58;
					_st58:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 108: {
							goto _ctr108;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr108:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3832 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3838 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st59;
					_st59:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 108: {
							goto _st8;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_st60:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof60;
					st_case_60:
					if ( ( (*( p))) == 32 ) {
						goto _st60;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st60;
					}
					{
						goto _ctr109;
					}
					_ctr61:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3933 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 3939 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st61;
					_st61:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 97: {
							goto _ctr110;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr110:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4014 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 4020 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st62;
					_st62:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 108: {
							goto _ctr111;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr111:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4095 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 4101 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st63;
					_st63:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 115: {
							goto _ctr112;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr112:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4176 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 4182 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st64;
					_st64:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 101: {
							goto _ctr113;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr113:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4257 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 111 "lex/lemon_data_string_parser.rl"
						{act = 16;}}
					
#line 4263 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr116:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4271 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 112 "lex/lemon_data_string_parser.rl"
						{act = 17;}}
					
#line 4277 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_ctr119:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4285 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 110 "lex/lemon_data_string_parser.rl"
						{act = 15;}}
					
#line 4291 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_st65:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof65;
					st_case_65:
					switch( ( (*( p))) ) {
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _st8;
						}
						case 95: {
							goto _st8;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st8;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st8;
					}
					{
						goto _ctr15;
					}
					_ctr62:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4331 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 4337 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_st66:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
						case 117: {
							goto _ctr114;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr114:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4412 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 4418 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_st67:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 108: {
							goto _ctr115;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr115:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4493 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 4499 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_st68:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 108: {
							goto _ctr116;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr63:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4574 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 4580 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st69;
					_st69:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 114: {
							goto _ctr117;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr117:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4655 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 4661 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st70;
					_st70:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
						case 117: {
							goto _ctr118;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr118:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4736 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{act = 27;}}
					
#line 4742 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st71;
					_st71:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr69;
						}
						case 34: {
							goto _ctr69;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 58: {
							goto _st35;
						}
						case 63: {
							goto _ctr58;
						}
						case 91: {
							goto _ctr69;
						}
						case 95: {
							goto _ctr58;
						}
						case 101: {
							goto _ctr119;
						}
						case 102: {
							goto _ctr100;
						}
						case 110: {
							goto _ctr101;
						}
						case 116: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr69;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr58;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr69;
					}
					{
						goto _ctr49;
					}
					_ctr27:
					{
#line 147 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 147 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 4826 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr33:
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 170 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 4838 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr34:
					{
#line 159 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 159 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 4852 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr36:
					{
#line 164 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 164 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 72; top+= 1; goto _st22;}}
						}}
					
#line 4866 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr37:
					{
#line 1 "NONE"
						{switch( act ) {
								case 29:  {
									p = ((te))-1;
									{
#line 137 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 30:  {
									p = ((te))-1;
									{
#line 138 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 31:  {
									p = ((te))-1;
									{
#line 139 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 35:  {
									p = ((te))-1;
									{
#line 143 "lex/lemon_data_string_parser.rl"
										pushToken(TK_DICT_CLOSE); }
									break; 
								}
								case 42:  {
									p = ((te))-1;
									{
#line 170 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 4911 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr38:
					{
#line 154 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 154 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te-1);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 4925 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr123:
					{
#line 144 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 144 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 72; top+= 1; goto _st21;}}
						}}
					
#line 4936 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr124:
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 145 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 72; top+= 1; goto _st20;}}
						}}
					
#line 4947 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr126:
					{
#line 141 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 141 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 4958 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr141:
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 170 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 4969 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr148:
					{
#line 172 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 172 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 4980 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr149:
					{
#line 140 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 140 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 4991 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr151:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5003 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 147 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 147 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 5017 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr154:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5030 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 147 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 147 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 5044 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr155:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5056 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 147 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 147 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 5070 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr158:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5081 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 147 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 147 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 5095 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr159:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5106 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 147 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 147 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 5120 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_ctr169:
					{
#line 142 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 142 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); }
						}}
					
#line 5131 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_st72:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 5145 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof72;
					st_case_72:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 5155 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _st83;
						}
						case 34: {
							goto _ctr123;
						}
						case 39: {
							goto _ctr124;
						}
						case 40: {
							goto _st84;
						}
						case 41: {
							goto _ctr126;
						}
						case 43: {
							goto _ctr127;
						}
						case 45: {
							goto _ctr127;
						}
						case 48: {
							goto _ctr128;
						}
						case 91: {
							goto _st104;
						}
						case 93: {
							goto _ctr132;
						}
						case 102: {
							goto _ctr134;
						}
						case 110: {
							goto _ctr135;
						}
						case 116: {
							goto _ctr136;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _st83;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st94;
						}
					} else {
						goto _ctr129;
					}
					{
						goto _ctr121;
					}
					_ctr121:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5221 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 5227 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st73;
					_ctr132:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5235 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 143 "lex/lemon_data_string_parser.rl"
						{act = 35;}}
					
#line 5241 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st73;
					_ctr137:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5249 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 5255 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 5261 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st73;
					_st73:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr37;
						}
						case 34: {
							goto _ctr37;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr37;
						}
						case 102: {
							goto _st74;
						}
						case 110: {
							goto _st77;
						}
						case 116: {
							goto _st78;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr37;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr37;
						}
					} else {
						goto _ctr37;
					}
					{
						goto _ctr121;
					}
					_st74:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 97: {
							goto _st75;
						}
						case 102: {
							goto _st74;
						}
						case 110: {
							goto _st77;
						}
						case 116: {
							goto _st78;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr141;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st75:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 102: {
							goto _st74;
						}
						case 108: {
							goto _st76;
						}
						case 110: {
							goto _st77;
						}
						case 116: {
							goto _st78;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr141;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st76:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 102: {
							goto _st74;
						}
						case 110: {
							goto _st77;
						}
						case 115: {
							goto _st80;
						}
						case 116: {
							goto _st78;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr141;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st77:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 102: {
							goto _st74;
						}
						case 110: {
							goto _st77;
						}
						case 116: {
							goto _st78;
						}
						case 117: {
							goto _st81;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr141;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st78:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 102: {
							goto _st74;
						}
						case 110: {
							goto _st77;
						}
						case 114: {
							goto _st79;
						}
						case 116: {
							goto _st78;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr141;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st79:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 102: {
							goto _st74;
						}
						case 110: {
							goto _st77;
						}
						case 116: {
							goto _st78;
						}
						case 117: {
							goto _st80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr141;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st80:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 101: {
							goto _ctr141;
						}
						case 102: {
							goto _st74;
						}
						case 110: {
							goto _st77;
						}
						case 116: {
							goto _st78;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr141;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st81:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 102: {
							goto _st74;
						}
						case 108: {
							goto _st82;
						}
						case 110: {
							goto _st77;
						}
						case 116: {
							goto _st78;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr141;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st82:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 102: {
							goto _st74;
						}
						case 108: {
							goto _ctr141;
						}
						case 110: {
							goto _st77;
						}
						case 116: {
							goto _st78;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr141;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st83:
					if ( p == eof ) {
						if ( cs >= 19 )
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
						goto _ctr148;
					}
					_st84:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof84;
					st_case_84:
					if ( ( (*( p))) == 32 ) {
						goto _st84;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st84;
					}
					{
						goto _ctr149;
					}
					_ctr127:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 5846 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st85;
					_st85:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 48: {
							goto _ctr150;
						}
						case 91: {
							goto _ctr141;
						}
						case 102: {
							goto _st74;
						}
						case 110: {
							goto _st77;
						}
						case 116: {
							goto _st78;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr129;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr150:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5908 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5914 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5920 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st86;
					_st86:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof86;
					st_case_86:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr152;
						}
						case 47: {
							goto _st14;
						}
					}
					{
						goto _ctr151;
					}
					_ctr152:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 5954 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 5963 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st13;
					_st13:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof13;
					st_case_13:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr28;
					}
					{
						goto _ctr27;
					}
					_ctr28:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 5991 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st87;
					_st87:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof87;
					st_case_87:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr28;
					}
					{
						goto _ctr154;
					}
					_st14:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof14;
					st_case_14:
					if ( ( (*( p))) == 48 ) {
						goto _ctr29;
					}
					if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr30;
					}
					{
						goto _ctr27;
					}
					_ctr29:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 6036 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_st88:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof88;
					st_case_88:
					{
						goto _ctr155;
					}
					_ctr30:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 6058 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_st89:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof89;
					st_case_89:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr30;
					}
					{
						goto _ctr155;
					}
					_ctr129:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6083 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 6089 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 6095 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st90;
					_st90:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof90;
					st_case_90:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr152;
						}
						case 47: {
							goto _st14;
						}
					}
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr129;
					}
					{
						goto _ctr151;
					}
					_ctr128:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6128 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 6134 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 6140 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st91;
					_st91:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof91;
					st_case_91:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr152;
						}
						case 47: {
							goto _st14;
						}
						case 98: {
							goto _st15;
						}
						case 120: {
							goto _st16;
						}
					}
					{
						goto _ctr151;
					}
					_st15:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof15;
					st_case_15:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr31;
					}
					{
						goto _ctr27;
					}
					_ctr31:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 6193 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st92;
					_st92:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof92;
					st_case_92:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr31;
					}
					{
						goto _ctr158;
					}
					_st16:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof16;
					st_case_16:
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr32;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr32;
						}
					} else {
						goto _ctr32;
					}
					{
						goto _ctr27;
					}
					_ctr32:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 6243 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st93;
					_st93:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof93;
					st_case_93:
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr32;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr32;
						}
					} else {
						goto _ctr32;
					}
					{
						goto _ctr159;
					}
					_st94:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr34;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr36;
						}
						case 102: {
							goto _st95;
						}
						case 110: {
							goto _st98;
						}
						case 116: {
							goto _st99;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st94;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st94;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st95:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr34;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr36;
						}
						case 97: {
							goto _st96;
						}
						case 102: {
							goto _st95;
						}
						case 110: {
							goto _st98;
						}
						case 116: {
							goto _st99;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st94;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st94;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st96:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr34;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr36;
						}
						case 102: {
							goto _st95;
						}
						case 108: {
							goto _st97;
						}
						case 110: {
							goto _st98;
						}
						case 116: {
							goto _st99;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st94;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st94;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st97:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr34;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr36;
						}
						case 102: {
							goto _st95;
						}
						case 110: {
							goto _st98;
						}
						case 115: {
							goto _ctr165;
						}
						case 116: {
							goto _st99;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st94;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st94;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st98:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr34;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr36;
						}
						case 102: {
							goto _st95;
						}
						case 110: {
							goto _st98;
						}
						case 116: {
							goto _st99;
						}
						case 117: {
							goto _st102;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st94;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st94;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st99:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr34;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr36;
						}
						case 102: {
							goto _st95;
						}
						case 110: {
							goto _st98;
						}
						case 114: {
							goto _st100;
						}
						case 116: {
							goto _st99;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st94;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st94;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st100:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr34;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr36;
						}
						case 102: {
							goto _st95;
						}
						case 110: {
							goto _st98;
						}
						case 116: {
							goto _st99;
						}
						case 117: {
							goto _ctr165;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st94;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st94;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr165:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6728 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st101;
					_st101:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr34;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr36;
						}
						case 101: {
							goto _st17;
						}
						case 102: {
							goto _st95;
						}
						case 110: {
							goto _st98;
						}
						case 116: {
							goto _st99;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st94;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st94;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st17:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof17;
					st_case_17:
					switch( ( (*( p))) ) {
						case 40: {
							goto _ctr34;
						}
						case 91: {
							goto _ctr36;
						}
					}
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st17;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st17;
					}
					{
						goto _ctr33;
					}
					_st102:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr34;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr36;
						}
						case 102: {
							goto _st95;
						}
						case 108: {
							goto _ctr168;
						}
						case 110: {
							goto _st98;
						}
						case 116: {
							goto _st99;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st94;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st94;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr168:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6895 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st103;
					_st103:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr34;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr36;
						}
						case 102: {
							goto _st95;
						}
						case 108: {
							goto _st17;
						}
						case 110: {
							goto _st98;
						}
						case 116: {
							goto _st99;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr141;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st94;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st94;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st104:
					if ( p == eof ) {
						if ( cs >= 19 )
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
						goto _ctr169;
					}
					_ctr133:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6988 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 6994 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st105;
					_st105:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_st18:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof18;
					st_case_18:
					switch( ( (*( p))) ) {
						case 40: {
							goto _ctr38;
						}
						case 95: {
							goto _st18;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st18;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st18;
					}
					{
						goto _ctr37;
					}
					_ctr170:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7092 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7098 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st106;
					_st106:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 97: {
							goto _ctr173;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr173:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7170 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7176 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st107;
					_st107:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 108: {
							goto _ctr174;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr174:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7248 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7254 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st108;
					_st108:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 115: {
							goto _ctr175;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr171:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7326 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7332 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_st109:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
						case 117: {
							goto _ctr176;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr172:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7404 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7410 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_st110:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 114: {
							goto _ctr177;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr177:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7482 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7488 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_st111:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
						case 117: {
							goto _ctr175;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr175:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7560 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7566 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st112;
					_st112:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 101: {
							goto _st18;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr176:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7638 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7644 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st113;
					_st113:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 108: {
							goto _ctr178;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr178:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7716 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7722 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st114;
					_st114:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 108: {
							goto _st18;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr134:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7794 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7800 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st115;
					_st115:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 97: {
							goto _ctr179;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr179:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7872 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7878 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st116;
					_st116:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 108: {
							goto _ctr180;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr180:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7950 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 7956 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st117;
					_st117:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 115: {
							goto _ctr181;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr181:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8028 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 8034 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st118;
					_st118:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 101: {
							goto _ctr182;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr182:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8106 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 138 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 8112 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st119;
					_ctr185:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8120 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 139 "lex/lemon_data_string_parser.rl"
						{act = 31;}}
					
#line 8126 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st119;
					_ctr188:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8134 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 137 "lex/lemon_data_string_parser.rl"
						{act = 29;}}
					
#line 8140 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st119;
					_st119:
					if ( p == eof ) {
						if ( cs >= 19 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof119;
					st_case_119:
					switch( ( (*( p))) ) {
						case 40: {
							goto _ctr38;
						}
						case 95: {
							goto _st18;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st18;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st18;
					}
					{
						goto _ctr37;
					}
					_ctr135:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8177 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 8183 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st120;
					_st120:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
						case 117: {
							goto _ctr183;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr183:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8255 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 8261 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st121;
					_st121:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 108: {
							goto _ctr184;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr184:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8333 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 8339 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st122;
					_st122:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 108: {
							goto _ctr185;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr136:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8411 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 8417 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st123;
					_st123:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 114: {
							goto _ctr186;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr186:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8489 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 8495 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st124;
					_st124:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
						case 117: {
							goto _ctr187;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					_ctr187:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8567 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{act = 42;}}
					
#line 8573 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st125;
					_st125:
					if ( p == eof ) {
						if ( cs >= 19 )
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
							goto _ctr141;
						}
						case 34: {
							goto _ctr141;
						}
						case 40: {
							goto _ctr38;
						}
						case 43: {
							goto _ctr137;
						}
						case 45: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr141;
						}
						case 95: {
							goto _ctr133;
						}
						case 101: {
							goto _ctr188;
						}
						case 102: {
							goto _ctr170;
						}
						case 110: {
							goto _ctr171;
						}
						case 116: {
							goto _ctr172;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr141;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr133;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st18;
						}
					} else {
						goto _ctr141;
					}
					{
						goto _ctr121;
					}
					st_out:
					_test_eof19: cs = 19; goto _test_eof; 
					_test_eof20: cs = 20; goto _test_eof; 
					_test_eof1: cs = 1; goto _test_eof; 
					_test_eof21: cs = 21; goto _test_eof; 
					_test_eof2: cs = 2; goto _test_eof; 
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
					_test_eof3: cs = 3; goto _test_eof; 
					_test_eof4: cs = 4; goto _test_eof; 
					_test_eof5: cs = 5; goto _test_eof; 
					_test_eof35: cs = 35; goto _test_eof; 
					_test_eof6: cs = 6; goto _test_eof; 
					_test_eof7: cs = 7; goto _test_eof; 
					_test_eof8: cs = 8; goto _test_eof; 
					_test_eof36: cs = 36; goto _test_eof; 
					_test_eof37: cs = 37; goto _test_eof; 
					_test_eof38: cs = 38; goto _test_eof; 
					_test_eof39: cs = 39; goto _test_eof; 
					_test_eof40: cs = 40; goto _test_eof; 
					_test_eof9: cs = 9; goto _test_eof; 
					_test_eof41: cs = 41; goto _test_eof; 
					_test_eof10: cs = 10; goto _test_eof; 
					_test_eof42: cs = 42; goto _test_eof; 
					_test_eof43: cs = 43; goto _test_eof; 
					_test_eof44: cs = 44; goto _test_eof; 
					_test_eof45: cs = 45; goto _test_eof; 
					_test_eof11: cs = 11; goto _test_eof; 
					_test_eof46: cs = 46; goto _test_eof; 
					_test_eof12: cs = 12; goto _test_eof; 
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
					_test_eof13: cs = 13; goto _test_eof; 
					_test_eof87: cs = 87; goto _test_eof; 
					_test_eof14: cs = 14; goto _test_eof; 
					_test_eof88: cs = 88; goto _test_eof; 
					_test_eof89: cs = 89; goto _test_eof; 
					_test_eof90: cs = 90; goto _test_eof; 
					_test_eof91: cs = 91; goto _test_eof; 
					_test_eof15: cs = 15; goto _test_eof; 
					_test_eof92: cs = 92; goto _test_eof; 
					_test_eof16: cs = 16; goto _test_eof; 
					_test_eof93: cs = 93; goto _test_eof; 
					_test_eof94: cs = 94; goto _test_eof; 
					_test_eof95: cs = 95; goto _test_eof; 
					_test_eof96: cs = 96; goto _test_eof; 
					_test_eof97: cs = 97; goto _test_eof; 
					_test_eof98: cs = 98; goto _test_eof; 
					_test_eof99: cs = 99; goto _test_eof; 
					_test_eof100: cs = 100; goto _test_eof; 
					_test_eof101: cs = 101; goto _test_eof; 
					_test_eof17: cs = 17; goto _test_eof; 
					_test_eof102: cs = 102; goto _test_eof; 
					_test_eof103: cs = 103; goto _test_eof; 
					_test_eof104: cs = 104; goto _test_eof; 
					_test_eof105: cs = 105; goto _test_eof; 
					_test_eof18: cs = 18; goto _test_eof; 
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
					
					_test_eof: {}
					if ( p == eof ) {
						switch ( cs ) {
							case 19: {
								break;
							}
							case 20: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 8778 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 1: {
								break;
							}
							case 0: {
								break;
							}
							case 21: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 8793 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 2: {
								break;
							}
							case 22: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 8805 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
							case 3: {
								break;
							}
							case 4: {
								break;
							}
							case 5: {
								break;
							}
							case 35: {
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
							case 9: {
								break;
							}
							case 41: {
								break;
							}
							case 10: {
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
							case 11: {
								break;
							}
							case 46: {
								break;
							}
							case 12: {
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
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 8991 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
							case 13: {
								break;
							}
							case 87: {
								break;
							}
							case 14: {
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
							case 15: {
								break;
							}
							case 92: {
								break;
							}
							case 16: {
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
							case 17: {
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
							case 18: {
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
						}
						switch ( cs ) {
						}
						switch ( cs ) {
							case 19:
							goto _st19;case 20:
							goto _st20;case 1:
							goto _st1;case 0:
							goto _st0;case 21:
							goto _st21;case 2:
							goto _st2;case 22:
							goto _st22;case 23:
							goto _ctr15;case 24:
							goto _ctr69;case 25:
							goto _ctr69;case 26:
							goto _ctr69;case 27:
							goto _ctr69;case 28:
							goto _ctr69;case 29:
							goto _ctr69;case 30:
							goto _ctr69;case 31:
							goto _ctr69;case 32:
							goto _ctr69;case 33:
							goto _ctr69;case 34:
							goto _ctr76;case 3:
							goto _ctr13;case 4:
							goto _ctr15;case 5:
							goto _ctr15;case 35:
							goto _ctr79;case 6:
							goto _ctr13;case 7:
							goto _ctr15;case 8:
							goto _ctr15;case 36:
							goto _ctr80;case 37:
							goto _ctr81;case 38:
							goto _ctr82;case 39:
							goto _ctr69;case 40:
							goto _ctr85;case 9:
							goto _ctr20;case 41:
							goto _ctr88;case 10:
							goto _ctr20;case 42:
							goto _ctr89;case 43:
							goto _ctr89;case 44:
							goto _ctr85;case 45:
							goto _ctr85;case 11:
							goto _ctr20;case 46:
							goto _ctr94;case 12:
							goto _ctr20;case 47:
							goto _ctr97;case 48:
							goto _ctr97;case 49:
							goto _ctr85;case 50:
							goto _ctr69;case 51:
							goto _ctr69;case 52:
							goto _ctr69;case 53:
							goto _ctr69;case 54:
							goto _ctr69;case 55:
							goto _ctr69;case 56:
							goto _ctr69;case 57:
							goto _ctr69;case 58:
							goto _ctr69;case 59:
							goto _ctr69;case 60:
							goto _ctr109;case 61:
							goto _ctr69;case 62:
							goto _ctr69;case 63:
							goto _ctr69;case 64:
							goto _ctr69;case 65:
							goto _ctr15;case 66:
							goto _ctr69;case 67:
							goto _ctr69;case 68:
							goto _ctr69;case 69:
							goto _ctr69;case 70:
							goto _ctr69;case 71:
							goto _ctr69;case 72:
							goto _st72;case 73:
							goto _ctr37;case 74:
							goto _ctr141;case 75:
							goto _ctr141;case 76:
							goto _ctr141;case 77:
							goto _ctr141;case 78:
							goto _ctr141;case 79:
							goto _ctr141;case 80:
							goto _ctr141;case 81:
							goto _ctr141;case 82:
							goto _ctr141;case 83:
							goto _ctr148;case 84:
							goto _ctr149;case 85:
							goto _ctr141;case 86:
							goto _ctr151;case 13:
							goto _ctr27;case 87:
							goto _ctr154;case 14:
							goto _ctr27;case 88:
							goto _ctr155;case 89:
							goto _ctr155;case 90:
							goto _ctr151;case 91:
							goto _ctr151;case 15:
							goto _ctr27;case 92:
							goto _ctr158;case 16:
							goto _ctr27;case 93:
							goto _ctr159;case 94:
							goto _ctr141;case 95:
							goto _ctr141;case 96:
							goto _ctr141;case 97:
							goto _ctr141;case 98:
							goto _ctr141;case 99:
							goto _ctr141;case 100:
							goto _ctr141;case 101:
							goto _ctr141;case 17:
							goto _ctr33;case 102:
							goto _ctr141;case 103:
							goto _ctr141;case 104:
							goto _ctr169;case 105:
							goto _ctr141;case 18:
							goto _ctr37;case 106:
							goto _ctr141;case 107:
							goto _ctr141;case 108:
							goto _ctr141;case 109:
							goto _ctr141;case 110:
							goto _ctr141;case 111:
							goto _ctr141;case 112:
							goto _ctr141;case 113:
							goto _ctr141;case 114:
							goto _ctr141;case 115:
							goto _ctr141;case 116:
							goto _ctr141;case 117:
							goto _ctr141;case 118:
							goto _ctr141;case 119:
							goto _ctr37;case 120:
							goto _ctr141;case 121:
							goto _ctr141;case 122:
							goto _ctr141;case 123:
							goto _ctr141;case 124:
							goto _ctr141;case 125:
							goto _ctr141;	}
					}
					
					if ( cs >= 19 )
						goto _out; _pop: {}
					_out: {}
				}
				
#line 262 "lex/lemon_data_string_parser.rl"
				
				
			} catch(std::exception& e) {
				setErrorMsg(e.what());
				return false;
			}
			
			if (cs < 
#line 9319 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
			19
#line 269 "lex/lemon_data_string_parser.rl"
			) {
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
