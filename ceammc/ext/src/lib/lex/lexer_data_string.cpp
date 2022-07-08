#line 1 "lex/lexer_data_string.rl"
# include "lexer_data_string.h"
# include "parser_numeric.h"
# include "data_string_parser2.h"
# include "data_string_location2.h"

# include <cstdint>

#define VERBOSE

# ifdef VERBOSE
# define DUMP_TOKEN(token, var) std::cerr << "[token] " #token << ' ' << var << std::endl;
# else
# define DUMP_TOKEN(token)
# endif

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


#line 48 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
static const int data_string_lexer_start = 6;
static const int data_string_lexer_first_final = 6;
static const int data_string_lexer_error = 0;

static const int data_string_lexer_en_main = 6;


#line 124 "lex/lexer_data_string.rl"


# include <cstring>

namespace ceammc {
	namespace parser {
		
		static boost::string_view trim(boost::string_view src)
		{
			constexpr auto SPACES = " \t\n";
			const auto first_ns = src.find_first_not_of(SPACES);
			
			if (first_ns != std::string::npos)
				return src.substr(first_ns, src.find_last_not_of(SPACES) - first_ns + 1);
			else
				return "";
		}
		
		DataStringLexer::DataStringLexer(string_view src)
		: source_(trim(src))
		, p(source_.begin())
		, ts(source_.begin())
		, te(source_.begin())
		, pe(source_.end())
		, eof(source_.end())
		, cs(data_string_lexer_start)
		, act(0)
		{
			
#line 86 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
			{
				cs = (int)data_string_lexer_start;
				ts = 0;
				te = 0;
				act = 0;
			}
			
#line 152 "lex/lexer_data_string.rl"
			
		}
		
		void DataStringLexer::saveFunction()
		{
			const auto NMAX = std::min<int>(sizeof(text_) - 1, (te - ts) - 1);
			for (int i = 0; i < NMAX; i++)
			text_[i] = ts[i];
			
			text_[NMAX] = '\0';
		}
		
		void DataStringLexer::saveSymbol()
		{
			const auto NMAX = std::min<int>(sizeof(text_) - 1, (te - ts));
			for (int i = 0; i < NMAX; i++)
			text_[i] = ts[i];
			
			text_[NMAX] = '\0';
		}
		
		location DataStringLexer::tokenRange() const
		{
			int start = ts - source_.begin();
			int end = te - source_.begin();
			//printf("loc: %d..%d\n", start, end);
			return location{ position{&filename_, start}, position{&filename_, end }};
		}
		
