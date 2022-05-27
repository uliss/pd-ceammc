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
#ifndef GRAIN_EXPR_PARSER_H
#define GRAIN_EXPR_PARSER_H

#include "byte_code.h"
#include "ceammc_atomlist_view.h"
#include "grain.h"

namespace ceammc {

class GrainExprParser {
    GrainExprParser(const GrainExprParser&) = delete;
    GrainExprParser& operator=(const GrainExprParser&) = delete;

public:
    GrainExprParser(Grain* g = nullptr);

    Grain* grain() { return grain_; }
    void setGrain(Grain* g) { grain_ = g; }

    GrainCalcMoment calcMoment() const { return calc_moment_; }
    void setCalcMoment(GrainCalcMoment moment) { calc_moment_ = moment; }

    const char* errorMsg() const { return err_buf_; }
    void setErrorMsg(const char* str);
    void clearErrorMsg() { err_buf_[0] = '\0'; }

    bool parse(const char* data);
    bool parse(const AtomListView& lv);

    double result() const { return res_; }

    void bcPushValue(double v);
    void bcPopValue();
    void bcPushCode(ByteCodeOp op);
    void bcVarArg() { num_vargs_++; }
    void bcPushVarArgs() { bcPushValue(num_vargs_); num_vargs_ = 0; }

    void setAssignType(ByteCodeAssignType type);

    void setPanOverflow(GrainPropOverflow of);
    void setPanMode(GrainPan m);

    const ByteCode& byteCode() const { return bc_; }
    bool byteCodeEval();

    GrainPropId currentProp() const { return current_prop_; }
    void startProp(GrainPropId prop);

    void setWinType(GrainWindowType wt);
    void setWinParam(double v);
    void setInterp(GrainInterp i);
    void setModulation(double a, double b, GrainModulation m, float freq);

    void setOnDone();

    void saveGrainTag(t_symbol* t);

public:
    constexpr static size_t PARSER_SIZE = 350;

private:
    bool lexAtom(const Atom& a);
    void reset();
    bool parseEof();
    bool doParse(const char* data);

    double grainAmp() const { return grain_ ? grain_->amplitude() : 1; }
    double grainAt() const { return grain_ ? grain_->arrayPosInSamples() : 0; }
    float grainPan() const { return grain_ ? grain_->pan() : 0.5; }
    float grainSpeed() const { return grain_ ? grain_->speed() : 1; }
    float grainTimeBefore() const { return grain_ ? grain_->timeBefore() : 0; }
    float grainTimeAfter() const { return grain_ ? grain_->timeAfter() : 0; }
    double grainLength() const { return grain_ ? grain_->durationInSamples() : 0; }
    double grainRepeatCount() const { return grain_ ? grain_->doneCounter() : 0; }
    double grainRepeats() const { return grain_ ? grain_->repeats() : -1; }

    bool grainPropGet(double& val) const;
    bool grainPropSet(double val);

    inline void* parser() { return parser_data_; }

    void pushToken(int token);
    void pushDouble(double val);
    void pushSymbol(t_symbol* sym);

private:
    Grain* grain_; ///< grain pointer
    char parser_data_[PARSER_SIZE]; ///< parser is on the stack to avoid dynamic alloc
    ByteCode bc_;
    char err_buf_[64] = { 0 };
    double res_;
    int curtok_ = { -1 };
    int num_vargs_ = { 0 };

    bool parse_ok_;
    bool bc_err_;
    GrainPropId current_prop_;
    GrainCalcMoment calc_moment_;
};

}

#endif // GRAIN_EXPR_PARSER_H
