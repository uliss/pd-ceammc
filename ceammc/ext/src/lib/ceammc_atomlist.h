/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_ATOMLIST_H
#define CEAMMC_ATOMLIST_H

#include "ceammc_atom.h"
#include "ceammc_atomlist_view.h"

#include <boost/iterator/filter_iterator.hpp>
#include <boost/optional.hpp>
#include <deque>
#include <functional>
#include <initializer_list>
#include <string>
#include <vector>

namespace ceammc {

typedef boost::optional<t_float> MaybeFloat;

typedef std::vector<t_float> FloatList;
typedef Atom (*AtomGenerator)();

enum class AtomListMapType {
    FILTER,
    KEEP
};

class AtomList {
public:
    using Container = std::vector<Atom>;
    using const_iterator = Container::const_iterator;
    using iterator = Container::iterator;
    using reverse_iterator = Container::reverse_iterator;
    using const_reverse_iterator = Container::const_reverse_iterator;

    using atom_filter_iterator = boost::filter_iterator<AtomPredicate, iterator>;
    using const_atom_filter_iterator = boost::filter_iterator<AtomPredicate, const_iterator>;

public:
    AtomList();
    AtomList(const AtomList& l);
    AtomList(AtomList&& l) noexcept;
    AtomList(const Atom& a);
    AtomList(const Atom& a, const Atom& b);
    AtomList(size_t n, t_atom* lst);
    explicit AtomList(int n, t_atom* lst);
    AtomList(std::initializer_list<t_float> l);
    AtomList(std::initializer_list<Atom> l);

    void operator=(const AtomList& l);
    void operator=(AtomList&& l);

    /**
     * Returns number of elements in list
     */
    size_t size() const { return atoms_.size(); }

    /**
     * Reserves space to avoid extra memory reallocations
     * @param n - new size
     */
    void reserve(size_t n);

    /**
     * Checks if list is empty
     * @return true if list is empty
     */
    bool empty() const { return atoms_.empty(); }

    // iterators
    iterator begin() { return atoms_.begin(); }
    const_iterator begin() const { return atoms_.begin(); }
    iterator end() { return atoms_.end(); }
    const_iterator end() const { return atoms_.end(); }
    const_iterator cbegin() const { return atoms_.cbegin(); }
    const_iterator cend() const { return atoms_.cend(); }
    reverse_iterator rbegin() { return atoms_.rbegin(); }
    const_reverse_iterator rbegin() const { return atoms_.rbegin(); }
    reverse_iterator rend() { return atoms_.rend(); }
    const_reverse_iterator rend() const { return atoms_.rend(); }
    const_reverse_iterator crbegin() { return atoms_.crbegin(); }
    const_reverse_iterator crend() const { return atoms_.crend(); }

    const_atom_filter_iterator begin_atom_filter(AtomPredicate pred) const { return const_atom_filter_iterator(pred, begin(), end()); }
    const_atom_filter_iterator end_atom_filter() const { return const_atom_filter_iterator(nullptr, end(), end()); }

    /**
     * @brief returns reference to element at specified position
     * @param pos - position (starting from 0)
     * @return reference to element
     * @throw exception if invalid position given
     */
    Atom& at(size_t pos) { return atoms_.at(pos); }
    const Atom& at(size_t pos) const { return atoms_.at(pos); }
    Atom& operator[](size_t pos) { return atoms_.at(pos); }
    const Atom& operator[](size_t pos) const { return atoms_.at(pos); }

    /**
     * @brief returns pointer to element at specified relative position
     * @param pos - positive value means position from the begining, negative position - from end.
     * @example pos == -1 means last element (if exists)
     * @return pointer to element, or NULL if no element at given position
     */
    Atom* relativeAt(int pos);
    const Atom* relativeAt(int pos) const;

    /**
     * Same as at(), but values for index greater than the size of the
     * List will be clipped to the last index.
     * @param pos - position index
     * @return pointer to element, or NULL if empty
     */
    Atom* clipAt(int pos);
    const Atom* clipAt(int pos) const;

    /**
     * Same as at(), but values for index greater than the size of the
     * List will be wrapped around to 0.
     * @param pos - position index
     * @return pointer to element, or NULL if empty
     */
    Atom* wrapAt(int pos);
    const Atom* wrapAt(int pos) const;

