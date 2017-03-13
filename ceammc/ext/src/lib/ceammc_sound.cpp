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

#include "ceammc_loader_sndfile.h"

namespace ceammc {
namespace sound {
    SoundFile::SoundFile(const std::string& fname)
        : fname_(fname)
    {
    }

    SoundFile::~SoundFile()
    {
    }

    StringList SoundFileLoader::supportedFormats()
    {
        return StringList();
    }

    bool SoundFileLoader::isSupported(const std::string& format)
    {
        return false;
    }

    SoundFilePtr SoundFileLoader::open(const std::string& path)
    {
        return SoundFilePtr(new LibSndFile(path));
    }

    std::string SoundFile::filename()
    {
        return fname_;
    }
}
}
