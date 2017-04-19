#include "array_range.h"
#include "ceammc_convert.h"

ArrayRange::ArrayRange(const PdArgs& a)
    : ArrayBase(a)
{
    createOutlet();
}

static Atom time2sec(const Atom& a)
{
    if (a.isSymbol())
        return Atom(convert::time::str2sec(a.asString()));
    else
        return a;
}

void ArrayRange::m_time(t_symbol* s, const AtomList& l)
{
    int sz = arraySize();
    if (sz < 0)
        return;

    listTo(0, l.map(time2sec) * (sys_getsr() / sz));
}

void ArrayRange::m_sec(t_symbol* s, const AtomList& l)
{
    int sz = arraySize();
    if (sz < 0)
        return;

    listTo(0, l * (sys_getsr() / sz));
}

void ArrayRange::m_ms(t_symbol* s, const AtomList& l)
{
    int sz = arraySize();
    if (sz < 0)
        return;

    listTo(0, l * ((sys_getsr() / sz) / 1000));
}

void ArrayRange::m_sample(t_symbol*, const AtomList& l)
{
    int sz = arraySize();
    if (sz < 0)
        return;

    listTo(0, l / sz);
}

extern "C" void setup_array0x2erange()
{
    ObjectFactory<ArrayRange> obj("array.range");
    obj.addMethod("time", &ArrayRange::m_time);
    obj.addMethod("sec", &ArrayRange::m_sec);
    obj.addMethod("ms", &ArrayRange::m_ms);
    obj.addMethod("sample", &ArrayRange::m_sample);
}
