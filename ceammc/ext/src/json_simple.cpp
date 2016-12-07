#include "ceammc.h"
#include <m_pd.h>
#include <stdlib.h>
#include "json.hpp"

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

using json = nlohmann::json;

t_class* json_simple_class;
struct t_json_simple {
    t_object x_obj;
    
    json j_j;
    
    t_atom *out1;
    
    t_outlet *outlet1, *outlet2;
};

static inline json from_atom(t_atom atom)
{
    json ret = json::object();
    
    int at1 = (int)atom.a_type;
    ret["type"] = at1;
    ret["symbol"] = atom.a_w.w_symbol->s_name;
    ret["float"] = atom.a_w.w_float;
    
    return ret;
}

static inline t_atom from_json(json in1)
{
    t_atom ret;
    
    ret.a_type = A_SYMBOL;
    ret.a_w.w_symbol = gensym("<>");
    
    if (in1.is_object())
    {
        int at1 = in1["type"];
        ret.a_type = (t_atomtype)at1;
        std::string sym = in1["symbol"];
        ret.a_w.w_symbol = gensym(sym.c_str());
        ret.a_w.w_float = in1["float"];
    }
    
    return ret;
}

static inline t_atom from_json_string(json in1)
{
    t_atom ret;
    
    ret.a_type = A_SYMBOL;
    ret.a_w.w_symbol = gensym("<>");
    
    if (in1.is_string())
    {
        std::string sym = in1;
        ret.a_w.w_symbol = gensym(sym.c_str());
    }
    
    return ret;
}

static inline std::string to_key(t_atom* atom)
{
    std::string key;
    if (atom->a_type == A_FLOAT)
    {
        key = std::to_string((int)atom->a_w.w_float);
    }
    if (atom->a_type == A_SYMBOL)
    {
        key = atom->a_w.w_symbol->s_name;
    }
    return key;
}

static void json_simple_set(t_json_simple* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2 )
        return;
    
    json arr = json::array();
    
    for (int i=1;i<argc;i++)
    {
        arr.push_back(from_atom(argv[i]));
    }
    
    x->j_j[to_key(&argv[0]).c_str()] = arr;
    
}

static void json_simple_get(t_json_simple* x, t_symbol* s, int argc, t_atom* argv)
{
    
    int l=0;
    
    json arr = x->j_j[to_key(&argv[0]).c_str()];
    
    l = (int)arr.size();
    
    free(x->out1);
    x->out1 = (t_atom*) malloc(sizeof(t_atom)*(l));
    
    for (int i=0;i<l;i++)
    {
        json j = arr[i];
        x->out1[i] = from_json(j);
    }
    
    outlet_list(x->outlet1, &s_list, l,  x->out1);
    
}

static void json_simple_dump(t_json_simple* x, t_symbol* s, int argc, t_atom* argv)
{
    int l=0;
    
    for (json::iterator it = x->j_j.begin(); it != x->j_j.end(); ++it)
    {
        json arr = it.value();
        
        l = (int)arr.size()+1;
        
        free(x->out1);
        x->out1 = (t_atom*) malloc(sizeof(t_atom)*(l));;
        
        x->out1[0] = from_json_string(it.key());
        
        for (int i=1;i<l;i++)
        {
            json j = arr[i-1];
            x->out1[i] = from_json(j);
        }
        
        outlet_list(x->outlet2, &s_list, l,  x->out1);
    }
    
}

static void json_simple_delete(t_json_simple* x, t_symbol* s, int argc, t_atom* argv)
{
    x->j_j.erase(to_key(&argv[0]).c_str());
}


static void json_simple_clear(t_json_simple* x, t_symbol* s, int argc, t_atom* argv)
{
    x->j_j.clear();
}

static void json_simple_anything(t_json_simple* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 )
        return;
    
    if (strcmp(s->s_name, "get")==0)
        json_simple_get(x,s,argc,argv);
    if (strcmp(s->s_name, "set")==0)
        json_simple_set(x,s,argc,argv);
    if (strcmp(s->s_name, "delete")==0)
        json_simple_delete(x,s,argc,argv);
    
}


static void* json_simple_new()
{
    t_json_simple* x = reinterpret_cast<t_json_simple*>(pd_new(json_simple_class));
    
    x->j_j = json::object();
    
    x->out1 = (t_atom*)malloc(0);   //dummy
    
    x->outlet1 = outlet_new(&x->x_obj, &s_list);
    x->outlet2 = outlet_new(&x->x_obj, &s_list);
    
    return static_cast<void*>(x);
}

extern "C" void setup_json0x2esimple()
{
    json_simple_class = class_new(gensym("json.simple"),
                                  reinterpret_cast<t_newmethod>(json_simple_new),
                                  reinterpret_cast<t_method>(0),
                                  sizeof(t_json_simple), 0, A_NULL);
    class_addanything(json_simple_class, json_simple_anything);
    
    class_addmethod(json_simple_class, reinterpret_cast<t_method>(json_simple_clear), gensym("clear"), A_NULL);
    class_addmethod(json_simple_class, reinterpret_cast<t_method>(json_simple_dump), gensym("dump"), A_NULL);
    
    
    
}
