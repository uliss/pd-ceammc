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
#include "ceammc_pd.h"
#include "ceammc_atomlist.h"
#include "ceammc_externals.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_platform.h"

#include "m_pd.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
void pd_init();
void obj_sendinlet(t_object* x, int n, t_symbol* s, int argc, t_atom* argv);
}

#include <exception>
#include <iostream>

typedef t_object* (*t_newempty)();
typedef t_object* (*t_newfloat)(t_float);
typedef t_object* (*t_newsymbol)(t_symbol*);
typedef t_object* (*t_newgimme)(t_symbol* s, int argc, t_atom* argv);

using namespace ceammc;

static t_listmethod old_print_mlist = nullptr;

static void print_list_replace(t_pd* pd, t_symbol* s, int argc, t_atom* argv)
{
    if (!old_print_mlist)
        return;

    bool contains_data = false;
    for (int i = 0; i < argc; i++) {
        if (Atom::is_data(argv[i])) {
            contains_data = true;
            break;
        }
    }

    if (contains_data) {
        constexpr size_t T = MAXPDSTRING - 25;
        auto str = to_string(AtomList(argc, argv));
        const size_t N = (str.size() / T) + 1;
        startpost("[data]");

        for (size_t i = 0; i < N; i++) {
            poststring(str.substr(i * T, T).c_str());
        }

        endpost();
    } else
        old_print_mlist(pd, s, argc, argv);
}

bool ceammc::pd::addPdPrintDataSupport()
{
    pd::External p("print");
    if (!p.object())
        return false;

    t_class* print_class = p.object()->te_g.g_pd;
    if (!print_class)
        return false;

    // save old callback
    old_print_mlist = print_class->c_listmethod;
    print_class->c_listmethod = print_list_replace;
    return true;
}

std::vector<std::string> pd::currentListOfExternals()
{
    std::vector<std::string> res;
    t_methodentry* m = pd_objectmaker->c_methods;
    if (!m)
        return res;

    res.reserve(pd_objectmaker->c_nmethod);

    for (int i = 0; i < pd_objectmaker->c_nmethod; i++)
        res.push_back(m[i].me_name->s_name);

    return res;
}

pd::External::External(const char* name, const AtomList& lst)
    : obj_(nullptr)
    , parent_(nullptr)
{
    try {
        t_symbol* OBJ_NAME = gensym(name);
        pd_typedmess(&pd_objectmaker, OBJ_NAME, int(lst.size()), lst.toPdData());

        t_pd* ptr = pd_newest();
        if (!ptr) {
            printf("object creation failed: [%s]\n", name);
            return;
        }

        t_object* res = pd_checkobject(ptr);
        if (!res) {
            printf("invalid object: [%s]\n", name);
            return;
        }

        obj_ = res;
    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << " while object creation [" << name << "]" << std::endl;
        obj_ = 0;
    }
}

pd::External::~External()
{
    if (obj_) {
        if (parent_) {
            t_gobj* y = &obj_->te_g;

            if (parent_->gl_list == y) {
                parent_->gl_list = y->g_next;
            } else {
                for (auto g = parent_->gl_list; g; g = g->g_next) {
                    if (g->g_next == y) {
                        g->g_next = y->g_next;
                        break;
                    }
                }
            }
        }

        pd_free(&obj_->te_g.g_pd);
    }
}

bool pd::External::isNull() const
{
    return obj_ == 0;
}

t_symbol* pd::External::className() const
{
    if (isNull())
        return gensym("NULL");

    return obj_->te_g.g_pd->c_name;
}

bool pd::External::connectTo(size_t outn, t_object* dest, size_t inln)
{
    if (!obj_) {
        printf("[connectTo] NULL object\n");
        return false;
    }

#define OBJ_NAME(obj) obj->te_g.g_pd->c_name->s_name

    if (obj_ == dest) {
        printf("[%s: connectTo] self-connect\n", OBJ_NAME(obj_));
        return false;
    }

    if (!dest) {
        printf("[%s: connectTo] NULL destination\n", OBJ_NAME(obj_));
        return false;
    }

    if (int(inln) >= obj_ninlets(dest)) {
        printf("[%s: connectTo %s] invalid destination inlet: %d\n",
            OBJ_NAME(obj_), OBJ_NAME(dest), int(inln));
        return false;
    }

    if (int(outn) >= numOutlets()) {
        printf("[%s: connectTo %s] invalid source outlet: %d\n",
            OBJ_NAME(obj_), OBJ_NAME(dest), int(outn));
        return false;
    }

#undef OBJ_NAME

    return obj_connect(obj_, int(outn), dest, int(inln)) != 0;
}

bool pd::External::connectTo(size_t outn, pd::External& ext, size_t inln)
{
    return connectTo(outn, ext.object(), inln);
}

