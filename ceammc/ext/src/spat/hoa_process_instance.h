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
#ifndef HOA_PROCESS_INSTANCE_H
#define HOA_PROCESS_INSTANCE_H

#include <forward_list>

#include "hoa_connections.h"

class ProcessInstance {
    t_canvas* canvas_;
    std::forward_list<HoaIn*> f_ins;
    std::forward_list<HoaOut*> f_outs;
    std::forward_list<HoaInTilde*> f_ins_sig;
    std::forward_list<HoaOutTilde*> f_outs_sig;

public:
    ProcessInstance();

public:
    void setCanvas(t_canvas* c);
    const t_canvas* canvas() const { return canvas_; }
    t_canvas* canvas() { return canvas_; }

    void loadBang();
    void show();
    void scanCanvas(t_canvas* cnv);

    void bangTo(size_t inlet_idx);
    void floatTo(size_t inlet_idx, t_float v);
    void symbolTo(size_t inlet_idx, t_symbol* s);
    void listTo(size_t inlet_idx, const AtomList& l);

    bool hasStaticInputSignal() const;
    bool hasStaticOutputSignal() const;
    size_t numExtraSignalInputs() const;
    size_t numExtraSignalOutputs() const;
    size_t numControlInputs() const;
    size_t numControlOutputs() const;
    void setOutlet(t_outlet* outl, size_t idx);
    void setInletBuffer(t_sample* s, size_t idx);
    void setOutletBuffer(t_sample* s, size_t idx);
};

#endif // HOA_PROCESS_INSTANCE_H
