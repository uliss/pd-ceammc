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
#include "byte_code.h"
#include "ceammc_convert.h"
#include "grain_random.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>

namespace ceammc {

static const char* op_str[] = {
    "push",
    "pop",
    "+",
    "-",
    "*",
    "/",
    "pow",
    "%",
    "abs",
    "sqrt",
    "sin",
    "cos",
    "tan",
    "asin",
    "acos",
    "atan",
    "ceil",
    "round",
    "floor",
    "trunc",
    "log",
    "log10",
    "log2",
    "frac",
    "wrap0",
    "wrap1",
    "wrap2",
    "fold0",
    "fold1",
    "clip",
    "rand0",
    "rand1",
    "rand2",
    "choice",
    "$0",
    "$1",
    "$2",
    "$3",
    "$4",
    "$5",
    "$6",
    "$7",
    "$8",
    "$9",
    "$10",
    "$11",
    "$12",
    "$13",
};

static const char* toString(ByteCodeOp op)
{
    return op_str[op];
}

static_assert(OP_MAX_ == sizeof(op_str) / sizeof(op_str[0]), "");

constexpr size_t VSIZE = sizeof(BcValueType) / sizeof(BcOpType);

ByteCode::ByteCode()
    : assign_type_(BC_ASSIGN_SET)
{
}

BcValueType ByteCode::valueAt(size_t n) const
{
    BcValueType res = 0;
    auto pv = static_cast<BcOpType*>(reinterpret_cast<void*>(&res));

    for (size_t i = 0; i < VSIZE; i++)
        pv[i] = bc_[n + i];

    return res;
}

void ByteCode::reset()
{
    bp_ = 0;
    assign_type_ = BC_ASSIGN_SET;
}

#ifndef NDEBUG
#define PRINT_RESULT(op) printf("[%s] =%f\n", toString(static_cast<ByteCodeOp>(op)), stack.top());
#else
#define PRINT_RESULT(op)
#endif

bool ByteCode::eval(BcValueType& res) const
{
    if (bp_ == 0)
        return false;

    ValueStack stack;

    int i = 0;
    while (i < bp_) {
        auto ii = evalOp(i, stack);

        PRINT_RESULT(bc_[i]);

        if (ii > 0 && ii < bp_)
            i += ii;
        else if (ii == OP_ERR_STACK_OVERFLOW) {
            printf("stack overflow\n");
            break;
        } else
            break;
    }

    if (stack.size() == 1) {

        switch (assign_type_) {
        case BC_ASSIGN_ADD:
            res += stack.top();
            break;
        case BC_ASSIGN_SUB:
            res -= stack.top();
            break;
        case BC_ASSIGN_MUL:
            res *= stack.top();
            break;
        case BC_ASSIGN_SET:
        default:
            res = stack.top();
            break;
        }

        return true;
    } else {
        std::cerr << "stack is not fully reduced: " << stack.size() << std::endl;
        return false;
    }
}

bool ByteCode::optimize()
{
    return true;
}

void ByteCode::setConst(size_t n, BcValueType val)
{
    if (n < const_.size())
        const_[n] = val;
    else {
#ifndef NDEBUG
        std::cerr << "invalid constant index: " << n << "\n";
#endif
    }
}

bool ByteCode::pushByteValue(BcValueType val)
{
    if ((bp_ + VSIZE + 1) < bc_.size()) {
        bc_[bp_++] = OP_PUSH;

        auto op = static_cast<BcOpType*>(reinterpret_cast<void*>(&val));

        for (size_t i = 0; i < VSIZE; i++)
            bc_[bp_++] = op[i];

        return true;
    } else
        return false;
}

bool ByteCode::popByteValue()
{
    if (bp_ >= (VSIZE + 1)) {
        bp_ -= (VSIZE + 1);
        return true;
    } else
        return false;
}

bool ByteCode::pushByteCode(ByteCodeOp op)
{
    if (bp_ < bc_.size()) {
        bc_[bp_++] = op;
        return true;
    } else
        return false;
}

#define MIN_STACK_SIZE(n) assert(stack.size() >= n)

int ByteCode::evalOp(int i, ValueStack& stack) const
{
    const int op = bc_[i];

    switch (op) {
    case OP_PUSH: {
        if (stack.push(valueAt(i + 1)))
            return 1 + VSIZE;
    } break;
    case OP_POP:
        MIN_STACK_SIZE(VSIZE);
        stack.pop();
        return 1;
    case OP_ADD: {
        MIN_STACK_SIZE(2);
        stack[1] += stack[0];
        stack.pop();
        return 1;
    }
    case OP_SUB: {
        MIN_STACK_SIZE(2);
        stack[1] -= stack[0];
        stack.unsafePop();
        return 1;
    }
    case OP_MUL: {
        MIN_STACK_SIZE(2);
        stack[1] *= stack[0];
        stack.unsafePop();
        return 1;
    }
    case OP_DIV: {
        MIN_STACK_SIZE(2);
        stack[1] /= stack[0];
        stack.unsafePop();
        return 1;
    }
    case OP_POW: {
        MIN_STACK_SIZE(2);
        stack[1] = std::pow(stack[1], stack[0]);
        stack.unsafePop();
        return 1;
    }
    case OP_MOD: {
        MIN_STACK_SIZE(2);
        stack[1] = std::fmod(stack[1], stack[0]);
        stack.unsafePop();
        return 1;
    }
    case OP_ABS: {
        MIN_STACK_SIZE(1);
        stack.top() = std::abs(stack.top());
        return 1;
    }
    case OP_SQRT: {
        MIN_STACK_SIZE(1);
        stack.top() = std::sqrt(stack.top());
        return 1;
    }
    case OP_SIN: {
        MIN_STACK_SIZE(1);
        stack.top() = std::sin(stack.top());
        return 1;
    }
    case OP_COS: {
        MIN_STACK_SIZE(1);
        stack.top() = std::cos(stack.top());
        return 1;
    }
    case OP_TAN: {
        MIN_STACK_SIZE(1);
        stack.top() = std::tan(stack.top());
        return 1;
    }
    case OP_ASIN: {
        MIN_STACK_SIZE(1);
        stack.top() = std::asin(stack.top());
        return 1;
    }
    case OP_ACOS: {
        MIN_STACK_SIZE(1);
        stack.top() = std::acos(stack.top());
        return 1;
    }
    case OP_ATAN: {
        MIN_STACK_SIZE(1);
        stack.top() = std::atan(stack.top());
        return 1;
    }
    case OP_CEIL: {
        MIN_STACK_SIZE(1);
        stack.top() = std::ceil(stack.top());
        return 1;
    }
    case OP_ROUND: {
        MIN_STACK_SIZE(1);
        stack.top() = std::round(stack.top());
        return 1;
    }
    case OP_FLOOR: {
        MIN_STACK_SIZE(1);
        stack.top() = std::floor(stack.top());
        return 1;
    }
    case OP_TRUNC: {
        MIN_STACK_SIZE(1);
        stack.top() = std::trunc(stack.top());
        return 1;
    }
    case OP_FRAC: {
        MIN_STACK_SIZE(1);
        stack.top() = std::fmod(stack.top(), 1);
        return 1;
    }
    case OP_LOG: {
        MIN_STACK_SIZE(1);
        stack.top() = std::log(stack.top());
        return 1;
    }
    case OP_LOG10: {
        MIN_STACK_SIZE(1);
        stack.top() = std::log10(stack.top());
        return 1;
    }
    case OP_LOG2: {
        MIN_STACK_SIZE(1);
        stack.top() = std::log2(stack.top());
        return 1;
    }
    case OP_WRAP0: {
        MIN_STACK_SIZE(1);
        stack.top() = wrapFloatMax<double>(stack.top(), 1);
        return 1;
    }
    case OP_WRAP1: {
        MIN_STACK_SIZE(2);
        stack[1] = wrapFloatMax(stack[1], stack[0]);
        stack.unsafePop();
        return 1;
    }
    case OP_WRAP2: {
        MIN_STACK_SIZE(3);
        stack[2] = wrapFloatMinMax(stack[2], stack[1], stack[0]);
        stack.unsafePop();
        stack.unsafePop();
        return 1;
    }
    case OP_FOLD0: {
        MIN_STACK_SIZE(1);
        auto w = wrapFloatMax<double>(stack.top(), 2);
        stack.top() = std::min<float>(2 - w, w);
        return 1;
    }
    case OP_FOLD1: {
        MIN_STACK_SIZE(2);
        auto w = wrapFloatMax(stack[1], 2 * stack[0]);
        stack[1] = std::min<float>(2 * stack[0] - w, w);;
        stack.unsafePop();
        return 1;
    }
    case OP_CLIP: {
        MIN_STACK_SIZE(3);
        stack[2] = clip(stack[2], stack[1], stack[0]);
        stack.unsafePop();
        stack.unsafePop();
        return 1;
    }
    case OP_URAND0: {
        if (stack.push(GrainRandom::urandf(0, 1)))
            return 1;
    } break;
    case OP_URAND1: {
        MIN_STACK_SIZE(1);
        stack.top() = GrainRandom::urandf(0, stack.top());
        return 1;
    }
    case OP_URAND2: {
        MIN_STACK_SIZE(2);
        stack[1] = GrainRandom::urandf(stack[1], stack[0]);
        stack.unsafePop();
        return 1;
    }
    case OP_CHOICE: {
        MIN_STACK_SIZE(1);
        int num = stack.top();

        // TODO: check num!
        stack.pop();
        double val = 0;
        const int random_idx = GrainRandom::urandul(0, num - 1);
        while (--num >= 0) {
            if (num == random_idx)
                val = stack.top();

            stack.pop();
        }
        stack.push(val);
        return 1;
    }
    case OP_CONST0: {
        if (stack.push(const_[0]))
            return 1;
    } break;
    case OP_CONST1: {
        if (stack.push(const_[1]))
            return 1;
    } break;
    case OP_CONST2: {
        if (stack.push(const_[2]))
            return 1;
    } break;
    case OP_CONST3: {
        if (stack.push(const_[3]))
            return 1;
    } break;
    case OP_CONST4: {
        if (stack.push(const_[4]))
            return 1;
    } break;
    case OP_CONST5: {
        if (stack.push(const_[5]))
            return 1;
    } break;
    case OP_CONST6: {
        if (stack.push(const_[6]))
            return 1;
    } break;
    case OP_CONST7: {
        if (stack.push(const_[7]))
            return 1;
    } break;
    case OP_CONST8: {
        if (stack.push(const_[8]))
            return 1;
    } break;
    case OP_CONST9: {
        if (stack.push(const_[9]))
            return 1;
    } break;
    case OP_CONST10: {
        if (stack.push(const_[10]))
            return 1;
    } break;
    case OP_CONST11: {
        if (stack.push(const_[11]))
            return 1;
    } break;
    case OP_CONST12: {
        if (stack.push(const_[12]))
            return 1;
    } break;
    case OP_CONST13: {
        if (stack.push(const_[13]))
            return 1;
    } break;
    default:
        return OP_ERR_UNKNOWN_OP;
    }

    return OP_ERR_STACK_OVERFLOW;
}

std::ostream& operator<<(std::ostream& os, const ByteCode& bc)
{
    os << "ByteCode:\n__________________\n";
    const auto N = bc.size();
    for (size_t i = 0; i < N; i++) {
        const auto op = bc.opAt(i);
        switch (op) {
        case OP_PUSH:
            if ((i + VSIZE) < N) {
                os << op_str[op] << ' ' << bc.valueAt(i + 1) << '\n';
                i += VSIZE;
            } else
                os << op_str[op] << "?\n";

            break;
        default:
            os << op_str[op] << "\n";
            break;
        }
    }
    os << "------------------\n";
    return os;
}
}
