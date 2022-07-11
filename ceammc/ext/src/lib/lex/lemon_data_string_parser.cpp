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


#line 202 "lex/lemon_data_string_parser.rl"


# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 57 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
		static const int lemon_data_string_lexer_start = 9;
		static const int lemon_data_string_lexer_first_final = 9;
		static const int lemon_data_string_lexer_error = 0;
		
		static const int lemon_data_string_lexer_en_scan_sqstring = 10;
		static const int lemon_data_string_lexer_en_scan_dqstring = 11;
		static const int lemon_data_string_lexer_en_scan_dict = 12;
		static const int lemon_data_string_lexer_en_scan_token = 54;
		static const int lemon_data_string_lexer_en_main = 9;
		
		
#line 209 "lex/lemon_data_string_parser.rl"
		
		
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
				
				
#line 149 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					cs = (int)lemon_data_string_lexer_start;
					top = 0;
					ts = 0;
					te = 0;
					act = 0;
				}
				
#line 287 "lex/lemon_data_string_parser.rl"
				
				
#line 161 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					if ( p == pe )
						goto _test_eof;
					goto _resume;
					
					_again: {}
					switch ( cs ) {
						case 9: goto _st9;
						case 10: goto _st10;
						case 1: goto _st1;
						case 0: goto _st0;
						case 11: goto _st11;
						case 2: goto _st2;
						case 12: goto _st12;
						case 13: goto _st13;
						case 14: goto _st14;
						case 15: goto _st15;
						case 3: goto _st3;
						case 4: goto _st4;
						case 5: goto _st5;
						case 16: goto _st16;
						case 6: goto _st6;
						case 7: goto _st7;
						case 8: goto _st8;
						case 17: goto _st17;
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
						case 35: goto _st35;
						case 36: goto _st36;
						case 37: goto _st37;
						case 38: goto _st38;
						case 39: goto _st39;
						case 40: goto _st40;
						case 41: goto _st41;
						case 42: goto _st42;
						case 43: goto _st43;
						case 44: goto _st44;
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
					}
					
					_resume: {}
					switch ( cs ) {
						case 9:
						goto st_case_9;
						case 10:
						goto st_case_10;
						case 1:
						goto st_case_1;
						case 0:
						goto st_case_0;
						case 11:
						goto st_case_11;
						case 2:
						goto st_case_2;
						case 12:
						goto st_case_12;
						case 13:
						goto st_case_13;
						case 14:
						goto st_case_14;
						case 15:
						goto st_case_15;
						case 3:
						goto st_case_3;
						case 4:
						goto st_case_4;
						case 5:
						goto st_case_5;
						case 16:
						goto st_case_16;
						case 6:
						goto st_case_6;
						case 7:
						goto st_case_7;
						case 8:
						goto st_case_8;
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
					}
					goto st_out;
					_ctr21:
					{
#line 201 "lex/lemon_data_string_parser.rl"
						{p = p - 1; } {stack[top] = 9; top+= 1; goto _st54;}}
					
#line 441 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st9;
					_st9:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 455 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof9;
					st_case_9:
					if ( ( (*( p))) == 32 ) {
						goto _st9;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st9;
					}
					{
						goto _ctr21;
					}
					_ctr2:
					{
#line 135 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 135 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 478 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st10;
					_ctr3:
					{
#line 134 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 134 "lex/lemon_data_string_parser.rl"
								ragel_string += '\''; }
						}}
					
