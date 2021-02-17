/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

namespace ceammc {
namespace ui {
    enum UIFactoryFlags : uint16_t {
        UI_FACTORY_NONE = 0,
        UI_FACTORY_FLAG_DISPLACE = 1,
        UI_FACTORY_FLAG_SELECT = (1 << 1),
        UI_FACTORY_FLAG_ACTIVATE = (1 << 2),
        UI_FACTORY_FLAG_DELETE = (1 << 3),
        UI_FACTORY_FLAG_VIS = (1 << 4),
        UI_FACTORY_FLAG_CLICK = (1 << 5),
        UI_FACTORY_FLAG_MOUSE_ENTER = (1 << 6),
        UI_FACTORY_FLAG_MOUSE_LEAVE = (1 << 7),
        UI_FACTORY_FLAG_MOUSE_MOVE = (1 << 8),
        UI_FACTORY_FLAG_MOUSE_DOWN = (1 << 9),
        UI_FACTORY_FLAG_MOUSE_UP = (1 << 10),
        UI_FACTORY_FLAG_DEFAULT = UI_FACTORY_FLAG_VIS
            | UI_FACTORY_FLAG_DISPLACE
            | UI_FACTORY_FLAG_SELECT
            | UI_FACTORY_FLAG_DELETE
    };
}
}

#endif // COMMON_H
