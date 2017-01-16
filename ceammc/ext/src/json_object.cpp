#include "ceammc.h"
#include <m_pd.h>

//#include <stdlib.h>
//#include <vector>
//#include "json.hpp"

#include "ceammc_atomlist.h"
#include "ceammc_json_objects.hpp"
#include "ceammc_globaldata.h"

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#define OBJ_NAME "json.object"

typedef ceammc::GlobalData<Document> GlobalJSON;

t_class* json_object_class;


struct t_json_object {
    t_object x_obj;
    
    Document *json_local;
    GlobalJSON *json_global;

    t_outlet *outlet1, *outlet2;
};



static void json_object_set(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2 )
        return;
    
    AtomList list = AtomList(argc,argv);
    
    cmJSONFromAtomList(x->json_local, list);
   
}

static void json_object_get(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{

    if (argc < 1 )
        return;
    
    Atom k = argv[0];
    
    AtomList list =  cmAtomListFromJSON(x->json_local, k.asString());
    
    list.output(x->outlet1);

}

static void json_object_dump(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{
    StringBuffer buffer;
    Writer<StringBuffer, Document::EncodingType, UTF8<> > writer(buffer);
    x->json_local->Accept(writer);
    std::string res = buffer.GetString();
    
    //list.output(x->outlet1);
    post(res.c_str());

}

static void json_object_delete(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{
    
    AtomList list = AtomList(argc,argv);
    
    Document::AllocatorType& allocator = x->json_local->GetAllocator();
    
    Value *vKey = new Value;
    
    vKey->SetString(list.at(0).asString().c_str() , (int)(list.at(0).asString().size()), allocator );
    
    if (x->json_local->HasMember(*vKey))
    {
        x->json_local->RemoveMember(*vKey);
    }
    
}


static void json_object_clear(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{
    x->json_local->SetObject();
}

static void json_object_setobject(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{

    if (argc<1) return;
    Atom a = Atom(argv[0]);
    
    x->json_global = new GlobalJSON(a.asString(), OBJ_NAME);
    x->json_local = &x->json_global->ref();
    //x->json_local->SetObject();
    
}

static void json_object_getobject(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{
    AtomList list;
    Atom a = Atom(gensym(x->json_global->name().c_str()));
    list.append(a);
    list.output(x->outlet1);
}


static void json_object_anything(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 )
        return;

    if (strcmp(s->s_name, "get")==0)
        json_object_get(x,s,argc,argv);
    if (strcmp(s->s_name, "set")==0)
        json_object_set(x,s,argc,argv);
    if (strcmp(s->s_name, "delete")==0)
        json_object_delete(x,s,argc,argv);
//    if (strcmp(s->s_name, "setobject")==0)
//        json_object_setobject(x,s,argc,argv);

}

static void json_object_objectlist(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{
    
}


static void* json_object_new(t_symbol *s, int argc, t_atom *argv)
{
    t_json_object* x = reinterpret_cast<t_json_object*>(pd_new(json_object_class));
    
    x->outlet1 = outlet_new(&x->x_obj, &s_list);
    x->outlet2 = outlet_new(&x->x_obj, &s_list);
    
    t_symbol *id = gensym("local");
    
    if (argc>0)
    {
        Atom a = Atom(argv[0]);
        if (a.isSymbol())
        {
            id = gensym(a.asString().c_str());
        }
    }
    
    x->json_global = new GlobalJSON(id->s_name, OBJ_NAME);
    x->json_local = &x->json_global->ref();
    x->json_local->SetObject();
    

    return static_cast<void*>(x);
}



extern "C" void setup_json0x2eobject()
{
    json_object_class = class_new(gensym(OBJ_NAME),
                                  reinterpret_cast<t_newmethod>(json_object_new),
                                  reinterpret_cast<t_method>(0),
                                  sizeof(t_json_object), CLASS_PATCHABLE, A_GIMME,0);
    class_addanything(json_object_class, json_object_anything);

    class_addmethod(json_object_class, reinterpret_cast<t_method>(json_object_clear), gensym("clear"), A_NULL);
    class_addmethod(json_object_class, reinterpret_cast<t_method>(json_object_dump), gensym("dump"), A_NULL);
    
    class_addmethod(json_object_class, reinterpret_cast<t_method>(json_object_getobject), gensym("getobject"), A_NULL);
    class_addmethod(json_object_class, reinterpret_cast<t_method>(json_object_setobject), gensym("setobject"), A_GIMME);
    
//    class_addmethod(json_object_class, reinterpret_cast<t_method>(json_object_objectlist), gensym("objectlist"), A_GIMME);


}
