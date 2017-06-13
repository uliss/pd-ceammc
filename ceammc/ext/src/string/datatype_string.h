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

#include "ceammc_atomlist.h"
#include "ceammc_data.h"

#include <iostream>
#include <string>
#include <vector>

using namespace ceammc;

class DataTypeString : public AbstractData {
    std::string str_;

public:
    DataTypeString(t_symbol* s);
    DataTypeString(const Atom& a);
    DataTypeString(const AtomList& l);
    DataTypeString(const char* str);
    DataTypeString(const std::string& str);
    ~DataTypeString();

    void clear();

    std::string& str() { return str_; }
    const std::string& str() const { return str_; }

    DataType type() const;
    DataTypeString* clone() const;
    std::string toString() const;

    void set(float f);
    void set(t_symbol* s);
    void set(const std::string& s);

    void split(std::vector<std::string>& res, const std::string& sep = "") const;

    bool operator==(const DataTypeString& s) const;

    DataTypeString removeAll(const std::string& s) const;
    DataTypeString removeFirst(const std::string& s) const;
    DataTypeString removeLast(const std::string& s) const;

    DataTypeString replaceAll(const std::string& from, const std::string& to) const;
    DataTypeString replaceFirst(const std::string& from, const std::string& to) const;
    DataTypeString replaceLast(const std::string& from, const std::string& to) const;

    bool contains(const std::string& s) const;
    bool startsWith(const std::string& s) const;
    bool endsWith(const std::string& s) const;

    size_t length() const;

    DataTypeString toLower() const;
    DataTypeString toUpper() const;

    DataTypeString substr(int from, size_t len) const;

public:
    static const DataType dataType;

private:
    void splitEveryChar(std::vector<std::string>& res) const;
    void splitBySep(std::vector<std::string>& res, const std::string& sep) const;
};

std::ostream& operator<<(std::ostream& os, const DataTypeString& d);

#endif // DATASTRING_H
