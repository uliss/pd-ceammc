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


#line 192 "lex/lemon_data_string_parser.rl"


# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 57 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
		static const int lemon_data_string_lexer_start = 21;
		static const int lemon_data_string_lexer_first_final = 21;
		static const int lemon_data_string_lexer_error = 0;
		
		static const int lemon_data_string_lexer_en_scan_sqstring = 22;
		static const int lemon_data_string_lexer_en_scan_dqstring = 23;
		static const int lemon_data_string_lexer_en_scan_dict = 24;
		static const int lemon_data_string_lexer_en_scan_token = 95;
		static const int lemon_data_string_lexer_en_main = 21;
		
		
#line 199 "lex/lemon_data_string_parser.rl"
		
		
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
				
#line 277 "lex/lemon_data_string_parser.rl"
				
				
#line 161 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					if ( p == pe )
						goto _test_eof;
					goto _resume;
					
					_again: {}
					switch ( cs ) {
						case 21: goto _st21;
						case 22: goto _st22;
						case 1: goto _st1;
						case 0: goto _st0;
						case 23: goto _st23;
						case 2: goto _st2;
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
						case 35: goto _st35;
						case 36: goto _st36;
						case 3: goto _st3;
						case 4: goto _st4;
						case 5: goto _st5;
						case 37: goto _st37;
						case 6: goto _st6;
						case 7: goto _st7;
						case 8: goto _st8;
						case 38: goto _st38;
						case 39: goto _st39;
						case 40: goto _st40;
						case 41: goto _st41;
						case 42: goto _st42;
						case 9: goto _st9;
						case 43: goto _st43;
						case 10: goto _st10;
						case 44: goto _st44;
						case 45: goto _st45;
						case 46: goto _st46;
						case 47: goto _st47;
						case 11: goto _st11;
						case 48: goto _st48;
						case 12: goto _st12;
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
						case 13: goto _st13;
						case 71: goto _st71;
						case 72: goto _st72;
						case 73: goto _st73;
						case 74: goto _st74;
						case 14: goto _st14;
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
						case 15: goto _st15;
						case 110: goto _st110;
						case 16: goto _st16;
						case 111: goto _st111;
						case 112: goto _st112;
						case 113: goto _st113;
						case 114: goto _st114;
						case 17: goto _st17;
						case 115: goto _st115;
						case 18: goto _st18;
						case 116: goto _st116;
						case 117: goto _st117;
						case 118: goto _st118;
						case 119: goto _st119;
						case 120: goto _st120;
						case 121: goto _st121;
						case 122: goto _st122;
						case 123: goto _st123;
						case 124: goto _st124;
						case 19: goto _st19;
						case 125: goto _st125;
						case 126: goto _st126;
						case 127: goto _st127;
						case 128: goto _st128;
						case 20: goto _st20;
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
					}
					
					_resume: {}
					switch ( cs ) {
						case 21:
						goto st_case_21;
						case 22:
						goto st_case_22;
						case 1:
						goto st_case_1;
						case 0:
						goto st_case_0;
						case 23:
						goto st_case_23;
						case 2:
						goto st_case_2;
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
						case 35:
						goto st_case_35;
						case 36:
						goto st_case_36;
						case 3:
						goto st_case_3;
						case 4:
						goto st_case_4;
						case 5:
						goto st_case_5;
						case 37:
						goto st_case_37;
						case 6:
						goto st_case_6;
						case 7:
						goto st_case_7;
						case 8:
						goto st_case_8;
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
						case 9:
						goto st_case_9;
						case 43:
						goto st_case_43;
						case 10:
						goto st_case_10;
						case 44:
						goto st_case_44;
						case 45:
						goto st_case_45;
						case 46:
						goto st_case_46;
						case 47:
						goto st_case_47;
						case 11:
						goto st_case_11;
						case 48:
						goto st_case_48;
						case 12:
						goto st_case_12;
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
						case 13:
						goto st_case_13;
						case 71:
						goto st_case_71;
						case 72:
						goto st_case_72;
						case 73:
						goto st_case_73;
						case 74:
						goto st_case_74;
						case 14:
						goto st_case_14;
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
						case 15:
						goto st_case_15;
						case 110:
						goto st_case_110;
						case 16:
						goto st_case_16;
						case 111:
						goto st_case_111;
						case 112:
						goto st_case_112;
						case 113:
						goto st_case_113;
						case 114:
						goto st_case_114;
						case 17:
						goto st_case_17;
						case 115:
						goto st_case_115;
						case 18:
						goto st_case_18;
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
						case 19:
						goto st_case_19;
						case 125:
						goto st_case_125;
						case 126:
						goto st_case_126;
						case 127:
						goto st_case_127;
						case 128:
						goto st_case_128;
						case 20:
						goto st_case_20;
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
					}
					goto st_out;
					_ctr47:
					{
#line 191 "lex/lemon_data_string_parser.rl"
						{p = p - 1; } {stack[top] = 21; top+= 1; goto _st95;}}
					
#line 627 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_st21:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 641 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof21;
					st_case_21:
					if ( ( (*( p))) == 32 ) {
						goto _st21;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st21;
					}
					{
						goto _ctr47;
					}
					_ctr2:
					{
#line 87 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 87 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 664 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr3:
					{
#line 86 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 86 "lex/lemon_data_string_parser.rl"
								ragel_string += '\''; }
						}}
					
