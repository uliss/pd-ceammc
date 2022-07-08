#ifndef LEMON_DATA_STRING_PARSER_H
#define LEMON_DATA_STRING_PARSER_H

#include <cstddef>

#include "ceammc_atomlist.h"

namespace ceammc {
namespace parser {

    class LemonDataStringParser {
    public:
        LemonDataStringParser();
        ~LemonDataStringParser();

        bool parse(const char* str);
        bool doParse(const char* data);

        inline void* parser() { return parser_data_; }

        void pushToken(int token);
        void pushSymbolToken(int token, const char* begin, const char* end);
        void pushDouble(double val);

        void pPushProp(t_symbol* name);
        void pPushPropAtom(const t_atom& a);
        void pPushListAtom(const t_atom& a);

        const AtomList& result() const { return res_; }

        void setErrorMsg(const char* msg);

    public:
        constexpr static size_t PARSER_SIZE = 512* 16;

    private:
        void reset();

    private:
        char parser_data_[PARSER_SIZE]; ///< parser is on the stack to avoid dynamic alloc
        char parser_buf_[256];
        char err_buf_[64];
        bool parse_ok_;
        AtomList res_;
        AtomList prop_;
    };

}
}

#endif // LEMON_DATA_STRING_PARSER_H
