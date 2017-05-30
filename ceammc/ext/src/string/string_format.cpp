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
#include "string_format.h"
#include "data_string_factory.h"

#include <sstream>
#include <stdexcept>

#define TINYFORMAT_ERROR(msg)          \
    {                                  \
        throw std::runtime_error(msg); \
    }

#include "tinyformat.h"
#include "vformatlist.h"

StringFormat::StringFormat(const PdArgs& a)
    : BaseObject(a)
    , str_(new String(new DataString(positionalArguments())))
    , str_formatted_(new String(new DataString("")))
{
    createOutlet();
}

void StringFormat::onData(const DataString& d)
{
}

void StringFormat::onBang()
{
    if (!str_formatted_)
        return;

    atomTo(0, str_formatted_->toAtom());
}

void StringFormat::onFloat(float v)
{
    if (!str_)
        return;

    try {
        const char* format = str_->data()->str().c_str();
        str_formatted_->data()->str() = tfm::format(format, v);
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
        return;
    }

    onBang();
}

void StringFormat::onSymbol(t_symbol* s)
{
    if (!str_)
        return;

    try {
        const char* format = str_->data()->str().c_str();
        str_formatted_->data()->str() = tfm::format(format, s->s_name);
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
        return;
    }

    onBang();
}

void StringFormat::onList(const AtomList& lst)
{
    VFormatList args;
    for (size_t i = 0; i < lst.size(); i++) {
        switch (lst[i].type()) {
        case Atom::FLOAT:
            args.add(lst[i].asFloat());
            break;
        case Atom::SYMBOL:
        case Atom::PROPERTY:
            args.add(std::string(lst[i].asSymbol()->s_name));
            break;
        }
    }

    try {
        const char* format = str_->data()->str().c_str();
        std::ostringstream buf;
        tfm::vformat(buf, format, args);
        str_formatted_->data()->str() = buf.str();
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
        return;
    }

    onBang();
}

extern "C" void setup_string0x2eformat()
{
    DataStringFactory<StringFormat, DataString> obj("string.format");
    obj.addAlias("str.format");
}
