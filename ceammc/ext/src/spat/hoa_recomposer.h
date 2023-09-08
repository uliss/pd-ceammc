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
#ifndef HOA_2D_RECOMPOSER_H
#define HOA_2D_RECOMPOSER_H

#include "ceammc_property_enum.h"
#include "hoa_common.h"

class Hoa2dRecomposer : public HoaBase<hoa::Hoa2d> {
    Buffer in_buf_;
    Buffer out_buf_;
    Buffer line_buf_;
    std::unique_ptr<MultiEncoder2d> processor_;
    std::unique_ptr<PolarLines2d> free_mode_lines_;
    IntProperty* plane_waves_;
    SymbolEnumProperty* mode_;
    t_float ramp_;

public:
    Hoa2dRecomposer(const PdArgs& args);
    void initDone() override;
    void blockSizeChanged(size_t bs) override;
    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** sp) override;

    void m_angles(t_symbol* s, const AtomListView& lv);
    void m_wide(t_symbol* s, const AtomListView& lv);

    bool propSetRamp(t_float f);

private:
    void parseNumPlaneWaves();

    void processFixE();
    void processFree();
    void processFisheye();

    inline static t_int* dspPerformFixE(t_int* w)
    {
        auto ext = reinterpret_cast<Hoa2dRecomposer*>(w[1]);
        ext->processFixE();
        return (w + 2);
    }

    inline static t_int* dspPerformFree(t_int* w)
    {
        auto ext = reinterpret_cast<Hoa2dRecomposer*>(w[1]);
        ext->processFree();
        return (w + 2);
    }

    inline static t_int* dspPerformFisheye(t_int* w)
    {
        auto ext = reinterpret_cast<Hoa2dRecomposer*>(w[1]);
        ext->processFisheye();
        return (w + 2);
    }
};

void setup_spat_hoa_recomposer_2d();

#endif // HOA_2D_RECOMPOSER_H
