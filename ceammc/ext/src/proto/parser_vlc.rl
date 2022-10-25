# include "parser_vlc.h"

%%{

machine vlc_seek_parser;


int = [0-9]+ '%'?;
date = ([0-9]+ [Hh] ':')? ([0-9]+ [Mm']? ':')? ([0-9]+ [Ss"]);
time = int | date;

main := ('-'|'+')? time 0 @{ fbreak; };
write data;

}%%

namespace ceammc {
namespace parser {

bool check_vlc_seek(const Atom& a)
{
    if (a.isInteger()) return true;
    if (a.isSymbol()) return check_vlc_seek(a.asT<t_symbol*>()->s_name);
    return false;
}

bool check_vlc_seek(const char* str)
{
    int cs = 0;
    auto* p = str;

    %% write init;
    %% write exec noend;

    return cs >= %%{ write first_final; }%%;
}

}
}


