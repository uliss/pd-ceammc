# include "parser_canvas_id.h"
# include "lex/ragel_common.h"

# include <cstring>

namespace ceammc {

%%{

machine canvas_id;

hex = [0-9a-f]+ @{ (canvas_id <<= 4) |= parser::xchar2digit(fc); };
canvas = '.ecanvas' [0-9a-f]+;

main := '.x' hex '.c' canvas? 0 @{ fbreak; };
write data;

}%%

bool try_parse_canvas_id(const char* str, CanvasId& id) {
    int cs = 0;
    const char* p = str;
    CanvasId canvas_id = 0;

    %% write init;
    %% write exec noend;

    const auto ok = cs >= %%{ write first_final; }%%;
    if (ok)
        id = canvas_id;

    return ok;
}

}
