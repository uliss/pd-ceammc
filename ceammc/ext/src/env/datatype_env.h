#ifndef DATATYPE_ENV_H
#define DATATYPE_ENV_H

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"
#include "ceammc_convert.h"
#include "ceammc_data.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

using namespace ceammc;

/* @note when add new type you have to update render functions and to_string function */
enum CurveType {
    CURVE_STEP = 0,
    CURVE_LINE = 1,
    CURVE_EXP = 2,
    CURVE_SIN2 = 3,
    CURVE_SIGMOID = 4
};

/**
 * @brief symbol2curve
 * @param s - curve symbol name
 * @param t - reference to output value
 * @return true on success, false on error
 */
bool symbol2curve(t_symbol* s, CurveType& t);

/**
 * @brief curve2symbol
 * @param t - curve type
 * @return 0-pointer on error
 */
t_symbol* curve2symbol(CurveType t);

/**
 * @brief isValidCurve
 * @param s
 * @return
 */
bool isValidCurve(t_symbol* s);

struct EnvelopePoint {
    size_t utime;
    double value;
    float data;
    float sigmoid_skew;
    CurveType type;
    bool stop;

    enum FixType : std::uint8_t {
        FIX_NONE = 0,
        FIX_TIME = 1 << 0,
        FIX_VALUE = 1 << 1,
        FIX_BOTH = FIX_TIME | FIX_VALUE
    };

    std::uint8_t fix_pos;

    EnvelopePoint(size_t time_us, double v, bool stop_node = false, CurveType t = CURVE_LINE, float curve = 0.f)
        : utime(time_us > 0 ? time_us : 0)
        , value(v)
        , data(curve)
        , sigmoid_skew(16)
        , type(t)
        , stop(stop_node)
        , fix_pos(FIX_NONE)
    {
    }

    double timeMs() const { return utime / 1000.0; }
    void setFixTime() { fix_pos |= FIX_TIME; }
    void setFixValue() { fix_pos |= FIX_VALUE; }
    void setFixNone() { fix_pos = FIX_NONE; }
    void setFixBoth() { fix_pos = FIX_BOTH; }
    void toggleFixTime() { fix_pos ^= FIX_TIME; }
    void toggleFixValue() { fix_pos ^= FIX_VALUE; }
    void unsetFixTime() { fix_pos &= (~FIX_TIME); }
    void unsetFixValue() { fix_pos &= (~FIX_VALUE); }

    AtomList toList() const;
};

/**
 * Compare envelope points
 * @param p0
 * @param p1
 * @return true if envelope points are identical
 */
bool operator==(const EnvelopePoint& p0, const EnvelopePoint& p1);

/**
 * Calculate time interval between points (signed) in millieseconds
 * @param p0
 * @param p1
 * @return
 */
static double intervalMs(const EnvelopePoint& p0, const EnvelopePoint& p1)
{
    return double(p0.utime - p1.utime) / 1000.0;
}

typedef std::vector<EnvelopePoint> PointList;

class DataTypeEnv;
using EnvAtom = DataAtom<DataTypeEnv>;

class DataTypeEnv : public AbstractData {
    PointList points_;

public:
    typedef PointList::iterator iterator;

public:
    DataTypeEnv();
    DataTypeEnv(const DataTypeEnv& env);
    DataTypeEnv(DataTypeEnv&& env);
    DataTypeEnv& operator=(const DataTypeEnv& env);
    DataTypeEnv& operator=(DataTypeEnv&& env);

    int type() const noexcept;
    std::string toString() const;
    bool isEqual(const AbstractData* d) const noexcept;

    iterator begin();
    iterator end();
    bool empty() const;
    const EnvelopePoint& front() const;
    const EnvelopePoint& back() const;
    EnvelopePoint& front();
    EnvelopePoint& back();

    /**
     * Envelope length (between first and last points), in microseconds, unsigned.
     */
    size_t totalLength() const;

    /**
     * Returns number of points in envelope
     */
    size_t numPoints() const;

