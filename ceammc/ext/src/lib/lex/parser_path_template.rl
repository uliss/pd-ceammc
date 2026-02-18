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

s = 'S'?;
G = '%';
home      = ((G 'HOME' G)      | '~')                 %{ res = DirectoryTemplate::Home; };
audio     = ( G ('AUDIO'|'MUSIC')                     G) %{ res = DirectoryTemplate::Audio; };
video     = ( G ('VIDEO'|('MOVIE' s))                 G) %{ res = DirectoryTemplate::Video; };
picture   = ( G ('IMAGE' | 'IMG' | ('PIC' 'TURE'? s)) G) %{ res = DirectoryTemplate::Picture; };
document  = ( G 'DOC' 'UMENT'? s                      G) %{ res = DirectoryTemplate::Document; };
download  = ( G 'DOWNLOAD' s                          G) %{ res = DirectoryTemplate::Download; };
desktop   = ( G 'DESKTOP'                             G) %{ res = DirectoryTemplate::Desktop; };
tmp       = ( G 'TMP'                                 G) %{ res = DirectoryTemplate::Tmp; };
cwd       = ( G 'CWD'                                 G) %{ res = DirectoryTemplate::Cwd; };

proto  = 'file://';
prefix = (home | audio | video | picture | document | download | desktop | tmp | cwd)
            >prefix_start
            %prefix_done;

unknown = ((G [A-Z0-1]+ G) - prefix) %{ res = DirectoryTemplate::Unknown; }
            >prefix_start
            %prefix_done;

other  = [^0];

main := proto? (prefix | unknown) other* 0 @{ fbreak; };
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
