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
#include <deque>
#include <string>
#include <vector>

namespace ceammc {

typedef std::vector<t_float> FloatList;
typedef bool (*AtomPredicate)(const Atom& a);
typedef Atom (*AtomGenerator)();

class AtomList {
    std::vector<Atom> atoms_;
    typedef std::vector<Atom> atom_list;
    typedef atom_list::const_iterator const_atom_iterator;
    typedef atom_list::iterator atom_iterator;
    typedef atom_list::reverse_iterator atom_riterator;

    static bool calc_rel_idx(int pos, size_t* dest, size_t sz);
    bool getRelativeIdx(int pos, size_t* idx) const;

public:
    AtomList();
    AtomList(const Atom& a);
    AtomList(const Atom& a, const Atom& b);
    AtomList(size_t n, t_atom* lst);
    explicit AtomList(int n, t_atom* lst);
    size_t size() const;
    void reserve(size_t n);
    bool empty() const;

    /**
         * @brief returns reference to element at specified position
         * @param pos - position (starting from 0)
         * @return reference to element
         * @throw exception if invalid position given
         */
    Atom& at(size_t pos);
    const Atom& at(size_t pos) const;
    Atom& operator[](size_t pos);
    const Atom& operator[](size_t pos) const;

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
    bool property(const std::string& name, Atom* dest) const;

    /**
         * Get property value from list
         * @param name - property name with (starts with '@')
         * @param dest - output destination
         * @return true if property was found and it has value
         */
    bool property(const std::string& name, AtomList* dest) const;

    /**
         * Returns all properties and their values from list
         */
    std::deque<AtomList> properties() const;

    /**
         * Checks is has property in list
         */
    bool hasProperty(const std::string& name) const;

    AtomList map(AtomMapFunction f) const;
    AtomList map(AtomFloatMapFunction f) const;
    AtomList map(AtomSymbolMapFunction f) const;

    AtomList slice(int start) const;
    AtomList slice(int start, int end, size_t step = 1) const;

    void fromPdData(size_t n, t_atom* lst);
    void fromPdData(int n, t_atom* lst);
    t_atom* toPdData() const;
    void append(const Atom& a);
    void append(const AtomList& l);
    bool insert(size_t pos, const Atom& a);
    bool insert(size_t pos, const AtomList& l);
    bool remove(size_t pos);
    void removeAll(const Atom& a);
    void removeAll(AtomPredicate pred);
    void replaceAll(const Atom& old_value, const Atom& new_value);
    void replaceAll(AtomPredicate pred, const Atom& new_value);
    void clear();
    void fill(const Atom& a);
    void fill(const Atom& a, size_t sz);
    Atom* first();
    Atom* last();
    const Atom* first() const;
    const Atom* last() const;

    bool isBang() const;
    bool isFloat() const;
    bool isSymbol() const;
    bool isProperty() const;
    bool isList() const;

    void sort();
    void shuffle();
    void reverse();

    AtomList filtered(AtomPredicate pred) const;

    const Atom* min() const;
    const Atom* max() const;
    bool range(Atom& min, Atom& max) const;
    const Atom* find(const Atom& a) const;
    const Atom* find(AtomPredicate pred) const;
    const Atom* findLast(const Atom& a) const;
    const Atom* findLast(AtomPredicate pred) const;
    Atom* min();
    Atom* max();
    Atom* find(const Atom& a);
    Atom* find(AtomPredicate pred);
    Atom* findLast(const Atom& a);
    Atom* findLast(AtomPredicate pred);

    /**
         * Returns sum of floats in list or 0 if empty
         */
    float sum() const;

    /**
         * Returns product of floats in list or 0 if empty
         */
    float product() const;

    bool contains(const Atom& a) const;
    int findPos(const Atom& a) const;
    int findPos(AtomPredicate pred) const;
    size_t count(const Atom& a) const;
    size_t count(AtomPredicate pred) const;

    bool allOf(AtomPredicate pred) const;
    bool anyOf(AtomPredicate pred) const;
    bool noneOf(AtomPredicate pred) const;

    FloatList asFloats() const;
    size_t asSizeT(size_t defaultValue = 0) const;

    /**
         * @brief output list atoms separatly, one by one
         * @param x - output outlet
         */
    void outputAtoms(t_outlet* x) const;

    /**
         * Outputs list to given outlet
         * @param x - pointer to outlet
         */
    void output(t_outlet* x) const;

    /**
         * Outputs list content as any message. First list atom became selector
         */
    void outputAsAny(t_outlet* x) const;

    /**
         * Outputs list content as any message.
         * @param x - pointer to outlet
         * @param s - any selector
         */
    void outputAsAny(t_outlet* x, t_symbol* s) const;

    enum NonEqualLengthBehaivor {
        MINSIZE = 0, // result of min size
        PADZERO, // result of max size, min list padded with zeroes
        CLIP, // result of max size, min list clipped with last value
        WRAP, // result of max size, min list wraped
        FOLD // result of max size, min list wraped
    };

    template <typename T>
    T reduce(T init, T (*fn)(const Atom&, const Atom&)) const;
    t_float reduceFloat(t_float init, t_float def, t_float (*fn)(t_float, t_float)) const;

    bool normalizeFloats();

public:
    static AtomList zeroes(size_t n);
    static AtomList ones(size_t n);
    static AtomList filled(const Atom& a, size_t n);
    static AtomList values(size_t n, ...);

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
    AtomList& operator+=(double v);
    AtomList& operator-=(double v);
    AtomList& operator*=(double v);
    AtomList& operator/=(double v);

    AtomList operator+(double v) const;
    AtomList operator-(double v) const;
    AtomList operator*(double v) const;
    AtomList operator/(double v) const;

public:
    friend bool operator==(const AtomList& l1, const AtomList& l2);
    friend bool operator!=(const AtomList& l1, const AtomList& l2);
};

template <typename T>
T AtomList::reduce(T init, T (*fn)(const Atom&, const Atom&)) const
{
    T accum(init);
    AtomList::const_atom_iterator it;
    for (it = atoms_.begin(); it != atoms_.end(); ++it) {
        accum = fn(accum, *it);
    }

    return accum;
}

bool operator==(const AtomList& l1, const AtomList& l2);
bool operator!=(const AtomList& l1, const AtomList& l2);
std::ostream& operator<<(std::ostream& os, const AtomList& l);

void to_outlet(t_outlet* x, const AtomList& a);

template <typename T>
static Atom atomFrom(T v) { return Atom(v); }

Atom atomFrom(const std::string& v);

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

    if (l[0].isFloat())
        return l[0].asFloat(0.f) != 0.f;

    if (l[0].isSymbol())
        return l[0].asSymbol() == gensym("true");

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
