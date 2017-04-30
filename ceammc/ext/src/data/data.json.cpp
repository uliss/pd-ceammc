//#include "ceammc.h"
//#include <m_pd.h>
//
////#include <stdlib.h>
////#include <vector>
////#include "json.hpp"
//
//#include "ceammc_json_objects.hpp"
//
//
//#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
//
////using json = nlohmann::json;
//
//t_class* data_json_class;
//
//
//struct t_json_object {
//    t_object x_obj;
//
////    json *j_j;   //local
//    
//    Document j_jn;
//    GlobalData<Document*> j_g;
//    bool local;
//    
//    t_symbol *j_name;
//
//    t_atom *out1;
//    t_atom *out2;
//
//    t_outlet *outlet1, *outlet2;
//};
//
//
//
//static void json_object_set(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
//{
//    if (argc < 2 )
//        return;
//
//    //json arr = json::array();
//    
//    //Document j;
//    
//    Value arr(kArrayType);
//    Value::AllocatorType& allocator = x->j_jn.GetAllocator();
//    
//    for (int i=1;i<argc;i++)
//    {
//        arr.PushBack(cm_json_from_atom(argv[i]), allocator);
//    }
//
//    //json j = *x->j_j;   //?
//    //j.AddMember(cm_jsonkey_from_atom(&argv[0]).c_str(), arr, allocator);
//    
//   x->j_jn[cm_jsonkey_from_atom(&argv[0]).c_str()] = arr;
//    
//   // x->j_jn = j;
//
//}
//
//static void json_object_get(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
//{
//
//    int l=0;
//
//    //json j = *x->j_j;
//    
//    Value arr(kArrayType);
//    arr = x->j_jn[cm_jsonkey_from_atom(&argv[0]).c_str()];
//    
//    l = (int)arr.Size();
//    
//    free(x->out1);
//    x->out1 = (t_atom*) malloc(sizeof(t_atom)*(l));
//
//    for (int i=0;i<l;i++)
//    {
//        //json j = arr[i];
//        Value *n = &arr[i];
//        x->out1[i] = ncm_atom_from_json_value(*n);
//    }
//
//    outlet_list(x->outlet1, &s_list, l,  x->out1);
//
//}
//
//static void json_object_dump(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
//{
//    int l=0;
//
//    for (json::iterator it = x->j_j->begin(); it != x->j_j->end(); ++it)
//    {
//        json arr = it.value();
//
//        l = (int)arr.size()+1;
//
//        free(x->out1);
//        x->out1 = (t_atom*) malloc(sizeof(t_atom)*(l));;
//
//        x->out1[0] = cm_atom_from_json_string(it.key());
//
//        for (int i=1;i<l;i++)
//        {
//            json j = arr[i-1];
//            x->out1[i] = cm_atom_from_json(j);
//        }
//
//        outlet_list(x->outlet2, &s_list, l,  x->out1);
//    }
//
//}
//
//static void json_object_delete(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
//{
//    x->j_j->erase(cm_jsonkey_from_atom(&argv[0]).c_str());
//}
//
//
//static void json_object_clear(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
//{
//    x->j_j->clear();
//}
//
//static void json_object_setobject(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
//{
//
//    if (argv[0].a_type==A_SYMBOL){
//        std::vector<json_object_ptr>::iterator it;
//        for (it = json_objects.begin(); it != json_objects.end(); ++it)
//        {
//            if (strcmp(it->symbol->s_name, argv[0].a_w.w_symbol->s_name)==0)
//            {
//                x->j_j = it->object;
//                x->j_name = gensym(it->symbol->s_name);
//                break;
//            }
//        }
//        
//        x-> j_g = GlobalData<Document*>(argv[0].a_w.w_symbol->s_name);
//        
//        x->local = false;
//        
//    }
//    else
//    {
//        x->j_j = (json*)malloc(sizeof(json));
//        *x->j_j = json::object();
//        x->j_name = gensym("<local>");
//        
//        x->j_jn = (Document*)malloc(sizeof(Document));
//        
//        x->local = true;
//        
//        //GlobalData<Document> j_n = GlobalData<Document>("<local>");
//
//
//    }
//
//
//}
//
//static void json_object_getobject(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
//{
//        if (x->j_name)
//        {
//            t_atom at;
//            at.a_type = A_SYMBOL;
//            at.a_w.w_symbol = x->j_name;
//            x->out2 = &at;
//            outlet_list(x->outlet1, &s_list, 1,  x->out2);
//        }
//}
//
//
//static void json_object_anything(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
//{
//    if (argc < 1 )
//        return;
//
//    if (strcmp(s->s_name, "get")==0)
//        json_object_get(x,s,argc,argv);
//    if (strcmp(s->s_name, "set")==0)
//        json_object_set(x,s,argc,argv);
//    if (strcmp(s->s_name, "delete")==0)
//        json_object_delete(x,s,argc,argv);
//    if (strcmp(s->s_name, "setobject")==0)
//        json_object_setobject(x,s,argc,argv);
//
//}
//
//static void json_object_objectlist(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
//{
//    int l=0;
//
//    std::vector<json_object_ptr>::iterator it;
//
//    l = (int)json_objects.size()+1;
//    free(x->out1);
//    x->out1 = (t_atom*) malloc(sizeof(t_atom)*(l));;
//
//    t_atom hdr;
//    hdr.a_type = A_SYMBOL;
//    hdr.a_w.w_symbol = gensym("objectlist");
//    x->out1[0] = hdr;
//    int i=1;
//    
//    std::vector<std::string> obj_names;
//    obj_names = x->j_g.getKeys();
//
//    for (it = json_objects.begin(); it != json_objects.end(); ++it)
//    {
//        t_atom sym;
//        sym.a_type = A_SYMBOL;
//        sym.a_w.w_symbol = it->symbol;
//        x->out1[i] = sym;
//        i++;
//    }
//    outlet_list(x->outlet2, &s_list, l,  x->out1);
//}
//
//
//static void* json_object_new(t_symbol *s, int argc, t_atom *argv)
//{
//    t_json_object* x = reinterpret_cast<t_json_object*>(pd_new(data_json_class));
//
//    printf("argc %i\n",argc);
//
//    if (argc>0)
//    {
//        if (argv[0].a_type==A_SYMBOL){
//            std::vector<json_object_ptr>::iterator it;
//            for (it = json_objects.begin(); it != json_objects.end(); ++it)
//            {
//                if (strcmp(it->symbol->s_name, argv[0].a_w.w_symbol->s_name)==0)
//                {
//                    x->j_j = it->object;
//                    break;
//                }
//            }
//
//            if (it == json_objects.end())
//            {
//                x->j_j = (json*)malloc(sizeof(json));
//                *x->j_j = json::object();
//
//                json_object_ptr obj1;
//                obj1.symbol = gensym(argv[0].a_w.w_symbol->s_name);
//                obj1.object = x->j_j;
//                json_objects.push_back(obj1);
//                x->j_name = obj1.symbol;
//
//            }
//        }
//    }
//    else
//    {
//        x->j_j = (json*)malloc(sizeof(json));
//        *x->j_j = json::object();
//        x->j_name = gensym("<local>");
//    }
//
//    x->out1 = (t_atom*)malloc(0);   //dummy
//
//    x->outlet1 = outlet_new(&x->x_obj, &s_list);
//    x->outlet2 = outlet_new(&x->x_obj, &s_list);
//
//    return static_cast<void*>(x);
//}
//
//
//
//extern "C" void setup_json0x2eobject()
//{
//    data_json_class = class_new(gensym("json.object"),
//                                  reinterpret_cast<t_newmethod>(json_object_new),
//                                  reinterpret_cast<t_method>(0),
//                                  sizeof(t_json_object), CLASS_PATCHABLE, A_GIMME,0);
//    class_addanything(data_json_class, json_object_anything);
//
//    class_addmethod(data_json_class, reinterpret_cast<t_method>(json_object_clear), gensym("clear"), A_NULL);
//    class_addmethod(data_json_class, reinterpret_cast<t_method>(json_object_dump), gensym("dump"), A_NULL);
//    class_addmethod(data_json_class, reinterpret_cast<t_method>(json_object_getobject), gensym("getobject"), A_NULL);
//    class_addmethod(data_json_class, reinterpret_cast<t_method>(json_object_setobject), gensym("setobject"), A_GIMME);
//    class_addmethod(data_json_class, reinterpret_cast<t_method>(json_object_objectlist), gensym("objectlist"), A_GIMME);
//
//
//}
