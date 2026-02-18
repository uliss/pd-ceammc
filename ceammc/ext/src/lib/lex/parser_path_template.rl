# include "lex/parser_path_template.h"
# include "lex/ragel_common.h"

# include <cstring>
# include <limits>
# include <cstdint>

namespace ceammc {
namespace parser {

%%{

machine parser_path_template;

action prefix_start  { if(begin) *begin = fpc; }
action prefix_done   { if(end)   *end = fpc; }

home      = ('%HOME%'   | '~')                      %{ res = DirectoryTemplate::Home; };
audio     = ('%AUDIO%'   | '%MUSIC%')               %{ res = DirectoryTemplate::Audio; };
video     = ('%VIDEO%'   | ('%MOVIE' 'S'? '%'))     %{ res = DirectoryTemplate::Video; };
picture   = ('%IMAGE%'   | ('%PICTURE' 'S'? '%'))   %{ res = DirectoryTemplate::Picture; };
document  = ('%DOC%'     | ('%DOCUMENT' 'S'? '%'))  %{ res = DirectoryTemplate::Document; };
desktop   = ('%DESKTOP%')                           %{ res = DirectoryTemplate::Desktop; };
download  = ('%DOWNLOAD' 'S'? '%')                  %{ res = DirectoryTemplate::Download; };

proto  = 'file://';
prefix = (home | audio | video | picture | document | download | desktop)
            >prefix_start
            %prefix_done;

other  = [^0];

main := proto? prefix other* 0 @{ fbreak; };
write data;

}%%

DirectoryTemplate path_get_dir_template(const char* path, const char** begin, const char** end) {
    int cs = 0;
    const char* p = path;
    DirectoryTemplate res = DirectoryTemplate::None;

    if (begin)
        *begin = nullptr;

    if (end)
        *end = nullptr;

    %% write init;
    %% write exec noend;

    if (cs >= %%{ write first_final; }%%)
        return res;
    else
        return DirectoryTemplate::None;
}

} // namespace parser
} // namespace ceammc
