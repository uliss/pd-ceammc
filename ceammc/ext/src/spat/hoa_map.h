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
#ifndef HOA_MAP_H
#define HOA_MAP_H

#include "hoa_common.h"

class HoaMap : public HoaBase {
    IntProperty* nins_;
    FloatProperty* ramp_;

    std::unique_ptr<MultiEncoder2d> map_;
    std::unique_ptr<PolarLines2d> lines_;

    Buffer in_buf_;
    Buffer out_buf_;

    std::vector<t_sample> lines_vec_;

public:
    HoaMap(const PdArgs& args);

    void parseProperties() final;
    void setupDSP(t_signal** sp) final;
    void blockSizeChanged(size_t bs) override;
    void processBlock(const t_sample** in, t_sample** out) final;

    void processMultiSource();
    void processIn1In2();

    void m_polar(t_symbol* s, const AtomListView& l);
    void m_mute(t_symbol* s, const AtomListView& l);

private:
    static t_int* dspPerformMultiSource(t_int* w)
    {
        HoaMap* ext = reinterpret_cast<HoaMap*>(w[1]);
        ext->processMultiSource();
        return (w + 2);
    }

    static t_int* dspPerformIn1In2(t_int* w)
    {
        HoaMap* ext = reinterpret_cast<HoaMap*>(w[1]);
        ext->processIn1In2();
        return (w + 2);
    }
};

void setup_spat_hoa_map();

#endif // HOA_MAP_H