#line 675 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr4:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 88 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 686 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr5:
					{
#line 89 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 89 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 697 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr6:
					{
#line 90 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 90 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 708 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr49:
					{
#line 85 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 85 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 719 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr50:
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 91 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 730 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_st22:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 744 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof22;
					st_case_22:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 754 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 39: {
							goto _ctr50;
						}
						case 96: {
							goto _st1;
						}
					}
					{
						goto _ctr49;
					}
					_st1:
					if ( p == eof ) {
						if ( cs >= 21 )
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
					
#line 810 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st23;
					_ctr9:
					{
#line 97 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 97 "lex/lemon_data_string_parser.rl"
								ragel_string += '"'; }
						}}
					
#line 821 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st23;
					_ctr10:
					{
#line 99 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 99 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 832 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st23;
					_ctr11:
					{
#line 100 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 100 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 843 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st23;
					_ctr12:
					{
#line 101 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 101 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 854 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st23;
					_ctr52:
					{
#line 96 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 96 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 865 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st23;
					_ctr53:
					{
#line 102 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 102 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 876 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st23;
					_st23:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 890 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof23;
					st_case_23:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 900 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 34: {
							goto _ctr53;
						}
						case 96: {
							goto _st2;
						}
					}
					{
						goto _ctr52;
					}
					_st2:
					if ( p == eof ) {
						if ( cs >= 21 )
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
#line 144 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 144 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 953 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
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
										ragel_string.clear(); {stack[top] = 24; top+= 1; goto _st23;}}
									break; 
								}
								case 23:  {
									p = ((te))-1;
									{
#line 118 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 24; top+= 1; goto _st22;}}
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
								case 29:  {
									p = ((te))-1;
									{
#line 144 "lex/lemon_data_string_parser.rl"
										pushToken(TK_SPACE); }
									break; 
								}
								case 30:  {
									p = ((te))-1;
									{
#line 145 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 1031 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
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
					
#line 1048 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr27:
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 145 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 1060 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr28:
					{
#line 132 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 132 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 1074 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr30:
					{
#line 137 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 137 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 24; top+= 1; goto _st24;}}
						}}
					
#line 1088 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr31:
					{
#line 127 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 127 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te-1);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 1102 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr60:
					{
#line 114 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 114 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 1113 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr77:
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 145 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 1124 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr84:
					{
#line 144 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 144 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 1135 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr87:
					{
#line 116 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 116 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_CLOSE); }
						}}
					
#line 1146 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr88:
					{
#line 143 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 143 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_KEY); }
						}}
					
#line 1157 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr89:
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 117 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 24; top+= 1; goto _st23;}}
						}}
					
#line 1168 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr90:
					{
#line 118 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 118 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 24; top+= 1; goto _st22;}}
						}}
					
#line 1179 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr91:
					{
#line 113 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 113 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 1190 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr94:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1202 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 1216 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr97:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1229 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 1243 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr98:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1255 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 1269 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr103:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1280 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 1294 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr106:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1305 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 1319 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_ctr128:
					{
#line 115 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 115 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); }
						}}
					
#line 1330 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
					_st24:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 1344 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof24;
					st_case_24:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 1354 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr56;
						}
						case 34: {
							goto _ctr57;
						}
						case 39: {
							goto _ctr58;
						}
						case 40: {
							goto _st40;
						}
						case 41: {
							goto _ctr60;
						}
						case 43: {
							goto _ctr61;
						}
						case 45: {
							goto _ctr61;
						}
						case 48: {
							goto _ctr62;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _st73;
						}
						case 93: {
							goto _ctr67;
						}
						case 95: {
							goto _ctr64;
						}
						case 102: {
							goto _ctr69;
						}
						case 110: {
							goto _ctr70;
						}
						case 116: {
							goto _ctr71;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr56;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr63;
					}
					{
						goto _ctr55;
					}
					_ctr55:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1426 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 1432 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st25;
					_ctr67:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1440 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 116 "lex/lemon_data_string_parser.rl"
						{act = 21;}}
					
#line 1446 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st25;
					_ctr72:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1454 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 1460 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 1466 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st25;
					_st25:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr15;
						}
						case 34: {
							goto _ctr15;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr15;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st27;
						}
						case 110: {
							goto _st30;
						}
						case 116: {
							goto _st31;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr15;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr15;
						}
					} else {
						goto _ctr15;
					}
					{
						goto _ctr55;
					}
					_st26:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st27;
						}
						case 110: {
							goto _st30;
						}
						case 116: {
							goto _st31;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr77;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st27:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _st26;
						}
						case 97: {
							goto _st28;
						}
						case 102: {
							goto _st27;
						}
						case 110: {
							goto _st30;
						}
						case 116: {
							goto _st31;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr77;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st28:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st27;
						}
						case 108: {
							goto _st29;
						}
						case 110: {
							goto _st30;
						}
						case 116: {
							goto _st31;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr77;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st29:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st27;
						}
						case 110: {
							goto _st30;
						}
						case 115: {
							goto _st33;
						}
						case 116: {
							goto _st31;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr77;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st30:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st27;
						}
						case 110: {
							goto _st30;
						}
						case 116: {
							goto _st31;
						}
						case 117: {
							goto _st34;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr77;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st31:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st27;
						}
						case 110: {
							goto _st30;
						}
						case 114: {
							goto _st32;
						}
						case 116: {
							goto _st31;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr77;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st32:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st27;
						}
						case 110: {
							goto _st30;
						}
						case 116: {
							goto _st31;
						}
						case 117: {
							goto _st33;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr77;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st33:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _st26;
						}
						case 101: {
							goto _ctr77;
						}
						case 102: {
							goto _st27;
						}
						case 110: {
							goto _st30;
						}
						case 116: {
							goto _st31;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr77;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st34:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st27;
						}
						case 108: {
							goto _st35;
						}
						case 110: {
							goto _st30;
						}
						case 116: {
							goto _st31;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr77;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st35:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof35;
					st_case_35:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st27;
						}
						case 108: {
							goto _ctr77;
						}
						case 110: {
							goto _st30;
						}
						case 116: {
							goto _st31;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 58 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr77;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr56:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2172 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 144 "lex/lemon_data_string_parser.rl"
						{act = 29;}}
					
#line 2178 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st36;
					_st36:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr56;
						}
						case 34: {
							goto _st3;
						}
						case 39: {
							goto _st6;
						}
						case 41: {
							goto _ctr60;
						}
						case 63: {
							goto _st8;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _st8;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr56;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st8;
						}
					} else {
						goto _st8;
					}
					{
						goto _ctr84;
					}
					_st3:
					if ( p == eof ) {
						if ( cs >= 21 )
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
						if ( cs >= 21 )
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
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof5;
					st_case_5:
					if ( ( (*( p))) == 58 ) {
						goto _st37;
					}
					{
						goto _ctr15;
					}
					_ctr100:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2316 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st37;
					_ctr105:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2327 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st37;
					_ctr107:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2338 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st37;
					_st37:
					if ( p == eof ) {
						if ( cs >= 21 )
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
						goto _ctr88;
					}
					_st6:
					if ( p == eof ) {
						if ( cs >= 21 )
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
						if ( cs >= 21 )
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
					_ctr99:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2431 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st8;
					_ctr104:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2442 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st8;
					_ctr108:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2453 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st8;
					_st8:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _st37;
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
					_ctr57:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2493 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 22;}}
					
