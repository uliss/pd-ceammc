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

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

#include <algorithm>
#include <cmath>

using namespace ceammc;

Array::Array()
    : name_(&s_)
    , array_(nullptr)
    , size_(0)
    , data_(nullptr)
{
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

bool Array::set(const AtomList& l)
{
    if (!resize(l.size()))
        return false;

    const size_t N = std::min(size(), l.size());
    for (size_t i = 0; i < N; i++)
        data_[i].w_float = l[i].asFloat();

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
    static t_symbol* SYM_BOUNDS = gensym("bounds");

    if (!array_ || !name_->s_thing)
        return false;

    t_atom args[4];
    SETFLOAT(&args[0], 0);
    SETFLOAT(&args[1], yTop);
    SETFLOAT(&args[2], t_float(size_));
    SETFLOAT(&args[3], yBottom);
    pd_typedmess(name_->s_thing, SYM_BOUNDS, 4, args);
    return true;
}

bool Array::setYTicks(t_float step, size_t bigN)
{
    static t_symbol* SYM_YTICKS = gensym("yticks");

    if (!array_ || !name_->s_thing)
        return false;

    t_atom args[4];
    SETFLOAT(&args[0], 0);
    SETFLOAT(&args[1], step);
    SETFLOAT(&args[2], bigN);
    pd_typedmess(name_->s_thing, SYM_YTICKS, 3, args);
    return true;
}

bool Array::setYLabels(const AtomList& labels)
{
    static t_symbol* SYM_YLABELS = gensym("ylabel");

    if (!array_ || !name_->s_thing)
        return false;

    if (!(*name_->s_thing)->c_gobj)
        return false;

    auto gl = garray_getglist(array_);
    t_float el_wd = 1;
    if (gl)
        el_wd = t_float(size()) / gl->gl_pixwidth;

    AtomList args(std::round(-4 * el_wd));
    args.append(labels);
    pd_typedmess(name_->s_thing, SYM_YLABELS, args.size(), args.toPdData());
    return true;
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
    static t_symbol* SYM_NORMALIZE = gensym("normalize");

    if (!array_ || !name_->s_thing)
        return false;

    t_atom arg;
    SETFLOAT(&arg, f);
    pd_typedmess(name_->s_thing, SYM_NORMALIZE, 1, &arg);
    return true;
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
