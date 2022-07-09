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


#line 170 "lex/lemon_data_string_parser.rl"


# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 57 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
		static const int lemon_data_string_lexer_start = 15;
		static const int lemon_data_string_lexer_first_final = 15;
		static const int lemon_data_string_lexer_error = 0;
		
		static const int lemon_data_string_lexer_en_scan_sqstring = 16;
		static const int lemon_data_string_lexer_en_scan_dqstring = 17;
		static const int lemon_data_string_lexer_en_scan_dict = 18;
		static const int lemon_data_string_lexer_en_scan_token = 21;
		static const int lemon_data_string_lexer_en_main = 15;
		
		
#line 177 "lex/lemon_data_string_parser.rl"
		
		
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
				
#line 255 "lex/lemon_data_string_parser.rl"
				
				
#line 161 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					if ( p == pe )
						goto _test_eof;
					goto _resume;
					
					_again: {}
					switch ( cs ) {
						case 15: goto _st15;
						case 16: goto _st16;
						case 1: goto _st1;
						case 0: goto _st0;
						case 17: goto _st17;
						case 2: goto _st2;
						case 18: goto _st18;
						case 19: goto _st19;
						case 3: goto _st3;
						case 4: goto _st4;
						case 5: goto _st5;
						case 20: goto _st20;
						case 6: goto _st6;
						case 7: goto _st7;
						case 8: goto _st8;
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
						case 9: goto _st9;
						case 36: goto _st36;
						case 10: goto _st10;
						case 37: goto _st37;
						case 38: goto _st38;
						case 39: goto _st39;
						case 40: goto _st40;
						case 11: goto _st11;
						case 41: goto _st41;
						case 12: goto _st12;
						case 42: goto _st42;
						case 43: goto _st43;
						case 44: goto _st44;
						case 45: goto _st45;
						case 46: goto _st46;
						case 47: goto _st47;
						case 48: goto _st48;
						case 49: goto _st49;
						case 50: goto _st50;
						case 13: goto _st13;
						case 51: goto _st51;
						case 52: goto _st52;
						case 53: goto _st53;
						case 54: goto _st54;
						case 14: goto _st14;
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
					}
					
					_resume: {}
					switch ( cs ) {
						case 15:
						goto st_case_15;
						case 16:
						goto st_case_16;
						case 1:
						goto st_case_1;
						case 0:
						goto st_case_0;
						case 17:
						goto st_case_17;
						case 2:
						goto st_case_2;
						case 18:
						goto st_case_18;
						case 19:
						goto st_case_19;
						case 3:
						goto st_case_3;
						case 4:
						goto st_case_4;
						case 5:
						goto st_case_5;
						case 20:
						goto st_case_20;
						case 6:
						goto st_case_6;
						case 7:
						goto st_case_7;
						case 8:
						goto st_case_8;
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
						case 9:
						goto st_case_9;
						case 36:
						goto st_case_36;
						case 10:
						goto st_case_10;
						case 37:
						goto st_case_37;
						case 38:
						goto st_case_38;
						case 39:
						goto st_case_39;
						case 40:
						goto st_case_40;
						case 11:
						goto st_case_11;
						case 41:
						goto st_case_41;
						case 12:
						goto st_case_12;
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
						case 13:
						goto st_case_13;
						case 51:
						goto st_case_51;
						case 52:
						goto st_case_52;
						case 53:
						goto st_case_53;
						case 54:
						goto st_case_54;
						case 14:
						goto st_case_14;
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
					}
					goto st_out;
					_ctr33:
					{
#line 169 "lex/lemon_data_string_parser.rl"
						{p = p - 1; } {stack[top] = 15; top+= 1; goto _st21;}}
					
#line 405 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st15;
					_st15:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 419 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof15;
					st_case_15:
					if ( ( (*( p))) == 32 ) {
						goto _st15;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st15;
					}
					{
						goto _ctr33;
					}
					_ctr2:
					{
#line 89 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 89 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 442 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st16;
					_ctr3:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 88 "lex/lemon_data_string_parser.rl"
								ragel_string += '\''; }
						}}
					