bool pd::External::connectFrom(size_t outn, t_object* src, size_t inln)
{
    if (!obj_) {
        printf("[connectFrom] NULL object\n");
        return false;
    }

#define OBJ_NAME(obj) obj->te_g.g_pd->c_name->s_name

    if (obj_ == src) {
        printf("[%s: connectFrom] self-connect\n", OBJ_NAME(obj_));
        return false;
    }

    if (!src) {
        printf("[%s: connectFrom] NULL source\n", OBJ_NAME(obj_));
        return false;
    }

    if (int(inln) >= numInlets()) {
        printf("[%s: connectFrom %s] invalid destination inlet: %d\n",
            OBJ_NAME(obj_), OBJ_NAME(src), int(inln));
        return false;
    }

    if (int(outn) >= obj_noutlets(src)) {
        printf("[%s: connectFrom %s] invalid source outlet: %d\n",
            OBJ_NAME(obj_), OBJ_NAME(src), int(outn));
        return false;
    }

#undef OBJ_NAME

    return obj_connect(src, int(outn), obj_, int(inln)) != 0;
}

bool pd::External::connectFrom(size_t outn, pd::External& ext, size_t inln)
{
    return connectFrom(outn, ext.object(), inln);
}

t_object* pd::External::object()
{
    return obj_;
}

const t_object* pd::External::object() const
{
    return obj_;
}

void pd::External::setParent(t_canvas* cnv)
{
    parent_ = cnv;
}

void pd::External::sendBang()
{
    if (!obj_)
        return;

    pd_bang(pd());
}

void pd::External::sendFloat(t_float v)
{
    if (!obj_)
        return;

    pd_float(pd(), v);
}

void pd::External::sendSymbol(t_symbol* s)
{
    if (!obj_)
        return;

    pd_symbol(pd(), s);
}

void pd::External::sendList(const AtomList& l)
{
    if (!obj_)
        return;

    pd_list(pd(), &s_list, int(l.size()), l.toPdData());
}

void pd::External::sendBangTo(size_t inlet)
{
    if (inlet == 0)
        sendBang();
    else {
        External pd_b("bang");
        if (pd_b.connectTo(0, *this, inlet))
            pd_bang(pd_b.pd());
    }
}

void pd::External::sendFloatTo(t_float v, size_t inlet)
{
    if (inlet == 0)
        sendFloat(v);
    else {
        External pd_f("float");
        if (pd_f.connectTo(0, *this, inlet))
            pd_float(pd_f.pd(), v);
    }
}

void pd::External::sendSymbolTo(t_symbol* s, size_t inlet)
{
    if (inlet == 0)
        sendSymbol(s);
    else {
        External pd_s("symbol");
        if (pd_s.connectTo(0, *this, inlet))
            pd_symbol(pd_s.pd(), s);
    }
}

void pd::External::sendListTo(const AtomList& l, size_t inlet)
{
    if (inlet == 0)
        sendList(l);
    else {
        External pd_l("list");
        if (pd_l.connectTo(0, *this, inlet))
            pd_list(pd_l.pd(), &s_list, int(l.size()), l.toPdData());
    }
}

void pd::External::sendMessage(t_symbol* msg, const AtomList& args)
{
    if (!obj_)
        return;

    pd_typedmess(&obj_->te_g.g_pd, msg, int(args.size()), args.toPdData());
}

void pd::External::sendMessage(const Message& m)
{
    if (!obj_)
        return;

    if (m.isBang())
        sendBang();
    else if (m.isFloat())
        sendFloat(m.atomValue().asFloat());
    else if (m.isSymbol())
        sendSymbol(m.atomValue().asSymbol());
    else if (m.isList())
        sendList(m.listValue());
    else
        sendMessage(m.atomValue().asSymbol(), m.listValue());
}

void pd::External::sendMessageTo(const Message& m, size_t inlet)
{
    if (!obj_)
        return;

    if (inlet == 0)
        return sendMessage(m);

    if (m.isBang())
        sendBangTo(inlet);
    else if (m.isFloat())
        sendFloatTo(m.atomValue().asFloat(), inlet);
    else if (m.isSymbol())
        sendSymbolTo(m.atomValue().asSymbol(), inlet);
    else if (m.isList())
        sendListTo(m.listValue(), inlet);
    else {
        External pd_a("t", AtomList(gensym("a")));
        if (pd_a.connectTo(0, *this, inlet)) {
            const auto& l = m.listValue();
            pd_anything(pd_a.pd(), m.atomValue().asSymbol(), int(l.size()), l.toPdData());
        }
    }
}

int pd::External::numOutlets() const
{
    if (!obj_)
        return 0;

    return obj_noutlets(obj_);
}

int pd::External::numInlets() const
{
    if (!obj_)
        return 0;

    return obj_ninlets(obj_);
}

pd::XletInfo pd::External::inletInfo(int i) const
{
    if (i < 0 || i >= obj_ninlets(obj_))
        return { XletInfo::NONE };

    return { obj_issignalinlet(obj_, i) ? XletInfo::SIGNAL : XletInfo::CONTROL };
}

