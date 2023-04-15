/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_FAUST_FACTORY_H
#define CEAMMC_FAUST_FACTORY_H

#include "ceammc_factory.h"

namespace ceammc {

template <typename T>
class FaustFactory : public SoundExternalFactory<T> {
public:
    FaustFactory(const char* name,
        int flags = OBJECT_FACTORY_DEFAULT | OBJECT_FACTORY_MAIN_SIGNAL_INLET | OBJECT_FACTORY_NO_FLOAT)
        : SoundExternalFactory<T>(name, flags)
    {
        this->addMethod(OSC_METHOD_UPDATE, &T::updateOscServer);
    }
};

}

#endif // CEAMMC_FAUST_FACTORY_H