#line 489 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st10;
					_ctr4:
					{
#line 136 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 136 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 500 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st10;
					_ctr5:
					{
#line 137 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 137 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 511 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st10;
					_ctr6:
					{
#line 138 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 138 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 522 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st10;
					_ctr23:
					{
#line 133 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 133 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 533 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st10;
					_ctr24:
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 129 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 544 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st10;
					_st10:
					if ( p == eof ) {
						if ( cs >= 9 )
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
						goto _test_eof10;
					st_case_10:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 568 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 39: {
							goto _ctr24;
						}
						case 96: {
							goto _st1;
						}
					}
					{
						goto _ctr23;
					}
					_st1:
					if ( p == eof ) {
						if ( cs >= 9 )
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
#line 146 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 146 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 624 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st11;
					_ctr9:
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 145 "lex/lemon_data_string_parser.rl"
								ragel_string += '"'; }
						}}
					
#line 635 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st11;
					_ctr10:
					{
#line 147 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 147 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 646 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st11;
					_ctr11:
					{
#line 148 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 148 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 657 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st11;
					_ctr12:
					{
#line 149 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 149 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 668 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st11;
					_ctr26:
					{
#line 144 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 144 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 679 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st11;
					_ctr27:
					{
#line 129 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 129 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 690 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st11;
					_st11:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 704 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof11;
					st_case_11:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 714 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 34: {
							goto _ctr27;
						}
						case 96: {
							goto _st2;
						}
					}
					{
						goto _ctr26;
					}
					_st2:
					if ( p == eof ) {
						if ( cs >= 9 )
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
#line 174 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 174 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 767 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr15:
					{
#line 1 "NONE"
						{switch( act ) {
								case 15:  {
									p = ((te))-1;
									{
#line 50 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 16:  {
									p = ((te))-1;
									{
#line 51 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 17:  {
									p = ((te))-1;
									{
#line 52 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 21:  {
									p = ((te))-1;
									{
#line 62 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 12; top+= 1; goto _st11;}}
									break; 
								}
								case 22:  {
									p = ((te))-1;
									{
#line 63 "lex/lemon_data_string_parser.rl"
										ragel_string.clear(); {stack[top] = 12; top+= 1; goto _st10;}}
									break; 
								}
								case 28:  {
									p = ((te))-1;
									{
#line 174 "lex/lemon_data_string_parser.rl"
										pushToken(TK_SPACE); }
									break; 
								}
								case 29:  {
									p = ((te))-1;
									{
#line 91 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 826 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr35:
					{
#line 55 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 55 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 837 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr42:
					{
#line 178 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 178 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_CLOSE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 848 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr44:
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 91 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 859 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr46:
					{
#line 174 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 174 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 870 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr49:
					{
#line 84 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 84 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DICT_KEY, ts, te0);
							}
						}}
					
#line 886 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr50:
					{
#line 62 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 62 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 12; top+= 1; goto _st11;}}
						}}
					
#line 897 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr64:
					{
#line 63 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 63 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 12; top+= 1; goto _st10;}}
						}}
					
#line 908 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr65:
					{
#line 54 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 54 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 919 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr68:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 931 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 56 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 945 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr74:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 958 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 56 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 972 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr79:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 984 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 56 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 998 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr87:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1009 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 56 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1023 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr93:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1034 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 56 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1048 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr100:
					{
#line 78 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 78 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 12; top+= 1; goto _st12;}}
						}}
					
#line 1062 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr102:
					{
#line 71 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 71 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DATA_NAME, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 1079 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr103:
					{
#line 83 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 83 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); }
						}}
					
#line 1090 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr105:
					{
#line 64 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 64 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 1107 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_st12:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 1121 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof12;
					st_case_12:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 1131 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr30;
						}
						case 34: {
							goto _ctr31;
						}
						case 35: {
							goto _st18;
						}
						case 39: {
							goto _ctr33;
						}
						case 40: {
							goto _st30;
						}
						case 41: {
							goto _ctr35;
						}
						case 43: {
							goto _ctr36;
						}
						case 45: {
							goto _ctr36;
						}
						case 48: {
							goto _ctr37;
						}
						case 63: {
							goto _st40;
						}
						case 91: {
							goto _st51;
						}
						case 93: {
							goto _ctr42;
						}
						case 95: {
							goto _st40;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr30;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st52;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st48;
						}
					} else {
						goto _ctr38;
					}
					{
						goto _st13;
					}
					_ctr69:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1201 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st13;
					_ctr75:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1214 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st13;
					_ctr80:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1226 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st13;
					_ctr88:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1237 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st13;
					_ctr94:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1248 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st13;
					_st13:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof13;
					st_case_13:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_ctr45:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1305 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 29;}}
					
#line 1311 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st14;
					_ctr57:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1319 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 51 "lex/lemon_data_string_parser.rl"
						{act = 16;}}
					
#line 1325 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st14;
					_ctr60:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1333 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 52 "lex/lemon_data_string_parser.rl"
						{act = 17;}}
					
#line 1339 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st14;
					_ctr63:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1347 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/lemon_data_string_parser.rl"
						{act = 15;}}
					
#line 1353 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st14;
					_ctr72:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1361 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1371 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 29;}}
					
#line 1377 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st14;
					_ctr76:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1385 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1396 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 29;}}
					
#line 1402 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st14;
					_ctr81:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1410 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1420 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 29;}}
					
#line 1426 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st14;
					_ctr98:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1434 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1443 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 29;}}
					
#line 1449 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st14;
					_st14:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof14;
					st_case_14:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr15;
						}
						case 34: {
							goto _ctr15;
						}
						case 58: {
							goto _ctr15;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr15;
						}
						case 93: {
							goto _ctr15;
						}
						case 95: {
							goto _ctr45;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr15;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr15;
					}
					{
						goto _st13;
					}
					_ctr30:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1509 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 174 "lex/lemon_data_string_parser.rl"
						{act = 28;}}
					
