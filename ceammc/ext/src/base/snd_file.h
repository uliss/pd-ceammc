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

#include "ceammc_object.h"
#include "ceammc_sound.h"

using namespace ceammc;

class SndFile : public ceammc::BaseObject {
public:
    SndFile(const PdArgs& a);

public:
    void m_load(t_symbol* sel, const AtomList& lst);
    void m_info(t_symbol* sel, const AtomList& lst);
    void dump() const;

    AtomList supportedFormats() const;

protected:
    void postInfoUsage();
    void postLoadUsage();
    t_garray* findArray(const Atom& name) const;
    bool checkArray(const Atom& name);
    bool resizeArray(const Atom& name, long newSize);
    long loadArray(sound::SoundFilePtr file, const Atom& name, size_t channel, long offset);
    void outputInfo(sound::SoundFilePtr file);
    bool arrayNameContainsPattern(const std::string& name) const;
    AtomList findPatternArrays(const std::string& pattern) const;

private:
    t_canvas* cnv_;
};

#endif // SND_FILE_H