		DataStringLexer::parser::symbol_type DataStringLexer::exec()
		{
			if (p == pe)
				return parser::make_END(tokenRange());
			
			DECLARE_RAGEL_COMMON_VARS;
			DECLARE_RAGEL_NUMERIC_VARS;
			t_float token_float = 0;
			
			
#line 134 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
			{
				if ( p == pe )
					goto _test_eof;
				switch ( cs ) {
					case 6:
					goto st_case_6;
					case 0:
					goto st_case_0;
					case 7:
					goto st_case_7;
					case 1:
					goto st_case_1;
					case 8:
					goto st_case_8;
					case 2:
					goto st_case_2;
					case 9:
					goto st_case_9;
					case 3:
					goto st_case_3;
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
					case 4:
					goto st_case_4;
					case 21:
					goto st_case_21;
					case 5:
					goto st_case_5;
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
				}
				goto st_out;
				_ctr4:
				{
#line 87 "lex/lexer_data_string.rl"
					{p = ((te))-1;
						{
#line 87 "lex/lexer_data_string.rl"
							
							DUMP_TOKEN(FLOAT, token_float);
							return parser::make_FLOAT(token_float, tokenRange());
						}
					}}
				
#line 219 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr11:
				{
#line 104 "lex/lexer_data_string.rl"
					{te = p+1;{
#line 104 "lex/lexer_data_string.rl"
							
							saveFunction();
							DUMP_TOKEN(DATA_CALL_LIST, text_);
							return parser::make_DATA_CALL_LIST(text_, location());
						}
					}}
				
#line 234 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr12:
				{
#line 110 "lex/lexer_data_string.rl"
					{te = p+1;{
#line 110 "lex/lexer_data_string.rl"
							
							saveFunction();
							DUMP_TOKEN(DATA_CALL_DICT, text_);
							return parser::make_DATA_CALL_DICT(text_, location());
						}
					}}
				
#line 249 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr15:
				{
#line 82 "lex/lexer_data_string.rl"
					{te = p+1;{
#line 82 "lex/lexer_data_string.rl"
							return parser::make_LIST_OPEN(tokenRange()); }
					}}
				
#line 260 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr16:
				{
#line 83 "lex/lexer_data_string.rl"
					{te = p+1;{
#line 83 "lex/lexer_data_string.rl"
							return parser::make_LIST_CLOSE(tokenRange()); }
					}}
				
#line 271 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr21:
				{
#line 84 "lex/lexer_data_string.rl"
					{te = p+1;{
#line 84 "lex/lexer_data_string.rl"
							return parser::make_DICT_OPEN(tokenRange()); }
					}}
				
#line 282 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr22:
				{
#line 85 "lex/lexer_data_string.rl"
					{te = p+1;{
#line 85 "lex/lexer_data_string.rl"
							return parser::make_DICT_CLOSE(tokenRange()); }
					}}
				
#line 293 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr27:
				{
#line 78 "lex/lexer_data_string.rl"
					{te = p;p = p - 1;{
#line 78 "lex/lexer_data_string.rl"
							DUMP_TOKEN(SPACE, ' '); return parser::make_SPACE(tokenRange()); }
					}}
				
#line 304 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr28:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 316 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 49 "lex/lexer_data_string.rl"
					
					t_float v = 0;
					switch(ragel_type) {
						case TYPE_HEX:
						case TYPE_BIN:
						case TYPE_INT:
						token_float = ragel_num.getInteger();
						break;
						case TYPE_FLOAT:
						token_float = ragel_num.getFloat();
						break;
						case TYPE_RATIO:
						token_float = ragel_num.getRatioAsFloat();
						break;
						default:
						return parser::make_LEXER_ERROR(tokenRange());
						break;
					}
				}
				
#line 340 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 87 "lex/lexer_data_string.rl"
					{te = p;p = p - 1;{
#line 87 "lex/lexer_data_string.rl"
							
							DUMP_TOKEN(FLOAT, token_float);
							return parser::make_FLOAT(token_float, tokenRange());
						}
					}}
				
#line 352 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr31:
				{
#line 58 "lex/ragel_numeric.rl"
					
					const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
					ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
					ragel_type = TYPE_FLOAT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 365 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 49 "lex/lexer_data_string.rl"
					
					t_float v = 0;
					switch(ragel_type) {
						case TYPE_HEX:
						case TYPE_BIN:
						case TYPE_INT:
						token_float = ragel_num.getInteger();
						break;
						case TYPE_FLOAT:
						token_float = ragel_num.getFloat();
						break;
						case TYPE_RATIO:
						token_float = ragel_num.getRatioAsFloat();
						break;
						default:
						return parser::make_LEXER_ERROR(tokenRange());
						break;
					}
				}
				
#line 389 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 87 "lex/lexer_data_string.rl"
					{te = p;p = p - 1;{
#line 87 "lex/lexer_data_string.rl"
							
							DUMP_TOKEN(FLOAT, token_float);
							return parser::make_FLOAT(token_float, tokenRange());
						}
					}}
				
#line 401 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr32:
				{
#line 44 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num *= ragel_num.sign;
					ragel_type = TYPE_RATIO;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 413 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 49 "lex/lexer_data_string.rl"
					
					t_float v = 0;
					switch(ragel_type) {
						case TYPE_HEX:
						case TYPE_BIN:
						case TYPE_INT:
						token_float = ragel_num.getInteger();
						break;
						case TYPE_FLOAT:
						token_float = ragel_num.getFloat();
						break;
						case TYPE_RATIO:
						token_float = ragel_num.getRatioAsFloat();
						break;
						default:
						return parser::make_LEXER_ERROR(tokenRange());
						break;
					}
				}
				
#line 437 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 87 "lex/lexer_data_string.rl"
					{te = p;p = p - 1;{
#line 87 "lex/lexer_data_string.rl"
							
							DUMP_TOKEN(FLOAT, token_float);
							return parser::make_FLOAT(token_float, tokenRange());
						}
					}}
				
