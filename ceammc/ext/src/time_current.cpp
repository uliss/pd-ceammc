#include "ceammc.h"
#include <cstring>
#include <ctime>
#include <m_pd.h>

static const size_t BUFSIZE = 200;

t_class* time_current_class;
struct t_time_current {
    t_object x_obj;
    t_outlet* out_fmt;
    t_symbol fmt;
    char buf[BUFSIZE];
};

static void time_current_bang(t_time_current* x)
{
    time_t t = time(0);
    tm* now = localtime(&t);
    t_atom current[3];
    SETFLOAT(current, now->tm_hour);
    SETFLOAT(current + 1, now->tm_min);
    SETFLOAT(current + 2, now->tm_sec);
    outlet_list(x->x_obj.te_outlet, &s_list, 3, current);
    strftime(x->buf, BUFSIZE - 1, "%H:%M:%S", now);

    t_binbuf* bb = binbuf_new();
    binbuf_text(bb, x->buf, strlen(x->buf));
    int ac = binbuf_getnatom(bb);
    t_atom* av = binbuf_getvec(bb);
    if (ac) {
        outlet_symbol(x->out_fmt, atom_getsymbol(av));
    }
    binbuf_free(bb);
}

static void* time_current_new()
{
    t_time_current* x = reinterpret_cast<t_time_current*>(pd_new(time_current_class));
    outlet_new(&x->x_obj, &s_float);
    x->out_fmt = outlet_new(&x->x_obj, &s_symbol);
    return static_cast<void*>(x);
}

static void time_current_free(t_time_current* x)
{
    outlet_free(x->out_fmt);
}

extern "C" void setup_time0x2ecurrent()
{
    time_current_class = class_new(gensym("time.current"),
        reinterpret_cast<t_newmethod>(time_current_new),
        reinterpret_cast<t_method>(time_current_free),
        sizeof(t_time_current), 0, A_NULL);
    class_addbang(time_current_class, time_current_bang);
}
