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
#include <initializer_list>

#include <boost/container/small_vector.hpp>
#include <boost/optional.hpp>

namespace ceammc {

using MaybeFloat = boost::optional<t_float>;

class AtomList;

enum class AtomListMapType {
    FILTER,
    KEEP
};

class AtomListView {
    const Atom* data_;
    size_t n_;

public:
    using iterator = const Atom*;
    using const_iterator = const Atom*;

public:
    AtomListView() noexcept;
    AtomListView(const t_atom* a, size_t n) noexcept;
    AtomListView(const Atom* a, size_t n) noexcept;
    AtomListView(const Atom& a) noexcept;
    AtomListView(const AtomList& l) noexcept;

    void set(t_atom* a, size_t n);
    void set(const Atom& a);
    void set(const AtomList& l);
    bool get(t_atom*& a, int& n);

    bool operator==(const AtomListView& v) const;
    bool operator!=(const AtomListView& v) const { return !operator==(v); }
    bool operator==(const AtomList& l) const;
    bool operator!=(const AtomList& l) const { return !operator==(l); }

    bool operator==(bool v) const { return isBool() && v == asBool(); }
    bool operator!=(bool v) const { return !operator==(v); }
    bool operator==(t_float v) const;
    bool operator!=(t_float v) const { return !operator==(v); }
    bool operator==(int v) const { return isInteger() && asInt() == v; }
    bool operator!=(int v) const { return !operator==(v); }
    bool operator==(t_symbol* s) const { return isSymbol() && asSymbol() == s; }
    bool operator!=(t_symbol* s) const { return !operator==(s); }
    bool operator==(const Atom& a) const { return isAtom() && front() == a; }
    bool operator!=(const Atom& a) const { return !operator==(a); }

    // containter checks
    inline bool empty() const { return n_ == 0; }
    inline size_t size() const { return n_; }
    inline bool isNull() const { return data_ == nullptr; }

    // type checks
    bool isBool() const;
    bool isFloat() const { return data_ && n_ == 1 && atom().a_type == A_FLOAT; }
    bool isInteger() const;
    bool isSymbol() const { return data_ && n_ == 1 && atom().a_type == A_SYMBOL; }
    bool isAtom() const { return data_ && n_ == 1; }
    bool isData() const { return data_ && n_ == 1 && data_->isData(); }
    bool isDataType(int t) const { return data_ && n_ == 1 && data_->isDataType(t); }
    template <typename T>
    bool isA() const { return n_ == 1 && data_ && data_->isA<T>(); }

    /// value access
    /**
     * @brief returns pointer to element at specified relative position
     * @param pos - positive value means position from the begining, negative position - from end.
     * @example pos == -1 means last element (if exists)
     * @return pointer to element, or NULL if no element at given position
     */
    const Atom* relativeAt(int pos) const;

    /**
     * Same as at(), but values for index greater than the size of the
     * List will be clipped to the last index.
     * @param pos - position index
     * @return pointer to element, or NULL if empty
     */
    const Atom* clipAt(int pos) const;

    /**
     * Same as at(), but values for index greater than the size of the
     * List will be wrapped around to 0.
     * @param pos - position index
     * @return pointer to element, or NULL if empty
     */
    const Atom* wrapAt(int pos) const;

    /**
     * Same as at(), but values for index greater than the size of the
     * List will be folded back.
     * @param pos - position index
     * @return pointer to element, or NULL if empty
     */
    const Atom* foldAt(int pos) const;

    /**
     * Convert atomlist to parametrised type
     * @throw std::logic_error on error
     * @see Atom::asT
     */
    template <typename T>
    T asT() const
    {
        if (n_ == 1 && data_)
            return data_->asT<T>();
        else
            throw std::logic_error("not a single atom list view");
    }

    /**
     * Returns pointer to data
     * @warning no type checks are done
     */
    template <typename T>
    const T* asD() const
    {
        if (n_ == 1 && data_)
            return data_->asD<T>();
        else
            return nullptr;
    }

    /**
     * Get list value
     * @return def value if list contains other type then specified
     */
    template <typename T>
    T toT(T def) const
    {
        if (isA<T>())
            return asT<T>();
        else
            return def;
    }

    // iterators
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + n_; }

    // no range checks!!!
    /** returns first atom in view, no range check */
    const Atom& front() const { return *data_; }
    /** returns last atom in view, no range check */
    const Atom& back() const { return data_[n_ - 1]; }
    /** returns atom at specified position in view, no range check */
    const Atom& at(size_t pos) const { return data_[pos]; }
    /** returns atom at specified position in view, no range check */
    const Atom& operator[](size_t pos) const { return data_[pos]; }
    const Atom& relativeAt(long pos) const;

