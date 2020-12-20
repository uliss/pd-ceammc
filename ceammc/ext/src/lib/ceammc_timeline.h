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
        t_object* object() { return obj_; }
        const t_object* object() const { return obj_; }
        void setXPos(int x);
        void setIndex(int idx);
        int xPos() const { return xpos_; }
        int index() const { return index_; }
        std::string name() const { return str_; }

    public:
        bool operator==(const CueData& c) const;
    };

    typedef std::vector<CueData*> CueList;

    class CueStorage {
        typedef std::map<const t_canvas*, CueList> CanvasCueMap;

        static CanvasCueMap cue_map_;
        CueStorage();

    public:
        static bool add(CueData* c);
        static bool remove(CueData* c);
        static int index(CueData* c);
        static CueData* at(t_canvas* cnv, size_t pos);
        static CueData* find(const t_canvas* cnv, t_object* obj);
        static bool exists(CueData* c);
        static bool exists(const t_canvas* cnv);
        static void sort(t_canvas* cnv);
        static bool enumerate(t_canvas* cnv);
        static size_t cueCount(const t_canvas* cnv);

        static CueList* cueList(const t_canvas* c);
        static CueList* cueList(CueData* c);

        static void clear();
    };

    class TimelineData;
    typedef void (*UIAction)(TimelineData*, int v);

    class TimelineData {
        t_canvas* canvas_;
        t_object* obj_;
        int xpos_;
        UIAction cb_;

    public:
        TimelineData(t_canvas* c, t_object* obj);

        int xPos() const { return xpos_; }
        void setXPos(int x);

        t_canvas* canvas() { return canvas_; }
        const t_canvas* canvas() const { return canvas_; }
        t_object* object() { return obj_; }
        const t_object* object() const { return obj_; }

        UIAction action() { return cb_; }
        void setAction(UIAction fn);
        void triggerAction(int in);

        const CueData* findCue() const;
    };

    typedef std::vector<TimelineData*> UIDataList;

    class UIStorage {
        static UIDataList data_;

    public:
        static bool add(TimelineData* data);
        static TimelineData* at(size_t pos);
        static void remove(TimelineData* data);
        static bool exists(TimelineData* data);
        static size_t size();
    };

    int trigger_actions(t_canvas* cnv, size_t idx);
}
}

#endif // CEAMMC_TIMELINE_H
