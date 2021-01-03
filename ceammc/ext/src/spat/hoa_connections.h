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
#ifndef HOA_CONNECTIONS_H
#define HOA_CONNECTIONS_H

#include "ceammc_object.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class HoaXlet : public BaseObject {
    IntProperty* extra_;

public:
    HoaXlet(const PdArgs& args);
    int extra() const { return extra_->value(); }
};

class HoaIn : public HoaXlet {
public:
    HoaIn(const PdArgs& args);
    void onBang() final;
    void onFloat(t_float v) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomList& l) final;
    void onAny(t_symbol* s, const AtomListView& lv) final;

public:
    static HoaIn* fromObject(void* obj);
    static bool isA(void* obj);
};

class HoaOut : public HoaXlet {
    t_outlet* outlet_;

public:
    HoaOut(const PdArgs& args);
    void onBang() final;
    void onFloat(t_float v) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomList& l) final;
    void onAny(t_symbol* s, const AtomListView& l) final;

    bool processAnyProps(t_symbol* sel, const AtomListView& lst) override;

    void setOutlet(t_outlet* x);

public:
    static HoaOut* fromObject(void* obj);
    static bool isA(void* obj);
};

class HoaXletTilde : public SoundExternal {
    IntProperty* extra_;

protected:
    t_sample* signal_;

public:
    HoaXletTilde(const PdArgs& args);
    int extra() const { return extra_->value(); }
    void setSignal(t_sample* sig);

    void processBlock(const t_sample** in, t_sample** out) final;
};

class HoaInTilde : public HoaXletTilde {
public:
    HoaInTilde(const PdArgs& args);
    void setupDSP(t_signal** sp) final;

public:
    static HoaInTilde* fromObject(void* obj);
    static bool isA(void* obj);
};

class HoaOutTilde : public HoaXletTilde {
public:
    HoaOutTilde(const PdArgs& args);
    void setupDSP(t_signal** sp) final;

public:
    static HoaOutTilde* fromObject(void* obj);
    static bool isA(void* obj);
};

void setup_spat_hoa_connections();

#endif // HOA_CONNECTIONS_H