#line 449 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr36:
				{
#line 116 "lex/lexer_data_string.rl"
					{te = p;p = p - 1;{
#line 116 "lex/lexer_data_string.rl"
							
							saveFunction();
							DUMP_TOKEN(DICT_KEY, text_);
							return parser::make_DICT_KEY(text_, location());
						}
					}}
				
#line 464 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr38:
				{
#line 24 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_BIN;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 475 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 49 "lex/lexer_data_string.rl"
					
					t_float v = 0;
					switch(ragel_type) {
						case TYPE_HEX:
						case TYPE_BIN:
						case TYPE_INT:
						token_float = ragel_num.getInteger();
						break;
						case TYPE_FLOAT:
						token_float = ragel_num.getFloat();
						break;
						case TYPE_RATIO:
						token_float = ragel_num.getRatioAsFloat();
						break;
						default:
						return parser::make_LEXER_ERROR(tokenRange());
						break;
					}
				}
				
#line 499 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 87 "lex/lexer_data_string.rl"
					{te = p;p = p - 1;{
#line 87 "lex/lexer_data_string.rl"
							
							DUMP_TOKEN(FLOAT, token_float);
							return parser::make_FLOAT(token_float, tokenRange());
						}
					}}
				
#line 511 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr42:
				{
#line 37 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_HEX;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 522 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 49 "lex/lexer_data_string.rl"
					
					t_float v = 0;
					switch(ragel_type) {
						case TYPE_HEX:
						case TYPE_BIN:
						case TYPE_INT:
						token_float = ragel_num.getInteger();
						break;
						case TYPE_FLOAT:
						token_float = ragel_num.getFloat();
						break;
						case TYPE_RATIO:
						token_float = ragel_num.getRatioAsFloat();
						break;
						default:
						return parser::make_LEXER_ERROR(tokenRange());
						break;
					}
				}
				
#line 546 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 87 "lex/lexer_data_string.rl"
					{te = p;p = p - 1;{
#line 87 "lex/lexer_data_string.rl"
							
							DUMP_TOKEN(FLOAT, token_float);
							return parser::make_FLOAT(token_float, tokenRange());
						}
					}}
				
#line 558 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr44:
				{
#line 92 "lex/lexer_data_string.rl"
					{te = p;p = p - 1;{
#line 92 "lex/lexer_data_string.rl"
							
							saveSymbol();
							DUMP_TOKEN(PROPERTY, text_);
							return parser::make_PROPERTY(text_, tokenRange());
						}
					}}
				
#line 573 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr45:
				{
#line 1 "NONE"
					{switch( act ) {
							case 2:  {
								p = ((te))-1;
								{
#line 79 "lex/lexer_data_string.rl"
									return parser::make_FLOAT(1, tokenRange()); }
								break; 
							}
							case 3:  {
								p = ((te))-1;
								{
#line 80 "lex/lexer_data_string.rl"
									return parser::make_FLOAT(0, tokenRange()); }
								break; 
							}
							case 4:  {
								p = ((te))-1;
								{
#line 81 "lex/lexer_data_string.rl"
									return parser::make_NULL(tokenRange()); }
								break; 
							}
							case 14:  {
								p = ((te))-1;
								{
#line 116 "lex/lexer_data_string.rl"
									
									saveFunction();
									DUMP_TOKEN(DICT_KEY, text_);
									return parser::make_DICT_KEY(text_, location());
								}
								break; 
							}
						}}
				}
				
#line 615 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_ctr46:
				{
#line 98 "lex/lexer_data_string.rl"
					{te = p+1;{
#line 98 "lex/lexer_data_string.rl"
							
							saveFunction();
							DUMP_TOKEN(FUNC_CALL, text_);
							return parser::make_FUNC_CALL(text_, location());
						}
					}}
				
