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
#include "ceammc_regexp.h"
#include "re2/re2.h"

#include <algorithm>

StringMatch::StringMatch(const PdArgs& args)
    : BaseObject(args)
{
    createCbProperty("@re", &StringMatch::propRe2, &StringMatch::propSetRe2);
    if (!positionalArguments().empty())
        propSetRe2(positionalArguments());

    createOutlet();
    createInlet();
}

void StringMatch::onSymbol(t_symbol* s)
{
    if (!re_) {
        OBJ_ERR << "no regexp specified";
        return;
    }

    if (!re_->ok()) {
        OBJ_ERR << "invalid regexp: " << re_->error();
        return;
    }

    bool v = (RE2::FullMatch(s->s_name, *re_));
    floatTo(0, v ? 1 : 0);
}

void StringMatch::onDataT(const DataTypeString& s)
{
    if (!re_) {
        OBJ_ERR << "no regexp specified";
        return;
    }

    if (!re_->ok()) {
        OBJ_ERR << "invalid regexp: " << re_->error();
        return;
    }

    bool v = (RE2::FullMatch(s.str(), *re_));
    floatTo(0, v ? 1 : 0);
}

void StringMatch::onInlet(size_t n, const AtomList& l)
{
    if (l.empty())
        return;

    propSetRe2(l);
}

AtomList StringMatch::propRe2() const
{
    if (re_)
        return Atom(gensym(regexp::unescape(re_->pattern()).c_str()));
    else
        return Atom(&s_);
}

void StringMatch::propSetRe2(const AtomList& lst)
{
    if (lst.empty())
        return;

    re_.reset(new re2::RE2(regexp::escape(to_string(lst, " "))));
    if (!re_->ok())
        OBJ_ERR << "invalid regexp: " << lst[0];
}

void setup_string_match()
{
    ObjectFactory<StringMatch> obj("string.match");
    obj.processData<DataTypeString>();
    obj.addAlias("str.match");
}
