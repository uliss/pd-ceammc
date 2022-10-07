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
#ifndef DATATYPE_DICT_H
#define DATATYPE_DICT_H

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"
#include "ceammc_maybe.h"

#include <map>
#include <type_traits>

namespace ceammc {

using MaybeString = Maybe<std::string>;

class DataTypeDict : public AbstractData {
public:
    using DictMap = std::map<t_symbol*, AtomList>;
    using DictKeyValue = DictMap::value_type;

    struct DictEntry : public DictKeyValue {
        template <typename... Values>
        DictEntry(const char* k, Values... args)
            : DictKeyValue(gensym(k), AtomList(args...))
        {
        }

        template <typename... Values>
        DictEntry(t_symbol* k, Values... args)
            : DictKeyValue(k, AtomList(args...))
        {
        }
    };

    using iterator = DictMap::iterator;
    using const_iterator = DictMap::const_iterator;

private:
    DictMap dict_;

public:
    /**
     * create empty dict
     */
    DataTypeDict() noexcept;

    /**
     * Create dict from doing string parse
     * If parsing error occurs returns ???
     * @throw std::logic_error
     */
    DataTypeDict(const std::string& str);

    /**
     * Copy ctor
     */
    DataTypeDict(const DataTypeDict& dict);

    /**
     * Init list initializer
     */
    DataTypeDict(std::initializer_list<DictKeyValue> pairs);

    /**
     * Move ctor
     */
    DataTypeDict(DataTypeDict&& dict) noexcept;

    /**
     * Assign operators
     */
    DataTypeDict& operator=(const DataTypeDict& dict);
    DataTypeDict& operator=(DataTypeDict&& dict) noexcept;

    /**
     * Polymorphic copy
     */
    DataTypeDict* clone() const override;

    /**
     * Polynorphic data type
     */
    DataTypeId type() const noexcept final;

    /**
     * Polymorphic string value
     */
    std::string toString() const override;

    /**
     * Output as JSON string
     */
    std::string toJsonString() const final;

    std::string toListStringContent() const final;
    std::string toDictStringContent() const final;
    bool set(const AbstractData* d) noexcept final;

    /**
     * Polymorphics equality check
     */
    bool isEqual(const AbstractData* d) const noexcept final;

    /**
     * Equal check
     */
    bool operator==(const DataTypeDict& d) const noexcept;

    // iterators
    iterator begin() { return dict_.begin(); }
    iterator end() { return dict_.end(); }
    const_iterator begin() const { return dict_.begin(); }
    const_iterator end() const { return dict_.end(); }
    const_iterator cbegin() { return dict_.begin(); }
    const_iterator cend() { return dict_.end(); }

    /**
     * Return number of entries in dict
     */
    size_t size() const noexcept { return dict_.size(); }

    /**
     * Checks if dict contains key
     */
    bool contains(t_symbol* key) const noexcept { return dict_.find(key) != dict_.end(); }
    bool contains(const char* key) const noexcept { return contains(gensym(key)); }

    /**
     * Returns list of all keys in dict
     */
    AtomList keys() const;

    /**
     * Returns keys and values intermixed as single list
     */
    AtomList flattenToList() const;

    /**
     * Get element value by specified key
     * @param key
     * @return reference to element value or
     * @throw std::out_of_range if key not found
     */
    AtomList& at(t_symbol* key);
    AtomList& at(const char* key) { return at(gensym(key)); }
    const AtomList& at(t_symbol* key) const;
    const AtomList& at(const char* key) const { return at(gensym(key)); }

    /**
     * Insert key for dict
     */
    void insert(const char* key, const Atom& atom) { insert(gensym(key), atom); }
    void insert(t_symbol* key, const Atom& atom) { dict_[key] = atom; }
    void insert(const char* key, const AtomListView& value) { insert(gensym(key), value); }
    void insert(t_symbol* key, const AtomListView& value) { dict_[key] = value; }

    /**
     * Remove dict entries by key predicate
     */
    void removeIf(std::function<bool(t_symbol*)> key_pred);

    /**
     * Tries to remove specified key
     * @return true on success, false if key not found
     */
    bool remove(t_symbol* key);
    bool remove(const char* key) { return remove(gensym(key)); }

    /**
     * Removes all data from dict
     */
    void clear() noexcept { dict_.clear(); }

    MaybeString toJSON(int indent = -1) const;
    bool fromJSON(const std::string& str);

    bool read(const std::string& path);
    bool write(const std::string& path) const;

    /**
     * Randomly choose key from dict
     * @param key - destination to write random element
     * @return true on success, false on error
     */
    bool choose(Atom& key) const noexcept;

    const_iterator find(t_symbol* key) const { return dict_.find(key); }
    const_iterator find(const char* key) const { return dict_.find(gensym(key)); }

public:
    /**
     * Creates dict from list, splitting it by step
     * @param step - should be >0
     */
    static DataTypeDict fromList(const AtomListView& lv, size_t step = 2);

    DictMap& innerData() { return dict_; }
    const DictMap& innerData() const { return dict_; }

public:
    static const DataTypeId dataType;
};

std::ostream& operator<<(std::ostream& os, const DataTypeDict& dict);
}

#endif // DATATYPE_DICT_H
