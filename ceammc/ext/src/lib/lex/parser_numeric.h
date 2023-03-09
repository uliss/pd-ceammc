/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef RAGEL_NUMERIC_H
#define RAGEL_NUMERIC_H

#include <cstddef>
#include <cstdint>

#include <boost/container/small_vector.hpp>

#include "ceammc_atomlist_view.h"
#include "ragel_common.h"

namespace ceammc {
namespace parser {

    template <size_t N>
    using SmallFVecN = boost::container::small_vector<t_float, N>;
    using SmallFVec = SmallFVecN<8>;

    namespace fsm {
        struct NumericData {
            double vdouble { 0 };
            int64_t vint { 0 };
            struct ratio {
                int64_t num { 0 };
                int64_t den { 0 };
            } ratio;

            double getFloat() const { return vdouble; }
            int64_t getInteger() const { return vint; }
            struct ratio getRatio() const { return ratio; }
            double getRatioAsFloat() const { return double(ratio.num) / ratio.den; }

            int8_t sign { 1 };
        };

    }

#ifndef DECLARE_RAGEL_NUMERIC_VARS
#define DECLARE_RAGEL_NUMERIC_VARS fsm::NumericData ragel_num
#endif

    struct NumericResult {
        AtomType t { TYPE_UNKNOWN };
        int64_t i0 { 0 }, i1 { 0 };
        double f { 0 };

        double asFloat(double def) const
        {
            switch (t) {
            case TYPE_PHASE:
            case TYPE_INF:
            case TYPE_FLOAT:
                return f;
            case TYPE_INT:
            case TYPE_HEX:
            case TYPE_BIN:
                return i0;
            case TYPE_RATIO:
                if (i1 == 0)
                    return 0;

                return double(i0) / double(i1);
            case TYPE_PERCENT:
                return f / 100.0;
            default:
                return def;
            }
        }

        double percent(double def = 0) const
        {
            return t == TYPE_PERCENT ? f : def;
        }
    };

    class NumericFullMatch {
        int cs { 0 };
        NumericResult res_;

    public:
        NumericFullMatch();
        void reset();

        AtomType type() const { return res_.t; }
        bool isInt() const { return res_.t == TYPE_INT; }
        bool isBin() const { return res_.t == TYPE_BIN; }
        bool isHex() const { return res_.t == TYPE_HEX; }
        bool isRatio() const { return res_.t == TYPE_RATIO; }
        bool isFloat() const { return res_.t == TYPE_FLOAT; }
        bool isInfinity() const { return res_.t == TYPE_INF; }
        bool isPercent() const { return res_.t == TYPE_PERCENT; }
        bool isPhase() const { return res_.t == TYPE_PHASE; }

        double asFloat(double def = 0.0) const { return res_.asFloat(def); }

        bool getBin(t_int& b) const;
        bool getInt(t_int& i) const;
        bool getHex(t_int& x) const;
        bool getFloat(t_float& f) const;
        bool getRatio(t_int& num, t_int& den) const;

        const NumericResult& result() const { return res_; }

        bool parse(const char* str);
        bool parse(const Atom& a);
        bool parseAll(const AtomListView& lv, SmallFVec& out);
        size_t parseSome(const AtomListView& lv, SmallFVec& out);
    };

    class NumericMatchSome {
    public:
        using ResultList = boost::container::small_vector<NumericResult, 16>;

    private:
        ResultList res_;
        int cs { 0 };

    public:
        NumericMatchSome();
        void reset();

        bool parseSingle(const char* str, const char*& endptr);
        size_t parseMany(const char* str, const char*& endptr);

        size_t size() const { return res_.size(); }
        const ResultList& result() const { return res_; }

        bool isIntAt(size_t idx) const { return idx < res_.size() && res_[idx].t == TYPE_INT; }
        bool isBinAt(size_t idx) const { return idx < res_.size() && res_[idx].t == TYPE_BIN; }
        bool isHexAt(size_t idx) const { return idx < res_.size() && res_[idx].t == TYPE_HEX; }
        bool isRatioAt(size_t idx) const { return idx < res_.size() && res_[idx].t == TYPE_RATIO; }
        bool isFloatAt(size_t idx) const { return idx < res_.size() && res_[idx].t == TYPE_FLOAT; }
        bool isInfinityAt(size_t idx) const { return idx < res_.size() && res_[idx].t == TYPE_INF; }

        bool getBinAt(size_t idx, t_int& b) const;
        bool getIntAt(size_t idx, t_int& i) const;
        bool getHexAt(size_t idx, t_int& x) const;
        bool getFloatAt(size_t idx, t_float& f) const;
        bool getRatioAt(size_t idx, t_int& num, t_int& den) const;

        double asFloatAt(size_t idx, double def = 0.0) const;
    };
}
}

#endif // RAGEL_NUMERIC_H
