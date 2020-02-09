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
#ifndef FLOW_PACK_H
#define FLOW_PACK_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowPack : public BaseObject {
private:
    size_t n_;

protected:
    AtomList msg_;

public:
    FlowPack(const PdArgs& args);
    void parseProperties() final;

    void onBang() final;
    void onFloat(t_float f) final;
    void onSymbol(t_symbol* s) final;
    void onInlet(size_t idx, const AtomList& l) final;
    void onList(const AtomList& l) final;
    void onAny(t_symbol* s, const AtomList& l) final;
    bool processAnyProps(t_symbol* s, const AtomList& l) final;

    virtual void output(size_t inlet_idx);
};

void setup_flow_pack();

#endif // FLOW_PACK_H