#line 1515 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st15;
					_st15:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof15;
					st_case_15:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr30;
						}
						case 34: {
							goto _st3;
						}
						case 39: {
							goto _st6;
						}
						case 41: {
							goto _ctr35;
						}
						case 63: {
							goto _st8;
						}
						case 93: {
							goto _ctr42;
						}
						case 95: {
							goto _st8;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr30;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st8;
						}
					} else {
						goto _st8;
					}
					{
						goto _ctr46;
					}
					_st3:
					if ( p == eof ) {
						if ( cs >= 9 )
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
						if ( cs >= 9 )
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
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof5;
					st_case_5:
					if ( ( (*( p))) == 58 ) {
						goto _st16;
					}
					{
						goto _ctr15;
					}
					_ctr83:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1653 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st16;
					_ctr90:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1664 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st16;
					_ctr95:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1675 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st16;
					_st16:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof16;
					st_case_16:
					if ( ( (*( p))) == 32 ) {
						goto _st16;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st16;
					}
					{
						goto _ctr49;
					}
					_st6:
					if ( p == eof ) {
						if ( cs >= 9 )
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
						if ( cs >= 9 )
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
					_st8:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _st16;
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
						goto _ctr13;
					}
					_ctr31:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1796 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 62 "lex/lemon_data_string_parser.rl"
						{act = 21;}}
					
