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
#ifndef GLOBAL_MLIST_H
#define GLOBAL_MLIST_H

#include "global_base.h"
#include "mlist_iface.h"

typedef DataMListIFace<GlobalBase<MListAtom>> GlobalMListBase;

class GlobalMList : public GlobalMListBase {
public:
    GlobalMList(const PdArgs& args);
    MListAtom& mlist() override { return ref(); }
    const MListAtom& mlist() const override { return ref(); }
};

void setup_global_mlist();

#endif // GLOBAL_MLIST_H
