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
#include "ceammc_loader_sndfile.h"

#include <iostream>

namespace ceammc {

namespace sound {

    int toEnum(sf_mode mode)
    {
        switch (mode) {
        case SF_READ:
            return SFM_READ;
        case SF_WRITE:
            return SFM_WRITE;
        }
    }

    LibSndFile::LibSndFile(const std::string& fname, sf_mode mode)
        : SoundFile(fname, mode)
        , handle_(fname, toEnum(mode), 0, 0, 0)
    {
    }

    size_t LibSndFile::sampleCount() const
    {
        return handle_.frames();
    }

    size_t LibSndFile::sampleRate() const
    {
        return handle_.samplerate();
    }

    size_t LibSndFile::channels() const
    {
        return handle_.channels();
    }

    bool LibSndFile::isOpened() const
    {
        return handle_.rawHandle() != NULL;
    }

    bool LibSndFile::close()
    {
        handle_ = SndfileHandle();
        return true;
    }

    bool LibSndFile::read(float* dest, size_t sz, size_t ch)
    {
        if (!handle_)
            return false;

        if (ch > channels())
            return false;

        if (ch == channels())
            return handle_.readf(dest, sz) == sz;
        else {
            return false;
        }
    }

    bool LibSndFile::write(const float* src, size_t sz, size_t ch)
    {
        if (!handle_)
            return false;

        if (ch > channels())
            return false;

        handle_.writef(src, sz);
        return true;
    }

    StringList LibSndFile::supportedFormats()
    {
        StringList res;

        SF_FORMAT_INFO format_info;
        int count;
        sf_command(0, SFC_GET_FORMAT_MAJOR_COUNT, &count, sizeof(int));

        for (int k = 0; k < count; k++) {
            format_info.format = k;
            sf_command(0, SFC_GET_FORMAT_MAJOR, &format_info, sizeof(format_info));
            res.push_back(format_info.name);
        };

        return res;
    }
}
}
