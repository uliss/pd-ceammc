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


#line 156 "lex/lemon_data_string_parser.rl"


# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 57 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
		static const int lemon_data_string_lexer_start = 9;
		static const int lemon_data_string_lexer_first_final = 9;
		static const int lemon_data_string_lexer_error = 0;
		
		static const int lemon_data_string_lexer_en_sqstring = 10;
		static const int lemon_data_string_lexer_en_dqstring = 11;
		static const int lemon_data_string_lexer_en_token = 12;
		static const int lemon_data_string_lexer_en_main = 9;
		
		
#line 163 "lex/lemon_data_string_parser.rl"
		
		
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
				
				
#line 148 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				{
					cs = (int)lemon_data_string_lexer_start;
					top = 0;
					ts = 0;
					te = 0;
					act = 0;
				}
				
#line 241 "lex/lemon_data_string_parser.rl"
				
				
#line 160 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
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
						case 16: goto _st16;
						case 17: goto _st17;
						case 18: goto _st18;
						case 19: goto _st19;
						case 20: goto _st20;
						case 21: goto _st21;
						case 22: goto _st22;
						case 23: goto _st23;
						case 24: goto _st24;
						case 25: goto _st25;
						case 3: goto _st3;
						case 26: goto _st26;
						case 4: goto _st4;
						case 27: goto _st27;
						case 28: goto _st28;
						case 29: goto _st29;
						case 30: goto _st30;
						case 5: goto _st5;
						case 31: goto _st31;
						case 6: goto _st6;
						case 32: goto _st32;
						case 33: goto _st33;
						case 34: goto _st34;
						case 35: goto _st35;
						case 36: goto _st36;
						case 37: goto _st37;
						case 38: goto _st38;
						case 39: goto _st39;
						case 40: goto _st40;
						case 7: goto _st7;
						case 41: goto _st41;
						case 42: goto _st42;
						case 43: goto _st43;
						case 8: goto _st8;
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
						case 23:
						goto st_case_23;
						case 24:
						goto st_case_24;
						case 25:
						goto st_case_25;
						case 3:
						goto st_case_3;
						case 26:
						goto st_case_26;
						case 4:
						goto st_case_4;
						case 27:
						goto st_case_27;
						case 28:
						goto st_case_28;
						case 29:
						goto st_case_29;
						case 30:
						goto st_case_30;
						case 5:
						goto st_case_5;
						case 31:
						goto st_case_31;
						case 6:
						goto st_case_6;
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
						case 7:
						goto st_case_7;
						case 41:
						goto st_case_41;
						case 42:
						goto st_case_42;
						case 43:
						goto st_case_43;
						case 8:
						goto st_case_8;
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
					}
					goto st_out;
					_ctr23:
					{
#line 155 "lex/lemon_data_string_parser.rl"
						{p = p - 1; } {stack[top] = 9; top+= 1; goto _st12;}}
					
#line 371 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 385 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
						goto _ctr23;
					}
					_ctr2:
					{
#line 88 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 88 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 408 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st10;
					_ctr3:
					{
#line 87 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 87 "lex/lemon_data_string_parser.rl"
								ragel_string += '\''; }
						}}
					
#line 419 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st10;
					_ctr4:
					{
#line 89 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 89 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 430 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st10;
					_ctr25:
					{
#line 86 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 86 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p)))); }
						}}
					
#line 441 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st10;
					_ctr26:
					{
#line 90 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 90 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 452 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 466 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof10;
					st_case_10:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 476 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 39: {
							goto _ctr26;
						}
						case 96: {
							goto _st1;
						}
					}
					{
						goto _ctr25;
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
						case 96: {
							goto _ctr4;
						}
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr2;
					}
					{
						goto _st0;
					}
					st_case_0:
					_st0:
					cs = 0;
					goto _pop;
					_ctr6:
					{
#line 97 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 97 "lex/lemon_data_string_parser.rl"
								ragel_string += ' '; }
						}}
					
#line 529 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st11;
					_ctr7:
					{
#line 96 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 96 "lex/lemon_data_string_parser.rl"
								ragel_string += '"'; }
						}}
					
#line 540 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st11;
					_ctr8:
					{
#line 98 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 98 "lex/lemon_data_string_parser.rl"
								ragel_string += '`'; }
						}}
					
#line 551 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st11;
					_ctr28:
					{
#line 95 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 95 "lex/lemon_data_string_parser.rl"
								ragel_string += (( (*( p)))); }
						}}
					
