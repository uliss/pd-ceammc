/*****************************************************************************
 * Copyright 2017 Alex Nadzharov, Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_TIMELINE_H
#define CEAMMC_TIMELINE_H

#include "m_pd.h"

#include <map>
#include <string>
#include <vector>

namespace ceammc {
namespace tl {

    class CueData {
        t_canvas* c_;
        t_object* obj_;
        int index_;
        int xpos_;
        std::string str_;

    public:
        CueData(t_canvas* c, t_object* o);
        t_canvas* canvas() { return c_; }
        const t_canvas* canvas() const { return c_; }
        void setXPos(int x);
        void setIndex(int idx);
        int xPos() const { return xpos_; }
        int index() const { return index_; }
        std::string name() const { return str_; }

    public:
        bool operator<(const CueData& b) const;
        bool operator==(const CueData& c) const;
    };

    class CueStorage {
        typedef std::vector<CueData> CueList;
        typedef std::map<t_canvas*, CueList> CanvasCueMap;

        static CanvasCueMap cue_map_;
        CueStorage();

    public:
        static void add(const CueData& c);
        static void remove(const CueData& c);
        static int index(const CueData& c);
        static bool exists(const CueData& c);
        static bool exists(t_canvas* cnv);
        static void sort(t_canvas* cnv);
        static void enumerate(t_canvas* cnv);

    private:
        static CueList* cueList(t_canvas* c);
        static CueList* cueList(const CueData& c);
    };
}
}

#endif // CEAMMC_TIMELINE_H
