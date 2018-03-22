#ifndef UI_EXTERNAL_TEST_H
#define UI_EXTERNAL_TEST_H

#include "ceammc_atomlist.h"
#include "ceammc_dsp_ui.h"
#include "ceammc_format.h"
#include "ceammc_pd.h"
#include "ceammc_ui.h"
#include "ceammc_ui_object.h"
#include "common_external_test.h"
#include "test_datatypes.h"
#include "test_external.h"

#include "catch.hpp"
#include "m_pd.h"

#include <map>

using namespace ceammc;

// clang-format off
#define UI_COMPLETE_TEST_SETUP(T)                                                   \
    using namespace ceammc;                                                         \
    typedef TestUIPtr<UI##T, UIObjectFactory<UI##T> > Test##T;                      \
    typedef TestPdExternal<UI##T> TestExt##T;                                       \
    static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");     \
    static void ui_test_init()                                                      \
    {                                                                               \
        pd_init();                                                                  \
        UI##T::setup();                                                             \
        ExternalOutput::setup();                                                    \
        ListenerExternal::setup();                                                  \
    }

#define UI_COMPLETE_DSP_TEST_SETUP(T)                                               \
    using namespace ceammc;                                                         \
    typedef TestUIPtr<UI##T, UIDspFactory<UI##T> > Test##T;                         \
    typedef TestPdExternal<UI##T> TestExt##T;                                       \
    static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");     \
    static void ui_test_init()                                                      \
    {                                                                               \
        pd_init();                                                                  \
        UI##T::setup();                                                             \
        ExternalOutput::setup();                                                    \
        ListenerExternal::setup();                                                  \
    }

template <class T, class F = UIObjectFactory<T> >
// clang-format on
class TestUIPtr {
    T* obj;

public:
    TestUIPtr(const char* name, const AtomList& args = AtomList())
    {
        obj = F::alloc(gensym(name), args.size(), args.toPdData());
        REQUIRE(obj);
    }

    ~TestUIPtr()
    {
        pd_free(obj->asPd());
    }

    T* operator->()
    {
        return obj;
    }

    void send(float f)
    {
        pd_float(obj->asPd(), f);
    }

    void call(const char* method, const AtomList& l = AtomList())
    {
        pd_typedmess(obj->asPd(), gensym(method), l.size(), l.toPdData());
    }

    void call(const char* method, float f)
    {
        t_atom argv;
        atom_setfloat(&argv, f);
        pd_typedmess(obj->asPd(), gensym(method), 1, &argv);
    }

    void mouseDown(float x, float y, long modifiers = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        obj->onMouseDown(0, p, modifiers);
    }

    void mouseUp(float x, float y, long modifiers = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        obj->onMouseUp(0, p, modifiers);
    }

    void mouseMove(float x, float y, long modifiers = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        obj->onMouseMove(0, p, modifiers);
    }

    void mouseDrag(float x, float y, long modifiers = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        obj->onMouseDrag(0, p, modifiers);
    }

    void doubleClick(float x, float y, long modifiers = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        obj->onDblClick(0, p, modifiers);
    }
};

class PropertySetter {
    mutable t_symbol* name_;
    AtomList value_;

public:
    PropertySetter(const char* s, const AtomList& v)
        : name_(gensym(s + 1))
        , value_(v)
    {
    }

    t_symbol* name() const
    {
        return name_;
    }

    const AtomList& value() const
    {
        return value_;
    }

    static PropertySetter create(const char* name, t_float f)
    {
        return PropertySetter(name, AtomList(Atom(f)));
    }

    static PropertySetter create(const char* name, t_symbol* s)
    {
        return PropertySetter(name, AtomList(Atom(s)));
    }

    static PropertySetter create(const char* name, const char* s)
    {
        return PropertySetter(name, AtomList(Atom(gensym(s))));
    }

    static PropertySetter create(const char* name, const AtomList& lst)
    {
        return PropertySetter(name, lst);
    }
};

template <class A>
PropertySetter $1(const char* n, A v) { return PropertySetter::create(n, v); }
template <class A, class B>
PropertySetter $2(const char* n, A a, B b) { return PropertySetter::create(n, L2(a, b)); }