    /**
     * Same as at(), but values for index greater than the size of the
     * List will be folded back.
     * @param pos - position index
     * @return pointer to element, or NULL if empty
     */
    Atom* foldAt(int pos);
    const Atom* foldAt(int pos) const;

    /**
     * Try to get int from specified list position. If no int - return default value
     */
    bool boolAt(size_t pos, bool def) const;

    /**
     * Try to get int from specified list position. If no int - return default value
     */
    int intAt(size_t pos, int def) const;

    /**
     * Try to get float from specified list position. If no float - return default value
     */
    t_float floatAt(size_t pos, t_float def) const;

    /**
     * Try to get symbol from specified list position. If no symbol - return default value
     */
    t_symbol* symbolAt(size_t pos, t_symbol* def) const;

    /**
     * Resize list. If new size is less than current, last values are dropped.
     * If new size is bigger - pad with given value
     * @param n - new size
     * @param v - pad value
     */
    void resizePad(size_t n, const Atom& v);

    /**
     * Resize list. If new size is less than current, last values are dropped.
     * If new size is bigger - pad with last value
     * @param n - new size
     * @param v - pad value
     * @note do nothing on empty list
     */
    void resizeClip(size_t n);

    /**
     * Resize list. If new size is less than current, last values are dropped.
     * If new size is bigger - wrap with list values
     * @param n - new size
     * @param v - pad value
     * @note do nothing on empty list
     */
    void resizeWrap(size_t n);

    /**
     * Resize list. If new size is less than current, last values are dropped.
     * If new size is bigger - fold with list values
     * @param n - new size
     * @param v - pad value
     * @note do nothing on empty list
     */
    void resizeFold(size_t n);

    /**
     * Get property value from list
     * @param name - property name with (starts with '@')
     * @param dest - output destination
     * @return true if property was found and it has value
     */
    bool property(t_symbol* name, Atom* dest) const;

    /**
     * Get property value from list
     * @param name - property name with (starts with '@')
     * @param dest - output destination
     * @return true if property was found and it has value
     */
    bool property(t_symbol* name, AtomList* dest) const;

    /**
     * Returns all properties and their values from list
     */
    std::deque<AtomList> properties() const;

    /**
     * Checks if property exists in list
     * @return true in property found
     */
    bool hasProperty(t_symbol* name) const;
    bool hasProperty(const char* name) const { return hasProperty(gensym(name)); }

    /**
     * New list with mapped atom values
     * @param fn - atom map function
     * @return new list
     */
    AtomList map(const AtomMapFunction& fn) const;

    /**
     * New list with mapped float values
     * @param fn - function
     * @param t - AtomListMapType::KEEP - non-float values are left untouched,
     *            if AtomListMapType::FILTER - non-float values are removed
     * @return new list
     */
    AtomList mapFloat(const FloatMapFunction& fn, AtomListMapType t = AtomListMapType::KEEP) const;

    /**
     * New list with mapped symbol values
     * @param fn - function
     * @param t - AtomListMapType::KEEP - non-symbol values are left untouched,
     *            if AtomListMapType::FILTER - non-symbol values are removed
     * @return new list
     */
    AtomList mapSymbol(const SymbolMapFunction& fn, AtomListMapType t = AtomListMapType::KEEP) const;

    /**
     * Returns new list with values for which atom predicate returns true
     */
    AtomList filtered(const AtomPredicate& fn) const;

    /**
     * Returns new list with values for which float predicate returns true
     */
    AtomList filteredFloat(const FloatPredicate& pred) const;

    /**
     * Returns new list with values for which symbol predicate returns true
     */
    AtomList filteredSymbol(const SymbolPredicate& pred) const;

    /**
     * Reduce to t_float
     * @param init - init value
     * @param fn - reduce function
     * @return - result, or boost::none if no floats in list
     */
    MaybeFloat reduceFloat(t_float init, std::function<t_float(t_float, t_float)> fn) const;

    template <typename T>
    T reduce(T init, std::function<T(const Atom&, const Atom&)> fn) const;

    /**
     * Returns sublist from specified position
     * @param start - start position (may be relative)
     * @param end - end position (may be relative)
     * @param step - slice step
     * @return new list
     */
    AtomList slice(int start, int end = -1, size_t step = 1) const;

