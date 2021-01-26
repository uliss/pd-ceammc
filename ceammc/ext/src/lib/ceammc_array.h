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
#ifndef CEAMMC_ARRAY_H
#define CEAMMC_ARRAY_H

#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <string>

#include "ceammc_atomlist.h"
#include "ceammc_signal.h"

namespace ceammc {

class Array;

class ArrayIterator : public std::iterator<std::random_access_iterator_tag, t_float> {
    word* data_;

    ArrayIterator(word* ptr);
    friend class Array;

public:
    ArrayIterator();
    ArrayIterator(const ArrayIterator& i);
    ArrayIterator& operator=(const ArrayIterator& i);

    bool operator!=(ArrayIterator const& other) const { return data_ != other.data_; }
    bool operator==(ArrayIterator const& other) const { return data_ == other.data_; }
    bool operator>=(const ArrayIterator& it) const { return data_ >= it.data_; }
    bool operator>(const ArrayIterator& it) const { return data_ > it.data_; }
    bool operator<(const ArrayIterator& it) const { return data_ < it.data_; }
    bool operator<=(const ArrayIterator& it) const { return data_ <= it.data_; }

    t_float& operator*() { return data_->w_float; }
    const t_float& operator*() const { return data_->w_float; }
    t_float& operator[](const size_t n) { return data_[n].w_float; }

    inline ArrayIterator& operator++()
    {
        ++data_;
        return *this;
    }

    inline ArrayIterator& operator--()
    {
        --data_;
        return *this;
    }

    inline ArrayIterator operator++(int)
    {
        ArrayIterator tmp(*this);
        ++data_;
        return tmp;
    }

    inline ArrayIterator operator--(int)
    {
        ArrayIterator tmp(*this);
        --data_;
        return tmp;
    }

    difference_type operator-(const ArrayIterator& it) const;

    inline ArrayIterator& operator+=(difference_type v)
    {
        data_ += v;
        return *this;
    }

    inline ArrayIterator& operator-=(difference_type v)
    {
        data_ -= v;
        return *this;
    }

    inline ArrayIterator operator+(difference_type v) { return ArrayIterator(data_ + v); }
    inline ArrayIterator operator-(difference_type v) { return ArrayIterator(data_ - v); }

    friend ArrayIterator operator+(difference_type, const ArrayIterator& it);
};

ArrayIterator operator+(ArrayIterator::difference_type, const ArrayIterator& it);

typedef t_float (*FloatValueGenerator)(size_t n);

class Array {
    t_symbol* name_;
    _garray* array_;
    size_t size_;
    word* data_;

public:
    typedef ArrayIterator iterator;
    typedef ArrayIterator const_iterator;
    typedef t_sample value_type;

public:
    Array();
    Array(t_symbol* name);
    Array(const char* name);
    Array(const char* name, std::initializer_list<t_sample> l);

    Array(const Array& array);
    Array& operator=(const Array& array);

    /** iterators */
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const iterator end() const;

    /**
     * You should call this function every time before other function, to assure that array was not
     * resized or deleted
     * @return false if array is invalid and can not be opened
     */
    bool update();

    /**
     * You should call this function after all array operation, to update array graphics view
     */
    void redraw();

    /**
     * Check if array is opened
     */
    bool isValid() const;

    /**
     * Tries to open array specified by name
     * @return true on success, false on error
     */
    bool open(t_symbol* name);
    bool open(const char* name);

    /**
     * Sets *use_in_dsp* array flag:
     *  if array will be resized, setupDSP procedure will be called, otherwise not
     * @return true on success. false on error
     */
    void useInDSP();

    /**
     * @brief returns number of samples in array
     */
    size_t size() const { return size_; }

    /**
     * @brief array name as string
     */
    t_symbol* name() const { return name_; }

    /**
     * @brief at - get reference to sample with at position
     * @param n - sample position
     * @return reference to sample
     * @throw Exception if invalid array or invalid sample number
     */
    const t_float& at(size_t n) const;
    t_float& at(size_t n);

    /**
     * @brief get reference to sample with at position
     * @param n - sample position
     * @return reference to sample
     * @note no checks are performed
     */
    inline const t_float& operator[](size_t n) const { return data_[n].w_float; }
    inline t_float& operator[](size_t n) { return data_[n].w_float; }

    /**
     * @brief resize array to new size
     * @param n - new size
     * @return true on success, false on error
     */
    bool resize(size_t n);

    /**
     * @brief copyFrom source array
     * @param src - pointer to source array
     * @param n - number of samples to copy (performs array bound check)
     */
    void copyFrom(const t_float* src, size_t n);

    /**
     * @brief copyTo dest array
     * @param dest - pointer to destination
     * @param n - number of samples to copy
     */
    void copyTo(t_float* dest, size_t n);

    /**
     * @brief fillWith - fill all array with specified value
     * @param v - fill value
     */
    void fillWith(t_float v);

    /**
     * @brief fillWith - fill all array with generated values
     * @param gen - generator function, first argument is current array index
     */
    void fillWith(FloatValueGenerator gen);

