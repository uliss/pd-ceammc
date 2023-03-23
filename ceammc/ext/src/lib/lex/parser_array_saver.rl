# include "parser_array_saver.h"
# include "parser_numeric.h"
# include "ragel_common.h"

# include <algorithm>
# include <cmath>
# include <cstdint>
# include <string>

namespace {
struct NumCleaner {
    ceammc::parser::fsm::NumericData& num_;
    ~NumCleaner() { num_ = {}; }
};

template<typename T>
inline T getValue(ceammc::parser::fsm::NumericData& num, ceammc::parser::AtomType type)
{
    NumCleaner cleaner { num };
    return (type != ceammc::parser::TYPE_FLOAT) ? num.getInteger() : num.getFloat();
}

inline size_t clipArraySize(std::int64_t v, size_t arraySize)
{
    return std::min<std::int64_t>(arraySize - 1, std::max<std::int64_t>(0, v));
}

inline std::int64_t getUnitValue(
    ceammc::parser::ArraySaverParams& params,
    ceammc::parser::fsm::NumericData& num,
    ceammc::parser::AtomType type,
    size_t arraySize)
{
    switch (type) {
    case ceammc::parser::TYPE_PERCENT:
        return std::round(num.getFloat() * arraySize * 0.01);
    case ceammc::parser::TYPE_PHASE:
        return std::round(num.getFloat() * arraySize);
    case ceammc::parser::TYPE_FLOAT:
    case ceammc::parser::TYPE_INT:
    case ceammc::parser::TYPE_SAMP:
        return std::round(num.getFloat());
    case ceammc::parser::TYPE_MSEC:
        return std::round(num.getFloat() * 0.001 * params.in_sr);
    case ceammc::parser::TYPE_SEC:
        return std::round(num.getFloat() * params.in_sr);
    default:
        return 0;
    }
}

inline size_t getArrayAbsPos(ceammc::parser::ArraySaverParams& params, std::int64_t pos, std::int64_t arraySize)
{
    return (params.origin == ceammc::parser::ORIGIN_BEGIN) ? pos : (arraySize + pos);
}

inline size_t getArrayPos(ceammc::parser::ArraySaverParams& params,
    ceammc::parser::fsm::NumericData& num,
    ceammc::parser::AtomType type,
    size_t arraySize)
{
    NumCleaner cleaner { num };

    auto pos = getUnitValue(params, num, type, arraySize);
    return clipArraySize(getArrayAbsPos(params, pos, arraySize), arraySize);
}
}

%%{
machine array_saver;
include numeric_common "ragel_numeric.rl";
include units_common "ragel_units.rl";

array_value = num_float | (num_int %{ ragel_num.vdouble = ragel_num.vint; });
array_units = unit_perc | unit_phase | unit_samp | unit_msec | unit_sec;
array_unit_value = ('$' @{ params.origin = ORIGIN_END; })? array_value '_'? array_units?;

atom_char = (any - space) @{ ragel_atom += fc; };
atom_char0 = atom_char - '@';
atom = (atom_char0 atom_char*);
val = num_float | num_int;
ws = space+;
bits_val     = '0'  %{ params.bits = 0; }
             | '8'  %{ params.bits = 8; }
             | '16' %{ params.bits = 16; }
             | '24' %{ params.bits = 24; }
             | '32' %{ params.bits = 32; }
             ;

opt_gain     = '@gain'   ws val;
opt_begin    = '@begin'  >{ params.origin = ORIGIN_BEGIN; } ws array_unit_value;
opt_end      = '@end'    >{ params.origin = ORIGIN_BEGIN; } ws array_unit_value;
opt_in_sr    = '@in_sr'  ws val;
opt_out_sr   = '@out_sr' ws val;
opt_bits     = '@bits'   ws bits_val;
opt_norm     = '@norm';
opt_wav      = '@wav';
opt_mp3      = '@mp3';
opt_ogg      = '@ogg';
opt_opus     = '@opus';
opt_aiff     = '@aiff';
opt_txt      = '@txt';
opt_raw      = '@raw';

opt = opt_gain   %{ params.gain = getValue<float>(ragel_num, ragel_type); }
    | opt_begin  %{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
    | opt_end    %{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
    | opt_bits
    | opt_in_sr  %{ params.in_sr  = getValue<float>(ragel_num, ragel_type); }
    | opt_out_sr %{ params.out_sr = getValue<float>(ragel_num, ragel_type); }
    | opt_norm   %{ params.normalize = true; }
    | opt_wav    %{ params.format = FORMAT_WAV; }
    | opt_mp3    %{ params.format = FORMAT_MP3; }
    | opt_ogg    %{ params.format = FORMAT_VORBIS; }
    | opt_opus   %{ params.format = FORMAT_OPUS; }
    | opt_aiff   %{ params.format = FORMAT_AIFF; }
    | opt_txt    %{ params.format = FORMAT_TEXT; }
    | opt_raw    %{ params.format = FORMAT_RAW; }
    ;

options = opt (ws opt)*;

file =  atom >{ ragel_atom.clear(); } %{ params.filename = ragel_atom; };
array = atom >{ ragel_atom.clear(); } %{ params.arrays.push_back(ragel_atom); };
array_list = array (ws array)*;

main :=
    array_list
    ws
    '@to'
    ws
    file
    (ws options)?;

write data;

}%%

namespace ceammc {
namespace parser {

bool parse_array_saver_params(const char* str, size_t arraySize, ArraySaverParams& params)
{
    auto len = strlen(str);
    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = p + len;

    std::string ragel_atom;
    double val = 0;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

%%{
    write init;
    write exec;
}%%

    return cs >= %%{ write first_final; }%%;
}


}
}
