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
#ifndef HOA_ENCODER_H
#define HOA_ENCODER_H

#include <memory>

#include "hoa_common.h"

class HoaEncoder : public HoaBase {
    std::unique_ptr<Encoder2d> encoder_;
    Buffer signals_;

public:
    HoaEncoder(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) override;
    void parseProperties() override;
    void blockSizeChanged(size_t bs) override;

    const char* annotateInlet(size_t n) const override;
};

void setup_spat_hoa_encoder();

#endif // HOA_ENCODER_H
