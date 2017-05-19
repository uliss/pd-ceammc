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
    , name_(0)
{
    Atom name = positionalArgument(0, Atom());
    if (name.isNone() || !name.isSymbol()) {
        throw std::runtime_error("function name required!");
    }

    name_ = name.asSymbol();

    if (FunctionMap::instance().exists(name_)) {
        throw std::runtime_error("function already exists");
    }

    FunctionMap::instance().add(name_, this);

    createInlet();
    createOutlet();
    createOutlet();

    OBJ_DBG << "function '" << name_->s_name << "' created.";
}

Function::~Function()
{
    FunctionMap::instance().remove(name_);
    OBJ_DBG << "function '" << name_->s_name << "' removed.";
}

void Function::onBang()
{
    bangTo(0);
}

void Function::onFloat(float f)
{
    result_ = Message();
    floatTo(1, f);
}

void Function::onList(const AtomList& l)
{
    listTo(1, l);
}

void Function::onSymbol(t_symbol* s)
{
    symbolTo(1, s);
}

void Function::onInlet(size_t n, const AtomList& l)
{
    if (n != 1)
        return;

    result_ = l;
    listTo(0, l);
}

Message& Function::result()
{
    return result_;
}

const Message& Function::result() const
{
    return result_;
}

bool Function::exists(t_symbol* name)
{
    return FunctionMap::instance().exists(name);
}

Function* Function::function(t_symbol* name)
{
    return FunctionMap::instance().get(name);
}

extern "C" void function_setup()
{
    ObjectFactory<Function> f("function");
    f.addAlias("func");
}
