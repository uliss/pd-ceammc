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

#include "ceammc_canvas.h"
#include "hoa_connections.h"

class ProcessInstance {
    Canvas canvas_;
    t_object* switch_;
    std::forward_list<HoaIn*> f_ins;
    std::forward_list<HoaOut*> f_outs;
    std::forward_list<HoaInTilde*> f_ins_sig;
    std::forward_list<HoaOutTilde*> f_outs_sig;
    AtomList args_;
    bool dsp_state_;

public:
    ProcessInstance();
    ~ProcessInstance();

public:
    void setCanvas(t_canvas* c);
    const Canvas& canvas() const { return canvas_; }
    Canvas& canvas() { return canvas_; }

    void loadBang();
    void show();
    void scanCanvas();
    void createSwitch();
    void dspOn(bool state);
    void dspCalc();
    bool init(t_symbol* name, const AtomListView& args);

    void bangTo(size_t inlet_idx);
    void floatTo(size_t inlet_idx, t_float v);
    void symbolTo(size_t inlet_idx, t_symbol* s);
    void listTo(size_t inlet_idx, const AtomList& l);
    void anyTo(size_t inlet_idx, t_symbol* s, const AtomListView& l);

    bool hasStaticInputSignal() const;
    bool hasStaticOutputSignal() const;
    size_t numExtraSignalInputs() const;
    size_t numExtraSignalOutputs() const;
    size_t numControlInputs() const;
    size_t numControlOutputs() const;
    void setOutlet(t_outlet* outl, size_t idx);
    void setInletBuffer(t_sample* s, size_t idx);
    void setOutletBuffer(t_sample* s, size_t idx);

    const AtomList& args() const { return args_; }
    void setArgs(const AtomListView& args) { args_ = args; }

private:
    void doScanCanvas(t_canvas* cnv);
};

#endif // HOA_PROCESS_INSTANCE_H
