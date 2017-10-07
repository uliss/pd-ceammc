//
//  ceammc_bpf.h
//  pd_ext
//
//  Created by Alex Nadzharov on 21/01/17.
//
//

#include <algorithm>
#include <cstdio>
#include <map>
#include <vector>

#include "ceammc_atomlist.h"
#include "ceammc_gui.h"

using namespace std;
using namespace ceammc;
using namespace ceammc_gui;

namespace ceammc {

/**
 * @brief structure for single point
 * @details
 */

struct t_bpt {
    int idx;
    float x;
    float y;

    float dist; ///< distance from mouse. recalculated on hover
    float ldist; ///< distance to line
    float range_x; ///< stub for envelopes with different x scale

    bool selected; ///< selected flag. should be moved
    bool end_segment; ///< end segment flag. for envelopes
    bool lock_x; ///< lock flag x
    bool lock_y; ///< lock flag y
    bool join_next; ///< join flag - for hold envelopes
};

typedef std::vector<t_bpt> t_bpf_points;

/**
 * @brief   Breakpoint Function Class
 * @details
 * @param
 */

class BPF {
    t_bpf_points points_;
    int _seg_count;
    vector<int> seg_idx_;

    static bool sort_by_x_pred(const t_bpt& a, const t_bpt& b)
    {
        return a.x < b.x;
    }

public:
    float range_x;
    float range_y;
    float shift_x;
    float shift_y;

    bool drag_limit;

    /**
     * @brief   Accepts raw pairs of x y values and creates a function
     * @details
     * @param
     */
    void setBPFListRaw(const AtomList& list)
    {
        empty();

        size_t num_points = list.size() / 2;

        for (size_t i = 0; i < num_points; i++)
            addPointRaw(i, list.at(i * 2).asFloat(), list.at(i * 2 + 1).asFloat());
    }

    /**
     * @brief   Accepts pairs of x y values in current scales and creates a function
     * @details
     * @param
     */
    void setBPFList(const AtomList& list)
    {
        empty();
        size_t num_points = list.size() / 2;

        for (size_t i = 0; i < num_points; i++)
            addPoint(i, list.at(i * 2).asFloat(), list.at(i * 2 + 1).asFloat());
    }

    /**
     * @brief creates empty BPF and sets several default properties
     * @details
     * @param
     */
    void empty()
    {
        points_.clear();
        _seg_count = 1;
        seg_idx_.clear();
        seg_idx_.push_back(0);
    }

    /**
     * @brief inits ranges. should probably be moved
     * @details
     * @param
     */
    //temporary
    void initRange()
    {
        shift_x = 0;
        shift_y = 0;
        range_x = 1000;
        range_y = 1;
    }

    /**
     * @brief creates default BPF with 3 points
     * @details
     * @param
     */
    void clear()
    {
        empty();
        addPointRaw(0, 0, 0);
        addPointRaw(1, 0.5, 0.75);
        addPointRaw(2, 1, 1);
    }

    BPF()
    {
        clear();
    }

#pragma mark point

    /**
     * @brief sorts points. should be private (see ui.bpfunc mouse handling)
     * @details
     * @param
     */
    void sortPoints()
    {
        std::sort(points_.begin(), points_.end(), sort_by_x_pred);
    }

    /**
     * @brief add point for specified index using raw values
     * @details
     * @param
     */
    void addPointRaw(size_t idx, float x, float y)
    {
        t_bpt pt1;
        pt1.idx = idx;
        pt1.x = x;
        pt1.y = y;

        pt1.lock_x = false;
        pt1.lock_y = false;
        pt1.end_segment = false;

        points_.push_back(pt1);
        sortPoints();
    }

    /**
     * @brief add point for specified index with scaled values
     * @details
     * @param
     */
    void addPoint(size_t idx, float x, float y)
    {
        addPointRaw(idx, (x - shift_x) / range_x, (y - shift_y) / range_y);
    }

    /**
     * @brief delete point at specified index
     * @details
     * @param
     */
    void deletePoint(size_t idx)
    {
        if (idx < points_.size() && points_.size() > 2) {
            points_.erase(points_.begin() + idx);
            sortPoints();
        }
    }