#line 1802 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st17;
					_st17:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof17;
					st_case_17:
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
						goto _ctr50;
					}
					_st18:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof18;
					st_case_18:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
						case 102: {
							goto _st19;
						}
						case 110: {
							goto _st23;
						}
						case 116: {
							goto _st26;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st19:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof19;
					st_case_19:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
						case 97: {
							goto _st20;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st20:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof20;
					st_case_20:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
						case 108: {
							goto _st21;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st21:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof21;
					st_case_21:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
						case 115: {
							goto _st22;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st22:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof22;
					st_case_22:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
						case 101: {
							goto _ctr57;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st23:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
						case 117: {
							goto _st24;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st24:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
						case 108: {
							goto _st25;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st25:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
						case 108: {
							goto _ctr60;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st26:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
						case 114: {
							goto _st27;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st27:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
						case 117: {
							goto _st28;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st28:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
						case 101: {
							goto _ctr63;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr45;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_ctr33:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2447 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 63 "lex/lemon_data_string_parser.rl"
						{act = 22;}}
					
#line 2453 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st29;
					_st29:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof29;
					st_case_29:
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
						goto _ctr64;
					}
					_st30:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof30;
					st_case_30:
					if ( ( (*( p))) == 32 ) {
						goto _st30;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st30;
					}
					{
						goto _ctr65;
					}
					_ctr36:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 2510 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st31;
					_st31:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 48: {
							goto _ctr66;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 49 ) {
							goto _ctr67;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_ctr66:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2570 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2576 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st32;
					_st32:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 46: {
							goto _ctr70;
						}
						case 47: {
							goto _st35;
						}
						case 58: {
							goto _ctr68;
						}
						case 63: {
							goto _ctr72;
						}
						case 91: {
							goto _ctr68;
						}
						case 93: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr72;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr72;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr72;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr69;
					}
					_ctr70:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2646 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 2655 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st33;
					_st33:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr73;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_ctr73:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 2715 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st34;
					_st34:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr74;
						}
						case 34: {
							goto _ctr74;
						}
						case 58: {
							goto _ctr74;
						}
						case 63: {
							goto _ctr76;
						}
						case 91: {
							goto _ctr74;
						}
						case 93: {
							goto _ctr74;
						}
						case 95: {
							goto _ctr76;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr74;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr76;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr73;
						}
					} else {
						goto _ctr74;
					}
					{
						goto _ctr75;
					}
					_st35:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 48: {
							goto _ctr77;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr45;
							}
						} else if ( ( (*( p))) >= 49 ) {
							goto _ctr78;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_ctr77:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 2827 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st36;
					_st36:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr79;
						}
						case 34: {
							goto _ctr79;
						}
						case 58: {
							goto _ctr79;
						}
						case 63: {
							goto _ctr81;
						}
						case 91: {
							goto _ctr79;
						}
						case 93: {
							goto _ctr79;
						}
						case 95: {
							goto _ctr81;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr79;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr81;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr81;
						}
					} else {
						goto _ctr79;
					}
					{
						goto _ctr80;
					}
					_ctr78:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 2887 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st37;
					_st37:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof37;
					st_case_37:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr79;
						}
						case 34: {
							goto _ctr79;
						}
						case 58: {
							goto _ctr79;
						}
						case 63: {
							goto _ctr81;
						}
						case 91: {
							goto _ctr79;
						}
						case 93: {
							goto _ctr79;
						}
						case 95: {
							goto _ctr81;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr79;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr81;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr78;
						}
					} else {
						goto _ctr79;
					}
					{
						goto _ctr80;
					}
					_ctr67:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2947 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2953 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st38;
					_st38:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof38;
					st_case_38:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 46: {
							goto _ctr70;
						}
						case 47: {
							goto _st35;
						}
						case 58: {
							goto _ctr68;
						}
						case 63: {
							goto _ctr72;
						}
						case 91: {
							goto _ctr68;
						}
						case 93: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr72;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr72;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr67;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr69;
					}
					_ctr37:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 3019 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 3025 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st39;
					_st39:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 46: {
							goto _ctr70;
						}
						case 47: {
							goto _st35;
						}
						case 58: {
							goto _ctr83;
						}
						case 63: {
							goto _ctr82;
						}
						case 91: {
							goto _ctr68;
						}
						case 93: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr82;
						}
						case 98: {
							goto _st41;
						}
						case 120: {
							goto _st43;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr82;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr82;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr69;
					}
					_ctr82:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3101 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st40;
					_ctr89:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3112 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st40;
					_ctr96:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3123 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st40;
					_st40:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof40;
					st_case_40:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _st16;
						}
						case 63: {
							goto _st40;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _st40;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st40;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st40;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st41:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _st16;
						}
						case 63: {
							goto _st40;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _st40;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr44;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 49 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st40;
							}
						} else {
							goto _st40;
						}
					} else {
						goto _ctr86;
					}
					{
						goto _st13;
					}
					_ctr86:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 3239 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_st42:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof42;
					st_case_42:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr87;
						}
						case 34: {
							goto _ctr87;
						}
						case 58: {
							goto _ctr90;
						}
						case 63: {
							goto _ctr89;
						}
						case 91: {
							goto _ctr87;
						}
						case 93: {
							goto _ctr87;
						}
						case 95: {
							goto _ctr89;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr87;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr87;
						}
					} else if ( ( (*( p))) > 49 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr89;
							}
						} else {
							goto _ctr89;
						}
					} else {
						goto _ctr86;
					}
					{
						goto _ctr88;
					}
					_st43:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof43;
					st_case_43:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 58: {
							goto _st16;
						}
						case 63: {
							goto _st40;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _st40;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr44;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) < 97 ) {
							if ( 65 <= ( (*( p))) && ( (*( p))) <= 70 ) {
								goto _ctr92;
							}
						} else if ( ( (*( p))) > 102 ) {
							if ( ( (*( p))) <= 122 ) {
								goto _st40;
							}
						} else {
							goto _ctr91;
						}
					} else {
						goto _ctr91;
					}
					{
						goto _st13;
					}
					_ctr91:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 3363 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st44;
					_st44:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof44;
					st_case_44:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr93;
						}
						case 34: {
							goto _ctr93;
						}
						case 58: {
							goto _ctr95;
						}
						case 63: {
							goto _ctr96;
						}
						case 91: {
							goto _ctr93;
						}
						case 93: {
							goto _ctr93;
						}
						case 95: {
							goto _ctr96;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr93;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr93;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) < 97 ) {
							if ( 65 <= ( (*( p))) && ( (*( p))) <= 70 ) {
								goto _ctr92;
							}
						} else if ( ( (*( p))) > 102 ) {
							if ( ( (*( p))) <= 122 ) {
								goto _ctr96;
							}
						} else {
							goto _ctr91;
						}
					} else {
						goto _ctr91;
					}
					{
						goto _ctr94;
					}
					_ctr92:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 3431 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st45;
					_st45:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof45;
					st_case_45:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr93;
						}
						case 34: {
							goto _ctr93;
						}
						case 63: {
							goto _ctr98;
						}
						case 91: {
							goto _ctr93;
						}
						case 93: {
							goto _ctr93;
						}
						case 95: {
							goto _ctr98;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr93;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr93;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) < 97 ) {
							if ( 65 <= ( (*( p))) && ( (*( p))) <= 70 ) {
								goto _ctr92;
							}
						} else if ( ( (*( p))) > 102 ) {
							if ( ( (*( p))) <= 122 ) {
								goto _ctr98;
							}
						} else {
							goto _ctr97;
						}
					} else {
						goto _ctr97;
					}
					{
						goto _ctr94;
					}
					_ctr97:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 3496 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st46;
					_st46:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof46;
					st_case_46:
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr93;
						}
						case 34: {
							goto _ctr93;
						}
						case 58: {
							goto _ctr93;
						}
						case 63: {
							goto _ctr98;
						}
						case 91: {
							goto _ctr93;
						}
						case 93: {
							goto _ctr93;
						}
						case 95: {
							goto _ctr98;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr93;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr93;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) < 97 ) {
							if ( 65 <= ( (*( p))) && ( (*( p))) <= 70 ) {
								goto _ctr92;
							}
						} else if ( ( (*( p))) > 102 ) {
							if ( ( (*( p))) <= 122 ) {
								goto _ctr98;
							}
						} else {
							goto _ctr97;
						}
					} else {
						goto _ctr97;
					}
					{
						goto _ctr94;
					}
					_ctr38:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 3564 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 3570 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st47;
					_st47:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 46: {
							goto _ctr70;
						}
						case 47: {
							goto _st35;
						}
						case 58: {
							goto _ctr83;
						}
						case 63: {
							goto _ctr82;
						}
						case 91: {
							goto _ctr68;
						}
						case 93: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr82;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr82;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _ctr38;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr69;
					}
					_st48:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 40: {
							goto _st49;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr100;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr44;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st50;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st48;
						}
					} else {
						goto _ctr45;
					}
					{
						goto _st13;
					}
					_st49:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof49;
					st_case_49:
					if ( ( (*( p))) == 32 ) {
						goto _st49;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st49;
					}
					{
						goto _ctr102;
					}
					_st50:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 40: {
							goto _st49;
						}
						case 58: {
							goto _ctr44;
						}
						case 63: {
							goto _ctr45;
						}
						case 91: {
							goto _ctr100;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _ctr45;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr44;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st50;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st48;
						}
					} else {
						goto _ctr45;
					}
					{
						goto _st13;
					}
					_st51:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof51;
					st_case_51:
					if ( ( (*( p))) == 32 ) {
						goto _st51;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st51;
					}
					{
						goto _ctr103;
					}
					_st52:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr44;
						}
						case 34: {
							goto _ctr44;
						}
						case 40: {
							goto _st53;
						}
						case 58: {
							goto _st16;
						}
						case 63: {
							goto _st40;
						}
						case 91: {
							goto _ctr44;
						}
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _st52;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr44;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st52;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st52;
						}
					} else {
						goto _ctr44;
					}
					{
						goto _st13;
					}
					_st53:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof53;
					st_case_53:
					if ( ( (*( p))) == 32 ) {
						goto _st53;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st53;
					}
					{
						goto _ctr105;
					}
					_ctr109:
					{
#line 62 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 62 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 54; top+= 1; goto _st11;}}
						}}
					
