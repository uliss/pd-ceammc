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

#include "ceammc_clock.h"
#include "ceammc_property_extra.h"
#include "hoa_common.h"
#include "hoa_connections.h"
#include "hoa_process_inlet.h"
#include "hoa_process_instance.h"

typedef void (*t_bangmethod)(t_pd* x);

typedef LambdaCheckProperty<int> TargetProperty;

class HoaProcess : public HoaBase {
    std::vector<ProcessInstance> instances_;
    std::vector<ProcessInlet> ins_;

    t_canvas* canvas_;
    t_object* block_obj_;
    t_bangmethod block_obj_method_;

    t_float canvas_yoff_;

    Buffer in_buf_;
    Buffer out_buf_;

    SymbolEnumProperty* domain_;
    IntPropertyMinEq* plane_waves_;
    TargetProperty* target_;
    int target_value_;

    // used to send loadbang to instances
    ClockMemberFunction<HoaProcess> clock_;

public:
    HoaProcess(const PdArgs& args);
    ~HoaProcess();

    void parseProperties() override;
    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** sp) final;

    void m_click(t_symbol* m, const AtomList& lst);
    void m_open(t_symbol* m, const AtomList& lst);
    void m_open_cnv(t_symbol* m, const AtomList& lst);

private:
    bool init();
    void clockTick();

    bool loadHarmonics(t_symbol* name, const AtomList& patch_args);
    bool loadPlaneWaves(t_symbol* name, const AtomList& patch_args);
    bool processInstanceInit(ProcessInstance& x, t_canvas* parent, t_symbol* name, const AtomList& args);

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
    size_t target() const { return target_->value(); }
    bool targetAll() const { return target_->value() == -1; }

    void sendBangToInstance(size_t inst_idx, size_t inlet_idx);
    void sendBangToAll(size_t inlet_idx);
    void sendFloatToInstance(size_t inst_idx, size_t inlet_idx, t_float v);
    void sendFloatToAll(size_t inlet_idx, t_float v);
    void sendSymbolToInstance(size_t inst_idx, size_t inlet_idx, t_symbol* s);
    void sendSymbolToAll(size_t inlet_idx, t_symbol* s);
    void sendListToInstance(size_t inst_idx, size_t inlet_idx, const AtomList& l);
    void sendListToAll(size_t inlet_idx, const AtomList& l);
    void sendAnyToInstance(size_t inst_idx, size_t inlet_idx, t_symbol* s, const AtomList& l);
    void sendAnyToAll(size_t inlet_idx, t_symbol* s, const AtomList& l);

public:
    static t_symbol* SYM_SWITCH;
    static t_symbol* SYM_BLOCK;
    static t_symbol* SYM_OBJ;
    static t_symbol* SYM_HARMONICS;
    static t_symbol* SYM_PLANEWAVES;
    static t_symbol* SYM_2D;
    static t_symbol* SYM_CANVAS;

    static size_t calcIndexDegree(size_t index);
    static long calcAzimuthalOrder(size_t index);
    static size_t orderToIndex(long order);

public:
    // only for tests
    Buffer& inputBuffer() { return in_buf_; }
    Buffer& outputBuffer() { return out_buf_; }
};

void setup_spat_hoa_process();

#endif // HOA_PROCESS_H
