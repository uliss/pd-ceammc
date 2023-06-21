/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef MSG_PACK_H
#define MSG_PACK_H

#include "ceammc_object.h"
using namespace ceammc;

class MsgPack : public BaseObject {
    IntProperty* n_ { nullptr };
    ListProperty* prefix_ { nullptr };
    BoolProperty* sync_ { nullptr };
    AtomList msg_;

public:
    MsgPack(const PdArgs& args);
    void initDone() final;

    void onBang() final;
    void onFloat(t_float f) final;
    void onSymbol(t_symbol* s) final;
    void onInlet(size_t n, const AtomListView& lv) final;

private:
    void updateMessage();
    void output();
    size_t prefixSize() const { return prefix_->value().size(); }
    bool checkIndex(size_t n) const { return prefixSize() + n < msg_.size(); }
    Atom& at(size_t n) { return msg_[prefixSize() + n]; }
};

void setup_base_msg_pack();

#endif // MSG_PACK_H
