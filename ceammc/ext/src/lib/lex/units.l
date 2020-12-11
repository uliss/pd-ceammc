%top{
    # include <cerrno>
    # include <cstdlib>
    # include <cstdint>
    # include <cmath>
    # include <array>

    # include "ceammc_log.h"
%}

%class{
    public:
        enum UnitType : uint8_t {
            U_UNKNOWN,
            U_MSEC,
            U_SEC,
            U_MINUTE,
            U_HOUR,
            U_DAY,
            U_SAMP,
            U_HZ,
            U_CENT,
            U_SEMITONE,
            U_TONE,
            U_BPM,
            U_RADIANS,
            U_DEGREE,
            U_DB,
            U_SMPTE,
            U_PERCENT,
            U_RATIO,
            U_PHASE
        };

        enum ValueType : uint8_t {
            T_DOUBLE, T_LONG, T_SMPTE, T_RATIO
        };

        enum Status {
            STATUS_UNKNOWN_TOKEN = -2,
            STATUS_PARSE_ERROR = -1,
            STATUS_EOF = 0,
            STATUS_OK = 1,
            STATUS_CONTINUE
        };

        struct Smpte {
            uint8_t hour;
            uint8_t min;
            uint8_t sec;
            uint8_t frame;

            double toSeconds(double framerate) const {
                return 3600 * hour + 60 * min + sec + (frame / framerate);
            }
        };

        struct Ratio {
            int32_t num;
            int32_t den;
        };

        union Value {
            long int_val;
            double dbl_val;
            Ratio ratio_val;
            Smpte smpte_val;
        };

        struct UnitValue {
            Value val;
            ValueType type;
            UnitType unit = {U_UNKNOWN};
            bool end_offset = {false};
        };

        static constexpr size_t MAX_UNITS = 8;

        struct UnitValues {
            std::array<UnitValue, MAX_UNITS> data;
            size_t n = {1};

            UnitValue& back() { return data[n-1]; }
            void append() { if(n < MAX_UNITS-1) data[n++].val.int_val = 0; }
            void set(double v, UnitType u) { back().val.dbl_val = v; back().unit = u; back().type = T_DOUBLE; }
            void set(long v, UnitType u) { back().val.int_val = v; back().unit = u; back().type = T_LONG; }
            void set(const Smpte& v) { back().val.smpte_val = v; back().unit = U_SMPTE; back().type = T_SMPTE; }
            void set(const Ratio& v) { back().val.ratio_val = v; back().unit = U_RATIO; back().type = T_RATIO; }

            UnitValue& operator[](size_t i) { return data[i]; }
        };

    public:
        UnitValues values;

        int parseSingle()
        {
            values.n = 1;
            int rc = STATUS_OK;
            for(rc = STATUS_OK; rc == STATUS_OK || rc == STATUS_CONTINUE; rc = lex()) {
                if(rc == STATUS_CONTINUE) {
                    // lookup next
                    if (lex() != STATUS_OK) // invalid next token
                       return STATUS_UNKNOWN_TOKEN;

                    // next token is valid, but we are parsing just one
                    values.n = 1;
                    return STATUS_EOF;
                }
            }

            return rc;
        }

        int parseMany()
        {
            values.n = 1;
            int rc;
            for(rc = STATUS_OK; rc == STATUS_OK || rc == STATUS_CONTINUE; rc = lex())
            ;

            return rc;
        }
%}

%option freespace
%option namespace=ceammc::units
%option lexer=UnitsLexer
%option header-file=units.lexer.h
%option outfile=units.lexer.cpp
%option noyywrap
/*%option debug*/
%option fast

space       [ \t\n]+
udec        0 | [1-9] [0-9]*
frac        \. \d+

int         [-+]? {udec}
uint        {udec}
float       {int} {frac}?
ufloat      {udec} {frac}?
percent     {float}_?%
phase       {float}_?[*]
ratio       {uint} [/] {uint}

min         {float}_?min
umin        {ufloat}_?min
cent        {float}_?cent
semitone    {float}_?semitone
tone        {float}_?tone
msec        {float}_?(ms|msec)
umsec       {ufloat}_?(ms|msec)
samp        {int}_?(sa|samp)
usamp       {uint}_?(sa|samp)
sec         {float}_?(s|sec)
usec        {ufloat}_?(s|sec)
bpm_frac    #(1|2|4|8|16|32|64|128|256)
bpm         {float}{bpm_frac}?_?bpm
rad         {float}_?rad
deg         {float}_?deg
db          {float}_?db
day         {float}_?d
hz          {float}_?hz
hour        {float}_?h
end_offset  (\$|end)/-({usamp}|{umsec}|{usec}|{umin}|{uint})
smpte       [0-9]{2}:[0-9]{2}(:[0-9]{2})?(.[0-9]{2})?

