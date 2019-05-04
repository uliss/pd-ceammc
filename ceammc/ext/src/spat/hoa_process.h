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

typedef struct _hoa_out {
    t_object f_obj;
    int f_extra;
    t_outlet* f_outlet;
    struct _hoa_out* f_next;
} t_hoa_out;

typedef struct _hoa_io_tilde {
    t_object f_obj;
    int f_extra;
    t_sample* f_signal;
    t_float f_f;
    struct _hoa_io_tilde* f_next;
} t_hoa_io_tilde;

struct t_hoa_process_instance {
    t_canvas* f_canvas;
    std::forward_list<HoaIn*> f_ins;
    t_hoa_out* f_outs;
    t_hoa_io_tilde* f_ins_sig;
    t_hoa_io_tilde* f_outs_sig;

    bool has_inputs_sig_static();
    bool has_outputs_sig_static();
    size_t get_ninputs_sig_extra();
    size_t get_noutputs_sig_extra();
    size_t get_ninputs();
    size_t get_noutputs();
};

class HoaProcess : public HoaBase {
    t_bangmethod method_;
    t_object* block_;
    std::vector<t_hoa_process_instance> instances_;
    t_canvas* hoa_canvas_;
    t_float canvas_yoff_;

    size_t nins_;
    size_t target_;
    size_t nsigout_;

    std::vector<t_hoa_process_inlet> ins_;

    SymbolEnumProperty* domain_;

public:
    HoaProcess(const PdArgs& args);
    ~HoaProcess();

    void parseProperties() override;
    void processBlock(const t_sample** in, t_sample** out) override;

    void m_click(t_symbol* m, const AtomList& lst);
    void m_open_cnv(t_symbol* m, const AtomList& lst);

private:
    bool init();

    bool loadHarmonics(t_symbol* name, const AtomList& patch_args);
    bool processInstanceInit(t_hoa_process_instance& x, t_canvas* parent, t_symbol* name, const AtomList& args);

    void showInstance(t_hoa_process_instance& instance);
    void allocSignals();
    void allocInlets();

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