template <class T>
class TestPdExternal : private pd::External {
    typedef std::map<t_symbol*, ListenerExternal*> ListenerMap;

    std::vector<ExternalOutput*> outs_;
    ListenerMap listeners_;

public:
    TestPdExternal(const char* name, const AtomList& args = AtomList())
        : pd::External(name, args)
    {
        REQUIRE(object());

        for (size_t i = 0; i < numOutlets(); i++) {
            ExternalOutput* e = new ExternalOutput;
            connectTo(i, e->object(), 0);
            outs_.push_back(e);
        }
    }

    T* operator->()
    {
        return (T*)object();
    }

    void mouseDown(float x, float y, long mod = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        ((T*)object())->onMouseDown(0, p, mod);
    }

    void mouseUp(float x, float y, long mod = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        ((T*)object())->onMouseUp(0, p, mod);
    }

    void mouseMove(float x, float y, long mod = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        ((T*)object())->onMouseMove(0, p, mod);
    }

    void mouseDrag(float x, float y, long mod = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        ((T*)object())->onMouseDrag(0, p, mod);
    }

    void doubleClick(float x, float y, long mod = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        ((T*)object())->onDblClick(0, p, mod);
    }

    void call(const char* method, const AtomList& l = AtomList())
    {
        clearAll();
        sendMessage(gensym(method), l);
    }

    void call(t_symbol* method, const AtomList& l = AtomList())
    {
        clearAll();
        sendMessage(method, l);
    }

    void bang()
    {
        clearAll();
        External::bang();
    }

    void call(const char* method, float f)
    {
        clearAll();
        sendMessage(gensym(method), AtomList(Atom(f)));
    }

    void send(float f)
    {
        clearAll();
        sendFloat(f);
    }

    void send(t_symbol* s)
    {
        clearAll();
        sendSymbol(s);
    }

    void send(const char* s)
    {
        send(gensym(s));
    }

    void send(const AtomList& lst)
    {
        clearAll();
        sendList(lst);
    }

    void send(const AbstractData& data)
    {
        clearAll();
        DataPtr ptr(data.clone());
        sendList(AtomList(ptr.asAtom()));
    }

    ListenerExternal* addListener(const char* l)
    {
        return addListener(gensym(l));
    }

