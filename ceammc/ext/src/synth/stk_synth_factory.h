/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef STK_SYNTH_FACTORY_H
#define STK_SYNTH_FACTORY_H

#include "ceammc_factory.h"

namespace ceammc {

template <typename T>
class StkSynthFactory : public SoundExternalFactory<T> {
    bool rawpath_init_;

public:
    StkSynthFactory(const char* name, int flags = OBJECT_FACTORY_DEFAULT)
        : SoundExternalFactory<T>(name, flags)
        , rawpath_init_(false)
    {
        SoundExternalFactory<T>::addMethod("cc", &T::m_cc);
        T::initRawWaves(SoundExternalFactory<T>::classPointer());
    }
};
}

#endif // STK_SYNTH_FACTORY_H