    inline bool asBool(bool def = false) const { return empty() ? def : front().asBool(def); }
    inline t_symbol* asSymbol(t_symbol* def = &s_) const { return empty() ? def : front().asSymbol(def); }
    inline t_float asFloat(t_float def = 0) const { return empty() ? def : front().asFloat(def); }
    inline t_int asInt(t_int def = 0) const { return empty() ? def : static_cast<t_int>(front().asFloat(def)); }

    // with range checks
    /**
     * Returns boolean value at specified position or default if not found
     * @param pos - position
     * @param def - default value if not found
     * @return true or false
     */
    bool boolAt(size_t pos, bool def) const;

    /**
     * Returns float value at specified position or default if not found
     * @param pos - position
     * @param def - default value if not found
     * @return float value
     */
    t_float floatAt(size_t pos, t_float def) const;

    /**
     * Returns int value at specified position or default if not found
     * @param pos - position
     * @param def - default value if not found
     * @return int value
     */
    int intAt(size_t pos, int def) const;

    /**
     * Returns symbol value at specified position or default if not found
     * @param pos - position
     * @param def - default value if not found
     * @return symbol value
     */
    t_symbol* symbolAt(size_t pos, t_symbol* def) const;

    /**
     * Returns float at specified position that is greater
     *   then specified value or default if index out of range or value not satisfied
     * @param pos - position
     * @param min - lower bound
     * @param def - default value if not found or not satisfied
     * @return float value
     */
    t_float floatGreaterThenAt(size_t pos, t_float min, t_float def) const;

    /**
     * Returns float at specified position that is greater
     *   or equal to specified value or default if index out of range or value not satisfied
     * @param pos - position
     * @param min - lower bound
     * @param def - default value if not found or not satisfied
     * @return float value
     */
    t_float floatGreaterEqualAt(size_t pos, t_float min, t_float def) const;

    /**
     * Returns float at specified position that is less
     *   then specified value or default if index out of range or value not satisfied
     * @param pos - position
     * @param max - upper bound
     * @param def - default value if not found or not satisfied
     * @return float value
     */
    t_float floatLessThenAt(size_t pos, t_float max, t_float def) const;

    /**
     * Returns float at specified position that is less
     *   or equal to specified value or default if index out of range or value not satisfied
     * @param pos - position
     * @param max - upper bound
     * @param def - default value if not found or not satisfied
     * @return int value
     */
    t_float floatLessEqualAt(size_t pos, t_float max, t_float def) const;

    /**
     * Returns int at specified position that is greater
     *   then specified value or default if index out of range or value not satisfied
     * @param pos - position
     * @param min - lower bound
     * @param def - default value if not found or not satisfied
     * @return int value
     */
    t_int intGreaterThenAt(size_t pos, t_int min, t_int def) const;

    /**
     * Returns int at specified position that is greater
     *   or equal to specified value or default if index out of range or value not satisfied
     * @param pos - position
     * @param min - lower bound
     * @param def - default value if not found or not satisfied
     * @return int value
     */
    t_int intGreaterEqualAt(size_t pos, t_int min, t_int def) const;

    /**
     * Returns int at specified position that is less
     *   then specified value or default if index out of range or value not satisfied
     * @param pos - position
     * @param max - upper bound
     * @param def - default value if not found or not satisfied
     * @return int value
     */
    t_int intLessThenAt(size_t pos, t_int max, t_int def) const;

    /**
     * Returns int at specified position that is less
     *   or equal to specified value or default if index out of range or value not satisfied
     * @param pos - position
     * @param max - upper bound
     * @param def - default value if not found or not satisfied
     * @return int value
     */
    t_int intLessEqualAt(size_t pos, t_int max, t_int def) const;

    /**
     * Returns int at specified position that is lin specified closed range
     * or default if index out of range or value not satisfied
     * @param pos - position
     * @param min - lower bound
     * @param max - upper bound
     * @param def - default value if not found or not satisfied
     * @return int value
     */
    t_int intInClosedIntervalAt(size_t pos, t_int min, t_int max, t_int def) const;

    /**
     * Returns subview from specified position
     */
    AtomListView subView(size_t from) const;

    /**
     * Returns subview from specified position and length
     */
    AtomListView subView(size_t from, size_t len) const;

    /**
     * Returns true if contains specified atom, otherwise false
     * @param a - atom value
     */
    bool contains(const Atom& a) const;

    /**
     * Check if all atoms satisfies to predicate
     * @param pred - predicate function
     * @return true if for all atoms predicate returns true
     */
    bool allOf(std::function<bool(const Atom&)> pred) const;

