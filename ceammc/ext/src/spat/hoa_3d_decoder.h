/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#ifndef HOA_3D_DECODER_H
#define HOA_3D_DECODER_H

#include "ceammc_property_enum.h"
#include "hoa_common.h"

#include <memory>

class Hoa3dDecoder : public HoaBase {
    SymbolEnumProperty* mode_ { nullptr };
    IntProperty* num_chan_ { nullptr };
    std::unique_ptr<Decoder3d> decoder_;
    Buffer in_buf_;
    Buffer out_buf_;
    AtomList cache_angles_;

public:
    Hoa3dDecoder(const PdArgs& args);

    void initDone() override;
    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** sp) override;
    void blockSizeChanged(size_t bs) override;

    AtomList propPlaneWavesX() const;
    AtomList propPlaneWavesY() const;
    AtomList propPlaneWavesZ() const;

    AtomList propAngles() const;
    bool propSetAngles(const AtomListView& lv);

    bool propSetOffset(const AtomListView& lv);

private:
    void processCommon();
    void processBinaural();

    inline static t_int* dspPerformCommon(t_int* w)
    {
        auto ext = reinterpret_cast<Hoa3dDecoder*>(w[1]);
        ext->processCommon();
        return (w + 2);
    }

    inline static t_int* dspPerformBinaural(t_int* w)
    {
        auto ext = reinterpret_cast<Hoa3dDecoder*>(w[1]);
        ext->processBinaural();
        return (w + 2);
    }

    void initDecoder();
    void initChannelNum();
};

void setup_spat_hoa_decoder_3d();

#endif // HOA_3D_DECODER_H