    /**
     * Sets list content from Pd typical args
     * @param n - number of elements
     * @param lst - pointer to list
     */
    void fromPdData(size_t n, t_atom* lst);
    void fromPdData(int n, t_atom* lst);

    /**
     * Returns pointer to Pd list data
     */
    t_atom* toPdData() const { return reinterpret_cast<t_atom*>(const_cast<Atom*>(atoms_.data())); }

    /**
     * Appends atom to the end of list
     */
    void append(const Atom& a) { atoms_.push_back(a); }

    /**
     * Appends another list to the end of list
     */
    void append(const AtomList& l);
    bool insert(size_t pos, const Atom& a);
    bool insert(size_t pos, const AtomList& l);
    bool remove(size_t pos);
    void removeAll(const Atom& a);
    void removeAll(AtomPredicate pred);
    void replaceAll(const Atom& old_value, const Atom& new_value);
    void replaceAll(AtomPredicate pred, const Atom& new_value);

    /**
     * Remove all list values
     */
    void clear();

    /**
     * Fill list with specified value
     * @param a - fill value
     */
    void fill(const Atom& a);
    void fill(const Atom& a, size_t sz);

    /**
     * Returns pointer to first element or NULL if list is empty
     * @see last()
     */
    Atom* first();
    const Atom* first() const;

    /**
     * Returns pointer to last element or NULL if list is empty
     * @see first()
     */
    Atom* last();
    const Atom* last() const;

    /**
     * types
     */
    bool isBang() const { return empty(); }
    bool isBool() const { return atoms_.size() == 1 && atoms_[0].isBool(); }
    bool isFloat() const { return atoms_.size() == 1 && atoms_[0].isFloat(); }
    bool isInteger() const { return atoms_.size() == 1 && atoms_[0].isInteger(); }
    bool isSymbol() const { return atoms_.size() == 1 && atoms_[0].isSymbol(); }
    bool isProperty() const { return atoms_.size() == 1 && atoms_[0].isProperty(); }
    bool isAtom() const { return atoms_.size() == 1; }
    bool isList() const { return atoms_.size() > 1; }
    bool isData() const { return atoms_.size() == 1 && atoms_.front().isData(); }
    bool isDataType(DataType t) const { return atoms_.size() == 1 && atoms_.front().isDataType(t); }
    template <class T>
    bool isDataType() const { return isDataType(T::dataType); }

    /**
     * Check if list is of specified type
     */
    template <typename T>
    inline bool isA() const { return atoms_.size() == 1 && atoms_[0].isA<T>(); }

    /**
     * Sorts list values in ascending order
     */
    void sort();

    /**
     * Randomly reorder list elements
     */
    void shuffle();

    /**
     * Put list elements in back-order
     */
    void reverse();

    /**
     * Returns pointer to minimal element in list or nullptr if empty
     */
    const Atom* min() const;

    /**
     * Returns pointer to minimal element in list or nullptr if empty
     */
    Atom* min();

    /**
     * Returns pointer to greatest element in list or nullptr if empty
     */
    const Atom* max() const;

    /**
     * Returns pointer to greatest element in list or nullptr if empty
     */
    Atom* max();

    /**
     * Get min and max elements from list
     * @param min - min destination
     * @param max - max destination
     * @return false if empty
     */
    bool range(Atom& min, Atom& max) const;

    /**
     * Returns sum of floats in list or boost::none if empty
     */
    MaybeFloat sum() const;

    /**
     * Returns product of floats in list or boost::none if empty
     */
    MaybeFloat product() const;

    /**
     * Checks if atom is in list
     * @param a - searched atom
     * @return true if list contains, otherwise false
     */
    bool contains(const Atom& a) const;
    long findPos(const Atom& a) const;
    long findPos(AtomPredicate pred) const;
    size_t count(const Atom& a) const;
    size_t count(AtomPredicate pred) const;

    bool allOf(AtomPredicate pred) const;
    bool anyOf(AtomPredicate pred) const;
    bool noneOf(AtomPredicate pred) const;

    size_t asSizeT(size_t defaultValue = 0) const;

    enum NonEqualLengthBehaivor {
        MINSIZE = 0, // result of min size
        PADZERO, // result of max size, min list padded with zeroes
        CLIP, // result of max size, min list clipped with last value
        WRAP, // result of max size, min list wraped
        FOLD // result of max size, min list wraped
    };

    /**
     * Return full list view
     */
    AtomListView view() const { return AtomListView(toPdData(), atoms_.size()); }

