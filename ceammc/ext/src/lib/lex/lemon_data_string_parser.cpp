#line 1 "lex/lemon_data_string_parser.rl"
# include "lemon_data_string_parser.h"
# include "lemon_data_string.h"
# include "parser_numeric.h"

# include <cstdint>

# ifdef NDEBUG
# undef NDEBUG
# endif
# include "lemon_data_parser_impl.h"

namespace {
	
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


#line 107 "lex/lemon_data_string_parser.rl"


# include <cstring>

namespace ceammc {
	namespace parser {
		
		
#line 54 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
		static const int lemon_data_string_lexer_start = 1;
		static const int lemon_data_string_lexer_first_final = 38;
		static const int lemon_data_string_lexer_error = 0;
		
		static const int lemon_data_string_lexer_en_main = 1;
		
		
#line 114 "lex/lemon_data_string_parser.rl"
		
		
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
			std::cerr << __FUNCTION__ << ' ' << token << '\n';
			lemon_data_string_parser(parser(), token, {}, this);
		}
		
		void LemonDataStringParser::pushDouble(double val)
		{
			std::cerr << __FUNCTION__ << " TK_FLOAT " << val << '\n';
			lemon_data_string_parser(parser(), TK_FLOAT, val, this);
		}
		
		void LemonDataStringParser::pushSymbolToken(int token, const char* begin, const char* end)
		{
			const auto N = std::min<int>(sizeof(parser_buf_) - 1, end - begin);
			
			for (int i = 0; i < N; i++)
			parser_buf_[i] = begin[i];
			
			parser_buf_[N] = 0;
			std::cerr << __FUNCTION__ << " token '" << parser_buf_ << "'\n";
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
			if (data == nullptr)
				return false;
			
			if (data[0] == '\0')
				return true;
			
			int cs;
			int act;
			const char* ts = 0;
			const char* te = 0;
			const char* p = data;
			parse_ok_ = true;
			
			t_float token_float = 0;
			
			DECLARE_RAGEL_COMMON_VARS;
			DECLARE_RAGEL_NUMERIC_VARS;
			
			
#line 138 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
			{
				cs = (int)lemon_data_string_lexer_start;
			}
			
#line 188 "lex/lemon_data_string_parser.rl"
			
			
#line 146 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
			{
				switch ( cs ) {
					case 1:
					goto st_case_1;
					case 0:
					goto st_case_0;
					case 2:
					goto st_case_2;
					case 38:
					goto st_case_38;
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
				}
				goto st_out;
				p += 1;
				st_case_1:
				switch( ( (*( p))) ) {
					case 32: {
						goto _st2;
					}
					case 40: {
						goto _st3;
					}
					case 41: {
						goto _st5;
					}
					case 43: {
						goto _ctr5;
					}
					case 45: {
						goto _ctr5;
					}
					case 48: {
						goto _ctr6;
					}
					case 64: {
						goto _ctr8;
					}
					case 91: {
						goto _st21;
					}
					case 93: {
						goto _st22;
					}
					case 102: {
						goto _ctr12;
					}
					case 110: {
						goto _ctr13;
					}
					case 116: {
						goto _ctr14;
					}
				}
				if ( ( (*( p))) < 49 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st2;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr11;
					}
				} else {
					goto _ctr7;
				}
				{
					goto _st0;
				}
				st_case_0:
				_st0:
				cs = 0;
				goto _pop;
				_ctr16:
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 294 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st2;
				_st2:
				p += 1;
				st_case_2:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr15;
					}
					case 32: {
						goto _ctr16;
					}
					case 40: {
						goto _st3;
					}
					case 41: {
						goto _st5;
					}
					case 43: {
						goto _ctr5;
					}
					case 45: {
						goto _ctr5;
					}
					case 48: {
						goto _ctr6;
					}
					case 64: {
						goto _ctr8;
					}
					case 91: {
						goto _st21;
					}
					case 93: {
						goto _st22;
					}
					case 102: {
						goto _ctr12;
					}
					case 110: {
						goto _ctr13;
					}
					case 116: {
						goto _ctr14;
					}
				}
				if ( ( (*( p))) < 49 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr16;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr11;
					}
				} else {
					goto _ctr7;
				}
				{
					goto _st0;
				}
				_ctr15:
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 362 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 368 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr17:
				{
#line 89 "lex/lemon_data_string_parser.rl"
					pushToken(TK_LIST_OPEN);  }
				
#line 376 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 384 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 390 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr20:
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 398 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr21:
				{
#line 90 "lex/lemon_data_string_parser.rl"
					pushToken(TK_LIST_CLOSE); }
				
