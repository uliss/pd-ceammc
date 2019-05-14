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
#ifndef HOA_ROTATE_H
#define HOA_ROTATE_H

#include <memory>

#include "ceammc_sound_external.h"
#include "hoa_common.h"
using namespace ceammc;

class HoaRotate : public HoaBase {
    Buffer in_buf_;
    Buffer out_buf_;
    std::unique_ptr<Rotate2d> rotate_;

public:
    HoaRotate(const PdArgs& args);
    void parseProperties() override;

    void processBlock(const t_sample** in, t_sample** out) override;
    void blockSizeChanged(size_t bs) override;
};

void setup_spat_hoa_rotate();

#endif // HOA_ROTATE_H
