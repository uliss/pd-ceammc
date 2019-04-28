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
#ifndef HOA_DECODER_H
#define HOA_DECODER_H

#include "hoa_common.h"

#include <memory>

class HoaDecoder : public HoaBase {
    SymbolEnumProperty* mode_;
    std::unique_ptr<Decoder2d> decoder_;
    Buffer in_buf_;
    Buffer out_buf_;
    size_t crop_size_;
    size_t num_plain_waves_;

public:
    HoaDecoder(const PdArgs& args);

    void parseProperties() override;
    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** sp) override;
    void blocksizeChanged(size_t bs) override;

    AtomList propCropSize() const;
    void propSetCropSize(const AtomList& lst);
    AtomList propPlaneWavesAzimuth() const;
    AtomList propPlaneWavesX() const;
    AtomList propPlaneWavesY() const;
    AtomList propPlaneWavesZ() const;

    AtomList propNumPlainWaves() const;
    void propSetNumPlainWaves(const AtomList& lst);
    AtomList propNumHarmonics() const;

private:
    void processCommon();
    void processBinaural();

    inline static t_int* dspPerformCommon(t_int* w)
    {
        HoaDecoder* ext = reinterpret_cast<HoaDecoder*>(w[1]);
        ext->processCommon();
        return (w + 2);
    }

    inline static t_int* dspPerformBinaural(t_int* w)
    {
        HoaDecoder* ext = reinterpret_cast<HoaDecoder*>(w[1]);
        ext->processBinaural();
        return (w + 2);
    }

    void initDecoder();
};

void setup_spat_hoa_decoder();

#endif // HOA_DECODER_H