    /**
     * @brief get the index of the point that has x value less than specified x
     * @details
     * @param
     */
    int findPointByRawX(float x) const
    {
        for (size_t i = 0; i < points_.size(); i++) {
            if (points_[i].x >= x)
                return int(i);
        }

        return -1;
    }

#pragma mark set point values

    /**
     * @brief sets raw point value - similar to addraw
     * @details
     * @param
     */
    void setPointRaw(size_t idx, float x, float y)
    {
        if (idx >= points_.size())
            return;

        points_[idx].x = x;
        points_[idx].y = y;
        sortPoints();
    }

    /**
     * @brief sets scaled point value - similar to add
     * @details
     * @param
     */
    void setPoint(size_t idx, float x, float y)
    {
        setPointRaw(idx, (x - shift_x) / range_x, (y - shift_y) / range_y);
    }

    /** sets/inverts split flag - should be merged with setPointSeg
     * @brief
     * @details
     * @param
     */
    void setSplit(size_t idx)
    {
        if (idx >= points_.size())
            return;

        points_[idx].end_segment = !points_[idx].end_segment;

        if (points_[idx].end_segment) {
            _seg_count++;

            seg_idx_.push_back(idx);
            std::sort(seg_idx_.begin(), seg_idx_.end());
        } else {
            _seg_count--;

            vector<int>::iterator pos = find(seg_idx_.begin(), seg_idx_.end(), idx);
            if (pos != seg_idx_.end())
                seg_idx_.erase(pos);
        }
    }

    /**
     * @brief sets split flag
     * @details
     * @param
     */
    void setPointSeg(size_t idx, bool val)
    {
        if (idx >= points_.size())
            return;

        points_[idx].end_segment = val;

        if (val) {
            _seg_count++;
            seg_idx_.push_back(idx);
            std::sort(seg_idx_.begin(), seg_idx_.end());
        } else {
            _seg_count--;

            vector<int>::iterator pos = find(seg_idx_.begin(), seg_idx_.end(), idx);
            if (pos != seg_idx_.end())
                seg_idx_.erase(pos);
        }
    }

    /**
     * @brief sets lock x flag
     * @details
     * @param
     */
    void lockX(size_t idx, bool value)
    {
        if (idx >= points_.size())
            return;

        points_.at(idx).lock_x = value;
    }

    /**
     * @brief sets lock y flag
     * @details
     * @param
     */
    void lockY(size_t idx, bool value)
    {
        if (idx >= points_.size())
            return;

        points_.at(idx).lock_y = value;
    }

    /**
     * @brief sets join next flag
     * @details
     * @param
     */
    void joinNext(size_t idx, bool value)
    {
        if (idx >= points_.size())
            return;

        points_.at(idx).join_next = value;
    }

#pragma mark needed by paint

    float getPointRawX(size_t idx) const
    {
        if (idx >= points_.size())
            return 0;

        return points_.at(idx).x;
    }

    float getPointRawY(size_t idx) const
    {
        if (idx >= points_.size())
            return 0;

        return points_.at(idx).y;
    }

    float getPointLockX(size_t idx) const
    {
        if (idx >= points_.size())
            return 0;

        return points_.at(idx).lock_x;
    }

    float getPointLockY(size_t idx) const
    {
        if (idx >= points_.size())
            return 0;

        return points_.at(idx).lock_y;
    }

    void setPointDist(size_t idx, float dist)
    {
        if (idx >= points_.size())
            return;

        points_.at(idx).dist = dist;
    }

    float getPointDist(size_t idx) const
    {
        if (idx >= points_.size())
            return 0;

        return points_.at(idx).dist;
    }

    bool getEndSeg(size_t idx) const
    {
        if (idx >= points_.size())
            return false;

        return points_.at(idx).end_segment;
    }

#pragma mark needed by mouse

    void setPointSel(size_t idx, bool sel)
    {
        if (idx >= points_.size())
            return;

        points_.at(idx).selected = sel;
    }

    bool getPointSel(size_t idx) const
    {
        if (idx >= points_.size())
            return false;

        return points_.at(idx).selected;
    }

    bool getJoinNext(size_t idx) const
    {
        if (idx >= points_.size())
            return false;

        return points_.at(idx).join_next;
    }

#pragma mark get messages

