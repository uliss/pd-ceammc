/* C++ code produced by gperf version 3.1 */
/* Command-line: /opt/local/bin/gperf -L C++ --class-name=MaterialFontHash -m 50 --struct-type --output-file=gperf_material_font.h material_font.gperf  */
/* Computed positions: -k'1-4,6,8-9,13-15,17,21,39,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 5 "material_font.gperf"

# include <cstring>
# include <vector>
# include <algorithm>

class MaterialFontNamesAll {
    mutable std::vector<const char*> names_;
    mutable bool sorted_ {false};
    MaterialFontNamesAll() { names_.reserve(2300); };
public:
    static inline MaterialFontNamesAll& instance() {
        static MaterialFontNamesAll instance_;
        return instance_;
    }

    inline void add(const char* name) { names_.push_back(name); }
    const std::vector<const char*>& names() const {
        if (!sorted_) { // signle sort
            sorted_ = true;
            std::sort(names_.begin(), names_.end(), [](const char* a, const char* b) { return std::strcmp(a, b) < 0; });
        }
        return names_;
    }
};
#line 30 "material_font.gperf"
struct MaterialFontNames
{
    const char* name;
    const char* glyph;
    MaterialFontNames(const char* n, const char* g = "")
        : name(n),
        glyph(g)
    {
        if (n[0] != '\0')
            MaterialFontNamesAll::instance().add(name);
    }
};
#include <string.h>
/* maximum key range = 40272, duplicates = 0 */

class MaterialFontHash
{
private:
  static inline unsigned int hash (const char *str, size_t len);
public:
  static const struct MaterialFontNames *in_word_set (const char *str, size_t len);
};

inline unsigned int
MaterialFontHash::hash (const char *str, size_t len)
{
  static const unsigned short asso_values[] =
    {
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,    12,    47,
         22,    84,    69,    18,    26,    17,    21,    14,    13,    12,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353,    81,    17,    27,  7361,    22,
        199,    14,   972,   314,  6159,   474,  3101,  3033,    15,  1750,
         53,   103,   491,  1617,    12,    17,    12,  4288,  8268,  5446,
        959,  8625,    57,    54, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353,
      40353, 40353, 40353, 40353, 40353, 40353, 40353, 40353
    };
  unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[static_cast<unsigned char>(str[38])];
      /*FALLTHROUGH*/
      case 38:
      case 37:
      case 36:
      case 35:
      case 34:
      case 33:
      case 32:
      case 31:
      case 30:
      case 29:
      case 28:
      case 27:
      case 26:
      case 25:
      case 24:
      case 23:
      case 22:
      case 21:
        hval += asso_values[static_cast<unsigned char>(str[20])];
      /*FALLTHROUGH*/
      case 20:
      case 19:
      case 18:
      case 17:
        hval += asso_values[static_cast<unsigned char>(str[16]+1)];
      /*FALLTHROUGH*/
      case 16:
      case 15:
        hval += asso_values[static_cast<unsigned char>(str[14])];
      /*FALLTHROUGH*/
      case 14:
        hval += asso_values[static_cast<unsigned char>(str[13])];
      /*FALLTHROUGH*/
      case 13:
        hval += asso_values[static_cast<unsigned char>(str[12])];
      /*FALLTHROUGH*/
      case 12:
      case 11:
      case 10:
      case 9:
        hval += asso_values[static_cast<unsigned char>(str[8])];
      /*FALLTHROUGH*/
      case 8:
        hval += asso_values[static_cast<unsigned char>(str[7])];
      /*FALLTHROUGH*/
      case 7:
      case 6:
        hval += asso_values[static_cast<unsigned char>(str[5])];
      /*FALLTHROUGH*/
      case 5:
      case 4:
        hval += asso_values[static_cast<unsigned char>(str[3])];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[static_cast<unsigned char>(str[2])];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[static_cast<unsigned char>(str[1]+2)];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[static_cast<unsigned char>(str[0])];
        break;
    }
  return hval + asso_values[static_cast<unsigned char>(str[len - 1])];
}

