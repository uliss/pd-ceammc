#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

t_class* list_wrap_class;
struct t_list_wrap {
    t_object x_obj;

    t_atom *outlist1;
    t_atom *outlist2;

    t_outlet *out1;
    t_outlet *out2;
};


static void list_wrap_list(t_list_wrap* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 )
        return;

    t_atom *temp1 = (t_atom*)malloc(sizeof(t_atom)*argc);
    t_atom *temp2 = (t_atom*)malloc(sizeof(t_atom)*argc);
    t_atom *prev = (t_atom*)malloc(sizeof(t_atom));

    int j=1, k=0;

    for (int i=0;i<argc;i++)
    {
        if (argv[i].a_w.w_float != prev->a_w.w_float)
        {
            j=1;
            temp1[k].a_type = argv[i].a_type;
            temp1[k].a_w.w_float = argv[i].a_w.w_float;
            temp2[k].a_w.w_float = j;
            temp2[k].a_type = A_FLOAT;

            k++;
        }
        else
        {
            j++;
            temp2[k-1].a_w.w_float = j;
        }

        prev->a_w.w_float = argv[i].a_w.w_float;
    }

    free(x->outlist1);
    free(x->outlist2);
    x->outlist1 = (t_atom*)malloc(sizeof(t_atom)*k);
    x->outlist2 = (t_atom*)malloc(sizeof(t_atom)*k);

    // ?
    for (int i =0; i<k; i++)
    {
        x->outlist1[i].a_type = temp1[i].a_type;
        x->outlist2[i].a_type = temp2[i].a_type;
        x->outlist1[i].a_w.w_float = temp1[i].a_w.w_float;
        x->outlist2[i].a_w.w_float = temp2[i].a_w.w_float;
    }

    outlet_list(x->out1, &s_list, k, x->outlist1);
    outlet_list(x->out2, &s_list, k, x->outlist2);

    free(temp1);
    free(temp2);
    free(prev);
    
}

static void* list_wrap_new()
{
    t_list_wrap* x = reinterpret_cast<t_list_wrap*>(pd_new(list_wrap_class));

    x->out1 = outlet_new(&x->x_obj, &s_list);
    x->out2 = outlet_new(&x->x_obj, &s_list);


    x->outlist1 = (t_atom*)malloc(0);    //dummy
    x->outlist2 = (t_atom*)malloc(0);    //dummy

    return static_cast<void*>(x);
}

extern "C" void setup_list0x2ewrap()
{
    list_wrap_class = class_new(gensym("list.wrap"),
                                      reinterpret_cast<t_newmethod>(list_wrap_new),
                                      reinterpret_cast<t_method>(0),
                                      sizeof(t_list_wrap), 0, A_NULL);
    class_addlist(list_wrap_class, list_wrap_list);



}