    /**
     * Get specified point
     * @param idx - point index
     * @return reference to specified point
     * @throw std::out_of_range if wrong index given
     */
    EnvelopePoint& pointAt(size_t idx);
    const EnvelopePoint& pointAt(size_t idx) const;

    /**
     * Calculate envelope value at specified time in microseconds
     * @param time_us - time in microseconds
     * @return envelope value
     */
    double valueAtTime(size_t time_us) const;

    /**
     * Resize envelope to specified length in microseconds
     */
    void resizeTime(size_t time_us);

    /**
     * Shifts all envelope points by specified time in microseconds
     */
    void shiftTime(long time_us);

    /**
     * Scales all envelope points time by specified factor.
     */
    void scaleTime(double factor);

    /**
      * Scales all envelope values by specified factor
      */
    void scaleValue(double factor);

    /**
     * Returns new envelope in which point values are normalized to range [0-1] and
     * shifted to start at 0 time.
     */
    DataTypeEnv normalize() const;

    /**
     * Returns pair of min and max point values in envelope.
     * @return pair(0, 0) on empty envelope
     */
    std::pair<float, float> valueRange() const;

    /**
     * Returns iterator to next stop point or end() if no more stop points
     * @param pos - start point
     * @return end() on error
     */
    iterator nextStopPoint(iterator pos);

    /**
     * Returns next stop point index or index of last point if no more stop points
     * @param current_stop index
     * @return -1 on error
     */
    long nextStopIdx(size_t current_stop);

    /**
     * Returns true if envelope has stop points
     */
    bool hasStopPoints() const;

    /**
     * Returns number of stop points in envelope
     */
    size_t numStopPoints() const;

    bool hasPointAtTime(size_t time_us) const;
    EnvelopePoint* pointAtTime(size_t time_us);
    const EnvelopePoint* pointAtTime(size_t time_us) const;

    /**
     * Insert point to envelope. Point is inserted into right place by its time value
     * If point already exists at this time - replaces it
     * @param n - new point
     * @return index of inserted point
     */
    size_t insertPoint(const EnvelopePoint& n);
    bool removePointByTime(size_t time_us);

    /**
     * Remove point by specified index
     * @return true on success
     */
    bool removePoint(size_t idx);

    /**
     * Appends new segment to the end of envelope
     * @param time_us - length of segment in microseconds
     * @param end_value - envelope value at the end of segment
     * @param t - segment type
     * @param curve_skew - if exponential curve or sigmoid curve used specified curve degree
     */
    bool appendSegment(size_t time_us, double end_value, CurveType t = CURVE_LINE, double curve_skew = 0);

    /**
     * Remove all points
     */
    void clear();

    void setAR(size_t attack_us, size_t release_us, double value = 1);
    void setEAR(size_t attack_us, float attack_curve, size_t release_us, float release_curve);
    void setASR(size_t attack_us, size_t release_us, double value = 1);
    void setEASR(size_t attack_us, float attack_curve, size_t release_us, float release_curve);
    void setADSR(size_t attack_us, size_t decay_us, double sustain_level, size_t release_us);
    void setEADSR(size_t attack_us, float attack_curve,
        size_t decay_us, float decay_curve,
        double sustain_level,
        size_t release_us, float release_curve);

    // linear envelopes
    bool setAR(const AtomListView& lv);
    bool setASR(const AtomListView& lv);
    bool setADSR(const AtomListView& lv);

    // exponential envelopes
    bool setEAR(const AtomListView& lv);
    bool setEASR(const AtomListView& lv);
    bool setEADSR(const AtomListView& lv);

    bool setStep(const AtomListView& lv);
    bool setLine(const AtomListView& kv);
    bool setExponential(const AtomListView& lv);
    bool setSin2(const AtomListView& lv);
    bool setSigmoid(const AtomListView& lv);

    bool setNamedEnvelope(const char* name, const AtomListView& args);
    bool isNamedEnvelope(const char* name) const;

    DataTypeEnv* clone() const;

    /**
     * Render envelope to fit output range (end-begin)
     */
    template <class T>
    void render(T begin, T end) const;
    void render(std::vector<float>::iterator first, std::vector<float>::iterator last);

