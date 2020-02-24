/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "datatype_mlist.h"
#include "ceammc_datatypes.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "mlist_parser_impl.h"

#include <cassert>
#include <cmath>
#include <cstring>

DataType DataTypeMList::dataType = data::DATA_MLIST;

DataTypeMList::DataTypeMList()
{
}

DataTypeMList::DataTypeMList(const std::string& str)
{
    auto l = parse(str);
    if (l)
        data_ = l->data_;
}

DataTypeMList::DataTypeMList(const AtomList& lst)
    : data_(lst)
{
}

DataTypeMList::DataTypeMList(AtomList&& lst)
    : data_(lst)
{
}

DataTypeMList::DataTypeMList(const DataTypeMList& mlist)
    : data_(mlist.data_)
{
}

DataTypeMList::DataTypeMList(DataTypeMList&& mlist)
    : data_(std::move(mlist.data_))
{
}

DataTypeMList& DataTypeMList::operator=(const DataTypeMList& mlist)
{
    if (this == &mlist)
        return *this;

    data_ = mlist.data_;
    return *this;
}

DataTypeMList& DataTypeMList::operator=(DataTypeMList&& mlist)
{
    if (this == &mlist)
        return *this;

    data_ = std::move(mlist.data_);
    return *this;
}

DataType DataTypeMList::type() const
{
    return data::DATA_MLIST;
}

DataTypeMList* DataTypeMList::clone() const
{
    return new DataTypeMList(*this);
}

bool DataTypeMList::isEqual(const AbstractData* cmp) const
{
    auto mlist = cmp->as<DataTypeMList>();
    if (!mlist)
        return false;

    return mlist->data_ == data_;
}

void DataTypeMList::dump()
{
    LIB_DBG << toString();
}

std::string DataTypeMList::toString() const
{
    std::string res("(");

    for (size_t i = 0; i < data_.size(); i++) {
        if (i != 0)
            res.push_back(' ');

        res += to_string_quoted(data_[i].asAtom());
    }

    res.push_back(')');
    return res;
}

AtomList DataTypeMList::toList() const
{
    return data_.toList();
}

bool DataTypeMList::empty() const
{
    return data_.empty();
}

size_t DataTypeMList::size() const
{
    return data_.size();
}

const DataAtom& DataTypeMList::at(size_t n) const
{
    return data_[n];
}

DataAtom& DataTypeMList::at(size_t n)
{
    return data_[n];
}

const DataAtom& DataTypeMList::operator[](size_t n) const
{
    return data_[n];
}

DataAtom& DataTypeMList::operator[](size_t n)
{
    return data_[n];
}

void DataTypeMList::append(const Atom& a)
{
    data_.append(a);
}

void DataTypeMList::append(const DataAtom& a)
{
    data_.append(a);
}

void DataTypeMList::append(const AtomList& lst)
{
    data_.append(lst);
}

void DataTypeMList::append(const DataAtomList& lst)
{
    data_.append(lst);
}

void DataTypeMList::clear()
{
    data_.clear();
}

bool DataTypeMList::insert(size_t idx, const AtomList& lst)
{
    return data_.insert(idx, lst);
}

bool DataTypeMList::insert(size_t idx, const DataAtomList& lst)
{
    return data_.insert(idx, lst);
}

void DataTypeMList::prepend(const Atom& a)
{
    data_.prepend(a);
}

void DataTypeMList::prepend(const DataAtom& a)
{
    data_.prepend(a);
}

void DataTypeMList::prepend(const AtomList& lst)
{
    data_.prepend(lst);
}

void DataTypeMList::prepend(const DataAtomList& lst)
{
    data_.prepend(lst);
}

bool DataTypeMList::pop()
{
    return data_.pop();
}

bool DataTypeMList::remove(size_t idx)
{
    return data_.remove(idx);
}

void DataTypeMList::reserve(size_t n)
{
    data_.reserve(n);
}

void DataTypeMList::set(const AtomList& lst)
{
    data_.set(lst);
}

DataTypeMList DataTypeMList::rotateLeft(int steps) const
{
    if (size() < 2 || steps == 0)
        return *this;

    int sz = int(size());
    steps = steps % sz;
    if (steps < 0)
        steps += sz;

    DataTypeMList res(*this);
    std::rotate(res.begin(), res.begin() + steps, res.end());
    return res;
}

