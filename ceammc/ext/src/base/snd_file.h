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
#ifndef SND_FILE_H
#define SND_FILE_H

#include "ceammc_maybe.h"
#include "ceammc_object.h"

using namespace ceammc;

using MaybeString = Maybe<std::string>;

class SndFile : public ceammc::BaseObject {
    FlagProperty* verbose_;
    FlagProperty* debug_;
    FloatProperty* smpte_framerate_;
    AtomList samplerates_, filenames_, samplecount_, channels_; // results of last load

public:
    SndFile(const PdArgs& a);

public:
    void m_load(t_symbol* s, const AtomListView& lv);
    void m_save(t_symbol* s, const AtomListView& lv);

private:
    void postInfoUsage();
    void postLoadUsage();

    MaybeString fullLoadPath(const std::string& fname) const;

    bool extractLoadArgs(const AtomListView& lv, std::string& fname, std::string& array_opts);
};

void setup_snd_file();

#endif // SND_FILE_H