#line 453 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st16;
					_ctr4:
					{
#line 90 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 90 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 464 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st16;
					_ctr5:
					{
#line 91 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 91 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 475 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st16;
					_ctr6:
					{
#line 92 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 92 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 486 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st16;
					_ctr35:
					{
#line 87 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 87 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));   }
						}}
					
#line 497 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st16;
					_ctr36:
					{
#line 93 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 93 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 508 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st16;
					_st16:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 522 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof16;
					st_case_16:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 532 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 39: {
							goto _ctr36;
						}
						case 96: {
							goto _st1;
						}
					}
					{
						goto _ctr35;
					}
					_st1:
					if ( p == eof ) {
						if ( cs >= 15 )
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
#line 100 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 100 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 588 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st17;
					_ctr9:
					{
#line 99 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 99 "lex/lemon_data_string_parser.rl"
								ragel_string += '"'; }
						}}
					
#line 599 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st17;
					_ctr10:
					{
#line 101 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 101 "lex/lemon_data_string_parser.rl"
								ragel_string += ','; }
						}}
					
#line 610 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st17;
					_ctr11:
					{
#line 102 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 102 "lex/lemon_data_string_parser.rl"
								ragel_string += ';'; }
						}}
					
#line 621 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st17;
					_ctr12:
					{
#line 103 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 103 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 632 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st17;
					_ctr38:
					{
#line 98 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 98 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p))));  }
						}}
					
#line 643 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st17;
					_ctr39:
					{
#line 104 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 104 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 654 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st17;
					_st17:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 668 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof17;
					st_case_17:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 678 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 34: {
							goto _ctr39;
						}
						case 96: {
							goto _st2;
						}
					}
					{
						goto _ctr38;
					}
					_st2:
					if ( p == eof ) {
						if ( cs >= 15 )
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
#line 1 "NONE"
						{switch( act ) {
								case 0:  {
									{
#line 1 "NONE"
										{goto _st0;}}
									break; 
								}
								case 16:  {
									p = ((te))-1;
									{
#line 111 "lex/lemon_data_string_parser.rl"
										pushToken(TK_SPACE); }
									break; 
								}
							}}
					}
					
#line 742 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st18;
					_ctr44:
					{
#line 112 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 112 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_CLOSE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 753 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st18;
					_ctr45:
					{
#line 111 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 111 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); }
						}}
					
#line 764 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st18;
					_ctr46:
					{
#line 110 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 110 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_KEY); }
						}}
					
#line 775 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st18;
					_st18:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 789 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 1 "NONE"
						{act = 0;}}
					
#line 795 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof18;
					st_case_18:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 805 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr41;
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
						case 93: {
							goto _ctr44;
						}
						case 95: {
							goto _st8;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr41;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st8;
						}
					} else {
						goto _st8;
					}
					{
						goto _st0;
					}
					_ctr41:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 846 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 111 "lex/lemon_data_string_parser.rl"
						{act = 16;}}
					
#line 852 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st19;
					_st19:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr41;
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
							goto _ctr41;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st8;
						}
					} else {
						goto _st8;
					}
					{
						goto _ctr45;
					}
					_st3:
					if ( p == eof ) {
						if ( cs >= 15 )
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
						if ( cs >= 15 )
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
						goto _ctr13;
					}
					_st5:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof5;
					st_case_5:
					if ( ( (*( p))) == 58 ) {
						goto _st20;
					}
					{
						goto _ctr13;
					}
					_st20:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof20;
					st_case_20:
					if ( ( (*( p))) == 32 ) {
						goto _st20;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st20;
					}
					{
						goto _ctr46;
					}
					_st6:
					if ( p == eof ) {
						if ( cs >= 15 )
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
						if ( cs >= 15 )
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
						goto _ctr13;
					}
					_st8:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _st20;
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
					_ctr19:
					{
#line 126 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 126 "lex/lemon_data_string_parser.rl"
								
								switch(ragel_type) {
									case TYPE_FLOAT:
									pushFloat(ragel_num.getFloat());
									break;
									case TYPE_INT:
									case TYPE_BIN:
									case TYPE_HEX:
									pushFloat(ragel_num.getInteger());
									break;
									case TYPE_RATIO:
									pushFloat(ragel_num.getRatioAsFloat());
									break;
									default:
									break;
								}
								
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1117 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr25:
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 166 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 1129 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr26:
					{
#line 153 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 153 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 1143 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr28:
					{
#line 158 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 158 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
								{stack[top] = 21; top+= 1; goto _st18;}}
						}}
					
