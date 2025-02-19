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
#ifndef HOA_3D_OPTIM_H
#define HOA_3D_OPTIM_H

#include "ceammc_property_enum.h"
#include "hoa_common.h"

#include <memory>

class HoaOptim3D : public HoaBase {
    Buffer in_buf_;
    Buffer out_buf_;
    std::unique_ptr<Optim3d> optim_;
    SymbolEnumProperty* mode_;

public:
    HoaOptim3D(const PdArgs& args);
    void initDone() override;

    void processBlock(const t_sample** in, t_sample** out) override;

    void adjustMode();
};

void setup_spat_hoa_optim_3d();

#endif // HOA_3D_OPTIM_H
