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
#include "ceammc_factory.h"
#include "hoa_encoder_base.h"

using Hoa2dEncoder = HoaEncoderBase<Encoder2d, 2, hoa::Hoa2d>;

template <>
const char* Hoa2dEncoder::annotateInlet(size_t n) const
{
    if (n == 0)
        return "input signal";
    else
        return "angle in radians (ccv)";
}

void setup_spat_hoa_2d_encoder()
{

    SoundExternalFactory<Hoa2dEncoder> obj("hoa.2d.encoder~");
    obj.addAlias("hoa.encoder~");
}