%%

{space}         {   values.append(); return STATUS_CONTINUE; }
{percent}       {
                    auto v = strtod(text(), nullptr);
                    values.set(v, U_PERCENT);
                    return STATUS_OK;
                }
{phase}         {
                    auto v = strtod(text(), nullptr);
                    values.set(v, U_PHASE);
                    return STATUS_OK;
                }
{ratio}         {
                    errno = 0;
                    char* endptr = nullptr;
                    const int32_t n = strtol(text(), &endptr, 10);
                    const int32_t d = strtol(endptr+1, nullptr, 10);

                    if (errno)
                        return STATUS_PARSE_ERROR;

                    if (d == 0) {
                        LIB_ERR << "[units] division by zero: " << text();
                        return STATUS_PARSE_ERROR;
                    }

                    values.set(Ratio{n, d});
                    return STATUS_OK;
                }
{msec}          {
                    const double v = strtod(text(), nullptr);
                    if(!std::isnormal(v) && v != 0)
                        return STATUS_PARSE_ERROR;

                    values.set(v, U_MSEC);
                    return STATUS_OK;
                }
{int}           {
                    errno = 0;
                    auto v = strtol(text(), nullptr, 10);
                    if (errno)
                        return STATUS_PARSE_ERROR;

                    values.set(v, U_MSEC);
                    return STATUS_OK;
                }
{samp}          {
                    errno = 0;
                    auto v = strtol(text(), nullptr, 10);
                    if (errno)
                        return STATUS_PARSE_ERROR;
                    values.set(v, U_SAMP);
                    return STATUS_OK;
                }
{sec}           {
                    auto v = strtod(text(), nullptr);
                    if(!std::isnormal(v) && v != 0)
                        return STATUS_PARSE_ERROR;

                    values.set(v, U_SEC);
                    return STATUS_OK;
                }
{min}           {
                    auto v = strtod(text(), nullptr);
                    if(!std::isnormal(v) && v != 0)
                        return STATUS_PARSE_ERROR;

                    values.set(v, U_MINUTE);
                    return STATUS_OK;
                }
{hz}            {
                    auto v = strtod(text(), nullptr);
                    values.set(v, U_HZ);
                    return STATUS_OK;
                }
{hour}          {
                    auto v = strtod(text(), nullptr);
                    values.set(v, U_HOUR);
                    return STATUS_OK;
                }
{day}           {
                    auto v = strtod(text(), nullptr);
                    values.set(v, U_DAY);
                    return STATUS_OK;
                }
{bpm}           {
                    char* end = 0;
                    auto v = strtod(text(), &end);
                    if(!std::isnormal(v) && v != 0)
                        return STATUS_PARSE_ERROR;

                    int subdiv = 1;
                    if(end && end[0] == '#')
                        subdiv = strtol(end+1, nullptr, 10);

                    values.set(v * subdiv, U_BPM);
                    return STATUS_OK;
                }
{smpte}         {
                    uint8_t d0 = 0; // hour
                    uint8_t d1 = 0; // min
                    uint8_t d2 = 0; // sec
                    uint8_t d3 = 0; // frame
                    char* end = 0;
                    const char* txt = text();
                    d0 = strtol(txt, &end, 10);
                    if (*end == ':') { // 00:
                        d1 = strtol(txt+3, &end, 10);
                        if (*end == ':') { // 00:00:
                            d2 = strtol(txt+6, &end, 10);
                            if (*end == '.') // 00:00:00.
                                d3 = strtol(txt+9, nullptr, 10);
                        } else if(*end == '.') { // 00:00.
                            d3 = strtol(txt+6, nullptr, 10);
                            d2 = d1;
                            d1 = d0;
                            d0 = 0;
                        } else { // 00:00
                            d2 = d1;
                            d1 = d0;
                            d0 = 0;
                        }
                    } else {
                        std::cerr << text() << "\n";
                        return STATUS_PARSE_ERROR;
                        }

                    values.set(Smpte{d0, d1, d2, d3});
                    return STATUS_OK;
                }
{end_offset}    {
                    values.back().end_offset = true;
                    return STATUS_OK;
                }
<<EOF>>         {   return STATUS_EOF;  }
<*> .           {   return STATUS_UNKNOWN_TOKEN; }

%%


