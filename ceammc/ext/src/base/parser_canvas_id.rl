# include "parser_canvas_id.h"
# include "lex/ragel_common.h"

# include <cstring>
# include <limits>

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
    if (ok) {
#ifdef __WIN32__
        id = (std::numeric_limits<std::uint32_t>::max() & canvas_id);
#else
        id = canvas_id;
#endif
    }

    return ok;
}

}
