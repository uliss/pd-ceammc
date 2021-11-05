/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef DATA_SYMBOL_H
#define DATA_SYMBOL_H

#include "ceammc_object.h"
#include "data_protocol.h"

using namespace ceammc;
using DataSymbolBase = StringIFace<BaseObject, t_symbol*>;

class DataSymbol : public DataSymbolBase {
    t_symbol* value_;

public:
    DataSymbol(const PdArgs& args);

    t_symbol*& value() override;
    t_symbol* const& value() const override;

public:
    void proto_set(const AtomListView& lv) override;
    void proto_clear() override;
    void proto_append(t_symbol* const& s) override;
    void proto_prepend(t_symbol* const& s) override;
    long proto_length() const override;
    void proto_lower() override;
    void proto_upper() override;
    void proto_output(t_symbol* const& s) override;
    void proto_substr(int from, size_t len) override;
    void proto_replace(t_symbol* const& subj, t_symbol* const& to) override;
    void proto_insert(int idx, t_symbol* const& str) override;
    bool proto_pop() override;
    void proto_reverse() override;
    void proto_shuffle() override;
    void proto_sort() override;
    void proto_fill(int c) override;
    bool proto_remove_at(int pos) override;
    char32_t proto_choose() const override;
    t_symbol* toString(const AtomListView& lv) const override;
    bool isString(const Atom& a) const override { return a.isSymbol(); }
};

void setup_data_symbol();

#endif // DATA_SYMBOL_H