#line 406 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 414 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 420 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr26:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 432 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 55 "lex/lemon_data_string_parser.rl"
					
					switch(ragel_type) {
						case TYPE_FLOAT:
						pushDouble(ragel_num.getFloat());
						break;
						case TYPE_INT:
						case TYPE_BIN:
						case TYPE_HEX:
						pushDouble(ragel_num.getInteger());
						break;
						case TYPE_RATIO:
						pushDouble(ragel_num.getRatioAsFloat());
						break;
						default:
						break;
					}
				}
				
#line 454 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 462 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 468 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr33:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 481 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 55 "lex/lemon_data_string_parser.rl"
					
					switch(ragel_type) {
						case TYPE_FLOAT:
						pushDouble(ragel_num.getFloat());
						break;
						case TYPE_INT:
						case TYPE_BIN:
						case TYPE_HEX:
						pushDouble(ragel_num.getInteger());
						break;
						case TYPE_RATIO:
						pushDouble(ragel_num.getRatioAsFloat());
						break;
						default:
						break;
					}
				}
				
#line 503 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 511 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 517 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr38:
				{
#line 44 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num *= ragel_num.sign;
					ragel_type = TYPE_RATIO;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 529 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 55 "lex/lemon_data_string_parser.rl"
					
					switch(ragel_type) {
						case TYPE_FLOAT:
						pushDouble(ragel_num.getFloat());
						break;
						case TYPE_INT:
						case TYPE_BIN:
						case TYPE_HEX:
						pushDouble(ragel_num.getInteger());
						break;
						case TYPE_RATIO:
						pushDouble(ragel_num.getRatioAsFloat());
						break;
						default:
						break;
					}
				}
				
#line 551 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 559 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 565 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr47:
				{
#line 24 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_BIN;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 576 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 55 "lex/lemon_data_string_parser.rl"
					
					switch(ragel_type) {
						case TYPE_FLOAT:
						pushDouble(ragel_num.getFloat());
						break;
						case TYPE_INT:
						case TYPE_BIN:
						case TYPE_HEX:
						pushDouble(ragel_num.getInteger());
						break;
						case TYPE_RATIO:
						pushDouble(ragel_num.getRatioAsFloat());
						break;
						default:
						break;
					}
				}
				
#line 598 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 606 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 612 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr51:
				{
#line 37 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_HEX;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 623 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 55 "lex/lemon_data_string_parser.rl"
					
					switch(ragel_type) {
						case TYPE_FLOAT:
						pushDouble(ragel_num.getFloat());
						break;
						case TYPE_INT:
						case TYPE_BIN:
						case TYPE_HEX:
						pushDouble(ragel_num.getInteger());
						break;
						case TYPE_RATIO:
						pushDouble(ragel_num.getRatioAsFloat());
						break;
						default:
						break;
					}
				}
				
#line 645 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 653 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 659 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr55:
				{
#line 52 "lex/lemon_data_string_parser.rl"
					
					pushSymbolToken(TK_PROPERTY, ts, p);
				}
				
#line 669 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 677 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 683 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr57:
				{
#line 91 "lex/lemon_data_string_parser.rl"
					pushToken(TK_DICT_OPEN);  }
				
#line 691 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 699 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 705 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr59:
				{
#line 92 "lex/lemon_data_string_parser.rl"
					pushToken(TK_DICT_CLOSE); }
				
#line 713 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 721 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 727 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr63:
				{
#line 47 "lex/lemon_data_string_parser.rl"
					
					pushSymbolToken(TK_FUNC_LIST_CALL, ts, p-1);
					pushToken(TK_LIST_OPEN);
				}
				
#line 738 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 746 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 752 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr70:
				{
#line 87 "lex/lemon_data_string_parser.rl"
					pushDouble(0); }
				
#line 760 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 768 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 774 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr76:
				{
#line 88 "lex/lemon_data_string_parser.rl"
					pushToken(TK_NULL); }
				
#line 782 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 790 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 796 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_ctr82:
				{
#line 86 "lex/lemon_data_string_parser.rl"
					pushDouble(1); }
				
#line 804 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 812 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 105 "lex/lemon_data_string_parser.rl"
					pushToken(0); {p+= 1; cs = 38; goto _out;} }
				
#line 818 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st38;
				_st38:
				p += 1;
				st_case_38:
				{
					goto _st0;
				}
				_st3:
				p += 1;
				st_case_3:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr17;
					}
					case 32: {
						goto _ctr18;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _ctr18;
				}
				{
					goto _st0;
				}
				_ctr18:
				{
#line 89 "lex/lemon_data_string_parser.rl"
					pushToken(TK_LIST_OPEN);  }
				
