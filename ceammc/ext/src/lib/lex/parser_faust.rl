# include "ragel_common.h"
# include "parser_faust.h"

# include <cstdint>
# include <string>

namespace ceammc {
namespace parser {

%%{
    machine faust_style_menu;

    action key_init { key.clear(); }
    action on_key_char { key += fc; }
    action pair_init { key.clear(); idx = 0; }
    action on_digit  { idx = fc - '0'; }
    action pair_done { fn(key, idx); }

    key = ("'" ([A-Za-z_0-9]+ $on_key_char) "'") >key_init;
    pair = (key ":" space* digit $on_digit) >pair_init %pair_done;
    pair_list = space* pair (',' pair)* space*;

    main := '{' pair_list '}' 0 @{ fbreak; };
    write data;
}%%

bool parse_faust_style_menu(const char* str, FaustStyleMenuCb fn)
{
    int cs = 0;
    const char* p = str;

    std::string key;
    int idx = 0;

    %% write init;
    %% write exec noend;

    const auto ok = cs >= %%{ write first_final; }%%;
    return ok;
}

}
}


