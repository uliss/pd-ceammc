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
#include "function.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

#include <map>
#include <stdexcept>
#include <stdio.h>
#include <string>

class FunctionMap {
    typedef std::map<t_symbol*, Function*> FuncMap;
    FuncMap fmap_;

    FunctionMap()
    {
    }

public:
    static FunctionMap& instance()
    {
        static FunctionMap f;
        return f;
    }

    bool exists(t_symbol* name) const
    {
        return fmap_.find(name) != fmap_.end();
    }

    Function* get(t_symbol* name)
    {
        FuncMap::iterator it = fmap_.find(name);
        return it == fmap_.end() ? 0 : it->second;
    }

    void add(t_symbol* name, Function* f)
    {
        fmap_[name] = f;
    }

    void remove(t_symbol* name)
    {
        fmap_.erase(name);
    }
};

Function::Function(const PdArgs& a)
    : BaseObject(a)
    , inlet_(this)
{
    name_ = new SymbolProperty("@name", &s_);
    name_->setArgIndex(0);
    name_->setSymbolCheckFn([this](t_symbol* s) -> bool {
        const bool not_ok = (s == &s_) || FunctionMap::instance().exists(s);
        if (not_ok)
            OBJ_ERR << fmt::format("function should be unique and non-empty: '{}'", s->s_name);

        return !not_ok;
    });
    name_->setSuccessFn([this](Property*) {
        if (!empty()) {
            FunctionMap::instance().add(name_->value(), this);
            OBJ_DBG << "function '" << name_->value()->s_name << "' created";
        }
    });
    name_->setInitOnly();
    addProperty(name_);

    inlet_new(owner(), &inlet_.x_obj, nullptr, nullptr);

    createOutlet();
    createOutlet();
}

Function::~Function()
{
    if (!empty()) {
        FunctionMap::instance().remove(name_->value());
        OBJ_DBG << "function '" << name_->value()->s_name << "' removed";
    }
}

void Function::initDone()
{
    BaseObject::initDone();
    if (empty())
        OBJ_DBG << "function name is empty";
}

void Function::onBang()
{
    result_.clear();
    bangTo(1);
}

void Function::onFloat(t_float f)
{
    result_.clear();
    floatTo(1, f);
}

void Function::onList(const AtomListView& lv)
{
    result_.clear();
    listTo(1, lv);
}

void Function::onSymbol(t_symbol* s)
{
    result_.clear();
    symbolTo(1, s);
}

std::vector<Message>& Function::result()
{
    return result_;
}

const std::vector<Message>& Function::result() const
{
    return result_;
}

void Function::proxy_any(int, t_symbol* s, const AtomListView& v)
{
    if (s == &s_bang) {
        result_.push_back(Message::makeBang());
        bangTo(0);
    } else if (s == &s_float) {
        auto f = v.asFloat();
        result_.emplace_back(f);
        floatTo(0, f);
    } else if (s == &s_symbol) {
        auto s = v.asSymbol();
        result_.emplace_back(s);
        symbolTo(0, s);
    } else if (s == &s_list) {
        result_.emplace_back(v);
        listTo(0, v);
    } else {
        result_.emplace_back(s, v);
        anyTo(0, s, v);
    }
}

bool Function::exists(t_symbol* name)
{
    return FunctionMap::instance().exists(name);
}

Function* Function::function(t_symbol* name)
{
    return FunctionMap::instance().get(name);
}

void setup_base_function()
{
    ObjectFactory<Function> f("function");
    f.addAlias("func");
    f.noPropsDispatch();

    f.setDescription("named function");
    f.addAuthor("Serge Poltavsky");
    f.setKeywords({ "function" });
    f.setCategory("base");
    f.setSinceVersion(0, 3);

    f.setXletsInfo({ "function input", "input from side-chain" }, { "function output", "output to side chain" });

    InletProxy<Function>::init();
    InletProxy<Function>::set_any_callback(&Function::proxy_any);
}
