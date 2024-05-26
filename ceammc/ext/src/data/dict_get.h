/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef DICT_GET_H
#define DICT_GET_H

#include "dict_base.h"
#include "lex/parser_dict_expr.h"

using namespace ceammc;

struct DictGetKeyData {
    t_symbol* key_;
    parser::DictExprMatchList match_;
    bool isMatch() const;
};

class DictGet : public DictBase {
    std::vector<DictGetKeyData> key_data_;
    ListProperty* keys_;
    AtomProperty* default_;

public:
    DictGet(const PdArgs& args);
    void initDone() final;

    void onDataT(const DictAtom& dict);

private:
    void findMatches(const AtomListView &lst, const parser::DictExprMatcher* m, size_t level, size_t outlet);
};

void setup_dict_get();

#endif // DICT_GET_H
