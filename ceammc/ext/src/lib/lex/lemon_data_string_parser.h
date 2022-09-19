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

        /**
         * Parse specified string
         * @param str - C-string to parse
         * @return true on success, false on error
         */
        bool parse(const char* str) noexcept;

        /**
         * Returns error string if it occured while parsing
         */
        const std::string errorString() const { return error_; }

        /**
         * Return parsing result
         */
        AtomListView result() const { return res_ ? res_->view() : AtomListView(); }

        /// functions for lemon parser
        void setResult(SmallList* res) { res_ = res; }

        void onParseFailure();
        void onParseAccept() { parse_ok_ = true; }
        void onStackOverflow();

        SmallListPool& pool() { return pool_; }
        const SmallListPool& pool() const { return pool_; }
        SmallList* makeList() { return pool_.construct(); }

    public:
        constexpr static size_t PARSER_SIZE = 1096;

    private:
        inline void* parser() { return parser_data_; }
        void reset();
        void onFloat(AtomCategory cat, AtomType type, const fsm::NumericData& num);
        void pushToken(int token);
        void pushSymbolToken(int token, const char* begin, const char* end);
        void pushFloat(double val);

    private:
        char parser_data_[PARSER_SIZE]; ///< parser is on the stack to avoid dynamic alloc
        char parser_buf_[256];
        bool parse_ok_;
        SmallList* res_;
        SmallListPool pool_;
        std::string error_;
    };

}
}

#endif // LEMON_DATA_STRING_PARSER_H