#line 849 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 857 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr22:
				{
#line 90 "lex/lemon_data_string_parser.rl"
					pushToken(TK_LIST_CLOSE); }
				
#line 865 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 873 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr27:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 885 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 55 "lex/lemon_data_string_parser.rl"
					
					switch(ragel_type) {
						case TYPE_FLOAT:
						pushDouble(ragel_num.getFloat());
						break;
						case TYPE_INT:
						case TYPE_BIN:
						case TYPE_HEX:
						pushDouble(ragel_num.getInteger());
						break;
						case TYPE_RATIO:
						pushDouble(ragel_num.getRatioAsFloat());
						break;
						default:
						break;
					}
				}
				
#line 907 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 915 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr34:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 928 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 55 "lex/lemon_data_string_parser.rl"
					
					switch(ragel_type) {
						case TYPE_FLOAT:
						pushDouble(ragel_num.getFloat());
						break;
						case TYPE_INT:
						case TYPE_BIN:
						case TYPE_HEX:
						pushDouble(ragel_num.getInteger());
						break;
						case TYPE_RATIO:
						pushDouble(ragel_num.getRatioAsFloat());
						break;
						default:
						break;
					}
				}
				
#line 950 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 958 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr39:
				{
#line 44 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num *= ragel_num.sign;
					ragel_type = TYPE_RATIO;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 970 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 55 "lex/lemon_data_string_parser.rl"
					
					switch(ragel_type) {
						case TYPE_FLOAT:
						pushDouble(ragel_num.getFloat());
						break;
						case TYPE_INT:
						case TYPE_BIN:
						case TYPE_HEX:
						pushDouble(ragel_num.getInteger());
						break;
						case TYPE_RATIO:
						pushDouble(ragel_num.getRatioAsFloat());
						break;
						default:
						break;
					}
				}
				
#line 992 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 1000 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr48:
				{
#line 24 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_BIN;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1011 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 55 "lex/lemon_data_string_parser.rl"
					
					switch(ragel_type) {
						case TYPE_FLOAT:
						pushDouble(ragel_num.getFloat());
						break;
						case TYPE_INT:
						case TYPE_BIN:
						case TYPE_HEX:
						pushDouble(ragel_num.getInteger());
						break;
						case TYPE_RATIO:
						pushDouble(ragel_num.getRatioAsFloat());
						break;
						default:
						break;
					}
				}
				
#line 1033 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 1041 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr52:
				{
#line 37 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_HEX;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1052 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 55 "lex/lemon_data_string_parser.rl"
					
					switch(ragel_type) {
						case TYPE_FLOAT:
						pushDouble(ragel_num.getFloat());
						break;
						case TYPE_INT:
						case TYPE_BIN:
						case TYPE_HEX:
						pushDouble(ragel_num.getInteger());
						break;
						case TYPE_RATIO:
						pushDouble(ragel_num.getRatioAsFloat());
						break;
						default:
						break;
					}
				}
				
#line 1074 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 1082 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr56:
				{
#line 52 "lex/lemon_data_string_parser.rl"
					
					pushSymbolToken(TK_PROPERTY, ts, p);
				}
				
#line 1092 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 1100 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr58:
				{
#line 91 "lex/lemon_data_string_parser.rl"
					pushToken(TK_DICT_OPEN);  }
				
#line 1108 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 1116 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr60:
				{
#line 92 "lex/lemon_data_string_parser.rl"
					pushToken(TK_DICT_CLOSE); }
				
#line 1124 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 1132 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr64:
				{
#line 47 "lex/lemon_data_string_parser.rl"
					
					pushSymbolToken(TK_FUNC_LIST_CALL, ts, p-1);
					pushToken(TK_LIST_OPEN);
				}
				
#line 1143 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 1151 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr71:
				{
#line 87 "lex/lemon_data_string_parser.rl"
					pushDouble(0); }
				
#line 1159 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 1167 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr77:
				{
#line 88 "lex/lemon_data_string_parser.rl"
					pushToken(TK_NULL); }
				
#line 1175 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 1183 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_ctr83:
				{
#line 86 "lex/lemon_data_string_parser.rl"
					pushDouble(1); }
				
#line 1191 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 72 "lex/lemon_data_string_parser.rl"
					
					token_float = 0; ragel_num = {}; ragel_cat = CAT_UNKNOWN; ragel_type = TYPE_UNKNOWN;
				}
				
