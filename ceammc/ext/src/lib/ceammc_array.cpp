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
#include "ceammc_array.h"
#include "m_pd.h"

#include <algorithm>

using namespace ceammc;

Array::Array()
    : name_(0)
    , array_(0)
    , size_(0)
    , data_(0)
{
}

Array::Array(t_symbol* name)
    : name_(0)
    , array_(0)
    , size_(0)
    , data_(0)
{
    open(name);
}

Array::Array(const char* name)
    : name_(0)
    , array_(0)
    , size_(0)
    , data_(0)
{
    open(name);
}

Array::iterator Array::begin()
{
    return ArrayIterator(data_);
}

Array::const_iterator Array::begin() const
{
    return ArrayIterator(data_);
}

Array::iterator Array::end()
{
    return ArrayIterator(data_ + size_);
}

const Array::iterator Array::end() const
{
    return ArrayIterator(data_ + size_);
}

bool Array::update()
{
    if (!name_)
        return false;

    return open(name_);
}

void Array::redraw()
{
    if (!isValid())
        return;

    garray_redraw(array_);
}

bool Array::isValid() const
{
    return array_ != 0;
}

bool Array::open(_symbol* name)
{
    array_ = reinterpret_cast<t_garray*>(pd_findbyclass(name, garray_class));
    if (!array_) {
        size_ = 0;
        data_ = 0;
        array_ = 0;
        name_ = 0;
        return false;
    }

    int vecsize = 0;
    t_word* vec = 0;
    if (!garray_getfloatwords(array_, &vecsize, &vec)) {
        size_ = 0;
        data_ = 0;
        array_ = 0;
        name_ = 0;
        return false;
    }

    size_ = static_cast<size_t>(vecsize);
    data_ = vec;
    name_ = name;
    return true;
}

bool Array::open(const char* name)
{
    return open(gensym(name));
}

std::string Array::name() const
{
    return name_ == 0 ? std::string() : std::string(name_->s_name);
}

const float& Array::at(size_t n) const
{
    if (array_ == 0)
        throw Exception("invalid array");

    if (n >= size_)
        throw Exception("invalid index");

    return data_[n].w_float;
}

float& Array::at(size_t n)
{
    if (array_ == 0)
        throw Exception("invalid array");

    if (n >= size_)
        throw Exception("invalid index");

    return data_[n].w_float;
}

const float& Array::operator[](size_t n) const
{
    return data_[n].w_float;
}

float& Array::operator[](size_t n)
{
    return data_[n].w_float;
}

bool Array::resize(size_t n)
{
    if (!isValid())
        return false;

    garray_resize_long(array_, n);
    return update();
}

void Array::copyFrom(const float* src, size_t n)
{
    std::copy(src, src + std::min(n, size_), begin());
}

void Array::copyTo(float* dest, size_t n)
{
    std::copy(begin(), begin() + n, dest);
}

void Array::fillWith(float v)
{
    std::fill(begin(), end(), v);
}

struct SampleGenerator {
    size_t n;
    FloatValueGenerator g;
    SampleGenerator(FloatValueGenerator gen)
        : n(0)
        , g(gen)
    {
    }

    float operator()() { return g(n++); }
};

void Array::fillWith(FloatValueGenerator gen)
{
    SampleGenerator g(gen);
    std::generate(begin(), end(), g);
}

Array::Exception::Exception(const char* what)
    : std::runtime_error(what)
{
}

ArrayIterator::ArrayIterator(word* ptr)
    : data_(ptr)
{
}

ArrayIterator::ArrayIterator(const ArrayIterator& i)
    : data_(i.data_)
{
}

float& ArrayIterator::operator*()
{
    return data_->w_float;
}

const float& ArrayIterator::operator*() const
{
    return data_->w_float;
}

float& ArrayIterator::operator[](const size_t n)
{
    return data_[n].w_float;
}

ceammc::ArrayIterator& ceammc::ArrayIterator::operator++()
{
    ++data_;
    return *this;
}

ArrayIterator& ArrayIterator::operator--()
{
    --data_;
    return *this;
}

ArrayIterator ArrayIterator::operator++(int)
{
    ArrayIterator tmp(*this);
    ++data_;
    return tmp;
}

ArrayIterator ArrayIterator::operator--(int)
{
    ArrayIterator tmp(*this);
    --data_;
    return tmp;
}

ArrayIterator& ArrayIterator::operator+=(difference_type v)
{
    data_ += v;
    return *this;
}

ArrayIterator& ArrayIterator::operator-=(difference_type v)
{
    data_ -= v;
    return *this;
}

ArrayIterator ArrayIterator::operator+(difference_type v)
{
    return ArrayIterator(data_ + v);
}

ArrayIterator ArrayIterator::operator-(difference_type v)
{
    return ArrayIterator(data_ - v);
}

ArrayIterator::difference_type ArrayIterator::operator-(const ArrayIterator& it) const
{
    return data_ - it.data_;
}

namespace ceammc {
ArrayIterator operator+(ArrayIterator::difference_type v, const ArrayIterator& it)
{
    return ArrayIterator(v + it.data_);
}
}
