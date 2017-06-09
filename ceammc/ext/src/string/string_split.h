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
#ifndef STRING_SPLIT_H
#define STRING_SPLIT_H

#include "ceammc_dataatom.h"
#include "ceammc_object.h"
#include "datatype_string.h"

#include <boost/shared_ptr.hpp>
#include <vector>

using namespace ceammc;

class StringSplit : public BaseObject {
    typedef DataT<DataTypeString> String;
    typedef boost::shared_ptr<String> StringPtr;

    std::vector<DataAtom> tokens_;
    std::string sep_;

public:
    StringSplit(const PdArgs& a);
    void onSymbol(t_symbol* s);
    void onDataT(const DataTypeString& s);

    AtomList propGetSeparator() const;
    void propSetSeparator(const AtomList& l);

private:
    void split(const DataTypeString& s);
    void output();
    void parseArgs();
};

extern "C" void setup_string0x2esplit();

#endif // STRING_SPLIT_H