#line 630 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st6;
				_st6:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				{
#line 1 "NONE"
					{ts = 0;}}
				
#line 644 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				{
#line 1 "NONE"
					{ts = p;}}
				
#line 654 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				switch( ( (*( p))) ) {
					case 32: {
						goto _st7;
					}
					case 40: {
						goto _ctr15;
					}
					case 41: {
						goto _ctr16;
					}
					case 43: {
						goto _ctr17;
					}
					case 45: {
						goto _ctr17;
					}
					case 48: {
						goto _ctr18;
					}
					case 63: {
						goto _st14;
					}
					case 64: {
						goto _st4;
					}
					case 91: {
						goto _ctr21;
					}
					case 93: {
						goto _ctr22;
					}
					case 95: {
						goto _st14;
					}
					case 102: {
						goto _st23;
					}
					case 110: {
						goto _st27;
					}
					case 116: {
						goto _st30;
					}
				}
				if ( ( (*( p))) < 49 ) {
					if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
						goto _st7;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( ( (*( p))) > 90 ) {
						if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
							goto _ctr23;
						}
					} else if ( ( (*( p))) >= 65 ) {
						goto _st5;
					}
				} else {
					goto _ctr19;
				}
				{
					goto _st0;
				}
				st_case_0:
				_st0:
				cs = 0;
				goto _pop;
				_st7:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 32 ) {
					goto _st7;
				}
				if ( 9 <= ( (*( p))) && ( (*( p))) <= 13 ) {
					goto _st7;
				}
				{
					goto _ctr27;
				}
				_ctr17:
				{
#line 21 "lex/ragel_numeric.rl"
					ragel_num.sign = ((( (*( p))))=='-') ? -1 : 1; }
				
#line 747 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st1;
				_st1:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof1;
				st_case_1:
				if ( ( (*( p))) == 48 ) {
					goto _ctr2;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr3;
				}
				{
					goto _st0;
				}
				_ctr2:
				{
#line 1 "NONE"
					{te = p+1;}}
				
#line 775 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 781 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 787 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st8;
				_st8:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr29;
					}
					case 47: {
						goto _st3;
					}
				}
				{
					goto _ctr28;
				}
				_ctr29:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 821 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 50 "lex/ragel_numeric.rl"
					
					ragel_num.ratio.num = 0;
					ragel_num.ratio.den = 1;
				}
				
#line 830 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st2;
				_st2:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr5;
				}
				{
					goto _ctr4;
				}
				_ctr5:
				{
#line 54 "lex/ragel_numeric.rl"
					
					(ragel_num.ratio.num *= 10) += ((( (*( p)))) - '0');
					ragel_num.ratio.den *= 10;
				}
				
#line 858 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st9;
				_st9:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr5;
				}
				{
					goto _ctr31;
				}
				_st3:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				if ( ( (*( p))) == 48 ) {
					goto _ctr6;
				}
				if ( 49 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr7;
				}
				{
					goto _ctr4;
				}
				_ctr6:
				{
#line 43 "lex/ragel_numeric.rl"
					(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
				
#line 903 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st10;
				_st10:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				{
					goto _ctr32;
				}
				_ctr7:
				{
#line 43 "lex/ragel_numeric.rl"
					(ragel_num.ratio.den *= 10) += ((( (*( p))))-'0'); }
				
#line 925 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st11;
				_st11:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr7;
				}
				{
					goto _ctr32;
				}
				_ctr3:
				{
#line 1 "NONE"
					{te = p+1;}}
				
#line 950 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 956 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 962 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st12;
				_st12:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr29;
					}
					case 47: {
						goto _st3;
					}
				}
				if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
					goto _ctr3;
				}
				{
					goto _ctr28;
				}
				_ctr18:
				{
#line 1 "NONE"
					{te = p+1;}}
				
