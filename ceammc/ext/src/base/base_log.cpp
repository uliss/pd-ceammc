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

#include <algorithm>
#include <array>

static t_symbol* SYM_ERROR;
static t_symbol* SYM_POST;
static t_symbol* SYM_DEBUG;
static t_symbol* SYM_VERBOSE;
static t_symbol* SYM_STDOUT;
static t_symbol* SYM_STDERR;
static std::array<t_symbol*, 6> sym_modes;

#define OUT(os, v)                                    \
    do {                                              \
        if (!prefix_->value().empty())                \
            os << to_string(prefix_->value()) << ' '; \
                                                      \
        os << v;                                      \
    } while (0)

#define OUT_DO(obj, v)        \
    do {                      \
        obj log(this);        \
        OUT(log.stream(), v); \
    } while (0)

#define OUT_ERROR(v) OUT_DO(Error, v)
#define OUT_POST(v) OUT_DO(Post, v)
#define OUT_DEBUG(v) OUT_DO(Debug, v)
#define OUT_VERBOSE(v) OUT_DO(Log, v)

#define LOG_VALUE(v)                        \
    do {                                    \
        switch (mode_) {                    \
        case LOG_ERROR:                     \
            OUT_ERROR(v);                   \
            break;                          \
        case LOG_POST:                      \
            OUT_POST(v);                    \
            break;                          \
        case LOG_DEBUG: {                   \
            Debug log(this);                \
            OUT(log.stream(), v);           \
        } break;                            \
        case LOG_VERBOSE:                   \
            OUT_VERBOSE(v);                 \
            break;                          \
        case LOG_STDERR: {                  \
            OUT(std::cerr, v << std::endl); \
        } break;                            \
        case LOG_STDOUT: {                  \
            OUT(std::cout, v << std::endl); \
        } break;                            \
        default:                            \
            break;                          \
        }                                   \
    } while (0)

BaseLog::BaseLog(const PdArgs& args)
    : BaseObject(args)
    , active_(nullptr)
{
    createInlet();

    active_ = new BoolProperty("@active", true);
    addProperty(active_);

    prefix_ = new ListProperty("@prefix");
    prefix_->setArgIndex(0);
    addProperty(prefix_);

    for (size_t i = 0; i < sym_modes.size(); i++) {
        if (sym_modes[i] == args.creationName) {
            mode_ = static_cast<Mode>(i);
            break;
        }
    }
}

void BaseLog::onBang()
{
    if (!active_->value())
        return;

    const auto v = s_bang.s_name;

    LOG_VALUE(v);
}

void BaseLog::onFloat(t_float f)
{
    if (!active_->value())
        return;

    LOG_VALUE(f);
}

void BaseLog::onSymbol(t_symbol* s)
{
    if (!active_->value())
        return;

    const auto v = s->s_name;

    LOG_VALUE("symbol " << v);
}

void BaseLog::onList(const AtomList& l)
{
    if (!active_->value())
        return;

    LOG_VALUE("list " << to_string(l));
}

void BaseLog::onAny(t_symbol* s, const AtomListView& lv)
{
    if (!active_->value())
        return;

    LOG_VALUE(s->s_name << ' ' << to_string(lv));
}

void BaseLog::onInlet(size_t n, const AtomList& l)
{
    active_->set(l);
}

void setup_base_log()
{
    SYM_ERROR = gensym("log.error");
    SYM_POST = gensym("log.post");
    SYM_DEBUG = gensym("log.debug");
    SYM_VERBOSE = gensym("log.verbose");
    SYM_STDOUT = gensym("log.stdout");
    SYM_STDERR = gensym("log.stderr");

    sym_modes = {
        SYM_ERROR,
        SYM_POST,
        SYM_DEBUG,
        SYM_VERBOSE,
        SYM_STDOUT,
        SYM_STDERR
    };

    ObjectFactory<BaseLog> obj("logger");
    obj.addAlias("log.error");
    obj.addAlias("log.post");
    obj.addAlias("log.debug");
    obj.addAlias("log.verbose");
    obj.addAlias("log.stdout");
    obj.addAlias("log.stderr");
    obj.noPropsDispatch();
    obj.setXletsInfo({ "input", "1: to enable logging, 0 to disable" }, {});
}
