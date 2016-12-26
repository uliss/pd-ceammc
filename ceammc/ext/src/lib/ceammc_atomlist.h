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
#include <string>
#include <vector>

namespace ceammc {

typedef std::vector<t_float> FloatList;
typedef bool (*AtomPredicate)(const Atom& a);
typedef Atom (*AtomGenerator)();
typedef Atom (*AtomMapFunction)(const Atom& a);

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
    AtomList(size_t n, t_atom* lst);
    explicit AtomList(int n, t_atom* lst);
    size_t size() const;
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
    Atom* clipAt(size_t pos);
    const Atom* clipAt(size_t pos) const;

    /**
     * Same as at(), but values for index greater than the size of the
     * List will be wrapped around to 0.
     * @param pos - position index
     * @return pointer to element, or NULL if empty
     */
    Atom* wrapAt(size_t pos);
    const Atom* wrapAt(size_t pos) const;

    /**
     * Same as at(), but values for index greater than the size of the
     * List will be folded back.
     * @param pos - position index
     * @return pointer to element, or NULL if empty
     */
    Atom* foldAt(size_t pos);
    const Atom* foldAt(size_t pos) const;

    /**
     * Resize list. If new size is less than current, last values are dropped.
     * If new size is bigger - pad with given value
     * @param n - new size
     * @param v - pad value
     */
    void resizePad(size_t n, const Atom& v);

    /**
     * Get property value from list
     * @param name - property name with (starts with '@')
     * @param dest - output destination
     * @return true if property was found and it has value
     */
    bool property(const std::string& name, Atom* dest) const;

    /**
     * Checks is has property in list
     */
    bool hasProperty(const std::string& name) const;

    void fromPdData(size_t n, t_atom* lst);
    void fromPdData(int n, t_atom* lst);
    t_atom* toPdData() const;
    void append(const Atom& a);
    bool insert(size_t pos, const Atom& a);
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

    void sort();
    void shuffle();
    void reverse();
    //
    AtomList sub(int begin, int end);
    AtomList filtered(AtomPredicate pred) const;

    const Atom* min() const;
    const Atom* max() const;
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

    bool contains(const Atom& a) const;
    int findPos(const Atom& a) const;
    size_t count(const Atom& a) const;
    size_t count(AtomPredicate pred) const;

    bool allOff(AtomPredicate pred) const;
    bool anyOff(AtomPredicate pred) const;
    bool noneOff(AtomPredicate pred) const;

    FloatList asFloats() const;

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

    /**
     * @brief returns new list that is contains from pair difference
     * @param l - list
     * @param b - behaivor flag, when lists are different lengths
     * @return new list
     */
    AtomList sub(const AtomList& l, NonEqualLengthBehaivor b = MINSIZE) const;

public:
    static AtomList zeroes(size_t n);
    static AtomList ones(size_t n);
    static AtomList filled(const Atom& a, size_t n);
    static AtomList values(size_t n, ...);

public:
    friend bool operator==(const AtomList& l1, const AtomList& l2);
    friend bool operator!=(const AtomList& l1, const AtomList& l2);
};

bool operator==(const AtomList& l1, const AtomList& l2);
bool operator!=(const AtomList& l1, const AtomList& l2);
std::ostream& operator<<(std::ostream& os, const AtomList& l);

void to_outlet(t_outlet* x, const AtomList& a);

} // namespace ceammc

#endif // CEAMMC_ATOMLIST_H