    /**
     * Return list view from specified position till the end
     */
    AtomListView view(size_t from) const;

    /**
     * Return list view from specified position till and specified length
     */
    AtomListView view(size_t from, size_t length) const;

public:
    static AtomList zeroes(size_t n);
    static AtomList ones(size_t n);
    static AtomList filled(const Atom& a, size_t n);

    /**
     * @brief returns new list that is a sum of original list values and new list ("l") values
     * @param l - list
     * @param b - behaivor flag, when lists are different lengths
     * @return new list
     */
    static AtomList add(const AtomList& a, const AtomList& b, NonEqualLengthBehaivor lb = MINSIZE);

    /**
     * @brief returns new list that contains difference from given lists
     * @param a - first list
     * @param b - second list
     * @param lb - behaivor flag, when lists are different lengths
     * @return new list
     */
    static AtomList sub(const AtomList& a, const AtomList& b, NonEqualLengthBehaivor lb = MINSIZE);

    /**
     * arithmetic operators
     */
    AtomList& operator+=(t_float v);
    AtomList& operator-=(t_float v);
    AtomList& operator*=(t_float v);
    AtomList& operator/=(t_float v);

    AtomList operator+(t_float v) const;
    AtomList operator-(t_float v) const;
    AtomList operator*(t_float v) const;
    AtomList operator/(t_float v) const;

    bool operator==(const AtomList& x) const;
    bool operator!=(const AtomList& x) const { return !operator==(x); }
    bool operator==(const AtomListView& x) const;
    bool operator!=(const AtomListView& x) const { return !operator==(x); }

    friend class AtomListView;

private:
    Container atoms_;
};

template <>
inline bool AtomList::isA<Atom>() const { return size() == 1; }
template <>
inline bool AtomList::isA<AtomList>() const { return true; }

AtomList operator+(const AtomList& l1, const AtomList& l2);
AtomList operator+(const AtomList& l, const Atom& a);
AtomList operator+(const Atom& a, const AtomList& l);

template <typename T>
T AtomList::reduce(T init, std::function<T(const Atom&, const Atom&)> fn) const
{
    T accum(init);

    for (auto& el : atoms_)
        accum = fn(accum, el);

    return accum;
}

std::ostream& operator<<(std::ostream& os, const AtomList& l);

template <typename T>
static AtomList listFrom(T v)
{
    AtomList res;
    res.append(atomFrom<T>(v));
    return res;
}

AtomList listFrom(bool v);
AtomList listFrom(const std::string& v);
AtomList listFrom(const std::vector<std::string>& v);
AtomList listFrom(const AtomList& v);

template <typename T>
static T atomlistToValue(const AtomList&, const T& def) { return def; }

template <>
bool atomlistToValue(const AtomList& l, const bool& def)
{
    if (l.empty())
        return def;

    if (l[0].isBool())
        return l[0].asBool(def);

    if (l[0].isFloat())
        return !std::equal_to<t_float>()(l[0].asFloat(0), 0);

    return false;
}

template <>
float atomlistToValue(const AtomList& l, const float& def)
{
    if (l.empty())
        return def;

    return l[0].asFloat(def);
}

template <>
double atomlistToValue(const AtomList& l, const double& def)
{
    if (l.empty())
        return def;

    return static_cast<double>(l[0].asFloat(static_cast<float>(def)));
}

template <>
int atomlistToValue(const AtomList& l, const int& def)
{
    if (l.empty())
        return def;

    return static_cast<int>(l[0].asFloat(def));
}

template <>
size_t atomlistToValue(const AtomList& l, const size_t& def)
{
    if (l.empty())
        return def;

    t_float v = def;
    if (!l[0].getFloat(&v))
        return def;

    if (v < 0)
        return def;

    return static_cast<size_t>(v);
}

template <>
t_symbol* atomlistToValue(const AtomList& l, t_symbol* const& def)
{
    if (l.empty())
        return const_cast<t_symbol*>(def);

    if (!l[0].isSymbol())
        return const_cast<t_symbol*>(def);

    return l[0].asSymbol();
}

template <>
Atom atomlistToValue(const AtomList& l, const Atom& def)
{
    if (l.empty())
        return def;

    return l[0];
}

} // namespace ceammc

#endif // CEAMMC_ATOMLIST_H
