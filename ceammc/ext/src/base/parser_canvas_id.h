#ifndef PARSER_CANVAS_ID_H
#define PARSER_CANVAS_ID_H

#include <cstdint>

namespace ceammc {

using CanvasId = std::uintptr_t;

bool try_parse_canvas_id(const char* str, CanvasId& id);

}

#endif // PARSER_CANVAS_ID_H
