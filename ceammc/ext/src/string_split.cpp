#include "ceammc.h"
#include <glib.h>
#include <stdlib.h>
#include <m_pd.h>
#include <string>
#include <vector>
#include <sstream>

t_class* string_split_class;
struct t_string_split {
    t_object x_obj;
    t_symbol *str2;
    t_atom *out1;
    int out1count;
    t_outlet *outlet1;
};


//!
void string_split_one(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}
std::vector<std::string> string_split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    string_split_one(s, delim, elems);
    return elems;
}

static void string_split_symbol(t_string_split* x, t_symbol* s)
{

    std::string str1 = s->s_name;
    std::string del1 = x->str2->s_name;

    std::vector<std::string> vec = string_split(str1, del1.c_str()[0]);

    x->out1count = (int)vec.size();
    x->out1 = (t_atom*)malloc(sizeof(t_atom)*x->out1count);

    std::vector<std::string>::iterator it;
    int i=0;
    for (it = vec.begin(); it != vec.end(); ++it)
    {
        x->out1[i].a_w.w_symbol = gensym(it->c_str());
        x->out1[i].a_type = A_SYMBOL;
        printf("%s\n",it->c_str());
        i++;
    }

    outlet_list(x->outlet1, &s_list, x->out1count, x->out1);
}

static void* string_split_new()
{
    t_string_split* x = reinterpret_cast<t_string_split*>(pd_new(string_split_class));

    symbolinlet_new(&x->x_obj, &x->str2);

    x->str2 = gensym(" ");

    x->outlet1 = outlet_new(&x->x_obj, &s_list);

    x->out1 = (t_atom*)malloc(0);    //dummy
    return static_cast<void*>(x);
}

extern "C" void setup_string0x2esplit()
{
    string_split_class = class_new(gensym("string.split"),
        reinterpret_cast<t_newmethod>(string_split_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_string_split), 0, A_NULL);

    class_addsymbol(string_split_class, string_split_symbol);
}
