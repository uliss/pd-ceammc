//Set of C++ wave table function
//©Romain Michon (rmichon@ccrma.stanford.edu), 2011
//licence: STK-4.3

#ifndef FAUST_STK_INSTRUMENT_H_
#define FAUST_STK_INSTRUMENT_H_

#include <cstddef>
#include <cstdio>
#include <cstdlib>

namespace faust {
namespace synth {

    constexpr size_t TABLE_SIZE = 1024;

    //******************************************************
    //functions for parameters lookup and interpolation
    //******************************************************

    class LookupTable {
    protected:
        // Note: Actual array size is 2*m_nPoints;
        const double* points_;
        size_t size_;

    public:
        LookupTable(const double* points, size_t num_points);
        double getValue(double x) const;
    };

    inline LookupTable::LookupTable(const double* points, size_t num_points)
        : points_(points)
        , size_(num_points)
    {
        // Note: Actual array size is 2*num_points
    }

    //********************************
    //stick for modal synthesis
    //********************************
    double readMarmstk1(size_t index);

}
}

#endif
