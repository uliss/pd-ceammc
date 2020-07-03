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
#ifndef HW_KBD_LIGHT_H
#define HW_KBD_LIGHT_H

#include "ceammc_object.h"
using namespace ceammc;

#ifdef __APPLE__
#include <IOKit/IOTypes.h>
#endif

class HwKeyboardLight : public BaseObject {
#ifdef __APPLE__
    io_service_t io_service_;
    io_connect_t io_connect_;
#endif
public:
    HwKeyboardLight(const PdArgs& args);
    ~HwKeyboardLight();
    void onBang() override;
    void onFloat(t_float v) override;

private:
    t_float currentLevel() const;
    bool setLevel(t_float v);
};

void setup_hw_keyboard_light();

#endif // HW_KBD_LIGHT_H