    /**
     * Check if at least one atom satisfies to predicate
     * @param pred - predicate function
     * @return true if for at least one atom predicate returns true
     */
    bool anyOf(std::function<bool(const Atom&)> pred) const;

    /**
     * Check if no one atom satisfies to predicate
     * @param pred - predicate function
     * @return true if for all atoms predicate returns false
     */
    bool noneOf(std::function<bool(const Atom&)> pred) const;

    friend class AtomList;

    /** pointer to pd raw t_atoms */
    t_atom* toPdData() const { return reinterpret_cast<t_atom*>(const_cast<Atom*>(data_)); }

    /**
     * Returns sum of floats in list or boost::none if empty
     */
    MaybeFloat sum() const noexcept;

    /**
     * Returns product of floats in list or boost::none if empty
     */
    MaybeFloat product() const noexcept;

    /**
     * Reduce to t_float
     * @param init - init value
     * @param fn - reduce function
     * @return - result, or boost::none if no floats in list
     */
    MaybeFloat reduceFloat(t_float init, std::function<t_float(t_float, t_float)> fn) const;

    /**
     * Get min and max elements from list
     * @param min - min destination
     * @param max - max destination
     * @return false if empty
     */
    bool range(Atom& min, Atom& max) const noexcept;

    /**
     * Map atom values
     * @param fn - atom map function
     * @param out - map output
     */
    template <typename T>
    void map(const AtomMapFunction& fn, T& out) const
    {
        if (!fn)
            return;

        out.reserve(n_);

        for (auto& a : *this)
            out.push_back(fn(a));
    }

    /**
     * Map float values
     * @param fn - function
     * @param out - map output
     * @param t - AtomListMapType::KEEP - non-float values are left untouched,
     *            if AtomListMapType::FILTER - non-float values are removed
     */
    template <typename T>
    void mapFloat(const FloatMapFunction& fn, T& out, AtomListMapType t = AtomListMapType::KEEP) const
    {
        if (!fn)
            return;

        out.reserve(n_);
        for (auto& a : *this) {
            if (a.isFloat())
                out.push_back(fn(a.asT<t_float>()));
            else if (t == AtomListMapType::KEEP)
                out.push_back(a);
        }
    }

    /**
     * Map symbol values
     * @param fn - function
     * @param out - map output
     * @param t - AtomListMapType::KEEP - non-symbol values are left untouched,
     *            if AtomListMapType::FILTER - non-symbol values are removed
     */
    template <typename T>
    void mapSymbol(const SymbolMapFunction& fn, T& out, AtomListMapType t = AtomListMapType::KEEP) const
    {
        if (!fn)
            return;

        out.reserve(n_);
        for (auto& a : *this) {
            if (a.isSymbol())
                out.push_back(fn(a.asT<t_symbol*>()));
            else if (t == AtomListMapType::KEEP)
                out.push_back(a);
        }
    }

    /**
     * Select values for which atom predicate returns true
     */
    template <typename T>
    void filter(const AtomPredicate& fn, T& out) const
    {
        out.reserve(n_);
        for (auto& a : *this) {
            if (!fn || fn(a))
                out.push_back(a);
        }
    }

    /**
     * Select values for which float predicate returns true
     */
    template <typename T>
    void filterFloat(const FloatPredicate& fn, T& out) const
    {
        out.reserve(n_);
        for (auto& a : *this) {
            if (!fn || (a.isFloat() && fn(a.asT<t_float>())))
                out.push_back(a);
        }
    }

    /**
     * Select values for which symbol predicate returns true
     */
    template <typename T>
    void filterSymbol(const SymbolPredicate& fn, T& out) const
    {
        out.reserve(n_);
        for (auto& a : *this) {
            if (!fn || (a.isSymbol() && fn(a.asT<t_symbol*>())))
                out.push_back(a);
        }
    }

    bool getProperty(t_symbol* name, AtomListView& res) const;

private:
    inline const t_atom& atom() const { return data_->atom(); }
};

std::ostream& operator<<(std::ostream& os, const AtomListView& l);

template <>
inline bool AtomListView::isA<bool>() const { return isBool(); }
template <>
inline bool AtomListView::isA<t_float>() const { return isFloat(); }
template <>
inline bool AtomListView::isA<int>() const { return isInteger(); }
template <>
inline bool AtomListView::isA<t_symbol*>() const { return isSymbol(); }
template <>
inline bool AtomListView::isA<Atom>() const { return isAtom(); }
template <>
inline bool AtomListView::isA<AtomList>() const { return true; }
}

#endif // CEAMMC_ATOMLIST_VIEW_H