    ListenerExternal* addListener(t_symbol* addr)
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it != listeners_.end())
            return it->second;

        ListenerExternal* l = new ListenerExternal(addr);
        listeners_[addr] = l;
        return l;
    }

    bool hasListener(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        return it != listeners_.end();
    }

    void listenerReset(t_symbol* addr)
    {
        if (!hasListener(addr))
            return;

        listeners_[addr]->reset();
    }

    bool bangWasSentTo(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it == listeners_.end())
            return false;

        return it->second->msg().isBang();
    }

    bool floatWasSentTo(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it == listeners_.end())
            return false;

        return it->second->msg().isFloat();
    }

    bool symbolWasSentTo(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it == listeners_.end())
            return false;

        return it->second->msg().isSymbol();
    }

    bool listWasSentTo(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it == listeners_.end())
            return false;

        return it->second->msg().isList();
    }

    bool anyWasSentTo(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it == listeners_.end())
            return false;

        return it->second->msg().isAny();
    }

    bool noneWasSentTo(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it == listeners_.end())
            return true;

        return it->second->msg().isNone();
    }

    bool dataWasSentTo(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it == listeners_.end())
            return false;

        return it->second->msg().isData();
    }

    t_float lastSentFloat(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it == listeners_.end())
            return std::numeric_limits<t_float>::min();

        return it->second->msg().atomValue().asFloat();
    }

    t_symbol* lastSentSymbol(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it == listeners_.end())
            return &s_;

        return it->second->msg().atomValue().asSymbol();
    }

    AtomList lastSentList(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it == listeners_.end())
            return AtomList();

        return it->second->msg().listValue();
    }

    AtomList lastSentAny(t_symbol* addr) const
    {
        ListenerMap::const_iterator it = listeners_.find(addr);
        if (it == listeners_.end())
            return AtomList();

        return it->second->msg().anyValue();
    }

    bool hasOutputAt(size_t n) const
    {
        return !outs_.at(n)->msg().isNone();
    }

    bool hasOutput() const
    {
        for (size_t i = 0; i < outs_.size(); i++) {
            if (!outs_.at(i)->msg().isNone())
                return true;
        }

        return false;
    }

    bool isOutputListAt(size_t n) const
    {
        return outs_.at(n)->msg().isList();
    }

    bool isOutputAnyAt(size_t n) const
    {
        return outs_.at(n)->msg().isAny();
    }

    bool isOutputFloatAt(size_t n) const
    {
        return outs_.at(n)->msg().isFloat();
    }

    bool isOutputSymbolAt(size_t n) const
    {
        return outs_.at(n)->msg().isSymbol();
    }

    bool isOutputBangAt(size_t n) const
    {
        return outs_.at(n)->msg().isBang();
    }

    AtomList outputListAt(size_t n) const
    {
        return outs_.at(n)->msg().listValue();
    }

    AtomList outputAnyAt(size_t n) const
    {
        return outs_.at(n)->msg().anyValue();
    }

    float outputFloatAt(size_t n) const
    {
        return outs_.at(n)->msg().atomValue().asFloat();
    }

    Atom outputAtomAt(size_t n) const
    {
        return outs_.at(n)->msg().atomValue();
    }

    t_symbol* outputSymbolAt(size_t n) const
    {
        return outs_.at(n)->msg().atomValue().asString();
    }

    t_float floatProperty(const char* name) const
    {
        TestPdExternal<T>* t = (TestPdExternal<T>*)this;
        const T* obj = (const T*)(t->object());
        t_float res = std::numeric_limits<float>::min();
        obj->getProperty(gensym(name), res);
        return res;
    }

    AtomList listProperty(const char* name) const
    {
        TestPdExternal<T>* t = (TestPdExternal<T>*)this;
        const T* obj = (const T*)(t->object());
        AtomList res;
        obj->getProperty(gensym(name), res);
        return res;
    }

    void clearAll()
    {
        for (size_t i = 0; i < outs_.size(); i++)
            outs_[i]->reset();
    }

    void clearAt(size_t n)
    {
        outs_.at(n)->reset();
    }

    TestPdExternal& operator<<(const BANG__&)
    {
        bang();
        return *this;
    }

    TestPdExternal& operator<<(const char* s)
    {
        send(gensym(s));
        return *this;
    }

    TestPdExternal& operator<<(t_float f)
    {
        send(f);
        return *this;
    }

    TestPdExternal& operator<<(t_symbol* s)
    {
        send(s);
        return *this;
    }

    TestPdExternal& operator<<(const AtomList& lst)
    {
        send(lst);
        return *this;
    }

    TestPdExternal& operator<<=(const AtomList& lst)
    {
        call(lst[0].asSymbol(), lst.slice(1));
        return *this;
    }

    TestPdExternal& operator<<(const PropertySetter& p)
    {
        operator->()->setProperty(p.name(), p.value());
        return *this;
    }

    AtomList operator>>(const char* name)
    {
        t_symbol* p = gensym(name + 1);
        AtomList res;
        operator->()->getProperty(p, res);
        return res;
    }

    ~TestPdExternal()
    {
        for (size_t i = 0; i < outs_.size(); i++)
            delete outs_[i];

        ListenerMap::iterator it = listeners_.begin();
        for (; it != listeners_.end(); ++it)
            delete it->second;
    }
};

#define HAS_PROPERTY(t, name) REQUIRE(t->hasProperty(gensym(name)))
#define HAS_UI_PROPERTY(t, name) REQUIRE(t->hasProperty(gensym(name)))

#define REQUIRE_UI_FLOAT_PROPERTY(t, name, f)     \
    {                                             \
        float p;                                  \
        REQUIRE(t->getProperty(gensym(name), p)); \
        REQUIRE(p == Approx(f));                  \
    }

#define REQUIRE_UI_LIST_PROPERTY(t, name, l)      \
    {                                             \
        AtomList p;                               \
        REQUIRE(t->getProperty(gensym(name), p)); \
        REQUIRE(p == l);                          \
    }

