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
#ifndef HOA_PROCESS_H
#define HOA_PROCESS_H

#include "hoa_common.h"
#include "hoa_connections.h"
#include "hoa_process_inlet.h"

#include <forward_list>

typedef void (*t_bangmethod)(t_pd* x);

struct t_hoa_process_instance {
private:
    t_canvas* canvas_;

public:
    t_hoa_process_instance();

    std::forward_list<HoaIn*> f_ins;
    std::forward_list<HoaOut*> f_outs;
    std::forward_list<HoaInTilde*> f_ins_sig;
    std::forward_list<HoaOutTilde*> f_outs_sig;

public:
    void setCanvas(t_canvas* c);
    const t_canvas* canvas() const { return canvas_; }
    t_canvas* canvas() { return canvas_; }

    void loadBang();
    void show();
    void scanCanvas(t_canvas* cnv);

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

class HoaProcess : public HoaBase {
    t_bangmethod method_;
    t_object* block_;
    std::vector<t_hoa_process_instance> instances_;
    t_canvas* hoa_canvas_;
    t_float canvas_yoff_;

    size_t target_;

    std::vector<t_hoa_process_inlet> ins_;
    Buffer in_buf_;
    Buffer out_buf_;

    SymbolEnumProperty* domain_;

public:
    HoaProcess(const PdArgs& args);
    ~HoaProcess();

    void parseProperties() override;
    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** sp) final;

    void m_click(t_symbol* m, const AtomList& lst);
    void m_open_cnv(t_symbol* m, const AtomList& lst);

private:
    bool init();

    bool loadHarmonics(t_symbol* name, const AtomList& patch_args);
    bool processInstanceInit(t_hoa_process_instance& x, t_canvas* parent, t_symbol* name, const AtomList& args);

    void allocSignals();
    void allocInlets();
    void allocOutlets();

    struct ChannelInfo {
        size_t num_chan;
        size_t num_static_chan;
        size_t num_extra_chan;
        bool has_static_ch;
    };

    struct InOutInfo {
        ChannelInfo in, out;
    };

    InOutInfo calcNumChannels() const;

public:
    size_t target() const { return target_; }
    bool targetAll() const { return target_ == size_t(-1); }

    void sendBangToInstance(size_t inst_idx, size_t inlet_idx);
    void sendBangToAll(size_t inlet_idx);
    void sendFloatToInstance(size_t inst_idx, size_t inlet_idx, t_float v);
    void sendFloatToAll(size_t inlet_idx, t_float v);
};

void setup_spat_hoa_process();

#endif // HOA_PROCESS_H
