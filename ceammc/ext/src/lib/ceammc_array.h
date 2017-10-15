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

#include <iterator>
#include <stdexcept>
#include <string>

struct _garray;
struct _symbol;
union word;

namespace ceammc {

class Array;

class ArrayIterator : public std::iterator<std::random_access_iterator_tag, float> {
    word* data_;

    ArrayIterator(word* ptr);
    friend class Array;

public:
    ArrayIterator(const ArrayIterator& i);

    bool operator!=(ArrayIterator const& other) const { return data_ != other.data_; }
    bool operator==(ArrayIterator const& other) const { return data_ == other.data_; }
    bool operator>=(const ArrayIterator& it) const { return data_ >= it.data_; }
    bool operator>(const ArrayIterator& it) const { return data_ > it.data_; }
    bool operator<(const ArrayIterator& it) const { return data_ < it.data_; }
    bool operator<=(const ArrayIterator& it) const { return data_ <= it.data_; }

    float& operator*();
    const float& operator*() const;
    float& operator[](const size_t n);

    ArrayIterator& operator++();
    ArrayIterator& operator--();
    ArrayIterator operator++(int);
    ArrayIterator operator--(int);
    difference_type operator-(const ArrayIterator& it) const;
    ArrayIterator& operator+=(difference_type v);
    ArrayIterator& operator-=(difference_type v);
    ArrayIterator operator+(difference_type v);
    ArrayIterator operator-(difference_type v);

    friend ArrayIterator operator+(difference_type, const ArrayIterator& it);
};

ArrayIterator operator+(ArrayIterator::difference_type, const ArrayIterator& it);

typedef float (*FloatValueGenerator)(size_t n);

class Array {
    _symbol* name_;
    _garray* array_;
    size_t size_;
    word* data_;

public:
    typedef ArrayIterator iterator;
    typedef ArrayIterator const_iterator;

public:
    Array();
    Array(_symbol* name);
    Array(const char* name);

    /** iterators */
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const iterator end() const;

    /**
     * You should call this function every time before other function, to assure that array was not
     * resized or deleted.
     */
    bool update();

    /**
     * You should call this function after all array operation, to update array graphics view
     */
    void redraw();

    bool isValid() const;
    bool open(_symbol* name);
    bool open(const char* name);

    /**
     * @brief returns number of samples in array
     */
    size_t size() const { return size_; }
    std::string name() const;

    /**
     * @brief at - get reference to sample with at position
     * @param n - sample position
     * @return reference to sample
     * @throw Exception if invalid array or invalid sample number
     */
    const float& at(size_t n) const;
    float& at(size_t n);

    /**
     * @brief get reference to sample with at position
     * @param n - sample position
     * @return reference to sample
     * @note no checks are performed
     */
    const float& operator[](size_t n) const;
    float& operator[](size_t n);

    bool resize(size_t n);

    void copyFrom(const float* src, size_t n);
    void copyTo(float* dest, size_t n);
    void fillWith(float v);
    void fillWith(FloatValueGenerator gen);

public:
    struct Exception : public std::runtime_error {
        Exception(const char* what);
    };
};
}

#endif // CEAMMC_ARRAY_H
