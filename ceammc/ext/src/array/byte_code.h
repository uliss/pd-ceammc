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
#ifndef BYTE_CODE_H
#define BYTE_CODE_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <iosfwd>

namespace ceammc {

template <typename T, int N>
class StaticStack {
    T stack_[N];
    int sp_;

public:
    StaticStack()
        : sp_(0)
    {
        stack_[0] = 0;
    }

    bool pop()
    {
        if (sp_ > 0) {
            sp_--;
            return true;
        } else
            return false;
    }

    bool push(T v)
    {
        if (sp_ < N) {
            stack_[sp_++] = v;
            return true;
        } else
            return false;
    }

    bool empty() const { return sp_ == 0; }
    size_t size() const { return sp_; }

    // unsafe
    T& top() { return stack_[sp_ - 1]; }
    const T& top() const { return stack_[sp_ - 1]; }
    T& operator[](size_t n) { return stack_[sp_ - (n + 1)]; }
    const T& operator[](size_t n) const { return stack_[sp_ - (n + 1)]; }
    void unsafePop() { sp_--; }
};

enum ByteCodeOp : uint8_t {
    OP_PUSH,
    OP_POP,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
    OP_MOD,
    OP_ABS,
    OP_SQRT,
    OP_SIN,
    OP_COS,
    OP_TAN,
    OP_ASIN,
    OP_ACOS,
    OP_ATAN,
    OP_CEIL,
    OP_ROUND,
    OP_FLOOR,
    OP_TRUNC,
    OP_LOG,
    OP_LOG10,
    OP_LOG2,
    OP_FRAC,
    OP_WRAP0,
    OP_WRAP1,
    OP_WRAP2,
    OP_FOLD0,
    OP_FOLD1,
    OP_CLIP,
    OP_URAND0,
    OP_URAND1,
    OP_URAND2,
    OP_CHOICE,
    OP_CONST0,
    OP_CONST1,
    OP_CONST2,
    OP_CONST3,
    OP_CONST4,
    OP_CONST5,
    OP_CONST6,
    OP_CONST7,
    OP_CONST8,
    OP_CONST9,
    OP_CONST10,
    OP_CONST11,
    OP_CONST12,
    OP_CONST13,
    OP_CONSTMAX,
    OP_MAX_
};

constexpr const size_t OP_NUM_CONST = OP_CONSTMAX - OP_CONST0;

enum ByteCodeErr {
    OP_ERR_UNKNOWN_OP = -1,
    OP_ERR_STACK_OVERFLOW = -2,
};

enum ByteCodeAssignType : char {
    BC_ASSIGN_SET,
    BC_ASSIGN_ADD,
    BC_ASSIGN_SUB,
    BC_ASSIGN_MUL
};

using BcOpType = uint8_t;
using BcValueType = double;

class ByteCode {
    static const int MAX_LEN { 128 };
    static const int STACK_SIZE { 16 };

    using ValueStack = StaticStack<BcValueType, STACK_SIZE>;

    std::array<BcOpType, MAX_LEN> bc_;
    std::array<BcValueType, OP_NUM_CONST> const_;
    uint16_t bp_ { 0 };
    ByteCodeAssignType assign_type_;

public:
    ByteCode();
    ByteCode(const ByteCode&) = default;

    bool empty() const { return bp_ == 0; }
    size_t size() const { return bp_; }
    BcOpType opAt(size_t n) const { return bc_[n]; }
    BcValueType valueAt(size_t n) const;

    void reset();
    bool eval(BcValueType& res) const;
    bool optimize();

    void setConst(size_t n, BcValueType val);

    bool pushByteValue(BcValueType val);
    bool popByteValue();
    bool pushByteCode(ByteCodeOp op);

    ByteCodeAssignType assignType() const { return assign_type_; }
    void setAssignType(ByteCodeAssignType type) { assign_type_ = type; }

private:
    /**
     * @brief evalOp
     * @param i - current byte code index
     * @param stack - value stack
     * @return current byte code size in bytes
     */
    int evalOp(int i, ValueStack& stack) const;
};

std::ostream& operator<<(std::ostream& os, const ByteCode& bc);

}

#endif // BYTE_CODE_H
