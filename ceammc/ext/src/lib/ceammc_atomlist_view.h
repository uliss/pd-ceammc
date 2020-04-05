/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_ATOMLIST_VIEW_H
#define CEAMMC_ATOMLIST_VIEW_H

#include "ceammc_atom.h"

#include <algorithm>
#include <functional>

namespace ceammc {

class AtomList;

class AtomListView {
    const Atom* data_;
    size_t n_;

public:
    AtomListView();
    AtomListView(const t_atom* a, size_t n);
    AtomListView(const AtomList& l);

    void set(t_atom* a, size_t n);
    void set(const AtomList& l);
    bool get(t_atom*& a, int& n);

    operator AtomList() const;
    bool operator==(const AtomListView& v) const;
    bool operator!=(const AtomListView& v) const { return !operator==(v); }
    bool operator==(const AtomList& l) const;
    bool operator!=(const AtomList& l) const { return !operator==(l); }

    bool operator==(bool v) const { return isBool() && v == asBool(); }
    bool operator==(t_float v) const;
    bool operator==(int v) const { return isInt() && asInt() == v; }
    bool operator==(t_symbol* s) const { return isSymbol() && asSymbol() == s; }
    bool operator==(const Atom& a) const { return isAtom() && asAtom() == a; }

    // containter checks
    inline bool empty() const { return n_ == 0; }
    inline size_t size() const { return n_; }
    inline bool isNull() const { return data_ == nullptr; }

    // type checks
    bool isBool() const;
    bool isFloat() const { return data_ && n_ == 1 && atom().a_type == A_FLOAT; }
    bool isInt() const;
    bool isSymbol() const { return data_ && n_ == 1 && atom().a_type == A_SYMBOL; }
    bool isAtom() const { return data_ && n_ == 1; }
    bool isData() const { return data_ && n_ == 1 && data_->isData(); }
    bool isDataType(int t) const { return data_ && n_ == 1 && data_->isDataType(t); }
    template <typename T>
    bool isA() const;

    // iterators
    const Atom* begin() const { return data_; }
    const Atom* end() const { return &data_[n_]; }

    // no range checks!!!
    const Atom& front() const { return *data_; }
    const Atom& back() const { return data_[n_]; }
    const Atom& at(size_t pos) const { return data_[pos]; }
    const Atom& operator[](size_t pos) const { return data_[pos]; }
    const Atom& relativeAt(long pos) const;

    // no type checks!!!
    inline bool asBool() const { return front().asBool(); }
    inline t_symbol* asSymbol() const { return atom().a_w.w_symbol; }
    inline t_float asFloat() const { return atom().a_w.w_float; }
    inline int asInt() const { return static_cast<int>(atom().a_w.w_float); }
    inline const Atom& asAtom() const { return front(); }

    // with range checks
    bool boolAt(size_t pos, bool def) const;
    t_float floatAt(size_t pos, t_float def) const;
    int intAt(size_t pos, int def) const;
    t_symbol* symbolAt(size_t pos, t_symbol* def) const;

    /**
     * Returns subview from specified position
     */
    AtomListView subView(size_t from) const;

    /**
     * Returns subview from specified position and length
     */
    AtomListView subView(size_t from, size_t len) const;

    bool contains(const Atom& a) const;

    friend class AtomList;

private:
    inline const t_atom& atom() const { return data_->atom(); }
};

template <>
inline bool AtomListView::isA<bool>() const { return isBool(); }
template <>
inline bool AtomListView::isA<t_float>() const { return isFloat(); }
template <>
inline bool AtomListView::isA<int>() const { return isInt(); }
template <>
inline bool AtomListView::isA<t_symbol*>() const { return isSymbol(); }
template <>
inline bool AtomListView::isA<Atom>() const { return isAtom(); }
template <>
inline bool AtomListView::isA<AtomList>() const { return true; }
}

#endif // CEAMMC_ATOMLIST_VIEW_H