#line 562 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st11;
					_ctr29:
					{
#line 99 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 99 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, &(*ragel_string.begin()), (&*ragel_string.end())); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 573 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 587 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof11;
					st_case_11:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 597 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 34: {
							goto _ctr29;
						}
						case 96: {
							goto _st2;
						}
					}
					{
						goto _ctr28;
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
							goto _ctr6;
						}
						case 34: {
							goto _ctr7;
						}
						case 96: {
							goto _ctr8;
						}
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr6;
					}
					{
						goto _st0;
					}
					_ctr9:
					{
#line 113 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 113 "lex/lemon_data_string_parser.rl"
								
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
					
#line 667 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr15:
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{p = ((te))-1;
							{
#line 152 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 679 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr16:
					{
#line 140 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 140 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 693 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr18:
					{
#line 145 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 145 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_DATA_NAME, ts, te-1);
								pushToken(TK_DICT_OPEN);
							}
						}}
					
#line 707 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr19:
					{
#line 1 "NONE"
						{switch( act ) {
								case 11:  {
									p = ((te))-1;
									{
#line 103 "lex/lemon_data_string_parser.rl"
										pushFloat(1); }
									break; 
								}
								case 12:  {
									p = ((te))-1;
									{
#line 104 "lex/lemon_data_string_parser.rl"
										pushFloat(0); }
									break; 
								}
								case 13:  {
									p = ((te))-1;
									{
#line 105 "lex/lemon_data_string_parser.rl"
										pushToken(TK_NULL); }
									break; 
								}
								case 17:  {
									p = ((te))-1;
									{
#line 109 "lex/lemon_data_string_parser.rl"
										pushToken(TK_DICT_CLOSE); }
									break; 
								}
								case 25:  {
									p = ((te))-1;
									{
#line 152 "lex/lemon_data_string_parser.rl"
										pushSymbolToken(TK_SYMBOL, ts, te); }
									break; 
								}
							}}
					}
					
#line 752 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr20:
					{
#line 135 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 135 "lex/lemon_data_string_parser.rl"
								
								pushSymbolToken(TK_FUNC_LIST_CALL, ts, te-1);
								pushToken(TK_LIST_OPEN);
							}
						}}
					
#line 766 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr33:
					{
#line 110 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 110 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 12; top+= 1; goto _st11;}}
						}}
					
#line 777 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr34:
					{
#line 111 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 111 "lex/lemon_data_string_parser.rl"
								ragel_string.clear(); {stack[top] = 12; top+= 1; goto _st10;}}
						}}
					
#line 788 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr35:
					{
#line 106 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 106 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_OPEN); }
						}}
					
#line 799 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr36:
					{
#line 107 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 107 "lex/lemon_data_string_parser.rl"
								pushToken(TK_LIST_CLOSE); }
						}}
					
#line 810 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr41:
					{
#line 108 "lex/lemon_data_string_parser.rl"
						{te = p+1;{
#line 108 "lex/lemon_data_string_parser.rl"
								pushToken(TK_DICT_OPEN); }
						}}
					
#line 821 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr51:
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 152 "lex/lemon_data_string_parser.rl"
								pushSymbolToken(TK_SYMBOL, ts, te); }
						}}
					
#line 832 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr58:
					{
#line 150 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 150 "lex/lemon_data_string_parser.rl"
								pushToken(TK_SPACE); {top -= 1;cs = stack[top];goto _again;} }
						}}
					
#line 843 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr60:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 855 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 113 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 113 "lex/lemon_data_string_parser.rl"
								
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
					
