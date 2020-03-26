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
#ifndef HW_APPLE_SMS_H
#define HW_APPLE_SMS_H

#include "ceammc_object.h"
using namespace ceammc;

#ifdef WITH_SMS
#include "apple/suddenmotionsensor.h"
#endif

class HwAppleSMS : public BaseObject {
#ifdef WITH_SMS
    apple_sms::SuddenMotionSensor sensor_;
#endif

    BoolProperty* raw_;

public:
    HwAppleSMS(const PdArgs& args);
    ~HwAppleSMS();

    void onBang() override;

private:
    AtomList getXYZ() const;
};

void setup_hw_apple_sms();

#endif // HW_APPLE_SMS_H