#line 2499 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st38;
					_st38:
					if ( p == eof ) {
						if ( cs >= 21 )
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
						goto _ctr89;
					}
					_ctr58:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2536 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 118 "lex/lemon_data_string_parser.rl"
						{act = 23;}}
					
#line 2542 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st39;
					_st39:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof39;
					st_case_39:
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
						goto _ctr90;
					}
					_st40:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof40;
					st_case_40:
					if ( ( (*( p))) == 32 ) {
						goto _st40;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st40;
					}
					{
						goto _ctr91;
					}
					_ctr61:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 2599 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st41;
					_st41:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof41;
					st_case_41:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 48: {
							goto _ctr92;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st27;
						}
						case 110: {
							goto _st30;
						}
						case 116: {
							goto _st31;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st26;
							}
						} else if ( ( (*( p))) >= 49 ) {
							goto _ctr93;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr92:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2671 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2677 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2683 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_st42:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr95;
						}
						case 47: {
							goto _st10;
						}
					}
					{
						goto _ctr94;
					}
					_ctr95:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2717 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 2726 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st9;
					_st9:
					if ( p == eof ) {
						if ( cs >= 21 )
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
					
#line 2754 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st43;
					_st43:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof43;
					st_case_43:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr21;
					}
					{
						goto _ctr97;
					}
					_st10:
					if ( p == eof ) {
						if ( cs >= 21 )
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
					
#line 2799 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_st44:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof44;
					st_case_44:
					{
						goto _ctr98;
					}
					_ctr23:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 2821 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st45;
					_st45:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof45;
					st_case_45:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr23;
					}
					{
						goto _ctr98;
					}
					_ctr93:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2846 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2852 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2858 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_st46:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof46;
					st_case_46:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr95;
						}
						case 47: {
							goto _st10;
						}
					}
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr93;
					}
					{
						goto _ctr94;
					}
					_ctr62:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2891 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2897 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2903 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 2909 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_st47:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof47;
					st_case_47:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr95;
						}
						case 47: {
							goto _st10;
						}
						case 58: {
							goto _ctr100;
						}
						case 63: {
							goto _ctr99;
						}
						case 95: {
							goto _ctr99;
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
							goto _ctr99;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _ctr99;
					}
					{
						goto _ctr94;
					}
					_st11:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _st37;
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
					
#line 2997 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 3003 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 3009 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
					_st48:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof48;
					st_case_48:
					switch( ( (*( p))) ) {
						case 58: {
							goto _ctr105;
						}
						case 63: {
							goto _ctr104;
						}
						case 95: {
							goto _ctr104;
						}
					}
					if ( ( (*( p))) < 50 ) {
						if ( 48 <= ( (*( p))) ) {
							goto _ctr24;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr104;
						}
					} else {
						goto _ctr104;
					}
					{
						goto _ctr103;
					}
					_st12:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _st37;
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
					
