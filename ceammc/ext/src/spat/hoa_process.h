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
#include "hoa_common.h"
#include "hoa_connections.h"
#include "hoa_process_inlet.h"
#include "hoa_process_instance.h"

#include <functional>

typedef void (*t_bangmethod)(t_pd* x);

class HoaProcess : public SoundExternal {
    std::vector<ProcessInstance> instances_;
    std::vector<ProcessInlet> ins_;

    Buffer in_buf_;
    Buffer out_buf_;

    SymbolEnumProperty* domain_;
    SymbolProperty* patch_;
    IntProperty* num_;

    // used to send loadbang to instances
    ClockMemberFunction<HoaProcess> clock_;

public:
    HoaProcess(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** sp) final;

    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override;
    void m_open(t_symbol* m, const AtomListView& lv);
    void m_dsp_on(t_symbol* m, const AtomListView& lv);

    void initDone() override;

private:
    void clockTick();

    bool loadHarmonics(t_symbol* name, const AtomListView& patch_args);
    bool loadPlaneWaves(t_symbol* name, const AtomListView& patch_args);

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
    void sendToN(std::function<void(ProcessInstance*)> fn, size_t inst_idx);
    void sendToAll(std::function<void(ProcessInstance*)> fn);
    void sendToRange(std::function<void(ProcessInstance*)> fn, size_t from, size_t to);
    void sendToLessThen(std::function<void(ProcessInstance*)> fn, size_t inst_idx);
    void sendToGreaterThen(std::function<void(ProcessInstance*)> fn, size_t inst_idx);
    void sendToGreaterEq(std::function<void(ProcessInstance*)> fn, size_t inst_idx);

    void sendBangToInstance(size_t inst_idx, size_t inlet_idx);
    void sendBangToAll(size_t inlet_idx);
    void sendBangToRange(size_t from, size_t to, size_t inlet_idx);
    void sendBangToLessThen(size_t inst_idx, size_t inlet_idx);
    void sendBangToGreaterEq(size_t inst_idx, size_t inlet_idx);

    void sendFloatToInstance(size_t inst_idx, size_t inlet_idx, t_float v);
    void sendFloatToAll(size_t inlet_idx, t_float v);
    void sendFloatToLessThen(size_t inst_idx, size_t inlet_idx, t_float v);
    void sendFloatToGreaterEq(size_t inst_idx, size_t inlet_idx, t_float v);
    void sendFloatToRange(size_t from, size_t to, size_t inlet_idx, t_float v);

    void sendSymbolToInstance(size_t inst_idx, size_t inlet_idx, t_symbol* s);
    void sendSymbolToAll(size_t inlet_idx, t_symbol* s);
    void sendSymbolToLessThen(size_t inst_idx, size_t inlet_idx, t_symbol* s);
    void sendSymbolToGreaterEq(size_t inst_idx, size_t inlet_idx, t_symbol* s);
    void sendSymbolToRange(size_t from, size_t to, size_t inlet_idx, t_symbol* s);

    void sendListToInstance(size_t inst_idx, size_t inlet_idx, const AtomList& l);
    void sendListToAll(size_t inlet_idx, const AtomList& l);
    void sendListToLessThen(size_t inst_idx, size_t inlet_idx, const AtomList& l);
    void sendListToGreaterEq(size_t inst_idx, size_t inlet_idx, const AtomList& l);
    void sendListToRange(size_t from, size_t to, size_t inlet_idx, const AtomList& l);
    void sendListToSpread(size_t from, size_t inlet_idx, const AtomList& l);

    void sendAnyToInstance(size_t inst_idx, size_t inlet_idx, t_symbol* s, const AtomListView& l);
    void sendAnyToAll(size_t inlet_idx, t_symbol* s, const AtomListView& l);
    void sendAnyToLessThen(size_t inst_idx, size_t inlet_idx, t_symbol* s, const AtomListView& l);
    void sendAnyToGreaterEq(size_t inst_idx, size_t inlet_idx, t_symbol* s, const AtomListView& l);
    void sendAnyToRange(size_t from, size_t to, size_t inlet_idx, t_symbol* s, const AtomListView& l);
    void sendAnyToSpread(size_t inst_idx, size_t inlet_idx, t_symbol* s, const AtomListView& l);

public:
    static t_symbol* SYM_SWITCH;
    static t_symbol* SYM_BLOCK;
    static t_symbol* SYM_OBJ;
    static t_symbol* SYM_HARMONICS;
    static t_symbol* SYM_PLANEWAVES;
    static t_symbol* SYM_2D;
    static t_symbol* SYM_CANVAS;
    static t_symbol* SYM_DSP;

    static size_t calcHarmDegree2d(size_t index);
    static size_t calcHarmDegree3d(size_t index);
    static long calcAzimuthalOrder2d(size_t index);
    static long calcAzimuthalOrder3d(size_t index);
    static size_t calcNumHarm2d(size_t order);
    static size_t calcNumHarm3d(size_t order);

    static size_t harmToIndex2d(size_t degree, long order);
    static size_t harmToIndex3d(size_t degree, long order);

public:
    // only for tests
    Buffer& inputBuffer() { return in_buf_; }
    Buffer& outputBuffer() { return out_buf_; }
};

void setup_spat_hoa_process();

#endif // HOA_PROCESS_H