#line 995 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 1001 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 1007 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st13;
				_st13:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr29;
					}
					case 47: {
						goto _st3;
					}
					case 63: {
						goto _ctr33;
					}
					case 95: {
						goto _ctr33;
					}
					case 98: {
						goto _st15;
					}
					case 120: {
						goto _st17;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr33;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr33;
				}
				{
					goto _ctr28;
				}
				_ctr33:
				{
#line 30 "lex/ragel_numeric.rl"
					
					ragel_num.vint *= ragel_num.sign;
					ragel_type = TYPE_INT;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1060 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 49 "lex/lexer_data_string.rl"
					
					t_float v = 0;
					switch(ragel_type) {
						case TYPE_HEX:
						case TYPE_BIN:
						case TYPE_INT:
						token_float = ragel_num.getInteger();
						break;
						case TYPE_FLOAT:
						token_float = ragel_num.getFloat();
						break;
						case TYPE_RATIO:
						token_float = ragel_num.getRatioAsFloat();
						break;
						default:
						return parser::make_LEXER_ERROR(tokenRange());
						break;
					}
				}
				
#line 1084 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st14;
				_ctr39:
				{
#line 24 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_BIN;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1095 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 49 "lex/lexer_data_string.rl"
					
					t_float v = 0;
					switch(ragel_type) {
						case TYPE_HEX:
						case TYPE_BIN:
						case TYPE_INT:
						token_float = ragel_num.getInteger();
						break;
						case TYPE_FLOAT:
						token_float = ragel_num.getFloat();
						break;
						case TYPE_RATIO:
						token_float = ragel_num.getRatioAsFloat();
						break;
						default:
						return parser::make_LEXER_ERROR(tokenRange());
						break;
					}
				}
				
#line 1119 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st14;
				_ctr43:
				{
#line 37 "lex/ragel_numeric.rl"
					
					ragel_type = TYPE_HEX;
					ragel_cat  = CAT_NUMBER;
				}
				
#line 1130 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 49 "lex/lexer_data_string.rl"
					
					t_float v = 0;
					switch(ragel_type) {
						case TYPE_HEX:
						case TYPE_BIN:
						case TYPE_INT:
						token_float = ragel_num.getInteger();
						break;
						case TYPE_FLOAT:
						token_float = ragel_num.getFloat();
						break;
						case TYPE_RATIO:
						token_float = ragel_num.getRatioAsFloat();
						break;
						default:
						return parser::make_LEXER_ERROR(tokenRange());
						break;
					}
				}
				
#line 1154 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st14;
				_st14:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				switch( ( (*( p))) ) {
					case 63: {
						goto _st14;
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
					goto _ctr36;
				}
				_st15:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof15;
				st_case_15:
				switch( ( (*( p))) ) {
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _st14;
					}
				}
				if ( ( (*( p))) < 50 ) {
					if ( 48 <= ( (*( p))) ) {
						goto _ctr37;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st14;
					}
				} else {
					goto _st14;
				}
				{
					goto _ctr36;
				}
				_ctr37:
				{
#line 23 "lex/ragel_numeric.rl"
					(ragel_num.vint <<= 1) |= ((( (*( p))))=='1'); }
				
