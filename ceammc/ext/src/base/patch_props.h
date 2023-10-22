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
#ifndef PATCH_PROPS_H
#define PATCH_PROPS_H

#include "ceammc_object.h"
using namespace ceammc;

class PatchProps : public BaseObject {
public:
    PatchProps(const PdArgs& args);
    bool processAnyProps(t_symbol* sel, const AtomListView& lv) override;
    void onBang() override;
    void onAny(t_symbol* s, const AtomListView& l) override;
    void dump() const final;
    void outputAllProperties() final;

    void m_default(t_symbol*, const AtomListView&);

private:
    void outputProp(const std::string& name, t_float f);
    void outputProp(const std::string& name, t_symbol* s);
    void outputProp(const std::string& name, const AtomList& l);
};

void setup_patch_props();

#endif // PATCH_PROPS_H
