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

#include "ceammc_data.h"
#include "ceammc_object.h"

#include <memory>

// forward definition
namespace re2 {
class RE2;
}

using namespace ceammc;

class StringMatch : public BaseObject {
    using Regexp = std::unique_ptr<re2::RE2>;
    Regexp re_;

public:
    StringMatch(const PdArgs& args);
    ~StringMatch();

    void onSymbol(t_symbol* s) override;
    void onDataT(const StringAtom& str);
    void onInlet(size_t n, const AtomListView& l) override;

    void update(const std::string& re);

private:
    void doMatch(const char* str);
    AtomList propRe2() const;
    bool propSetRe2(const AtomList& lst);
};

void setup_string_match();

#endif // STRING_MATCH_H