#line 1199 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st4;
				_st4:
				p += 1;
				st_case_4:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr20;
					}
					case 32: {
						goto _st2;
					}
					case 40: {
						goto _st3;
					}
					case 41: {
						goto _st5;
					}
					case 43: {
						goto _ctr5;
					}
					case 45: {
						goto _ctr5;
					}
					case 48: {
						goto _ctr6;
					}
					case 64: {
						goto _ctr8;
					}
					case 91: {
						goto _st21;
					}
					case 93: {
						goto _st22;
					}
					case 102: {
						goto _ctr12;
					}
					case 110: {
						goto _ctr13;
					}
					case 116: {
						goto _ctr14;
					}
				}
				if ( ( (*( p))) < 49 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st2;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr11;
					}
				} else {
					goto _ctr7;
				}
				{
					goto _st0;
				}
				_st5:
				p += 1;
				st_case_5:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr21;
					}
					case 32: {
						goto _ctr22;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _ctr22;
				}
				{
					goto _st0;
				}
				_ctr5:
				{
#line 21 "lex/ragel_numeric.rl"
					ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
				
#line 1282 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st6;
				_st6:
				p += 1;
				st_case_6:
				if ( ( (*( p))) == 48 ) {
					goto _ctr24;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr7;
				}
				{
					goto _st0;
				}
				_ctr24:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 1302 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 1308 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st7;
				_st7:
				p += 1;
				st_case_7:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr26;
					}
					case 32: {
						goto _ctr27;
					}
					case 46: {
						goto _ctr28;
					}
					case 47: {
						goto _st10;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _ctr27;
				}
				{
					goto _st0;
				}
				_ctr28:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1343 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 50 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num = 0;
					ragel_num.ratio.den = 1;
				}
				
#line 1352 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st8;
				_st8:
				p += 1;
				st_case_8:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr31;
				}
				{
					goto _st0;
				}
				_ctr31:
				{
#line 54 "lex/ragel_numeric.rl"
					
					(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
					ragel_num.ratio.den *= 10;
				}
				
#line 1372 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st9;
				_st9:
				p += 1;
				st_case_9:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr33;
					}
					case 32: {
						goto _ctr34;
					}
				}
				if ( ( (*( p))) > 13 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr31;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr34;
				}
				{
					goto _st0;
				}
				_st10:
				p += 1;
				st_case_10:
				if ( ( (*( p))) == 48 ) {
					goto _ctr35;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr36;
				}
				{
					goto _st0;
				}
				_ctr35:
				{
#line 43 "lex/ragel_numeric.rl"
					(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
				
#line 1413 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st11;
				_st11:
				p += 1;
				st_case_11:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr38;
					}
					case 32: {
						goto _ctr39;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _ctr39;
				}
				{
					goto _st0;
				}
				_ctr36:
				{
#line 43 "lex/ragel_numeric.rl"
					(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
				
#line 1438 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st12;
				_st12:
				p += 1;
				st_case_12:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr38;
					}
					case 32: {
						goto _ctr39;
					}
				}
				if ( ( (*( p))) > 13 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr36;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr39;
				}
				{
					goto _st0;
				}
				_ctr7:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 1467 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 1473 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st13;
				_st13:
				p += 1;
				st_case_13:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr26;
					}
					case 32: {
						goto _ctr27;
					}
					case 46: {
						goto _ctr28;
					}
					case 47: {
						goto _st10;
					}
				}
				if ( ( (*( p))) > 13 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr7;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr27;
				}
				{
					goto _st0;
				}
				_ctr6:
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 1508 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 1514 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st14;
				_st14:
				p += 1;
				st_case_14:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr26;
					}
					case 32: {
						goto _ctr27;
					}
					case 46: {
						goto _ctr28;
					}
					case 47: {
						goto _st10;
					}
					case 98: {
						goto _st15;
					}
					case 120: {
						goto _st17;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _ctr27;
				}
				{
					goto _st0;
				}
				_st15:
				p += 1;
				st_case_15:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
					goto _ctr45;
				}
				{
					goto _st0;
				}
				_ctr45:
				{
#line 23 "lex/ragel_numeric.rl"
					(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
				
#line 1560 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st16;
				_st16:
				p += 1;
				st_case_16:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr47;
					}
					case 32: {
						goto _ctr48;
					}
				}
				if ( ( (*( p))) > 13 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 49 ) {
						goto _ctr45;
					}
				} else if ( ( (*( p))) >= 9 ) {
					goto _ctr48;
				}
				{
					goto _st0;
				}
				_st17:
				p += 1;
				st_case_17:
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr49;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr49;
					}
				} else {
					goto _ctr49;
				}
				{
					goto _st0;
				}
				_ctr49:
				{
#line 36 "lex/ragel_numeric.rl"
					(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
				
#line 1606 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st18;
				_st18:
				p += 1;
				st_case_18:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr51;
					}
					case 32: {
						goto _ctr52;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr52;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( ( (*( p))) > 70 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
							goto _ctr49;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _ctr49;
					}
				} else {
					goto _ctr49;
				}
				{
					goto _st0;
				}
				_ctr8:
				{
#line 51 "lex/lemon_data_string_parser.rl"
					ts = p; }
				
#line 1643 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st19;
				_st19:
				p += 1;
				st_case_19:
				switch( ( (*( p))) ) {
					case 63: {
						goto _st20;
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
					goto _st0;
				}
				_st20:
				p += 1;
				st_case_20:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr55;
					}
					case 32: {
						goto _ctr56;
					}
					case 63: {
						goto _st20;
					}
					case 95: {
						goto _st20;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr56;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st20;
					}
				} else {
					goto _st20;
				}
				{
					goto _st0;
				}
				_st21:
				p += 1;
				st_case_21:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr57;
					}
					case 32: {
						goto _ctr58;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _ctr58;
				}
				{
					goto _st0;
				}
				_st22:
				p += 1;
				st_case_22:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr59;
					}
					case 32: {
						goto _ctr60;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _ctr60;
				}
				{
					goto _st0;
				}
				_ctr11:
				{
#line 46 "lex/lemon_data_string_parser.rl"
					ts = p; }
				
