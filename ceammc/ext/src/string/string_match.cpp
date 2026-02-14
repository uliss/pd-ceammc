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
#include "string_match.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "core_rust.hpp"
#include "datatype_string.h"

namespace {
void on_error(void* user, const char* msg)
{
    Error(static_cast<StringMatch*>(user)) << msg;
}
}

StringMatch::StringMatch(const PdArgs& args)
    : BaseObject(args)
    , re_(nullptr, &ceammc_regexp_free)
{
    createCbListProperty(
        "@re",
        [this]() -> AtomList { return propRe2(); },
        [this](const AtomListView& lv) -> bool { return propSetRe2(lv); })
        ->setArgIndex(0);

    createOutlet();
    createInlet();
}

// for std::unique_ptr
StringMatch::~StringMatch() = default;

void StringMatch::onSymbol(t_symbol* s)
{
    doMatch(s->s_name);
}

void StringMatch::onDataT(const StringAtom& str)
{
    doMatch(str->str().c_str());
}

void StringMatch::doMatch(const char* str)
{
    if (!re_) {
        OBJ_ERR << "no regexp specified";
        return;
    }

    boolTo(0, ceammc_regexp_is_match(re_.get(), str, { this, on_error }));
}

void StringMatch::onInlet(size_t n, const AtomListView& l)
{
    if (l.empty())
        return;

    propSetRe2(l);
}

AtomList StringMatch::propRe2() const
{
    if (re_)
        return Atom(sym_re_);
    else
        return Atom(&s_);
}

bool StringMatch::propSetRe2(const AtomListView& lv)
{
    if (lv.empty())
        return false;

    sym_re_ = gensym(to_string(lv, " ").c_str());
    auto re = ceammc_regexp_create(sym_re_->s_name,
        ceammc_regexp_mode::FULL_MATCH,
        ceammc_regexp_syntax::PURE_DATA,
        { this, on_error });
    re_.reset(re);

    if (!re_) {
        OBJ_ERR << "invalid regexp: " << lv[0];
        return false;
    } else
        return true;
}

void setup_string_match()
{
    ObjectFactory<StringMatch> obj("string.match");
    obj.processData<DataTypeString>();
    obj.addAlias("str.match");
    obj.parseArgsMode(PdArgs::PARSE_UNQUOTE);
    obj.parsePropsMode(PdArgs::PARSE_UNQUOTE);

    obj.setDescription("check if string match regular expression");
    obj.setCategory("string");
    obj.setKeywords({ "match", "regex" });
}
