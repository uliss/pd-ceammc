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
#ifndef IODISPLAY_H
#define IODISPLAY_H

#include <IOKit/IOTypes.h>

namespace io_display {

class IODisplay {
    io_object_t service_;

public:
    IODisplay();
    ~IODisplay();

    bool setBrightness(float v);
    bool getBrightness(float* v) const;
};

}

#endif // IODISPLAY_H
