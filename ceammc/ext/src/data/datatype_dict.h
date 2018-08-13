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
#include "ceammc_data.h"
#include "ceammc_dataatom.h"

#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <map>
#include <type_traits>

namespace ceammc {

typedef boost::variant<boost::blank, DataAtom, Atom, AtomList> DictValue;
typedef boost::optional<std::string> MaybeString;

/**
 * Outputs dict value to specified outlet
 * @return true on success
 */
bool to_outlet(t_outlet* x, const DictValue& v);

class DataTypeDict : public AbstractData {
public:
    typedef std::map<Atom, DictValue> DictMap;

private:
    DictMap dict_;

public:
    DataTypeDict();
    DataTypeDict(const DataTypeDict& dict);
    DataTypeDict(const std::string& str);

    DataTypeDict* clone() const override;
    DataType type() const override;
    std::string toString() const override;
    bool isEqual(const AbstractData* d) const override;
    bool operator==(const DataTypeDict& d) const;

    size_t size() const;
    bool contains(const Atom& key) const;
    DictValue value(const Atom& key) const;

    template <class T>
    boost::optional<T> valueT(const Atom& key) const
    {
        static_assert(std::is_same<T, DataAtom>()
                || std::is_same<T, Atom>()
                || std::is_same<T, AtomList>(),
            "unsupported type");

        DictValue val = value(key);

        if (val.type() == typeid(T))
            return boost::get<T>(val);
        else
            return boost::none;
    }

    void insert(const std::string& key, const std::string& value);
    void insert(const std::string& key, t_float value);
    void insert(const std::string& key, const Atom& value);
    void insert(const Atom& key, t_float value);
    void insert(const Atom& key, const Atom& value);
    void insert(const Atom& key, const AtomList& value);
    void insert(const Atom& key, const DataAtom& value);
    bool remove(const Atom& key);
    void clear();

    bool fromString(const std::string& str);

    MaybeString toJSON(int indent = -1) const;
    bool fromJSON(const std::string& str);

    bool read(const std::string& path);
    bool write(const std::string& path) const;

public:
    template <class T>
    static bool isType(const DictValue& v)
    {
        return v.type() == typeid(T);
    }

    static bool isNull(const DictValue& v);
    static bool isAtom(const DictValue& v);
    static bool isAtomList(const DictValue& v);
    static bool isDataAtom(const DictValue& v);

public:
    DictMap& innerData() { return dict_; }
    const DictMap& innerData() const { return dict_; }

public:
    static const DataType dataType;
};

}

#endif // DATATYPE_DICT_H
