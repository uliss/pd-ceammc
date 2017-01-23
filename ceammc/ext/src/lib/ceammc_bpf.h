//
//  ceammc_bpf.h
//  pd_ext
//
//  Created by Alex Nadzharov on 21/01/17.
//
//

#include <stdio.h>

#include "ceammc_gui.h"

#include "ceammc_atomlist.h"

#include <map>
#include <algorithm>

#include <vector>


using namespace std;
using namespace ceammc;
using namespace ceammc_gui;


namespace ceammc {
    
    /**
     * @brief       structure for single point
     * @details
     */
    
    typedef struct t_bpt
    {
        int idx;
        float x;
        float y;
        
        float dist;         ///< distance from mouse. recalculated on hover
        float ldist;        ///< distance to line
        bool selected;      ///< selected flag. should be moved
        
        bool end_segment;   ///< end segment flag. for envelopes
        float range_x;      ///< stub for envelopes with different x scale
        
        bool lock_x;        ///< lock flag x
        bool lock_y;        ///< lock flag y
        
        bool join_next;     ///< join flag - for hold envelopes
        
    } t_bpt;
    
    typedef std::vector<t_bpt> t_bpf_points;
    
    /**
     * @brief   Breakpoint Function Class
     * @details
     * @param
     */
    
    class BPF
    {
    private:
        t_bpf_points _points;
        int _seg_count;
        vector<int> _seg_idx;
        
