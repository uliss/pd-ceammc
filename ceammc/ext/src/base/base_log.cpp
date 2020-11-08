/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "base_log.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

static t_symbol* SYM_ERROR;
static t_symbol* SYM_POST;
static t_symbol* SYM_DEBUG;
static t_symbol* SYM_VERBOSE;
static t_symbol* SYM_STDOUT;
static t_symbol* SYM_STDERR;
static t_symbol* SYM_NULL;

BaseLog::BaseLog(const PdArgs& args)
    : BaseObject(args)
    , mode_(nullptr)
    , error_(nullptr)
    , post_(nullptr)
    , debug_(nullptr)
    , verbose_(nullptr)
    , stdout_(nullptr)
    , stderr_(nullptr)
    , null_(nullptr)
    , prefix_(nullptr)
{
    mode_ = new SymbolEnumProperty("@mode", { SYM_DEBUG, SYM_ERROR, SYM_POST, SYM_VERBOSE, SYM_STDOUT, SYM_STDERR, SYM_NULL });
    addProperty(mode_);

    error_ = new SymbolEnumAlias("@error", mode_, SYM_ERROR);
    addProperty(error_);

    post_ = new SymbolEnumAlias("@post", mode_, SYM_POST);
    addProperty(post_);

    debug_ = new SymbolEnumAlias("@debug", mode_, SYM_DEBUG);
    addProperty(debug_);

    verbose_ = new SymbolEnumAlias("@verbose", mode_, SYM_VERBOSE);
    addProperty(verbose_);

    stdout_ = new SymbolEnumAlias("@stdout", mode_, SYM_STDOUT);
    addProperty(stdout_);

    stderr_ = new SymbolEnumAlias("@stderr", mode_, SYM_STDERR);
    addProperty(stderr_);

    null_ = new SymbolEnumAlias("@null", mode_, SYM_NULL);
    addProperty(null_);

    prefix_ = new ListProperty("@prefix");
    prefix_->setArgIndex(0);
    addProperty(prefix_);
}

void BaseLog::onBang()
{
    const auto m = mode_->value();
    const auto v = s_bang.s_name;

#define OUT(os, v)                                    \
    {                                                 \
        if (!prefix_->value().empty())                \
            os << to_string(prefix_->value()) << ' '; \
                                                      \
        os << v;                                      \
    }

    if (m == SYM_ERROR) {
        Error log(this);
        OUT(log.stream(), v);
    } else if (m == SYM_POST) {
        Post log(this);
        OUT(log.stream(), v);
    } else if (m == SYM_DEBUG) {
        Debug log(this);
        OUT(log.stream(), v);
    } else if (m == SYM_VERBOSE) {
        Log log(this);
        OUT(log.stream(), v);
    } else if (m == SYM_STDERR)
        OUT(std::cerr, v << std::endl)
    else if (m == SYM_STDOUT)
        OUT(std::cout, v << std::endl);
}

void BaseLog::onFloat(t_float f)
{
    const auto m = mode_->value();

    if (m == SYM_ERROR) {
        Error log(this);
        OUT(log.stream(), f);
    } else if (m == SYM_POST) {
        Post log(this);
        OUT(log.stream(), f);
    } else if (m == SYM_DEBUG) {
        Debug log(this);
        OUT(log.stream(), f);
    } else if (m == SYM_VERBOSE) {
        Log log(this);
        OUT(log.stream(), f);
    } else if (m == SYM_STDERR)
        OUT(std::cerr, f << std::endl)
    else if (m == SYM_STDOUT)
        OUT(std::cout, f << std::endl);
}

void BaseLog::onSymbol(t_symbol* s)
{
    const auto m = mode_->value();
    const auto v = s->s_name;

#define OUTS(os, s) OUT(os, "symbol " << s)

    if (m == SYM_ERROR) {
        Error log(this);
        OUTS(log.stream(), v);
    } else if (m == SYM_POST) {
        Post log(this);
        OUTS(log.stream(), v);
    } else if (m == SYM_DEBUG) {
        Debug log(this);
        OUTS(log.stream(), v);
    } else if (m == SYM_VERBOSE) {
        Log log(this);
        OUTS(log.stream(), v);
    } else if (m == SYM_STDERR)
        OUTS(std::cerr, v << std::endl)
    else if (m == SYM_STDOUT)
        OUTS(std::cout, v << std::endl);
}

void BaseLog::onList(const AtomList& l)
{
    const auto m = mode_->value();

#define OUTL(os, l) OUT(os, "list " << l)

    if (m == SYM_ERROR) {
        Error log(this);
        OUT(log.stream(), l);
    } else if (m == SYM_POST) {
        Post log(this);
        OUT(log.stream(), l);
    } else if (m == SYM_DEBUG) {
        Debug log(this);
        OUT(log.stream(), l);
    } else if (m == SYM_VERBOSE) {
        Log log(this);
        OUT(log.stream(), l);
    } else if (m == SYM_STDERR)
        OUT(std::cerr, l << std::endl)
    else if (m == SYM_STDOUT)
        OUT(std::cout, l << std::endl);
}

void BaseLog::onAny(t_symbol* s, const AtomListView& lv)
{
    const auto m = mode_->value();

#define OUTA(os, s, lv) OUT(os, s << ' ' << lv)

    if (m == SYM_ERROR) {
        Error log(this);
        OUTA(log.stream(), s, lv);
    } else if (m == SYM_POST) {
        Post log(this);
        OUTA(log.stream(), s, lv);
    } else if (m == SYM_DEBUG) {
        Debug log(this);
        OUTA(log.stream(), s, lv);
    } else if (m == SYM_VERBOSE) {
        Log log(this);
        OUTA(log.stream(), s, lv);
    } else if (m == SYM_STDERR)
        OUTA(std::cerr, s, lv << std::endl)
    else if (m == SYM_STDOUT)
        OUTA(std::cout, s, lv << std::endl);
}

void setup_base_log()
{
    SYM_ERROR = gensym("error");
    SYM_POST = gensym("post");
    SYM_DEBUG = gensym("debug");
    SYM_VERBOSE = gensym("verbose");
    SYM_STDOUT = gensym("stdout");
    SYM_STDERR = gensym("stderr");
    SYM_NULL = gensym("null");

    ObjectFactory<BaseLog> obj("log");
    obj.noPropsDispatch();
}
