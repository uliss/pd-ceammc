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
#include "ceammc_datapointer.h"

#include <string>
#include <vector>

using namespace ceammc;

class DataString : public BaseData {
    std::string str_;

public:
    DataString(t_symbol* s);
    DataString(const Atom& a);
    DataString(const AtomList& l);
    DataString(const char* str);
    DataString(const std::string& str);
    ~DataString();

    void clear();

    std::string& str() { return str_; }
    const std::string& str() const { return str_; }

    DataType type() const;
    DataString* clone() const;
    std::string toString() const;

    void set(float f);
    void set(t_symbol* s);
    void set(const std::string& s);

    void split(std::vector<std::string>& res, const std::string& sep = "") const;

public:
    static const DataType dataType;

private:
    void splitEveryChar(std::vector<std::string>& res) const;
    void splitBySep(std::vector<std::string>& res, const std::string& sep) const;
};

#endif // DATASTRING_H
