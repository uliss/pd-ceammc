#line 1 "new_file_pattern.rl"
# include "new_file_pattern.h"
# include "ceammc_log.h"

# include <cmath>
# include <cstring>
# include <ctime>
# include <limits>

# include "fmt/core.h"


#line 14 "new_file_pattern.cpp"
static const int expand_filename_pattern_start = 63;
static const int expand_filename_pattern_first_final = 63;
static const int expand_filename_pattern_error = 0;

static const int expand_filename_pattern_en_main = 63;


#line 113 "new_file_pattern.rl"


namespace ceammc {
	namespace path {
		
		PatternStatus expand_filename_pattern(const std::string& pattern, std::string& res, uint32_t idx)
		{
			int cs = 0;
			auto* p = pattern.data();
			auto* pe = p + pattern.size();
			auto* eof = pe;
			
			auto tm = std::time(nullptr);
			auto rl_now = *std::localtime(&tm);
			bool rl_pattern_found = false;
			bool rl_date_found = false;
			const char* rl_err = 0;
			int rl_pat_cnum = 0;
			std::string rl_fname;
			rl_fname.reserve(pattern.size());
			
			
#line 45 "new_file_pattern.cpp"
			{
				cs = (int)expand_filename_pattern_start;
			}
			
#line 134 "new_file_pattern.rl"
			
			
#line 53 "new_file_pattern.cpp"
			{
				if ( p == pe )
					goto _test_eof;
				switch ( cs ) {
					case 63:
					goto st_case_63;
					case 64:
					goto st_case_64;
					case 65:
					goto st_case_65;
					case 0:
					goto st_case_0;
					case 1:
					goto st_case_1;
					case 66:
					goto st_case_66;
					case 2:
					goto st_case_2;
					case 3:
					goto st_case_3;
					case 4:
					goto st_case_4;
					case 5:
					goto st_case_5;
					case 67:
					goto st_case_67;
					case 6:
					goto st_case_6;
					case 68:
					goto st_case_68;
					case 7:
					goto st_case_7;
					case 8:
					goto st_case_8;
					case 9:
					goto st_case_9;
					case 10:
					goto st_case_10;
					case 69:
					goto st_case_69;
					case 11:
					goto st_case_11;
					case 12:
					goto st_case_12;
					case 13:
					goto st_case_13;
					case 70:
					goto st_case_70;
					case 14:
					goto st_case_14;
					case 15:
					goto st_case_15;
					case 16:
					goto st_case_16;
					case 17:
					goto st_case_17;
					case 71:
					goto st_case_71;
					case 18:
					goto st_case_18;
					case 19:
					goto st_case_19;
					case 20:
					goto st_case_20;
					case 72:
					goto st_case_72;
					case 21:
					goto st_case_21;
					case 22:
					goto st_case_22;
					case 23:
					goto st_case_23;
					case 73:
					goto st_case_73;
					case 24:
					goto st_case_24;
					case 25:
					goto st_case_25;
					case 26:
					goto st_case_26;
					case 27:
					goto st_case_27;
					case 74:
					goto st_case_74;
					case 28:
					goto st_case_28;
					case 29:
					goto st_case_29;
					case 30:
					goto st_case_30;
					case 31:
					goto st_case_31;
					case 75:
					goto st_case_75;
					case 32:
					goto st_case_32;
					case 76:
					goto st_case_76;
					case 33:
					goto st_case_33;
					case 34:
					goto st_case_34;
					case 35:
					goto st_case_35;
					case 36:
					goto st_case_36;
					case 77:
					goto st_case_77;
					case 37:
					goto st_case_37;
					case 78:
					goto st_case_78;
					case 38:
					goto st_case_38;
					case 39:
					goto st_case_39;
					case 40:
					goto st_case_40;
					case 41:
					goto st_case_41;
					case 79:
					goto st_case_79;
					case 42:
					goto st_case_42;
					case 43:
					goto st_case_43;
					case 44:
					goto st_case_44;
					case 80:
					goto st_case_80;
					case 45:
					goto st_case_45;
					case 46:
					goto st_case_46;
					case 47:
					goto st_case_47;
					case 48:
					goto st_case_48;
					case 81:
					goto st_case_81;
					case 49:
					goto st_case_49;
					case 50:
					goto st_case_50;
					case 51:
					goto st_case_51;
					case 82:
					goto st_case_82;
					case 52:
					goto st_case_52;
					case 53:
					goto st_case_53;
					case 54:
					goto st_case_54;
					case 83:
					goto st_case_83;
					case 55:
					goto st_case_55;
					case 56:
					goto st_case_56;
					case 57:
					goto st_case_57;
					case 58:
					goto st_case_58;
					case 84:
					goto st_case_84;
					case 59:
					goto st_case_59;
					case 60:
					goto st_case_60;
					case 61:
					goto st_case_61;
					case 62:
					goto st_case_62;
					case 85:
					goto st_case_85;
				}
				goto st_out;
				_ctr145:
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 237 "new_file_pattern.cpp"
				
				goto _st63;
				_ctr185:
				{
#line 98 "new_file_pattern.rl"
					rl_fname += '%'; }
				
#line 245 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 251 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 257 "new_file_pattern.cpp"
				
				goto _st63;
				_ctr189:
				{
#line 71 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 269 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 275 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 281 "new_file_pattern.cpp"
				
				goto _st63;
				_ctr193:
				{
#line 47 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 293 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 299 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 305 "new_file_pattern.cpp"
				
				goto _st63;
				_ctr197:
				{
#line 53 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
					rl_fname.append(buf);
				}
				
#line 317 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 323 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 329 "new_file_pattern.cpp"
				
				goto _st63;
				_ctr201:
				{
#line 59 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
					rl_fname.append(buf);
				}
				
#line 341 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 347 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 353 "new_file_pattern.cpp"
				
				goto _st63;
				_ctr205:
				{
#line 40 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
					rl_fname.append(buf);
				}
				
#line 366 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 372 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 378 "new_file_pattern.cpp"
				
				goto _st63;
				_ctr209:
				{
#line 83 "new_file_pattern.rl"
					
					char buf[64];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 390 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 396 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 402 "new_file_pattern.cpp"
				
				goto _st63;
				_ctr213:
				{
#line 65 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 414 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 420 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 426 "new_file_pattern.cpp"
				
				goto _st63;
				_ctr217:
				{
#line 77 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 438 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 444 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 450 "new_file_pattern.cpp"
				
				goto _st63;
				_ctr221:
				{
#line 33 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
					rl_fname.append(buf);
				}
				
#line 463 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 469 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 475 "new_file_pattern.cpp"
				
				goto _st63;
				_st63:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof63;
				st_case_63:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr146;
					}
					case 37: {
						goto _ctr147;
					}
				}
				{
					goto _ctr145;
				}
				_ctr146:
				{
#line 107 "new_file_pattern.rl"
					rl_pat_cnum++; }
				
#line 505 "new_file_pattern.cpp"
				
				goto _st64;
				_ctr148:
				{
#line 15 "new_file_pattern.rl"
					
					using IndexT = decltype(idx);
					constexpr auto NDIGITS10 = std::numeric_limits<IndexT>::digits10;
					if (rl_pat_cnum > NDIGITS10) {
						LIB_ERR << fmt::format("[{}] '#'-pattern is too long: {}, max length is {}", __FUNCTION__, rl_pat_cnum, NDIGITS10);
						return PATTERN_LENGTH_OVERFLOW;
					}
					
					char buf[NDIGITS10+1];
					buf[rl_pat_cnum] = '\0';
					for (IndexT i = 0, dig = 1; i < rl_pat_cnum; i++, dig *= 10)
					buf[rl_pat_cnum - (i+1)] = ('0' + (idx / dig) % 10);
					
					
					rl_fname.append(buf);
					rl_pattern_found = true;
				}
				
#line 529 "new_file_pattern.cpp"
				
				goto _st64;
				_ctr186:
				{
#line 98 "new_file_pattern.rl"
					rl_fname += '%'; }
				
#line 537 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 543 "new_file_pattern.cpp"
				
				{
#line 107 "new_file_pattern.rl"
					rl_pat_cnum++; }
				
#line 549 "new_file_pattern.cpp"
				
				goto _st64;
				_ctr190:
				{
#line 71 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 561 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 567 "new_file_pattern.cpp"
				
				{
#line 107 "new_file_pattern.rl"
					rl_pat_cnum++; }
				
#line 573 "new_file_pattern.cpp"
				
				goto _st64;
				_ctr194:
				{
#line 47 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 585 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 591 "new_file_pattern.cpp"
				
				{
#line 107 "new_file_pattern.rl"
					rl_pat_cnum++; }
				
#line 597 "new_file_pattern.cpp"
				
				goto _st64;
				_ctr198:
				{
#line 53 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
					rl_fname.append(buf);
				}
				
#line 609 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 615 "new_file_pattern.cpp"
				
				{
#line 107 "new_file_pattern.rl"
					rl_pat_cnum++; }
				
#line 621 "new_file_pattern.cpp"
				
				goto _st64;
				_ctr202:
				{
#line 59 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
					rl_fname.append(buf);
				}
				
#line 633 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 639 "new_file_pattern.cpp"
				
				{
#line 107 "new_file_pattern.rl"
					rl_pat_cnum++; }
				
#line 645 "new_file_pattern.cpp"
				
				goto _st64;
				_ctr206:
				{
#line 40 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
					rl_fname.append(buf);
				}
				
#line 658 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 664 "new_file_pattern.cpp"
				
				{
#line 107 "new_file_pattern.rl"
					rl_pat_cnum++; }
				
#line 670 "new_file_pattern.cpp"
				
				goto _st64;
				_ctr210:
				{
#line 83 "new_file_pattern.rl"
					
					char buf[64];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 682 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 688 "new_file_pattern.cpp"
				
				{
#line 107 "new_file_pattern.rl"
					rl_pat_cnum++; }
				
#line 694 "new_file_pattern.cpp"
				
				goto _st64;
				_ctr214:
				{
#line 65 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 706 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 712 "new_file_pattern.cpp"
				
				{
#line 107 "new_file_pattern.rl"
					rl_pat_cnum++; }
				
#line 718 "new_file_pattern.cpp"
				
				goto _st64;
				_ctr218:
				{
#line 77 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 730 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 736 "new_file_pattern.cpp"
				
				{
#line 107 "new_file_pattern.rl"
					rl_pat_cnum++; }
				
#line 742 "new_file_pattern.cpp"
				
				goto _st64;
				_ctr222:
				{
#line 33 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
					rl_fname.append(buf);
				}
				
#line 755 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 761 "new_file_pattern.cpp"
				
				{
#line 107 "new_file_pattern.rl"
					rl_pat_cnum++; }
				
#line 767 "new_file_pattern.cpp"
				
				goto _st64;
				_st64:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof64;
				st_case_64:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr146;
					}
					case 37: {
						goto _ctr150;
					}
				}
				{
					goto _ctr149;
				}
				_ctr152:
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 797 "new_file_pattern.cpp"
				
				goto _st65;
				_ctr149:
				{
#line 15 "new_file_pattern.rl"
					
					using IndexT = decltype(idx);
					constexpr auto NDIGITS10 = std::numeric_limits<IndexT>::digits10;
					if (rl_pat_cnum > NDIGITS10) {
						LIB_ERR << fmt::format("[{}] '#'-pattern is too long: {}, max length is {}", __FUNCTION__, rl_pat_cnum, NDIGITS10);
						return PATTERN_LENGTH_OVERFLOW;
					}
					
					char buf[NDIGITS10+1];
					buf[rl_pat_cnum] = '\0';
					for (IndexT i = 0, dig = 1; i < rl_pat_cnum; i++, dig *= 10)
					buf[rl_pat_cnum - (i+1)] = ('0' + (idx / dig) % 10);
					
					
					rl_fname.append(buf);
					rl_pattern_found = true;
				}
				
#line 821 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 827 "new_file_pattern.cpp"
				
				goto _st65;
				_ctr155:
				{
#line 98 "new_file_pattern.rl"
					rl_fname += '%'; }
				
#line 835 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 841 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 847 "new_file_pattern.cpp"
				
				goto _st65;
				_ctr158:
				{
#line 71 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 859 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 865 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 871 "new_file_pattern.cpp"
				
				goto _st65;
				_ctr161:
				{
#line 47 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 883 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 889 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 895 "new_file_pattern.cpp"
				
				goto _st65;
				_ctr164:
				{
#line 53 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
					rl_fname.append(buf);
				}
				
#line 907 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 913 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 919 "new_file_pattern.cpp"
				
				goto _st65;
				_ctr167:
				{
#line 59 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
					rl_fname.append(buf);
				}
				
#line 931 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 937 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 943 "new_file_pattern.cpp"
				
				goto _st65;
				_ctr170:
				{
#line 40 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
					rl_fname.append(buf);
				}
				
#line 956 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 962 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 968 "new_file_pattern.cpp"
				
				goto _st65;
				_ctr173:
				{
#line 83 "new_file_pattern.rl"
					
					char buf[64];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 980 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 986 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 992 "new_file_pattern.cpp"
				
				goto _st65;
				_ctr176:
				{
#line 65 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1004 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1010 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 1016 "new_file_pattern.cpp"
				
				goto _st65;
				_ctr179:
				{
#line 77 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1028 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1034 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 1040 "new_file_pattern.cpp"
				
				goto _st65;
				_ctr182:
				{
#line 33 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1053 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1059 "new_file_pattern.cpp"
				
				{
#line 104 "new_file_pattern.rl"
					rl_fname += (( (*( p)))); }
				
#line 1065 "new_file_pattern.cpp"
				
				goto _st65;
				_st65:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof65;
				st_case_65:
				switch( ( (*( p))) ) {
					case 35: {
						goto _st0;
					}
					case 37: {
						goto _ctr153;
					}
				}
				{
					goto _ctr152;
				}
				_ctr2:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1095 "new_file_pattern.cpp"
				
				goto _st0;
				st_case_0:
				_st0:
				cs = 0;
				goto _pop;
				_ctr1:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1107 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr153:
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1115 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr150:
				{
#line 15 "new_file_pattern.rl"
					
					using IndexT = decltype(idx);
					constexpr auto NDIGITS10 = std::numeric_limits<IndexT>::digits10;
					if (rl_pat_cnum > NDIGITS10) {
						LIB_ERR << fmt::format("[{}] '#'-pattern is too long: {}, max length is {}", __FUNCTION__, rl_pat_cnum, NDIGITS10);
						return PATTERN_LENGTH_OVERFLOW;
					}
					
					char buf[NDIGITS10+1];
					buf[rl_pat_cnum] = '\0';
					for (IndexT i = 0, dig = 1; i < rl_pat_cnum; i++, dig *= 10)
					buf[rl_pat_cnum - (i+1)] = ('0' + (idx / dig) % 10);
					
					
					rl_fname.append(buf);
					rl_pattern_found = true;
				}
				
#line 1139 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1145 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr156:
				{
#line 98 "new_file_pattern.rl"
					rl_fname += '%'; }
				
#line 1153 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1159 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1165 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr159:
				{
#line 71 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1177 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1183 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1189 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr162:
				{
#line 47 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1201 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1207 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1213 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr165:
				{
#line 53 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1225 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1231 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1237 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr168:
				{
#line 59 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1249 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1255 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1261 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr171:
				{
#line 40 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1274 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1280 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1286 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr174:
				{
#line 83 "new_file_pattern.rl"
					
					char buf[64];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1298 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1304 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1310 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr177:
				{
#line 65 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1322 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1328 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1334 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr180:
				{
#line 77 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1346 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1352 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1358 "new_file_pattern.cpp"
				
				goto _st1;
				_ctr183:
				{
#line 33 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1371 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1377 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 1383 "new_file_pattern.cpp"
				
				goto _st1;
				_st1:
				if ( p == eof ) {
					if ( cs >= 63 )
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
						goto _st66;
					}
					case 68: {
						goto _st2;
					}
					case 72: {
						goto _st7;
					}
					case 77: {
						goto _st11;
					}
					case 78: {
						goto _st18;
					}
					case 83: {
						goto _st21;
					}
					case 84: {
						goto _st24;
					}
					case 89: {
						goto _st28;
					}
				}
				{
					goto _ctr2;
				}
				_ctr154:
				{
#line 98 "new_file_pattern.rl"
					rl_fname += '%'; }
				
#line 1431 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1437 "new_file_pattern.cpp"
				
				goto _st66;
				_st66:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof66;
				st_case_66:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr2;
					}
					case 37: {
						goto _ctr156;
					}
				}
				{
					goto _ctr155;
				}
				_ctr11:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1467 "new_file_pattern.cpp"
				
				goto _st2;
				_st2:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof2;
				st_case_2:
				if ( ( (*( p))) == 65 ) {
					goto _st3;
				}
				{
					goto _ctr2;
				}
				_ctr13:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1492 "new_file_pattern.cpp"
				
				goto _st3;
				_st3:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof3;
				st_case_3:
				switch( ( (*( p))) ) {
					case 84: {
						goto _st4;
					}
					case 89: {
						goto _st6;
					}
				}
				{
					goto _ctr2;
				}
				_ctr16:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1522 "new_file_pattern.cpp"
				
				goto _st4;
				_st4:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof4;
				st_case_4:
				if ( ( (*( p))) == 69 ) {
					goto _st5;
				}
				{
					goto _ctr2;
				}
				_ctr18:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1547 "new_file_pattern.cpp"
				
				goto _st5;
				_st5:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof5;
				st_case_5:
				if ( ( (*( p))) == 37 ) {
					goto _st67;
				}
				{
					goto _ctr2;
				}
				_ctr157:
				{
#line 71 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1576 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1582 "new_file_pattern.cpp"
				
				goto _st67;
				_st67:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof67;
				st_case_67:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr2;
					}
					case 37: {
						goto _ctr159;
					}
				}
				{
					goto _ctr158;
				}
				_ctr20:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1612 "new_file_pattern.cpp"
				
				goto _st6;
				_st6:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof6;
				st_case_6:
				if ( ( (*( p))) == 37 ) {
					goto _st68;
				}
				{
					goto _ctr2;
				}
				_ctr160:
				{
#line 47 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1641 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1647 "new_file_pattern.cpp"
				
				goto _st68;
				_st68:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof68;
				st_case_68:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr2;
					}
					case 37: {
						goto _ctr162;
					}
				}
				{
					goto _ctr161;
				}
				_ctr22:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1677 "new_file_pattern.cpp"
				
				goto _st7;
				_st7:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof7;
				st_case_7:
				if ( ( (*( p))) == 79 ) {
					goto _st8;
				}
				{
					goto _ctr2;
				}
				_ctr24:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1702 "new_file_pattern.cpp"
				
				goto _st8;
				_st8:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof8;
				st_case_8:
				if ( ( (*( p))) == 85 ) {
					goto _st9;
				}
				{
					goto _ctr2;
				}
				_ctr26:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1727 "new_file_pattern.cpp"
				
				goto _st9;
				_st9:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof9;
				st_case_9:
				if ( ( (*( p))) == 82 ) {
					goto _st10;
				}
				{
					goto _ctr2;
				}
				_ctr28:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1752 "new_file_pattern.cpp"
				
				goto _st10;
				_st10:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof10;
				st_case_10:
				if ( ( (*( p))) == 37 ) {
					goto _st69;
				}
				{
					goto _ctr2;
				}
				_ctr163:
				{
#line 53 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1781 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1787 "new_file_pattern.cpp"
				
				goto _st69;
				_st69:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof69;
				st_case_69:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr2;
					}
					case 37: {
						goto _ctr165;
					}
				}
				{
					goto _ctr164;
				}
				_ctr30:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1817 "new_file_pattern.cpp"
				
				goto _st11;
				_st11:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof11;
				st_case_11:
				switch( ( (*( p))) ) {
					case 73: {
						goto _st12;
					}
					case 79: {
						goto _st14;
					}
				}
				{
					goto _ctr2;
				}
				_ctr33:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1847 "new_file_pattern.cpp"
				
				goto _st12;
				_st12:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof12;
				st_case_12:
				if ( ( (*( p))) == 78 ) {
					goto _st13;
				}
				{
					goto _ctr2;
				}
				_ctr35:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1872 "new_file_pattern.cpp"
				
				goto _st13;
				_st13:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof13;
				st_case_13:
				if ( ( (*( p))) == 37 ) {
					goto _st70;
				}
				{
					goto _ctr2;
				}
				_ctr166:
				{
#line 59 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
					rl_fname.append(buf);
				}
				
#line 1901 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 1907 "new_file_pattern.cpp"
				
				goto _st70;
				_st70:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof70;
				st_case_70:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr2;
					}
					case 37: {
						goto _ctr168;
					}
				}
				{
					goto _ctr167;
				}
				_ctr37:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1937 "new_file_pattern.cpp"
				
				goto _st14;
				_st14:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof14;
				st_case_14:
				if ( ( (*( p))) == 78 ) {
					goto _st15;
				}
				{
					goto _ctr2;
				}
				_ctr39:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1962 "new_file_pattern.cpp"
				
				goto _st15;
				_st15:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof15;
				st_case_15:
				if ( ( (*( p))) == 84 ) {
					goto _st16;
				}
				{
					goto _ctr2;
				}
				_ctr41:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 1987 "new_file_pattern.cpp"
				
				goto _st16;
				_st16:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof16;
				st_case_16:
				if ( ( (*( p))) == 72 ) {
					goto _st17;
				}
				{
					goto _ctr2;
				}
				_ctr43:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2012 "new_file_pattern.cpp"
				
				goto _st17;
				_st17:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof17;
				st_case_17:
				if ( ( (*( p))) == 37 ) {
					goto _st71;
				}
				{
					goto _ctr2;
				}
				_ctr169:
				{
#line 40 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2042 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2048 "new_file_pattern.cpp"
				
				goto _st71;
				_st71:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof71;
				st_case_71:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr2;
					}
					case 37: {
						goto _ctr171;
					}
				}
				{
					goto _ctr170;
				}
				_ctr45:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2078 "new_file_pattern.cpp"
				
				goto _st18;
				_st18:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof18;
				st_case_18:
				if ( ( (*( p))) == 79 ) {
					goto _st19;
				}
				{
					goto _ctr2;
				}
				_ctr47:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2103 "new_file_pattern.cpp"
				
				goto _st19;
				_st19:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof19;
				st_case_19:
				if ( ( (*( p))) == 87 ) {
					goto _st20;
				}
				{
					goto _ctr2;
				}
				_ctr49:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2128 "new_file_pattern.cpp"
				
				goto _st20;
				_st20:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof20;
				st_case_20:
				if ( ( (*( p))) == 37 ) {
					goto _st72;
				}
				{
					goto _ctr2;
				}
				_ctr172:
				{
#line 83 "new_file_pattern.rl"
					
					char buf[64];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2157 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2163 "new_file_pattern.cpp"
				
				goto _st72;
				_st72:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof72;
				st_case_72:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr2;
					}
					case 37: {
						goto _ctr174;
					}
				}
				{
					goto _ctr173;
				}
				_ctr51:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2193 "new_file_pattern.cpp"
				
				goto _st21;
				_st21:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof21;
				st_case_21:
				if ( ( (*( p))) == 69 ) {
					goto _st22;
				}
				{
					goto _ctr2;
				}
				_ctr53:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2218 "new_file_pattern.cpp"
				
				goto _st22;
				_st22:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof22;
				st_case_22:
				if ( ( (*( p))) == 67 ) {
					goto _st23;
				}
				{
					goto _ctr2;
				}
				_ctr55:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2243 "new_file_pattern.cpp"
				
				goto _st23;
				_st23:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof23;
				st_case_23:
				if ( ( (*( p))) == 37 ) {
					goto _st73;
				}
				{
					goto _ctr2;
				}
				_ctr175:
				{
#line 65 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2272 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2278 "new_file_pattern.cpp"
				
				goto _st73;
				_st73:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof73;
				st_case_73:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr2;
					}
					case 37: {
						goto _ctr177;
					}
				}
				{
					goto _ctr176;
				}
				_ctr57:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2308 "new_file_pattern.cpp"
				
				goto _st24;
				_st24:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof24;
				st_case_24:
				if ( ( (*( p))) == 73 ) {
					goto _st25;
				}
				{
					goto _ctr2;
				}
				_ctr59:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2333 "new_file_pattern.cpp"
				
				goto _st25;
				_st25:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof25;
				st_case_25:
				if ( ( (*( p))) == 77 ) {
					goto _st26;
				}
				{
					goto _ctr2;
				}
				_ctr61:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2358 "new_file_pattern.cpp"
				
				goto _st26;
				_st26:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof26;
				st_case_26:
				if ( ( (*( p))) == 69 ) {
					goto _st27;
				}
				{
					goto _ctr2;
				}
				_ctr63:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2383 "new_file_pattern.cpp"
				
				goto _st27;
				_st27:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof27;
				st_case_27:
				if ( ( (*( p))) == 37 ) {
					goto _st74;
				}
				{
					goto _ctr2;
				}
				_ctr178:
				{
#line 77 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2412 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2418 "new_file_pattern.cpp"
				
				goto _st74;
				_st74:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof74;
				st_case_74:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr2;
					}
					case 37: {
						goto _ctr180;
					}
				}
				{
					goto _ctr179;
				}
				_ctr65:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2448 "new_file_pattern.cpp"
				
				goto _st28;
				_st28:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof28;
				st_case_28:
				if ( ( (*( p))) == 69 ) {
					goto _st29;
				}
				{
					goto _ctr2;
				}
				_ctr67:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2473 "new_file_pattern.cpp"
				
				goto _st29;
				_st29:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof29;
				st_case_29:
				if ( ( (*( p))) == 65 ) {
					goto _st30;
				}
				{
					goto _ctr2;
				}
				_ctr69:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2498 "new_file_pattern.cpp"
				
				goto _st30;
				_st30:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof30;
				st_case_30:
				if ( ( (*( p))) == 82 ) {
					goto _st31;
				}
				{
					goto _ctr2;
				}
				_ctr71:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2523 "new_file_pattern.cpp"
				
				goto _st31;
				_st31:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof31;
				st_case_31:
				if ( ( (*( p))) == 37 ) {
					goto _st75;
				}
				{
					goto _ctr2;
				}
				_ctr181:
				{
#line 33 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2553 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2559 "new_file_pattern.cpp"
				
				goto _st75;
				_st75:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof75;
				st_case_75:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr2;
					}
					case 37: {
						goto _ctr183;
					}
				}
				{
					goto _ctr182;
				}
				_ctr73:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2589 "new_file_pattern.cpp"
				
				goto _st32;
				_ctr147:
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 2597 "new_file_pattern.cpp"
				
				goto _st32;
				_ctr187:
				{
#line 98 "new_file_pattern.rl"
					rl_fname += '%'; }
				
#line 2605 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2611 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 2617 "new_file_pattern.cpp"
				
				goto _st32;
				_ctr191:
				{
#line 71 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2629 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2635 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 2641 "new_file_pattern.cpp"
				
				goto _st32;
				_ctr195:
				{
#line 47 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2653 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2659 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 2665 "new_file_pattern.cpp"
				
				goto _st32;
				_ctr199:
				{
#line 53 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2677 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2683 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 2689 "new_file_pattern.cpp"
				
				goto _st32;
				_ctr203:
				{
#line 59 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2701 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2707 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 2713 "new_file_pattern.cpp"
				
				goto _st32;
				_ctr207:
				{
#line 40 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2726 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2732 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 2738 "new_file_pattern.cpp"
				
				goto _st32;
				_ctr211:
				{
#line 83 "new_file_pattern.rl"
					
					char buf[64];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2750 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2756 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 2762 "new_file_pattern.cpp"
				
				goto _st32;
				_ctr215:
				{
#line 65 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2774 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2780 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 2786 "new_file_pattern.cpp"
				
				goto _st32;
				_ctr219:
				{
#line 77 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2798 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2804 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 2810 "new_file_pattern.cpp"
				
				goto _st32;
				_ctr223:
				{
#line 33 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
					rl_fname.append(buf);
				}
				
#line 2823 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2829 "new_file_pattern.cpp"
				
				{
#line 99 "new_file_pattern.rl"
					rl_err = p; }
				
#line 2835 "new_file_pattern.cpp"
				
				goto _st32;
				_st32:
				if ( p == eof ) {
					if ( cs >= 63 )
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
						goto _st76;
					}
					case 68: {
						goto _st33;
					}
					case 72: {
						goto _st38;
					}
					case 77: {
						goto _st42;
					}
					case 78: {
						goto _st49;
					}
					case 83: {
						goto _st52;
					}
					case 84: {
						goto _st55;
					}
					case 89: {
						goto _st59;
					}
				}
				{
					goto _ctr2;
				}
				_ctr184:
				{
#line 98 "new_file_pattern.rl"
					rl_fname += '%'; }
				
#line 2883 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 2889 "new_file_pattern.cpp"
				
				goto _st76;
				_st76:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof76;
				st_case_76:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr186;
					}
					case 37: {
						goto _ctr187;
					}
				}
				{
					goto _ctr185;
				}
				_ctr82:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2919 "new_file_pattern.cpp"
				
				goto _st33;
				_st33:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof33;
				st_case_33:
				if ( ( (*( p))) == 65 ) {
					goto _st34;
				}
				{
					goto _ctr2;
				}
				_ctr84:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2944 "new_file_pattern.cpp"
				
				goto _st34;
				_st34:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof34;
				st_case_34:
				switch( ( (*( p))) ) {
					case 84: {
						goto _st35;
					}
					case 89: {
						goto _st37;
					}
				}
				{
					goto _ctr2;
				}
				_ctr87:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2974 "new_file_pattern.cpp"
				
				goto _st35;
				_st35:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof35;
				st_case_35:
				if ( ( (*( p))) == 69 ) {
					goto _st36;
				}
				{
					goto _ctr2;
				}
				_ctr89:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 2999 "new_file_pattern.cpp"
				
				goto _st36;
				_st36:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof36;
				st_case_36:
				if ( ( (*( p))) == 37 ) {
					goto _st77;
				}
				{
					goto _ctr2;
				}
				_ctr188:
				{
#line 71 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 3028 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 3034 "new_file_pattern.cpp"
				
				goto _st77;
				_st77:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof77;
				st_case_77:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr190;
					}
					case 37: {
						goto _ctr191;
					}
				}
				{
					goto _ctr189;
				}
				_ctr91:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3064 "new_file_pattern.cpp"
				
				goto _st37;
				_st37:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof37;
				st_case_37:
				if ( ( (*( p))) == 37 ) {
					goto _st78;
				}
				{
					goto _ctr2;
				}
				_ctr192:
				{
#line 47 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
					rl_fname.append(buf);
				}
				
#line 3093 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 3099 "new_file_pattern.cpp"
				
				goto _st78;
				_st78:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof78;
				st_case_78:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr194;
					}
					case 37: {
						goto _ctr195;
					}
				}
				{
					goto _ctr193;
				}
				_ctr93:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3129 "new_file_pattern.cpp"
				
				goto _st38;
				_st38:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof38;
				st_case_38:
				if ( ( (*( p))) == 79 ) {
					goto _st39;
				}
				{
					goto _ctr2;
				}
				_ctr95:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3154 "new_file_pattern.cpp"
				
				goto _st39;
				_st39:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof39;
				st_case_39:
				if ( ( (*( p))) == 85 ) {
					goto _st40;
				}
				{
					goto _ctr2;
				}
				_ctr97:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3179 "new_file_pattern.cpp"
				
				goto _st40;
				_st40:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof40;
				st_case_40:
				if ( ( (*( p))) == 82 ) {
					goto _st41;
				}
				{
					goto _ctr2;
				}
				_ctr99:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3204 "new_file_pattern.cpp"
				
				goto _st41;
				_st41:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof41;
				st_case_41:
				if ( ( (*( p))) == 37 ) {
					goto _st79;
				}
				{
					goto _ctr2;
				}
				_ctr196:
				{
#line 53 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
					rl_fname.append(buf);
				}
				
#line 3233 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 3239 "new_file_pattern.cpp"
				
				goto _st79;
				_st79:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof79;
				st_case_79:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr198;
					}
					case 37: {
						goto _ctr199;
					}
				}
				{
					goto _ctr197;
				}
				_ctr101:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3269 "new_file_pattern.cpp"
				
				goto _st42;
				_st42:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof42;
				st_case_42:
				switch( ( (*( p))) ) {
					case 73: {
						goto _st43;
					}
					case 79: {
						goto _st45;
					}
				}
				{
					goto _ctr2;
				}
				_ctr104:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3299 "new_file_pattern.cpp"
				
				goto _st43;
				_st43:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof43;
				st_case_43:
				if ( ( (*( p))) == 78 ) {
					goto _st44;
				}
				{
					goto _ctr2;
				}
				_ctr106:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3324 "new_file_pattern.cpp"
				
				goto _st44;
				_st44:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof44;
				st_case_44:
				if ( ( (*( p))) == 37 ) {
					goto _st80;
				}
				{
					goto _ctr2;
				}
				_ctr200:
				{
#line 59 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
					rl_fname.append(buf);
				}
				
#line 3353 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 3359 "new_file_pattern.cpp"
				
				goto _st80;
				_st80:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof80;
				st_case_80:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr202;
					}
					case 37: {
						goto _ctr203;
					}
				}
				{
					goto _ctr201;
				}
				_ctr108:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3389 "new_file_pattern.cpp"
				
				goto _st45;
				_st45:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof45;
				st_case_45:
				if ( ( (*( p))) == 78 ) {
					goto _st46;
				}
				{
					goto _ctr2;
				}
				_ctr110:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3414 "new_file_pattern.cpp"
				
				goto _st46;
				_st46:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof46;
				st_case_46:
				if ( ( (*( p))) == 84 ) {
					goto _st47;
				}
				{
					goto _ctr2;
				}
				_ctr112:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3439 "new_file_pattern.cpp"
				
				goto _st47;
				_st47:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof47;
				st_case_47:
				if ( ( (*( p))) == 72 ) {
					goto _st48;
				}
				{
					goto _ctr2;
				}
				_ctr114:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3464 "new_file_pattern.cpp"
				
				goto _st48;
				_st48:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof48;
				st_case_48:
				if ( ( (*( p))) == 37 ) {
					goto _st81;
				}
				{
					goto _ctr2;
				}
				_ctr204:
				{
#line 40 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
					rl_fname.append(buf);
				}
				
#line 3494 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 3500 "new_file_pattern.cpp"
				
				goto _st81;
				_st81:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof81;
				st_case_81:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr206;
					}
					case 37: {
						goto _ctr207;
					}
				}
				{
					goto _ctr205;
				}
				_ctr116:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3530 "new_file_pattern.cpp"
				
				goto _st49;
				_st49:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof49;
				st_case_49:
				if ( ( (*( p))) == 79 ) {
					goto _st50;
				}
				{
					goto _ctr2;
				}
				_ctr118:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3555 "new_file_pattern.cpp"
				
				goto _st50;
				_st50:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof50;
				st_case_50:
				if ( ( (*( p))) == 87 ) {
					goto _st51;
				}
				{
					goto _ctr2;
				}
				_ctr120:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3580 "new_file_pattern.cpp"
				
				goto _st51;
				_st51:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof51;
				st_case_51:
				if ( ( (*( p))) == 37 ) {
					goto _st82;
				}
				{
					goto _ctr2;
				}
				_ctr208:
				{
#line 83 "new_file_pattern.rl"
					
					char buf[64];
					if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 3609 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 3615 "new_file_pattern.cpp"
				
				goto _st82;
				_st82:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof82;
				st_case_82:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr210;
					}
					case 37: {
						goto _ctr211;
					}
				}
				{
					goto _ctr209;
				}
				_ctr122:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3645 "new_file_pattern.cpp"
				
				goto _st52;
				_st52:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof52;
				st_case_52:
				if ( ( (*( p))) == 69 ) {
					goto _st53;
				}
				{
					goto _ctr2;
				}
				_ctr124:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3670 "new_file_pattern.cpp"
				
				goto _st53;
				_st53:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof53;
				st_case_53:
				if ( ( (*( p))) == 67 ) {
					goto _st54;
				}
				{
					goto _ctr2;
				}
				_ctr126:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3695 "new_file_pattern.cpp"
				
				goto _st54;
				_st54:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof54;
				st_case_54:
				if ( ( (*( p))) == 37 ) {
					goto _st83;
				}
				{
					goto _ctr2;
				}
				_ctr212:
				{
#line 65 "new_file_pattern.rl"
					
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 3724 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 3730 "new_file_pattern.cpp"
				
				goto _st83;
				_st83:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof83;
				st_case_83:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr214;
					}
					case 37: {
						goto _ctr215;
					}
				}
				{
					goto _ctr213;
				}
				_ctr128:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3760 "new_file_pattern.cpp"
				
				goto _st55;
				_st55:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof55;
				st_case_55:
				if ( ( (*( p))) == 73 ) {
					goto _st56;
				}
				{
					goto _ctr2;
				}
				_ctr130:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3785 "new_file_pattern.cpp"
				
				goto _st56;
				_st56:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof56;
				st_case_56:
				if ( ( (*( p))) == 77 ) {
					goto _st57;
				}
				{
					goto _ctr2;
				}
				_ctr132:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3810 "new_file_pattern.cpp"
				
				goto _st57;
				_st57:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof57;
				st_case_57:
				if ( ( (*( p))) == 69 ) {
					goto _st58;
				}
				{
					goto _ctr2;
				}
				_ctr134:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3835 "new_file_pattern.cpp"
				
				goto _st58;
				_st58:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof58;
				st_case_58:
				if ( ( (*( p))) == 37 ) {
					goto _st84;
				}
				{
					goto _ctr2;
				}
				_ctr216:
				{
#line 77 "new_file_pattern.rl"
					
					char buf[32];
					if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
					rl_fname.append(buf);
				}
				
#line 3864 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 3870 "new_file_pattern.cpp"
				
				goto _st84;
				_st84:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof84;
				st_case_84:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr218;
					}
					case 37: {
						goto _ctr219;
					}
				}
				{
					goto _ctr217;
				}
				_ctr136:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3900 "new_file_pattern.cpp"
				
				goto _st59;
				_st59:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof59;
				st_case_59:
				if ( ( (*( p))) == 69 ) {
					goto _st60;
				}
				{
					goto _ctr2;
				}
				_ctr138:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3925 "new_file_pattern.cpp"
				
				goto _st60;
				_st60:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof60;
				st_case_60:
				if ( ( (*( p))) == 65 ) {
					goto _st61;
				}
				{
					goto _ctr2;
				}
				_ctr140:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3950 "new_file_pattern.cpp"
				
				goto _st61;
				_st61:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof61;
				st_case_61:
				if ( ( (*( p))) == 82 ) {
					goto _st62;
				}
				{
					goto _ctr2;
				}
				_ctr142:
				{
#line 101 "new_file_pattern.rl"
					rl_fname += '%'; p = rl_err; {goto _st63;}}
				
#line 3975 "new_file_pattern.cpp"
				
				goto _st62;
				_st62:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof62;
				st_case_62:
				if ( ( (*( p))) == 37 ) {
					goto _st85;
				}
				{
					goto _ctr2;
				}
				_ctr220:
				{
#line 33 "new_file_pattern.rl"
					
					std::time_t t = std::time(nullptr);
					char buf[8];
					if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
					rl_fname.append(buf);
				}
				
#line 4005 "new_file_pattern.cpp"
				
				{
#line 100 "new_file_pattern.rl"
					rl_date_found = true; }
				
#line 4011 "new_file_pattern.cpp"
				
				goto _st85;
				_st85:
				if ( p == eof ) {
					if ( cs >= 63 )
						goto _out;
					else
						goto _pop;
				}
				p+= 1;
				if ( p == pe )
					goto _test_eof85;
				st_case_85:
				switch( ( (*( p))) ) {
					case 35: {
						goto _ctr222;
					}
					case 37: {
						goto _ctr223;
					}
				}
				{
					goto _ctr221;
				}
				st_out:
				_test_eof63: cs = 63; goto _test_eof; 
				_test_eof64: cs = 64; goto _test_eof; 
				_test_eof65: cs = 65; goto _test_eof; 
				_test_eof1: cs = 1; goto _test_eof; 
				_test_eof66: cs = 66; goto _test_eof; 
				_test_eof2: cs = 2; goto _test_eof; 
				_test_eof3: cs = 3; goto _test_eof; 
				_test_eof4: cs = 4; goto _test_eof; 
				_test_eof5: cs = 5; goto _test_eof; 
				_test_eof67: cs = 67; goto _test_eof; 
				_test_eof6: cs = 6; goto _test_eof; 
				_test_eof68: cs = 68; goto _test_eof; 
				_test_eof7: cs = 7; goto _test_eof; 
				_test_eof8: cs = 8; goto _test_eof; 
				_test_eof9: cs = 9; goto _test_eof; 
				_test_eof10: cs = 10; goto _test_eof; 
				_test_eof69: cs = 69; goto _test_eof; 
				_test_eof11: cs = 11; goto _test_eof; 
				_test_eof12: cs = 12; goto _test_eof; 
				_test_eof13: cs = 13; goto _test_eof; 
				_test_eof70: cs = 70; goto _test_eof; 
				_test_eof14: cs = 14; goto _test_eof; 
				_test_eof15: cs = 15; goto _test_eof; 
				_test_eof16: cs = 16; goto _test_eof; 
				_test_eof17: cs = 17; goto _test_eof; 
				_test_eof71: cs = 71; goto _test_eof; 
				_test_eof18: cs = 18; goto _test_eof; 
				_test_eof19: cs = 19; goto _test_eof; 
				_test_eof20: cs = 20; goto _test_eof; 
				_test_eof72: cs = 72; goto _test_eof; 
				_test_eof21: cs = 21; goto _test_eof; 
				_test_eof22: cs = 22; goto _test_eof; 
				_test_eof23: cs = 23; goto _test_eof; 
				_test_eof73: cs = 73; goto _test_eof; 
				_test_eof24: cs = 24; goto _test_eof; 
				_test_eof25: cs = 25; goto _test_eof; 
				_test_eof26: cs = 26; goto _test_eof; 
				_test_eof27: cs = 27; goto _test_eof; 
				_test_eof74: cs = 74; goto _test_eof; 
				_test_eof28: cs = 28; goto _test_eof; 
				_test_eof29: cs = 29; goto _test_eof; 
				_test_eof30: cs = 30; goto _test_eof; 
				_test_eof31: cs = 31; goto _test_eof; 
				_test_eof75: cs = 75; goto _test_eof; 
				_test_eof32: cs = 32; goto _test_eof; 
				_test_eof76: cs = 76; goto _test_eof; 
				_test_eof33: cs = 33; goto _test_eof; 
				_test_eof34: cs = 34; goto _test_eof; 
				_test_eof35: cs = 35; goto _test_eof; 
				_test_eof36: cs = 36; goto _test_eof; 
				_test_eof77: cs = 77; goto _test_eof; 
				_test_eof37: cs = 37; goto _test_eof; 
				_test_eof78: cs = 78; goto _test_eof; 
				_test_eof38: cs = 38; goto _test_eof; 
				_test_eof39: cs = 39; goto _test_eof; 
				_test_eof40: cs = 40; goto _test_eof; 
				_test_eof41: cs = 41; goto _test_eof; 
				_test_eof79: cs = 79; goto _test_eof; 
				_test_eof42: cs = 42; goto _test_eof; 
				_test_eof43: cs = 43; goto _test_eof; 
				_test_eof44: cs = 44; goto _test_eof; 
				_test_eof80: cs = 80; goto _test_eof; 
				_test_eof45: cs = 45; goto _test_eof; 
				_test_eof46: cs = 46; goto _test_eof; 
				_test_eof47: cs = 47; goto _test_eof; 
				_test_eof48: cs = 48; goto _test_eof; 
				_test_eof81: cs = 81; goto _test_eof; 
				_test_eof49: cs = 49; goto _test_eof; 
				_test_eof50: cs = 50; goto _test_eof; 
				_test_eof51: cs = 51; goto _test_eof; 
				_test_eof82: cs = 82; goto _test_eof; 
				_test_eof52: cs = 52; goto _test_eof; 
				_test_eof53: cs = 53; goto _test_eof; 
				_test_eof54: cs = 54; goto _test_eof; 
				_test_eof83: cs = 83; goto _test_eof; 
				_test_eof55: cs = 55; goto _test_eof; 
				_test_eof56: cs = 56; goto _test_eof; 
				_test_eof57: cs = 57; goto _test_eof; 
				_test_eof58: cs = 58; goto _test_eof; 
				_test_eof84: cs = 84; goto _test_eof; 
				_test_eof59: cs = 59; goto _test_eof; 
				_test_eof60: cs = 60; goto _test_eof; 
				_test_eof61: cs = 61; goto _test_eof; 
				_test_eof62: cs = 62; goto _test_eof; 
				_test_eof85: cs = 85; goto _test_eof; 
				
				_test_eof: {}
				if ( p == eof ) {
					switch ( cs ) {
						case 63: {
							break;
						}
						case 64: {
							break;
						}
						case 65: {
							break;
						}
						case 0: {
							break;
						}
						case 1: {
							break;
						}
						case 66: {
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
						case 67: {
							break;
						}
						case 6: {
							break;
						}
						case 68: {
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
						case 69: {
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
						case 70: {
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
						case 71: {
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
						case 72: {
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
						case 73: {
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
						case 74: {
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
						case 75: {
							break;
						}
						case 32: {
							break;
						}
						case 76: {
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
						case 77: {
							break;
						}
						case 37: {
							break;
						}
						case 78: {
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
						case 79: {
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
						case 80: {
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
						case 81: {
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
						case 82: {
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
						case 83: {
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
						case 84: {
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
						case 85: {
							break;
						}
					}
					switch ( cs ) {
					}
					switch ( cs ) {
						case 63:
						goto _st63;case 64:
						goto _ctr148;case 65:
						goto _st65;case 0:
						goto _st0;case 1:
						goto _ctr1;case 66:
						goto _ctr154;case 2:
						goto _ctr11;case 3:
						goto _ctr13;case 4:
						goto _ctr16;case 5:
						goto _ctr18;case 67:
						goto _ctr157;case 6:
						goto _ctr20;case 68:
						goto _ctr160;case 7:
						goto _ctr22;case 8:
						goto _ctr24;case 9:
						goto _ctr26;case 10:
						goto _ctr28;case 69:
						goto _ctr163;case 11:
						goto _ctr30;case 12:
						goto _ctr33;case 13:
						goto _ctr35;case 70:
						goto _ctr166;case 14:
						goto _ctr37;case 15:
						goto _ctr39;case 16:
						goto _ctr41;case 17:
						goto _ctr43;case 71:
						goto _ctr169;case 18:
						goto _ctr45;case 19:
						goto _ctr47;case 20:
						goto _ctr49;case 72:
						goto _ctr172;case 21:
						goto _ctr51;case 22:
						goto _ctr53;case 23:
						goto _ctr55;case 73:
						goto _ctr175;case 24:
						goto _ctr57;case 25:
						goto _ctr59;case 26:
						goto _ctr61;case 27:
						goto _ctr63;case 74:
						goto _ctr178;case 28:
						goto _ctr65;case 29:
						goto _ctr67;case 30:
						goto _ctr69;case 31:
						goto _ctr71;case 75:
						goto _ctr181;case 32:
						goto _ctr73;case 76:
						goto _ctr184;case 33:
						goto _ctr82;case 34:
						goto _ctr84;case 35:
						goto _ctr87;case 36:
						goto _ctr89;case 77:
						goto _ctr188;case 37:
						goto _ctr91;case 78:
						goto _ctr192;case 38:
						goto _ctr93;case 39:
						goto _ctr95;case 40:
						goto _ctr97;case 41:
						goto _ctr99;case 79:
						goto _ctr196;case 42:
						goto _ctr101;case 43:
						goto _ctr104;case 44:
						goto _ctr106;case 80:
						goto _ctr200;case 45:
						goto _ctr108;case 46:
						goto _ctr110;case 47:
						goto _ctr112;case 48:
						goto _ctr114;case 81:
						goto _ctr204;case 49:
						goto _ctr116;case 50:
						goto _ctr118;case 51:
						goto _ctr120;case 82:
						goto _ctr208;case 52:
						goto _ctr122;case 53:
						goto _ctr124;case 54:
						goto _ctr126;case 83:
						goto _ctr212;case 55:
						goto _ctr128;case 56:
						goto _ctr130;case 57:
						goto _ctr132;case 58:
						goto _ctr134;case 84:
						goto _ctr216;case 59:
						goto _ctr136;case 60:
						goto _ctr138;case 61:
						goto _ctr140;case 62:
						goto _ctr142;case 85:
						goto _ctr220;	}
				}
				
				if ( cs >= 63 )
					goto _out; _pop: {}
				_out: {}
			}
			
#line 135 "new_file_pattern.rl"
			
			
			if ( cs >= 
#line 4486 "new_file_pattern.cpp"
			63
#line 137 "new_file_pattern.rl"
			) {
				if (rl_pattern_found || rl_date_found) {
					res = rl_fname;
					return PATTERN_DONE;
				} else {
					char suffix[8];
					snprintf(suffix, sizeof(suffix), "-%03d", idx % 1000);
					auto extpos = rl_fname.find_last_of(".");
					if (extpos == std::string::npos) {
						rl_fname += suffix;
					} else {
						rl_fname.insert(extpos, suffix);
					}
					res = rl_fname;
					return PATTERN_NOMATCH;
				}
			} else
			return PATTERN_ERROR;
		}
		
	}
}


