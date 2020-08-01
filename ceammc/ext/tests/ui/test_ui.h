#ifndef UI_EXTERNAL_TEST_H
#define UI_EXTERNAL_TEST_H

#include "ceammc_atomlist.h"
#include "ceammc_format.h"
#include "ceammc_pd.h"
#include "ceammc_ui.h"
#include "ceammc_ui_object.h"

#include "test_base.h"
#include "test_external.h"

#include "m_pd.h"

#include <map>

using namespace ceammc;

void test_ui_main_init();

// clang-format off
#define UI_COMPLETE_TEST_SETUP(T)                                                   \
    using namespace ceammc;                                                         \
    typedef TestUIPtr<UI##T, UIObjectFactory<UI##T> > Test##T;                      \
    typedef TestUIExternal<UI##T> TestExt##T;                                       \
    static void ui_test_init()                                                      \
    {                                                                               \
        test_ui_main_init();                                                        \
    }

template <class T, class F = UIObjectFactory<T> >
// clang-format on
class TestUIPtr {
    T* obj;

public:
    TestUIPtr(const char* name, const AtomList& args = L())
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

    void call(const char* method, const AtomList& l = L())
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
        obj->onMouseDown(0, p, p, modifiers);
    }

    void mouseUp(float x, float y, long modifiers = 0)
    {
        obj->onMouseUp(0, { x, y }, modifiers);
    }

    void mouseMove(float x, float y, long modifiers = 0)
    {
        obj->onMouseMove(0, { x, y }, modifiers);
    }

    void mouseDrag(float x, float y, long modifiers = 0)
    {
        obj->onMouseDrag(0, { x, y }, modifiers);
    }

    void doubleClick(float x, float y, long modifiers = 0)
    {
        obj->onDblClick(0, { x, y }, modifiers);
    }
};

template <class T>
class TestUIExternal : public TestPdExternal<T> {
    typedef std::map<t_symbol*, ListenerExternal*> ListenerMap;

    std::vector<LogExternalOutput*> outs_;
    ListenerMap listeners_;

public:
    TestUIExternal(const char* name, const AtomList& args = L())
        : TestPdExternal<T>(name, args)
    {
    }

    T* operator->() override
    {
        return (T*)TestPdExternal<T>::pdObject();
    }

    void mouseDown(float x, float y, long mod = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        (*this)->onMouseDown(0, p, p, mod);
    }

    void mouseUp(float x, float y, long mod = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        (*this)->onMouseUp(0, p, mod);
    }

    void mouseMove(float x, float y, long mod = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        (*this)->onMouseMove(0, p, mod);
    }

    void mouseDrag(float x, float y, long mod = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        (*this)->onMouseDrag(0, p, mod);
    }

    void doubleClick(float x, float y, long mod = 0)
    {
        t_pt p;
        p.x = x;
        p.y = y;
        (*this)->onDblClick(0, p, mod);
    }

    void show()
    {
        ebox_wvis((*this)->asGObj(), (*this)->canvas(), 1);
    }

    void hide()
    {
        ebox_wvis((*this)->asGObj(), (*this)->canvas(), 0);
    }
};

#define HAS_PROPERTY(t, name) REQUIRE(t->hasProperty(gensym(name)))
#define HAS_UI_PROPERTY(t, name) REQUIRE(t->hasProperty(gensym(name)))

#define REQUIRE_UI_FLOAT_PROPERTY(t, name, f)     \
    {                                             \
        t_float p;                                \
        REQUIRE(t->getProperty(gensym(name), p)); \
        REQUIRE(p == Approx(f).epsilon(0.0001));  \
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

#define REQUIRE_OUTPUT_FLOAT(t, n, f)             \
    {                                             \
        REQUIRE(t.hasOutputAt(n));                \
        REQUIRE(t.isOutputFloatAt(n));            \
        REQUIRE(t.outputFloatAt(n) == Approx(f)); \
        t.clearAt(n);                             \
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
