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
#include "ceammc_containers.h"
#include "ceammc_pd.h"
#include "m_pd.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

#include <algorithm>
#include <cmath>
#include <cstring>

using namespace ceammc;

Array::Array()
    : name_(&s_)
    , array_(nullptr)
    , size_(0)
    , data_(nullptr)
{
}

Array::Array(Array&& a)
    : name_(a.name_)
    , array_(a.array_)
    , size_(a.size_)
    , data_(a.data_)
{
    a.name_ = &s_;
    a.array_ = nullptr;
    a.size_ = 0;
    a.data_ = nullptr;
}

Array::Array(t_symbol* name)
    : name_(&s_)
    , array_(nullptr)
    , size_(0)
    , data_(nullptr)
{
    open(name);
}

Array::Array(const char* name)
    : name_(&s_)
    , array_(nullptr)
    , size_(0)
    , data_(nullptr)
{
    open(name);
}

Array::Array(const char* name, std::initializer_list<t_sample> l)
    : name_(&s_)
    , array_(nullptr)
    , size_(0)
    , data_(nullptr)
{
    if (open(name)) {
        if (resize(l.size()))
            std::copy(l.begin(), l.end(), begin());
    }
}

Array::Array(const Array& array)
    : name_(array.name_)
    , array_(array.array_)
    , size_(array.size_)
    , data_(array.data_)
{
}

Array& Array::operator=(const Array& array)
{
    name_ = array.name_;
    array_ = array.array_;
    size_ = array.size_;
    data_ = array.data_;
    return *this;
}

Array& Array::operator=(Array&& array)
{
    name_ = array.name_;
    array_ = array.array_;
    size_ = array.size_;
    data_ = array.data_;

    array.name_ = &s_;
    array.size_ = 0;
    array.array_ = nullptr;
    array.data_ = nullptr;
    return *this;
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
    if (name_ == &s_)
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

bool Array::open(t_symbol* name)
{
    array_ = reinterpret_cast<t_garray*>(pd_findbyclass(name, garray_class));
    if (!array_) {
        size_ = 0;
        data_ = nullptr;
        array_ = nullptr;
        name_ = &s_;
        return false;
    }

    int vecsize = 0;
    t_word* vec = nullptr;
    if (!garray_getfloatwords(array_, &vecsize, &vec)) {
        size_ = 0;
        data_ = nullptr;
        array_ = nullptr;
        name_ = &s_;
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

void Array::useInDSP()
{
    if (array_)
        garray_usedindsp(array_);
}

const t_float& Array::at(size_t n) const
{
    if (!array_ || !data_)
        throw Exception("invalid array");

    if (n >= size_)
        throw Exception("invalid index");

    return data_[n].w_float;
}

t_float& Array::at(size_t n)
{
    if (!array_ || !data_)
        throw Exception("invalid array");

    if (n >= size_)
        throw Exception("invalid index");

    return data_[n].w_float;
}

t_float Array::ringPushBack(t_float f)
{
    if (!array_ || !data_ || size_ < 2)
        throw Exception("invalid array");

    const auto prev = data_[0].w_float;
    std::memmove(data_, data_ + 1, sizeof(data_[0]) * (size_ - 1));
    data_[size_ - 1].w_float = f;
    return prev;
}

t_float Array::ringPushFront(t_float f)
{
    if (!array_ || !data_ || size_ < 2)
        throw Exception("invalid array");

    const auto prev = data_[size_ - 1].w_float;
    std::memmove(data_ + 1, data_, sizeof(data_[0]) * (size_ - 1));
    data_[0].w_float = f;
    return prev;
}

bool Array::resize(size_t n)
{
    if (!array_)
        return false;

    garray_resize_long(array_, static_cast<long>(n));
    return update();
}

void Array::copyFrom(const t_float* src, size_t n)
{
    std::copy(src, src + std::min(n, size_), begin());
}

void Array::copyTo(t_float* dest, size_t n)
{
    std::copy(begin(), begin() + long(n), dest);
}

void Array::fillWith(t_float v)
{
    std::fill(begin(), end(), v);
}

void Array::fillWith(FloatValueGenerator gen)
{
    size_t n = 0;
    std::generate(begin(), end(), [&n, gen]() { return gen(n++); });
}

bool Array::set(const AtomListView& lv)
{
    if (!resize(lv.size()))
        return false;

    const size_t N = std::min(size(), lv.size());
    for (size_t i = 0; i < N; i++)
        data_[i].w_float = lv[i].asFloat();

    return true;
}

bool Array::set(std::initializer_list<t_sample> l)
{
    if (!resize(l.size()))
        return false;

    std::copy(l.begin(), l.end(), begin());
    return true;
}

bool Array::setYBounds(t_float yBottom, t_float yTop)
{
    if (!array_ || !name_->s_thing)
        return false;

    AtomArray<4> args(0.0, yTop, t_float(size_), yBottom);
    return pd::send_message(name_, gensym("bounds"), args.view());
}

bool Array::setYTicks(t_float y, t_float step, size_t bigN)
{
    if (!array_ || !name_->s_thing)
        return false;

    AtomArray<3> args(y, step, bigN);
    return pd::send_message(name_, gensym("yticks"), args.view());
}

bool Array::setYLabels(const AtomListView& labels)
{
    if (!array_ || !name_->s_thing)
        return false;

    if (!(*name_->s_thing)->c_gobj)
        return false;

    auto gl = garray_getglist(array_);
    t_float el_wd = 1;
    if (gl && gl->gl_pixwidth > 0)
        el_wd = t_float(size()) / gl->gl_pixwidth;

    SmallAtomList args(std::round(-4 * el_wd));
    args.insert_back(labels.begin(), labels.end());
    return pd::send_message(name_, gensym("ylabel"), args.view());
}

bool Array::setSaveInPatch(bool value)
{
    if (array_) {
        garray_setsaveit(array_, value ? 1 : 0);
        return true;
    } else
        return false;
}

bool Array::normalize(t_float f)
{
    if (!array_ || !name_->s_thing)
        return false;

    return pd::send_message(name_, gensym("normalize"), Atom(f));
}

Array::Exception::Exception(const char* what)
    : std::runtime_error(what)
{
}

ArrayIterator::ArrayIterator()
    : data_(nullptr)
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

ArrayIterator& ArrayIterator::operator=(const ArrayIterator& i)
{
    data_ = i.data_;
    return *this;
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
