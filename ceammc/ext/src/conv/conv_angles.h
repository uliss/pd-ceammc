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
#ifndef CONV_ANGLES_H
#define CONV_ANGLES_H

#include "simple_converter.h"

class Phase2Rad : public SimpleConverter {
public:
    Phase2Rad(const PdArgs& args);
};

class Rad2Phase : public SimpleConverter {
public:
    Rad2Phase(const PdArgs& args);
};

class Car2Pol : public BaseObject {
    BoolProperty* positive_range_;

public:
    Car2Pol(const PdArgs& args);
    void onList(const AtomListView& lv) override;
};

class Pol2Car : public BaseObject {
public:
    Pol2Car(const PdArgs& args);
    void onList(const AtomListView& lv) override;
};

class Phase2RadTilde : public SimpleConverterTilde {
public:
    Phase2RadTilde(const PdArgs& args);
};

class Rad2PhaseTilde : public SimpleConverterTilde {
public:
    Rad2PhaseTilde(const PdArgs& args);
};

void setup_conv_angles();

#endif // CONV_ANGLES_H
