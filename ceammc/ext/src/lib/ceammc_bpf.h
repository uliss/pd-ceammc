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
    
    typedef struct t_bpt
    {
        int idx;
        float x;
        float y;
        
        float dist;         // distance from mouse. recalculated on hover
        float ldist;        //  distance to line
        bool selected;
        
        bool end_segment;
        float range_x;
        
        bool lock_x;
        bool lock_y;
        
        bool join_next;     //
        
    } t_bpt;
    
    typedef std::vector<t_bpt> t_bpf_points;
    
    
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
        
 
        void setBPFList(AtomList list)
        {
            int num_points = floor(list.size()/2);
            
            
            this->clear();
            
            for (int i=0; i<num_points; i++)
            {
                this->addPointRaw(i, list.at(i*2).asFloat(), list.at(i*2+1).asFloat());
            }
        
        }
        void clear()
        {
            this->_points.clear();
            
            this->addPointRaw(0, 0, 0);
            this->addPointRaw(1, 0.5, 0.75);
            this->addPointRaw(2, 1, 1);
            
            this->_seg_count = 1;
            
            this->_seg_idx.clear();
            this->_seg_idx.push_back(0);
        }
        
#pragma mark point
        
        inline void _Sort()     //?
        {
            
            t_bpf_points *ps = &this->_points;
            
            std::sort(ps->begin(), ps->end(), _BPFSortPred);
            
            
        }
        
        void addPointRaw(int idx, float x, float y)
        {
            t_bpt pt1;
            pt1.idx = idx;
            pt1.x = x;
            pt1.y = y;
            
            pt1.lock_x = false;
            pt1.lock_y = false;
            
            pt1.end_segment = false;
            
            t_bpf_points *ps = &this->_points;
            
            ps->push_back(pt1);
            
            this->_Sort();
            
            
        }
        
        void addPoint(int idx, float x, float y)
        {
            this->addPointRaw(idx, (x-this->shift_x)/this->range_x, (y-this->shift_y)/this->range_y);
        }
        
        void deletePoint(int idx)
        {
            t_bpf_points *ps = &this->_points;
            
            if (ps->size()>2)
            {
                ps->erase(ps->begin()+idx);
                this->_Sort();
            }
        }
        
        
        
        
        
#pragma mark set point values
        
        void setPointRaw(int idx, float x, float y)
        {
            this->_points.at(idx).x = x;
            this->_points.at(idx).y = y;
            
            this->_Sort();
        }
        
        void setPoint(int idx, float x, float y)
        {
            this->setPointRaw(idx, (x-this->shift_x)/this->range_x, (y-this->shift_y)/this->range_y);
        }
        
        
        
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
        
        void lockX(int idx, bool value)
        {
            this->_points.at(idx).lock_x = value;
        }
        
        void lockY(int idx, bool value)
        {
            this->_points.at(idx).lock_y = value;
        }
        
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
        
        AtomList getPoint(int idx)
        {
            AtomList ret;
            
            ret.append(Atom(this->_points.at(idx).x * this->range_x + this->shift_x));
            ret.append(Atom(this->_points.at(idx).y * this->range_y + this->shift_y));
            
            return ret;
            
        }
        
        AtomList getPointRaw(int idx)
        {
            AtomList ret;
            
            ret.append(Atom(this->_points.at(idx).x));
            ret.append(Atom(this->_points.at(idx).y));
            
            return ret;
        }
        
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
