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
#include "hw_apple_sms.h"
#include "ceammc_factory.h"
#include "ceammc_stub.h"

HwAppleSMS::HwAppleSMS(const PdArgs& args)
    : BaseObject(args)
    , raw_(nullptr)
{
    raw_ = new BoolProperty("@raw", false);
    addProperty(raw_);

    createOutlet();
}

HwAppleSMS::~HwAppleSMS()
{
}

void HwAppleSMS::onBang()
{
    auto lst = getXYZ();
    if (!lst.empty())
        listTo(0, lst);
}

AtomList HwAppleSMS::getXYZ() const
{
#ifndef WITH_SMS
    return AtomList::zeroes(3);
#else
    if (!sensor_.isConnected()) {
        OBJ_ERR << "sensor is not found on this machine";
        return AtomList::zeroes(3);
    }

    apple_sms::AccelData res;
    if (raw_->value())
        res = sensor_.getRawAccel();
    else
        res = sensor_.getNormalizedAccel();

    AtomList lst;
    lst.append(std::get<0>(res));
    lst.append(std::get<1>(res));
    lst.append(std::get<2>(res));
    return lst;
#endif
}

#if !defined(__arm64__) && defined(WITH_SMS)
extern "C" void setup_hw_apple_sms()
{
    ObjectFactory<HwAppleSMS> obj("hw.apple_sms");
}
#else
CONTROL_OBJECT_STUB("hw.apple_sms", 1, 1, "not supported on this platform", hw_apple_sms);
#endif