pd::XletInfo pd::External::outletInfo(int i) const
{
    if (i < 0 || i >= obj_noutlets(obj_))
        return { XletInfo::NONE };

    return { obj_issignaloutlet(obj_, i) ? XletInfo::SIGNAL : XletInfo::CONTROL };
}

int pd::External::xPos() const
{
    if (!obj_)
        return 0;

    return obj_->te_xpix;
}

int pd::External::yPos() const
{
    if (!obj_)
        return 0;

    return obj_->te_ypix;
}

void pd::External::setXPos(int x)
{
    if (!obj_)
        return;

    obj_->te_xpix = short(x);
}

void pd::External::setYPos(int y)
{
    if (!obj_)
        return;

    obj_->te_ypix = short(y);
}

std::vector<t_symbol*> pd::External::methods() const
{
    std::vector<t_symbol*> res;
    if (!obj_)
        return res;

    t_class* c = obj_->te_g.g_pd;
    for (int i = 0; i < c->c_nmethod; i++) {
        auto m = &c->c_methods[i];
        res.push_back(m->me_name);
    }

    return res;
}

bool pd::External::isCeammc() const
{
    return is_ceammc(obj_);
}

bool pd::External::isCeammcBase() const
{
    return is_ceammc_base(obj_);
}

bool pd::External::isCeammcUI() const
{
    return is_ceammc_ui(obj_);
}

bool pd::External::isCeammcFaust() const
{
    return is_ceammc_faust(obj_);
}

bool pd::External::isCeammcFlext() const
{
    return is_ceammc_flext(obj_);
}

const BaseObject* pd::External::asCeammcBaseObject() const
{
    return ceammc_to_base_object(obj_);
}

const UIObject* pd::External::asCeammcUIObject() const
{
    return ceammc_to_ui_object(obj_);
}

std::vector<PropertyInfo> pd::External::properties() const
{
    if (!isCeammc())
        return {};

    if (isCeammcBase())
        return ceammc_base_properties(obj_);
    else if (isCeammcUI())
        return ceammc_ui_properties(obj_);
    else if (isCeammcFaust())
        return ceammc_faust_properties(obj_);
    else
        return {};
}

PureData::PureData()
{
    if (!pd_objectmaker)
        pd_init();
}

CanvasPtr PureData::createTopCanvas(const char* name, const AtomList& args)
{
    CanvasPtr ptr;

    AtomList l(0.f, 0.f); // x, y
    l.append(Atom(600)); // width
    l.append(Atom(400)); // height
    l.append(Atom(10)); // font size

    auto ccnv = canvas_getcurrent();

    if (ccnv) {
        canvas_unsetcurrent(ccnv);
        LIB_DBG << "after canvas_unsetcurrent(): " << canvas_getcurrent();
    }

    if (platform::is_path_relative(name)) {
        glob_setfilename(0, gensym(name), gensym("~"));
    } else {
        std::string dir = platform::dirname(name);
        std::string fname = platform::basename(name);
        glob_setfilename(0, gensym(fname.c_str()), gensym(dir.c_str()));
    }

    if (!args.empty())
        canvas_setargs(int(args.size()), args.toPdData());

    assert(l.size() == 5);
    t_canvas* cnv = canvas_new(0, gensym(name), int(l.size()), l.toPdData());

    if (!cnv)
        return ptr;

    cnv->gl_loading = 0;

    ptr.reset(new Canvas(cnv));
    return ptr;
}

CanvasPtr PureData::createSubpatch(_glist* parent, const char* name)
{
    t_canvas* cnv = canvas_new(0, gensym(name), 0, nullptr);
    if (!cnv)
        return CanvasPtr();

    cnv->gl_name = gensym(name);
    cnv->gl_owner = parent;

    return CanvasPtr(new Canvas(cnv));
}

CanvasPtr PureData::findCanvas(const char* name)
{
    t_symbol* s = gensym(name);
    for (auto c = pd_getcanvaslist(); c != nullptr; c = c->gl_next) {
        if (c->gl_name == s)
            return CanvasPtr(new Canvas(c));
    }

    return {};
}

PureData& PureData::instance()
{
    static PureData pd;
    return pd;
}

t_class* pd::object_class(t_object* x)
{
    if (!x)
        return nullptr;

    return x->te_g.g_pd;
}

t_symbol* pd::object_name(t_object* x)
{
    if (!x)
        return &s_;

    return x->te_g.g_pd->c_name;
}

t_symbol* pd::object_dir(t_object* x)
{
    if (!x)
        return &s_;

    return x->te_g.g_pd->c_externdir;
}

void pd::object_bang(t_object* x)
{
    if (!x || !x->te_g.g_pd->c_bangmethod)
        return;

    x->te_g.g_pd->c_bangmethod(&x->te_g.g_pd);
}
