/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef STRING_MATCH_H
#define STRING_MATCH_H

#include "ceammc_object.h"
#include "datatype_string.h"

#include <memory>

// forward definition
namespace re2 {
class RE2;
}

using namespace ceammc;

class StringMatch : public BaseObject {
    typedef std::shared_ptr<re2::RE2> Regexp;
    Regexp re_;

public:
    StringMatch(const PdArgs& args);

    void onSymbol(t_symbol* s);
    void onDataT(const DataTypeString& s);
    void onInlet(size_t n, const AtomList& l);

    void update(const std::string& re);

private:
    AtomList propRe2() const;
    void propSetRe2(const AtomList& lst);
};

void setup_string_match();

#endif // STRING_MATCH_H
