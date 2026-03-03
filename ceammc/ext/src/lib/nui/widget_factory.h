/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#ifndef WIDGET_FACTORY_H
#define WIDGET_FACTORY_H

#include "ceammc_factory.h"
#include "factory.h"

namespace ceammc {

namespace ui {

    template <typename T>
    class WidgetFactory : public UIFactory<ObjectFactory, T> {
    public:
        explicit WidgetFactory(const char* name, int flags = OBJECT_FACTORY_DEFAULT)
            : UIFactory<ObjectFactory, T>(name, flags)
        {
            this->useMouseDown();
            this->useMouseEnter();
            this->useMouseLeave();
            this->useMouseMove();
            this->useMouseRight();
            this->useMouseUp();
        }
    };

} // namespace ui
} // namespace ceammc

#endif // WIDGET_FACTORY_H
