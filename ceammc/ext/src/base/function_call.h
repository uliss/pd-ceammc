/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef FUNCTION_CALL_H
#define FUNCTION_CALL_H

#include "ceammc_object.h"

using namespace ceammc;

class Function;

class FunctionCall : public BaseObject {
    t_symbol* name_;

public:
    FunctionCall(const PdArgs& a);
    void onBang() final;
    void onFloat(t_float f) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomListView& lv) final;

private:
    Function* getFunc();
    void outputResult(Function* fn);
};

void setup_function_call();

#endif // FUNCTION_CALL_H