#line 3093 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 3099 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 3105 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st49;
					_st49:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof49;
					st_case_49:
					switch( ( (*( p))) ) {
						case 58: {
							goto _ctr107;
						}
						case 63: {
							goto _ctr108;
						}
						case 95: {
							goto _ctr108;
						}
					}
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr25;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( ( (*( p))) > 102 ) {
							if ( ( (*( p))) <= 122 ) {
								goto _ctr108;
							}
						} else if ( ( (*( p))) >= 97 ) {
							goto _ctr25;
						}
					} else {
						goto _ctr26;
					}
					{
						goto _ctr106;
					}
					_ctr26:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 3153 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_st50:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof50;
					st_case_50:
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
						goto _ctr106;
					}
					_ctr63:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3186 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 3192 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 3198 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 3204 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st51;
					_st51:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof51;
					st_case_51:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr95;
						}
						case 47: {
							goto _st10;
						}
						case 58: {
							goto _ctr100;
						}
						case 63: {
							goto _ctr99;
						}
						case 95: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr99;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _ctr63;
					}
					{
						goto _ctr94;
					}
					_ctr64:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3250 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 3256 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_st52:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr64;
						}
						case 102: {
							goto _ctr109;
						}
						case 110: {
							goto _ctr110;
						}
						case 116: {
							goto _ctr111;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr64;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr109:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3328 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 3334 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st53;
					_st53:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr64;
						}
						case 97: {
							goto _ctr112;
						}
						case 102: {
							goto _ctr109;
						}
						case 110: {
							goto _ctr110;
						}
						case 116: {
							goto _ctr111;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr64;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr112:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3409 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 3415 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_st54:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr64;
						}
						case 102: {
							goto _ctr109;
						}
						case 108: {
							goto _ctr113;
						}
						case 110: {
							goto _ctr110;
						}
						case 116: {
							goto _ctr111;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr64;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr113:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3490 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 3496 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_st55:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr64;
						}
						case 102: {
							goto _ctr109;
						}
						case 110: {
							goto _ctr110;
						}
						case 115: {
							goto _ctr114;
						}
						case 116: {
							goto _ctr111;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr64;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr110:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3571 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 3577 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st56;
					_st56:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr64;
						}
						case 102: {
							goto _ctr109;
						}
						case 110: {
							goto _ctr110;
						}
						case 116: {
							goto _ctr111;
						}
						case 117: {
							goto _ctr115;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr64;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr111:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3652 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 3658 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st57;
					_st57:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr64;
						}
						case 102: {
							goto _ctr109;
						}
						case 110: {
							goto _ctr110;
						}
						case 114: {
							goto _ctr116;
						}
						case 116: {
							goto _ctr111;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr64;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr116:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3733 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 3739 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st58;
					_st58:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr64;
						}
						case 102: {
							goto _ctr109;
						}
						case 110: {
							goto _ctr110;
						}
						case 116: {
							goto _ctr111;
						}
						case 117: {
							goto _ctr114;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr64;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr114:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3814 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 3820 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st59;
					_st59:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr64;
						}
						case 101: {
							goto _st8;
						}
						case 102: {
							goto _ctr109;
						}
						case 110: {
							goto _ctr110;
						}
						case 116: {
							goto _ctr111;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr64;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr115:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3895 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 3901 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st60;
					_st60:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr64;
						}
						case 102: {
							goto _ctr109;
						}
						case 108: {
							goto _ctr117;
						}
						case 110: {
							goto _ctr110;
						}
						case 116: {
							goto _ctr111;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr64;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr117:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3976 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 3982 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st61;
					_st61:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr64;
						}
						case 102: {
							goto _ctr109;
						}
						case 108: {
							goto _st8;
						}
						case 110: {
							goto _ctr110;
						}
						case 116: {
							goto _ctr111;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr64;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st62:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr28;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr30;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st64;
						}
						case 110: {
							goto _st67;
						}
						case 116: {
							goto _st68;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr77;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st63;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st63:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr28;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr30;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st64;
						}
						case 110: {
							goto _st67;
						}
						case 116: {
							goto _st68;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr77;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 58 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st63;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st64:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr28;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr30;
						}
						case 95: {
							goto _st26;
						}
						case 97: {
							goto _st65;
						}
						case 102: {
							goto _st64;
						}
						case 110: {
							goto _st67;
						}
						case 116: {
							goto _st68;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr77;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 58 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st63;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st65:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr28;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr30;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st64;
						}
						case 108: {
							goto _st66;
						}
						case 110: {
							goto _st67;
						}
						case 116: {
							goto _st68;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr77;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 58 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st63;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st66:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr28;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr30;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st64;
						}
						case 110: {
							goto _st67;
						}
						case 115: {
							goto _ctr124;
						}
						case 116: {
							goto _st68;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr77;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 58 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st63;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st67:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr28;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr30;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st64;
						}
						case 110: {
							goto _st67;
						}
						case 116: {
							goto _st68;
						}
						case 117: {
							goto _st71;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr77;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 58 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st63;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st68:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr28;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr30;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st64;
						}
						case 110: {
							goto _st67;
						}
						case 114: {
							goto _st69;
						}
						case 116: {
							goto _st68;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr77;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 58 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st63;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st69:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr28;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr30;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st64;
						}
						case 110: {
							goto _st67;
						}
						case 116: {
							goto _st68;
						}
						case 117: {
							goto _ctr124;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr77;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 58 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st63;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr124:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4619 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st70;
					_st70:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr28;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr30;
						}
						case 95: {
							goto _st26;
						}
						case 101: {
							goto _st13;
						}
						case 102: {
							goto _st64;
						}
						case 110: {
							goto _st67;
						}
						case 116: {
							goto _st68;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr77;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 58 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st63;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st13:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof13;
					st_case_13:
					switch( ( (*( p))) ) {
						case 40: {
							goto _ctr28;
						}
						case 91: {
							goto _ctr30;
						}
					}
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st13;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st13;
					}
					{
						goto _ctr27;
					}
					_st71:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr28;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr30;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st64;
						}
						case 108: {
							goto _ctr127;
						}
						case 110: {
							goto _st67;
						}
						case 116: {
							goto _st68;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr77;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 58 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st63;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr127:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4798 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_st72:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr28;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 63: {
							goto _st26;
						}
						case 91: {
							goto _ctr30;
						}
						case 95: {
							goto _st26;
						}
						case 102: {
							goto _st64;
						}
						case 108: {
							goto _st13;
						}
						case 110: {
							goto _st67;
						}
						case 116: {
							goto _st68;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr77;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 58 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st63;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st62;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st73:
					if ( p == eof ) {
						if ( cs >= 21 )
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
						goto _ctr128;
					}
					_ctr68:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4897 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 4903 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st74;
					_st74:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_st14:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof14;
					st_case_14:
					switch( ( (*( p))) ) {
						case 40: {
							goto _ctr31;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _st8;
						}
						case 95: {
							goto _st14;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st14;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st14;
					}
					{
						goto _ctr15;
					}
					_ctr129:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5013 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5019 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st75;
					_st75:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 97: {
							goto _ctr132;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr132:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5097 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5103 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st76;
					_st76:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 108: {
							goto _ctr133;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr133:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5181 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5187 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st77;
					_st77:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 115: {
							goto _ctr134;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr130:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5265 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5271 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st78;
					_st78:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
						case 117: {
							goto _ctr135;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr131:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5349 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5355 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st79;
					_st79:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 114: {
							goto _ctr136;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr136:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5433 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5439 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st80;
					_st80:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
						case 117: {
							goto _ctr134;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr134:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5517 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5523 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st81;
					_st81:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 101: {
							goto _st14;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr135:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5601 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5607 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st82;
					_st82:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 108: {
							goto _ctr137;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr137:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5685 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5691 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st83;
					_st83:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 108: {
							goto _st14;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr69:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5769 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5775 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st84;
					_st84:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 97: {
							goto _ctr138;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr138:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5853 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5859 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st85;
					_st85:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 108: {
							goto _ctr139;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr139:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 5937 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 5943 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st86;
					_st86:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 115: {
							goto _ctr140;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr140:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6021 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 6027 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st87;
					_st87:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 101: {
							goto _ctr141;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr141:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6105 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 111 "lex/lemon_data_string_parser.rl"
						{act = 16;}}
					
#line 6111 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr144:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6119 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 112 "lex/lemon_data_string_parser.rl"
						{act = 17;}}
					
#line 6125 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_ctr147:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6133 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 110 "lex/lemon_data_string_parser.rl"
						{act = 15;}}
					
#line 6139 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st88;
					_st88:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof88;
					st_case_88:
					switch( ( (*( p))) ) {
						case 40: {
							goto _ctr31;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _st8;
						}
						case 95: {
							goto _st14;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st14;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st14;
					}
					{
						goto _ctr15;
					}
					_ctr70:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6182 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 6188 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st89;
					_st89:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
						case 117: {
							goto _ctr142;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr142:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6266 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 6272 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st90;
					_st90:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 108: {
							goto _ctr143;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr143:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6350 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 6356 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st91;
					_st91:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 108: {
							goto _ctr144;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr71:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6434 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 6440 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st92;
					_st92:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 114: {
							goto _ctr145;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr145:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6518 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 6524 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st93;
					_st93:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
						case 117: {
							goto _ctr146;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr146:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 6602 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{act = 30;}}
					
#line 6608 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st94;
					_st94:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr77;
						}
						case 34: {
							goto _ctr77;
						}
						case 40: {
							goto _ctr31;
						}
						case 43: {
							goto _ctr72;
						}
						case 45: {
							goto _ctr72;
						}
						case 58: {
							goto _st37;
						}
						case 63: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr77;
						}
						case 95: {
							goto _ctr68;
						}
						case 101: {
							goto _ctr147;
						}
						case 102: {
							goto _ctr129;
						}
						case 110: {
							goto _ctr130;
						}
						case 116: {
							goto _ctr131;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr77;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr77;
					}
					{
						goto _ctr55;
					}
					_ctr33:
					{
#line 163 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 163 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6695 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr39:
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 186 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 6707 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr40:
					{
#line 175 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 175 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 6721 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr42:
					{
#line 180 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 180 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 95; top+= 1; goto _st24;}}
						}}
					
#line 6735 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr43:
					{
#line 1 "NONE"
						{switch( act ) {
								case 32:  {
									p = ((te))-1;
									{
#line 153 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 33:  {
									p = ((te))-1;
									{
#line 154 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 34:  {
									p = ((te))-1;
									{
#line 155 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 38:  {
									p = ((te))-1;
									{
#line 159 "lex/lemon_data_string_parser.rl"
										pushToken(TK_DICT_CLOSE); }
									break; 
								}
								case 45:  {
									p = ((te))-1;
									{
#line 186 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 6780 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr44:
					{
#line 170 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 170 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te-1);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 6794 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr151:
					{
#line 160 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 160 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 95; top+= 1; goto _st23;}}
						}}
					
#line 6805 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr152:
					{
#line 161 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 161 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 95; top+= 1; goto _st22;}}
						}}
					
#line 6816 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr154:
					{
#line 157 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 157 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 6827 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr169:
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 186 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 6838 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr176:
					{
#line 188 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 188 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 6849 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr177:
					{
#line 159 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 159 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_CLOSE); }
						}}
					
#line 6860 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr178:
					{
#line 156 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 156 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 6871 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr180:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6883 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 163 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 163 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6897 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr183:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6910 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 163 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 163 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6924 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr184:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6936 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 163 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 163 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6950 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr187:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6961 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 163 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 163 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 6975 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr188:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 6986 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 163 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 163 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 7000 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_ctr198:
					{
#line 158 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 158 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); }
						}}
					
#line 7011 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st95;
					_st95:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 7025 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof95;
					st_case_95:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 7035 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _st106;
						}
						case 34: {
							goto _ctr151;
						}
						case 39: {
							goto _ctr152;
						}
						case 40: {
							goto _st107;
						}
						case 41: {
							goto _ctr154;
						}
						case 43: {
							goto _ctr155;
						}
						case 45: {
							goto _ctr155;
						}
						case 48: {
							goto _ctr156;
						}
						case 91: {
							goto _st127;
						}
						case 93: {
							goto _ctr160;
						}
						case 102: {
							goto _ctr162;
						}
						case 110: {
							goto _ctr163;
						}
						case 116: {
							goto _ctr164;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _st106;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st117;
						}
					} else {
						goto _ctr157;
					}
					{
						goto _ctr149;
					}
					_ctr149:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7101 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 7107 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_ctr160:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7115 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 159 "lex/lemon_data_string_parser.rl"
						{act = 38;}}
					
#line 7121 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_ctr165:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7129 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 7135 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 7141 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st96;
					_st96:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr43;
						}
						case 34: {
							goto _ctr43;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr43;
						}
						case 102: {
							goto _st97;
						}
						case 110: {
							goto _st100;
						}
						case 116: {
							goto _st101;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr43;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr43;
						}
					} else {
						goto _ctr43;
					}
					{
						goto _ctr149;
					}
					_st97:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 97: {
							goto _st98;
						}
						case 102: {
							goto _st97;
						}
						case 110: {
							goto _st100;
						}
						case 116: {
							goto _st101;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr169;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st98:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 102: {
							goto _st97;
						}
						case 108: {
							goto _st99;
						}
						case 110: {
							goto _st100;
						}
						case 116: {
							goto _st101;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr169;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st99:
					if ( p == eof ) {
						if ( cs >= 21 )
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
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 102: {
							goto _st97;
						}
						case 110: {
							goto _st100;
						}
						case 115: {
							goto _st103;
						}
						case 116: {
							goto _st101;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr169;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st100:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 102: {
							goto _st97;
						}
						case 110: {
							goto _st100;
						}
						case 116: {
							goto _st101;
						}
						case 117: {
							goto _st104;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr169;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st101:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 102: {
							goto _st97;
						}
						case 110: {
							goto _st100;
						}
						case 114: {
							goto _st102;
						}
						case 116: {
							goto _st101;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr169;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st102:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 102: {
							goto _st97;
						}
						case 110: {
							goto _st100;
						}
						case 116: {
							goto _st101;
						}
						case 117: {
							goto _st103;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr169;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st103:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 101: {
							goto _ctr169;
						}
						case 102: {
							goto _st97;
						}
						case 110: {
							goto _st100;
						}
						case 116: {
							goto _st101;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr169;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st104:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 102: {
							goto _st97;
						}
						case 108: {
							goto _st105;
						}
						case 110: {
							goto _st100;
						}
						case 116: {
							goto _st101;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr169;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st105:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 102: {
							goto _st97;
						}
						case 108: {
							goto _ctr169;
						}
						case 110: {
							goto _st100;
						}
						case 116: {
							goto _st101;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr169;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st106:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _st106;
						}
						case 41: {
							goto _ctr154;
						}
						case 93: {
							goto _ctr177;
						}
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st106;
					}
					{
						goto _ctr176;
					}
					_st107:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof107;
					st_case_107:
					if ( ( (*( p))) == 32 ) {
						goto _st107;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st107;
					}
					{
						goto _ctr178;
					}
					_ctr155:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 7734 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st108;
					_st108:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 48: {
							goto _ctr179;
						}
						case 91: {
							goto _ctr169;
						}
						case 102: {
							goto _st97;
						}
						case 110: {
							goto _st100;
						}
						case 116: {
							goto _st101;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr157;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr179:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7796 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 7802 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 7808 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st109;
					_st109:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof109;
					st_case_109:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr181;
						}
						case 47: {
							goto _st16;
						}
					}
					{
						goto _ctr180;
					}
					_ctr181:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 7842 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 7851 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st15;
					_st15:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof15;
					st_case_15:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr34;
					}
					{
						goto _ctr33;
					}
					_ctr34:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 7879 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st110;
					_st110:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof110;
					st_case_110:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr34;
					}
					{
						goto _ctr183;
					}
					_st16:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof16;
					st_case_16:
					if ( ( (*( p))) == 48 ) {
						goto _ctr35;
					}
					if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr36;
					}
					{
						goto _ctr33;
					}
					_ctr35:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 7924 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st111;
					_st111:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof111;
					st_case_111:
					{
						goto _ctr184;
					}
					_ctr36:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 7946 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st112;
					_st112:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof112;
					st_case_112:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr36;
					}
					{
						goto _ctr184;
					}
					_ctr157:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 7971 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 7977 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 7983 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st113;
					_st113:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof113;
					st_case_113:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr181;
						}
						case 47: {
							goto _st16;
						}
					}
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr157;
					}
					{
						goto _ctr180;
					}
					_ctr156:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8016 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 8022 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 8028 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st114;
					_st114:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof114;
					st_case_114:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr181;
						}
						case 47: {
							goto _st16;
						}
						case 98: {
							goto _st17;
						}
						case 120: {
							goto _st18;
						}
					}
					{
						goto _ctr180;
					}
					_st17:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof17;
					st_case_17:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr37;
					}
					{
						goto _ctr33;
					}
					_ctr37:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 8081 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st115;
					_st115:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof115;
					st_case_115:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr37;
					}
					{
						goto _ctr187;
					}
					_st18:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof18;
					st_case_18:
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr38;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr38;
						}
					} else {
						goto _ctr38;
					}
					{
						goto _ctr33;
					}
					_ctr38:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 8131 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st116;
					_st116:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof116;
					st_case_116:
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr38;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr38;
						}
					} else {
						goto _ctr38;
					}
					{
						goto _ctr188;
					}
					_st117:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr40;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr42;
						}
						case 102: {
							goto _st118;
						}
						case 110: {
							goto _st121;
						}
						case 116: {
							goto _st122;
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
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st117;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st117;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st118:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr40;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr42;
						}
						case 97: {
							goto _st119;
						}
						case 102: {
							goto _st118;
						}
						case 110: {
							goto _st121;
						}
						case 116: {
							goto _st122;
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
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st117;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st117;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st119:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr40;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr42;
						}
						case 102: {
							goto _st118;
						}
						case 108: {
							goto _st120;
						}
						case 110: {
							goto _st121;
						}
						case 116: {
							goto _st122;
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
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st117;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st117;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st120:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr40;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr42;
						}
						case 102: {
							goto _st118;
						}
						case 110: {
							goto _st121;
						}
						case 115: {
							goto _ctr194;
						}
						case 116: {
							goto _st122;
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
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st117;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st117;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st121:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr40;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr42;
						}
						case 102: {
							goto _st118;
						}
						case 110: {
							goto _st121;
						}
						case 116: {
							goto _st122;
						}
						case 117: {
							goto _st125;
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
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st117;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st117;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st122:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr40;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr42;
						}
						case 102: {
							goto _st118;
						}
						case 110: {
							goto _st121;
						}
						case 114: {
							goto _st123;
						}
						case 116: {
							goto _st122;
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
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st117;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st117;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st123:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr40;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr42;
						}
						case 102: {
							goto _st118;
						}
						case 110: {
							goto _st121;
						}
						case 116: {
							goto _st122;
						}
						case 117: {
							goto _ctr194;
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
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st117;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st117;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr194:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8616 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st124;
					_st124:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr40;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr42;
						}
						case 101: {
							goto _st19;
						}
						case 102: {
							goto _st118;
						}
						case 110: {
							goto _st121;
						}
						case 116: {
							goto _st122;
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
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st117;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st117;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st19:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof19;
					st_case_19:
					switch( ( (*( p))) ) {
						case 40: {
							goto _ctr40;
						}
						case 91: {
							goto _ctr42;
						}
					}
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st19;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st19;
					}
					{
						goto _ctr39;
					}
					_st125:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr40;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr42;
						}
						case 102: {
							goto _st118;
						}
						case 108: {
							goto _ctr197;
						}
						case 110: {
							goto _st121;
						}
						case 116: {
							goto _st122;
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
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st117;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st117;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr197:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8783 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st126;
					_st126:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr40;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr42;
						}
						case 102: {
							goto _st118;
						}
						case 108: {
							goto _st19;
						}
						case 110: {
							goto _st121;
						}
						case 116: {
							goto _st122;
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
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st117;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st117;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st127:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof127;
					st_case_127:
					if ( ( (*( p))) == 32 ) {
						goto _st127;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st127;
					}
					{
						goto _ctr198;
					}
					_ctr161:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8876 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 8882 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st128;
					_st128:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_st20:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof20;
					st_case_20:
					switch( ( (*( p))) ) {
						case 40: {
							goto _ctr44;
						}
						case 95: {
							goto _st20;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st20;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st20;
					}
					{
						goto _ctr43;
					}
					_ctr199:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 8980 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 8986 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st129;
					_st129:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 97: {
							goto _ctr202;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr202:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9058 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9064 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st130;
					_st130:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 108: {
							goto _ctr203;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr203:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9136 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9142 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st131;
					_st131:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 115: {
							goto _ctr204;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr200:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9214 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9220 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st132;
					_st132:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
						case 117: {
							goto _ctr205;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr201:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9292 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9298 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st133;
					_st133:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 114: {
							goto _ctr206;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr206:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9370 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9376 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st134;
					_st134:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
						case 117: {
							goto _ctr204;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr204:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9448 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9454 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st135;
					_st135:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 101: {
							goto _st20;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr205:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9526 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9532 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st136;
					_st136:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 108: {
							goto _ctr207;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr207:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9604 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9610 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st137;
					_st137:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 108: {
							goto _st20;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr162:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9682 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9688 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st138;
					_st138:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 97: {
							goto _ctr208;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr208:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9760 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9766 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st139;
					_st139:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 108: {
							goto _ctr209;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr209:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9838 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9844 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st140;
					_st140:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 115: {
							goto _ctr210;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr210:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9916 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 9922 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st141;
					_st141:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 101: {
							goto _ctr211;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr211:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 9994 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 154 "lex/lemon_data_string_parser.rl"
						{act = 33;}}
					
#line 10000 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st142;
					_ctr214:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 10008 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 155 "lex/lemon_data_string_parser.rl"
						{act = 34;}}
					
#line 10014 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st142;
					_ctr217:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 10022 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 153 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 10028 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st142;
					_st142:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof142;
					st_case_142:
					switch( ( (*( p))) ) {
						case 40: {
							goto _ctr44;
						}
						case 95: {
							goto _st20;
						}
					}
					if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st20;
						}
					} else if ( ( (*( p))) >= 48 ) {
						goto _st20;
					}
					{
						goto _ctr43;
					}
					_ctr163:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 10065 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 10071 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st143;
					_st143:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
						case 117: {
							goto _ctr212;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr212:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 10143 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 10149 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st144;
					_st144:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 108: {
							goto _ctr213;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr213:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 10221 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 10227 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st145;
					_st145:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 108: {
							goto _ctr214;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr164:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 10299 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 10305 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st146;
					_st146:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 114: {
							goto _ctr215;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr215:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 10377 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 10383 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st147;
					_st147:
					if ( p == eof ) {
						if ( cs >= 21 )
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
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
						case 117: {
							goto _ctr216;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					_ctr216:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 10455 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 186 "lex/lemon_data_string_parser.rl"
						{act = 45;}}
					
#line 10461 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st148;
					_st148:
					if ( p == eof ) {
						if ( cs >= 21 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof148;
					st_case_148:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr169;
						}
						case 34: {
							goto _ctr169;
						}
						case 40: {
							goto _ctr44;
						}
						case 43: {
							goto _ctr165;
						}
						case 45: {
							goto _ctr165;
						}
						case 91: {
							goto _ctr169;
						}
						case 95: {
							goto _ctr161;
						}
						case 101: {
							goto _ctr217;
						}
						case 102: {
							goto _ctr199;
						}
						case 110: {
							goto _ctr200;
						}
						case 116: {
							goto _ctr201;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr169;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr161;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st20;
						}
					} else {
						goto _ctr169;
					}
					{
						goto _ctr149;
					}
					st_out:
					_test_eof21: cs = 21; goto _test_eof; 
					_test_eof22: cs = 22; goto _test_eof; 
					_test_eof1: cs = 1; goto _test_eof; 
					_test_eof23: cs = 23; goto _test_eof; 
					_test_eof2: cs = 2; goto _test_eof; 
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
					_test_eof35: cs = 35; goto _test_eof; 
					_test_eof36: cs = 36; goto _test_eof; 
					_test_eof3: cs = 3; goto _test_eof; 
					_test_eof4: cs = 4; goto _test_eof; 
					_test_eof5: cs = 5; goto _test_eof; 
					_test_eof37: cs = 37; goto _test_eof; 
					_test_eof6: cs = 6; goto _test_eof; 
					_test_eof7: cs = 7; goto _test_eof; 
					_test_eof8: cs = 8; goto _test_eof; 
					_test_eof38: cs = 38; goto _test_eof; 
					_test_eof39: cs = 39; goto _test_eof; 
					_test_eof40: cs = 40; goto _test_eof; 
					_test_eof41: cs = 41; goto _test_eof; 
					_test_eof42: cs = 42; goto _test_eof; 
					_test_eof9: cs = 9; goto _test_eof; 
					_test_eof43: cs = 43; goto _test_eof; 
					_test_eof10: cs = 10; goto _test_eof; 
					_test_eof44: cs = 44; goto _test_eof; 
					_test_eof45: cs = 45; goto _test_eof; 
					_test_eof46: cs = 46; goto _test_eof; 
					_test_eof47: cs = 47; goto _test_eof; 
					_test_eof11: cs = 11; goto _test_eof; 
					_test_eof48: cs = 48; goto _test_eof; 
					_test_eof12: cs = 12; goto _test_eof; 
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
					_test_eof13: cs = 13; goto _test_eof; 
					_test_eof71: cs = 71; goto _test_eof; 
					_test_eof72: cs = 72; goto _test_eof; 
					_test_eof73: cs = 73; goto _test_eof; 
					_test_eof74: cs = 74; goto _test_eof; 
					_test_eof14: cs = 14; goto _test_eof; 
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
					_test_eof15: cs = 15; goto _test_eof; 
					_test_eof110: cs = 110; goto _test_eof; 
					_test_eof16: cs = 16; goto _test_eof; 
					_test_eof111: cs = 111; goto _test_eof; 
					_test_eof112: cs = 112; goto _test_eof; 
					_test_eof113: cs = 113; goto _test_eof; 
					_test_eof114: cs = 114; goto _test_eof; 
					_test_eof17: cs = 17; goto _test_eof; 
					_test_eof115: cs = 115; goto _test_eof; 
					_test_eof18: cs = 18; goto _test_eof; 
					_test_eof116: cs = 116; goto _test_eof; 
					_test_eof117: cs = 117; goto _test_eof; 
					_test_eof118: cs = 118; goto _test_eof; 
					_test_eof119: cs = 119; goto _test_eof; 
					_test_eof120: cs = 120; goto _test_eof; 
					_test_eof121: cs = 121; goto _test_eof; 
					_test_eof122: cs = 122; goto _test_eof; 
					_test_eof123: cs = 123; goto _test_eof; 
					_test_eof124: cs = 124; goto _test_eof; 
					_test_eof19: cs = 19; goto _test_eof; 
					_test_eof125: cs = 125; goto _test_eof; 
					_test_eof126: cs = 126; goto _test_eof; 
					_test_eof127: cs = 127; goto _test_eof; 
					_test_eof128: cs = 128; goto _test_eof; 
					_test_eof20: cs = 20; goto _test_eof; 
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
					
					_test_eof: {}
					if ( p == eof ) {
						switch ( cs ) {
							case 21: {
								break;
							}
							case 22: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 10689 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 1: {
								break;
							}
							case 0: {
								break;
							}
							case 23: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 10704 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 2: {
								break;
							}
							case 24: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 10716 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
							case 35: {
								break;
							}
							case 36: {
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
							case 37: {
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
							case 9: {
								break;
							}
							case 43: {
								break;
							}
							case 10: {
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
							case 11: {
								break;
							}
							case 48: {
								break;
							}
							case 12: {
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
							case 13: {
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
							case 14: {
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
								
#line 10971 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
							case 15: {
								break;
							}
							case 110: {
								break;
							}
							case 16: {
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
							case 17: {
								break;
							}
							case 115: {
								break;
							}
							case 18: {
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
							case 19: {
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
							case 20: {
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
						}
						switch ( cs ) {
						}
						switch ( cs ) {
							case 21:
							goto _st21;case 22:
							goto _st22;case 1:
							goto _st1;case 0:
							goto _st0;case 23:
							goto _st23;case 2:
							goto _st2;case 24:
							goto _st24;case 25:
							goto _ctr15;case 26:
							goto _ctr77;case 27:
							goto _ctr77;case 28:
							goto _ctr77;case 29:
							goto _ctr77;case 30:
							goto _ctr77;case 31:
							goto _ctr77;case 32:
							goto _ctr77;case 33:
							goto _ctr77;case 34:
							goto _ctr77;case 35:
							goto _ctr77;case 36:
							goto _ctr84;case 3:
							goto _ctr13;case 4:
							goto _ctr15;case 5:
							goto _ctr15;case 37:
							goto _ctr88;case 6:
							goto _ctr13;case 7:
							goto _ctr15;case 8:
							goto _ctr15;case 38:
							goto _ctr89;case 39:
							goto _ctr90;case 40:
							goto _ctr91;case 41:
							goto _ctr77;case 42:
							goto _ctr94;case 9:
							goto _ctr20;case 43:
							goto _ctr97;case 10:
							goto _ctr20;case 44:
							goto _ctr98;case 45:
							goto _ctr98;case 46:
							goto _ctr94;case 47:
							goto _ctr94;case 11:
							goto _ctr20;case 48:
							goto _ctr103;case 12:
							goto _ctr20;case 49:
							goto _ctr106;case 50:
							goto _ctr106;case 51:
							goto _ctr94;case 52:
							goto _ctr77;case 53:
							goto _ctr77;case 54:
							goto _ctr77;case 55:
							goto _ctr77;case 56:
							goto _ctr77;case 57:
							goto _ctr77;case 58:
							goto _ctr77;case 59:
							goto _ctr77;case 60:
							goto _ctr77;case 61:
							goto _ctr77;case 62:
							goto _ctr77;case 63:
							goto _ctr77;case 64:
							goto _ctr77;case 65:
							goto _ctr77;case 66:
							goto _ctr77;case 67:
							goto _ctr77;case 68:
							goto _ctr77;case 69:
							goto _ctr77;case 70:
							goto _ctr77;case 13:
							goto _ctr27;case 71:
							goto _ctr77;case 72:
							goto _ctr77;case 73:
							goto _ctr128;case 74:
							goto _ctr77;case 14:
							goto _ctr15;case 75:
							goto _ctr77;case 76:
							goto _ctr77;case 77:
							goto _ctr77;case 78:
							goto _ctr77;case 79:
							goto _ctr77;case 80:
							goto _ctr77;case 81:
							goto _ctr77;case 82:
							goto _ctr77;case 83:
							goto _ctr77;case 84:
							goto _ctr77;case 85:
							goto _ctr77;case 86:
							goto _ctr77;case 87:
							goto _ctr77;case 88:
							goto _ctr15;case 89:
							goto _ctr77;case 90:
							goto _ctr77;case 91:
							goto _ctr77;case 92:
							goto _ctr77;case 93:
							goto _ctr77;case 94:
							goto _ctr77;case 95:
							goto _st95;case 96:
							goto _ctr43;case 97:
							goto _ctr169;case 98:
							goto _ctr169;case 99:
							goto _ctr169;case 100:
							goto _ctr169;case 101:
							goto _ctr169;case 102:
							goto _ctr169;case 103:
							goto _ctr169;case 104:
							goto _ctr169;case 105:
							goto _ctr169;case 106:
							goto _ctr176;case 107:
							goto _ctr178;case 108:
							goto _ctr169;case 109:
							goto _ctr180;case 15:
							goto _ctr33;case 110:
							goto _ctr183;case 16:
							goto _ctr33;case 111:
							goto _ctr184;case 112:
							goto _ctr184;case 113:
							goto _ctr180;case 114:
							goto _ctr180;case 17:
							goto _ctr33;case 115:
							goto _ctr187;case 18:
							goto _ctr33;case 116:
							goto _ctr188;case 117:
							goto _ctr169;case 118:
							goto _ctr169;case 119:
							goto _ctr169;case 120:
							goto _ctr169;case 121:
							goto _ctr169;case 122:
							goto _ctr169;case 123:
							goto _ctr169;case 124:
							goto _ctr169;case 19:
							goto _ctr39;case 125:
							goto _ctr169;case 126:
							goto _ctr169;case 127:
							goto _ctr198;case 128:
							goto _ctr169;case 20:
							goto _ctr43;case 129:
							goto _ctr169;case 130:
							goto _ctr169;case 131:
							goto _ctr169;case 132:
							goto _ctr169;case 133:
							goto _ctr169;case 134:
							goto _ctr169;case 135:
							goto _ctr169;case 136:
							goto _ctr169;case 137:
							goto _ctr169;case 138:
							goto _ctr169;case 139:
							goto _ctr169;case 140:
							goto _ctr169;case 141:
							goto _ctr169;case 142:
							goto _ctr43;case 143:
							goto _ctr169;case 144:
							goto _ctr169;case 145:
							goto _ctr169;case 146:
							goto _ctr169;case 147:
							goto _ctr169;case 148:
							goto _ctr169;	}
					}
					
					if ( cs >= 21 )
						goto _out; _pop: {}
					_out: {}
				}
				
#line 278 "lex/lemon_data_string_parser.rl"
				
				
			} catch(std::exception& e) {
				setErrorMsg(e.what());
				return false;
			}
			
			if (cs < 
#line 11322 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
			21
#line 285 "lex/lemon_data_string_parser.rl"
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
