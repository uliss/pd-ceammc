/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef MATH_SYNC_OP_H
#define MATH_SYNC_OP_H

#include "math_sync_base.h"

using namespace ceammc;

class MathSyncMul : public MathSyncBase {
public:
    MathSyncMul(const PdArgs& args);
};

class MathSyncDiv : public MathSyncBase {
public:
    MathSyncDiv(const PdArgs& args);
};

class MathSyncAdd : public MathSyncBase {
public:
    MathSyncAdd(const PdArgs& args);
};

class MathSyncSub : public MathSyncBase {
public:
    MathSyncSub(const PdArgs& args);
};

class MathSyncEqual : public MathSyncBase {
    FloatProperty* epsilon_;

public:
    MathSyncEqual(const PdArgs& args);
};

class MathSyncNotEqual : public MathSyncBase {
    FloatProperty* epsilon_;

public:
    MathSyncNotEqual(const PdArgs& args);
};

class MathSyncLessThen : public MathSyncBase {
public:
    MathSyncLessThen(const PdArgs& args);
};

class MathSyncLessEqual : public MathSyncBase {
public:
    MathSyncLessEqual(const PdArgs& args);
};

class MathSyncGreaterThen : public MathSyncBase {
public:
    MathSyncGreaterThen(const PdArgs& args);
};

class MathSyncGreaterEqual : public MathSyncBase {
public:
    MathSyncGreaterEqual(const PdArgs& args);
};

class MathSyncMod : public MathSyncBase {
public:
    MathSyncMod(const PdArgs& args);
};

class MathSyncAnd : public MathSyncBool {
public:
    MathSyncAnd(const PdArgs& args);
};

class MathSyncOr : public MathSyncBool {
public:
    MathSyncOr(const PdArgs& args);
};

class MathSyncXor : public MathSyncBool {
public:
    MathSyncXor(const PdArgs& args);
};

class MathSyncLeftShift : public MathSyncBase {
public:
    MathSyncLeftShift(const PdArgs& args);
};

class MathSyncRightShift : public MathSyncBase {
public:
    MathSyncRightShift(const PdArgs& args);
};

void setup_math_sync_op();

#endif // MATH_SYNC_OP_H