    /**
     * @brief set array content from list
     * @param l - list of values
     * @return true on success, false on error
     */
    bool set(const AtomList& l);

    /**
     * @brief set array content with initializer list
     * @param l - list of values
     * @return true on success, false on error
     */
    bool set(std::initializer_list<t_sample> l);

    /**
     * Sets array bounds
     */
    bool setYBounds(t_float yBottom, t_float yTop);

    /**
     * @brief setYTicks - set array y ticks
     * @param y - value to draw big tick
     * @param step - tick value step
     * @param bigN - big tick every bigN, 0 - to disable all ticks
     * @return true on sucess, false on error
     */
    bool setYTicks(t_float y, t_float step, size_t bigN);

    /**
     * @brief setYLabels
     * @param labels - list of float values on which show labels
     * @return true on sucess, false on error
     */
    bool setYLabels(const AtomList& labels);

    /**
     * If true - save array contains in patch (convenient for small arrays)
     */
    bool setSaveInPatch(bool value);

    /**
     * Sends normalize message to array, redraws
     */
    bool normalize(t_float f = 1);

    /**
     * Non interpolated read
     * @param fpos - fraction read position, expected: 0 <= fpos && fpos < ARRAYSIZE
     * @return sample value
     * @note no range check is performed, could segfault
     */
    t_sample readUnsafe0(double fpos) const { return data_[static_cast<size_t>(fpos)].w_float; }

    /**
     * Read value from array with linear interpolation
     * @param fpos - fraction read position, expected: 0 <= fpos && fpos < ARRAYSIZE-1
     * @return interpolated sample value
     * @note no range check is performed, could segfault
     */
    t_sample readUnsafe1(double fpos) const;

    /**
     * Read value from array with cubic hermite interpolation
     * @param fpos - fraction read position, expected: 1 <= fpos && fpos < ARRAYSIZE-1
     * @return interpolated sample value
     * @note no range check is performed, could segfault
     */
    t_sample readUnsafe3(double fpos) const;

    /**
     * Non interpolated read
     * @arg fpos - fraction read position
     * @return array sample value if 0 <= fpos && fpos < ARRAYSIZE, otherwise 0
     */
    t_sample readSafe0(double fpos) const;

    /**
     * Read value from array with linear interpolation
     * @param fpos - fraction read position
     * @return array sample value if 0 <= fpos && fpos < ARRAYSIZE, otherwise 0
     */
    t_sample readSafe1(double fpos) const;

    /**
     * Read value from array with cubic hermite interpolation
     * @param fpos - fraction read position
     * @return array sample value if 0 <= fpos && fpos < ARRAYSIZE, otherwise 0
     */
    t_sample readSafe3(double fpos) const;

public:
    struct Exception : public std::runtime_error {
        Exception(const char* what);
    };
};

inline t_sample Array::readUnsafe1(double fpos) const
{
    const auto ipos = static_cast<size_t>(fpos);
    const auto x0 = data_[ipos].w_float;
    const auto x1 = data_[ipos + 1].w_float;
    const auto t = fpos - double(ipos); // fractional part

    return interpolate::linear<double>(x0, x1, t);
}

inline t_sample Array::readUnsafe3(double fpos) const
{
    const auto ipos = static_cast<size_t>(fpos);
    const auto x0 = data_[ipos - 1].w_float;
    const auto x1 = data_[ipos].w_float;
    const auto x2 = data_[ipos + 1].w_float;
    const auto x3 = data_[ipos + 2].w_float;
    const double t = fpos - double(ipos); // fractional part

    return interpolate::cubic_hermite<double>(x0, x1, x2, x3, t);
}

inline t_sample Array::readSafe0(double fpos) const
{
    if (fpos < 0)
        return 0;

    const auto ipos = static_cast<size_t>(fpos);
    return (ipos >= size_) ? 0 : data_[ipos].w_float;
}

inline t_sample Array::readSafe1(double fpos) const
{
    if (fpos < 0)
        return 0;

    const auto ipos = static_cast<size_t>(fpos);

    if (ipos >= size_)
        return 0;

    const auto x0 = data_[ipos].w_float;
    if ((ipos + 1) >= size_)
        return x0;

    const auto x1 = data_[ipos + 1].w_float;
    const auto t = fpos - double(ipos); // fractional part

    return interpolate::linear<double>(x0, x1, t);
}

inline t_sample Array::readSafe3(double fpos) const
{
    if (fpos < 0)
        return 0;

    const auto ipos = static_cast<size_t>(fpos);

    if (ipos >= size_)
        return 0;

    const auto x1 = data_[ipos].w_float;
    const auto x0 = (ipos < 1) ? x1 : data_[ipos - 1].w_float;
    const auto x2 = (ipos + 1 >= size_) ? x1 : data_[ipos + 1].w_float;
    const auto x3 = (ipos + 2 >= size_) ? x2 : data_[ipos + 2].w_float;
    const double t = fpos - double(ipos); // fractional part

    return interpolate::cubic_hermite<double>(x0, x1, x2, x3, t);
}
}

#endif // CEAMMC_ARRAY_H
