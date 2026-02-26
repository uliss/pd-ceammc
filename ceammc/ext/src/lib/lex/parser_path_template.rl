# include "lex/parser_path_template.h"

# include <cstring>

namespace ceammc {
namespace parser {

%%{

machine parser_path_template;

action prefix_start  { if(begin) *begin = fpc; }
action prefix_done   { if(end)   *end = fpc; }

s = 'S'?;
B = '$';
E = '$';
home      = ((B 'HOME' E)      | '~')                 %{ res = DirectoryTemplate::Home; };
audio     = ( B ('AUDIO'|'MUSIC')                     E) %{ res = DirectoryTemplate::Audio; };
video     = ( B ('VIDEO'|('MOVIE' s))                 E) %{ res = DirectoryTemplate::Video; };
picture   = ( B ('IMAGE' | 'IMG' | ('PIC' 'TURE'? s)) E) %{ res = DirectoryTemplate::Picture; };
document  = ( B 'DOC' 'UMENT'? s                      E) %{ res = DirectoryTemplate::Document; };
download  = ( B 'DOWNLOAD' s                          E) %{ res = DirectoryTemplate::Download; };
desktop   = ( B 'DESKTOP'                             E) %{ res = DirectoryTemplate::Desktop; };
tmp       = ( B 'TMP'                                 E) %{ res = DirectoryTemplate::Tmp; };
cwd       = ( B 'CWD'                                 E) %{ res = DirectoryTemplate::Cwd; };

proto  = 'file://';
prefix = (home | audio | video | picture | document | download | desktop | tmp | cwd)
            >prefix_start
            %prefix_done;

unknown = ((B [A-Z0-1]+ E) - prefix) %{ res = DirectoryTemplate::Unknown; }
            >prefix_start
            %prefix_done;

other  = [^0];

main := proto? (prefix | unknown) other* 0 @{ fbreak; };
write data;

}%%

std::string path_dir_template_subst(const char* path, const path_dir_template_subst_cb& fn)
{
    const char* begin = nullptr;
    const char* end = nullptr;
    auto type = path_get_dir_template(path, &begin, &end);
    if (type != DirectoryTemplate::None) {
        std::string res;
        res.reserve(std::strlen(path) + 64);
        res.assign(path, begin);
        res.append(fn(type, begin, end));
        res.append(end);
        return res;
    } else {
        return path;
    }
}

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
