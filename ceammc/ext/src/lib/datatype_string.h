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
#ifndef DATASTRING_H
#define DATASTRING_H

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"

#include <iostream>
#include <string>
#include <vector>

namespace ceammc {

class DataTypeString : public AbstractData {
    std::string str_;

public:
    /**
     * Creates from pd symbol
     */
    DataTypeString(t_symbol* s);

    /**
     * Creates from string
     */
    DataTypeString(const char* str);

    /**
     * Creates from string
     */
    DataTypeString(const std::string& str);

    /**
     * Creates from atom
     *  - if atom is data, uses data->toString() result
     */
    DataTypeString(const Atom& a);

    /**
     * Create string from list of atoms - converts using to_string with space separator
     * @example list [1,2,3] -> string "1 2 3"
     */
    DataTypeString(const AtomList& l);

    // copy/move
    DataTypeString(const DataTypeString& d);
    DataTypeString(DataTypeString&& d) noexcept;
    // assign
    DataTypeString& operator=(const DataTypeString& s);
    DataTypeString& operator=(DataTypeString&& s);

    /**
     * Polymorphic copy creation
     */
    DataTypeString* clone() const override;

    /** dtor */
    ~DataTypeString() noexcept;

    /**
     * Reference to underlying string
     */
    std::string& str() noexcept { return str_; }
    const std::string& str() const noexcept { return str_; }

    /**
     * Polymorphic string representation
     */
    std::string toString() const final;

    /**
     * Polymorphic JSON string representation (quoted escaped string)
     */
    std::string valueToJsonString() const override;

    /**
     * Check for equality with abstract data pointer
     */
    bool isEqual(const AbstractData* d) const noexcept final;

    /**
     * Compare with abstract data pointer
     */
    bool isLess(const AbstractData* d) const noexcept final;

    /**
     * Set new string content from pd symbol
     */
    void set(t_symbol* s);

    /**
     * Set new string content
     */
    void set(const std::string& s);

    /**
     * Makes empty
     */
    void clear() noexcept;

    /**
     * Split current string to vector of string by given separator
     */
    void split(std::vector<std::string>& res, const std::string& sep = "") const;

    // operators
    bool operator==(const DataTypeString& s) const noexcept { return str_ == s.str_; }
    bool operator!=(const DataTypeString& s) const noexcept { return !operator==(s); }

    /**
     * Remove all occurences of s from current string
     * @return new string
     */
    DataTypeString removeAll(const std::string& s) const;

    /**
     * Remove first occurence of s from current string
     * @return new string
     */
    DataTypeString removeFirst(const std::string& s) const;

    /**
     * Remove last occurence of s from current string
     * @return new string
     */
    DataTypeString removeLast(const std::string& s) const;

    /**
     * Replace all occurences to specified value
     * @return new string
     */
    DataTypeString replaceAll(const std::string& from, const std::string& to) const;

    /**
     * Replace first occurence to specified value
     * @return new string
     */
    DataTypeString replaceFirst(const std::string& from, const std::string& to) const;

    /**
     * Replace last occurence to specified value
     * @return new string
     */
    DataTypeString replaceLast(const std::string& from, const std::string& to) const;

    /**
     * Returns string length (UTF-8 encoding is supported)
     */
    size_t length() const;

    /**
     * Returns new string in lower case (UTF-8 is supported)
     */
    DataTypeString toLower() const;

    /**
     * Returns new string in upper case (UTF-8 is supported)
     */
    DataTypeString toUpper() const;

    /**
     * Returns substring
     */
    DataTypeString substr(int from, size_t len) const;

    int type() const noexcept final;

public:
    static const int dataType;

private:
    void splitEveryChar(std::vector<std::string>& res) const;
    void splitBySep(std::vector<std::string>& res, const std::string& sep) const;
};

std::ostream& operator<<(std::ostream& os, const DataTypeString& d);

}

#endif // DATASTRING_H