#line 884 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr63:
					{
#line 58 "lex/ragel_numeric.rl"
						
						const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
						ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
						ragel_type = TYPE_FLOAT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 897 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 113 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 113 "lex/lemon_data_string_parser.rl"
								
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
					
#line 926 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr64:
					{
#line 44 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num *= ragel_num.sign;
						ragel_type = TYPE_RATIO;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 938 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 113 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 113 "lex/lemon_data_string_parser.rl"
								
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
					
#line 967 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr67:
					{
#line 24 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_BIN;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 978 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 113 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 113 "lex/lemon_data_string_parser.rl"
								
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
					
#line 1007 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st12;
					_ctr68:
					{
#line 37 "lex/ragel_numeric.rl"
						
						ragel_type = TYPE_HEX;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1018 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 113 "lex/lemon_data_string_parser.rl"
						{te = p;p = p - 1;{
#line 113 "lex/lemon_data_string_parser.rl"
								
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
					
#line 1047 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					
#line 1061 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					p+= 1;
					if ( p == pe )
						goto _test_eof12;
					st_case_12:
					{
#line 1 "NONE"
						{ts = p;}}
					
#line 1071 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					switch( ( (*( p))) ) {
						case 32: {
							goto _st23;
						}
						case 34: {
							goto _ctr33;
						}
						case 39: {
							goto _ctr34;
						}
						case 40: {
							goto _ctr35;
						}
						case 41: {
							goto _ctr36;
						}
						case 43: {
							goto _ctr37;
						}
						case 45: {
							goto _ctr37;
						}
						case 48: {
							goto _ctr38;
						}
						case 91: {
							goto _ctr41;
						}
						case 93: {
							goto _ctr42;
						}
						case 102: {
							goto _ctr44;
						}
						case 110: {
							goto _ctr45;
						}
						case 116: {
							goto _ctr46;
						}
					}
					if ( ( (*( p))) < 49 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _st23;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st33;
						}
					} else {
						goto _ctr39;
					}
					{
						goto _ctr31;
					}
					_ctr31:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1137 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 1143 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st13;
					_ctr42:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1151 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 109 "lex/lemon_data_string_parser.rl"
						{act = 17;}}
					
#line 1157 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st13;
					_ctr47:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1165 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 1171 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 1177 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr19;
						}
						case 34: {
							goto _ctr19;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr19;
						}
						case 102: {
							goto _st14;
						}
						case 110: {
							goto _st17;
						}
						case 116: {
							goto _st18;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr19;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr19;
						}
					} else {
						goto _ctr19;
					}
					{
						goto _ctr31;
					}
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 97: {
							goto _st15;
						}
						case 102: {
							goto _st14;
						}
						case 110: {
							goto _st17;
						}
						case 116: {
							goto _st18;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr51;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 102: {
							goto _st14;
						}
						case 108: {
							goto _st16;
						}
						case 110: {
							goto _st17;
						}
						case 116: {
							goto _st18;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr51;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
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
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 102: {
							goto _st14;
						}
						case 110: {
							goto _st17;
						}
						case 115: {
							goto _st20;
						}
						case 116: {
							goto _st18;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr51;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
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
						case 32: {
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 102: {
							goto _st14;
						}
						case 110: {
							goto _st17;
						}
						case 116: {
							goto _st18;
						}
						case 117: {
							goto _st21;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr51;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 102: {
							goto _st14;
						}
						case 110: {
							goto _st17;
						}
						case 114: {
							goto _st19;
						}
						case 116: {
							goto _st18;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr51;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 102: {
							goto _st14;
						}
						case 110: {
							goto _st17;
						}
						case 116: {
							goto _st18;
						}
						case 117: {
							goto _st20;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr51;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 101: {
							goto _ctr51;
						}
						case 102: {
							goto _st14;
						}
						case 110: {
							goto _st17;
						}
						case 116: {
							goto _st18;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr51;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 102: {
							goto _st14;
						}
						case 108: {
							goto _st22;
						}
						case 110: {
							goto _st17;
						}
						case 116: {
							goto _st18;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr51;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 102: {
							goto _st14;
						}
						case 108: {
							goto _ctr51;
						}
						case 110: {
							goto _st17;
						}
						case 116: {
							goto _st18;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr51;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
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
					if ( ( (*( p))) == 32 ) {
						goto _st23;
					}
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st23;
					}
					{
						goto _ctr58;
					}
					_ctr37:
					{
#line 21 "lex/ragel_numeric.rl"
						ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
					
#line 1742 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st24;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 48: {
							goto _ctr59;
						}
						case 91: {
							goto _ctr51;
						}
						case 102: {
							goto _st14;
						}
						case 110: {
							goto _st17;
						}
						case 116: {
							goto _st18;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr39;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr59:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1804 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 1810 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 1816 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st25;
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
						case 46: {
							goto _ctr61;
						}
						case 47: {
							goto _st4;
						}
					}
					{
						goto _ctr60;
					}
					_ctr61:
					{
#line 30 "lex/ragel_numeric.rl"
						
						ragel_num.vint *= ragel_num.sign;
						ragel_type = TYPE_INT;
						ragel_cat  = CAT_NUMBER;
					}
					
#line 1850 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 50 "lex/ragel_numeric.rl"
						
						ragel_num.ratio.num = 0;
						ragel_num.ratio.den = 1;
					}
					
#line 1859 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st3;
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
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr10;
					}
					{
						goto _ctr9;
					}
					_ctr10:
					{
#line 54 "lex/ragel_numeric.rl"
						
						(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
						ragel_num.ratio.den *= 10;
					}
					
#line 1887 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st26;
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
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr10;
					}
					{
						goto _ctr63;
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
					if ( ( (*( p))) == 48 ) {
						goto _ctr11;
					}
					if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr12;
					}
					{
						goto _ctr9;
					}
					_ctr11:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 1932 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st27;
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
					{
						goto _ctr64;
					}
					_ctr12:
					{
#line 43 "lex/ragel_numeric.rl"
						(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
					
#line 1954 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st28;
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
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr12;
					}
					{
						goto _ctr64;
					}
					_ctr39:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 1979 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 1985 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 1991 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
						case 46: {
							goto _ctr61;
						}
						case 47: {
							goto _st4;
						}
					}
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr39;
					}
					{
						goto _ctr60;
					}
					_ctr38:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2024 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 29 "lex/ragel_numeric.rl"
						(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
					
#line 2030 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 42 "lex/ragel_numeric.rl"
						(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
					
#line 2036 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st30;
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
					switch( ( (*( p))) ) {
						case 46: {
							goto _ctr61;
						}
						case 47: {
							goto _st4;
						}
						case 98: {
							goto _st5;
						}
						case 120: {
							goto _st6;
						}
					}
					{
						goto _ctr60;
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
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr13;
					}
					{
						goto _ctr9;
					}
					_ctr13:
					{
#line 23 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
					
#line 2089 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr13;
					}
					{
						goto _ctr67;
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
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr14;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr14;
						}
					} else {
						goto _ctr14;
					}
					{
						goto _ctr9;
					}
					_ctr14:
					{
#line 36 "lex/ragel_numeric.rl"
						(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
					
#line 2139 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
					if ( ( (*( p))) < 65 ) {
						if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
							goto _ctr14;
						}
					} else if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr14;
						}
					} else {
						goto _ctr14;
					}
					{
						goto _ctr68;
					}
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr16;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr18;
						}
						case 102: {
							goto _st34;
						}
						case 110: {
							goto _st37;
						}
						case 116: {
							goto _st38;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr51;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st33;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st33;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr16;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr18;
						}
						case 97: {
							goto _st35;
						}
						case 102: {
							goto _st34;
						}
						case 110: {
							goto _st37;
						}
						case 116: {
							goto _st38;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr51;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st33;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st33;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr16;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr18;
						}
						case 102: {
							goto _st34;
						}
						case 108: {
							goto _st36;
						}
						case 110: {
							goto _st37;
						}
						case 116: {
							goto _st38;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr51;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st33;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st33;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr16;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr18;
						}
						case 102: {
							goto _st34;
						}
						case 110: {
							goto _st37;
						}
						case 115: {
							goto _ctr74;
						}
						case 116: {
							goto _st38;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr51;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st33;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st33;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr16;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr18;
						}
						case 102: {
							goto _st34;
						}
						case 110: {
							goto _st37;
						}
						case 116: {
							goto _st38;
						}
						case 117: {
							goto _st41;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr51;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st33;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st33;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr16;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr18;
						}
						case 102: {
							goto _st34;
						}
						case 110: {
							goto _st37;
						}
						case 114: {
							goto _st39;
						}
						case 116: {
							goto _st38;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr51;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st33;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st33;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr16;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr18;
						}
						case 102: {
							goto _st34;
						}
						case 110: {
							goto _st37;
						}
						case 116: {
							goto _st38;
						}
						case 117: {
							goto _ctr74;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr51;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st33;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st33;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr74:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2624 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr16;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr18;
						}
						case 101: {
							goto _st7;
						}
						case 102: {
							goto _st34;
						}
						case 110: {
							goto _st37;
						}
						case 116: {
							goto _st38;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr51;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st33;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st33;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
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
						case 40: {
							goto _ctr16;
						}
						case 91: {
							goto _ctr18;
						}
					}
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _st7;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st7;
					}
					{
						goto _ctr15;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr16;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr18;
						}
						case 102: {
							goto _st34;
						}
						case 108: {
							goto _ctr77;
						}
						case 110: {
							goto _st37;
						}
						case 116: {
							goto _st38;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr51;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st33;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st33;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr77:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2791 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr16;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr18;
						}
						case 102: {
							goto _st34;
						}
						case 108: {
							goto _st7;
						}
						case 110: {
							goto _st37;
						}
						case 116: {
							goto _st38;
						}
					}
					if ( ( (*( p))) < 48 ) {
						if ( ( (*( p))) > 13 ) {
							if ( 39 <= ( (*( p))) && ( (*( p))) <= 41 ) {
								goto _ctr51;
							}
						} else if ( ( (*( p))) >= 9 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 57 ) {
						if ( ( (*( p))) > 90 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _st33;
							}
						} else if ( ( (*( p))) >= 65 ) {
							goto _st33;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr43:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2864 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 2870 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st43;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
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
						case 40: {
							goto _ctr20;
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
						goto _ctr19;
					}
					_ctr78:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 2968 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 2974 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 97: {
							goto _ctr81;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr81:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3046 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3052 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 108: {
							goto _ctr82;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr82:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3124 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3130 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 115: {
							goto _ctr83;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr79:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3202 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3208 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
						case 117: {
							goto _ctr84;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr80:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3280 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3286 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st48;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 114: {
							goto _ctr85;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr85:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3358 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3364 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st49;
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
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
						case 117: {
							goto _ctr83;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr83:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3436 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3442 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st50;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 101: {
							goto _st8;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr84:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3514 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3520 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st51;
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
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 108: {
							goto _ctr86;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr86:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3592 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3598 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st52;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 108: {
							goto _st8;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr44:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3670 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3676 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st53;
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
					switch( ( (*( p))) ) {
						case 32: {
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 97: {
							goto _ctr87;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr87:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3748 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3754 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st54;
					_st54:
					if ( p == eof ) {
						if ( cs >= 9 )
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 108: {
							goto _ctr88;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr88:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3826 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3832 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 115: {
							goto _ctr89;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr89:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3904 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 3910 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st56;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 101: {
							goto _ctr90;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr90:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3982 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 104 "lex/lemon_data_string_parser.rl"
						{act = 12;}}
					
#line 3988 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st57;
					_ctr93:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 3996 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 105 "lex/lemon_data_string_parser.rl"
						{act = 13;}}
					
#line 4002 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st57;
					_ctr96:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4010 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 103 "lex/lemon_data_string_parser.rl"
						{act = 11;}}
					
#line 4016 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st57;
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
						case 40: {
							goto _ctr20;
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
						goto _ctr19;
					}
					_ctr45:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4053 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 4059 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st58;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
						case 117: {
							goto _ctr91;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr91:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4131 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 4137 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st59;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 108: {
							goto _ctr92;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr92:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4209 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 4215 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st60;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 108: {
							goto _ctr93;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr46:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4287 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 4293 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st61;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 114: {
							goto _ctr94;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr94:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4365 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 4371 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st62;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
						case 117: {
							goto _ctr95;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
					}
					_ctr95:
					{
#line 1 "NONE"
						{te = p+1;}}
					
#line 4443 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					{
#line 152 "lex/lemon_data_string_parser.rl"
						{act = 25;}}
					
#line 4449 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
					
					goto _st63;
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
							goto _ctr51;
						}
						case 34: {
							goto _ctr51;
						}
						case 40: {
							goto _ctr20;
						}
						case 43: {
							goto _ctr47;
						}
						case 45: {
							goto _ctr47;
						}
						case 91: {
							goto _ctr51;
						}
						case 95: {
							goto _ctr43;
						}
						case 101: {
							goto _ctr96;
						}
						case 102: {
							goto _ctr78;
						}
						case 110: {
							goto _ctr79;
						}
						case 116: {
							goto _ctr80;
						}
					}
					if ( ( (*( p))) < 39 ) {
						if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
							goto _ctr51;
						}
					} else if ( ( (*( p))) > 41 ) {
						if ( ( (*( p))) > 57 ) {
							if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
								goto _ctr43;
							}
						} else if ( ( (*( p))) >= 48 ) {
							goto _st8;
						}
					} else {
						goto _ctr51;
					}
					{
						goto _ctr31;
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
					_test_eof16: cs = 16; goto _test_eof; 
					_test_eof17: cs = 17; goto _test_eof; 
					_test_eof18: cs = 18; goto _test_eof; 
					_test_eof19: cs = 19; goto _test_eof; 
					_test_eof20: cs = 20; goto _test_eof; 
					_test_eof21: cs = 21; goto _test_eof; 
					_test_eof22: cs = 22; goto _test_eof; 
					_test_eof23: cs = 23; goto _test_eof; 
					_test_eof24: cs = 24; goto _test_eof; 
					_test_eof25: cs = 25; goto _test_eof; 
					_test_eof3: cs = 3; goto _test_eof; 
					_test_eof26: cs = 26; goto _test_eof; 
					_test_eof4: cs = 4; goto _test_eof; 
					_test_eof27: cs = 27; goto _test_eof; 
					_test_eof28: cs = 28; goto _test_eof; 
					_test_eof29: cs = 29; goto _test_eof; 
					_test_eof30: cs = 30; goto _test_eof; 
					_test_eof5: cs = 5; goto _test_eof; 
					_test_eof31: cs = 31; goto _test_eof; 
					_test_eof6: cs = 6; goto _test_eof; 
					_test_eof32: cs = 32; goto _test_eof; 
					_test_eof33: cs = 33; goto _test_eof; 
					_test_eof34: cs = 34; goto _test_eof; 
					_test_eof35: cs = 35; goto _test_eof; 
					_test_eof36: cs = 36; goto _test_eof; 
					_test_eof37: cs = 37; goto _test_eof; 
					_test_eof38: cs = 38; goto _test_eof; 
					_test_eof39: cs = 39; goto _test_eof; 
					_test_eof40: cs = 40; goto _test_eof; 
					_test_eof7: cs = 7; goto _test_eof; 
					_test_eof41: cs = 41; goto _test_eof; 
					_test_eof42: cs = 42; goto _test_eof; 
					_test_eof43: cs = 43; goto _test_eof; 
					_test_eof8: cs = 8; goto _test_eof; 
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
								
#line 4592 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
								
#line 4607 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
								break;
							}
							case 2: {
								break;
							}
							case 12: {
								{
#line 1 "NONE"
									{ts = p;}}
								
#line 4619 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
								
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
							case 3: {
								break;
							}
							case 26: {
								break;
							}
							case 4: {
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
							case 5: {
								break;
							}
							case 31: {
								break;
							}
							case 6: {
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
							case 7: {
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
							case 8: {
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
							goto _ctr19;case 14:
							goto _ctr51;case 15:
							goto _ctr51;case 16:
							goto _ctr51;case 17:
							goto _ctr51;case 18:
							goto _ctr51;case 19:
							goto _ctr51;case 20:
							goto _ctr51;case 21:
							goto _ctr51;case 22:
							goto _ctr51;case 23:
							goto _ctr58;case 24:
							goto _ctr51;case 25:
							goto _ctr60;case 3:
							goto _ctr9;case 26:
							goto _ctr63;case 4:
							goto _ctr9;case 27:
							goto _ctr64;case 28:
							goto _ctr64;case 29:
							goto _ctr60;case 30:
							goto _ctr60;case 5:
							goto _ctr9;case 31:
							goto _ctr67;case 6:
							goto _ctr9;case 32:
							goto _ctr68;case 33:
							goto _ctr51;case 34:
							goto _ctr51;case 35:
							goto _ctr51;case 36:
							goto _ctr51;case 37:
							goto _ctr51;case 38:
							goto _ctr51;case 39:
							goto _ctr51;case 40:
							goto _ctr51;case 7:
							goto _ctr15;case 41:
							goto _ctr51;case 42:
							goto _ctr51;case 43:
							goto _ctr51;case 8:
							goto _ctr19;case 44:
							goto _ctr51;case 45:
							goto _ctr51;case 46:
							goto _ctr51;case 47:
							goto _ctr51;case 48:
							goto _ctr51;case 49:
							goto _ctr51;case 50:
							goto _ctr51;case 51:
							goto _ctr51;case 52:
							goto _ctr51;case 53:
							goto _ctr51;case 54:
							goto _ctr51;case 55:
							goto _ctr51;case 56:
							goto _ctr51;case 57:
							goto _ctr19;case 58:
							goto _ctr51;case 59:
							goto _ctr51;case 60:
							goto _ctr51;case 61:
							goto _ctr51;case 62:
							goto _ctr51;case 63:
							goto _ctr51;	}
					}
					
					if ( cs >= 9 )
						goto _out; _pop: {}
					_out: {}
				}
				
#line 242 "lex/lemon_data_string_parser.rl"
				
				
			} catch(std::exception& e) {
				setErrorMsg(e.what());
				return false;
			}
			
			if (cs < 
#line 4879 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
			9
#line 249 "lex/lemon_data_string_parser.rl"
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