#line 1224 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st16;
				_st16:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof16;
				st_case_16:
				switch( ( (*( p))) ) {
					case 63: {
						goto _ctr39;
					}
					case 95: {
						goto _ctr39;
					}
				}
				if ( ( (*( p))) < 50 ) {
					if ( 48 <= ( (*( p))) ) {
						goto _ctr37;
					}
				} else if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr39;
					}
				} else {
					goto _ctr39;
				}
				{
					goto _ctr38;
				}
				_st17:
				if ( p == eof ) {
					if ( cs >= 6 )
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
						goto _st14;
					}
					case 95: {
						goto _st14;
					}
				}
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr40;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( ( (*( p))) > 102 ) {
						if ( ( (*( p))) <= 122 ) {
							goto _st14;
						}
					} else if ( ( (*( p))) >= 97 ) {
						goto _ctr40;
					}
				} else {
					goto _ctr41;
				}
				{
					goto _ctr36;
				}
				_ctr40:
				{
#line 36 "lex/ragel_numeric.rl"
					(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
				
#line 1302 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st18;
				_st18:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof18;
				st_case_18:
				switch( ( (*( p))) ) {
					case 63: {
						goto _ctr43;
					}
					case 95: {
						goto _ctr43;
					}
				}
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr40;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( ( (*( p))) > 102 ) {
						if ( ( (*( p))) <= 122 ) {
							goto _ctr43;
						}
					} else if ( ( (*( p))) >= 97 ) {
						goto _ctr40;
					}
				} else {
					goto _ctr41;
				}
				{
					goto _ctr42;
				}
				_ctr41:
				{
#line 36 "lex/ragel_numeric.rl"
					(ragel_num.vint <<= 4) |= xchar2digit((( (*( p))))); }
				
#line 1347 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st19;
				_st19:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				if ( ( (*( p))) < 65 ) {
					if ( 48 <= ( (*( p))) && ( (*( p))) <= 57 ) {
						goto _ctr41;
					}
				} else if ( ( (*( p))) > 70 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 102 ) {
						goto _ctr41;
					}
				} else {
					goto _ctr41;
				}
				{
					goto _ctr42;
				}
				_ctr19:
				{
#line 1 "NONE"
					{te = p+1;}}
				
#line 1380 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 29 "lex/ragel_numeric.rl"
					(ragel_num.vint *= 10) += ((( (*( p))))-'0'); }
				
#line 1386 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 42 "lex/ragel_numeric.rl"
					(ragel_num.ratio.num *= 10) += ((( (*( p))))-'0'); }
				
#line 1392 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st20;
				_st20:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof20;
				st_case_20:
				switch( ( (*( p))) ) {
					case 46: {
						goto _ctr29;
					}
					case 47: {
						goto _st3;
					}
					case 63: {
						goto _ctr33;
					}
					case 95: {
						goto _ctr33;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr33;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr19;
				}
				{
					goto _ctr28;
				}
				_st4:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				switch( ( (*( p))) ) {
					case 63: {
						goto _st21;
					}
					case 95: {
						goto _st21;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st21;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st21;
				}
				{
					goto _st0;
				}
				_st21:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof21;
				st_case_21:
				switch( ( (*( p))) ) {
					case 63: {
						goto _st21;
					}
					case 95: {
						goto _st21;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st21;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _st21;
				}
				{
					goto _ctr44;
				}
				_st5:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr11;
					}
					case 91: {
						goto _ctr12;
					}
				}
				if ( ( (*( p))) > 90 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _st5;
					}
				} else if ( ( (*( p))) >= 65 ) {
					goto _st5;
				}
				{
					goto _st0;
				}
				_ctr23:
				{
#line 1 "NONE"
					{te = p+1;}}
				
#line 1522 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 116 "lex/lexer_data_string.rl"
					{act = 14;}}
				
#line 1528 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st22;
				_ctr50:
				{
#line 1 "NONE"
					{te = p+1;}}
				
#line 1536 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 80 "lex/lexer_data_string.rl"
					{act = 3;}}
				
#line 1542 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st22;
				_ctr53:
				{
#line 1 "NONE"
					{te = p+1;}}
				
#line 1550 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 81 "lex/lexer_data_string.rl"
					{act = 4;}}
				
#line 1556 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st22;
				_ctr56:
				{
#line 1 "NONE"
					{te = p+1;}}
				
#line 1564 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				{
#line 79 "lex/lexer_data_string.rl"
					{act = 2;}}
				
