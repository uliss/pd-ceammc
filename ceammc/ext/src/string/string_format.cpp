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
#include "ceammc_factory.h"
#include "ceammc_format.h"

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
    , fmt_result_("")
    , int_mode_(0)
{
    createOutlet();

    createCbProperty("@format", &StringFormat::propGetFormat, &StringFormat::propSetFormat)
        ->setArgIndex(0);

    int_mode_ = new FlagProperty("@int");
    addProperty(int_mode_);
}

void StringFormat::onBang()
{
    atomTo(0, new DataTypeString(fmt_result_));
}

void StringFormat::onData(const Atom& d)
{
    try {
        fmt_result_ = tfm::format(fmt_str_.c_str(), d.asData()->toString());
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
        return;
    }

    onBang();
}

void StringFormat::onFloat(t_float v)
{
    try {
        if (int_mode_->value()) {
            fmt_result_ = tfm::format(fmt_str_.c_str(), int(v));
        } else {
            fmt_result_ = tfm::format(fmt_str_.c_str(), v);
        }
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
        return;
    }

    onBang();
}

void StringFormat::onSymbol(t_symbol* s)
{
    try {
        fmt_result_ = tfm::format(fmt_str_.c_str(), s->s_name);
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
        if (lst[i].isFloat()) {
            if (int_mode_->value())
                args.add(lst[i].asInt());
            else
                args.add(lst[i].asFloat());
        } else
            args.add(to_string(lst[i]));
    }

    try {
        std::ostringstream buf;
        tfm::vformat(buf, fmt_str_.c_str(), args);
        fmt_result_ = buf.str();
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
        return;
    }

    onBang();
}

void StringFormat::dump() const
{
    BaseObject::dump();
    OBJ_DBG << "formated value: " << fmt_result_;
}

AtomList StringFormat::propGetFormat() const
{
    return fmt_atoms_;
}

void StringFormat::propSetFormat(const AtomList& lst)
{
    fmt_atoms_ = lst;
    fmt_str_ = to_string(lst, " ");
}

void setup_string_format()
{
    ObjectFactory<StringFormat> obj("string.format");
    obj.processData();
    obj.addAlias("str.format");
}
