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
#ifdef WITH_LIBSOUNDFILE
#include "ceammc_loader_sndfile.h"
#endif

#include <algorithm>
#include <iostream>

using namespace std;

namespace ceammc {
namespace sound {

#ifdef WITH_LIBSOUNDFILE
    static SoundFilePtr libsndfile_load_func(const std::string& path)
    {
        return SoundFilePtr(new LibSndFile(path));
    }

    static const bool libsndfile_register = SoundFileLoader::registerLoader(
        LoaderDescr("libsndfile", &libsndfile_load_func, LibSndFile::supportedFormats));
#endif

    SoundFile::SoundFile(const std::string& fname)
        : fname_(fname)
    {
    }

    SoundFile::~SoundFile()
    {
    }

    bool SoundFileLoader::registerLoader(const LoaderDescr& l)
    {
        if (std::find(loaders().begin(), loaders().end(), l) == loaders().end()) {
            loaders().push_back(l);
            std::cerr << "register loader: " << l.name;
            return true;
        } else {
            std::cerr << "loader already registered: " << l.name;
            return false;
        }
    }

    StringList SoundFileLoader::supportedFormats()
    {
        StringList res;

        if (loaders().empty())
            return res;

        for (size_t i = 0; i < loaders().size(); i++) {
            StringList fmt = loaders().at(i).formats();
            res.insert(res.end(), fmt.begin(), fmt.end());
        }

        return res;
    }

    SoundFilePtr SoundFileLoader::open(const std::string& path)
    {
        if (loaders().empty()) {
            std::cerr << "no loaders registered";
            return SoundFilePtr();
        }

        return loaders().front().func(path);
    }

    SoundFileLoader::LoaderList& SoundFileLoader::loaders()
    {
        static SoundFileLoader::LoaderList loaders;
        return loaders;
    }

    std::string SoundFile::filename()
    {
        return fname_;
    }

    bool LoaderDescr::operator==(const LoaderDescr& l)
    {
        return l.func == func;
    }
}
}