#line 1570 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
				
				goto _st22;
				_st22:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof22;
				st_case_22:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr46;
					}
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _ctr23;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _ctr45;
				}
				_st23:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof23;
				st_case_23:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr46;
					}
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _ctr23;
					}
					case 97: {
						goto _st24;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 98 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _ctr36;
				}
				_st24:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof24;
				st_case_24:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr46;
					}
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _ctr23;
					}
					case 108: {
						goto _st25;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _ctr36;
				}
				_st25:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof25;
				st_case_25:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr46;
					}
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _ctr23;
					}
					case 115: {
						goto _st26;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _ctr36;
				}
				_st26:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof26;
				st_case_26:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr46;
					}
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _ctr23;
					}
					case 101: {
						goto _ctr50;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _ctr36;
				}
				_st27:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof27;
				st_case_27:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr46;
					}
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _ctr23;
					}
					case 117: {
						goto _st28;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _ctr36;
				}
				_st28:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof28;
				st_case_28:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr46;
					}
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _ctr23;
					}
					case 108: {
						goto _st29;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _ctr36;
				}
				_st29:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof29;
				st_case_29:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr46;
					}
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _ctr23;
					}
					case 108: {
						goto _ctr53;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _ctr36;
				}
				_st30:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof30;
				st_case_30:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr46;
					}
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _ctr23;
					}
					case 114: {
						goto _st31;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _ctr36;
				}
				_st31:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof31;
				st_case_31:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr46;
					}
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _ctr23;
					}
					case 117: {
						goto _st32;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _ctr36;
				}
				_st32:
				if ( p == eof ) {
					if ( cs >= 6 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof32;
				st_case_32:
				switch( ( (*( p))) ) {
					case 40: {
						goto _ctr46;
					}
					case 63: {
						goto _st14;
					}
					case 95: {
						goto _ctr23;
					}
					case 101: {
						goto _ctr56;
					}
				}
				if ( ( (*( p))) > 57 ) {
					if ( 97 <= ( (*( p))) && ( (*( p))) <= 122 ) {
						goto _ctr23;
					}
				} else if ( ( (*( p))) >= 48 ) {
					goto _ctr23;
				}
				{
					goto _ctr36;
				}
				st_out:
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
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
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof21: cs = 21; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
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
				
				_test_eof: {}
				if ( p == eof ) {
					switch ( cs ) {
						case 6: {
							{
#line 1 "NONE"
								{ts = p;}}
							
#line 1997 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/lexer_data_string.cpp"
							
							break;
						}
						case 0: {
							break;
						}
						case 7: {
							break;
						}
						case 1: {
							break;
						}
						case 8: {
							break;
						}
						case 2: {
							break;
						}
						case 9: {
							break;
						}
						case 3: {
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
						case 18: {
							break;
						}
						case 19: {
							break;
						}
						case 20: {
							break;
						}
						case 4: {
							break;
						}
						case 21: {
							break;
						}
						case 5: {
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
					}
					switch ( cs ) {
					}
					switch ( cs ) {
						case 6:
						goto _st6;case 0:
						goto _st0;case 7:
						goto _ctr27;case 1:
						goto _st1;case 8:
						goto _ctr28;case 2:
						goto _ctr4;case 9:
						goto _ctr31;case 3:
						goto _ctr4;case 10:
						goto _ctr32;case 11:
						goto _ctr32;case 12:
						goto _ctr28;case 13:
						goto _ctr28;case 14:
						goto _ctr36;case 15:
						goto _ctr36;case 16:
						goto _ctr38;case 17:
						goto _ctr36;case 18:
						goto _ctr42;case 19:
						goto _ctr42;case 20:
						goto _ctr28;case 4:
						goto _st4;case 21:
						goto _ctr44;case 5:
						goto _st5;case 22:
						goto _ctr45;case 23:
						goto _ctr36;case 24:
						goto _ctr36;case 25:
						goto _ctr36;case 26:
						goto _ctr36;case 27:
						goto _ctr36;case 28:
						goto _ctr36;case 29:
						goto _ctr36;case 30:
						goto _ctr36;case 31:
						goto _ctr36;case 32:
						goto _ctr36;	}
				}
				
				if ( cs >= 6 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 190 "lex/lexer_data_string.rl"
			
			printf("cs=%d\n", cs);
			DUMP_TOKEN(SYMBOL, text_);
			return parser::make_SYMBOL(text_, tokenRange());
		}
		
		DataStringLexer::parser::symbol_type DataStringLexer::next()
		{
			auto result = this->exec();
			this->p = te;
			return result;
		}
		
		
		
	}
}