#line 1737 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st23;
				_st23:
				p += 1;
				st_case_23:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st24:
				p += 1;
				st_case_24:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr63;
					}
					case 32: {
						goto _ctr64;
					}
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _ctr64;
				}
				{
					goto _st0;
				}
				_ctr12:
				{
#line 46 "lex/lemon_data_string_parser.rl"
					ts = p; }
				
#line 1783 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st25;
				_st25:
				p += 1;
				st_case_25:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
					case 97: {
						goto _st26;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st26:
				p += 1;
				st_case_26:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
					case 108: {
						goto _st27;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st27:
				p += 1;
				st_case_27:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
					case 115: {
						goto _st28;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st28:
				p += 1;
				st_case_28:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
					case 101: {
						goto _st29;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st29:
				p += 1;
				st_case_29:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr70;
					}
					case 32: {
						goto _ctr71;
					}
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr71;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else {
					goto _st23;
				}
				{
					goto _st0;
				}
				_ctr13:
				{
#line 46 "lex/lemon_data_string_parser.rl"
					ts = p; }
				
#line 1918 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st30;
				_st30:
				p += 1;
				st_case_30:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
					case 117: {
						goto _st31;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st31:
				p += 1;
				st_case_31:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
					case 108: {
						goto _st32;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st32:
				p += 1;
				st_case_32:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
					case 108: {
						goto _st33;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st33:
				p += 1;
				st_case_33:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr76;
					}
					case 32: {
						goto _ctr77;
					}
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr77;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else {
					goto _st23;
				}
				{
					goto _st0;
				}
				_ctr14:
				{
#line 46 "lex/lemon_data_string_parser.rl"
					ts = p; }
				
#line 2029 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
				
				goto _st34;
				_st34:
				p += 1;
				st_case_34:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
					case 114: {
						goto _st35;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st35:
				p += 1;
				st_case_35:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
					case 117: {
						goto _st36;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st36:
				p += 1;
				st_case_36:
				switch( ( (*( p))) ) {
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
					case 101: {
						goto _st37;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st23;
				}
				{
					goto _st0;
				}
				_st37:
				p += 1;
				st_case_37:
				switch( ( (*( p))) ) {
					case 0: {
						goto _ctr82;
					}
					case 32: {
						goto _ctr83;
					}
					case 40: {
						goto _st24;
					}
					case 95: {
						goto _st23;
					}
				}
				if ( ( (*( p))) < 48 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _ctr83;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st23;
					}
				} else {
					goto _st23;
				}
				{
					goto _st0;
				}
				st_out:
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof38: cs = 38; goto _test_eof; 
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
				
				_test_eof: {}
				if ( cs >= 38 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 189 "lex/lemon_data_string_parser.rl"
			
			
			if (cs < 
#line 2185 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lemon_data_string_parser.cpp"
			38
#line 191 "lex/lemon_data_string_parser.rl"
			) {
				char buf[32] = "";
				snprintf(buf, sizeof(buf)-1, "unknown token: '%s'", p);
				setErrorMsg(buf);
				return false;
			} else {
				if (!parse_ok_) {
					pushToken(0);
					return false;
				} else
				return true;
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
