/* C++ code produced by gperf version 3.1 */
/* Command-line: /opt/local/bin/gperf -L C++ --class-name=MorsePerfectHash --struct-type --output-file=gperf_morse.h morse.gperf  */
/* Computed positions: -k'1-2' */

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

#line 5 "morse.gperf"

#include <cstdint>
struct MorseBitCode {
    std::uint8_t code;
    std::uint8_t length;
    char toChar(std::uint8_t pos) const {
        auto idx = std::int16_t(length) - (pos+1);
        if (idx < 0 || idx >= length)
            return ' ';

        return (code & (0x1 << idx)) ? '_' : '.' ;
    }
    std::int8_t at(std::uint8_t pos) const {
        auto idx = std::int16_t(length) - (pos+1);
        if (idx < 0 || idx >= length)
            return -1;

        return (code & (0x1 << idx)) ? 1 : 0;
    }
};
#define MORSE(code) { 0b ## code, sizeof(#code)-1 }
#line 27 "morse.gperf"
struct MorseCode
{
    const char* name;
    MorseBitCode code;
};
#include <string.h>
/* maximum key range = 257, duplicates = 0 */

class MorsePerfectHash
{
private:
  static inline unsigned int hash (const char *str, size_t len);
public:
  static const struct MorseCode *in_word_set (const char *str, size_t len);
};

inline unsigned int
MorsePerfectHash::hash (const char *str, size_t len)
{
  static const unsigned short asso_values[] =
    {
      258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
      258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
      258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
      258, 258, 258, 258, 258, 258, 258, 258, 258, 159,
      258, 258, 258, 154, 258, 149, 144, 258, 139, 134,
      129, 124, 119, 114, 109, 104,  99,  94,  89,  84,
      258, 258, 258,  79,  74,  69,  64,  59,  54,  49,
       44,  39,  34,  29,  24,  19,  14,   9,   4, 255,
      250, 245, 240, 235, 230, 225, 220, 215, 210, 205,
      200, 258, 258, 258, 258, 258, 258, 195, 190, 185,
      180, 175, 170, 165, 160, 155, 150, 145, 140, 135,
      130, 125, 120, 115, 110, 105, 100,  95,  90,  85,
       80,  75,   0, 258, 258, 258, 258, 258,  69,  10,
       64,  59,  54,  49,  44,  39,  34,  29,  24,  19,
       14,   9,   4, 255, 245,   0, 240, 235, 230, 225,
      220, 215, 210, 205, 200, 195, 190, 185, 180, 175,
      170, 165, 160, 155, 150, 145, 140, 135, 130, 125,
      120, 115, 110, 105, 100,  95,  90,  85,  80,  75,
       70,  65,  60,  55,  50,  45,  40,  35,  30,  25,
       20,  15, 258, 258, 258, 258, 258, 258, 258, 258,
      258, 258, 258, 258, 258, 258, 258, 258,   5,   0,
      258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
      258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
      258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
      258, 258, 258, 258, 258, 258, 258, 258, 258, 258,
      258, 258, 258, 258, 258, 258
    };
  unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[static_cast<unsigned char>(str[1])];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[static_cast<unsigned char>(str[0])];
        break;
    }
  return hval;
}