DataTypeMList DataTypeMList::flatten() const
{
    DataTypeMList res;
    res.data_.reserve(size());

    for (auto& el : data_) {
        if (el.isAtom()) {
            res.data_.append(el.asAtom());
        } else {
            auto data = el.data();
            // skipping null pointer
            if (data.isNull()) {
                LIB_ERR << "invalid data pointer";
                continue;
            }

            if (data->type() == DataTypeMList::dataType) {
                auto mlist = data->as<DataTypeMList>();
                if (!mlist) {
                    LIB_ERR << "invalid mlist pointer";
                    continue;
                }

                // recursion
                auto mlist_flatten = mlist->flatten();
                res.data_.append(mlist_flatten.data_);
            } else
                res.data_.append(el);
        }
    }

    return res;
}

template <class T>
T sign(T v)
{
    return v < 0 ? -1 : 1;
}

static size_t normalizeIdx(int idx, size_t N, bool clip)
{
    const bool is_negative = idx < 0;
    int abs_idx = idx;

    if (is_negative)
        abs_idx = -(idx + 1);

    if (clip)
        abs_idx = std::min<int>(abs_idx, N - 1);

    return is_negative ? (N - 1 - abs_idx) : abs_idx;
}

DataTypeMList DataTypeMList::slice(int start, int end, size_t step) const
{
    DataTypeMList res;

    if (step < 1 || data_.empty())
        return res;

    const size_t N = data_.size();
    size_t nfirst = normalizeIdx(start, N, false);
    if (nfirst >= N)
        return res;

    size_t last = normalizeIdx(end, N, true);

    if (nfirst <= last) {
        for (size_t i = nfirst; i <= last; i += step)
            res.append(data_[i]);
    } else {
        for (long i = nfirst; i >= long(last); i -= step)
            res.append(data_[i]);
    }

    return res;
}

void DataTypeMList::sort()
{
    auto pred = [](const DataAtom& a0, const DataAtom& a1) {
        if (a0.isAtom() && a1.isAtom())
            return a0.asAtom() < a1.asAtom();

        return false;
    };

    std::sort(begin(), end(), pred);
}

void DataTypeMList::reverse()
{
    std::reverse(data_.begin(), data_.end());
}

void DataTypeMList::shuffle()
{
    std::random_shuffle(data_.begin(), data_.end());
}

bool DataTypeMList::contains(const Atom& a) const
{
    return data_.contains(a);
}

bool DataTypeMList::contains(const AtomList& l) const
{
    return data_.contains(l);
}

bool DataTypeMList::contains(const DataAtom& d) const
{
    return data_.contains(d);
}

bool DataTypeMList::contains(const DataPtr& ptr) const
{
    return data_.contains(ptr);
}

bool DataTypeMList::contains(const DataTypeMList& l) const
{
    auto it = std::search(data_.begin(), data_.end(), l.begin(), l.end());
    return it != data_.end();
}

DataTypeMList::MaybeList DataTypeMList::parse(const AtomList& lst)
{
    if (lst.anyOf(isData)) {
        LIB_ERR << "only core atom types allowed for parsing....";
        return DataTypeMList();
    }

    return parse(to_string(lst, " "));
}

DataTypeMList::MaybeList DataTypeMList::parse(const std::string& str)
{
    DataTypeMList res;

    mlist_ast* ast = mlist_ast_create();
    int err = mlist_parse_string(ast, str.c_str());

    if (err) {
        mlist_ast_free(ast);
        LIB_ERR << "[mlist] parse error: " << str;
        return res;
    }

    std::vector<DataTypeMList*> stack;
    mlist_ast_traverse(ast, &res, &stack, (traverse_fn)traverse);

    mlist_ast_free(ast);

    if (res.data_.size() == 0)
        return res;

    return *res.data_[0].data()->as<DataTypeMList>();
}

void DataTypeMList::traverse(mlist_node* node, DataTypeMList* data, MListStack* stack, int act, const char* txt)
{
    switch (act) {
    case TRAVERSE_PUSH: {
        auto mlist = new DataTypeMList;
        auto lst_ptr = DataPtr(mlist);

        // insert stack top
        if (stack->empty()) {
            data->data_.append(lst_ptr);
            stack->push_back(mlist);
        } else {
            stack->back()->data_.append(lst_ptr);
            stack->push_back(mlist);
        }

        break;
    }
    case TRAVERSE_ADD: {
        assert(!stack->empty());

        if (mlist_node_is_quoted(node)) {
            stack->back()->data_.append(Atom(gensym(txt)));
        } else {
            auto b = binbuf_new();
            binbuf_text(b, (char*)txt, strlen(txt));
            const int N = binbuf_getnatom(b);
            auto a = binbuf_getvec(b);

            if (N > 0)
                stack->back()->data_.append(Atom(*a));
            else
                std::cerr << "invalid atom: " << txt;

            binbuf_free(b);
        }

        break;
    }
    case TRAVERSE_POP: {
        assert(!stack->empty());
        stack->pop_back();
        break;
    }
    }
}