const struct MaterialFontNames *
MaterialFontHash::in_word_set (const char *str, size_t len)
{
  enum
    {
      TOTAL_KEYWORDS = 2233,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 45,
      MIN_HASH_VALUE = 81,
      MAX_HASH_VALUE = 40352
    };

  static const struct MaterialFontNames wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 688 "material_font.gperf"
      {"east", "\uF1DF"},
#line 705 "material_font.gperf"
      {"eject", "\uE8FB"},
      {""}, {""}, {""},
#line 1879 "material_font.gperf"
      {"spa", "\uEB4C"},
      {""}, {""},
#line 416 "material_font.gperf"
      {"cast", "\uE307"},
      {""}, {""},
#line 1723 "material_font.gperf"
      {"scale", "\uEB5F"},
#line 384 "material_font.gperf"
      {"call", "\uE0B0"},
      {""}, {""}, {""},
#line 414 "material_font.gperf"
      {"cases", "\uE992"},
      {""}, {""}, {""}, {""},
#line 1338 "material_font.gperf"
      {"nat", "\uEF5C"},
#line 1885 "material_font.gperf"
      {"speaker", "\uE32D"},
      {""}, {""}, {""},
#line 419 "material_font.gperf"
      {"castle", "\uEAB1"},
      {""},
#line 523 "material_font.gperf"
      {"create", "\uE150"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1716 "material_font.gperf"
      {"satellite", "\uE562"},
      {""}, {""},
#line 70 "material_font.gperf"
      {"360", "\uE577"},
#line 455 "material_font.gperf"
      {"clear", "\uE14C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 452 "material_font.gperf"
      {"class", "\uE86E"},
      {""},
#line 1717 "material_font.gperf"
      {"satellite_alt", "\uEB3A"},
#line 1724 "material_font.gperf"
      {"scanner", "\uE329"},
#line 378 "material_font.gperf"
      {"calculate", "\uEA5F"},
#line 1120 "material_font.gperf"
      {"lan", "\uEB2F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 403 "material_font.gperf"
      {"cancel", "\uE5C9"},
      {""},
#line 1858 "material_font.gperf"
      {"sms", "\uE625"},
#line 735 "material_font.gperf"
      {"error", "\uE000"},
      {""}, {""}, {""}, {""},
#line 206 "material_font.gperf"
      {"area_chart", "\uE770"},
      {""},
#line 1731 "material_font.gperf"
      {"score", "\uE269"},
      {""}, {""},
#line 2085 "material_font.gperf"
      {"translate", "\uE8E2"},
      {""}, {""}, {""},
#line 1992 "material_font.gperf"
      {"sync", "\uE627"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 104 "material_font.gperf"
      {"access_alarms", "\uE191"},
#line 1894 "material_font.gperf"
      {"sports", "\uEA30"},
      {""}, {""},
#line 555 "material_font.gperf"
      {"cyclone", "\uEBD5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1993 "material_font.gperf"
      {"sync_alt", "\uEA18"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 457 "material_font.gperf"
      {"close", "\uE5CD"},
#line 408 "material_font.gperf"
      {"car_rental", "\uEA55"},
      {""}, {""}, {""},
#line 1880 "material_font.gperf"
      {"space_bar", "\uE256"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1910 "material_font.gperf"
      {"sports_score", "\uF06E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1912 "material_font.gperf"
      {"sports_tennis", "\uEA32"},
      {""}, {""},
#line 689 "material_font.gperf"
      {"eco", "\uEA35"},
      {""}, {""}, {""}, {""},
#line 710 "material_font.gperf"
      {"electric_car", "\uEB1C"},
      {""}, {""}, {""}, {""},
#line 1898 "material_font.gperf"
      {"sports_cricket", "\uEA27"},
      {""}, {""}, {""},
#line 456 "material_font.gperf"
      {"clear_all", "\uE0B8"},
#line 1899 "material_font.gperf"
      {"sports_esports", "\uEA28"},
      {""}, {""},
#line 709 "material_font.gperf"
      {"electric_bolt", "\uEC1C"},
      {""}, {""}, {""},
#line 708 "material_font.gperf"
      {"electric_bike", "\uEB1B"},
      {""},
#line 1890 "material_font.gperf"
      {"speed", "\uE9E4"},
      {""}, {""}, {""},
#line 1887 "material_font.gperf"
      {"speaker_notes", "\uE8CD"},
      {""}, {""}, {""}, {""},
#line 711 "material_font.gperf"
      {"electric_meter", "\uEC1B"},
      {""}, {""},
#line 101 "material_font.gperf"
      {"abc", "\uEB94"},
      {""}, {""}, {""},
#line 1618 "material_font.gperf"
      {"radar", "\uF04E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 47 "material_font.gperf"
      {"123", "\uEB8D"},
      {""}, {""}, {""}, {""}, {""},
#line 567 "material_font.gperf"
      {"dataset", "\uF8EE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 453 "material_font.gperf"
      {"clean_hands", "\uF21F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 174 "material_font.gperf"
      {"alarm_on", "\uE858"},
      {""}, {""}, {""},
#line 1911 "material_font.gperf"
      {"sports_soccer", "\uEA2F"},
      {""}, {""}, {""}, {""}, {""},
#line 527 "material_font.gperf"
      {"credit_score", "\uEFF1"},
#line 185 "material_font.gperf"
      {"alt_route", "\uF184"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 764 "material_font.gperf"
      {"extension", "\uE87B"},
      {""}, {""}, {""}, {""}, {""},
#line 115 "material_font.gperf"
      {"account_tree", "\uE97A"},
#line 1853 "material_font.gperf"
      {"smart_screen", "\uF06B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 714 "material_font.gperf"
      {"electric_scooter", "\uEB1F"},
      {""}, {""}, {""},
#line 114 "material_font.gperf"
      {"account_circle", "\uE853"},
#line 1756 "material_font.gperf"
      {"sell", "\uF05B"},
      {""}, {""}, {""},
#line 1733 "material_font.gperf"
      {"screen_lock_landscape", "\uE1BE"},
      {""}, {""}, {""},
#line 1679 "material_font.gperf"
      {"restore", "\uE8B3"},
      {""}, {""}, {""},
#line 951 "material_font.gperf"
      {"grass", "\uF205"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 928 "material_font.gperf"
      {"garage", "\uF011"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 392 "material_font.gperf"
      {"call_to_action", "\uE06C"},
      {""}, {""},
#line 1137 "material_font.gperf"
      {"lens", "\uE3FA"},
      {""},
#line 1882 "material_font.gperf"
      {"spatial_audio", "\uEBEB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 208 "material_font.gperf"
      {"arrow_back_ios", "\uE5E0"},
      {""},
#line 1764 "material_font.gperf"
      {"sensors", "\uE51E"},
      {""},
#line 1754 "material_font.gperf"
      {"select_all", "\uE162"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1773 "material_font.gperf"
      {"settings", "\uE8B8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1639 "material_font.gperf"
      {"rectangle", "\uEB54"},
      {""},
#line 1123 "material_font.gperf"
      {"language", "\uE894"},
      {""}, {""},
#line 1636 "material_font.gperf"
      {"recent_actors", "\uE03F"},
      {""}, {""}, {""},
#line 1656 "material_font.gperf"
      {"reorder", "\uE8FE"},
      {""},
#line 1772 "material_font.gperf"
      {"set_meal", "\uF1EA"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1859 "material_font.gperf"
      {"sms_failed", "\uE626"},
      {""},
#line 925 "material_font.gperf"
      {"g_translate", "\uE927"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 385 "material_font.gperf"
      {"call_end", "\uE0B1"},
#line 929 "material_font.gperf"
      {"gas_meter", "\uEC19"},
      {""}, {""},
#line 1676 "material_font.gperf"
      {"restart_alt", "\uF053"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2014 "material_font.gperf"
      {"tag_faces", "\uE420"},
      {""}, {""}, {""}, {""},
#line 525 "material_font.gperf"
      {"credit_card", "\uE870"},
#line 524 "material_font.gperf"
      {"create_new_folder", "\uE2CC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 390 "material_font.gperf"
      {"call_received", "\uE0B5"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1122 "material_font.gperf"
      {"landslide", "\uEBD7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1779 "material_font.gperf"
      {"settings_cell", "\uE8BC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1789 "material_font.gperf"
      {"settings_power", "\uE8C6"},
      {""}, {""},
#line 226 "material_font.gperf"
      {"article", "\uEF42"},
#line 1730 "material_font.gperf"
      {"science", "\uEA4B"},
      {""},
#line 200 "material_font.gperf"
      {"apple", "\uEA80"},
      {""}, {""}, {""},
#line 202 "material_font.gperf"
      {"apps", "\uE5C3"},
      {""},
#line 1793 "material_font.gperf"
      {"settings_voice", "\uE8C8"},
#line 946 "material_font.gperf"
      {"grade", "\uE885"},
      {""},
#line 1438 "material_font.gperf"
      {"pallet", "\uF86A"},
#line 2017 "material_font.gperf"
      {"tapas", "\uF1E9"},
#line 1437 "material_font.gperf"
      {"palette", "\uE40A"},
#line 573 "material_font.gperf"
      {"delete", "\uE872"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2078 "material_font.gperf"
      {"train", "\uE570"},
      {""}, {""}, {""}, {""}, {""},
#line 76 "material_font.gperf"
      {"3p", "\uEFD1"},
#line 583 "material_font.gperf"
      {"deselect", "\uEBB6"},
#line 589 "material_font.gperf"
      {"details", "\uE3C8"},
      {""}, {""}, {""},
#line 1226 "material_font.gperf"
      {"lyrics", "\uEC0B"},
      {""}, {""},
#line 947 "material_font.gperf"
      {"gradient", "\uE3E9"},
      {""}, {""},
#line 1788 "material_font.gperf"
      {"settings_phone", "\uE8C5"},
#line 391 "material_font.gperf"
      {"call_split", "\uE0B6"},
      {""}, {""},
#line 105 "material_font.gperf"
      {"access_time", "\uE192"},
      {""}, {""},
#line 529 "material_font.gperf"
      {"crisis_alert", "\uEBE9"},
#line 1456 "material_font.gperf"
      {"pattern", "\uF043"},
#line 754 "material_font.gperf"
      {"explore", "\uE87A"},
      {""},
#line 1548 "material_font.gperf"
      {"place", "\uE55F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1892 "material_font.gperf"
      {"splitscreen", "\uF06D"},
      {""},
#line 417 "material_font.gperf"
      {"cast_connected", "\uE308"},
      {""}, {""}, {""},
#line 753 "material_font.gperf"
      {"explicit", "\uE01E"},
      {""}, {""}, {""}, {""}, {""},
#line 188 "material_font.gperf"
      {"analytics", "\uEF3E"},
#line 717 "material_font.gperf"
      {"email", "\uE0BE"},
      {""}, {""}, {""}, {""}, {""},
#line 712 "material_font.gperf"
      {"electric_moped", "\uEB1D"},
      {""}, {""},
#line 413 "material_font.gperf"
      {"carpenter", "\uF1F8"},
      {""}, {""},
#line 569 "material_font.gperf"
      {"date_range", "\uE916"},
      {""}, {""}, {""}, {""},
#line 1881 "material_font.gperf"
      {"space_dashboard", "\uE66B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1790 "material_font.gperf"
      {"settings_remote", "\uE8C7"},
#line 2013 "material_font.gperf"
      {"tag", "\uE9EF"},
#line 459 "material_font.gperf"
      {"closed_caption", "\uE01C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 409 "material_font.gperf"
      {"car_repair", "\uEA56"},
      {""}, {""}, {""}, {""}, {""},
#line 1725 "material_font.gperf"
      {"scatter_plot", "\uE268"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 534 "material_font.gperf"
      {"crop_7_5", "\uE3C0"},
#line 932 "material_font.gperf"
      {"gesture", "\uE155"},
      {""}, {""},
#line 2083 "material_font.gperf"
      {"transgender", "\uE58D"},
      {""},
#line 1624 "material_font.gperf"
      {"railway_alert", "\uE9D1"},
      {""},
#line 1889 "material_font.gperf"
      {"speaker_phone", "\uE0D2"},
      {""},
#line 1079 "material_font.gperf"
      {"iron", "\uE583"},
      {""}, {""}, {""}, {""},
#line 540 "material_font.gperf"
      {"crop_rotate", "\uE437"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 203 "material_font.gperf"
      {"apps_outage", "\uE7CC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 636 "material_font.gperf"
      {"dns", "\uE875"},
      {""}, {""},
#line 1787 "material_font.gperf"
      {"settings_overscan", "\uE8C4"},
#line 186 "material_font.gperf"
      {"alternate_email", "\uE0E6"},
#line 578 "material_font.gperf"
      {"density_large", "\uEBA9"},
      {""}, {""},
#line 1643 "material_font.gperf"
      {"redo", "\uE15A"},
#line 580 "material_font.gperf"
      {"density_small", "\uEBA8"},
      {""},
#line 1761 "material_font.gperf"
      {"sensor_door", "\uF1B5"},
#line 1582 "material_font.gperf"
      {"present_to_all", "\uE0DF"},
      {""}, {""}, {""},
#line 2110 "material_font.gperf"
      {"type_specimen", "\uF8F0"},
      {""}, {""}, {""},
#line 415 "material_font.gperf"
      {"casino", "\uEB40"},
#line 1862 "material_font.gperf"
      {"snooze", "\uE046"},
      {""}, {""}, {""}, {""}, {""},
#line 1641 "material_font.gperf"
      {"reddit", "\uEAA0"},
      {""}, {""}, {""},
#line 1441 "material_font.gperf"
      {"panorama", "\uE40B"},
      {""}, {""},
#line 218 "material_font.gperf"
      {"arrow_forward", "\uE5C8"},
      {""}, {""},
#line 532 "material_font.gperf"
      {"crop_3_2", "\uE3BD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1757 "material_font.gperf"
      {"send", "\uE163"},
      {""},
#line 1439 "material_font.gperf"
      {"pan_tool", "\uE925"},
      {""}, {""}, {""},
#line 533 "material_font.gperf"
      {"crop_5_4", "\uE3BF"},
#line 531 "material_font.gperf"
      {"crop_16_9", "\uE3BC"},
      {""}, {""},
#line 568 "material_font.gperf"
      {"dataset_linked", "\uF8EF"},
      {""}, {""}, {""},
#line 1994 "material_font.gperf"
      {"sync_disabled", "\uE628"},
      {""}, {""}, {""}, {""}, {""},
#line 172 "material_font.gperf"
      {"alarm_add", "\uE856"},
      {""},
#line 82 "material_font.gperf"
      {"5g", "\uEF38"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1619 "material_font.gperf"
      {"radio", "\uE03E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1855 "material_font.gperf"
      {"smartphone", "\uE32C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1121 "material_font.gperf"
      {"landscape", "\uE3F7"},
      {""}, {""}, {""}, {""}, {""},
#line 751 "material_font.gperf"
      {"expand_less", "\uE5CE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1502 "material_font.gperf"
      {"pets", "\uE91D"},
      {""}, {""}, {""}, {""}, {""},
#line 1658 "material_font.gperf"
      {"repeat", "\uE040"},
      {""},
#line 1434 "material_font.gperf"
      {"pages", "\uE7F9"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1440 "material_font.gperf"
      {"pan_tool_alt", "\uEBB9"},
      {""},
#line 1634 "material_font.gperf"
      {"receipt", "\uE8B0"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1791 "material_font.gperf"
      {"settings_suggest", "\uF05E"},
      {""}, {""}, {""},
#line 1734 "material_font.gperf"
      {"screen_lock_portrait", "\uE1BF"},
#line 2025 "material_font.gperf"
      {"terrain", "\uE564"},
#line 949 "material_font.gperf"
      {"grain", "\uE3EA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1714 "material_font.gperf"
      {"sailing", "\uE502"},
      {""}, {""}, {""}, {""},
#line 1132 "material_font.gperf"
      {"leaderboard", "\uF20C"},
#line 1128 "material_font.gperf"
      {"last_page", "\uE5DD"},
      {""}, {""},
#line 1472 "material_font.gperf"
      {"percent", "\uEB58"},
      {""}, {""}, {""}, {""}, {""},
#line 704 "material_font.gperf"
      {"egg_alt", "\uEAC8"},
      {""}, {""}, {""}, {""}, {""},
#line 1736 "material_font.gperf"
      {"screen_rotation", "\uE1C1"},
      {""}, {""},
#line 948 "material_font.gperf"
      {"grading", "\uEA4F"},
#line 422 "material_font.gperf"
      {"celebration", "\uEA65"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1044 "material_font.gperf"
      {"image", "\uE3F4"},
      {""},
#line 1484 "material_font.gperf"
      {"person_2", "\uF8E4"},
      {""},
#line 535 "material_font.gperf"
      {"crop_din", "\uE3C1"},
      {""}, {""},
#line 1454 "material_font.gperf"
      {"party_mode", "\uE7FA"},
      {""},
#line 1483 "material_font.gperf"
      {"person", "\uE7FD"},
      {""},
#line 1500 "material_font.gperf"
      {"pest_control", "\uF0FA"},
      {""},
#line 1669 "material_font.gperf"
      {"report", "\uE160"},
      {""}, {""}, {""},
#line 1495 "material_font.gperf"
      {"person_remove", "\uEF66"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1681 "material_font.gperf"
      {"restore_page", "\uE929"},
      {""},
#line 1668 "material_font.gperf"
      {"reply_all", "\uE15F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 195 "material_font.gperf"
      {"api", "\uF1B7"},
      {""}, {""},
#line 749 "material_font.gperf"
      {"expand", "\uE94F"},
      {""}, {""}, {""},
#line 2088 "material_font.gperf"
      {"trending_flat", "\uE8E4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1136 "material_font.gperf"
      {"legend_toggle", "\uF11B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1660 "material_font.gperf"
      {"repeat_one", "\uE041"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1486 "material_font.gperf"
      {"person_4", "\uF8E6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2089 "material_font.gperf"
      {"trending_neutral", "\uE8E4"},
      {""}, {""}, {""},
#line 222 "material_font.gperf"
      {"arrow_right", "\uE5DF"},
#line 1586 "material_font.gperf"
      {"print", "\uE8AD"},
#line 766 "material_font.gperf"
      {"face", "\uE87C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1455 "material_font.gperf"
      {"password", "\uF042"},
#line 742 "material_font.gperf"
      {"event", "\uE878"},
      {""}, {""},
#line 1071 "material_font.gperf"
      {"interests", "\uE7C8"},
#line 1659 "material_font.gperf"
      {"repeat_on", "\uE9D6"},
      {""},
#line 1485 "material_font.gperf"
      {"person_3", "\uF8E5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 770 "material_font.gperf"
      {"face_5", "\uF8DD"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2087 "material_font.gperf"
      {"trending_down", "\uE8E3"},
#line 767 "material_font.gperf"
      {"face_2", "\uF8DA"},
      {""}, {""},
#line 844 "material_font.gperf"
      {"flare", "\uE3E4"},
      {""}, {""}, {""},
#line 1715 "material_font.gperf"
      {"sanitizer", "\uF21D"},
#line 771 "material_font.gperf"
      {"face_6", "\uF8DE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1581 "material_font.gperf"
      {"pregnant_woman", "\uE91E"},
      {""}, {""}, {""},
#line 100 "material_font.gperf"
      {"9mp", "\uE979"},
      {""},
#line 223 "material_font.gperf"
      {"arrow_right_alt", "\uE941"},
#line 94 "material_font.gperf"
      {"7mp", "\uE973"},
#line 85 "material_font.gperf"
      {"5mp", "\uE96D"},
      {""}, {""},
#line 97 "material_font.gperf"
      {"8mp", "\uE976"},
#line 67 "material_font.gperf"
      {"2mp", "\uE965"},
      {""}, {""}, {""},
#line 91 "material_font.gperf"
      {"6mp", "\uE970"},
      {""}, {""}, {""},
#line 1661 "material_font.gperf"
      {"repeat_one_on", "\uE9D7"},
#line 703 "material_font.gperf"
      {"egg", "\uEACC"},
      {""}, {""}, {""},
#line 530 "material_font.gperf"
      {"crop", "\uE3BE"},
      {""},
#line 88 "material_font.gperf"
      {"6_ft_apart", "\uF21E"},
      {""},
#line 850 "material_font.gperf"
      {"flatware", "\uF00C"},
      {""}, {""}, {""},
#line 940 "material_font.gperf"
      {"gpp_bad", "\uF012"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 219 "material_font.gperf"
      {"arrow_forward_ios", "\uE5E1"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 81 "material_font.gperf"
      {"4mp", "\uE96A"},
      {""}, {""}, {""}, {""},
#line 956 "material_font.gperf"
      {"grid_on", "\uE3EC"},
      {""}, {""}, {""},
#line 1488 "material_font.gperf"
      {"person_add_alt", "\uEA4D"},
      {""}, {""}, {""}, {""},
#line 1592 "material_font.gperf"
      {"propane", "\uEC14"},
#line 75 "material_font.gperf"
      {"3mp", "\uE968"},
#line 584 "material_font.gperf"
      {"design_services", "\uF10A"},
#line 769 "material_font.gperf"
      {"face_4", "\uF8DC"},
      {""}, {""}, {""}, {""},
#line 748 "material_font.gperf"
      {"exit_to_app", "\uE879"},
#line 539 "material_font.gperf"
      {"crop_portrait", "\uE3C5"},
      {""}, {""}, {""},
#line 747 "material_font.gperf"
      {"event_seat", "\uE903"},
#line 682 "material_font.gperf"
      {"dvr", "\uE1B2"},
      {""}, {""}, {""}, {""},
#line 1743 "material_font.gperf"
      {"sd", "\uE9DD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 768 "material_font.gperf"
      {"face_3", "\uF8DB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 953 "material_font.gperf"
      {"grid_4x4", "\uF016"},
      {""}, {""},
#line 1499 "material_font.gperf"
      {"personal_video", "\uE63B"},
#line 177 "material_font.gperf"
      {"align_horizontal_left", "\uE00D"},
      {""},
#line 176 "material_font.gperf"
      {"align_horizontal_center", "\uE00F"},
      {""}, {""}, {""},
#line 1060 "material_font.gperf"
      {"insert_comment", "\uE24C"},
      {""}, {""}, {""},
#line 187 "material_font.gperf"
      {"amp_stories", "\uEA13"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 220 "material_font.gperf"
      {"arrow_left", "\uE5DE"},
      {""}, {""}, {""}, {""}, {""},
#line 1745 "material_font.gperf"
      {"sd_card_alert", "\uF057"},
#line 669 "material_font.gperf"
      {"drafts", "\uE151"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 952 "material_font.gperf"
      {"grid_3x3", "\uF015"},
#line 745 "material_font.gperf"
      {"event_note", "\uE616"},
      {""},
#line 198 "material_font.gperf"
      {"app_settings_alt", "\uEF41"},
      {""},
#line 847 "material_font.gperf"
      {"flash_on", "\uE3E7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1635 "material_font.gperf"
      {"receipt_long", "\uEF6E"},
      {""},
#line 210 "material_font.gperf"
      {"arrow_circle_down", "\uF181"},
      {""},
#line 146 "material_font.gperf"
      {"addchart", "\uEF3C"},
      {""}, {""}, {""}, {""},
#line 1489 "material_font.gperf"
      {"person_add_alt_1", "\uEF65"},
#line 1433 "material_font.gperf"
      {"padding", "\uE9C8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1468 "material_font.gperf"
      {"pentagon", "\uEB50"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1744 "material_font.gperf"
      {"sd_card", "\uE623"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 943 "material_font.gperf"
      {"gps_fixed", "\uE1B3"},
      {""}, {""},
#line 1487 "material_font.gperf"
      {"person_add", "\uE7FE"},
#line 1900 "material_font.gperf"
      {"sports_football", "\uEA29"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2038 "material_font.gperf"
      {"texture", "\uE421"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 121 "material_font.gperf"
      {"add_alert", "\uE003"},
      {""}, {""}, {""}, {""},
#line 124 "material_font.gperf"
      {"add_call", "\uE0E8"},
#line 217 "material_font.gperf"
      {"arrow_drop_up", "\uE5C7"},
      {""}, {""},
#line 1416 "material_font.gperf"
      {"ondemand_video", "\uE63A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 920 "material_font.gperf"
      {"front_loader", "\uF869"},
#line 132 "material_font.gperf"
      {"add_ic_call", "\uE97C"},
      {""}, {""}, {""},
#line 790 "material_font.gperf"
      {"fence", "\uF1F6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1436 "material_font.gperf"
      {"paid", "\uF041"},
      {""},
#line 1049 "material_font.gperf"
      {"import_contacts", "\uE0E0"},
#line 138 "material_font.gperf"
      {"add_reaction", "\uE1D3"},
#line 1848 "material_font.gperf"
      {"sledding", "\uE512"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 118 "material_font.gperf"
      {"add", "\uE145"},
      {""}, {""}, {""}, {""}, {""},
#line 1657 "material_font.gperf"
      {"repartition", "\uF8E8"},
#line 941 "material_font.gperf"
      {"gpp_good", "\uF013"},
      {""},
#line 190 "material_font.gperf"
      {"android", "\uE859"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 130 "material_font.gperf"
      {"add_home", "\uF8EB"},
      {""}, {""}, {""}, {""},
#line 1467 "material_font.gperf"
      {"pending_actions", "\uF1BB"},
      {""}, {""}, {""}, {""},
#line 537 "material_font.gperf"
      {"crop_landscape", "\uE3C3"},
      {""}, {""},
#line 1466 "material_font.gperf"
      {"pending", "\uEF64"},
      {""},
#line 1469 "material_font.gperf"
      {"people", "\uE7FB"},
      {""},
#line 860 "material_font.gperf"
      {"flood", "\uEBE6"},
      {""}, {""},
#line 561 "material_font.gperf"
      {"data_exploration", "\uE76F"},
#line 1762 "material_font.gperf"
      {"sensor_occupied", "\uEC10"},
      {""}, {""}, {""}, {""},
#line 1446 "material_font.gperf"
      {"panorama_photosphere", "\uE9C9"},
#line 136 "material_font.gperf"
      {"add_moderator", "\uE97D"},
      {""},
#line 842 "material_font.gperf"
      {"flag_circle", "\uEAF8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2090 "material_font.gperf"
      {"trending_up", "\uE8E5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1886 "material_font.gperf"
      {"speaker_group", "\uE32E"},
      {""}, {""},
#line 1470 "material_font.gperf"
      {"people_alt", "\uEA21"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 134 "material_font.gperf"
      {"add_location", "\uE567"},
      {""},
#line 692 "material_font.gperf"
      {"edit", "\uE3C9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 919 "material_font.gperf"
      {"front_hand", "\uE769"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 581 "material_font.gperf"
      {"departure_board", "\uE576"},
      {""}, {""}, {""}, {""},
#line 2077 "material_font.gperf"
      {"traffic", "\uE565"},
      {""},
#line 1124 "material_font.gperf"
      {"laptop", "\uE31E"},
      {""},
#line 1447 "material_font.gperf"
      {"panorama_photosphere_select", "\uE9CA"},
      {""},
#line 780 "material_font.gperf"
      {"fastfood", "\uE57A"},
#line 86 "material_font.gperf"
      {"60fps", "\uEFD4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2020 "material_font.gperf"
      {"taxi_alert", "\uEF74"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1746 "material_font.gperf"
      {"sd_storage", "\uE1C2"},
      {""}, {""}, {""}, {""},
#line 694 "material_font.gperf"
      {"edit_calendar", "\uE742"},
#line 1901 "material_font.gperf"
      {"sports_golf", "\uEA2A"},
#line 538 "material_font.gperf"
      {"crop_original", "\uE3C4"},
      {""}, {""},
#line 1452 "material_font.gperf"
      {"paragliding", "\uE50F"},
      {""}, {""}, {""}, {""}, {""},
#line 2026 "material_font.gperf"
      {"text_decrease", "\uEADD"},
#line 135 "material_font.gperf"
      {"add_location_alt", "\uEF3A"},
      {""},
#line 698 "material_font.gperf"
      {"edit_note", "\uE745"},
      {""},
#line 582 "material_font.gperf"
      {"description", "\uE873"},
      {""}, {""}, {""},
#line 1765 "material_font.gperf"
      {"sensors_off", "\uE51F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 863 "material_font.gperf"
      {"fmd_bad", "\uF00E"},
      {""}, {""},
#line 68 "material_font.gperf"
      {"30fps", "\uEFCE"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 693 "material_font.gperf"
      {"edit_attributes", "\uE578"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 142 "material_font.gperf"
      {"add_to_drive", "\uE65C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 87 "material_font.gperf"
      {"60fps_select", "\uEFD5"},
      {""}, {""}, {""}, {""},
#line 746 "material_font.gperf"
      {"event_repeat", "\uEB7B"},
#line 536 "material_font.gperf"
      {"crop_free", "\uE3C2"},
      {""}, {""}, {""},
#line 696 "material_font.gperf"
      {"edit_location", "\uE568"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2035 "material_font.gperf"
      {"text_rotation_none", "\uE93F"},
#line 1872 "material_font.gperf"
      {"sos", "\uEBF7"},
      {""}, {""},
#line 1870 "material_font.gperf"
      {"sort", "\uE164"},
      {""},
#line 2064 "material_font.gperf"
      {"toc", "\uE8DE"},
      {""},
#line 2069 "material_font.gperf"
      {"toll", "\uE8E0"},
      {""}, {""}, {""}, {""},
#line 574 "material_font.gperf"
      {"delete_forever", "\uE92B"},
#line 841 "material_font.gperf"
      {"flag", "\uE153"},
      {""},
#line 471 "material_font.gperf"
      {"co2", "\uE7B0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 787 "material_font.gperf"
      {"feed", "\uF009"},
#line 69 "material_font.gperf"
      {"30fps_select", "\uEFCF"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 933 "material_font.gperf"
      {"get_app", "\uE884"},
#line 71 "material_font.gperf"
      {"3d_rotation", "\uE84D"},
#line 125 "material_font.gperf"
      {"add_card", "\uEB86"},
      {""},
#line 1396 "material_font.gperf"
      {"note", "\uE06F"},
      {""}, {""}, {""},
#line 1399 "material_font.gperf"
      {"notes", "\uE26C"},
      {""}, {""}, {""},
#line 1888 "material_font.gperf"
      {"speaker_notes_off", "\uE92A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1696 "material_font.gperf"
      {"rotate_left", "\uE419"},
      {""},
#line 1420 "material_font.gperf"
      {"open_in_full", "\uF1CE"},
      {""},
#line 741 "material_font.gperf"
      {"ev_station", "\uE56D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1398 "material_font.gperf"
      {"note_alt", "\uF040"},
#line 755 "material_font.gperf"
      {"explore_off", "\uE9A8"},
      {""}, {""}, {""}, {""},
#line 418 "material_font.gperf"
      {"cast_for_education", "\uEFEC"},
      {""},
#line 511 "material_font.gperf"
      {"contrast", "\uEB37"},
      {""}, {""}, {""}, {""},
#line 504 "material_font.gperf"
      {"contacts", "\uE0BA"},
      {""}, {""}, {""}, {""}, {""},
#line 197 "material_font.gperf"
      {"app_registration", "\uEF40"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 503 "material_font.gperf"
      {"contactless", "\uEA71"},
      {""}, {""}, {""}, {""},
#line 139 "material_font.gperf"
      {"add_road", "\uEF3B"},
      {""}, {""}, {""},
#line 212 "material_font.gperf"
      {"arrow_circle_right", "\uEAAA"},
#line 1883 "material_font.gperf"
      {"spatial_audio_off", "\uEBE8"},
      {""},
#line 918 "material_font.gperf"
      {"free_cancellation", "\uE748"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1371 "material_font.gperf"
      {"no_cell", "\uF1A4"},
      {""}, {""},
#line 1192 "material_font.gperf"
      {"local_see", "\uE557"},
#line 127 "material_font.gperf"
      {"add_circle", "\uE147"},
      {""}, {""}, {""}, {""},
#line 1231 "material_font.gperf"
      {"male", "\uE58E"},
#line 497 "material_font.gperf"
      {"construction", "\uEA3C"},
      {""},
#line 1165 "material_font.gperf"
      {"local_bar", "\uE540"},
      {""}, {""}, {""}, {""}, {""},
#line 1391 "material_font.gperf"
      {"north_west", "\uF1E2"},
#line 1179 "material_font.gperf"
      {"local_mall", "\uE54C"},
      {""}, {""}, {""},
#line 1384 "material_font.gperf"
      {"no_stroller", "\uF1AF"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 671 "material_font.gperf"
      {"drag_indicator", "\uE945"},
#line 1390 "material_font.gperf"
      {"north_east", "\uF1E1"},
      {""},
#line 851 "material_font.gperf"
      {"flight", "\uE539"},
      {""}, {""},
#line 2029 "material_font.gperf"
      {"text_increase", "\uEAE2"},
      {""}, {""}, {""}, {""},
#line 786 "material_font.gperf"
      {"featured_video", "\uE06E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 864 "material_font.gperf"
      {"fmd_good", "\uF00F"},
      {""}, {""}, {""},
#line 393 "material_font.gperf"
      {"camera", "\uE3AF"},
      {""},
#line 473 "material_font.gperf"
      {"code", "\uE86F"},
      {""}, {""},
#line 1392 "material_font.gperf"
      {"not_accessible", "\uF0FE"},
#line 171 "material_font.gperf"
      {"alarm", "\uE855"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1232 "material_font.gperf"
      {"man", "\uE4EB"},
      {""},
#line 506 "material_font.gperf"
      {"content_cut", "\uE14E"},
#line 399 "material_font.gperf"
      {"camera_rear", "\uE3B2"},
      {""}, {""}, {""}, {""}, {""},
#line 1281 "material_font.gperf"
      {"mms", "\uE618"},
#line 852 "material_font.gperf"
      {"flight_class", "\uE7CB"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 103 "material_font.gperf"
      {"access_alarm", "\uE190"},
#line 394 "material_font.gperf"
      {"camera_alt", "\uE3B0"},
      {""},
#line 654 "material_font.gperf"
      {"done", "\uE876"},
      {""},
#line 1176 "material_font.gperf"
      {"local_hotel", "\uE549"},
      {""}, {""},
#line 1187 "material_font.gperf"
      {"local_police", "\uEF56"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1191 "material_font.gperf"
      {"local_restaurant", "\uE556"},
#line 194 "material_font.gperf"
      {"apartment", "\uEA40"},
#line 1493 "material_font.gperf"
      {"person_pin", "\uE55A"},
      {""},
#line 1172 "material_font.gperf"
      {"local_florist", "\uE545"},
#line 701 "material_font.gperf"
      {"edit_road", "\uEF4D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 479 "material_font.gperf"
      {"color_lens", "\uE3B7"},
#line 1233 "material_font.gperf"
      {"man_2", "\uF8E1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2091 "material_font.gperf"
      {"trip_origin", "\uE57B"},
      {""}, {""}, {""},
#line 655 "material_font.gperf"
      {"done_all", "\uE877"},
      {""}, {""}, {""},
#line 1602 "material_font.gperf"
      {"qr_code", "\uEF6B"},
#line 1175 "material_font.gperf"
      {"local_hospital", "\uE548"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 410 "material_font.gperf"
      {"card_giftcard", "\uE8F6"},
      {""},
#line 395 "material_font.gperf"
      {"camera_enhance", "\uE8FC"},
      {""}, {""}, {""}, {""},
#line 1360 "material_font.gperf"
      {"next_plan", "\uEF5D"},
      {""}, {""}, {""}, {""},
#line 400 "material_font.gperf"
      {"camera_roll", "\uE3B3"},
      {""},
#line 502 "material_font.gperf"
      {"contact_support", "\uE94C"},
      {""},
#line 1235 "material_font.gperf"
      {"man_4", "\uF8E3"},
      {""}, {""},
#line 662 "material_font.gperf"
      {"doorbell", "\uEFFF"},
#line 461 "material_font.gperf"
      {"closed_caption_off", "\uE996"},
      {""},
#line 938 "material_font.gperf"
      {"goat", "\u10FFFD"},
      {""}, {""}, {""},
#line 512 "material_font.gperf"
      {"control_camera", "\uE074"},
#line 944 "material_font.gperf"
      {"gps_not_fixed", "\uE1B4"},
      {""}, {""}, {""},
#line 1234 "material_font.gperf"
      {"man_3", "\uF8E2"},
      {""}, {""},
#line 521 "material_font.gperf"
      {"cottage", "\uE587"},
      {""}, {""}, {""}, {""},
#line 2072 "material_font.gperf"
      {"tornado", "\uE199"},
      {""}, {""}, {""}, {""}, {""},
#line 1395 "material_font.gperf"
      {"not_started", "\uF0D1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1587 "material_font.gperf"
      {"print_disabled", "\uE9CF"},
#line 1236 "material_font.gperf"
      {"manage_accounts", "\uF02E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 656 "material_font.gperf"
      {"done_outline", "\uE92F"},
      {""}, {""}, {""}, {""}, {""},
#line 193 "material_font.gperf"
      {"aod", "\uEFDA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 386 "material_font.gperf"
      {"call_made", "\uE0B2"},
      {""}, {""}, {""}, {""},
#line 1603 "material_font.gperf"
      {"qr_code_2", "\uE00A"},
      {""}, {""},
#line 388 "material_font.gperf"
      {"call_missed", "\uE0B4"},
      {""}, {""},
#line 683 "material_font.gperf"
      {"dynamic_feed", "\uEA14"},
      {""}, {""}, {""}, {""}, {""},
#line 853 "material_font.gperf"
      {"flight_land", "\uE904"},
      {""},
#line 1210 "material_font.gperf"
      {"logout", "\uE9BA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 181 "material_font.gperf"
      {"align_vertical_top", "\uE00C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1048 "material_font.gperf"
      {"imagesearch_roller", "\uE9B4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1906 "material_font.gperf"
      {"sports_martial_arts", "\uEAE9"},
#line 180 "material_font.gperf"
      {"align_vertical_center", "\uE011"},
      {""}, {""},
#line 731 "material_font.gperf"
      {"engineering", "\uEA3D"},
      {""},
#line 1501 "material_font.gperf"
      {"pest_control_rodent", "\uF0FD"},
      {""},
#line 927 "material_font.gperf"
      {"games", "\uE021"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1397 "material_font.gperf"
      {"note_add", "\uE89C"},
      {""}, {""}, {""}, {""}, {""},
#line 1604 "material_font.gperf"
      {"qr_code_scanner", "\uF206"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1055 "material_font.gperf"
      {"info", "\uE88E"},
      {""}, {""}, {""},
#line 772 "material_font.gperf"
      {"face_retouching_natural", "\uEF4E"},
      {""}, {""}, {""}, {""}, {""},
#line 1569 "material_font.gperf"
      {"poll", "\uE801"},
      {""},
#line 954 "material_font.gperf"
      {"grid_goldenratio", "\uF017"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 387 "material_font.gperf"
      {"call_merge", "\uE0B3"},
      {""}, {""},
#line 1344 "material_font.gperf"
      {"near_me", "\uE569"},
      {""},
#line 2024 "material_font.gperf"
      {"terminal", "\uEB8E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1574 "material_font.gperf"
      {"portrait", "\uE416"},
#line 1697 "material_font.gperf"
      {"rotate_right", "\uE41A"},
#line 1050 "material_font.gperf"
      {"import_export", "\uE0C3"},
      {""}, {""},
#line 1056 "material_font.gperf"
      {"info_outline", "\uE88F"},
      {""},
#line 1268 "material_font.gperf"
      {"messenger", "\uE0CA"},
      {""}, {""}, {""}, {""},
#line 147 "material_font.gperf"
      {"adf_scanner", "\uEADA"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1242 "material_font.gperf"
      {"margin", "\uE9BB"},
      {""}, {""},
#line 1647 "material_font.gperf"
      {"remove", "\uE15B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1063 "material_font.gperf"
      {"insert_invitation", "\uE24F"},
      {""}, {""}, {""}, {""}, {""},
#line 765 "material_font.gperf"
      {"extension_off", "\uE4F5"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 520 "material_font.gperf"
      {"corporate_fare", "\uF1D0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1572 "material_font.gperf"
      {"pool", "\uEB48"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1326 "material_font.gperf"
      {"mp", "\uE9C3"},
      {""}, {""}, {""},
#line 182 "material_font.gperf"
      {"all_inbox", "\uE97F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 480 "material_font.gperf"
      {"colorize", "\uE3B8"},
#line 1393 "material_font.gperf"
      {"not_interested", "\uE033"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1228 "material_font.gperf"
      {"mail", "\uE158"},
#line 1417 "material_font.gperf"
      {"online_prediction", "\uF0EB"},
      {""}, {""}, {""},
#line 1162 "material_font.gperf"
      {"local_airport", "\uE53D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 519 "material_font.gperf"
      {"coronavirus", "\uF221"},
#line 477 "material_font.gperf"
      {"collections", "\uE3B6"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2036 "material_font.gperf"
      {"text_snippet", "\uF1C6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 472 "material_font.gperf"
      {"co_present", "\uEAF0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 500 "material_font.gperf"
      {"contact_page", "\uF22E"},
      {""},
#line 2027 "material_font.gperf"
      {"text_fields", "\uE262"},
      {""}, {""}, {""}, {""},
#line 1185 "material_font.gperf"
      {"local_pizza", "\uE552"},
      {""},
#line 926 "material_font.gperf"
      {"gamepad", "\uE30F"},
      {""}, {""}, {""}, {""}, {""},
#line 501 "material_font.gperf"
      {"contact_phone", "\uE0CF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 122 "material_font.gperf"
      {"add_box", "\uE146"},
      {""}, {""},
#line 1631 "material_font.gperf"
      {"read_more", "\uEF6D"},
      {""},
#line 1199 "material_font.gperf"
      {"location_on", "\uE0C8"},
#line 1200 "material_font.gperf"
      {"location_pin", "\uF1DB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 507 "material_font.gperf"
      {"content_paste", "\uE14F"},
      {""}, {""}, {""}, {""},
#line 945 "material_font.gperf"
      {"gps_off", "\uE1B5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1566 "material_font.gperf"
      {"podcasts", "\uF048"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 397 "material_font.gperf"
      {"camera_indoor", "\uEFE9"},
      {""},
#line 1068 "material_font.gperf"
      {"install_desktop", "\uEB71"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1265 "material_font.gperf"
      {"merge", "\uEB98"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 513 "material_font.gperf"
      {"control_point", "\uE3BA"},
#line 1230 "material_font.gperf"
      {"mail_outline", "\uE0E1"},
      {""}, {""}, {""}, {""},
#line 425 "material_font.gperf"
      {"center_focus_strong", "\uE3B4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1267 "material_font.gperf"
      {"message", "\uE0C9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1182 "material_font.gperf"
      {"local_parking", "\uE54F"},
      {""},
#line 1164 "material_font.gperf"
      {"local_attraction", "\uE53F"},
#line 1378 "material_font.gperf"
      {"no_luggage", "\uF23B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1753 "material_font.gperf"
      {"segment", "\uE94B"},
      {""}, {""}, {""}, {""},
#line 1337 "material_font.gperf"
      {"my_location", "\uE55C"},
      {""}, {""}, {""}, {""}, {""},
#line 1208 "material_font.gperf"
      {"login", "\uEA77"},
      {""}, {""}, {""}, {""},
#line 1760 "material_font.gperf"
      {"send_to_mobile", "\uF05C"},
      {""},
#line 855 "material_font.gperf"
      {"flip", "\uE3E8"},
      {""}, {""}, {""},
#line 2067 "material_font.gperf"
      {"toggle_on", "\uE9F6"},
      {""}, {""},
#line 1173 "material_font.gperf"
      {"local_gas_station", "\uE546"},
#line 646 "material_font.gperf"
      {"do_not_step", "\uF19F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 192 "material_font.gperf"
      {"announcement", "\uE85A"},
      {""}, {""},
#line 1654 "material_font.gperf"
      {"remove_road", "\uEBFC"},
      {""},
#line 1650 "material_font.gperf"
      {"remove_done", "\uE9D3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1070 "material_font.gperf"
      {"integration_instructions", "\uEF54"},
      {""}, {""}, {""}, {""},
#line 1575 "material_font.gperf"
      {"post_add", "\uEA20"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1861 "material_font.gperf"
      {"snippet_folder", "\uF1C7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 750 "material_font.gperf"
      {"expand_circle_down", "\uE7CD"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1169 "material_font.gperf"
      {"local_dining", "\uE556"},
      {""}, {""},
#line 1617 "material_font.gperf"
      {"r_mobiledata", "\uF04D"},
      {""},
#line 685 "material_font.gperf"
      {"e_mobiledata", "\uF002"},
#line 2071 "material_font.gperf"
      {"topic", "\uF1C8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 909 "material_font.gperf"
      {"fort", "\uEAAD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 878 "material_font.gperf"
      {"forest", "\uEA99"},
      {""},
#line 1867 "material_font.gperf"
      {"soap", "\uF1B2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1637 "material_font.gperf"
      {"recommend", "\uE9D2"},
#line 699 "material_font.gperf"
      {"edit_notifications", "\uE525"},
      {""}, {""}, {""},
#line 752 "material_font.gperf"
      {"expand_more", "\uE5CF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2034 "material_font.gperf"
      {"text_rotation_down", "\uE93E"},
      {""}, {""}, {""}, {""},
#line 1402 "material_font.gperf"
      {"notifications", "\uE7F4"},
      {""}, {""}, {""}, {""},
#line 1218 "material_font.gperf"
      {"loop", "\uE028"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1648 "material_font.gperf"
      {"remove_circle", "\uE15C"},
      {""},
#line 1625 "material_font.gperf"
      {"ramen_dining", "\uEA64"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1239 "material_font.gperf"
      {"map", "\uE55B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 63 "material_font.gperf"
      {"23mp", "\uE961"},
      {""}, {""}, {""},
#line 60 "material_font.gperf"
      {"20mp", "\uE95E"},
      {""}, {""}, {""},
#line 64 "material_font.gperf"
      {"24mp", "\uE962"},
#line 1166 "material_font.gperf"
      {"local_cafe", "\uE541"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 56 "material_font.gperf"
      {"19mp", "\uE95B"},
#line 55 "material_font.gperf"
      {"18mp", "\uE95A"},
#line 53 "material_font.gperf"
      {"17mp", "\uE959"},
      {""}, {""},
#line 51 "material_font.gperf"
      {"15mp", "\uE957"},
#line 49 "material_font.gperf"
      {"13mp", "\uE955"},
#line 1385 "material_font.gperf"
      {"no_transfer", "\uF1D5"},
      {""},
#line 52 "material_font.gperf"
      {"16mp", "\uE958"},
#line 45 "material_font.gperf"
      {"10mp", "\uE952"},
      {""}, {""}, {""},
#line 50 "material_font.gperf"
      {"14mp", "\uE956"},
      {""}, {""}, {""}, {""}, {""},
#line 1474 "material_font.gperf"
      {"perm_contact_cal", "\uE8A3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 62 "material_font.gperf"
      {"22mp", "\uE960"},
      {""}, {""}, {""}, {""}, {""},
#line 866 "material_font.gperf"
      {"folder", "\uE2C7"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1126 "material_font.gperf"
      {"laptop_mac", "\uE320"},
      {""},
#line 61 "material_font.gperf"
      {"21mp", "\uE95F"},
#line 396 "material_font.gperf"
      {"camera_front", "\uE3B1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 48 "material_font.gperf"
      {"12mp", "\uE954"},
      {""}, {""}, {""}, {""},
#line 508 "material_font.gperf"
      {"content_paste_go", "\uEA8E"},
      {""}, {""}, {""},
#line 263 "material_font.gperf"
      {"av_timer", "\uE01B"},
      {""}, {""}, {""}, {""}, {""},
#line 46 "material_font.gperf"
      {"11mp", "\uE953"},
      {""}, {""}, {""}, {""}, {""},
#line 402 "material_font.gperf"
      {"campaign", "\uEF49"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 420 "material_font.gperf"
      {"catching_pokemon", "\uE508"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1069 "material_font.gperf"
      {"install_mobile", "\uEB72"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 784 "material_font.gperf"
      {"fax", "\uEAD8"},
      {""}, {""}, {""}, {""},
#line 924 "material_font.gperf"
      {"g_mobiledata", "\uF010"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1868 "material_font.gperf"
      {"social_distance", "\uE1CB"},
      {""}, {""},
#line 1406 "material_font.gperf"
      {"notifications_on", "\uE7F7"},
      {""}, {""}, {""}, {""},
#line 1491 "material_font.gperf"
      {"person_off", "\uE510"},
      {""}, {""},
#line 661 "material_font.gperf"
      {"door_sliding", "\uEFFE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1404 "material_font.gperf"
      {"notifications_none", "\uE7F5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1255 "material_font.gperf"
      {"mediation", "\uEFA7"},
      {""}, {""}, {""}, {""},
#line 191 "material_font.gperf"
      {"animation", "\uE71C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1671 "material_font.gperf"
      {"report_off", "\uE170"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1062 "material_font.gperf"
      {"insert_emoticon", "\uE24E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1377 "material_font.gperf"
      {"no_food", "\uF1A7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 950 "material_font.gperf"
      {"graphic_eq", "\uE1B8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 955 "material_font.gperf"
      {"grid_off", "\uE3EB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 868 "material_font.gperf"
      {"folder_delete", "\uEB34"},
      {""},
#line 2037 "material_font.gperf"
      {"textsms", "\uE0D8"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1690 "material_font.gperf"
      {"roofing", "\uF201"},
      {""}, {""},
#line 660 "material_font.gperf"
      {"door_front", "\uEFFD"},
      {""}, {""}, {""}, {""}, {""},
#line 153 "material_font.gperf"
      {"air", "\uEFD8"},
      {""}, {""}, {""},
#line 2063 "material_font.gperf"
      {"title", "\uE264"},
#line 859 "material_font.gperf"
      {"flip_to_front", "\uE883"},
#line 1156 "material_font.gperf"
      {"list", "\uE896"},
#line 1448 "material_font.gperf"
      {"panorama_vertical", "\uE40E"},
      {""}, {""},
#line 789 "material_font.gperf"
      {"female", "\uE590"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2108 "material_font.gperf"
      {"tv_off", "\uE647"},
      {""}, {""}, {""},
#line 377 "material_font.gperf"
      {"cake", "\uE7E9"},
      {""}, {""}, {""},
#line 1449 "material_font.gperf"
      {"panorama_vertical_select", "\uEF61"},
      {""}, {""}, {""}, {""},
#line 1053 "material_font.gperf"
      {"incomplete_circle", "\uE79B"},
      {""}, {""}, {""}, {""},
#line 450 "material_font.gperf"
      {"circle", "\uEF4A"},
      {""},
#line 173 "material_font.gperf"
      {"alarm_off", "\uE857"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1157 "material_font.gperf"
      {"list_alt", "\uE0EE"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2019 "material_font.gperf"
      {"task_alt", "\uE2E6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1891 "material_font.gperf"
      {"spellcheck", "\uE8CE"},
      {""}, {""}, {""},
#line 1549 "material_font.gperf"
      {"plagiarism", "\uEA5A"},
      {""}, {""}, {""}, {""}, {""},
#line 129 "material_font.gperf"
      {"add_comment", "\uE266"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1796 "material_font.gperf"
      {"share", "\uE80D"},
      {""},
#line 433 "material_font.gperf"
      {"chat", "\uE0B7"},
      {""}, {""}, {""},
#line 163 "material_font.gperf"
      {"airlines", "\uE7CA"},
      {""},
#line 1893 "material_font.gperf"
      {"spoke", "\uE9A7"},
#line 1799 "material_font.gperf"
      {"shelves", "\uF86E"},
      {""}, {""}, {""}, {""}, {""},
#line 1400 "material_font.gperf"
      {"notification_add", "\uE399"},
#line 1151 "material_font.gperf"
      {"linear_scale", "\uE260"},
      {""}, {""},
#line 429 "material_font.gperf"
      {"chalet", "\uE585"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2040 "material_font.gperf"
      {"theaters", "\uE8DA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 441 "material_font.gperf"
      {"checklist", "\uE6B1"},
      {""}, {""}, {""}, {""}, {""},
#line 106 "material_font.gperf"
      {"access_time_filled", "\uEFD6"},
      {""},
#line 1194 "material_font.gperf"
      {"local_taxi", "\uE559"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 442 "material_font.gperf"
      {"checklist_rtl", "\uE6B3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1670 "material_font.gperf"
      {"report_gmailerrorred", "\uF052"},
      {""}, {""},
#line 1198 "material_font.gperf"
      {"location_off", "\uE0C7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 162 "material_font.gperf"
      {"airline_stops", "\uE7D0"},
      {""},
#line 1811 "material_font.gperf"
      {"shortcut", "\uF060"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1626 "material_font.gperf"
      {"ramp_left", "\uEB9C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2041 "material_font.gperf"
      {"thermostat", "\uF076"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1414 "material_font.gperf"
      {"oil_barrel", "\uEC15"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 496 "material_font.gperf"
      {"connecting_airports", "\uE7C9"},
      {""}, {""},
#line 207 "material_font.gperf"
      {"arrow_back", "\uE5C4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 526 "material_font.gperf"
      {"credit_card_off", "\uE4F4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1494 "material_font.gperf"
      {"person_pin_circle", "\uE56A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1856 "material_font.gperf"
      {"smoke_free", "\uEB4A"},
      {""}, {""}, {""},
#line 937 "material_font.gperf"
      {"gite", "\uE58B"},
      {""},
#line 936 "material_font.gperf"
      {"girl", "\uEB68"},
#line 872 "material_font.gperf"
      {"folder_special", "\uE617"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1258 "material_font.gperf"
      {"medication", "\uF033"},
#line 1257 "material_font.gperf"
      {"medical_services", "\uF109"},
      {""}, {""}, {""}, {""}, {""},
#line 1306 "material_font.gperf"
      {"more", "\uE619"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 625 "material_font.gperf"
      {"dirty_lens", "\uEF4B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1857 "material_font.gperf"
      {"smoking_rooms", "\uEB4B"},
      {""}, {""}, {""},
#line 606 "material_font.gperf"
      {"directions", "\uE52E"},
      {""},
#line 1797 "material_font.gperf"
      {"share_arrival_time", "\uE524"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 725 "material_font.gperf"
      {"emoji_nature", "\uEA1C"},
      {""}, {""}, {""}, {""},
#line 723 "material_font.gperf"
      {"emoji_flags", "\uEA1A"},
#line 870 "material_font.gperf"
      {"folder_open", "\uE2C8"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 773 "material_font.gperf"
      {"face_retouching_off", "\uF007"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 158 "material_font.gperf"
      {"airline_seat_legroom_normal", "\uE634"},
#line 629 "material_font.gperf"
      {"discord", "\uEA6C"},
      {""},
#line 612 "material_font.gperf"
      {"directions_car", "\uE531"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 157 "material_font.gperf"
      {"airline_seat_legroom_extra", "\uE633"},
      {""},
#line 562 "material_font.gperf"
      {"data_object", "\uEAD3"},
      {""}, {""}, {""}, {""}, {""},
#line 601 "material_font.gperf"
      {"dialpad", "\uE0BC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 700 "material_font.gperf"
      {"edit_off", "\uE950"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 727 "material_font.gperf"
      {"emoji_people", "\uEA1D"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1839 "material_font.gperf"
      {"signpost", "\uEB91"},
      {""}, {""},
#line 2079 "material_font.gperf"
      {"tram", "\uE571"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1316 "material_font.gperf"
      {"motorcycle", "\uE91B"},
      {""}, {""}, {""}, {""},
#line 1379 "material_font.gperf"
      {"no_meals", "\uF1D6"},
#line 1168 "material_font.gperf"
      {"local_convenience_store", "\uE543"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 499 "material_font.gperf"
      {"contact_mail", "\uE0D0"},
#line 1820 "material_font.gperf"
      {"signal_cellular_4_bar", "\uE1C8"},
#line 1798 "material_font.gperf"
      {"share_location", "\uF05F"},
      {""}, {""}, {""},
#line 1072 "material_font.gperf"
      {"interpreter_mode", "\uE83B"},
      {""}, {""}, {""}, {""},
#line 608 "material_font.gperf"
      {"directions_boat", "\uE532"},
      {""}, {""},
#line 1286 "material_font.gperf"
      {"mode", "\uF097"},
#line 1490 "material_font.gperf"
      {"person_add_disabled", "\uE9CB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1819 "material_font.gperf"
      {"signal_cellular_0_bar", "\uF0A8"},
      {""}, {""},
#line 475 "material_font.gperf"
      {"coffee", "\uEFEF"},
      {""}, {""}, {""}, {""},
#line 684 "material_font.gperf"
      {"dynamic_form", "\uF1BF"},
      {""},
#line 2062 "material_font.gperf"
      {"tire_repair", "\uEBC8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1541 "material_font.gperf"
      {"pin", "\uF045"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 427 "material_font.gperf"
      {"chair", "\uEFED"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1148 "material_font.gperf"
      {"line_axis", "\uEA9A"},
#line 1828 "material_font.gperf"
      {"signal_cellular_null", "\uE1CF"},
      {""},
#line 2275 "material_font.gperf"
      {"zoom_out", "\uE900"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 159 "material_font.gperf"
      {"airline_seat_legroom_reduced", "\uE635"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1269 "material_font.gperf"
      {"messenger_outline", "\uE0CB"},
      {""}, {""}, {""}, {""},
#line 1827 "material_font.gperf"
      {"signal_cellular_nodata", "\uF062"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1907 "material_font.gperf"
      {"sports_mma", "\uEA2C"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 650 "material_font.gperf"
      {"domain", "\uE7EE"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1533 "material_font.gperf"
      {"piano", "\uE521"},
      {""}, {""},
#line 1252 "material_font.gperf"
      {"maximize", "\uE930"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1181 "material_font.gperf"
      {"local_offer", "\uE54E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 170 "material_font.gperf"
      {"airport_shuttle", "\uEB3C"},
      {""}, {""}, {""}, {""}, {""},
#line 1061 "material_font.gperf"
      {"insert_drive_file", "\uE24D"},
      {""}, {""},
#line 439 "material_font.gperf"
      {"check_circle", "\uE86C"},
#line 428 "material_font.gperf"
      {"chair_alt", "\uEFEE"},
#line 430 "material_font.gperf"
      {"change_circle", "\uE2E7"},
      {""},
#line 446 "material_font.gperf"
      {"child_care", "\uEB41"},
      {""}, {""}, {""}, {""}, {""},
#line 1803 "material_font.gperf"
      {"shop_2", "\uE19E"},
      {""}, {""}, {""}, {""},
#line 1504 "material_font.gperf"
      {"phone", "\uE0CD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1846 "material_font.gperf"
      {"skip_next", "\uE044"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1475 "material_font.gperf"
      {"perm_contact_calendar", "\uE8A3"},
#line 1824 "material_font.gperf"
      {"signal_cellular_connected_no_internet_0_bar", "\uF0AC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1567 "material_font.gperf"
      {"point_of_sale", "\uF17E"},
      {""}, {""}, {""}, {""},
#line 1522 "material_font.gperf"
      {"photo", "\uE410"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1133 "material_font.gperf"
      {"leak_add", "\uE3F8"},
      {""},
#line 1422 "material_font.gperf"
      {"open_in_new_off", "\uE4F6"},
      {""}, {""}, {""}, {""}, {""},
#line 509 "material_font.gperf"
      {"content_paste_off", "\uE4F8"},
      {""}, {""}, {""},
#line 1292 "material_font.gperf"
      {"mode_of_travel", "\uE7CE"},
      {""}, {""}, {""}, {""}, {""},
#line 1818 "material_font.gperf"
      {"sign_language", "\uEBE5"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 873 "material_font.gperf"
      {"folder_zip", "\uEB2C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1825 "material_font.gperf"
      {"signal_cellular_connected_no_internet_4_bar", "\uE1CD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1303 "material_font.gperf"
      {"mood", "\uE7F2"},
#line 1481 "material_font.gperf"
      {"perm_phone_msg", "\uE8A8"},
      {""},
#line 2021 "material_font.gperf"
      {"telegram", "\uEA6B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1387 "material_font.gperf"
      {"noise_control_off", "\uEBF3"},
      {""},
#line 404 "material_font.gperf"
      {"cancel_presentation", "\uE0E9"},
      {""}, {""}, {""}, {""},
#line 474 "material_font.gperf"
      {"code_off", "\uE4F3"},
      {""}, {""}, {""}, {""}, {""},
#line 486 "material_font.gperf"
      {"compare", "\uE3B9"},
      {""}, {""},
#line 489 "material_font.gperf"
      {"compost", "\uE761"},
#line 1543 "material_font.gperf"
      {"pin_end", "\uE767"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 491 "material_font.gperf"
      {"computer", "\uE30A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 490 "material_font.gperf"
      {"compress", "\uE94D"},
      {""},
#line 1804 "material_font.gperf"
      {"shop_two", "\uE8CA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2061 "material_font.gperf"
      {"tips_and_updates", "\uE79A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1260 "material_font.gperf"
      {"meeting_room", "\uEB4F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 196 "material_font.gperf"
      {"app_blocking", "\uEF3F"},
#line 621 "material_font.gperf"
      {"directions_train", "\uE534"},
      {""},
#line 1800 "material_font.gperf"
      {"shield", "\uE9E0"},
      {""}, {""}, {""}, {""}, {""},
#line 1299 "material_font.gperf"
      {"monitor", "\uEF5B"},
      {""},
#line 460 "material_font.gperf"
      {"closed_caption_disabled", "\uF1DC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1844 "material_font.gperf"
      {"sip", "\uF069"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1642 "material_font.gperf"
      {"redeem", "\uE8B1"},
#line 1043 "material_font.gperf"
      {"icecream", "\uEA69"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2028 "material_font.gperf"
      {"text_format", "\uE165"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2273 "material_font.gperf"
      {"zoom_in", "\uE8FF"},
#line 805 "material_font.gperf"
      {"filter", "\uE3D3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 846 "material_font.gperf"
      {"flash_off", "\uE3E6"},
#line 1305 "material_font.gperf"
      {"moped", "\uEB28"},
      {""},
#line 1884 "material_font.gperf"
      {"spatial_tracking", "\uEBEA"},
      {""},
#line 814 "material_font.gperf"
      {"filter_9", "\uE3D9"},
#line 651 "material_font.gperf"
      {"domain_add", "\uEB62"},
#line 1792 "material_font.gperf"
      {"settings_system_daydream", "\uE1C3"},
      {""}, {""}, {""},
#line 812 "material_font.gperf"
      {"filter_7", "\uE3D7"},
      {""},
#line 810 "material_font.gperf"
      {"filter_5", "\uE3D5"},
      {""}, {""}, {""}, {""}, {""},
#line 813 "material_font.gperf"
      {"filter_8", "\uE3D8"},
      {""},
#line 807 "material_font.gperf"
      {"filter_2", "\uE3D1"},
      {""}, {""}, {""}, {""},
#line 1288 "material_font.gperf"
      {"mode_edit", "\uE254"},
      {""}, {""},
#line 811 "material_font.gperf"
      {"filter_6", "\uE3D6"},
#line 836 "material_font.gperf"
      {"fireplace", "\uEA43"},
      {""}, {""},
#line 816 "material_font.gperf"
      {"filter_alt", "\uEF4F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 609 "material_font.gperf"
      {"directions_boat_filled", "\uEFF5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 775 "material_font.gperf"
      {"fact_check", "\uF0C5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1755 "material_font.gperf"
      {"self_improvement", "\uEA78"},
      {""}, {""}, {""}, {""}, {""},
#line 806 "material_font.gperf"
      {"filter_1", "\uE3D0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1513 "material_font.gperf"
      {"phone_locked", "\uE61E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1405 "material_font.gperf"
      {"notifications_off", "\uE7F6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 815 "material_font.gperf"
      {"filter_9_plus", "\uE3DA"},
      {""}, {""}, {""},
#line 809 "material_font.gperf"
      {"filter_4", "\uE3D4"},
      {""}, {""}, {""}, {""}, {""},
#line 1380 "material_font.gperf"
      {"no_meals_ouline", "\uF229"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1802 "material_font.gperf"
      {"shop", "\uE8C9"},
      {""}, {""}, {""},
#line 838 "material_font.gperf"
      {"fit_screen", "\uEA10"},
      {""}, {""}, {""}, {""},
#line 1652 "material_font.gperf"
      {"remove_moderator", "\uE9D4"},
      {""}, {""}, {""},
#line 808 "material_font.gperf"
      {"filter_3", "\uE3D2"},
      {""}, {""}, {""}, {""},
#line 826 "material_font.gperf"
      {"filter_none", "\uE3E0"},
#line 2276 "material_font.gperf"
      {"zoom_out_map", "\uE56B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 840 "material_font.gperf"
      {"fitness_center", "\uEB43"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1295 "material_font.gperf"
      {"monetization_on", "\uE263"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 801 "material_font.gperf"
      {"file_open", "\uEAF3"},
      {""},
#line 1795 "material_font.gperf"
      {"shape_line", "\uF8D3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 600 "material_font.gperf"
      {"dialer_sip", "\uE0BB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1593 "material_font.gperf"
      {"propane_tank", "\uEC13"},
      {""}, {""}, {""}, {""}, {""},
#line 820 "material_font.gperf"
      {"filter_drama", "\uE3DD"},
      {""}, {""},
#line 628 "material_font.gperf"
      {"disc_full", "\uE610"},
      {""}, {""}, {""},
#line 1527 "material_font.gperf"
      {"photo_filter", "\uE43B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1810 "material_font.gperf"
      {"short_text", "\uE261"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1509 "material_font.gperf"
      {"phone_enabled", "\uE9CD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2066 "material_font.gperf"
      {"toggle_off", "\uE9F5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 543 "material_font.gperf"
      {"css", "\uEB93"},
      {""}, {""}, {""}, {""},
#line 514 "material_font.gperf"
      {"control_point_duplicate", "\uE3BB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1905 "material_font.gperf"
      {"sports_kabaddi", "\uEA34"},
#line 1914 "material_font.gperf"
      {"square", "\uEB36"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 604 "material_font.gperf"
      {"dining", "\uEFF4"},
      {""},
#line 1294 "material_font.gperf"
      {"model_training", "\uF0CF"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 857 "material_font.gperf"
      {"flip_camera_ios", "\uEA38"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1339 "material_font.gperf"
      {"nature", "\uE406"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 887 "material_font.gperf"
      {"format_clear", "\uE239"},
      {""}, {""},
#line 1042 "material_font.gperf"
      {"ice_skating", "\uE50B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 931 "material_font.gperf"
      {"generating_tokens", "\uE749"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 228 "material_font.gperf"
      {"assessment", "\uE85C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 837 "material_font.gperf"
      {"first_page", "\uE5DC"},
      {""}, {""},
#line 184 "material_font.gperf"
      {"all_out", "\uE90B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1505 "material_font.gperf"
      {"phone_android", "\uE324"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 737 "material_font.gperf"
      {"escalator", "\uF1A1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1514 "material_font.gperf"
      {"phone_missed", "\uE61F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1064 "material_font.gperf"
      {"insert_link", "\uE250"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 823 "material_font.gperf"
      {"filter_list", "\uE152"},
      {""}, {""}, {""},
#line 2123 "material_font.gperf"
      {"update", "\uE923"},
      {""}, {""},
#line 736 "material_font.gperf"
      {"error_outline", "\uE001"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1510 "material_font.gperf"
      {"phone_forwarded", "\uE61C"},
      {""}, {""},
#line 183 "material_font.gperf"
      {"all_inclusive", "\uEB3D"},
      {""}, {""}, {""},
#line 1532 "material_font.gperf"
      {"php", "\uEB8F"},
      {""},
#line 1991 "material_font.gperf"
      {"synagogue", "\uEAB0"},
#line 802 "material_font.gperf"
      {"file_present", "\uEA0E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2082 "material_font.gperf"
      {"transform", "\uE428"},
#line 1482 "material_font.gperf"
      {"perm_scan_wifi", "\uE8A9"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1851 "material_font.gperf"
      {"smart_button", "\uF1C1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 824 "material_font.gperf"
      {"filter_list_alt", "\uE94E"},
      {""}, {""}, {""},
#line 1808 "material_font.gperf"
      {"shopping_cart", "\uE8CC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1465 "material_font.gperf"
      {"pedal_bike", "\uEB29"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1542 "material_font.gperf"
      {"pin_drop", "\uE55E"},
      {""}, {""}, {""},
#line 605 "material_font.gperf"
      {"dinner_dining", "\uEA57"},
      {""}, {""}, {""}, {""}, {""},
#line 462 "material_font.gperf"
      {"cloud", "\uE2BD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1536 "material_font.gperf"
      {"picture_in_picture", "\uE8AA"},
      {""}, {""}, {""}, {""}, {""},
#line 458 "material_font.gperf"
      {"close_fullscreen", "\uF1CF"},
#line 432 "material_font.gperf"
      {"charging_station", "\uF19D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1537 "material_font.gperf"
      {"picture_in_picture_alt", "\uE911"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1685 "material_font.gperf"
      {"rocket", "\uEBA5"},
      {""},
#line 1735 "material_font.gperf"
      {"screen_lock_rotation", "\uE1C0"},
      {""}, {""}, {""}, {""},
#line 405 "material_font.gperf"
      {"cancel_schedule_send", "\uEA39"},
      {""}, {""}, {""}, {""}, {""},
#line 464 "material_font.gperf"
      {"cloud_done", "\uE2BF"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2068 "material_font.gperf"
      {"token", "\uEA25"},
      {""}, {""}, {""}, {""},
#line 631 "material_font.gperf"
      {"display_settings", "\uEB97"},
      {""}, {""}, {""}, {""}, {""},
#line 1677 "material_font.gperf"
      {"restaurant", "\uE56C"},
#line 1508 "material_font.gperf"
      {"phone_disabled", "\uE9CC"},
      {""}, {""}, {""}, {""}, {""},
#line 1207 "material_font.gperf"
      {"lock_reset", "\uEADE"},
      {""}, {""}, {""}, {""},
#line 963 "material_font.gperf"
      {"groups", "\uF233"},
      {""}, {""}, {""},
#line 652 "material_font.gperf"
      {"domain_disabled", "\uE0EF"},
      {""}, {""},
#line 856 "material_font.gperf"
      {"flip_camera_android", "\uEA37"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 964 "material_font.gperf"
      {"groups_2", "\uF8DF"},
      {""}, {""}, {""},
#line 1211 "material_font.gperf"
      {"looks", "\uE3FC"},
      {""}, {""}, {""},
#line 854 "material_font.gperf"
      {"flight_takeoff", "\uE905"},
      {""}, {""}, {""}, {""},
#line 98 "material_font.gperf"
      {"9k", "\uE977"},
      {""},
#line 830 "material_font.gperf"
      {"find_replace", "\uE881"},
#line 92 "material_font.gperf"
      {"7k", "\uE971"},
#line 83 "material_font.gperf"
      {"5k", "\uE96B"},
      {""}, {""},
#line 95 "material_font.gperf"
      {"8k", "\uE974"},
#line 65 "material_font.gperf"
      {"2k", "\uE963"},
      {""},
#line 516 "material_font.gperf"
      {"cookie", "\uEAAC"},
      {""},
#line 89 "material_font.gperf"
      {"6k", "\uE96E"},
#line 1227 "material_font.gperf"
      {"macro_off", "\uF8D2"},
      {""}, {""},
#line 120 "material_font.gperf"
      {"add_alarm", "\uE193"},
      {""}, {""},
#line 1704 "material_font.gperf"
      {"rss_feed", "\uE0E5"},
      {""}, {""}, {""},
#line 2126 "material_font.gperf"
      {"upload", "\uF09B"},
      {""}, {""}, {""},
#line 2055 "material_font.gperf"
      {"timer", "\uE425"},
      {""}, {""}, {""},
#line 1270 "material_font.gperf"
      {"mic", "\uE029"},
      {""},
#line 57 "material_font.gperf"
      {"1k", "\uE95C"},
      {""}, {""}, {""}, {""},
#line 1457 "material_font.gperf"
      {"pause", "\uE034"},
      {""}, {""}, {""}, {""}, {""},
#line 1205 "material_font.gperf"
      {"lock_outline", "\uE899"},
#line 1251 "material_font.gperf"
      {"masks", "\uF218"},
      {""},
#line 2125 "material_font.gperf"
      {"upgrade", "\uF0FB"},
      {""}, {""},
#line 1045 "material_font.gperf"
      {"image_aspect_ratio", "\uE3F5"},
      {""}, {""}, {""}, {""},
#line 79 "material_font.gperf"
      {"4k", "\uE072"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 734 "material_font.gperf"
      {"equalizer", "\uE01D"},
      {""},
#line 73 "material_font.gperf"
      {"3k", "\uE966"},
      {""}, {""},
#line 869 "material_font.gperf"
      {"folder_off", "\uEB83"},
      {""},
#line 1214 "material_font.gperf"
      {"looks_5", "\uE3FE"},
#line 54 "material_font.gperf"
      {"18_up_rating", "\uF8FD"},
      {""}, {""}, {""},
#line 903 "material_font.gperf"
      {"format_size", "\uE245"},
      {""}, {""},
#line 1215 "material_font.gperf"
      {"looks_6", "\uE3FF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2053 "material_font.gperf"
      {"timelapse", "\uE422"},
      {""}, {""},
#line 831 "material_font.gperf"
      {"fingerprint", "\uE90D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1781 "material_font.gperf"
      {"settings_ethernet", "\uE8BE"},
#line 102 "material_font.gperf"
      {"ac_unit", "\uEB3B"},
#line 237 "material_font.gperf"
      {"assistant", "\uE39F"},
#line 900 "material_font.gperf"
      {"format_paint", "\uE243"},
#line 893 "material_font.gperf"
      {"format_italic", "\uE23F"},
#line 1806 "material_font.gperf"
      {"shopping_bag", "\uF1CC"},
      {""}, {""},
#line 965 "material_font.gperf"
      {"groups_3", "\uF8E0"},
      {""}, {""},
#line 566 "material_font.gperf"
      {"data_usage", "\uE1AF"},
      {""}, {""}, {""}, {""}, {""},
#line 1204 "material_font.gperf"
      {"lock_open", "\uE898"},
      {""}, {""},
#line 2056 "material_font.gperf"
      {"timer_10", "\uE423"},
      {""}, {""},
#line 1213 "material_font.gperf"
      {"looks_4", "\uE3FD"},
      {""}, {""}, {""}, {""}, {""},
#line 556 "material_font.gperf"
      {"dangerous", "\uE99A"},
      {""}, {""}, {""},
#line 1340 "material_font.gperf"
      {"nature_people", "\uE407"},
      {""}, {""}, {""},
#line 1216 "material_font.gperf"
      {"looks_one", "\uE400"},
#line 1212 "material_font.gperf"
      {"looks_3", "\uE3FB"},
      {""}, {""}, {""}, {""}, {""},
#line 221 "material_font.gperf"
      {"arrow_outward", "\uF8CE"},
      {""},
#line 229 "material_font.gperf"
      {"assignment", "\uE85D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2052 "material_font.gperf"
      {"time_to_leave", "\uE62C"},
      {""}, {""}, {""}, {""},
#line 1672 "material_font.gperf"
      {"report_problem", "\uE8B2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1080 "material_font.gperf"
      {"iso", "\uE3F6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2058 "material_font.gperf"
      {"timer_3", "\uE424"},
      {""}, {""}, {""},
#line 541 "material_font.gperf"
      {"crop_square", "\uE3C6"},
#line 227 "material_font.gperf"
      {"aspect_ratio", "\uE85B"},
      {""},
#line 1565 "material_font.gperf"
      {"plus_one", "\uE800"},
#line 756 "material_font.gperf"
      {"exposure", "\uE3CA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1829 "material_font.gperf"
      {"signal_cellular_off", "\uE1D0"},
      {""}, {""}, {""},
#line 232 "material_font.gperf"
      {"assignment_late", "\uE85F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1273 "material_font.gperf"
      {"mic_none", "\uE02A"},
      {""},
#line 1480 "material_font.gperf"
      {"perm_media", "\uE8A7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1310 "material_font.gperf"
      {"mosque", "\uEAB2"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 575 "material_font.gperf"
      {"delete_outline", "\uE92E"},
      {""}, {""}, {""}, {""}, {""},
#line 2059 "material_font.gperf"
      {"timer_3_select", "\uF07B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2057 "material_font.gperf"
      {"timer_10_select", "\uF07A"},
#line 463 "material_font.gperf"
      {"cloud_circle", "\uE2BE"},
      {""},
#line 1916 "material_font.gperf"
      {"ssid_chart", "\uEB66"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 821 "material_font.gperf"
      {"filter_frames", "\uE3DE"},
      {""}, {""}, {""}, {""}, {""},
#line 1692 "material_font.gperf"
      {"room_preferences", "\uF1B8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1782 "material_font.gperf"
      {"settings_input_antenna", "\uE8BF"},
#line 1632 "material_font.gperf"
      {"real_estate_agent", "\uE73A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1279 "material_font.gperf"
      {"miscellaneous_services", "\uF10C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 179 "material_font.gperf"
      {"align_vertical_bottom", "\uE015"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1249 "material_font.gperf"
      {"markunread", "\uE159"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1245 "material_font.gperf"
      {"mark_chat_unread", "\uF189"},
      {""},
#line 211 "material_font.gperf"
      {"arrow_circle_left", "\uEAA7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 448 "material_font.gperf"
      {"chrome_reader_mode", "\uE86D"},
      {""},
#line 484 "material_font.gperf"
      {"commit", "\uEAF5"},
      {""}, {""},
#line 485 "material_font.gperf"
      {"commute", "\uE940"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 760 "material_font.gperf"
      {"exposure_neg_2", "\uE3CC"},
#line 2115 "material_font.gperf"
      {"undo", "\uE166"},
      {""}, {""}, {""},
#line 1308 "material_font.gperf"
      {"more_time", "\uEA5D"},
#line 1503 "material_font.gperf"
      {"phishing", "\uEAD7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 762 "material_font.gperf"
      {"exposure_plus_2", "\uE3CE"},
      {""}, {""}, {""}, {""}, {""},
#line 481 "material_font.gperf"
      {"comment", "\uE0B9"},
      {""},
#line 958 "material_font.gperf"
      {"group", "\uE7EF"},
      {""}, {""}, {""}, {""},
#line 161 "material_font.gperf"
      {"airline_seat_recline_normal", "\uE637"},
#line 240 "material_font.gperf"
      {"assistant_photo", "\uE3A0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 160 "material_font.gperf"
      {"airline_seat_recline_extra", "\uE636"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1691 "material_font.gperf"
      {"room", "\uE8B4"},
#line 759 "material_font.gperf"
      {"exposure_neg_1", "\uE3CB"},
#line 1841 "material_font.gperf"
      {"sim_card_alert", "\uE624"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 557 "material_font.gperf"
      {"dark_mode", "\uE51C"},
      {""}, {""},
#line 761 "material_font.gperf"
      {"exposure_plus_1", "\uE3CD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 469 "material_font.gperf"
      {"cloud_upload", "\uE2C3"},
      {""}, {""},
#line 763 "material_font.gperf"
      {"exposure_zero", "\uE3CF"},
#line 602 "material_font.gperf"
      {"diamond", "\uEAD5"},
      {""},
#line 437 "material_font.gperf"
      {"check_box", "\uE834"},
#line 1206 "material_font.gperf"
      {"lock_person", "\uF8F3"},
      {""}, {""},
#line 1383 "material_font.gperf"
      {"no_sim", "\uE0CC"},
      {""}, {""}, {""}, {""},
#line 1134 "material_font.gperf"
      {"leak_remove", "\uE3F9"},
      {""}, {""}, {""},
#line 1163 "material_font.gperf"
      {"local_atm", "\uE53E"},
      {""}, {""}, {""},
#line 603 "material_font.gperf"
      {"difference", "\uEB7D"},
      {""}, {""}, {""}, {""},
#line 1170 "material_font.gperf"
      {"local_drink", "\uE544"},
      {""},
#line 934 "material_font.gperf"
      {"gif", "\uE908"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1188 "material_font.gperf"
      {"local_post_office", "\uE554"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1840 "material_font.gperf"
      {"sim_card", "\uE32B"},
      {""}, {""},
#line 1244 "material_font.gperf"
      {"mark_chat_read", "\uF18B"},
      {""},
#line 2054 "material_font.gperf"
      {"timeline", "\uE922"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 829 "material_font.gperf"
      {"find_in_page", "\uE880"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 721 "material_font.gperf"
      {"emoji_emotions", "\uEA22"},
      {""}, {""}, {""},
#line 1775 "material_font.gperf"
      {"settings_applications", "\uE8B9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1492 "material_font.gperf"
      {"person_outline", "\uE7FF"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 819 "material_font.gperf"
      {"filter_center_focus", "\uE3DC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1496 "material_font.gperf"
      {"person_remove_alt_1", "\uEF67"},
      {""},
#line 231 "material_font.gperf"
      {"assignment_ind", "\uE85E"},
      {""},
#line 1622 "material_font.gperf"
      {"radio_button_on", "\uE837"},
#line 1785 "material_font.gperf"
      {"settings_input_hdmi", "\uE8C2"},
      {""}, {""}, {""},
#line 476 "material_font.gperf"
      {"coffee_maker", "\uEFF0"},
      {""}, {""}, {""}, {""}, {""},
#line 959 "material_font.gperf"
      {"group_add", "\uE7F0"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1458 "material_font.gperf"
      {"pause_circle", "\uE1A2"},
      {""}, {""}, {""}, {""},
#line 1689 "material_font.gperf"
      {"roller_skating", "\uEBCD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 542 "material_font.gperf"
      {"cruelty_free", "\uE799"},
      {""}, {""}, {""}, {""}, {""},
#line 1784 "material_font.gperf"
      {"settings_input_composite", "\uE8C1"},
#line 891 "material_font.gperf"
      {"format_indent_decrease", "\uE23D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1547 "material_font.gperf"
      {"pix", "\uEAA3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1915 "material_font.gperf"
      {"square_foot", "\uEA49"},
      {""}, {""}, {""}, {""}, {""},
#line 1783 "material_font.gperf"
      {"settings_input_component", "\uE8C0"},
      {""},
#line 935 "material_font.gperf"
      {"gif_box", "\uE7A3"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 885 "material_font.gperf"
      {"format_align_right", "\uE237"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2222 "material_font.gperf"
      {"wc", "\uE63D"},
      {""}, {""}, {""}, {""},
#line 861 "material_font.gperf"
      {"flourescent", "\uEC31"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1373 "material_font.gperf"
      {"no_drinks", "\uF1A5"},
      {""}, {""}, {""}, {""},
#line 1707 "material_font.gperf"
      {"rule", "\uF1C2"},
      {""}, {""}, {""}, {""}, {""},
#line 2208 "material_font.gperf"
      {"water", "\uF084"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2196 "material_font.gperf"
      {"wallet", "\uF8FF"},
      {""},
#line 587 "material_font.gperf"
      {"desktop_mac", "\uE30B"},
      {""}, {""}, {""}, {""},
#line 2122 "material_font.gperf"
      {"upcoming", "\uF07E"},
      {""},
#line 230 "material_font.gperf"
      {"assignment_add", "\uF848"},
#line 1461 "material_font.gperf"
      {"pause_presentation", "\uE0EA"},
      {""}, {""}, {""}, {""},
#line 2097 "material_font.gperf"
      {"tune", "\uE429"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1401 "material_font.gperf"
      {"notification_important", "\uE004"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 845 "material_font.gperf"
      {"flash_auto", "\uE3E5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2200 "material_font.gperf"
      {"wallpaper", "\uE1BC"},
      {""}, {""}, {""}, {""},
#line 2199 "material_font.gperf"
      {"wallet_travel", "\uE8F8"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1289 "material_font.gperf"
      {"mode_edit_outline", "\uF035"},
      {""}, {""}, {""},
#line 1394 "material_font.gperf"
      {"not_listed_location", "\uE575"},
#line 1842 "material_font.gperf"
      {"sim_card_download", "\uF068"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 889 "material_font.gperf"
      {"format_color_reset", "\uE23B"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1524 "material_font.gperf"
      {"photo_camera", "\uE412"},
      {""},
#line 1430 "material_font.gperf"
      {"outlet", "\uF1D4"},
      {""},
#line 1766 "material_font.gperf"
      {"sentiment_dissatisfied", "\uE811"},
      {""}, {""},
#line 1759 "material_font.gperf"
      {"send_time_extension", "\uEADB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1247 "material_font.gperf"
      {"mark_email_unread", "\uF18A"},
      {""}, {""}, {""},
#line 2206 "material_font.gperf"
      {"watch_later", "\uE924"},
      {""}, {""}, {""}, {""}, {""},
#line 2201 "material_font.gperf"
      {"warehouse", "\uEBB8"},
      {""}, {""}, {""},
#line 2124 "material_font.gperf"
      {"update_disabled", "\uE075"},
      {""}, {""}, {""},
#line 168 "material_font.gperf"
      {"airplanemode_on", "\uE195"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1290 "material_font.gperf"
      {"mode_fan_off", "\uEC17"},
      {""},
#line 251 "material_font.gperf"
      {"auto_awesome", "\uE65F"},
#line 1630 "material_font.gperf"
      {"raw_on", "\uF050"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 739 "material_font.gperf"
      {"euro", "\uEA15"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 892 "material_font.gperf"
      {"format_indent_increase", "\uE23E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1801 "material_font.gperf"
      {"shield_moon", "\uEAA9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2203 "material_font.gperf"
      {"warning_amber", "\uF083"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 260 "material_font.gperf"
      {"auto_stories", "\uE666"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1444 "material_font.gperf"
      {"panorama_horizontal", "\uE40D"},
#line 1057 "material_font.gperf"
      {"input", "\uE890"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1445 "material_font.gperf"
      {"panorama_horizontal_select", "\uEF60"},
      {""}, {""}, {""}, {""},
#line 1246 "material_font.gperf"
      {"mark_email_read", "\uF18C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2230 "material_font.gperf"
      {"west", "\uF1E6"},
#line 2218 "material_font.gperf"
      {"wb_shade", "\uEA01"},
      {""},
#line 1083 "material_font.gperf"
      {"join_inner", "\uEAF4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1277 "material_font.gperf"
      {"minimize", "\uE931"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 702 "material_font.gperf"
      {"edit_square", "\uF88D"},
      {""}, {""}, {""}, {""}, {""},
#line 1535 "material_font.gperf"
      {"picture_as_pdf", "\uE415"},
      {""}, {""}, {""}, {""}, {""},
#line 254 "material_font.gperf"
      {"auto_delete", "\uEA4C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 681 "material_font.gperf"
      {"duo", "\uE9A5"},
      {""}, {""}, {""}, {""}, {""},
#line 423 "material_font.gperf"
      {"cell_tower", "\uEBBA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1826 "material_font.gperf"
      {"signal_cellular_no_sim", "\uE1CE"},
      {""}, {""}, {""},
#line 2214 "material_font.gperf"
      {"wb_auto", "\uE42C"},
      {""},
#line 1769 "material_font.gperf"
      {"sentiment_satisfied_alt", "\uE0ED"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 116 "material_font.gperf"
      {"ad_units", "\uEF39"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1298 "material_font.gperf"
      {"money_off_csred", "\uF038"},
#line 1471 "material_font.gperf"
      {"people_outline", "\uE7FC"},
      {""}, {""}, {""}, {""},
#line 1359 "material_font.gperf"
      {"newspaper", "\uEB81"},
      {""}, {""},
#line 2202 "material_font.gperf"
      {"warning", "\uE002"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 707 "material_font.gperf"
      {"elderly_woman", "\uEB69"},
      {""}, {""}, {""},
#line 2074 "material_font.gperf"
      {"tour", "\uEF75"},
      {""}, {""},
#line 1701 "material_font.gperf"
      {"route", "\uEACD"},
      {""}, {""},
#line 2098 "material_font.gperf"
      {"tungsten", "\uF07D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1702 "material_font.gperf"
      {"router", "\uE328"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1349 "material_font.gperf"
      {"network_cell", "\uE1B9"},
#line 1874 "material_font.gperf"
      {"source", "\uF1C4"},
      {""}, {""}, {""}, {""}, {""},
#line 1357 "material_font.gperf"
      {"new_label", "\uE609"},
#line 2084 "material_font.gperf"
      {"transit_enterexit", "\uE579"},
      {""}, {""},
#line 1358 "material_font.gperf"
      {"new_releases", "\uE031"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 553 "material_font.gperf"
      {"curtains", "\uEC1E"},
      {""}, {""}, {""}, {""}, {""},
#line 697 "material_font.gperf"
      {"edit_location_alt", "\uE1C5"},
      {""}, {""}, {""},
#line 695 "material_font.gperf"
      {"edit_document", "\uF88C"},
      {""},
#line 215 "material_font.gperf"
      {"arrow_drop_down", "\uE5C5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1768 "material_font.gperf"
      {"sentiment_satisfied", "\uE813"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2269 "material_font.gperf"
      {"wrong_location", "\uEF78"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1878 "material_font.gperf"
      {"south_west", "\uF1E5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1877 "material_font.gperf"
      {"south_east", "\uF1E4"},
#line 1432 "material_font.gperf"
      {"output", "\uEBBE"},
      {""}, {""}, {""}, {""},
#line 213 "material_font.gperf"
      {"arrow_circle_up", "\uF182"},
#line 2127 "material_font.gperf"
      {"upload_file", "\uE9FC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 2116 "material_font.gperf"
      {"unfold_less", "\uE5D6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2018 "material_font.gperf"
      {"task", "\uF075"},
      {""},
#line 123 "material_font.gperf"
      {"add_business", "\uE729"},
      {""}, {""}, {""},
#line 1709 "material_font.gperf"
      {"run_circle", "\uEF6F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 522 "material_font.gperf"
      {"countertops", "\uF1F7"},
#line 44 "material_font.gperf"
      {"10k", "\uE951"},
#line 1700 "material_font.gperf"
      {"rounded_corner", "\uE920"},
#line 881 "material_font.gperf"
      {"forklift", "\uF868"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1368 "material_font.gperf"
      {"no_accounts", "\uF03E"},
      {""}, {""}, {""}, {""},
#line 1113 "material_font.gperf"
      {"kitchen", "\uEB47"},
      {""},
#line 1272 "material_font.gperf"
      {"mic_external_on", "\uEF5A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2210 "material_font.gperf"
      {"water_drop", "\uE798"},
      {""}, {""}, {""}, {""}, {""},
#line 436 "material_font.gperf"
      {"check", "\uE5CA"},
      {""}, {""}, {""}, {""},
#line 216 "material_font.gperf"
      {"arrow_drop_down_circle", "\uE5C6"},
      {""}, {""}, {""}, {""}, {""},
#line 214 "material_font.gperf"
      {"arrow_downward", "\uE5DB"},
      {""},
#line 166 "material_font.gperf"
      {"airplanemode_inactive", "\uE194"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 599 "material_font.gperf"
      {"dew_point", "\uF879"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 261 "material_font.gperf"
      {"autofps_select", "\uEFDC"},
      {""}, {""},
#line 128 "material_font.gperf"
      {"add_circle_outline", "\uE148"},
#line 225 "material_font.gperf"
      {"art_track", "\uE060"},
      {""}, {""}, {""}, {""}, {""},
#line 1421 "material_font.gperf"
      {"open_in_new", "\uE89E"},
      {""},
#line 1012 "material_font.gperf"
      {"hls", "\uEB8A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1995 "material_font.gperf"
      {"sync_lock", "\uEAEE"},
#line 1362 "material_font.gperf"
      {"nfc", "\uE1BB"},
      {""}, {""}, {""}, {""},
#line 1728 "material_font.gperf"
      {"schema", "\uE4FD"},
#line 1084 "material_font.gperf"
      {"join_left", "\uEAF2"},
#line 1369 "material_font.gperf"
      {"no_adult_content", "\uF8FE"},
      {""}, {""}, {""}, {""},
#line 1082 "material_font.gperf"
      {"join_full", "\uEAEB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2106 "material_font.gperf"
      {"turned_in_not", "\uE8E7"},
      {""}, {""}, {""}, {""}, {""},
#line 204 "material_font.gperf"
      {"architecture", "\uEA3B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1428 "material_font.gperf"
      {"outdoor_grill", "\uEA47"},
#line 657 "material_font.gperf"
      {"donut_large", "\uE917"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2105 "material_font.gperf"
      {"turned_in", "\uE8E6"},
#line 1739 "material_font.gperf"
      {"screen_share", "\uE0E2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1729 "material_font.gperf"
      {"school", "\uE80C"},
      {""}, {""}, {""}, {""},
#line 2274 "material_font.gperf"
      {"zoom_in_map", "\uEB2D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 615 "material_font.gperf"
      {"directions_off", "\uF10F"},
      {""}, {""},
#line 1351 "material_font.gperf"
      {"network_locked", "\uE61A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 398 "material_font.gperf"
      {"camera_outdoor", "\uEFEA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 649 "material_font.gperf"
      {"document_scanner", "\uE5FA"},
      {""},
#line 224 "material_font.gperf"
      {"arrow_upward", "\uE5D8"},
      {""}, {""}, {""},
#line 2076 "material_font.gperf"
      {"track_changes", "\uE8E1"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1740 "material_font.gperf"
      {"screenshot", "\uF056"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1224 "material_font.gperf"
      {"luggage", "\uF235"},
      {""}, {""}, {""}, {""}, {""},
#line 1219 "material_font.gperf"
      {"loupe", "\uE402"},
#line 1864 "material_font.gperf"
      {"snowing", "\uE80F"},
      {""}, {""},
#line 1903 "material_font.gperf"
      {"sports_handball", "\uEA33"},
      {""}, {""}, {""}, {""}, {""},
#line 1297 "material_font.gperf"
      {"money_off", "\uE25C"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1960 "material_font.gperf"
      {"sunny_snowing", "\uE819"},
      {""},
#line 1154 "material_font.gperf"
      {"linked_camera", "\uE438"},
      {""}, {""}, {""}, {""}, {""},
#line 971 "material_font.gperf"
      {"hardware", "\uEA59"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 638 "material_font.gperf"
      {"do_disturb_alt", "\uF08D"},
      {""}, {""}, {""},
#line 1225 "material_font.gperf"
      {"lunch_dining", "\uEA61"},
      {""}, {""}, {""}, {""}, {""},
#line 1964 "material_font.gperf"
      {"support", "\uEF73"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1961 "material_font.gperf"
      {"superscript", "\uF112"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 554 "material_font.gperf"
      {"curtains_closed", "\uEC1D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 576 "material_font.gperf"
      {"delete_sweep", "\uE16C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 640 "material_font.gperf"
      {"do_disturb_on", "\uF08F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2099 "material_font.gperf"
      {"turn_left", "\uEBA6"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1431 "material_font.gperf"
      {"outlined_flag", "\uE16E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 921 "material_font.gperf"
      {"fullscreen", "\uE5D0"},
      {""}, {""}, {""},
#line 149 "material_font.gperf"
      {"admin_panel_settings", "\uEF3D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2211 "material_font.gperf"
      {"waterfall_chart", "\uEA00"},
      {""}, {""}, {""}, {""}, {""},
#line 2216 "material_font.gperf"
      {"wb_incandescent", "\uE42E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 961 "material_font.gperf"
      {"group_remove", "\uE7AD"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1453 "material_font.gperf"
      {"park", "\uEA63"},
      {""}, {""}, {""}, {""},
#line 585 "material_font.gperf"
      {"desk", "\uF8F4"},
      {""},
#line 1965 "material_font.gperf"
      {"support_agent", "\uF0E2"},
      {""},
#line 2101 "material_font.gperf"
      {"turn_sharp_left", "\uEBA7"},
#line 571 "material_font.gperf"
      {"deck", "\uEA42"},
      {""}, {""}, {""}, {""},
#line 1534 "material_font.gperf"
      {"piano_off", "\uE520"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1241 "material_font.gperf"
      {"maps_ugc", "\uEF58"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 376 "material_font.gperf"
      {"cached", "\uE86A"},
      {""}, {""},
#line 2030 "material_font.gperf"
      {"text_rotate_up", "\uE93A"},
      {""}, {""}, {""},
#line 1473 "material_font.gperf"
      {"perm_camera_mic", "\uE8A2"},
#line 1264 "material_font.gperf"
      {"menu_open", "\uE9BD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 99 "material_font.gperf"
      {"9k_plus", "\uE978"},
#line 622 "material_font.gperf"
      {"directions_transit", "\uE535"},
      {""},
#line 93 "material_font.gperf"
      {"7k_plus", "\uE972"},
#line 84 "material_font.gperf"
      {"5k_plus", "\uE96C"},
      {""},
#line 879 "material_font.gperf"
      {"fork_left", "\uEBA0"},
#line 96 "material_font.gperf"
      {"8k_plus", "\uE975"},
#line 66 "material_font.gperf"
      {"2k_plus", "\uE964"},
      {""}, {""}, {""},
#line 90 "material_font.gperf"
      {"6k_plus", "\uE96F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2000 "material_font.gperf"
      {"system_update", "\uE62A"},
      {""}, {""},
#line 1865 "material_font.gperf"
      {"snowmobile", "\uE503"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 58 "material_font.gperf"
      {"1k_plus", "\uE95D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 968 "material_font.gperf"
      {"hail", "\uE9B1"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1708 "material_font.gperf"
      {"rule_folder", "\uF1C9"},
#line 1863 "material_font.gperf"
      {"snowboarding", "\uE513"},
      {""}, {""}, {""}, {""}, {""},
#line 80 "material_font.gperf"
      {"4k_plus", "\uE969"},
      {""},
#line 1046 "material_font.gperf"
      {"image_not_supported", "\uF116"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 74 "material_font.gperf"
      {"3k_plus", "\uE967"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 189 "material_font.gperf"
      {"anchor", "\uF1CD"},
      {""}, {""},
#line 985 "material_font.gperf"
      {"headset", "\uE310"},
#line 572 "material_font.gperf"
      {"dehaze", "\uE3C7"},
      {""},
#line 1352 "material_font.gperf"
      {"network_ping", "\uEBCA"},
      {""}, {""}, {""}, {""},
#line 249 "material_font.gperf"
      {"audio_file", "\uEB82"},
      {""},
#line 2217 "material_font.gperf"
      {"wb_iridescent", "\uE436"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2197 "material_font.gperf"
      {"wallet_giftcard", "\uE8F6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1526 "material_font.gperf"
      {"photo_camera_front", "\uEF69"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 164 "material_font.gperf"
      {"airplane_ticket", "\uEFD9"},
      {""},
#line 992 "material_font.gperf"
      {"heart_broken", "\uEAC2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 443 "material_font.gperf"
      {"checkroom", "\uF19E"},
      {""}, {""}, {""},
#line 1429 "material_font.gperf"
      {"outgoing_mail", "\uF0D2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 199 "material_font.gperf"
      {"app_shortcut", "\uEAE4"},
      {""}, {""},
#line 1966 "material_font.gperf"
      {"surfing", "\uE515"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1274 "material_font.gperf"
      {"mic_off", "\uE02B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1673 "material_font.gperf"
      {"request_page", "\uF22C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1544 "material_font.gperf"
      {"pin_invoke", "\uE763"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 990 "material_font.gperf"
      {"hearing", "\uE023"},
      {""}, {""},
#line 988 "material_font.gperf"
      {"healing", "\uE3F3"},
      {""}, {""}, {""}, {""}, {""},
#line 1250 "material_font.gperf"
      {"markunread_mailbox", "\uE89B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 558 "material_font.gperf"
      {"dashboard", "\uE871"},
#line 451 "material_font.gperf"
      {"circle_notifications", "\uE994"},
#line 209 "material_font.gperf"
      {"arrow_back_ios_new", "\uE2EA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1517 "material_font.gperf"
      {"phonelink_erase", "\uE0DB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1908 "material_font.gperf"
      {"sports_motorsports", "\uEA2D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 607 "material_font.gperf"
      {"directions_bike", "\uE52F"},
      {""}, {""}, {""},
#line 2268 "material_font.gperf"
      {"wrap_text", "\uE25B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 670 "material_font.gperf"
      {"drag_handle", "\uE25D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 817 "material_font.gperf"
      {"filter_alt_off", "\uEB32"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2221 "material_font.gperf"
      {"wb_twilight", "\uE1C6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1580 "material_font.gperf"
      {"precision_manufacturing", "\uF049"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1348 "material_font.gperf"
      {"nest_cam_wired_stand", "\uEC16"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 923 "material_font.gperf"
      {"functions", "\uE24A"},
      {""}, {""},
#line 613 "material_font.gperf"
      {"directions_car_filled", "\uEFF7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 389 "material_font.gperf"
      {"call_missed_outgoing", "\uE0E4"},
      {""}, {""},
#line 996 "material_font.gperf"
      {"help_center", "\uF1C0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1171 "material_font.gperf"
      {"local_fire_department", "\uEF55"},
      {""}, {""},
#line 1403 "material_font.gperf"
      {"notifications_active", "\uE7F7"},
      {""},
#line 1450 "material_font.gperf"
      {"panorama_wide_angle", "\uE40F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1287 "material_font.gperf"
      {"mode_comment", "\uE253"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 997 "material_font.gperf"
      {"help_outline", "\uE8FD"},
      {""}, {""}, {""}, {""},
#line 1451 "material_font.gperf"
      {"panorama_wide_angle_select", "\uEF62"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 779 "material_font.gperf"
      {"fast_rewind", "\uE020"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 493 "material_font.gperf"
      {"confirmation_number", "\uE638"},
      {""}, {""},
#line 1041 "material_font.gperf"
      {"hvac", "\uF10E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 774 "material_font.gperf"
      {"facebook", "\uF234"},
      {""},
#line 151 "material_font.gperf"
      {"ads_click", "\uE762"},
#line 1616 "material_font.gperf"
      {"quora", "\uEA98"},
#line 1047 "material_font.gperf"
      {"image_search", "\uE43F"},
      {""},
#line 1058 "material_font.gperf"
      {"insert_chart", "\uE24B"},
      {""}, {""}, {""},
#line 1860 "material_font.gperf"
      {"snapchat", "\uEA6E"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1649 "material_font.gperf"
      {"remove_circle_outline", "\uE15D"},
      {""}, {""},
#line 586 "material_font.gperf"
      {"desktop_access_disabled", "\uE99D"},
      {""}, {""}, {""}, {""},
#line 1621 "material_font.gperf"
      {"radio_button_off", "\uE836"},
      {""}, {""}, {""}, {""},
#line 1606 "material_font.gperf"
      {"query_stats", "\uE4FC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 939 "material_font.gperf"
      {"golf_course", "\uEB45"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 980 "material_font.gperf"
      {"hdr_plus", "\uF01E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 994 "material_font.gperf"
      {"height", "\uEA16"},
#line 1584 "material_font.gperf"
      {"price_change", "\uF04A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 145 "material_font.gperf"
      {"add_to_queue", "\uE05C"},
      {""}, {""}, {""},
#line 623 "material_font.gperf"
      {"directions_transit_filled", "\uEFFA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1869 "material_font.gperf"
      {"solar_power", "\uEC0F"},
      {""}, {""},
#line 972 "material_font.gperf"
      {"hd", "\uE052"},
      {""}, {""}, {""},
#line 978 "material_font.gperf"
      {"hdr_on", "\uE3EE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 979 "material_font.gperf"
      {"hdr_on_select", "\uF01D"},
#line 2259 "material_font.gperf"
      {"wordpress", "\uEA9F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2103 "material_font.gperf"
      {"turn_slight_left", "\uEBA4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 178 "material_font.gperf"
      {"align_horizontal_right", "\uE010"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2209 "material_font.gperf"
      {"water_damage", "\uF203"},
      {""},
#line 141 "material_font.gperf"
      {"add_task", "\uF23A"},
      {""}, {""},
#line 1327 "material_font.gperf"
      {"multiline_chart", "\uE6DF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1115 "material_font.gperf"
      {"label", "\uE892"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 375 "material_font.gperf"
      {"cable", "\uEFE6"},
#line 2010 "material_font.gperf"
      {"tablet", "\uE32F"},
#line 1413 "material_font.gperf"
      {"offline_share", "\uE9C5"},
      {""},
#line 274 "material_font.gperf"
      {"ballot", "\uE172"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 272 "material_font.gperf"
      {"balance", "\uEAF6"},
      {""}, {""}, {""},
#line 466 "material_font.gperf"
      {"cloud_off", "\uE2C1"},
      {""},
#line 995 "material_font.gperf"
      {"help", "\uE887"},
      {""},
#line 565 "material_font.gperf"
      {"data_thresholding", "\uEB9F"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 126 "material_font.gperf"
      {"add_chart", "\uE97B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1645 "material_font.gperf"
      {"refresh", "\uE5D5"},
#line 109 "material_font.gperf"
      {"accessible", "\uE914"},
      {""}, {""}, {""},
#line 167 "material_font.gperf"
      {"airplanemode_off", "\uE194"},
      {""}, {""}, {""},
#line 2073 "material_font.gperf"
      {"touch_app", "\uE913"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1629 "material_font.gperf"
      {"raw_off", "\uF04F"},
      {""},
#line 304 "material_font.gperf"
      {"blender", "\uEFE3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1741 "material_font.gperf"
      {"screenshot_monitor", "\uEC08"},
      {""}, {""},
#line 1695 "material_font.gperf"
      {"rotate_90_degrees_cw", "\uEAAB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1520 "material_font.gperf"
      {"phonelink_ring", "\uE0DD"},
      {""}, {""}, {""}, {""},
#line 280 "material_font.gperf"
      {"battery_0_bar", "\uEBDC"},
      {""},
#line 2005 "material_font.gperf"
      {"table_bar", "\uEAD2"},
      {""}, {""},
#line 290 "material_font.gperf"
      {"battery_saver", "\uEFDE"},
#line 285 "material_font.gperf"
      {"battery_5_bar", "\uEBD4"},
#line 131 "material_font.gperf"
      {"add_home_work", "\uF8ED"},
      {""}, {""},
#line 282 "material_font.gperf"
      {"battery_2_bar", "\uEBE0"},
      {""}, {""}, {""},
#line 286 "material_font.gperf"
      {"battery_6_bar", "\uEBD2"},
#line 287 "material_font.gperf"
      {"battery_alert", "\uE19C"},
      {""}, {""},
#line 825 "material_font.gperf"
      {"filter_list_off", "\uEB57"},
      {""}, {""}, {""},
#line 1895 "material_font.gperf"
      {"sports_bar", "\uF1F3"},
      {""}, {""},
#line 259 "material_font.gperf"
      {"auto_mode", "\uEC20"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 281 "material_font.gperf"
      {"battery_1_bar", "\uEBD9"},
#line 1576 "material_font.gperf"
      {"power", "\uE63C"},
      {""}, {""}, {""}, {""}, {""},
#line 630 "material_font.gperf"
      {"discount", "\uEBC9"},
      {""}, {""}, {""},
#line 1738 "material_font.gperf"
      {"screen_search_desktop", "\uEF70"},
      {""},
#line 916 "material_font.gperf"
      {"foundation", "\uF200"},
      {""}, {""},
#line 788 "material_font.gperf"
      {"feedback", "\uE87F"},
      {""}, {""}, {""}, {""}, {""},
#line 284 "material_font.gperf"
      {"battery_4_bar", "\uEBE2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 283 "material_font.gperf"
      {"battery_3_bar", "\uEBDD"},
#line 999 "material_font.gperf"
      {"hexagon", "\uEB39"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 686 "material_font.gperf"
      {"earbuds", "\uF003"},
      {""}, {""}, {""},
#line 1896 "material_font.gperf"
      {"sports_baseball", "\uEA51"},
      {""}, {""}, {""}, {""}, {""},
#line 2007 "material_font.gperf"
      {"table_restaurant", "\uEAC6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1615 "material_font.gperf"
      {"quiz", "\uF04C"},
      {""},
#line 1067 "material_font.gperf"
      {"insights", "\uF092"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 778 "material_font.gperf"
      {"fast_forward", "\uE01F"},
      {""}, {""},
#line 1807 "material_font.gperf"
      {"shopping_basket", "\uE8CB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 849 "material_font.gperf"
      {"flashlight_on", "\uF00B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1317 "material_font.gperf"
      {"mouse", "\uE323"},
#line 1564 "material_font.gperf"
      {"plumbing", "\uF107"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1680 "material_font.gperf"
      {"restore_from_trash", "\uE938"},
      {""},
#line 981 "material_font.gperf"
      {"hdr_strong", "\uE3F1"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 960 "material_font.gperf"
      {"group_off", "\uE747"},
#line 1202 "material_font.gperf"
      {"lock", "\uE897"},
      {""}, {""},
#line 291 "material_font.gperf"
      {"battery_std", "\uE1A5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 119 "material_font.gperf"
      {"add_a_photo", "\uE439"},
      {""}, {""},
#line 666 "material_font.gperf"
      {"download_done", "\uF091"},
#line 1386 "material_font.gperf"
      {"noise_aware", "\uEBEC"},
#line 424 "material_font.gperf"
      {"cell_wifi", "\uE0EC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1732 "material_font.gperf"
      {"scoreboard", "\uEBD0"},
      {""}, {""}, {""}, {""}, {""},
#line 276 "material_font.gperf"
      {"barcode_reader", "\uF85C"},
#line 610 "material_font.gperf"
      {"directions_bus", "\uE530"},
#line 1381 "material_font.gperf"
      {"no_meeting_room", "\uEB4E"},
      {""}, {""}, {""},
#line 1408 "material_font.gperf"
      {"now_wallpaper", "\uE1BC"},
      {""}, {""}, {""}, {""}, {""},
#line 1066 "material_font.gperf"
      {"insert_photo", "\uE251"},
      {""}, {""}, {""}, {""}, {""},
#line 111 "material_font.gperf"
      {"account_balance", "\uE84F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 300 "material_font.gperf"
      {"beenhere", "\uE52D"},
      {""}, {""}, {""}, {""},
#line 888 "material_font.gperf"
      {"format_color_fill", "\uE23A"},
      {""}, {""}, {""}, {""},
#line 2042 "material_font.gperf"
      {"thermostat_auto", "\uF077"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 2060 "material_font.gperf"
      {"timer_off", "\uE426"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1025 "material_font.gperf"
      {"hotel", "\uE53A"},
      {""},
#line 665 "material_font.gperf"
      {"download", "\uF090"},
      {""}, {""},
#line 133 "material_font.gperf"
      {"add_link", "\uE178"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 618 "material_font.gperf"
      {"directions_run", "\uE566"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1876 "material_font.gperf"
      {"south_america", "\uE7E4"},
#line 2032 "material_font.gperf"
      {"text_rotation_angledown", "\uE93C"},
#line 301 "material_font.gperf"
      {"bento", "\uF1F4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1389 "material_font.gperf"
      {"north", "\uF1E0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1203 "material_font.gperf"
      {"lock_clock", "\uEF57"},
#line 293 "material_font.gperf"
      {"beach_access", "\uEB3E"},
      {""},
#line 987 "material_font.gperf"
      {"headset_off", "\uE33A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1687 "material_font.gperf"
      {"roller_shades", "\uEC12"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1331 "material_font.gperf"
      {"music_note", "\uE405"},
#line 648 "material_font.gperf"
      {"dock", "\uE30E"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 270 "material_font.gperf"
      {"badge", "\uEA67"},
      {""}, {""}, {""},
#line 570 "material_font.gperf"
      {"deblur", "\uEB77"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 729 "material_font.gperf"
      {"emoji_transportation", "\uEA1F"},
      {""},
#line 2118 "material_font.gperf"
      {"unfold_more", "\uE5D7"},
      {""}, {""}, {""}, {""}, {""},
#line 374 "material_font.gperf"
      {"cabin", "\uE589"},
#line 1412 "material_font.gperf"
      {"offline_pin", "\uE90A"},
      {""}, {""},
#line 1816 "material_font.gperf"
      {"shutter_speed", "\uE43D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1026 "material_font.gperf"
      {"hotel_class", "\uE743"},
      {""}, {""},
#line 962 "material_font.gperf"
      {"group_work", "\uE886"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1633 "material_font.gperf"
      {"rebase_edit", "\uF846"},
      {""}, {""}, {""}, {""}, {""},
#line 922 "material_font.gperf"
      {"fullscreen_exit", "\uE5D1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 306 "material_font.gperf"
      {"blinds", "\uE286"},
      {""}, {""},
#line 2004 "material_font.gperf"
      {"tab_unselected", "\uE8D9"},
#line 2080 "material_font.gperf"
      {"transcribe", "\uF8EC"},
      {""},
#line 1674 "material_font.gperf"
      {"request_quote", "\uF1B6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1778 "material_font.gperf"
      {"settings_brightness", "\uE8BD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1271 "material_font.gperf"
      {"mic_external_off", "\uEF59"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1409 "material_font.gperf"
      {"now_widgets", "\uE1BD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 658 "material_font.gperf"
      {"donut_small", "\uE918"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1184 "material_font.gperf"
      {"local_phone", "\uE551"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1442 "material_font.gperf"
      {"panorama_fish_eye", "\uE40C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1388 "material_font.gperf"
      {"nordic_walking", "\uE50E"},
      {""}, {""}, {""}, {""},
#line 294 "material_font.gperf"
      {"bed", "\uEFDF"},
      {""}, {""},
#line 144 "material_font.gperf"
      {"add_to_photos", "\uE39D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 238 "material_font.gperf"
      {"assistant_direction", "\uE988"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 305 "material_font.gperf"
      {"blind", "\uF8D6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 310 "material_font.gperf"
      {"bloodtype", "\uEFE4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1419 "material_font.gperf"
      {"open_in_browser", "\uE89D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 277 "material_font.gperf"
      {"batch_prediction", "\uF0F5"},
#line 1703 "material_font.gperf"
      {"rowing", "\uE921"},
      {""},
#line 914 "material_font.gperf"
      {"forward_5", "\uE058"},
      {""}, {""}, {""},
#line 1153 "material_font.gperf"
      {"link_off", "\uE16F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 358 "material_font.gperf"
      {"broadcast_on_personal", "\uF8F9"},
      {""}, {""}, {""}, {""}, {""},
#line 912 "material_font.gperf"
      {"forward_10", "\uE056"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1577 "material_font.gperf"
      {"power_input", "\uE336"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 352 "material_font.gperf"
      {"brightness_7", "\uE3AC"},
#line 350 "material_font.gperf"
      {"brightness_5", "\uE3AA"},
      {""}, {""}, {""},
#line 347 "material_font.gperf"
      {"brightness_2", "\uE3A7"},
      {""}, {""}, {""},
#line 351 "material_font.gperf"
      {"brightness_6", "\uE3AB"},
      {""}, {""}, {""},
#line 913 "material_font.gperf"
      {"forward_30", "\uE057"},
#line 1737 "material_font.gperf"
      {"screen_rotation_alt", "\uEBEE"},
#line 647 "material_font.gperf"
      {"do_not_touch", "\uF1B0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1013 "material_font.gperf"
      {"hls_off", "\uEB8C"},
      {""},
#line 346 "material_font.gperf"
      {"brightness_1", "\uE3A6"},
      {""},
#line 911 "material_font.gperf"
      {"forward", "\uE154"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1256 "material_font.gperf"
      {"medical_information", "\uEBED"},
      {""}, {""}, {""},
#line 349 "material_font.gperf"
      {"brightness_4", "\uE3A9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 970 "material_font.gperf"
      {"handyman", "\uF10B"},
      {""}, {""},
#line 348 "material_font.gperf"
      {"brightness_3", "\uE3A8"},
      {""}, {""}, {""}, {""},
#line 1767 "material_font.gperf"
      {"sentiment_neutral", "\uE812"},
      {""},
#line 1706 "material_font.gperf"
      {"rtt", "\uE9AD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 942 "material_font.gperf"
      {"gpp_maybe", "\uF014"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1718 "material_font.gperf"
      {"save", "\uE161"},
#line 1921 "material_font.gperf"
      {"star", "\uE838"},
#line 1929 "material_font.gperf"
      {"start", "\uE089"},
      {""}, {""},
#line 2015 "material_font.gperf"
      {"takeout_dining", "\uEA74"},
      {""},
#line 1928 "material_font.gperf"
      {"stars", "\uE8D0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 977 "material_font.gperf"
      {"hdr_off_select", "\uF01C"},
#line 1515 "material_font.gperf"
      {"phone_paused", "\uE620"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1720 "material_font.gperf"
      {"save_as", "\uEB60"},
#line 1229 "material_font.gperf"
      {"mail_lock", "\uEC0A"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1719 "material_font.gperf"
      {"save_alt", "\uE171"},
      {""}, {""}, {""}, {""},
#line 1927 "material_font.gperf"
      {"star_rate", "\uF0EC"},
      {""}, {""},
#line 2011 "material_font.gperf"
      {"tablet_android", "\uE330"},
      {""}, {""}, {""}, {""},
#line 716 "material_font.gperf"
      {"elevator", "\uF1A0"},
      {""}, {""}, {""},
#line 1078 "material_font.gperf"
      {"ios_share", "\uE6B8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 307 "material_font.gperf"
      {"blinds_closed", "\uEC1F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1814 "material_font.gperf"
      {"shuffle", "\uE043"},
      {""},
#line 668 "material_font.gperf"
      {"downloading", "\uF001"},
      {""},
#line 148 "material_font.gperf"
      {"adjust", "\uE39E"},
#line 1939 "material_font.gperf"
      {"store", "\uE8D1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2130 "material_font.gperf"
      {"vaccines", "\uE138"},
      {""},
#line 288 "material_font.gperf"
      {"battery_charging_full", "\uE1A3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1948 "material_font.gperf"
      {"stroller", "\uF1AE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 917 "material_font.gperf"
      {"free_breakfast", "\uEB44"},
      {""}, {""},
#line 975 "material_font.gperf"
      {"hdr_enhanced_select", "\uEF51"},
      {""}, {""},
#line 1776 "material_font.gperf"
      {"settings_backup_restore", "\uE8BA"},
      {""}, {""}, {""}, {""},
#line 1925 "material_font.gperf"
      {"star_outline", "\uF06F"},
      {""},
#line 150 "material_font.gperf"
      {"adobe", "\uEA96"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 289 "material_font.gperf"
      {"battery_full", "\uE1A4"},
      {""}, {""}, {""},
#line 296 "material_font.gperf"
      {"bedroom_child", "\uEFE1"},
      {""}, {""}, {""}, {""},
#line 2207 "material_font.gperf"
      {"watch_off", "\uEAE3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 309 "material_font.gperf"
      {"block_flipped", "\uEF46"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1333 "material_font.gperf"
      {"music_video", "\uE063"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 986 "material_font.gperf"
      {"headset_mic", "\uE311"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 257 "material_font.gperf"
      {"auto_fix_off", "\uE665"},
      {""},
#line 1748 "material_font.gperf"
      {"search_off", "\uEA76"},
      {""},
#line 412 "material_font.gperf"
      {"card_travel", "\uE8F8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 165 "material_font.gperf"
      {"airplanemode_active", "\uE195"},
      {""}, {""}, {""}, {""},
#line 154 "material_font.gperf"
      {"airline_seat_flat", "\uE630"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 113 "material_font.gperf"
      {"account_box", "\uE851"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 930 "material_font.gperf"
      {"gavel", "\uE90E"},
      {""},
#line 1815 "material_font.gperf"
      {"shuffle_on", "\uE9E1"},
      {""}, {""}, {""},
#line 910 "material_font.gperf"
      {"forum", "\uE0BF"},
      {""}, {""}, {""}, {""}, {""},
#line 803 "material_font.gperf"
      {"file_upload", "\uE2C6"},
      {""},
#line 297 "material_font.gperf"
      {"bedroom_parent", "\uEFE2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1943 "material_font.gperf"
      {"straight", "\uEB95"},
#line 2033 "material_font.gperf"
      {"text_rotation_angleup", "\uE93D"},
      {""}, {""}, {""}, {""}, {""},
#line 1688 "material_font.gperf"
      {"roller_shades_closed", "\uEC11"},
#line 564 "material_font.gperf"
      {"data_saver_on", "\uEFF3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 715 "material_font.gperf"
      {"electrical_services", "\uF102"},
      {""}, {""}, {""},
#line 1813 "material_font.gperf"
      {"shower", "\uF061"},
      {""},
#line 1530 "material_font.gperf"
      {"photo_size_select_large", "\uE433"},
      {""},
#line 1529 "material_font.gperf"
      {"photo_size_select_actual", "\uE432"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1531 "material_font.gperf"
      {"photo_size_select_small", "\uE434"},
#line 1130 "material_font.gperf"
      {"layers", "\uE53B"},
      {""}, {""}, {""},
#line 2111 "material_font.gperf"
      {"u_turn_left", "\uEBA1"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1944 "material_font.gperf"
      {"straighten", "\uE41C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1938 "material_font.gperf"
      {"storage", "\uE1DB"},
      {""}, {""},
#line 143 "material_font.gperf"
      {"add_to_home_screen", "\uE1FE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1812 "material_font.gperf"
      {"show_chart", "\uE6E1"},
      {""}, {""}, {""},
#line 1131 "material_font.gperf"
      {"layers_clear", "\uE53C"},
      {""}, {""},
#line 112 "material_font.gperf"
      {"account_balance_wallet", "\uE850"},
      {""}, {""}, {""}, {""}, {""},
#line 875 "material_font.gperf"
      {"font_download", "\uE167"},
      {""},
#line 966 "material_font.gperf"
      {"h_mobiledata", "\uF018"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1627 "material_font.gperf"
      {"ramp_right", "\uEB96"},
      {""}, {""}, {""}, {""}, {""},
#line 2092 "material_font.gperf"
      {"trolley", "\uF86B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1328 "material_font.gperf"
      {"multiple_stop", "\uF1B9"},
      {""},
#line 1519 "material_font.gperf"
      {"phonelink_off", "\uE327"},
      {""},
#line 1418 "material_font.gperf"
      {"opacity", "\uE91C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 894 "material_font.gperf"
      {"format_line_spacing", "\uE240"},
      {""}, {""}, {""}, {""},
#line 1920 "material_font.gperf"
      {"stairs", "\uF1A9"},
      {""}, {""}, {""}, {""}, {""},
#line 884 "material_font.gperf"
      {"format_align_left", "\uE236"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 882 "material_font.gperf"
      {"format_align_center", "\uE234"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 201 "material_font.gperf"
      {"approval", "\uE982"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 438 "material_font.gperf"
      {"check_box_outline_blank", "\uE835"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 467 "material_font.gperf"
      {"cloud_queue", "\uE2C2"},
      {""}, {""}, {""}, {""},
#line 2233 "material_font.gperf"
      {"where_to_vote", "\uE177"},
#line 1459 "material_font.gperf"
      {"pause_circle_filled", "\uE035"},
      {""}, {""}, {""}, {""}, {""},
#line 720 "material_font.gperf"
      {"emergency_share", "\uEBF6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2256 "material_font.gperf"
      {"woman", "\uE13E"},
      {""}, {""}, {""}, {""},
#line 492 "material_font.gperf"
      {"confirmation_num", "\uE638"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 890 "material_font.gperf"
      {"format_color_text", "\uE23C"},
#line 2236 "material_font.gperf"
      {"width_normal", "\uF8F6"},
      {""}, {""}, {""},
#line 1201 "material_font.gperf"
      {"location_searching", "\uE1B7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 706 "material_font.gperf"
      {"elderly", "\uF21A"},
      {""}, {""}, {""},
#line 1591 "material_font.gperf"
      {"production_quantity_limits", "\uE1D1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 247 "material_font.gperf"
      {"attractions", "\uEA52"},
      {""}, {""}, {""},
#line 2257 "material_font.gperf"
      {"woman_2", "\uF8E7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1936 "material_font.gperf"
      {"stop_circle", "\uEF71"},
#line 560 "material_font.gperf"
      {"data_array", "\uEAD1"},
#line 1794 "material_font.gperf"
      {"severe_cold", "\uEBD3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1342 "material_font.gperf"
      {"navigate_next", "\uE409"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1155 "material_font.gperf"
      {"liquor", "\uEA60"},
      {""}, {""},
#line 2139 "material_font.gperf"
      {"vertical_shades", "\uEC0E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2195 "material_font.gperf"
      {"vrpano", "\uF082"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 321 "material_font.gperf"
      {"bolt", "\uEA0B"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2234 "material_font.gperf"
      {"widgets", "\uE1BD"},
      {""}, {""}, {""}, {""}, {""},
#line 1189 "material_font.gperf"
      {"local_print_shop", "\uE555"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1222 "material_font.gperf"
      {"lte_mobiledata", "\uF02C"},
      {""}, {""}, {""}, {""},
#line 664 "material_font.gperf"
      {"downhill_skiing", "\uE509"},
      {""}, {""}, {""}, {""}, {""},
#line 2271 "material_font.gperf"
      {"yard", "\uF089"},
      {""}, {""}, {""}, {""},
#line 2229 "material_font.gperf"
      {"weekend", "\uE16B"},
      {""}, {""},
#line 1958 "material_font.gperf"
      {"summarize", "\uF071"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1346 "material_font.gperf"
      {"nearby_error", "\uF03B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1193 "material_font.gperf"
      {"local_shipping", "\uE558"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1341 "material_font.gperf"
      {"navigate_before", "\uE408"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 673 "material_font.gperf"
      {"drive_eta", "\uE613"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1722 "material_font.gperf"
      {"savings", "\uE2EB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 155 "material_font.gperf"
      {"airline_seat_flat_angled", "\uE631"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2258 "material_font.gperf"
      {"woo_commerce", "\uEA6D"},
      {""},
#line 1817 "material_font.gperf"
      {"sick", "\uF220"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 592 "material_font.gperf"
      {"developer_mode", "\uE1B0"},
      {""}, {""}, {""}, {""}, {""},
#line 1152 "material_font.gperf"
      {"link", "\uE157"},
      {""}, {""}, {""},
#line 976 "material_font.gperf"
      {"hdr_off", "\uE3ED"},
      {""}, {""},
#line 2112 "material_font.gperf"
      {"u_turn_right", "\uEBA2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1871 "material_font.gperf"
      {"sort_by_alpha", "\uE053"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2012 "material_font.gperf"
      {"tablet_mac", "\uE331"},
      {""}, {""}, {""},
#line 1774 "material_font.gperf"
      {"settings_accessibility", "\uF05D"},
#line 2141 "material_font.gperf"
      {"vertical_split", "\uE949"},
      {""}, {""},
#line 1968 "material_font.gperf"
      {"swap_calls", "\uE0D7"},
      {""},
#line 2107 "material_font.gperf"
      {"tv", "\uE333"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 250 "material_font.gperf"
      {"audiotrack", "\uE3A1"},
      {""}, {""},
#line 337 "material_font.gperf"
      {"border_left", "\uE22E"},
      {""},
#line 1600 "material_font.gperf"
      {"punch_clock", "\uEAA8"},
#line 340 "material_font.gperf"
      {"border_style", "\uE231"},
      {""}, {""},
#line 2050 "material_font.gperf"
      {"thunderstorm", "\uEBDB"},
      {""}, {""},
#line 333 "material_font.gperf"
      {"border_clear", "\uE22A"},
      {""}, {""}, {""}, {""}, {""},
#line 331 "material_font.gperf"
      {"border_all", "\uE228"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 907 "material_font.gperf"
      {"format_underline", "\uE249"},
#line 1551 "material_font.gperf"
      {"play_circle", "\uE1C4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 596 "material_font.gperf"
      {"devices", "\uE1B1"},
      {""}, {""}, {""},
#line 905 "material_font.gperf"
      {"format_textdirection_l_to_r", "\uE247"},
      {""}, {""},
#line 906 "material_font.gperf"
      {"format_textdirection_r_to_l", "\uE248"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1557 "material_font.gperf"
      {"play_lesson", "\uF047"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 345 "material_font.gperf"
      {"breakfast_dining", "\uEA54"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 680 "material_font.gperf"
      {"dry_cleaning", "\uEA58"},
      {""}, {""}, {""}, {""},
#line 639 "material_font.gperf"
      {"do_disturb_off", "\uF08E"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 334 "material_font.gperf"
      {"border_color", "\uE22B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2086 "material_font.gperf"
      {"travel_explore", "\uE2DB"},
      {""}, {""}, {""}, {""},
#line 235 "material_font.gperf"
      {"assignment_turned_in", "\uE862"},
      {""},
#line 1926 "material_font.gperf"
      {"star_purple500", "\uF09A"},
      {""}, {""}, {""},
#line 1150 "material_font.gperf"
      {"line_weight", "\uE91A"},
      {""},
#line 1935 "material_font.gperf"
      {"stop", "\uE047"},
      {""},
#line 590 "material_font.gperf"
      {"developer_board", "\uE30D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2140 "material_font.gperf"
      {"vertical_shades_closed", "\uEC0D"},
      {""}, {""}, {""},
#line 59 "material_font.gperf"
      {"1x_mobiledata", "\uEFCD"},
#line 1052 "material_font.gperf"
      {"inbox", "\uE156"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 871 "material_font.gperf"
      {"folder_shared", "\uE2C9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1023 "material_font.gperf"
      {"horizontal_split", "\uE947"},
#line 233 "material_font.gperf"
      {"assignment_return", "\uE860"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 256 "material_font.gperf"
      {"auto_fix_normal", "\uE664"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1075 "material_font.gperf"
      {"invert_colors", "\uE891"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1655 "material_font.gperf"
      {"remove_shopping_cart", "\uE928"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1667 "material_font.gperf"
      {"reply", "\uE15E"},
      {""}, {""}, {""}, {""}, {""},
#line 2237 "material_font.gperf"
      {"width_wide", "\uF8F7"},
      {""}, {""}, {""},
#line 758 "material_font.gperf"
      {"exposure_minus_2", "\uE3CC"},
      {""}, {""}, {""},
#line 908 "material_font.gperf"
      {"format_underlined", "\uE249"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1343 "material_font.gperf"
      {"navigation", "\uE55D"},
      {""}, {""}, {""}, {""}, {""},
#line 598 "material_font.gperf"
      {"devices_other", "\uE337"},
      {""}, {""}, {""},
#line 487 "material_font.gperf"
      {"compare_arrows", "\uE915"},
      {""}, {""},
#line 757 "material_font.gperf"
      {"exposure_minus_1", "\uE3CB"},
      {""}, {""}, {""},
#line 1665 "material_font.gperf"
      {"replay_5", "\uE05B"},
      {""}, {""}, {""}, {""}, {""},
#line 1190 "material_font.gperf"
      {"local_printshop", "\uE555"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1663 "material_font.gperf"
      {"replay_10", "\uE059"},
#line 1143 "material_font.gperf"
      {"light", "\uF02A"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1873 "material_font.gperf"
      {"soup_kitchen", "\uE7D3"},
      {""}, {""}, {""},
#line 624 "material_font.gperf"
      {"directions_walk", "\uE536"},
#line 1941 "material_font.gperf"
      {"storefront", "\uEA12"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1015 "material_font.gperf"
      {"home", "\uE88A"},
      {""}, {""},
#line 234 "material_font.gperf"
      {"assignment_returned", "\uE861"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1646 "material_font.gperf"
      {"remember_me", "\uF051"},
      {""}, {""}, {""}, {""},
#line 1664 "material_font.gperf"
      {"replay_30", "\uE05A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2193 "material_font.gperf"
      {"vpn_key_off", "\uEB7A"},
      {""}, {""}, {""}, {""},
#line 152 "material_font.gperf"
      {"agriculture", "\uEA79"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2121 "material_font.gperf"
      {"unsubscribe", "\uE0EB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 791 "material_font.gperf"
      {"festival", "\uEA68"},
      {""}, {""}, {""}, {""},
#line 1974 "material_font.gperf"
      {"swipe", "\uE9EC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1902 "material_font.gperf"
      {"sports_gymnastics", "\uEBC4"},
      {""}, {""},
#line 1243 "material_font.gperf"
      {"mark_as_unread", "\uE9BC"},
      {""},
#line 1077 "material_font.gperf"
      {"invert_colors_on", "\uE891"},
      {""}, {""},
#line 298 "material_font.gperf"
      {"bedtime", "\uEF44"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1464 "material_font.gperf"
      {"paypal", "\uEA8D"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 776 "material_font.gperf"
      {"factory", "\uEBBC"},
      {""}, {""}, {""}, {""}, {""},
#line 1640 "material_font.gperf"
      {"recycling", "\uE760"},
      {""},
#line 1780 "material_font.gperf"
      {"settings_display", "\uE8BD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 339 "material_font.gperf"
      {"border_right", "\uE230"},
      {""},
#line 2132 "material_font.gperf"
      {"vaping_rooms", "\uEBCF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 993 "material_font.gperf"
      {"heat_pump", "\uEC18"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 336 "material_font.gperf"
      {"border_inner", "\uE22D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1545 "material_font.gperf"
      {"pinch", "\uEB38"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1983 "material_font.gperf"
      {"swipe_vertical", "\uEB51"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1476 "material_font.gperf"
      {"perm_data_setting", "\uE8A4"},
      {""},
#line 454 "material_font.gperf"
      {"cleaning_services", "\uF0FF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 341 "material_font.gperf"
      {"border_top", "\uE232"},
#line 2131 "material_font.gperf"
      {"vape_free", "\uEBC6"},
      {""}, {""}, {""}, {""},
#line 1538 "material_font.gperf"
      {"pie_chart", "\uE6C4"},
      {""}, {""}, {""}, {""},
#line 2047 "material_font.gperf"
      {"thumb_up_alt", "\uE817"},
      {""}, {""},
#line 77 "material_font.gperf"
      {"4g_mobiledata", "\uEFD2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 72 "material_font.gperf"
      {"3g_mobiledata", "\uEFD0"},
#line 1845 "material_font.gperf"
      {"skateboarding", "\uE511"},
      {""}, {""},
#line 1555 "material_font.gperf"
      {"play_disabled", "\uEF6A"},
      {""}, {""},
#line 1116 "material_font.gperf"
      {"label_important", "\uE937"},
      {""}, {""}, {""}, {""}, {""},
#line 2134 "material_font.gperf"
      {"verified_user", "\uE8E8"},
      {""}, {""},
#line 1589 "material_font.gperf"
      {"privacy_tip", "\uF0DC"},
#line 1996 "material_font.gperf"
      {"sync_problem", "\uE629"},
      {""}, {""}, {""}, {""},
#line 2016 "material_font.gperf"
      {"tap_and_play", "\uE62B"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 781 "material_font.gperf"
      {"favorite", "\uE87D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1516 "material_font.gperf"
      {"phonelink", "\uE326"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1334 "material_font.gperf"
      {"my_library_add", "\uE02E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1196 "material_font.gperf"
      {"location_disabled", "\uE1B6"},
      {""}, {""}, {""}, {""}, {""},
#line 1144 "material_font.gperf"
      {"light_mode", "\uE518"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2133 "material_font.gperf"
      {"verified", "\uEF76"},
      {""},
#line 1259 "material_font.gperf"
      {"medication_liquid", "\uEA87"},
      {""}, {""}, {""}, {""},
#line 2048 "material_font.gperf"
      {"thumb_up_off_alt", "\uE9F3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1507 "material_font.gperf"
      {"phone_callback", "\uE649"},
#line 1558 "material_font.gperf"
      {"playlist_add", "\uE03B"},
      {""}, {""},
#line 559 "material_font.gperf"
      {"dashboard_customize", "\uE99B"},
      {""}, {""}, {""}, {""},
#line 440 "material_font.gperf"
      {"check_circle_outline", "\uE92D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1809 "material_font.gperf"
      {"shopping_cart_checkout", "\uEB88"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 876 "material_font.gperf"
      {"font_download_off", "\uE4F9"},
      {""}, {""},
#line 1345 "material_font.gperf"
      {"near_me_disabled", "\uF1EF"},
#line 1511 "material_font.gperf"
      {"phone_in_talk", "\uE61D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2243 "material_font.gperf"
      {"wifi_channel", "\uEB6A"},
      {""}, {""}, {""},
#line 1051 "material_font.gperf"
      {"important_devices", "\uE912"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2185 "material_font.gperf"
      {"volcano", "\uEBDA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 426 "material_font.gperf"
      {"center_focus_weak", "\uE3B5"},
      {""}, {""}, {""}, {""},
#line 1651 "material_font.gperf"
      {"remove_from_queue", "\uE067"},
      {""}, {""}, {""}, {""}, {""},
#line 1307 "material_font.gperf"
      {"more_horiz", "\uE5D3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1291 "material_font.gperf"
      {"mode_night", "\uF036"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 798 "material_font.gperf"
      {"file_download", "\uE2C4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1942 "material_font.gperf"
      {"storm", "\uF070"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1180 "material_font.gperf"
      {"local_movies", "\uE54D"},
      {""}, {""}, {""}, {""},
#line 1415 "material_font.gperf"
      {"on_device_training", "\uEBFD"},
      {""}, {""}, {""}, {""},
#line 2137 "material_font.gperf"
      {"vertical_align_top", "\uE25A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 782 "material_font.gperf"
      {"favorite_border", "\uE87E"},
      {""}, {""},
#line 2136 "material_font.gperf"
      {"vertical_align_center", "\uE259"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1623 "material_font.gperf"
      {"radio_button_unchecked", "\uE836"},
      {""}, {""},
#line 2046 "material_font.gperf"
      {"thumb_up", "\uE8DC"},
      {""}, {""}, {""}, {""}, {""},
#line 579 "material_font.gperf"
      {"density_medium", "\uEB9E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2242 "material_font.gperf"
      {"wifi_calling_3", "\uF085"},
      {""}, {""}, {""},
#line 1000 "material_font.gperf"
      {"hide_image", "\uF022"},
      {""}, {""}, {""}, {""},
#line 645 "material_font.gperf"
      {"do_not_disturb_on_total_silence", "\uEFFB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 495 "material_font.gperf"
      {"connected_tv", "\uE998"},
      {""},
#line 644 "material_font.gperf"
      {"do_not_disturb_on", "\uE644"},
      {""},
#line 2263 "material_font.gperf"
      {"work_outline", "\uE943"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1300 "material_font.gperf"
      {"monitor_heart", "\uEAA2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1982 "material_font.gperf"
      {"swipe_up_alt", "\uEB35"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2075 "material_font.gperf"
      {"toys", "\uE332"},
      {""},
#line 901 "material_font.gperf"
      {"format_quote", "\uE244"},
      {""}, {""},
#line 1585 "material_font.gperf"
      {"price_check", "\uF04B"},
      {""}, {""}, {""}, {""}, {""},
#line 2241 "material_font.gperf"
      {"wifi_calling", "\uEF77"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 236 "material_font.gperf"
      {"assist_walker", "\uF8D5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 799 "material_font.gperf"
      {"file_download_done", "\uE9AA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1713 "material_font.gperf"
      {"safety_divider", "\uE1CC"},
      {""}, {""}, {""}, {""},
#line 785 "material_font.gperf"
      {"featured_play_list", "\uE06D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 2238 "material_font.gperf"
      {"wifi", "\uE63E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1118 "material_font.gperf"
      {"label_off", "\uE9B6"},
      {""}, {""}, {""}, {""}, {""},
#line 822 "material_font.gperf"
      {"filter_hdr", "\uE3DF"},
#line 833 "material_font.gperf"
      {"fire_hydrant", "\uF1A3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1186 "material_font.gperf"
      {"local_play", "\uE553"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2065 "material_font.gperf"
      {"today", "\uE8DF"},
      {""},
#line 848 "material_font.gperf"
      {"flashlight_off", "\uF00A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 505 "material_font.gperf"
      {"content_copy", "\uE14D"},
      {""},
#line 1979 "material_font.gperf"
      {"swipe_right", "\uEB52"},
      {""}, {""}, {""}, {""}, {""},
#line 1512 "material_font.gperf"
      {"phone_iphone", "\uE325"},
#line 783 "material_font.gperf"
      {"favorite_outline", "\uE87E"},
      {""}, {""}, {""},
#line 1217 "material_font.gperf"
      {"looks_two", "\uE401"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1967 "material_font.gperf"
      {"surround_sound", "\uE049"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1726 "material_font.gperf"
      {"schedule", "\uE8B5"},
      {""}, {""}, {""}, {""},
#line 597 "material_font.gperf"
      {"devices_fold", "\uEBDE"},
      {""},
#line 465 "material_font.gperf"
      {"cloud_download", "\uE2C0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 834 "material_font.gperf"
      {"fire_hydrant_alt", "\uF8F1"},
      {""}, {""}, {""}, {""},
#line 1980 "material_font.gperf"
      {"swipe_right_alt", "\uEB56"},
      {""}, {""},
#line 1786 "material_font.gperf"
      {"settings_input_svideo", "\uE8C3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1561 "material_font.gperf"
      {"playlist_add_circle", "\uE7E5"},
      {""},
#line 1578 "material_font.gperf"
      {"power_off", "\uE646"},
      {""}, {""},
#line 308 "material_font.gperf"
      {"block", "\uE14B"},
      {""}, {""}, {""}, {""}, {""},
#line 1364 "material_font.gperf"
      {"nightlife", "\uEA62"},
      {""}, {""},
#line 902 "material_font.gperf"
      {"format_shapes", "\uE25E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2093 "material_font.gperf"
      {"troubleshoot", "\uE1D2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 667 "material_font.gperf"
      {"download_for_offline", "\uF000"},
      {""},
#line 677 "material_font.gperf"
      {"drive_file_rename_outline", "\uE9A2"},
      {""}, {""}, {""},
#line 591 "material_font.gperf"
      {"developer_board_off", "\uE4FF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2265 "material_font.gperf"
      {"workspaces", "\uE1A0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1981 "material_font.gperf"
      {"swipe_up", "\uEB2E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1332 "material_font.gperf"
      {"music_off", "\uE440"},
      {""}, {""},
#line 1977 "material_font.gperf"
      {"swipe_left", "\uEB59"},
#line 1262 "material_font.gperf"
      {"menu", "\uE5D2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 356 "material_font.gperf"
      {"brightness_medium", "\uE1AE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1978 "material_font.gperf"
      {"swipe_left_alt", "\uEB33"},
      {""}, {""}, {""}, {""}, {""},
#line 1330 "material_font.gperf"
      {"museum", "\uEA36"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1010 "material_font.gperf"
      {"history_toggle_off", "\uF17D"},
      {""}, {""}, {""},
#line 1552 "material_font.gperf"
      {"play_circle_fill", "\uE038"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1139 "material_font.gperf"
      {"library_add", "\uE02E"},
#line 1016 "material_font.gperf"
      {"home_filled", "\uE9B2"},
#line 253 "material_font.gperf"
      {"auto_awesome_motion", "\uE661"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1571 "material_font.gperf"
      {"polymer", "\uE8AB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 299 "material_font.gperf"
      {"bedtime_off", "\uEB76"},
      {""},
#line 1834 "material_font.gperf"
      {"signal_wifi_connected_no_internet_4", "\uF064"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 517 "material_font.gperf"
      {"copy_all", "\uE2EC"},
      {""}, {""}, {""}, {""}, {""},
#line 1313 "material_font.gperf"
      {"motion_photos_on", "\uE9C1"},
      {""},
#line 1117 "material_font.gperf"
      {"label_important_outline", "\uE948"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2247 "material_font.gperf"
      {"wifi_password", "\uEB6B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2095 "material_font.gperf"
      {"tsunami", "\uEBD8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1653 "material_font.gperf"
      {"remove_red_eye", "\uE417"},
      {""},
#line 1076 "material_font.gperf"
      {"invert_colors_off", "\uE0C4"},
      {""}, {""}, {""},
#line 577 "material_font.gperf"
      {"delivery_dining", "\uEA72"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 498 "material_font.gperf"
      {"contact_emergency", "\uF8D1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1821 "material_font.gperf"
      {"signal_cellular_alt", "\uE202"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1823 "material_font.gperf"
      {"signal_cellular_alt_2_bar", "\uEBE3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1462 "material_font.gperf"
      {"payment", "\uE8A1"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1822 "material_font.gperf"
      {"signal_cellular_alt_1_bar", "\uEBDF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1463 "material_font.gperf"
      {"payments", "\uEF63"},
      {""}, {""}, {""}, {""},
#line 267 "material_font.gperf"
      {"backspace", "\uE14A"},
      {""}, {""}, {""}, {""}, {""},
#line 269 "material_font.gperf"
      {"backup_table", "\uEF43"},
      {""}, {""}, {""},
#line 1727 "material_font.gperf"
      {"schedule_send", "\uEA0A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1843 "material_font.gperf"
      {"single_bed", "\uEA48"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1374 "material_font.gperf"
      {"no_encryption", "\uE641"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1612 "material_font.gperf"
      {"quick_contacts_dialer", "\uE0CF"},
      {""}, {""}, {""}, {""},
#line 1197 "material_font.gperf"
      {"location_history", "\uE55A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2001 "material_font.gperf"
      {"system_update_alt", "\uE8D7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1347 "material_font.gperf"
      {"nearby_off", "\uF03C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1777 "material_font.gperf"
      {"settings_bluetooth", "\uE8BB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1174 "material_font.gperf"
      {"local_grocery_store", "\uE547"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 672 "material_font.gperf"
      {"draw", "\uE746"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 832 "material_font.gperf"
      {"fire_extinguisher", "\uF1D8"},
      {""},
#line 1278 "material_font.gperf"
      {"minor_crash", "\uEBF1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1570 "material_font.gperf"
      {"polyline", "\uEBBB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 156 "material_font.gperf"
      {"airline_seat_individual_suite", "\uE632"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1539 "material_font.gperf"
      {"pie_chart_outline", "\uF044"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 406 "material_font.gperf"
      {"candlestick_chart", "\uEAD4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 611 "material_font.gperf"
      {"directions_bus_filled", "\uEFF6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2244 "material_font.gperf"
      {"wifi_find", "\uEB31"},
      {""}, {""}, {""}, {""}, {""},
#line 2179 "material_font.gperf"
      {"villa", "\uE586"},
      {""}, {""}, {""}, {""}, {""},
#line 2266 "material_font.gperf"
      {"workspaces_filled", "\uEA0D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1609 "material_font.gperf"
      {"queue", "\uE03C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 643 "material_font.gperf"
      {"do_not_disturb_off", "\uE643"},
      {""}, {""},
#line 835 "material_font.gperf"
      {"fire_truck", "\uF8F2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 839 "material_font.gperf"
      {"fitbit", "\uE82B"},
      {""}, {""}, {""}, {""},
#line 1698 "material_font.gperf"
      {"roundabout_left", "\uEB99"},
      {""}, {""}, {""}, {""},
#line 800 "material_font.gperf"
      {"file_download_off", "\uE4FE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 268 "material_font.gperf"
      {"backup", "\uE864"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1304 "material_font.gperf"
      {"mood_bad", "\uE7F3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2104 "material_font.gperf"
      {"turn_slight_right", "\uEB9A"},
      {""}, {""}, {""}, {""}, {""},
#line 627 "material_font.gperf"
      {"disabled_visible", "\uE76E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 804 "material_font.gperf"
      {"file_upload_off", "\uF886"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 563 "material_font.gperf"
      {"data_saver_off", "\uEFF2"},
      {""}, {""},
#line 1540 "material_font.gperf"
      {"pie_chart_outlined", "\uE6C5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 738 "material_font.gperf"
      {"escalator_warning", "\uF1AC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1573 "material_font.gperf"
      {"portable_wifi_off", "\uE0CE"},
      {""}, {""},
#line 1847 "material_font.gperf"
      {"skip_previous", "\uE045"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 730 "material_font.gperf"
      {"energy_savings_leaf", "\uEC1A"},
#line 867 "material_font.gperf"
      {"folder_copy", "\uEBBD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1266 "material_font.gperf"
      {"merge_type", "\uE252"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 444 "material_font.gperf"
      {"chevron_left", "\uE5CB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 445 "material_font.gperf"
      {"chevron_right", "\uE5CC"},
      {""}, {""},
#line 1460 "material_font.gperf"
      {"pause_circle_outline", "\uE036"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2152 "material_font.gperf"
      {"video_stable", "\uF081"},
      {""},
#line 2143 "material_font.gperf"
      {"video_call", "\uE070"},
      {""}, {""}, {""},
#line 2205 "material_font.gperf"
      {"watch", "\uE334"},
      {""}, {""},
#line 1087 "material_font.gperf"
      {"kebab_dining", "\uE842"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1309 "material_font.gperf"
      {"more_vert", "\uE5D4"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 722 "material_font.gperf"
      {"emoji_events", "\uEA23"},
      {""}, {""},
#line 2178 "material_font.gperf"
      {"vignette", "\uE435"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 973 "material_font.gperf"
      {"hdr_auto", "\uF01A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1407 "material_font.gperf"
      {"notifications_paused", "\uE7F8"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1085 "material_font.gperf"
      {"join_right", "\uEAEA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1849 "material_font.gperf"
      {"slideshow", "\uE41B"},
      {""}, {""}, {""},
#line 1620 "material_font.gperf"
      {"radio_button_checked", "\uE837"},
      {""}, {""},
#line 974 "material_font.gperf"
      {"hdr_auto_select", "\uF01B"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1710 "material_font.gperf"
      {"running_with_errors", "\uE51D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 862 "material_font.gperf"
      {"flutter_dash", "\uE00B"},
      {""}, {""}, {""}, {""},
#line 78 "material_font.gperf"
      {"4g_plus_mobiledata", "\uEFD3"},
#line 1149 "material_font.gperf"
      {"line_style", "\uE919"},
      {""},
#line 242 "material_font.gperf"
      {"atm", "\uE573"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1693 "material_font.gperf"
      {"room_service", "\uEB49"},
      {""},
#line 317 "material_font.gperf"
      {"blur_circular", "\uE3A2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1945 "material_font.gperf"
      {"stream", "\uE9E9"},
      {""},
#line 318 "material_font.gperf"
      {"blur_linear", "\uE3A3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1119 "material_font.gperf"
      {"label_outline", "\uE893"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2113 "material_font.gperf"
      {"umbrella", "\uF1AD"},
      {""}, {""}, {""}, {""}, {""},
#line 865 "material_font.gperf"
      {"foggy", "\uE818"},
      {""}, {""},
#line 1353 "material_font.gperf"
      {"network_wifi", "\uE1BA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 642 "material_font.gperf"
      {"do_not_disturb_alt", "\uE611"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 320 "material_font.gperf"
      {"blur_on", "\uE3A5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1553 "material_font.gperf"
      {"play_circle_filled", "\uE038"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 494 "material_font.gperf"
      {"connect_without_contact", "\uF223"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1605 "material_font.gperf"
      {"query_builder", "\uE8AE"},
      {""},
#line 2228 "material_font.gperf"
      {"wechat", "\uEA81"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1065 "material_font.gperf"
      {"insert_page_break", "\uEACA"},
      {""},
#line 2246 "material_font.gperf"
      {"wifi_off", "\uE648"},
      {""}, {""},
#line 2151 "material_font.gperf"
      {"video_settings", "\uEA75"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 617 "material_font.gperf"
      {"directions_railway_filled", "\uEFF8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1350 "material_font.gperf"
      {"network_check", "\uE640"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2100 "material_font.gperf"
      {"turn_right", "\uEBAB"},
      {""}, {""}, {""}, {""},
#line 898 "material_font.gperf"
      {"format_list_numbered_rtl", "\uE267"},
      {""},
#line 886 "material_font.gperf"
      {"format_bold", "\uE238"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 795 "material_font.gperf"
      {"fiber_pin", "\uE06A"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1021 "material_font.gperf"
      {"horizontal_distribute", "\uE014"},
      {""}, {""},
#line 1312 "material_font.gperf"
      {"motion_photos_off", "\uE9C0"},
      {""}, {""}, {""},
#line 1296 "material_font.gperf"
      {"money", "\uE57D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1138 "material_font.gperf"
      {"lens_blur", "\uF029"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1875 "material_font.gperf"
      {"south", "\uF1E3"},
      {""},
#line 1032 "material_font.gperf"
      {"house", "\uEA44"},
      {""}, {""}, {""}, {""},
#line 614 "material_font.gperf"
      {"directions_ferry", "\uE532"},
      {""}, {""},
#line 2148 "material_font.gperf"
      {"video_file", "\uEB87"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1321 "material_font.gperf"
      {"movie", "\uE02C"},
      {""}, {""}, {""}, {""}, {""},
#line 897 "material_font.gperf"
      {"format_list_numbered", "\uE242"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 880 "material_font.gperf"
      {"fork_right", "\uEBAC"},
      {""}, {""}, {""}, {""},
#line 1521 "material_font.gperf"
      {"phonelink_setup", "\uE0DE"},
      {""}, {""}, {""}, {""},
#line 1114 "material_font.gperf"
      {"kitesurfing", "\uE50D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2051 "material_font.gperf"
      {"tiktok", "\uEA7E"},
#line 2262 "material_font.gperf"
      {"work_off", "\uE942"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1030 "material_font.gperf"
      {"hourglass_full", "\uE88C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1435 "material_font.gperf"
      {"pageview", "\uE8A0"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 169 "material_font.gperf"
      {"airplay", "\uE055"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1018 "material_font.gperf"
      {"home_mini", "\uF025"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2248 "material_font.gperf"
      {"wifi_protected_setup", "\uF0FC"},
      {""}, {""}, {""}, {""}, {""},
#line 1293 "material_font.gperf"
      {"mode_standby", "\uF037"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1142 "material_font.gperf"
      {"library_music", "\uE030"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 793 "material_font.gperf"
      {"fiber_manual_record", "\uE061"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1329 "material_font.gperf"
      {"multitrack_audio", "\uE1B8"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2147 "material_font.gperf"
      {"video_collection", "\uE04A"},
      {""}, {""}, {""},
#line 719 "material_font.gperf"
      {"emergency_recording", "\uEBF4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1835 "material_font.gperf"
      {"signal_wifi_off", "\uE1DA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2220 "material_font.gperf"
      {"wb_twighlight", "\uEA02"},
#line 1160 "material_font.gperf"
      {"living", "\uF02B"},
      {""}, {""},
#line 1081 "material_font.gperf"
      {"javascript", "\uEB7C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1322 "material_font.gperf"
      {"movie_creation", "\uE404"},
      {""}, {""}, {""}, {""}, {""},
#line 1127 "material_font.gperf"
      {"laptop_windows", "\uE321"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2117 "material_font.gperf"
      {"unfold_less_double", "\uF8CF"},
      {""}, {""},
#line 691 "material_font.gperf"
      {"edgesensor_low", "\uF006"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 858 "material_font.gperf"
      {"flip_to_back", "\uE882"},
      {""}, {""}, {""},
#line 407 "material_font.gperf"
      {"car_crash", "\uEBF2"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1611 "material_font.gperf"
      {"queue_play_next", "\uE066"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2102 "material_font.gperf"
      {"turn_sharp_right", "\uEBAA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1335 "material_font.gperf"
      {"my_library_books", "\uE02F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 258 "material_font.gperf"
      {"auto_graph", "\uE4FB"},
      {""}, {""}, {""},
#line 1711 "material_font.gperf"
      {"rv_hookup", "\uE642"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2184 "material_font.gperf"
      {"voicemail", "\uE0D9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1747 "material_font.gperf"
      {"search", "\uE8B6"},
      {""},
#line 1370 "material_font.gperf"
      {"no_backpack", "\uF237"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1324 "material_font.gperf"
      {"movie_filter", "\uE43A"},
      {""}, {""}, {""}, {""}, {""},
#line 379 "material_font.gperf"
      {"calendar_month", "\uEBCC"},
      {""}, {""}, {""}, {""}, {""},
#line 843 "material_font.gperf"
      {"flaky", "\uEF50"},
      {""}, {""},
#line 1601 "material_font.gperf"
      {"push_pin", "\uF10D"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 353 "material_font.gperf"
      {"brightness_auto", "\uE1AB"},
      {""}, {""}, {""}, {""}, {""},
#line 323 "material_font.gperf"
      {"book_online", "\uF217"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1325 "material_font.gperf"
      {"moving", "\uE501"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1022 "material_font.gperf"
      {"horizontal_rule", "\uF108"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2031 "material_font.gperf"
      {"text_rotate_vertical", "\uE93B"},
      {""},
#line 635 "material_font.gperf"
      {"dnd_forwardslash", "\uE611"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 359 "material_font.gperf"
      {"broken_image", "\uE3AD"},
      {""}, {""}, {""}, {""},
#line 1086 "material_font.gperf"
      {"kayaking", "\uE50C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1054 "material_font.gperf"
      {"indeterminate_check_box", "\uE909"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 140 "material_font.gperf"
      {"add_shopping_cart", "\uE854"},
      {""}, {""}, {""}, {""}, {""},
#line 1361 "material_font.gperf"
      {"next_week", "\uE16A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1323 "material_font.gperf"
      {"movie_edit", "\uF840"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1955 "material_font.gperf"
      {"subtitles", "\uE048"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 483 "material_font.gperf"
      {"comments_disabled", "\uE7A2"},
      {""},
#line 1523 "material_font.gperf"
      {"photo_album", "\uE411"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 478 "material_font.gperf"
      {"collections_bookmark", "\uE431"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1518 "material_font.gperf"
      {"phonelink_lock", "\uE0DC"},
#line 983 "material_font.gperf"
      {"headphones", "\uF01F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 371 "material_font.gperf"
      {"bus_alert", "\uE98F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1694 "material_font.gperf"
      {"rotate_90_degrees_ccw", "\uE418"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1666 "material_font.gperf"
      {"replay_circle_filled", "\uE9D8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1866 "material_font.gperf"
      {"snowshoeing", "\uE514"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 626 "material_font.gperf"
      {"disabled_by_default", "\uF230"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2008 "material_font.gperf"
      {"table_rows", "\uF101"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1909 "material_font.gperf"
      {"sports_rugby", "\uEA2E"},
#line 2235 "material_font.gperf"
      {"width_full", "\uF8F5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1031 "material_font.gperf"
      {"hourglass_top", "\uEA5B"},
      {""}, {""}, {""}, {""}, {""},
#line 1477 "material_font.gperf"
      {"perm_device_info", "\uE8A5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 828 "material_font.gperf"
      {"filter_vintage", "\uE3E3"},
      {""}, {""},
#line 2260 "material_font.gperf"
      {"work", "\uE8F9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1033 "material_font.gperf"
      {"house_siding", "\uF202"},
      {""}, {""}, {""}, {""},
#line 344 "material_font.gperf"
      {"branding_watermark", "\uE06B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1425 "material_font.gperf"
      {"outbond", "\uF228"},
      {""}, {""},
#line 2135 "material_font.gperf"
      {"vertical_align_bottom", "\uE258"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1607 "material_font.gperf"
      {"question_answer", "\uE8AF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1017 "material_font.gperf"
      {"home_max", "\uF024"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 370 "material_font.gperf"
      {"burst_mode", "\uE43C"},
      {""}, {""}, {""},
#line 1019 "material_font.gperf"
      {"home_repair_service", "\uF100"},
      {""},
#line 1497 "material_font.gperf"
      {"person_search", "\uF106"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 468 "material_font.gperf"
      {"cloud_sync", "\uEB5A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 110 "material_font.gperf"
      {"accessible_forward", "\uE934"},
#line 2224 "material_font.gperf"
      {"web_asset", "\uE069"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1283 "material_font.gperf"
      {"mobile_off", "\uE201"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 899 "material_font.gperf"
      {"format_overline", "\uEB65"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 982 "material_font.gperf"
      {"hdr_weak", "\uE3F2"},
      {""}, {""}, {""},
#line 1805 "material_font.gperf"
      {"shopify", "\uEA9D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1007 "material_font.gperf"
      {"hiking", "\uE50A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 366 "material_font.gperf"
      {"bug_report", "\uE868"},
#line 372 "material_font.gperf"
      {"business", "\uE0AF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1596 "material_font.gperf"
      {"public", "\uE80B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1953 "material_font.gperf"
      {"subscript", "\uF111"},
      {""}, {""}, {""},
#line 1678 "material_font.gperf"
      {"restaurant_menu", "\uE561"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1954 "material_font.gperf"
      {"subscriptions", "\uE064"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 2009 "material_font.gperf"
      {"table_view", "\uF1BE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 373 "material_font.gperf"
      {"business_center", "\uEB3F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 175 "material_font.gperf"
      {"album", "\uE019"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 367 "material_font.gperf"
      {"build", "\uE869"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 2023 "material_font.gperf"
      {"temple_hindu", "\uEAAF"},
      {""}, {""}, {""}, {""}, {""},
#line 796 "material_font.gperf"
      {"fiber_smart_record", "\uE062"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1705 "material_font.gperf"
      {"rsvp", "\uF055"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1478 "material_font.gperf"
      {"perm_device_information", "\uE8A5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 338 "material_font.gperf"
      {"border_outer", "\uE22F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1009 "material_font.gperf"
      {"history_edu", "\uEA3E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2155 "material_font.gperf"
      {"videogame_asset", "\uE338"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 275 "material_font.gperf"
      {"bar_chart", "\uE26B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 302 "material_font.gperf"
      {"bike_scooter", "\uEF45"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2006 "material_font.gperf"
      {"table_chart", "\uE265"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2226 "material_font.gperf"
      {"web_stories", "\uE595"},
#line 1950 "material_font.gperf"
      {"subdirectory_arrow_left", "\uE5D9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1001 "material_font.gperf"
      {"hide_source", "\uF023"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1035 "material_font.gperf"
      {"how_to_reg", "\uE174"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 319 "material_font.gperf"
      {"blur_off", "\uE3A4"},
      {""}, {""},
#line 2212 "material_font.gperf"
      {"waves", "\uE176"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1608 "material_font.gperf"
      {"question_mark", "\uEB8B"},
      {""}, {""},
#line 637 "material_font.gperf"
      {"do_disturb", "\uF08C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 728 "material_font.gperf"
      {"emoji_symbols", "\uEA1E"},
      {""}, {""}, {""}, {""}, {""},
#line 1059 "material_font.gperf"
      {"insert_chart_outlined", "\uE26A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 470 "material_font.gperf"
      {"cloudy_snowing", "\uE810"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 368 "material_font.gperf"
      {"build_circle", "\uEF48"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1750 "material_font.gperf"
      {"security_update", "\uF058"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 726 "material_font.gperf"
      {"emoji_objects", "\uEA24"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 382 "material_font.gperf"
      {"calendar_view_month", "\uEFE7"},
      {""}, {""},
#line 401 "material_font.gperf"
      {"cameraswitch", "\uEFEB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1254 "material_font.gperf"
      {"media_bluetooth_on", "\uF032"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1595 "material_font.gperf"
      {"psychology_alt", "\uF8EA"},
      {""}, {""}, {""}, {""},
#line 1089 "material_font.gperf"
      {"key_off", "\uEB84"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1027 "material_font.gperf"
      {"hourglass_bottom", "\uEA5C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 744 "material_font.gperf"
      {"event_busy", "\uE615"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1336 "material_font.gperf"
      {"my_library_music", "\uE030"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1372 "material_font.gperf"
      {"no_crash", "\uEBF0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1319 "material_font.gperf"
      {"move_to_inbox", "\uE168"},
      {""}, {""},
#line 1167 "material_font.gperf"
      {"local_car_wash", "\uE542"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1285 "material_font.gperf"
      {"mobiledata_off", "\uF034"},
      {""}, {""}, {""}, {""},
#line 1951 "material_font.gperf"
      {"subdirectory_arrow_right", "\uE5DA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1959 "material_font.gperf"
      {"sunny", "\uE81A"},
#line 1112 "material_font.gperf"
      {"king_bed", "\uEA45"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1238 "material_font.gperf"
      {"manage_search", "\uF02F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1583 "material_font.gperf"
      {"preview", "\uF1C5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1946 "material_font.gperf"
      {"streetview", "\uE56E"},
#line 1770 "material_font.gperf"
      {"sentiment_very_dissatisfied", "\uE814"},
      {""}, {""},
#line 1628 "material_font.gperf"
      {"rate_review", "\uE560"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2119 "material_font.gperf"
      {"unfold_more_double", "\uF8D0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 550 "material_font.gperf"
      {"currency_rupee", "\uEAF7"},
#line 549 "material_font.gperf"
      {"currency_ruble", "\uEAEC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 551 "material_font.gperf"
      {"currency_yen", "\uEAFB"},
      {""},
#line 547 "material_font.gperf"
      {"currency_lira", "\uEAEF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2154 "material_font.gperf"
      {"videocam_off", "\uE04C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 546 "material_font.gperf"
      {"currency_franc", "\uEAFA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 552 "material_font.gperf"
      {"currency_yuan", "\uEAF9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 991 "material_font.gperf"
      {"hearing_disabled", "\uF104"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 205 "material_font.gperf"
      {"archive", "\uE149"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 241 "material_font.gperf"
      {"assured_workload", "\uEB6F"},
      {""},
#line 1579 "material_font.gperf"
      {"power_settings_new", "\uE8AC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1682 "material_font.gperf"
      {"reviews", "\uF054"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1919 "material_font.gperf"
      {"stadium", "\uEB90"},
      {""}, {""}, {""},
#line 1962 "material_font.gperf"
      {"supervised_user_circle", "\uE939"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2186 "material_font.gperf"
      {"volume_down", "\uE04D"},
#line 690 "material_font.gperf"
      {"edgesensor_high", "\uF005"},
#line 2145 "material_font.gperf"
      {"video_camera_front", "\uF080"},
      {""}, {""},
#line 488 "material_font.gperf"
      {"compass_calibration", "\uE57C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 246 "material_font.gperf"
      {"attachment", "\uE2BC"},
      {""}, {""},
#line 2187 "material_font.gperf"
      {"volume_down_alt", "\uE79C"},
      {""}, {""}, {""}, {""}, {""},
#line 2194 "material_font.gperf"
      {"vpn_lock", "\uE62F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1721 "material_font.gperf"
      {"saved_search", "\uEA11"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1963 "material_font.gperf"
      {"supervisor_account", "\uE8D3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1410 "material_font.gperf"
      {"numbers", "\uEAC7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2264 "material_font.gperf"
      {"workspace_premium", "\uE7AF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2253 "material_font.gperf"
      {"wind_power", "\uEC0C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 548 "material_font.gperf"
      {"currency_pound", "\uEAF1"},
      {""}, {""}, {""}, {""},
#line 678 "material_font.gperf"
      {"drive_folder_upload", "\uE9A3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 740 "material_font.gperf"
      {"euro_symbol", "\uE926"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 2232 "material_font.gperf"
      {"wheelchair_pickup", "\uF1AB"},
#line 1918 "material_font.gperf"
      {"stacked_line_chart", "\uF22B"},
      {""}, {""}, {""}, {""}, {""},
#line 2003 "material_font.gperf"
      {"tab", "\uE8D8"},
#line 2022 "material_font.gperf"
      {"temple_buddhist", "\uEAB3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1411 "material_font.gperf"
      {"offline_bolt", "\uE932"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2231 "material_font.gperf"
      {"whatshot", "\uE80E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 998 "material_font.gperf"
      {"hevc", "\uF021"},
#line 381 "material_font.gperf"
      {"calendar_view_day", "\uE936"},
#line 588 "material_font.gperf"
      {"desktop_windows", "\uE30C"},
      {""}, {""},
#line 1550 "material_font.gperf"
      {"play_arrow", "\uE037"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 2044 "material_font.gperf"
      {"thumb_down_alt", "\uE816"},
      {""},
#line 1956 "material_font.gperf"
      {"subtitles_off", "\uEF72"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2043 "material_font.gperf"
      {"thumb_down", "\uE8DB"},
      {""},
#line 1427 "material_font.gperf"
      {"outbox", "\uEF5F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1177 "material_font.gperf"
      {"local_laundry_service", "\uE54A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1253 "material_font.gperf"
      {"media_bluetooth_off", "\uF031"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 357 "material_font.gperf"
      {"broadcast_on_home", "\uF8F8"},
      {""}, {""}, {""}, {""}, {""},
#line 732 "material_font.gperf"
      {"enhance_photo_translate", "\uE8FC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 544 "material_font.gperf"
      {"currency_bitcoin", "\uEBC5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2270 "material_font.gperf"
      {"wysiwyg", "\uF1C3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1039 "material_font.gperf"
      {"https", "\uE88D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 252 "material_font.gperf"
      {"auto_awesome_mosaic", "\uE660"},
      {""}, {""},
#line 137 "material_font.gperf"
      {"add_photo_alternate", "\uE43E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2002 "material_font.gperf"
      {"system_update_tv", "\uE8D7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2109 "material_font.gperf"
      {"two_wheeler", "\uE9F9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2153 "material_font.gperf"
      {"videocam", "\uE04B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1376 "material_font.gperf"
      {"no_flash", "\uF1A6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2267 "material_font.gperf"
      {"workspaces_outline", "\uEA0F"},
      {""}, {""}, {""}, {""},
#line 1311 "material_font.gperf"
      {"motion_photos_auto", "\uF03A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 874 "material_font.gperf"
      {"follow_the_signs", "\uF222"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2156 "material_font.gperf"
      {"videogame_asset_off", "\uE500"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 322 "material_font.gperf"
      {"book", "\uE865"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2225 "material_font.gperf"
      {"web_asset_off", "\uE4F7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 330 "material_font.gperf"
      {"bookmarks", "\uE98B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 528 "material_font.gperf"
      {"crib", "\uE588"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1976 "material_font.gperf"
      {"swipe_down_alt", "\uEB30"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1975 "material_font.gperf"
      {"swipe_down", "\uEB53"},
#line 1613 "material_font.gperf"
      {"quick_contacts_mail", "\uE0D0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2081 "material_font.gperf"
      {"transfer_within_a_station", "\uE572"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1988 "material_font.gperf"
      {"switch_left", "\uF1D1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 383 "material_font.gperf"
      {"calendar_view_week", "\uEFE8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1135 "material_font.gperf"
      {"leave_bags_at_home", "\uF21B"},
      {""}, {""}, {""}, {""},
#line 1248 "material_font.gperf"
      {"mark_unread_chat_alt", "\uEB9D"},
#line 659 "material_font.gperf"
      {"door_back", "\uEFFC"},
      {""}, {""},
#line 1597 "material_font.gperf"
      {"public_off", "\uF1CA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1987 "material_font.gperf"
      {"switch_camera", "\uE41E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1038 "material_font.gperf"
      {"http", "\uE902"},
      {""},
#line 1986 "material_font.gperf"
      {"switch_account", "\uE9ED"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 325 "material_font.gperf"
      {"bookmark_add", "\uE598"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 827 "material_font.gperf"
      {"filter_tilt_shift", "\uE3E2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 327 "material_font.gperf"
      {"bookmark_border", "\uE867"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2198 "material_font.gperf"
      {"wallet_membership", "\uE8F7"},
      {""}, {""},
#line 777 "material_font.gperf"
      {"family_restroom", "\uF1A2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 653 "material_font.gperf"
      {"domain_verification", "\uEF4C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1443 "material_font.gperf"
      {"panorama_fisheye", "\uE40C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1752 "material_font.gperf"
      {"security_update_warning", "\uF05A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 969 "material_font.gperf"
      {"handshake", "\uEBCB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1280 "material_font.gperf"
      {"missed_video_call", "\uE073"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 482 "material_font.gperf"
      {"comment_bank", "\uEA4E"},
      {""},
#line 326 "material_font.gperf"
      {"bookmark_added", "\uE599"},
#line 2181 "material_font.gperf"
      {"visibility_off", "\uE8F5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1301 "material_font.gperf"
      {"monitor_weight", "\uF039"},
      {""}, {""},
#line 117 "material_font.gperf"
      {"adb", "\uE60E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1610 "material_font.gperf"
      {"queue_music", "\uE03D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1989 "material_font.gperf"
      {"switch_right", "\uF1D2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1758 "material_font.gperf"
      {"send_and_archive", "\uEA0C"},
      {""}, {""}, {""}, {""}, {""},
#line 328 "material_font.gperf"
      {"bookmark_outline", "\uE867"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 713 "material_font.gperf"
      {"electric_rickshaw", "\uEB1E"},
#line 1363 "material_font.gperf"
      {"night_shelter", "\uF1F1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1969 "material_font.gperf"
      {"swap_horiz", "\uE8D4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1922 "material_font.gperf"
      {"star_border", "\uE83A"},
      {""}, {""}, {""},
#line 1220 "material_font.gperf"
      {"low_priority", "\uE16D"},
      {""}, {""}, {""},
#line 1683 "material_font.gperf"
      {"rice_bowl", "\uF1F5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2255 "material_font.gperf"
      {"wine_bar", "\uF1E8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2120 "material_font.gperf"
      {"unpublished", "\uF236"},
      {""}, {""}, {""}, {""},
#line 244 "material_font.gperf"
      {"attach_file", "\uE226"},
      {""},
#line 1952 "material_font.gperf"
      {"subject", "\uE8D2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 510 "material_font.gperf"
      {"content_paste_search", "\uEA9B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1684 "material_font.gperf"
      {"ring_volume", "\uE0D1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2245 "material_font.gperf"
      {"wifi_lock", "\uE1E1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1365 "material_font.gperf"
      {"nightlight", "\uF03D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1525 "material_font.gperf"
      {"photo_camera_back", "\uEF68"},
      {""}, {""}, {""}, {""}, {""},
#line 273 "material_font.gperf"
      {"balcony", "\uE58F"},
      {""}, {""}, {""},
#line 1970 "material_font.gperf"
      {"swap_horizontal_circle", "\uE933"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1937 "material_font.gperf"
      {"stop_screen_share", "\uE0E3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 967 "material_font.gperf"
      {"h_plus_mobiledata", "\uF019"},
      {""}, {""}, {""},
#line 1037 "material_font.gperf"
      {"html", "\uEB7E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 248 "material_font.gperf"
      {"attribution", "\uEFDB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2189 "material_font.gperf"
      {"volume_off", "\uE04F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 243 "material_font.gperf"
      {"attach_email", "\uEA5E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1240 "material_font.gperf"
      {"maps_home_work", "\uF030"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1556 "material_font.gperf"
      {"play_for_work", "\uE906"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 877 "material_font.gperf"
      {"food_bank", "\uF1F2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1223 "material_font.gperf"
      {"lte_plus_mobiledata", "\uF02D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1320 "material_font.gperf"
      {"move_up", "\uEB64"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1924 "material_font.gperf"
      {"star_half", "\uE839"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 262 "material_font.gperf"
      {"autorenew", "\uE863"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 266 "material_font.gperf"
      {"backpack", "\uF19C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2250 "material_font.gperf"
      {"wifi_tethering_error", "\uEAD9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 915 "material_font.gperf"
      {"forward_to_inbox", "\uF187"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2182 "material_font.gperf"
      {"voice_chat", "\uE62E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 303 "material_font.gperf"
      {"biotech", "\uEA3A"},
      {""}, {""}, {""}, {""},
#line 1129 "material_font.gperf"
      {"launch", "\uE895"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1614 "material_font.gperf"
      {"quickreply", "\uEF6C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1897 "material_font.gperf"
      {"sports_basketball", "\uEA26"},
      {""}, {""}, {""}, {""}, {""},
#line 1560 "material_font.gperf"
      {"playlist_add_check_circle", "\uE7E6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1554 "material_font.gperf"
      {"play_circle_outline", "\uE039"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 2249 "material_font.gperf"
      {"wifi_tethering", "\uE1E2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 2171 "material_font.gperf"
      {"view_list", "\uE8EF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 743 "material_font.gperf"
      {"event_available", "\uE614"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1830 "material_font.gperf"
      {"signal_wifi_0_bar", "\uF0B0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2251 "material_font.gperf"
      {"wifi_tethering_error_rounded", "\uF086"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2157 "material_font.gperf"
      {"view_agenda", "\uE8E9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1831 "material_font.gperf"
      {"signal_wifi_4_bar", "\uE1D8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 2159 "material_font.gperf"
      {"view_carousel", "\uE8EB"},
      {""}, {""},
#line 1949 "material_font.gperf"
      {"style", "\uE41D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1675 "material_font.gperf"
      {"reset_tv", "\uE9D9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2174 "material_font.gperf"
      {"view_sidebar", "\uF114"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1014 "material_font.gperf"
      {"holiday_village", "\uE58A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 265 "material_font.gperf"
      {"back_hand", "\uE764"},
      {""}, {""},
#line 2192 "material_font.gperf"
      {"vpn_key", "\uE0DA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1686 "material_font.gperf"
      {"rocket_launch", "\uEB9B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1833 "material_font.gperf"
      {"signal_wifi_bad", "\uF063"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 278 "material_font.gperf"
      {"bathroom", "\uEFDD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1763 "material_font.gperf"
      {"sensor_window", "\uF1B4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 904 "material_font.gperf"
      {"format_strikethrough", "\uE246"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2094 "material_font.gperf"
      {"try", "\uF07C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2240 "material_font.gperf"
      {"wifi_2_bar", "\uE4D9"},
      {""}, {""}, {""}, {""}, {""},
#line 1699 "material_font.gperf"
      {"roundabout_right", "\uEBA3"},
      {""},
#line 2169 "material_font.gperf"
      {"view_in_ar", "\uE9FE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2239 "material_font.gperf"
      {"wifi_1_bar", "\uE4CA"},
#line 641 "material_font.gperf"
      {"do_not_disturb", "\uE612"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1850 "material_font.gperf"
      {"slow_motion_video", "\uE068"},
      {""},
#line 1318 "material_font.gperf"
      {"move_down", "\uEB61"},
      {""}, {""}, {""},
#line 335 "material_font.gperf"
      {"border_horizontal", "\uE22C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 518 "material_font.gperf"
      {"copyright", "\uE90C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 421 "material_font.gperf"
      {"category", "\uE574"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 718 "material_font.gperf"
      {"emergency", "\uE1EB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 292 "material_font.gperf"
      {"battery_unknown", "\uE1A6"},
      {""}, {""}, {""}, {""}, {""},
#line 679 "material_font.gperf"
      {"dry", "\uF1B3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1971 "material_font.gperf"
      {"swap_vert", "\uE8D5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1506 "material_font.gperf"
      {"phone_bluetooth_speaker", "\uE61B"},
      {""}, {""}, {""}, {""}, {""},
#line 1011 "material_font.gperf"
      {"hive", "\uEAA6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1638 "material_font.gperf"
      {"record_voice_over", "\uE91F"},
      {""}, {""}, {""},
#line 2138 "material_font.gperf"
      {"vertical_distribute", "\uE076"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1972 "material_font.gperf"
      {"swap_vert_circle", "\uE8D6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 342 "material_font.gperf"
      {"border_vertical", "\uE233"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1854 "material_font.gperf"
      {"smart_toy", "\uF06C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1426 "material_font.gperf"
      {"outbound", "\uE1CA"},
      {""}, {""},
#line 1934 "material_font.gperf"
      {"sticky_note_2", "\uF1FC"},
      {""},
#line 1973 "material_font.gperf"
      {"swap_vertical_circle", "\uE8D6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1930 "material_font.gperf"
      {"stay_current_landscape", "\uE0D3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 380 "material_font.gperf"
      {"calendar_today", "\uE935"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 895 "material_font.gperf"
      {"format_list_bulleted", "\uE241"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1836 "material_font.gperf"
      {"signal_wifi_statusbar_4_bar", "\uF065"},
      {""},
#line 1838 "material_font.gperf"
      {"signal_wifi_statusbar_null", "\uF067"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 362 "material_font.gperf"
      {"browser_updated", "\uE7CF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1125 "material_font.gperf"
      {"laptop_chromebook", "\uE31F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 896 "material_font.gperf"
      {"format_list_bulleted_add", "\uF849"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2146 "material_font.gperf"
      {"video_chat", "\uF8A0"},
      {""}, {""}, {""},
#line 2204 "material_font.gperf"
      {"wash", "\uF1B1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1837 "material_font.gperf"
      {"signal_wifi_statusbar_connected_no_internet_4", "\uF066"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 364 "material_font.gperf"
      {"brush", "\uE3AE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1302 "material_font.gperf"
      {"monochrome_photos", "\uE403"},
      {""}, {""}, {""}, {""},
#line 2252 "material_font.gperf"
      {"wifi_tethering_off", "\uF087"},
      {""}, {""}, {""}, {""},
#line 1008 "material_font.gperf"
      {"history", "\uE889"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1141 "material_font.gperf"
      {"library_books", "\uE02F"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1074 "material_font.gperf"
      {"inventory_2", "\uE1A1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 674 "material_font.gperf"
      {"drive_file_move", "\uE675"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 676 "material_font.gperf"
      {"drive_file_move_rtl", "\uE76D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1423 "material_font.gperf"
      {"open_with", "\uE89F"},
      {""},
#line 2129 "material_font.gperf"
      {"usb_off", "\uE4FA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1852 "material_font.gperf"
      {"smart_display", "\uF06A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1662 "material_font.gperf"
      {"replay", "\uE042"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1563 "material_font.gperf"
      {"playlist_remove", "\uEB80"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 324 "material_font.gperf"
      {"bookmark", "\uE866"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1367 "material_font.gperf"
      {"nights_stay", "\uEA46"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 239 "material_font.gperf"
      {"assistant_navigation", "\uE989"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1158 "material_font.gperf"
      {"live_help", "\uE0C6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2175 "material_font.gperf"
      {"view_stream", "\uE8F2"},
      {""}, {""},
#line 1275 "material_font.gperf"
      {"microwave", "\uF204"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2176 "material_font.gperf"
      {"view_timeline", "\uEB85"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2045 "material_font.gperf"
      {"thumb_down_off_alt", "\uE9F2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 515 "material_font.gperf"
      {"conveyor_belt", "\uF867"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 411 "material_font.gperf"
      {"card_membership", "\uE8F7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 361 "material_font.gperf"
      {"browser_not_supported", "\uEF47"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1209 "material_font.gperf"
      {"logo_dev", "\uEAD6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1284 "material_font.gperf"
      {"mobile_screen_share", "\uE0E7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1712 "material_font.gperf"
      {"safety_check", "\uEBEF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 312 "material_font.gperf"
      {"bluetooth_audio", "\uE60F"},
      {""}, {""}, {""}, {""},
#line 2173 "material_font.gperf"
      {"view_quilt", "\uE8F1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2142 "material_font.gperf"
      {"vibration", "\uE62D"},
      {""}, {""}, {""}, {""}, {""},
#line 675 "material_font.gperf"
      {"drive_file_move_outline", "\uE9A1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1221 "material_font.gperf"
      {"loyalty", "\uE89A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1355 "material_font.gperf"
      {"network_wifi_2_bar", "\uEBD6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1375 "material_font.gperf"
      {"no_encryption_gmailerrorred", "\uF03F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1354 "material_font.gperf"
      {"network_wifi_1_bar", "\uEBE4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 663 "material_font.gperf"
      {"double_arrow", "\uEA50"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1356 "material_font.gperf"
      {"network_wifi_3_bar", "\uEBE1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2149 "material_font.gperf"
      {"video_label", "\uE071"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1424 "material_font.gperf"
      {"other_houses", "\uE58C"},
      {""}, {""},
#line 2164 "material_font.gperf"
      {"view_compact", "\uE42B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1161 "material_font.gperf"
      {"local_activity", "\uE53F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 2165 "material_font.gperf"
      {"view_compact_alt", "\uEB74"},
#line 595 "material_font.gperf"
      {"device_unknown", "\uE339"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 363 "material_font.gperf"
      {"brunch_dining", "\uEA73"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 355 "material_font.gperf"
      {"brightness_low", "\uE1AD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1559 "material_font.gperf"
      {"playlist_add_check", "\uE065"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2114 "material_font.gperf"
      {"unarchive", "\uE169"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 733 "material_font.gperf"
      {"enhanced_encryption", "\uE63F"},
      {""}, {""}, {""},
#line 724 "material_font.gperf"
      {"emoji_food_beverage", "\uEA1B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1997 "material_font.gperf"
      {"system_security_update", "\uF072"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2070 "material_font.gperf"
      {"tonality", "\uE427"},
      {""}, {""}, {""}, {""},
#line 1314 "material_font.gperf"
      {"motion_photos_pause", "\uF227"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2190 "material_font.gperf"
      {"volume_up", "\uE050"},
      {""}, {""}, {""},
#line 1771 "material_font.gperf"
      {"sentiment_very_satisfied", "\uE815"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1598 "material_font.gperf"
      {"publish", "\uE255"},
      {""},
#line 1998 "material_font.gperf"
      {"system_security_update_good", "\uF073"},
      {""}, {""}, {""},
#line 1034 "material_font.gperf"
      {"houseboat", "\uE584"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2049 "material_font.gperf"
      {"thumbs_up_down", "\uE8DD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1024 "material_font.gperf"
      {"hot_tub", "\uEB46"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1195 "material_font.gperf"
      {"location_city", "\uE7F1"},
      {""}, {""}, {""}, {""}, {""},
#line 2163 "material_font.gperf"
      {"view_comfy_alt", "\uEB73"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1999 "material_font.gperf"
      {"system_security_update_warning", "\uF074"},
#line 1159 "material_font.gperf"
      {"live_tv", "\uE639"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2254 "material_font.gperf"
      {"window", "\uF088"},
      {""}, {""},
#line 1315 "material_font.gperf"
      {"motion_photos_paused", "\uE9C2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1091 "material_font.gperf"
      {"keyboard_alt", "\uF028"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1106 "material_font.gperf"
      {"keyboard_hide", "\uE31A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1111 "material_font.gperf"
      {"keyboard_voice", "\uE31D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 449 "material_font.gperf"
      {"church", "\uEAAE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 792 "material_font.gperf"
      {"fiber_dvr", "\uE05D"},
      {""}, {""}, {""},
#line 1090 "material_font.gperf"
      {"keyboard", "\uE312"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1100 "material_font.gperf"
      {"keyboard_control", "\uE5D3"},
      {""}, {""},
#line 1140 "material_font.gperf"
      {"library_add_check", "\uE9B7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1568 "material_font.gperf"
      {"policy", "\uEA17"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 594 "material_font.gperf"
      {"device_thermostat", "\uE1FF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 255 "material_font.gperf"
      {"auto_fix_high", "\uE663"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1263 "material_font.gperf"
      {"menu_book", "\uEA19"},
      {""},
#line 616 "material_font.gperf"
      {"directions_railway", "\uE534"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1832 "material_font.gperf"
      {"signal_wifi_4_bar_lock", "\uE1D9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 332 "material_font.gperf"
      {"border_bottom", "\uE229"},
      {""}, {""}, {""},
#line 957 "material_font.gperf"
      {"grid_view", "\uE9B0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 633 "material_font.gperf"
      {"diversity_2", "\uF8D8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 632 "material_font.gperf"
      {"diversity_1", "\uF8D7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1751 "material_font.gperf"
      {"security_update_good", "\uF059"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 634 "material_font.gperf"
      {"diversity_3", "\uF8D9"},
#line 1984 "material_font.gperf"
      {"switch_access_shortcut", "\uE7E1"},
#line 2188 "material_font.gperf"
      {"volume_mute", "\uE04E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1932 "material_font.gperf"
      {"stay_primary_landscape", "\uE0D5"},
      {""}, {""}, {""}, {""}, {""},
#line 1599 "material_font.gperf"
      {"published_with_changes", "\uF232"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1107 "material_font.gperf"
      {"keyboard_option", "\uEADF"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 271 "material_font.gperf"
      {"bakery_dining", "\uEA53"},
      {""},
#line 1479 "material_font.gperf"
      {"perm_identity", "\uE8A6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1985 "material_font.gperf"
      {"switch_access_shortcut_add", "\uE7E2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2223 "material_font.gperf"
      {"web", "\uE051"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1088 "material_font.gperf"
      {"key", "\uE73C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1366 "material_font.gperf"
      {"nightlight_round", "\uEF5E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1028 "material_font.gperf"
      {"hourglass_disabled", "\uEF53"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2039 "material_font.gperf"
      {"theater_comedy", "\uEA66"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 620 "material_font.gperf"
      {"directions_subway_filled", "\uEFF9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1742 "material_font.gperf"
      {"scuba_diving", "\uEBCE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2213 "material_font.gperf"
      {"waving_hand", "\uE766"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1020 "material_font.gperf"
      {"home_work", "\uEA09"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 2160 "material_font.gperf"
      {"view_column", "\uE8EC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1261 "material_font.gperf"
      {"memory", "\uE322"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1947 "material_font.gperf"
      {"strikethrough_s", "\uE257"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 354 "material_font.gperf"
      {"brightness_high", "\uE1AC"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2180 "material_font.gperf"
      {"visibility", "\uE8F4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 447 "material_font.gperf"
      {"child_friendly", "\uEB42"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 316 "material_font.gperf"
      {"bluetooth_searching", "\uE1AA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 545 "material_font.gperf"
      {"currency_exchange", "\uEB70"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 984 "material_font.gperf"
      {"headphones_battery", "\uF020"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2183 "material_font.gperf"
      {"voice_over_off", "\uE94A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1098 "material_font.gperf"
      {"keyboard_command", "\uEAE0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1036 "material_font.gperf"
      {"how_to_vote", "\uE175"},
      {""}, {""}, {""},
#line 797 "material_font.gperf"
      {"file_copy", "\uE173"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1940 "material_font.gperf"
      {"store_mall_directory", "\uE563"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1931 "material_font.gperf"
      {"stay_current_portrait", "\uE0D4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1003 "material_font.gperf"
      {"highlight", "\uE25F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1004 "material_font.gperf"
      {"highlight_alt", "\uEF52"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1749 "material_font.gperf"
      {"security", "\uE32A"},
      {""}, {""},
#line 794 "material_font.gperf"
      {"fiber_new", "\uE05E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2227 "material_font.gperf"
      {"webhook", "\uEB92"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 989 "material_font.gperf"
      {"health_and_safety", "\uE1D5"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 818 "material_font.gperf"
      {"filter_b_and_w", "\uE3DB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1923 "material_font.gperf"
      {"star_border_purple500", "\uF099"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1498 "material_font.gperf"
      {"personal_injury", "\uE6DA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1594 "material_font.gperf"
      {"psychology", "\uEA4A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1588 "material_font.gperf"
      {"priority_high", "\uE645"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 313 "material_font.gperf"
      {"bluetooth_connected", "\uE1A8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1644 "material_font.gperf"
      {"reduce_capacity", "\uF21C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1097 "material_font.gperf"
      {"keyboard_capslock", "\uE318"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2172 "material_font.gperf"
      {"view_module", "\uE8F0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 2219 "material_font.gperf"
      {"wb_sunny", "\uE430"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 108 "material_font.gperf"
      {"accessibility_new", "\uE92C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 2168 "material_font.gperf"
      {"view_headline", "\uE8EE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2128 "material_font.gperf"
      {"usb", "\uE1E0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2215 "material_font.gperf"
      {"wb_cloudy", "\uE42D"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1096 "material_font.gperf"
      {"keyboard_backspace", "\uE317"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1590 "material_font.gperf"
      {"private_connectivity", "\uE744"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 329 "material_font.gperf"
      {"bookmark_remove", "\uE59A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1904 "material_font.gperf"
      {"sports_hockey", "\uEA2B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 2144 "material_font.gperf"
      {"video_camera_back", "\uF07F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1109 "material_font.gperf"
      {"keyboard_return", "\uE31B"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 295 "material_font.gperf"
      {"bedroom_baby", "\uEFE0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 369 "material_font.gperf"
      {"bungalow", "\uE591"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1990 "material_font.gperf"
      {"switch_video", "\uE41F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 311 "material_font.gperf"
      {"bluetooth", "\uE1A7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1005 "material_font.gperf"
      {"highlight_off", "\uE888"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1146 "material_font.gperf"
      {"lightbulb_circle", "\uEBFE"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1933 "material_font.gperf"
      {"stay_primary_portrait", "\uE0D6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2191 "material_font.gperf"
      {"volunteer_activism", "\uEA70"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 883 "material_font.gperf"
      {"format_align_justify", "\uE235"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2261 "material_font.gperf"
      {"work_history", "\uEC09"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 245 "material_font.gperf"
      {"attach_money", "\uE227"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 107 "material_font.gperf"
      {"accessibility", "\uE84E"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 279 "material_font.gperf"
      {"bathtub", "\uEA41"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1183 "material_font.gperf"
      {"local_pharmacy", "\uE550"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 434 "material_font.gperf"
      {"chat_bubble", "\uE0CA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 2158 "material_font.gperf"
      {"view_array", "\uE8EA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2096 "material_font.gperf"
      {"tty", "\uF1AA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1147 "material_font.gperf"
      {"lightbulb_outline", "\uE90F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1237 "material_font.gperf"
      {"manage_history", "\uEBE7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1092 "material_font.gperf"
      {"keyboard_arrow_down", "\uE313"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 1002 "material_font.gperf"
      {"high_quality", "\uE024"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1382 "material_font.gperf"
      {"no_photography", "\uF1A8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 343 "material_font.gperf"
      {"boy", "\uEB67"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1093 "material_font.gperf"
      {"keyboard_arrow_left", "\uE314"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1917 "material_font.gperf"
      {"stacked_bar_chart", "\uE9E6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1913 "material_font.gperf"
      {"sports_volleyball", "\uEA31"},
      {""}, {""},
#line 1040 "material_font.gperf"
      {"hub", "\uE9F4"},
      {""}, {""}, {""}, {""}, {""},
#line 1276 "material_font.gperf"
      {"military_tech", "\uEA3F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1073 "material_font.gperf"
      {"inventory", "\uE179"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 314 "material_font.gperf"
      {"bluetooth_disabled", "\uE1A9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 315 "material_font.gperf"
      {"bluetooth_drive", "\uEFE5"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1178 "material_font.gperf"
      {"local_library", "\uE54B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2161 "material_font.gperf"
      {"view_comfortable", "\uE42A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1110 "material_font.gperf"
      {"keyboard_tab", "\uE31C"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1562 "material_font.gperf"
      {"playlist_play", "\uE05F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 593 "material_font.gperf"
      {"device_hub", "\uE335"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 431 "material_font.gperf"
      {"change_history", "\uE86B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2170 "material_font.gperf"
      {"view_kanban", "\uEB7F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 1095 "material_font.gperf"
      {"keyboard_arrow_up", "\uE316"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 2162 "material_font.gperf"
      {"view_comfy", "\uE42A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2272 "material_font.gperf"
      {"youtube_searched_for", "\uE8FA"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2177 "material_font.gperf"
      {"view_week", "\uE8F3"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1094 "material_font.gperf"
      {"keyboard_arrow_right", "\uE315"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1101 "material_font.gperf"
      {"keyboard_control_key", "\uEAE6"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1528 "material_font.gperf"
      {"photo_library", "\uE413"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 1108 "material_font.gperf"
      {"keyboard_option_key", "\uEAE8"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 619 "material_font.gperf"
      {"directions_subway", "\uE533"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 1282 "material_font.gperf"
      {"mobile_friendly", "\uE200"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1546 "material_font.gperf"
      {"pivot_table_chart", "\uE9CE"},
      {""}, {""}, {""},
#line 1029 "material_font.gperf"
      {"hourglass_empty", "\uE88B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1099 "material_font.gperf"
      {"keyboard_command_key", "\uEAE7"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 360 "material_font.gperf"
      {"browse_gallery", "\uEBD1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 264 "material_font.gperf"
      {"baby_changing_station", "\uF19B"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1145 "material_font.gperf"
      {"lightbulb", "\uE0F0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 687 "material_font.gperf"
      {"earbuds_battery", "\uF004"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1006 "material_font.gperf"
      {"highlight_remove", "\uE888"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 435 "material_font.gperf"
      {"chat_bubble_outline", "\uE0CB"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 365 "material_font.gperf"
      {"bubble_chart", "\uE6DD"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 2166 "material_font.gperf"
      {"view_cozy", "\uEB75"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 2167 "material_font.gperf"
      {"view_day", "\uE8ED"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 1957 "material_font.gperf"
      {"subway", "\uE56F"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 2150 "material_font.gperf"
      {"video_library", "\uE04A"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 1103 "material_font.gperf"
      {"keyboard_double_arrow_left", "\uEAC3"},
#line 1104 "material_font.gperf"
      {"keyboard_double_arrow_right", "\uEAC9"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1102 "material_font.gperf"
      {"keyboard_double_arrow_down", "\uEAD0"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 1105 "material_font.gperf"
      {"keyboard_double_arrow_up", "\uEACF"}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          const char *s = wordlist[key].name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return 0;
}
