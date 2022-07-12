#ifndef LEMON_DATA_STRING_PARSER_H
#define LEMON_DATA_STRING_PARSER_H

#include <boost/pool/object_pool.hpp>
#include <cstddef>

#include "ceammc_atomlist.h"
#include "ceammc_containers.h"
#include "ragel_common.h"

using SmallList = ceammc::SmallAtomListN<8>;
using SmallListPool = boost::object_pool<SmallList>;

namespace ceammc {
namespace parser {

    namespace fsm {
        class NumericData;
    }

    class LemonDataStringParser {
    public:
        LemonDataStringParser();
        ~LemonDataStringParser();

        bool parse(const char* str);
        bool doParse(const char* data);

        inline void* parser() { return parser_data_; }

        void pushToken(int token);
        void pushSymbolToken(int token, const char* begin, const char* end);
        void pushFloat(double val);

        void pPushListAtom(const t_atom& a);

        AtomListView result() const { return res_.view(); }

        void setErrorMsg(const char* msg);
        void parseFailure();
        void parseAccept() { parse_ok_ = true; }
        void stackOverflow();

        SmallListPool& pool() { return pool_; }
        const SmallListPool& pool() const { return pool_; }
        SmallList* makeList() { return pool_.construct(); }

    public:
        constexpr static size_t PARSER_SIZE = 1024;

    private:
        void reset();
        void onFloat(AtomCategory cat, AtomType type, const fsm::NumericData& num);

    private:
        char parser_data_[PARSER_SIZE]; ///< parser is on the stack to avoid dynamic alloc
        char parser_buf_[256];
        char err_buf_[64];
        bool parse_ok_;
        SmallList res_;
        SmallListPool pool_;
        // ragel fsm
        int top;
        int stack[16];
    };

}
}

#endif // LEMON_DATA_STRING_PARSER_H