const struct MorseCode *
MorsePerfectHash::in_word_set (const char *str, size_t len)
{
  enum
    {
      TOTAL_KEYWORDS = 136,
      MIN_WORD_LENGTH = 1,
      MAX_WORD_LENGTH = 2,
      MIN_HASH_VALUE = 1,
      MAX_HASH_VALUE = 257
    };

  static const struct MorseCode wordlist[] =
    {
      {""},
#line 104 "morse.gperf"
      {"z", MORSE(1100)},
#line 119 "morse.gperf"
      {"\321\221", MORSE(0)},
      {""}, {""},
#line 79 "morse.gperf"
      {"N", MORSE(10)},
#line 169 "morse.gperf"
      {"\321\216", MORSE(0011)},
#line 108 "morse.gperf"
      {"\320\221", MORSE(1000)},
      {""}, {""},
#line 77 "morse.gperf"
      {"M", MORSE(11)},
#line 167 "morse.gperf"
      {"\321\215", MORSE(00100)},
#line 143 "morse.gperf"
      {"\321\201", MORSE(000)},
      {""}, {""},
#line 75 "morse.gperf"
      {"L", MORSE(0100)},
#line 165 "morse.gperf"
      {"\321\214", MORSE(1001)},
#line 118 "morse.gperf"
      {"\320\201", MORSE(0)},
      {""}, {""},
#line 73 "morse.gperf"
      {"K", MORSE(101)},
#line 163 "morse.gperf"
      {"\321\213", MORSE(1011)},
#line 139 "morse.gperf"
      {"\320\277", MORSE(0110)},
      {""}, {""},
#line 71 "morse.gperf"
      {"J", MORSE(0111)},
#line 161 "morse.gperf"
      {"\321\212", MORSE(1001)},
#line 137 "morse.gperf"
      {"\320\276", MORSE(111)},
      {""}, {""},
#line 69 "morse.gperf"
      {"I", MORSE(00)},
#line 159 "morse.gperf"
      {"\321\211", MORSE(1101)},
#line 135 "morse.gperf"
      {"\320\275", MORSE(10)},
      {""}, {""},
#line 67 "morse.gperf"
      {"H", MORSE(0000)},
#line 157 "morse.gperf"
      {"\321\210", MORSE(1111)},
#line 133 "morse.gperf"
      {"\320\274", MORSE(11)},
      {""}, {""},
#line 65 "morse.gperf"
      {"G", MORSE(110)},
#line 155 "morse.gperf"
      {"\321\207", MORSE(1110)},
#line 131 "morse.gperf"
      {"\320\273", MORSE(0100)},
      {""}, {""},
#line 63 "morse.gperf"
      {"F", MORSE(0010)},
#line 153 "morse.gperf"
      {"\321\206", MORSE(1010)},
#line 129 "morse.gperf"
      {"\320\272", MORSE(101)},
      {""}, {""},
#line 61 "morse.gperf"
      {"E", MORSE(0)},
#line 151 "morse.gperf"
      {"\321\205", MORSE(0000)},
#line 127 "morse.gperf"
      {"\320\271", MORSE(0111)},
      {""}, {""},
#line 59 "morse.gperf"
      {"D", MORSE(100)},
#line 149 "morse.gperf"
      {"\321\204", MORSE(0010)},
#line 125 "morse.gperf"
      {"\320\270", MORSE(00)},
      {""}, {""},
#line 57 "morse.gperf"
      {"C", MORSE(1010)},
#line 147 "morse.gperf"
      {"\321\203", MORSE(001)},
#line 123 "morse.gperf"
      {"\320\267", MORSE(1100)},
      {""}, {""},
#line 55 "morse.gperf"
      {"B", MORSE(1000)},
#line 145 "morse.gperf"
      {"\321\202", MORSE(1)},
#line 121 "morse.gperf"
      {"\320\266", MORSE(0001)},
      {""}, {""},
#line 53 "morse.gperf"
      {"A", MORSE(01)},
#line 141 "morse.gperf"
      {"\321\200", MORSE(010)},
#line 117 "morse.gperf"
      {"\320\265", MORSE(0)},
      {""}, {""},
#line 51 "morse.gperf"
      {"@", MORSE(011010)},
#line 102 "morse.gperf"
      {"y", MORSE(1011)},
#line 115 "morse.gperf"
      {"\320\264", MORSE(100)},
      {""}, {""},
#line 45 "morse.gperf"
      {"?", MORSE(001100)},
#line 100 "morse.gperf"
      {"x", MORSE(1001)},
#line 113 "morse.gperf"
      {"\320\263", MORSE(110)},
      {""}, {""},
#line 48 "morse.gperf"
      {";", MORSE(101010)},
#line 98 "morse.gperf"
      {"w", MORSE(011)},
#line 111 "morse.gperf"
      {"\320\262", MORSE(011)},
      {""}, {""},
#line 47 "morse.gperf"
      {":", MORSE(111000)},
#line 96 "morse.gperf"
      {"v", MORSE(0001)},
#line 109 "morse.gperf"
      {"\320\261", MORSE(1000)},
      {""}, {""},
#line 43 "morse.gperf"
      {"9", MORSE(00001)},
#line 94 "morse.gperf"
      {"u", MORSE(001)},
#line 107 "morse.gperf"
      {"\320\260", MORSE(01)},
      {""}, {""},
#line 42 "morse.gperf"
      {"8", MORSE(00011)},
#line 92 "morse.gperf"
      {"t", MORSE(1)},
#line 170 "morse.gperf"
      {"\320\257", MORSE(0101)},
      {""}, {""},
#line 41 "morse.gperf"
      {"7", MORSE(00111)},
#line 90 "morse.gperf"
      {"s", MORSE(000)},
#line 168 "morse.gperf"
      {"\320\256", MORSE(0011)},
      {""}, {""},
#line 40 "morse.gperf"
      {"6", MORSE(01111)},
#line 88 "morse.gperf"
      {"r", MORSE(010)},
#line 166 "morse.gperf"
      {"\320\255", MORSE(00100)},
      {""}, {""},
#line 39 "morse.gperf"
      {"5", MORSE(11111)},
#line 86 "morse.gperf"
      {"q", MORSE(1101)},
#line 164 "morse.gperf"
      {"\320\254", MORSE(1001)},
      {""}, {""},
#line 38 "morse.gperf"
      {"4", MORSE(11110)},
#line 84 "morse.gperf"
      {"p", MORSE(0110)},
#line 162 "morse.gperf"
      {"\320\253", MORSE(1011)},
      {""}, {""},
#line 37 "morse.gperf"
      {"3", MORSE(11100)},
#line 82 "morse.gperf"
      {"o", MORSE(111)},
#line 160 "morse.gperf"
      {"\320\252", MORSE(1001)},
      {""}, {""},
#line 36 "morse.gperf"
      {"2", MORSE(11000)},
#line 80 "morse.gperf"
      {"n", MORSE(10)},
#line 158 "morse.gperf"
      {"\320\251", MORSE(1101)},
      {""}, {""},
#line 35 "morse.gperf"
      {"1", MORSE(10000)},
#line 78 "morse.gperf"
      {"m", MORSE(11)},
#line 156 "morse.gperf"
      {"\320\250", MORSE(1111)},
      {""}, {""},
#line 34 "morse.gperf"
      {"0", MORSE(00000)},
#line 76 "morse.gperf"
      {"l", MORSE(0100)},
#line 154 "morse.gperf"
      {"\320\247", MORSE(1110)},
      {""}, {""},
#line 44 "morse.gperf"
      {".", MORSE(010101)},
#line 74 "morse.gperf"
      {"k", MORSE(101)},
#line 152 "morse.gperf"
      {"\320\246", MORSE(1010)},
      {""}, {""},
#line 50 "morse.gperf"
      {"-", MORSE(100001)},
#line 72 "morse.gperf"
      {"j", MORSE(0111)},
#line 150 "morse.gperf"
      {"\320\245", MORSE(0000)},
      {""}, {""},
#line 49 "morse.gperf"
      {"+", MORSE(10101)},
#line 70 "morse.gperf"
      {"i", MORSE(00)},
#line 148 "morse.gperf"
      {"\320\244", MORSE(0010)},
      {""}, {""},
#line 46 "morse.gperf"
      {"'", MORSE(011110)},
#line 68 "morse.gperf"
      {"h", MORSE(0000)},
#line 146 "morse.gperf"
      {"\320\243", MORSE(001)},
      {""}, {""}, {""},
#line 66 "morse.gperf"
      {"g", MORSE(110)},
#line 144 "morse.gperf"
      {"\320\242", MORSE(1)},
      {""}, {""}, {""},
#line 64 "morse.gperf"
      {"f", MORSE(0010)},
#line 142 "morse.gperf"
      {"\320\241", MORSE(000)},
      {""}, {""}, {""},
#line 62 "morse.gperf"
      {"e", MORSE(0)},
#line 140 "morse.gperf"
      {"\320\240", MORSE(010)},
      {""}, {""}, {""},
#line 60 "morse.gperf"
      {"d", MORSE(100)},
#line 138 "morse.gperf"
      {"\320\237", MORSE(0110)},
      {""}, {""}, {""},
#line 58 "morse.gperf"
      {"c", MORSE(1010)},
#line 136 "morse.gperf"
      {"\320\236", MORSE(111)},
      {""}, {""}, {""},
#line 56 "morse.gperf"
      {"b", MORSE(1000)},
#line 134 "morse.gperf"
      {"\320\235", MORSE(10)},
      {""}, {""}, {""},
#line 54 "morse.gperf"
      {"a", MORSE(01)},
#line 132 "morse.gperf"
      {"\320\234", MORSE(11)},
      {""}, {""}, {""},
#line 103 "morse.gperf"
      {"Z", MORSE(1100)},
#line 130 "morse.gperf"
      {"\320\233", MORSE(0100)},
      {""}, {""}, {""},
#line 101 "morse.gperf"
      {"Y", MORSE(1011)},
#line 128 "morse.gperf"
      {"\320\232", MORSE(101)},
      {""}, {""}, {""},
#line 99 "morse.gperf"
      {"X", MORSE(1001)},
#line 126 "morse.gperf"
      {"\320\231", MORSE(0111)},
      {""}, {""}, {""},
#line 97 "morse.gperf"
      {"W", MORSE(011)},
#line 124 "morse.gperf"
      {"\320\230", MORSE(00)},
      {""}, {""}, {""},
#line 95 "morse.gperf"
      {"V", MORSE(0001)},
#line 122 "morse.gperf"
      {"\320\227", MORSE(1100)},
      {""}, {""}, {""},
#line 93 "morse.gperf"
      {"U", MORSE(001)},
#line 120 "morse.gperf"
      {"\320\226", MORSE(0001)},
      {""}, {""}, {""},
#line 91 "morse.gperf"
      {"T", MORSE(1)},
#line 116 "morse.gperf"
      {"\320\225", MORSE(0)},
      {""}, {""}, {""},
#line 89 "morse.gperf"
      {"S", MORSE(000)},
#line 114 "morse.gperf"
      {"\320\224", MORSE(100)},
      {""}, {""}, {""},
#line 87 "morse.gperf"
      {"R", MORSE(010)},
#line 112 "morse.gperf"
      {"\320\223", MORSE(110)},
      {""}, {""}, {""},
#line 85 "morse.gperf"
      {"Q", MORSE(1101)},
#line 110 "morse.gperf"
      {"\320\222", MORSE(011)},
      {""}, {""}, {""},
#line 83 "morse.gperf"
      {"P", MORSE(0110)},
#line 106 "morse.gperf"
      {"\320\220", MORSE(01)},
      {""}, {""}, {""},
#line 81 "morse.gperf"
      {"O", MORSE(111)},
#line 171 "morse.gperf"
      {"\321\217", MORSE(0101)}
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