#define REQUIRE_PRESETS(t)                \
    {                                     \
        HAS_UI_PROPERTY(t, "presetname"); \
    }

#define REQUIRE_NO_OUTPUT_AT(t, n)       \
    {                                    \
        REQUIRE_FALSE(t.hasOutputAt(n)); \
    }

#define REQUIRE_NO_OUTPUT(t)          \
    {                                 \
        REQUIRE_FALSE(t.hasOutput()); \
    }

#define REQUIRE_OUTPUT_BANG(t, n)     \
    {                                 \
        REQUIRE(t.hasOutputAt(n));    \
        REQUIRE(t.isOutputBangAt(n)); \
        t.clearAt(n);                 \
    }

#define REQUIRE_OUTPUT_FLOAT(t, n, f)     \
    {                                     \
        REQUIRE(t.hasOutputAt(n));        \
        REQUIRE(t.isOutputFloatAt(n));    \
        REQUIRE(t.outputFloatAt(n) == f); \
        t.clearAt(n);                     \
    }

#define REQUIRE_OUTPUT_LIST(t, n, l)     \
    {                                    \
        REQUIRE(t.hasOutputAt(n));       \
        REQUIRE(t.isOutputListAt(n));    \
        REQUIRE(t.outputListAt(n) == l); \
        t.clearAt(n);                    \
    }

#define REQUIRE_OUTPUT_ANY(t, n, l)     \
    {                                   \
        REQUIRE(t.hasOutputAt(n));      \
        REQUIRE(t.isOutputAnyAt(n));    \
        REQUIRE(t.outputAnyAt(n) == l); \
        t.clearAt(n);                   \
    }

static t_symbol* char_or_symbol(t_symbol* s)
{
    return s;
}

static t_symbol* char_or_symbol(const char* s)
{
    return gensym(s);
}

#define REQUIRE_NONE_WAS_SEND(t, addr)           \
    {                                            \
        t_symbol* s_addr = char_or_symbol(addr); \
        REQUIRE(t.hasListener(s_addr));          \
        REQUIRE(t.noneWasSentTo(s_addr));        \
    }

#define REQUIRE_BANG_WAS_SEND(t, addr)           \
    {                                            \
        t_symbol* s_addr = char_or_symbol(addr); \
        REQUIRE(t.hasListener(s_addr));          \
        REQUIRE(t.bangWasSentTo(s_addr));        \
        t.listenerReset(s_addr);                 \
    }

#define REQUIRE_FLOAT_WAS_SEND(t, addr, f)             \
    {                                                  \
        t_symbol* s_addr = char_or_symbol(addr);       \
        REQUIRE(t.hasListener(s_addr));                \
        REQUIRE(t.floatWasSentTo(s_addr));             \
        REQUIRE(t.lastSentFloat(s_addr) == Approx(f)); \
        t.listenerReset(s_addr);                       \
    }

#define REQUIRE_SYMBOL_WAS_SEND(t, addr, s)      \
    {                                            \
        t_symbol* s_addr = char_or_symbol(addr); \
        REQUIRE(t.hasListener(s_addr));          \
        REQUIRE(t.symbolWasSentTo(s_addr));      \
        REQUIRE(t.lastSentSymbol(s_addr) == s);  \
        t.listenerReset(s_addr);                 \
    }

#define REQUIRE_LIST_WAS_SEND(t, addr, lst)      \
    {                                            \
        t_symbol* s_addr = char_or_symbol(addr); \
        REQUIRE(t.hasListener(s_addr));          \
        REQUIRE(t.listWasSentTo(s_addr));        \
        REQUIRE(t.lastSentList(s_addr) == lst);  \
        t.listenerReset(s_addr);                 \
    }

#define REQUIRE_ANY_WAS_SEND(t, addr, lst)       \
    {                                            \
        t_symbol* s_addr = char_or_symbol(addr); \
        REQUIRE(t.hasListener(s_addr));          \
        REQUIRE(t.anyWasSentTo(s_addr));         \
        REQUIRE(t.lastSentAny(s_addr) == lst);   \
        t.listenerReset(s_addr);                 \
    }

#endif // UI_EXTERNAL_TEST_H