        static const bool _BPFSortPred(t_bpt const & a, t_bpt const & b)  {
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
        
        void setBPFListRaw(AtomList list)
        {
            int num_points = floor(list.size()/2);
            
            
            this->empty();
            
            for (int i=0; i<num_points; i++)
            {
                this->addPointRaw(i, list.at(i*2).asFloat(), list.at(i*2+1).asFloat());
            }
        
        }
        
        /**
         * @brief   Accepts pairs of x y values in current scales and creates a function
         * @details
         * @param
         */
        
        void setBPFList(AtomList list)
        {
            int num_points = floor(list.size()/2);
            
            
            this->empty();
            
            for (int i=0; i<num_points; i++)
            {
                this->addPoint(i, list.at(i*2).asFloat(), list.at(i*2+1).asFloat());
            }
            
        }
        
        /**
         * @brief   creates empty BPF and sets several defaupt properties
         * @details
         * @param
         */
        void empty()
        {
            this->_points.clear();
            
            this->_seg_count = 1;
            
            this->_seg_idx.clear();
            this->_seg_idx.push_back(0);
            
            
        }
        
        /**
         * @brief inits ranges. should probably be moved
         * @details
         * @param
         */
        
        //temporary
        void initRange()
        {
            this->shift_x = 0;
            this->shift_y = 0;
            this->range_x = 1000;
            this->range_y = 1;
        
        }
        
        /**
         * @brief creates default BPF with 3 points
         * @details
         * @param
         */
        
        void clear()
        {
            this->empty();
            
            this->addPointRaw(0, 0, 0);
            this->addPointRaw(1, 0.5, 0.75);
            this->addPointRaw(2, 1, 1);
            
        }
        
        BPF()
        {
            this->clear();
        }
        
        
        
#pragma mark point
        
        /**
         * @brief sorts points. should be private (see ui.bpfunc mouse handling)
         * @details
         * @param
         */
        
        inline void _Sort()     //?
        {
            
            t_bpf_points *ps = &this->_points;
            
            std::sort(ps->begin(), ps->end(), _BPFSortPred);
            
            
        }
        
        /**
         * @brief add point for specified index using raw values
         * @details
         * @param
         */
        
        void addPointRaw(int idx, float x, float y)
        {
            t_bpt pt1;
            pt1.idx = idx;
            pt1.x = x;
            pt1.y = y;
            
            pt1.lock_x = false;
            pt1.lock_y = false;
            
            pt1.end_segment = false;
            
            this->_points.push_back(pt1);
            
            this->_Sort();
            
            
        }
        
        /**
         * @brief add point for specified index with scaled values
         * @details
         * @param
         */
        void addPoint(int idx, float x, float y)
        {
            this->addPointRaw(idx, (x - this->shift_x)/this->range_x, (y - this->shift_y)/this->range_y);
        }
        
        /**
         * @brief delete point at specified index
         * @details
         * @param
         */
        
        void deletePoint(int idx)
        {
            t_bpf_points *ps = &this->_points;
            
            if (ps->size()>2)
            {
                ps->erase(ps->begin()+idx);
                this->_Sort();
            }
        }
        
        /**
         * @brief get the index of the point that has x value less than specified x
         * @details
         * @param
         */
        
        int findPointByRawX(float x)
        {
            for (int i=0;i<this->_points.size();i++)
            {
                if (this->_points.at(i).x>=x)
                {
                    return i;
                }
            }
            
            return -1;
        }
        
        
        
        
#pragma mark set point values
        
        /**
         * @brief sets raw point value - similar to addraw
         * @details
         * @param
         */
        
        void setPointRaw(int idx, float x, float y)
        {
            this->_points.at(idx).x = x;
            this->_points.at(idx).y = y;
            
            this->_Sort();
        }
        
        /**
         * @brief sets scaled point value - similar to add
         * @details
         * @param
         */
        
        void setPoint(int idx, float x, float y)
        {
            this->setPointRaw(idx, (x-this->shift_x)/this->range_x, (y-this->shift_y)/this->range_y);
        }
        
        /** sets/inverts split flag - should be merged with setPointSeg
         * @brief
         * @details
         * @param
         */
        
        void setSplit(int idx)
        {
            
            t_bpf_points *ps = &this->_points;
            
            ps->at(idx).end_segment = !ps->at(idx).end_segment;
            
            if (ps->at(idx).end_segment)
            {
                this->_seg_count++;
                
                this->_seg_idx.push_back(idx);
                std::sort(this->_seg_idx.begin(), this->_seg_idx.end());
            }
            else
            {
                this->_seg_count--;
                
                vector<int>::iterator pos = find(this->_seg_idx.begin(),this->_seg_idx.end(), idx);
                if (pos != this->_seg_idx.end())
                    this->_seg_idx.erase(pos);
                
            };
            
        }
        
        /**
         * @brief sets split flag
         * @details
         * @param
         */
        void setPointSeg(int idx, bool val)
        {
            
            t_bpf_points *ps = &this->_points;
            
            ps->at(idx).end_segment = val;//!ps->at(idx).end_segment;
            
            if (val)
            {
                this->_seg_count++;
                
                this->_seg_idx.push_back(idx);
                std::sort(this->_seg_idx.begin(), this->_seg_idx.end());
            }
            else
            {
                this->_seg_count--;
                
                vector<int>::iterator pos = find(this->_seg_idx.begin(),this->_seg_idx.end(), idx);
                if (pos != this->_seg_idx.end())
                    this->_seg_idx.erase(pos);
                
            };
            
        }
        
        /**
         * @brief sets lock x flag
         * @details
         * @param
         */
        void lockX(int idx, bool value)
        {
            this->_points.at(idx).lock_x = value;
        }
        
        /**
         * @brief sets lock y flag
         * @details
         * @param
         */
        void lockY(int idx, bool value)
        {
            this->_points.at(idx).lock_y = value;
        }
        
        /**
         * @brief sets join next flag
         * @details
         * @param
         */
        void joinNext(int idx, bool value)
        {
            this->_points.at(idx).join_next = value;
        }
        
#pragma mark needed by paint
        
        float getPointRawX(int idx)
        {
            return this->_points.at(idx).x;
        }
        
        float getPointRawY(int idx)
        {
            return this->_points.at(idx).y;
        }
        
        float getPointLockX(int idx)
        {
            return this->_points.at(idx).lock_x;
        }
        
        float getPointLockY(int idx)
        {
            return this->_points.at(idx).lock_y;
        }
        
        void setPointDist(int idx, float dist)
        {
            this->_points.at(idx).dist = dist;
        }
        
        float getPointDist(int idx)
        {
            return this->_points.at(idx).dist;
        }
        
        bool getEndSeg(int idx)
        {
            return this->_points.at(idx).end_segment;
        }
        
#pragma mark needed by mouse
        
        void setPointSel(int idx, bool sel)
        {
            this->_points.at(idx).selected = sel;
        }
        
        bool getPointSel(int idx)
        {
            return this->_points.at(idx).selected;
        }
        
        bool getJoinNext(int idx)
        {
            return this->_points.at(idx).join_next;
        }
        
        
        
#pragma mark get messages
        
        /**
         * @brief gets list of x y raw value pairs
         * @details
         * @param
         */
        AtomList getBpfListRaw()
        {
            AtomList ret;
            
            t_bpf_points::iterator it;
            for (it = this->_points.begin(); it != this->_points.end(); ++it)
            {
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
        AtomList getBpfList()
        {
            AtomList ret;
            
            t_bpf_points::iterator it;
            for (it = this->_points.begin(); it != this->_points.end(); ++it)
            {
                ret.append(Atom(it->x * this->range_x + this->shift_x));
                ret.append(Atom(it->y * this->range_y + this->shift_y));
            }
            
            return ret;
        }
        
        /**
         * @brief gets x y scaled values for point with specified index
         * @details
         * @param
         */
        AtomList getPoint(int idx)
        {
            AtomList ret;
            
            ret.append(Atom(this->_points.at(idx).x * this->range_x + this->shift_x));
            ret.append(Atom(this->_points.at(idx).y * this->range_y + this->shift_y));
            
            return ret;
            
        }
        
        /**
         * @brief gets raw x y values for point with specified index
         * @details
         * @param
         */
        AtomList getPointRaw(int idx)
        {
            AtomList ret;
            
            ret.append(Atom(this->_points.at(idx).x));
            ret.append(Atom(this->_points.at(idx).y));
            
            return ret;
        }
        
        /**
         * @brief gets messages suited for vline for the whole BPF
         * @details
         * @param
         */
        vector<AtomList> getVline()
        {
            vector<AtomList> ret;
            
            float last_time = 0;
            
            for (int j=0;j<this->_points.size(); j++)      //i is on vacation
            {
                AtomList list;
                
                float this_time = this->_points.at(j).x * this->range_x + this->shift_x;
                
                list.append(Atom(this->_points.at(j).y * this->range_y + this->shift_y));
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
        vector<AtomList> getVlineSegment(int seg)
        {
            vector<AtomList> ret;
            
            vector<int>::iterator it = this->_seg_idx.begin();
            advance(it, seg);
            
            int seg_index_start = *it ;
            int seg_index_end = (int)this->_points.size()-1;
            
            if (seg<(this->_seg_count-1))
            {
                advance(it,1);
                seg_index_end = *it;
            }
            
            float last_time = 0;
            
            for (int j=seg_index_start; j<=seg_index_end; j++)      //i is on vacation
            {
                
                AtomList list;
                
                float this_time = this->_points.at(j).x * this->range_x + this->shift_x;
                
                list.append(Atom(this->_points.at(j).y * this->range_y + this->shift_y));
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
        AtomList getVector(int size)
        {
            AtomList ret;
            
            for (int i=0;i<size;i++)
            {
                float x = float(i)/float(size);
                
                float y1 = 0;
                float y2 = 0;
                
                int next_idx = this->findPointByRawX(x);
                //int next_idx = (idx==(this->_points.size()-1)) ? idx : idx+1;
                
                int idx = (next_idx>0) ? next_idx-1 : next_idx;
                
                //if (idx)
                {
                    
                    float x1 = this->_points[idx].x;
                    float x2 = this->_points[next_idx].x;
                    float dx = x2 - x1;
                    float mx = (dx>0.00001) ? (x - x1)/dx : 0;
                    
                    y1 = this->_points[idx].y;
                    y2 = this->_points[next_idx].y;
                    
                    float y = mx * y2 + (1-mx) * y1;
                    
                    ret.append(Atom(y));
                }
                
                
            }
            
            return ret;
        }
        
        
        
#pragma mark get data
        
        int size()
        {
            return (int)_points.size();
        }
        
        int getSegCount()
        {
            return this->_seg_count;
        }
        
        
    };
    
    
}
