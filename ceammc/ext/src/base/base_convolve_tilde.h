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
#ifndef BASE_CONVOLVE_TILDE_H
#define BASE_CONVOLVE_TILDE_H

#include <memory>

#include "ceammc_sound_external.h"
using namespace ceammc;

namespace fftconvolver {
class FFTConvolver;
}

#if PD_FLOATSIZE != 32
#define USE_FLOAT_BUFFERS
#endif

using ConvImpl = std::unique_ptr<fftconvolver::FFTConvolver>;

class BaseConvolveTilde : public SoundExternal {
    enum SourceType {
        SRC_NONE,
        SRC_FILE,
        SRC_ARRAY
    };

    enum LoadState {
        NOT_LOADED,
        LOAD_OK
    };

private:
    ConvImpl conv_;
    IntProperty* max_size_ { nullptr };
    IntProperty* offset_ { nullptr };
    BoolProperty* norm_ { nullptr };
    std::vector<float> ir_data_;
    LoadState load_state_ { NOT_LOADED };

#ifdef USE_FLOAT_BUFFERS
    std::vector<float> in_buf64_, out_buf64_;
#endif

public:
    BaseConvolveTilde(const PdArgs& args);
    ~BaseConvolveTilde();

    void setupDSP(t_signal** sig) final;
    void processBlock(const t_sample** in, t_sample** out) final;

    void m_load_file(t_symbol* s, const AtomListView& lv);
    void m_load_array(t_symbol* s, const AtomListView& lv);
    void m_set(t_symbol* s, const AtomListView& lv);

    void dump() const final;

private:
    bool loadIRFromArray(const char* name);
    bool loadIRFromFile(const char* path);
    void normalizeIR();
};

void setup_base_convolve_tilde();

#endif // BASE_CONVOLVE_TILDE_H
