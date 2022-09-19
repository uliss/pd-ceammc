#ifndef PARSER_OSC_H
#define PARSER_OSC_H

#include "m_pd.h"

namespace ceammc {
namespace parser {

    bool parse_osc_url(const char* str, t_symbol*& proto, t_symbol*& host, t_symbol*& port);

}
}

#endif // PARSER_OSC_H
