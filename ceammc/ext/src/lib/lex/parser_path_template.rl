# include "lex/parser_path_template.h"
# include "lex/ragel_common.h"

# include <cstring>
# include <limits>

namespace ceammc {
namespace parser {

%%{

machine parser_path_template;

home      = '%HOME%'   | '~';
audio     = '%AUDIO%'  | '%MUSIC%';
video     = '%VIDEO%'  | '%MOVIE%'    | '%MOVIES%';
picture   = '%IMAGE%'  | '%PICTURE%';
document  = '%DOC%'    | '%DOCUMENT%';
desktop   = '%DESKTOP%';
download  = '%DOWNLOAD%';

proto  = 'file://';
prefix = (home | audio | video | picture | document | download | desktop);
other  = [^0];

main := proto? prefix other* 0 @{ fbreak; };
write data;

}%%

bool path_contains_template(const char* str) {
    int cs = 0;
    const char* p = str;

    %% write init;
    %% write exec noend;

    const auto ok = cs >= %%{ write first_final; }%%;
    return ok;
}

} // namespace parser
} // namespace ceammc
