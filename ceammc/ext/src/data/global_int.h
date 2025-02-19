/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef GLOBAL_INT_H
#define GLOBAL_INT_H

#include "data_protocol.h"
#include "global_base.h"

using namespace ceammc;

typedef t_int GlobalIntType;
using GlobalIntBase = NumericIFace<GlobalBase<GlobalIntType>, GlobalIntType>;

class GlobalInt : public GlobalIntBase {
public:
    GlobalInt(const PdArgs& a);
    GlobalIntType& value() final { return ref(); }
    const GlobalIntType& value() const final { return ref(); }
};

void setup_global_int();

#endif // GLOBAL_INT_H