    /**
     * @brief gets list of x y raw value pairs
     * @details
     * @param
     */
    AtomList getBpfListRaw() const
    {
        AtomList ret;

        t_bpf_points::const_iterator it;
        for (it = points_.begin(); it != points_.end(); ++it) {
            ret.append(Atom(it->x));
            ret.append(Atom(it->y));
        }

        return ret;
    }

    /**
     * @brief gets list of scaled x y value pairs
     * @details
     * @param
     */
    AtomList getBpfList() const
    {
        AtomList ret;

        t_bpf_points::const_iterator it;
        for (it = points_.begin(); it != points_.end(); ++it) {
            ret.append(Atom(it->x * range_x + shift_x));
            ret.append(Atom(it->y * range_y + shift_y));
        }

        return ret;
    }

    /**
     * @brief gets x y scaled values for point with specified index
     * @details
     * @param
     */
    AtomList getPoint(size_t idx) const
    {
        if (idx >= points_.size())
            return AtomList(Atom(0.f), Atom(0.f));

        AtomList ret;

        ret.append(Atom(points_.at(idx).x * range_x + shift_x));
        ret.append(Atom(points_.at(idx).y * range_y + shift_y));

        return ret;
    }

    /**
     * @brief gets raw x y values for point with specified index
     * @details
     * @param
     */
    AtomList getPointRaw(size_t idx) const
    {
        if (idx >= points_.size())
            return AtomList(Atom(0.f), Atom(0.f));

        AtomList ret;

        ret.append(Atom(points_.at(idx).x));
        ret.append(Atom(points_.at(idx).y));

        return ret;
    }

    /**
     * @brief gets messages suited for vline for the whole BPF
     * @details
     * @param
     */
    vector<AtomList> getVline() const
    {
        vector<AtomList> ret;

        float last_time = 0;

        for (size_t j = 0; j < points_.size(); j++) // i is on vacation
        {
            AtomList list;

            float this_time = points_.at(j).x * range_x + shift_x;

            list.append(Atom(points_.at(j).y * range_y + shift_y));
            list.append(Atom(this_time));
            list.append(Atom(last_time));

            last_time += this_time;

            ret.push_back(list);
        }

        return ret;
    }

    /**
     * @brief gets messages suited for vline for a specific segment
     * @details
     * @param
     */
    vector<AtomList> getVlineSegment(size_t seg) const
    {
        vector<AtomList> ret;

        vector<int>::const_iterator it = seg_idx_.begin();
        advance(it, seg);

        int seg_index_start = *it;
        int seg_index_end = (int)points_.size() - 1;

        if (seg < (_seg_count - 1)) {
            advance(it, 1);
            seg_index_end = *it;
        }

        float last_time = 0;

        for (int j = seg_index_start; j <= seg_index_end; j++) //i is on vacation
        {

            AtomList list;

            float this_time = points_.at(j).x * range_x + shift_x;

            list.append(Atom(points_.at(j).y * range_y + shift_y));
            list.append(Atom(this_time));
            list.append(Atom(last_time));

            last_time += this_time;

            ret.push_back(list);
        }

        return ret;
    }

    /**
     * @brief gets list of values - conv.bpf2list
     * @details
     * @param
     */
    AtomList getVector(int size) const
    {
        AtomList ret;

        for (int i = 0; i < size; i++) {
            float x = float(i) / float(size);

            float y1 = 0;
            float y2 = 0;

            int next_idx = findPointByRawX(x);
            //int next_idx = (idx==(_points.size()-1)) ? idx : idx+1;

            int idx = (next_idx > 0) ? next_idx - 1 : next_idx;

            //if (idx)
            {

                float x1 = points_[idx].x;
                float x2 = points_[next_idx].x;
                float dx = x2 - x1;
                float mx = (dx > 0.00001) ? (x - x1) / dx : 0;

                y1 = points_[idx].y;
                y2 = points_[next_idx].y;

                float y = mx * y2 + (1 - mx) * y1;

                ret.append(Atom(y));
            }
        }

        return ret;
    }

#pragma mark get data

    size_t size() const
    {
        return points_.size();
    }

    int getSegCount() const
    {
        return _seg_count;
    }
};
}
