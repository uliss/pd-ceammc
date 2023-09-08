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
#include "ceammc_factory.h"
#include "hoa_encoder_base.h"

using Hoa3dEncoder = HoaEncoderBase<Encoder3d, 3>;

template <>
const char* Hoa3dEncoder::annotateInlet(size_t n) const
{
    switch (n) {
    case 0:
        return "input signal";
    case 1:
        return "azimuth in radians (ccw)";
    case 2:
    default:
        return "elevation in radians";
    }
}

void setup_spat_hoa_encoder_3d()
{
    SoundExternalFactory<Hoa3dEncoder> obj("hoa.3d.encoder~");
}
