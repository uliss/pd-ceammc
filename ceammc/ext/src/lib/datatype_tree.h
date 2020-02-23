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
#ifndef DATATYPE_TREE_H
#define DATATYPE_TREE_H

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"
#include "ceammc_dataatom.h"
#include "ceammc_either.h"

#include <boost/variant.hpp>
#include <iostream>
#include <memory>

namespace ceammc {

class DataTypeTree;
class DataTypeTreeImpl;

using JsonValue = boost::variant<boost::blank, DataTypeTree, Atom, AtomList>;
using EitherTreeFloat = Either<t_float>;
using EitherTreeSymbol = Either<t_symbol*>;
using EitherTree = Either<DataTypeTree>;

/**
 * @brief The DataTypeTree class - represents tree node
 *
 * Tree syntax is like JSON, but as PureData can't use curly braces {}, it uses simple braces instead
 * array: [1,2,3]
 * string: "string"
 * object: ("key": value, "key2": value2)
 */
class DataTypeTree : public AbstractData {
    using PimplPtr = std::shared_ptr<DataTypeTreeImpl>;

private:
    PimplPtr pimpl_;
    bool copy_on_write_;

    DataTypeTree(DataTypeTreeImpl* pimpl);

public:
    /**
     * create empty tree
     */
    DataTypeTree();
    DataTypeTree(const DataTypeTree& tree);
    DataTypeTree(DataTypeTree&& tree);
    DataTypeTree(const DataTypeTreeImpl& imp);
    DataTypeTree(DataTypeTreeImpl&& imp);

    /**
     * create tree with single float node
     * @param f - node value
     */
    DataTypeTree(t_float f);

    /**
     * create tree with single symbol node
     * @param s - node value
     */
    DataTypeTree(t_symbol* s);
    DataTypeTree(const char* s);
    DataTypeTree(const std::string& s);

    /**
     * create tree with single array node
     * @param lst - list of floats
     */
    DataTypeTree(const FloatList& lst);

    /**
     * create tree with single array node
     * @param lst - list floats or symbols
     */
    DataTypeTree(const AtomList& lst);

    /**
     * assign operator
     * @return reference to this
     */
    DataTypeTree& operator=(const DataTypeTree& t);

    /**
     * Creates tree copy
     * @return pointer to new tree, should be deleted
     */
    DataTypeTree* clone() const final;

    /**
     * @return tree data type constant
     */
    DataType type() const final;

    /**
     * @return string tree representation
     */
    std::string toString() const final;

    /**
     * Compare with data pointer
     * @param d - data pointer
     */
    bool isEqual(const AbstractData* d) const final;

    /**
     * Comparasion operator
     */
    bool operator==(const DataTypeTree& d) const;

    /**
     * Returns number of elements
     * for null    - 0
     *     boolean - 1
     *     string  - 1
     *     float   - 1
     *     array   - number of array elements
     *     object  - number of keys
     */
    size_t size() const;

    /**
     * Checks if empty tree value
     * for null    - true
     *     boolean - false
     *     string  - false
     *     float   - false
     *     array   - true if empty array
     *     object  - true if empty object
     */
    bool empty() const;

    /**
     * @return true if tree value is array
     */
    bool isArray() const;

    /**
     * @return true if tree value is array and contains only simple object types: null, float or strings
     */
    bool isSimpleArray() const;

    /**
     * check if tree value is null
     */
    bool isNull() const;

    /**
     * check if tree value is float (for json boolean value also return true)
     */
    bool isFloat() const;

    /**
     * check if tree value is string
     */
    bool isString() const;

    /**
     * check if tree value is object
     */
    bool isObject() const;

    /**
     * try to get float value from tree
     * @param f - reference to write value, value is written only if tree value is float
     * @return true on success, false on error (if not float)
     */
    bool getFloat(t_float& f) const;

    /**
     * try to get symbol value from tree
     * @param s - pointer to write value, value is written only if tree value is string
     * @return true on success, false on error (if not string)
     */
    bool getSymbol(t_symbol** s) const;

    EitherTreeFloat getFloat() const;
    EitherTreeSymbol getSymbol() const;
    EitherTree getList() const;
    EitherTree getObject() const;

    /**
     * Returns object key list
     * @return empty list if tree has not object value
     */
    AtomList keys() const;

    /**
     * @brief clear tree value contents
     *
     * null   -> `null`
     * string -> `""`
     * number -> `0`
     * object -> `()`
     * array  -> `[]`
     */
    void clear();

    /**
     * Adds float to the end of tree array or creates array with single element if null tree
     * @param f - value to add
     * @return true on success, false on error (if value if not an array or null)
     * @example adding 10 to [1,2] = [1,2,10]
     * @example adding 10 to null = [10]
     */
    bool arrayAdd(t_float f);

    /**
     * Adds symbol to the end of tree array or creates array with single element if null tree
     * @param s - symbol to add
     * @return true on success, false on error (if value if not an array or null)
     */
    bool arrayAdd(t_symbol* s);

    /**
     * Adds array to the end of tree array or creates array with single element if null tree
     * @param l - array to add
     * @return true on success, false on error (if value if not an array or null)
     *
     * @example adding [1,2] to [1,2,3] = [1,2,3,[1,2]]
     * @example adding [1,2] to null = [[1,2]]
     */
    bool arrayAdd(const AtomList& l);

    bool addTree(const DataTypeTree& tree);

    void setFloat(t_float f);
    void setSymbol(t_symbol* s);
    bool set(const DataTPtr<DataTypeTree>& ptr);

    DataTPtr<DataTypeTree> match(const char* pattern) const;
    DataTypeTree at(size_t idx) const;
    DataTypeTree at(t_symbol* key) const;

    bool insertFloat(const char* key, t_float f);
    bool insertSymbol(const char* key, t_symbol* s);
    bool insertTree(const char* key, const DataTypeTree& tree);

    void outputTo(t_outlet* o) const;
    t_float asFloat(t_float def = 0) const;
    Atom asAtom() const;
    AtomList asAtomList() const;
    DataTPtr<DataTypeTree> asDataPtr() const;

public:
    /**
     * Parse given string and replace current tree value
     * @param str - string with tree syntax
     * @return true on sucess, false on parse error
     */
    bool parse(const char* str);

public:
    static const DataType dataType;
    static DataTPtr<DataTypeTree> newFromString(const char* str);
    static DataTPtr<DataTypeTree> newFromString(const std::string& str);
    static DataTPtr<DataTypeTree> newEmpty();

    static DataTypeTree fromString(const std::string& str);
    static DataTypeTree fromString(const char* str);

private:
    void detachPimpl();
    bool nullPimpl() const;
    static DataTypeTree& nullInstance();
};

std::ostream& operator<<(std::ostream& os, const DataTypeTree& t);
}

#endif // DATATYPE_TREE_H