    /**
     * Envelope compare
     */
    bool operator==(const DataTypeEnv& e) const;

    /**
     * Concatenates another envelope
     */
    DataTypeEnv& operator+=(const DataTypeEnv& env);

    /**
     * Create new envelope from raw list content
     */
    static DataTypeEnv fromListView(const AtomListView& lv);

    /**
     * Saves envelope to raw list data for usage in e.g. in preset system
     */
    AtomList toList() const;

    /**
     * Sorts points in envelope
     */
    void sort();

    static bool interpolated(const DataTypeEnv& env0, const DataTypeEnv& env1, float v, DataTypeEnv& dest);

    friend DataTypeEnv operator+(const DataTypeEnv& env0, const DataTypeEnv& env1);

    /**
     * Checks if envelope is linear A(S)R - contains three points connected with linear segments
     * @param checkVal if true - check values at envelope points: 0 at attack, 1 at sustain, 0 at release
     *        otherwise no checks are performed
     * @return true on success
     */
    bool isAR(bool checkVal = false) const;

    /**
     * Checks if envelope is linear ADSR - contains four points connected with linear segments
     * @param checkVal if true - check values at envelope points: 0 at attack, 1 at sustain, 0 at release
     *        otherwise no checks are performed
     * @return true on success
     */
    bool isADSR(bool checkVal = false) const;

public:
    static int dataType;

    using setMethod = bool (DataTypeEnv::*)(const AtomListView&);

    struct NamedMethod {
        uint32_t crc32_hash;
        setMethod m;
    };

    using NamedMethodList = std::array<NamedMethod, 11>;

    static const NamedMethodList named_methods;
};

std::ostream& operator<<(std::ostream& os, const DataTypeEnv& env);
DataTypeEnv operator+(const DataTypeEnv& env0, const DataTypeEnv& env1);

template <class T>
void DataTypeEnv::render(T first, T last) const
{
    if (points_.empty())
        return;

    size_t start_time = points_.front().utime;
    size_t stop_time = points_.back().utime;
    long time_len = stop_time - start_time;

    if (time_len < 1)
        return;

    double d = std::distance(first, last);
    long segment_idx = 0;
    size_t segment_begin_tm = 0;
    size_t segment_count = points_.size() - 1;
    size_t segment_end_tm = 0;

    for (long i = 0; first != last; ++first, ++i) {
        double t = (time_len * i) / (d - 1);

        // find segment index
        while (segment_idx < segment_count) {
            segment_begin_tm = points_[segment_idx].utime;
            segment_end_tm = points_[segment_idx + 1].utime;

            if (segment_begin_tm <= t && t < segment_end_tm)
                break;

            if (t == segment_end_tm) {
                *first = points_[segment_idx + 1].value;
                goto break2;
            }

            segment_idx++;
        }

        switch (points_[segment_idx].type) {
        case CURVE_STEP:
            *first = points_[segment_idx].value;
            break;
        case CURVE_LINE:
            *first = (t - segment_begin_tm) / (segment_end_tm - segment_begin_tm)
                    * (points_[segment_idx + 1].value - points_[segment_idx].value)
                + points_[segment_idx].value;
            break;
        case CURVE_EXP:
            *first = convert::lin2curve(double(t), double(segment_begin_tm), double(segment_end_tm),
                double(points_[segment_idx].value),
                double(points_[segment_idx + 1].value),
                double(points_[segment_idx].data));
            break;
        case CURVE_SIN2:
            *first = convert::lin2sin2(t, segment_begin_tm, segment_end_tm,
                points_[segment_idx].value, points_[segment_idx + 1].value);
            break;
        case CURVE_SIGMOID:
            *first = convert::lin2sigmoid(t, segment_begin_tm, segment_end_tm,
                points_[segment_idx].value, points_[segment_idx + 1].value, points_[segment_idx].sigmoid_skew);
            break;
        default:
            break;
        }
    break2:
        continue;
    }
}

#endif // DATATYPE_ENV_H