#line 3869 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr111:
					{
#line 63 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 63 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 54; top+= 1; goto _st10;}}
						}}
					
#line 3880 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr113:
					{
#line 55 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 55 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 3891 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr120:
					{
#line 1 "NONE"
						{switch( act ) {
								case 31:  {
									p = ((te))-1;
									{
#line 50 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 32:  {
									p = ((te))-1;
									{
#line 51 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 33:  {
									p = ((te))-1;
									{
#line 52 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 43:  {
									p = ((te))-1;
									{
#line 91 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 3929 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr121:
					{
#line 198 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 198 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 3940 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr122:
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 91 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 3951 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr136:
					{
#line 54 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 54 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 3962 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr138:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 3974 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 56 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 3988 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr143:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4001 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 56 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 4015 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr147:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4027 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 56 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 4041 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr152:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4052 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 56 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 4066 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr155:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4077 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 56 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 56 "lex/lemon_data_string_parser.rl"
								
								onFloat(ragel_cat, ragel_type, ragel_num);
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 4091 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr158:
					{
#line 78 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 78 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 54; top+= 1; goto _st12;}}
						}}
					
#line 4105 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr159:
					{
#line 71 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 71 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_DATA_NAME, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 4122 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr160:
					{
#line 187 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 187 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); {stack[top] = 54; top+= 1; goto _st12;}}
						}}
					
#line 4133 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_ctr162:
					{
#line 64 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 64 "lex/lemon_data_string_parser.rl"
								
								// skip trailing whitespaces
								auto te0 = te;
								while(*(--te0) == ' ') ;
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te0);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 4150 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_st54:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 4164 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof54;
					st_case_54:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 4174 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _st56;
						}
						case 34: {
							goto _ctr109;
						}
						case 35: {
							goto _st57;
						}
						case 39: {
							goto _ctr111;
						}
						case 40: {
							goto _st68;
						}
						case 41: {
							goto _ctr113;
						}
						case 43: {
							goto _ctr114;
						}
						case 45: {
							goto _ctr114;
						}
						case 48: {
							goto _ctr115;
						}
						case 91: {
							goto _st84;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _st56;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st85;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st82;
						}
					} else {
						goto _ctr116;
					}
					{
						goto _ctr107;
					}
					_ctr107:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4231 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 4237 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_ctr129:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4245 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 51 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4251 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_ctr132:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4259 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 52 "lex/lemon_data_string_parser.rl"
						{act = 33;}}
					
#line 4265 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_ctr135:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4273 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/lemon_data_string_parser.rl"
						{act = 31;}}
					
#line 4279 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_ctr139:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4287 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4297 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 4303 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_ctr144:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4311 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4322 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 4328 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_ctr148:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4336 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4346 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 4352 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_ctr153:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4360 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4369 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 4375 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_ctr156:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4383 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4392 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{act = 43;}}
					
#line 4398 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_st55:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr120;
						}
						case 34: {
							goto _ctr120;
						}
						case 91: {
							goto _ctr120;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr120;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr120;
					}
					{
						goto _ctr107;
					}
					_st56:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _st56;
						}
						case 41: {
							goto _ctr113;
						}
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st56;
					}
					{
						goto _ctr121;
					}
					_st57:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
						case 102: {
							goto _st58;
						}
						case 110: {
							goto _st62;
						}
						case 116: {
							goto _st65;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st58:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
						case 97: {
							goto _st59;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st59:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
						case 108: {
							goto _st60;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st60:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
						case 115: {
							goto _st61;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st61:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
						case 101: {
							goto _ctr129;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st62:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
						case 117: {
							goto _st63;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st63:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
						case 108: {
							goto _st64;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st64:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
						case 108: {
							goto _ctr132;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st65:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
						case 114: {
							goto _st66;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st66:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
						case 117: {
							goto _st67;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st67:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
						case 101: {
							goto _ctr135;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st68:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof68;
					st_case_68:
					if ( ( (*( p))) == 32 ) {
						goto _st68;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st68;
					}
					{
						goto _ctr136;
					}
					_ctr114:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 4874 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st69;
					_st69:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 48: {
							goto _ctr137;
						}
						case 91: {
							goto _ctr122;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr116;
						}
					} else {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_ctr137:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 4921 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 4927 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st70;
					_st70:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr138;
						}
						case 34: {
							goto _ctr138;
						}
						case 46: {
							goto _ctr140;
						}
						case 47: {
							goto _st73;
						}
						case 91: {
							goto _ctr138;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr138;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr138;
					}
					{
						goto _ctr139;
					}
					_ctr140:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 4977 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 4986 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st71;
					_st71:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr142;
						}
					} else {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_ctr142:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 5033 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_st72:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr143;
						}
						case 34: {
							goto _ctr143;
						}
						case 91: {
							goto _ctr143;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr143;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr142;
						}
					} else {
						goto _ctr143;
					}
					{
						goto _ctr144;
					}
					_st73:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 48: {
							goto _ctr145;
						}
						case 91: {
							goto _ctr122;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr146;
						}
					} else {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_ctr145:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 5116 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st74;
					_st74:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr147;
						}
						case 34: {
							goto _ctr147;
						}
						case 91: {
							goto _ctr147;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr147;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr147;
					}
					{
						goto _ctr148;
					}
					_ctr146:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 5156 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st75;
					_st75:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr147;
						}
						case 34: {
							goto _ctr147;
						}
						case 91: {
							goto _ctr147;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr147;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr146;
						}
					} else {
						goto _ctr147;
					}
					{
						goto _ctr148;
					}
					_ctr116:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5200 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5206 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st76;
					_st76:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr138;
						}
						case 34: {
							goto _ctr138;
						}
						case 46: {
							goto _ctr140;
						}
						case 47: {
							goto _st73;
						}
						case 91: {
							goto _ctr138;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr138;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr116;
						}
					} else {
						goto _ctr138;
					}
					{
						goto _ctr139;
					}
					_ctr115:
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 5256 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 5262 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st77;
					_st77:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr138;
						}
						case 34: {
							goto _ctr138;
						}
						case 46: {
							goto _ctr140;
						}
						case 47: {
							goto _st73;
						}
						case 91: {
							goto _ctr138;
						}
						case 98: {
							goto _st78;
						}
						case 120: {
							goto _st80;
						}
					}
					if ( ( (*( p))) > 13 ) {
						if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
							goto _ctr138;
						}
					} else if ( ( (*( p))) >= 9 ) {
						goto _ctr138;
					}
					{
						goto _ctr139;
					}
					_st78:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
							goto _ctr151;
						}
					} else {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_ctr151:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 5350 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st79;
					_st79:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr152;
						}
						case 34: {
							goto _ctr152;
						}
						case 91: {
							goto _ctr152;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr152;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
							goto _ctr151;
						}
					} else {
						goto _ctr152;
					}
					{
						goto _ctr153;
					}
					_st80:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 91: {
							goto _ctr122;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr122;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 70 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
								goto _ctr154;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _ctr154;
						}
					} else {
						goto _ctr154;
					}
					{
						goto _ctr107;
					}
					_ctr154:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 5438 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st81;
					_st81:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr155;
						}
						case 34: {
							goto _ctr155;
						}
						case 91: {
							goto _ctr155;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr155;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr155;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 70 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
								goto _ctr154;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _ctr154;
						}
					} else {
						goto _ctr154;
					}
					{
						goto _ctr156;
					}
					_st82:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 40: {
							goto _st83;
						}
						case 91: {
							goto _ctr158;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st82;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st82;
						}
					} else {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st83:
					if ( p == eof ) {
						if ( cs >= 9 )
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
						goto _ctr159;
					}
					_st84:
					if ( p == eof ) {
						if ( cs >= 9 )
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
						goto _ctr160;
					}
					_st85:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr122;
						}
						case 34: {
							goto _ctr122;
						}
						case 40: {
							goto _st86;
						}
						case 91: {
							goto _ctr122;
						}
						case 95: {
							goto _st85;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr122;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st85;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st85;
						}
					} else {
						goto _ctr122;
					}
					{
						goto _ctr107;
					}
					_st86:
					if ( p == eof ) {
						if ( cs >= 9 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof86;
					st_case_86:
					if ( ( (*( p))) == 32 ) {
						goto _st86;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st86;
					}
					{
						goto _ctr162;
					}
					st_out:
					_test_eof9: cs = 9; goto _test_eof; 
					_test_eof10: cs = 10; goto _test_eof; 
					_test_eof1: cs = 1; goto _test_eof; 
					_test_eof11: cs = 11; goto _test_eof; 
					_test_eof2: cs = 2; goto _test_eof; 
					_test_eof12: cs = 12; goto _test_eof; 
					_test_eof13: cs = 13; goto _test_eof; 
					_test_eof14: cs = 14; goto _test_eof; 
					_test_eof15: cs = 15; goto _test_eof; 
					_test_eof3: cs = 3; goto _test_eof; 
					_test_eof4: cs = 4; goto _test_eof; 
					_test_eof5: cs = 5; goto _test_eof; 
					_test_eof16: cs = 16; goto _test_eof; 
					_test_eof6: cs = 6; goto _test_eof; 
					_test_eof7: cs = 7; goto _test_eof; 
					_test_eof8: cs = 8; goto _test_eof; 
					_test_eof17: cs = 17; goto _test_eof; 
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
					
					_test_eof: {}
					if ( p == eof ) {
						switch ( cs ) {
							case 9: {
								break;
							}
							case 10: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 5733 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 1: {
								break;
							}
							case 0: {
								break;
							}
							case 11: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 5748 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 2: {
								break;
							}
							case 12: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 5760 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
							case 3: {
								break;
							}
							case 4: {
								break;
							}
							case 5: {
								break;
							}
							case 16: {
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
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 5910 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
						}
						switch ( cs ) {
						}
						switch ( cs ) {
							case 9:
							goto _st9;case 10:
							goto _st10;case 1:
							goto _st1;case 0:
							goto _st0;case 11:
							goto _st11;case 2:
							goto _st2;case 12:
							goto _st12;case 13:
							goto _ctr44;case 14:
							goto _ctr15;case 15:
							goto _ctr46;case 3:
							goto _ctr13;case 4:
							goto _ctr15;case 5:
							goto _ctr15;case 16:
							goto _ctr49;case 6:
							goto _ctr13;case 7:
							goto _ctr15;case 8:
							goto _ctr13;case 17:
							goto _ctr50;case 18:
							goto _ctr44;case 19:
							goto _ctr44;case 20:
							goto _ctr44;case 21:
							goto _ctr44;case 22:
							goto _ctr44;case 23:
							goto _ctr44;case 24:
							goto _ctr44;case 25:
							goto _ctr44;case 26:
							goto _ctr44;case 27:
							goto _ctr44;case 28:
							goto _ctr44;case 29:
							goto _ctr64;case 30:
							goto _ctr65;case 31:
							goto _ctr44;case 32:
							goto _ctr68;case 33:
							goto _ctr44;case 34:
							goto _ctr74;case 35:
							goto _ctr44;case 36:
							goto _ctr79;case 37:
							goto _ctr79;case 38:
							goto _ctr68;case 39:
							goto _ctr68;case 40:
							goto _ctr44;case 41:
							goto _ctr44;case 42:
							goto _ctr87;case 43:
							goto _ctr44;case 44:
							goto _ctr93;case 45:
							goto _ctr93;case 46:
							goto _ctr93;case 47:
							goto _ctr68;case 48:
							goto _ctr44;case 49:
							goto _ctr102;case 50:
							goto _ctr44;case 51:
							goto _ctr103;case 52:
							goto _ctr44;case 53:
							goto _ctr105;case 54:
							goto _st54;case 55:
							goto _ctr120;case 56:
							goto _ctr121;case 57:
							goto _ctr122;case 58:
							goto _ctr122;case 59:
							goto _ctr122;case 60:
							goto _ctr122;case 61:
							goto _ctr122;case 62:
							goto _ctr122;case 63:
							goto _ctr122;case 64:
							goto _ctr122;case 65:
							goto _ctr122;case 66:
							goto _ctr122;case 67:
							goto _ctr122;case 68:
							goto _ctr136;case 69:
							goto _ctr122;case 70:
							goto _ctr138;case 71:
							goto _ctr122;case 72:
							goto _ctr143;case 73:
							goto _ctr122;case 74:
							goto _ctr147;case 75:
							goto _ctr147;case 76:
							goto _ctr138;case 77:
							goto _ctr138;case 78:
							goto _ctr122;case 79:
							goto _ctr152;case 80:
							goto _ctr122;case 81:
							goto _ctr155;case 82:
							goto _ctr122;case 83:
							goto _ctr159;case 84:
							goto _ctr160;case 85:
							goto _ctr122;case 86:
							goto _ctr162;	}
					}
					
					if ( cs >= 9 )
						goto _out; _pop: {}
					_out: {}
				}
				
#line 288 "lex/lemon_data_string_parser.rl"
				
				
			} catch(std::exception& e) {
				LIB_ERR << e.what();
				return false;
			}
			
			if (cs < 
#line 6118 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
			9
#line 295 "lex/lemon_data_string_parser.rl"
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
