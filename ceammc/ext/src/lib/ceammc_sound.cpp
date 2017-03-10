/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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

#include "ceammc_sound.h"

namespace ceammc {
namespace sound {
    SoundFile::SoundFile(const std::string& fname, sf_mode mode)
        : fname_(fname)
        , mode_(mode)
    {
    }

    SoundFile::~SoundFile()
    {
    }

    StringList SoundFileLoader::supportedReadFormats()
    {
        return StringList();
    }

    StringList SoundFileLoader::supportedWriteFormats()
    {
        return StringList();
    }

    bool SoundFileLoader::isSupported(const std::string& format)
    {
        return false;
    }

    SoundFilePtr SoundFileLoader::open(const std::string& path)
    {
        return SoundFilePtr();
    }

    std::string SoundFile::filename()
    {
        return fname_;
    }
}
}