#line 1157 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr29:
					{
#line 1 "NONE"
						{switch( act ) {
								case 18:  {
									p = ((te))-1;
									{
#line 116 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 19:  {
									p = ((te))-1;
									{
#line 117 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 20:  {
									p = ((te))-1;
									{
#line 118 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 24:  {
									p = ((te))-1;
									{
#line 122 "lex/lemon_data_string_parser.rl"
										pushToken(TK_DICT_CLOSE); }
									break; 
								}
								case 32:  {
									p = ((te))-1;
									{
#line 166 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 1202 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr30:
					{
#line 148 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 148 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te-1);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 1216 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr50:
					{
#line 123 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 123 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 21; top+= 1; goto _st17;}}
						}}
					
#line 1227 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr51:
					{
#line 124 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 124 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 21; top+= 1; goto _st16;}}
						}}
					
#line 1238 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr53:
					{
#line 120 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 120 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 1249 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr68:
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 166 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 1260 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr75:
					{
#line 164 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 164 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 1271 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr76:
					{
#line 119 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 119 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 1282 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr78:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1294 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 126 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 126 "lex/lemon_data_string_parser.rl"
								
								switch(ragel_type) {
									case TYPE_FLOAT:
									pushFloat(ragel_num.getFloat());
									break;
									case TYPE_INT:
									case TYPE_BIN:
									case TYPE_HEX:
									pushFloat(ragel_num.getInteger());
									break;
									case TYPE_RATIO:
									pushFloat(ragel_num.getRatioAsFloat());
									break;
									default:
									break;
								}
								
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1323 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr81:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1336 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 126 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 126 "lex/lemon_data_string_parser.rl"
								
								switch(ragel_type) {
									case TYPE_FLOAT:
									pushFloat(ragel_num.getFloat());
									break;
									case TYPE_INT:
									case TYPE_BIN:
									case TYPE_HEX:
									pushFloat(ragel_num.getInteger());
									break;
									case TYPE_RATIO:
									pushFloat(ragel_num.getRatioAsFloat());
									break;
									default:
									break;
								}
								
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1365 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr82:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1377 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 126 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 126 "lex/lemon_data_string_parser.rl"
								
								switch(ragel_type) {
									case TYPE_FLOAT:
									pushFloat(ragel_num.getFloat());
									break;
									case TYPE_INT:
									case TYPE_BIN:
									case TYPE_HEX:
									pushFloat(ragel_num.getInteger());
									break;
									case TYPE_RATIO:
									pushFloat(ragel_num.getRatioAsFloat());
									break;
									default:
									break;
								}
								
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1406 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr85:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1417 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 126 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 126 "lex/lemon_data_string_parser.rl"
								
								switch(ragel_type) {
									case TYPE_FLOAT:
									pushFloat(ragel_num.getFloat());
									break;
									case TYPE_INT:
									case TYPE_BIN:
									case TYPE_HEX:
									pushFloat(ragel_num.getInteger());
									break;
									case TYPE_RATIO:
									pushFloat(ragel_num.getRatioAsFloat());
									break;
									default:
									break;
								}
								
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1446 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr86:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1457 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 126 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 126 "lex/lemon_data_string_parser.rl"
								
								switch(ragel_type) {
									case TYPE_FLOAT:
									pushFloat(ragel_num.getFloat());
									break;
									case TYPE_INT:
									case TYPE_BIN:
									case TYPE_HEX:
									pushFloat(ragel_num.getInteger());
									break;
									case TYPE_RATIO:
									pushFloat(ragel_num.getRatioAsFloat());
									break;
									default:
									break;
								}
								
								ragel_num = {};
								ragel_cat = CAT_UNKNOWN;
								ragel_type = TYPE_UNKNOWN;
							}
						}}
					
#line 1486 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_ctr96:
					{
#line 121 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 121 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); }
						}}
					
#line 1497 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st21;
					_st21:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					{
#line 1 "NONE"
						{ts = 0;}}
					
#line 1511 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof21;
					st_case_21:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 1521 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _st32;
						}
						case 34: {
							goto _ctr50;
						}
						case 39: {
							goto _ctr51;
						}
						case 40: {
							goto _st33;
						}
						case 41: {
							goto _ctr53;
						}
						case 43: {
							goto _ctr54;
						}
						case 45: {
							goto _ctr54;
						}
						case 48: {
							goto _ctr55;
						}
						case 91: {
							goto _st53;
						}
						case 93: {
							goto _ctr59;
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
							goto _st32;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st43;
						}
					} else {
						goto _ctr56;
					}
					{
						goto _ctr48;
					}
					_ctr48:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1587 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 1593 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr59:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1601 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 122 "lex/lemon_data_string_parser.rl"
						{act = 24;}}
					
#line 1607 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_ctr64:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1615 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 1621 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 1627 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st22;
					_st22:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr29;
						}
						case 34: {
							goto _ctr29;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr29;
						}
						case 102: {
							goto _st23;
						}
						case 110: {
							goto _st26;
						}
						case 116: {
							goto _st27;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr29;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr29;
						}
					} else {
						goto _ctr29;
					}
					{
						goto _ctr48;
					}
					_st23:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 97: {
							goto _st24;
						}
						case 102: {
							goto _st23;
						}
						case 110: {
							goto _st26;
						}
						case 116: {
							goto _st27;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr68;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st24:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 102: {
							goto _st23;
						}
						case 108: {
							goto _st25;
						}
						case 110: {
							goto _st26;
						}
						case 116: {
							goto _st27;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr68;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st25:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 102: {
							goto _st23;
						}
						case 110: {
							goto _st26;
						}
						case 115: {
							goto _st29;
						}
						case 116: {
							goto _st27;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr68;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st26:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 102: {
							goto _st23;
						}
						case 110: {
							goto _st26;
						}
						case 116: {
							goto _st27;
						}
						case 117: {
							goto _st30;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr68;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st27:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 102: {
							goto _st23;
						}
						case 110: {
							goto _st26;
						}
						case 114: {
							goto _st28;
						}
						case 116: {
							goto _st27;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr68;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st28:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 102: {
							goto _st23;
						}
						case 110: {
							goto _st26;
						}
						case 116: {
							goto _st27;
						}
						case 117: {
							goto _st29;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr68;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st29:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 101: {
							goto _ctr68;
						}
						case 102: {
							goto _st23;
						}
						case 110: {
							goto _st26;
						}
						case 116: {
							goto _st27;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr68;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st30:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 102: {
							goto _st23;
						}
						case 108: {
							goto _st31;
						}
						case 110: {
							goto _st26;
						}
						case 116: {
							goto _st27;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr68;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st31:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 102: {
							goto _st23;
						}
						case 108: {
							goto _ctr68;
						}
						case 110: {
							goto _st26;
						}
						case 116: {
							goto _st27;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr68;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st32:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof32;
					st_case_32:
					if ( ( (*( p))) == 32 ) {
						goto _st32;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st32;
					}
					{
						goto _ctr75;
					}
					_st33:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof33;
					st_case_33:
					if ( ( (*( p))) == 32 ) {
						goto _st33;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st33;
					}
					{
						goto _ctr76;
					}
					_ctr54:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 2212 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st34;
					_st34:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 48: {
							goto _ctr77;
						}
						case 91: {
							goto _ctr68;
						}
						case 102: {
							goto _st23;
						}
						case 110: {
							goto _st26;
						}
						case 116: {
							goto _st27;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr56;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr77:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2274 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2280 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2286 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st35;
					_st35:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof35;
					st_case_35:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr79;
						}
						case 47: {
							goto _st10;
						}
					}
					{
						goto _ctr78;
					}
					_ctr79:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 2320 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 2329 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st9;
					_st9:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof9;
					st_case_9:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr20;
					}
					{
						goto _ctr19;
					}
					_ctr20:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 2357 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st36;
					_st36:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof36;
					st_case_36:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr20;
					}
					{
						goto _ctr81;
					}
					_st10:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof10;
					st_case_10:
					if ( ( (*( p))) == 48 ) {
						goto _ctr21;
					}
					if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr22;
					}
					{
						goto _ctr19;
					}
					_ctr21:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 2402 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st37;
					_st37:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof37;
					st_case_37:
					{
						goto _ctr82;
					}
					_ctr22:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 2424 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st38;
					_st38:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof38;
					st_case_38:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr22;
					}
					{
						goto _ctr82;
					}
					_ctr56:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2449 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2455 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2461 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st39;
					_st39:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof39;
					st_case_39:
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr79;
						}
						case 47: {
							goto _st10;
						}
					}
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr56;
					}
					{
						goto _ctr78;
					}
					_ctr55:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2494 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2500 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2506 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st40;
					_st40:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr79;
						}
						case 47: {
							goto _st10;
						}
						case 98: {
							goto _st11;
						}
						case 120: {
							goto _st12;
						}
					}
					{
						goto _ctr78;
					}
					_st11:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof11;
					st_case_11:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr23;
					}
					{
						goto _ctr19;
					}
					_ctr23:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 2559 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st41;
					_st41:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof41;
					st_case_41:
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr23;
					}
					{
						goto _ctr85;
					}
					_st12:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof12;
					st_case_12:
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr24;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr24;
						}
					} else {
						goto _ctr24;
					}
					{
						goto _ctr19;
					}
					_ctr24:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 2609 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st42;
					_st42:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof42;
					st_case_42:
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr24;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr24;
						}
					} else {
						goto _ctr24;
					}
					{
						goto _ctr86;
					}
					_st43:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr26;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr28;
						}
						case 102: {
							goto _st44;
						}
						case 110: {
							goto _st47;
						}
						case 116: {
							goto _st48;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st43;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st43;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st44:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr26;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr28;
						}
						case 97: {
							goto _st45;
						}
						case 102: {
							goto _st44;
						}
						case 110: {
							goto _st47;
						}
						case 116: {
							goto _st48;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st43;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st43;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st45:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr26;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr28;
						}
						case 102: {
							goto _st44;
						}
						case 108: {
							goto _st46;
						}
						case 110: {
							goto _st47;
						}
						case 116: {
							goto _st48;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st43;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st43;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st46:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr26;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr28;
						}
						case 102: {
							goto _st44;
						}
						case 110: {
							goto _st47;
						}
						case 115: {
							goto _ctr92;
						}
						case 116: {
							goto _st48;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st43;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st43;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st47:
					if ( p == eof ) {
						if ( cs >= 15 )
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
						case 40: {
							goto _ctr26;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr28;
						}
						case 102: {
							goto _st44;
						}
						case 110: {
							goto _st47;
						}
						case 116: {
							goto _st48;
						}
						case 117: {
							goto _st51;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st43;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st43;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st48:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr26;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr28;
						}
						case 102: {
							goto _st44;
						}
						case 110: {
							goto _st47;
						}
						case 114: {
							goto _st49;
						}
						case 116: {
							goto _st48;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st43;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st43;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st49:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr26;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr28;
						}
						case 102: {
							goto _st44;
						}
						case 110: {
							goto _st47;
						}
						case 116: {
							goto _st48;
						}
						case 117: {
							goto _ctr92;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st43;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st43;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr92:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3094 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
					_st50:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr26;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr28;
						}
						case 101: {
							goto _st13;
						}
						case 102: {
							goto _st44;
						}
						case 110: {
							goto _st47;
						}
						case 116: {
							goto _st48;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st43;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st43;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st13:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr26;
						}
						case 91: {
							goto _ctr28;
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
						goto _ctr25;
					}
					_st51:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr26;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr28;
						}
						case 102: {
							goto _st44;
						}
						case 108: {
							goto _ctr95;
						}
						case 110: {
							goto _st47;
						}
						case 116: {
							goto _st48;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st43;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st43;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr95:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3261 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
					_st52:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr26;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr28;
						}
						case 102: {
							goto _st44;
						}
						case 108: {
							goto _st13;
						}
						case 110: {
							goto _st47;
						}
						case 116: {
							goto _st48;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr68;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st43;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st43;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st53:
					if ( p == eof ) {
						if ( cs >= 15 )
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
						goto _ctr96;
					}
					_ctr60:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3354 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 3360 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_st54:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_st14:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr30;
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
						goto _ctr29;
					}
					_ctr97:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3458 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 3464 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st55;
					_st55:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 97: {
							goto _ctr100;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr100:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3536 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 3542 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st56;
					_st56:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 108: {
							goto _ctr101;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr101:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3614 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 3620 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st57;
					_st57:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 115: {
							goto _ctr102;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr98:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3692 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 3698 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st58;
					_st58:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
						case 117: {
							goto _ctr103;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr99:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3770 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 3776 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st59;
					_st59:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 114: {
							goto _ctr104;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr104:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3848 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 3854 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st60;
					_st60:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
						case 117: {
							goto _ctr102;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr102:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3926 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 3932 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st61;
					_st61:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 101: {
							goto _st14;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr103:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4004 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4010 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st62;
					_st62:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 108: {
							goto _ctr105;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr105:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4082 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4088 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st63;
					_st63:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 108: {
							goto _st14;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr61:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4160 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4166 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st64;
					_st64:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 97: {
							goto _ctr106;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr106:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4238 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4244 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st65;
					_st65:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 108: {
							goto _ctr107;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr107:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4316 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4322 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st66;
					_st66:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 115: {
							goto _ctr108;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr108:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4394 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4400 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st67;
					_st67:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 101: {
							goto _ctr109;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr109:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4472 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 117 "lex/lemon_data_string_parser.rl"
						{act = 19;}}
					
#line 4478 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr112:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4486 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 118 "lex/lemon_data_string_parser.rl"
						{act = 20;}}
					
#line 4492 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_ctr115:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4500 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 116 "lex/lemon_data_string_parser.rl"
						{act = 18;}}
					
#line 4506 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st68;
					_st68:
					if ( p == eof ) {
						if ( cs >= 15 )
							goto _out;
						else
							goto _pop;
					}
					p+= 1;
					if ( p == pe )
						goto _test_eof68;
					st_case_68:
					switch( ( (*( p))) ) {
						case 40: {
							goto _ctr30;
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
						goto _ctr29;
					}
					_ctr62:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4543 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4549 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st69;
					_st69:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
						case 117: {
							goto _ctr110;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr110:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4621 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4627 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st70;
					_st70:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 108: {
							goto _ctr111;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr111:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4699 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4705 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st71;
					_st71:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 108: {
							goto _ctr112;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr63:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4777 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4783 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st72;
					_st72:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 114: {
							goto _ctr113;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr113:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4855 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4861 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st73;
					_st73:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
						case 117: {
							goto _ctr114;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					_ctr114:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4933 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 166 "lex/lemon_data_string_parser.rl"
						{act = 32;}}
					
#line 4939 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st74;
					_st74:
					if ( p == eof ) {
						if ( cs >= 15 )
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
							goto _ctr68;
						}
						case 34: {
							goto _ctr68;
						}
						case 40: {
							goto _ctr30;
						}
						case 43: {
							goto _ctr64;
						}
						case 45: {
							goto _ctr64;
						}
						case 91: {
							goto _ctr68;
						}
						case 95: {
							goto _ctr60;
						}
						case 101: {
							goto _ctr115;
						}
						case 102: {
							goto _ctr97;
						}
						case 110: {
							goto _ctr98;
						}
						case 116: {
							goto _ctr99;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr68;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr60;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st14;
						}
					} else {
						goto _ctr68;
					}
					{
						goto _ctr48;
					}
					st_out:
					_test_eof15: cs = 15; goto _test_eof; 
					_test_eof16: cs = 16; goto _test_eof; 
					_test_eof1: cs = 1; goto _test_eof; 
					_test_eof17: cs = 17; goto _test_eof; 
					_test_eof2: cs = 2; goto _test_eof; 
					_test_eof18: cs = 18; goto _test_eof; 
					_test_eof19: cs = 19; goto _test_eof; 
					_test_eof3: cs = 3; goto _test_eof; 
					_test_eof4: cs = 4; goto _test_eof; 
					_test_eof5: cs = 5; goto _test_eof; 
					_test_eof20: cs = 20; goto _test_eof; 
					_test_eof6: cs = 6; goto _test_eof; 
					_test_eof7: cs = 7; goto _test_eof; 
					_test_eof8: cs = 8; goto _test_eof; 
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
					_test_eof9: cs = 9; goto _test_eof; 
					_test_eof36: cs = 36; goto _test_eof; 
					_test_eof10: cs = 10; goto _test_eof; 
					_test_eof37: cs = 37; goto _test_eof; 
					_test_eof38: cs = 38; goto _test_eof; 
					_test_eof39: cs = 39; goto _test_eof; 
					_test_eof40: cs = 40; goto _test_eof; 
					_test_eof11: cs = 11; goto _test_eof; 
					_test_eof41: cs = 41; goto _test_eof; 
					_test_eof12: cs = 12; goto _test_eof; 
					_test_eof42: cs = 42; goto _test_eof; 
					_test_eof43: cs = 43; goto _test_eof; 
					_test_eof44: cs = 44; goto _test_eof; 
					_test_eof45: cs = 45; goto _test_eof; 
					_test_eof46: cs = 46; goto _test_eof; 
					_test_eof47: cs = 47; goto _test_eof; 
					_test_eof48: cs = 48; goto _test_eof; 
					_test_eof49: cs = 49; goto _test_eof; 
					_test_eof50: cs = 50; goto _test_eof; 
					_test_eof13: cs = 13; goto _test_eof; 
					_test_eof51: cs = 51; goto _test_eof; 
					_test_eof52: cs = 52; goto _test_eof; 
					_test_eof53: cs = 53; goto _test_eof; 
					_test_eof54: cs = 54; goto _test_eof; 
					_test_eof14: cs = 14; goto _test_eof; 
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
					
					_test_eof: {}
					if ( p == eof ) {
						switch ( cs ) {
							case 15: {
								break;
							}
							case 16: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 5093 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 1: {
								break;
							}
							case 0: {
								break;
							}
							case 17: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 5108 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 2: {
								break;
							}
							case 18: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 5120 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 19: {
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
							case 20: {
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
							case 21: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 5153 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
							case 9: {
								break;
							}
							case 36: {
								break;
							}
							case 10: {
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
							case 11: {
								break;
							}
							case 41: {
								break;
							}
							case 12: {
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
							case 13: {
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
							case 14: {
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
						}
						switch ( cs ) {
						}
						switch ( cs ) {
							case 15:
							goto _st15;case 16:
							goto _st16;case 1:
							goto _st1;case 0:
							goto _st0;case 17:
							goto _st17;case 2:
							goto _st2;case 18:
							goto _st18;case 19:
							goto _ctr45;case 3:
							goto _ctr13;case 4:
							goto _ctr13;case 5:
							goto _ctr13;case 20:
							goto _ctr46;case 6:
							goto _ctr13;case 7:
							goto _ctr13;case 8:
							goto _ctr13;case 21:
							goto _st21;case 22:
							goto _ctr29;case 23:
							goto _ctr68;case 24:
							goto _ctr68;case 25:
							goto _ctr68;case 26:
							goto _ctr68;case 27:
							goto _ctr68;case 28:
							goto _ctr68;case 29:
							goto _ctr68;case 30:
							goto _ctr68;case 31:
							goto _ctr68;case 32:
							goto _ctr75;case 33:
							goto _ctr76;case 34:
							goto _ctr68;case 35:
							goto _ctr78;case 9:
							goto _ctr19;case 36:
							goto _ctr81;case 10:
							goto _ctr19;case 37:
							goto _ctr82;case 38:
							goto _ctr82;case 39:
							goto _ctr78;case 40:
							goto _ctr78;case 11:
							goto _ctr19;case 41:
							goto _ctr85;case 12:
							goto _ctr19;case 42:
							goto _ctr86;case 43:
							goto _ctr68;case 44:
							goto _ctr68;case 45:
							goto _ctr68;case 46:
							goto _ctr68;case 47:
							goto _ctr68;case 48:
							goto _ctr68;case 49:
							goto _ctr68;case 50:
							goto _ctr68;case 13:
							goto _ctr25;case 51:
							goto _ctr68;case 52:
							goto _ctr68;case 53:
							goto _ctr96;case 54:
							goto _ctr68;case 14:
							goto _ctr29;case 55:
							goto _ctr68;case 56:
							goto _ctr68;case 57:
							goto _ctr68;case 58:
							goto _ctr68;case 59:
							goto _ctr68;case 60:
							goto _ctr68;case 61:
							goto _ctr68;case 62:
							goto _ctr68;case 63:
							goto _ctr68;case 64:
							goto _ctr68;case 65:
							goto _ctr68;case 66:
							goto _ctr68;case 67:
							goto _ctr68;case 68:
							goto _ctr29;case 69:
							goto _ctr68;case 70:
							goto _ctr68;case 71:
							goto _ctr68;case 72:
							goto _ctr68;case 73:
							goto _ctr68;case 74:
							goto _ctr68;	}
					}
					
					if ( cs >= 15 )
						goto _out; _pop: {}
					_out: {}
				}
				
#line 256 "lex/lemon_data_string_parser.rl"
				
				
			} catch(std::exception& e) {
				setErrorMsg(e.what());
				return false;
			}
			
			if (cs < 
#line 5430 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
			15
#line 263 "lex/lemon_data_string_parser.rl"
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
		
		
	}
}
