#include "ceammc.h"
#include <m_pd.h>

//#include <stdlib.h>
//#include <vector>
//#include "json.hpp"

#include "ceammc_atomlist.h"
#include "ceammc_globaldata.h"
#include "ceammc_json_objects.hpp"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define OBJ_NAME "global.json"

typedef ceammc::GlobalData<Document> GlobalJSON;

static t_class* json_global_object_class;

struct t_json_object {
    t_object x_obj;

    Document* json_local;
    GlobalJSON* json_global;

    t_outlet *outlet1, *outlet2;

    t_canvas* canvas;

    //t_scalar *scalar;
};

//click stub:
//static void textbuf_open(t_textbuf *x)
//{
//    if (x->b_guiconnect)
//    {
//        sys_vgui("wm deiconify .x%lx\n", x);
//        sys_vgui("raise .x%lx\n", x);
//        sys_vgui("focus .x%lx.text\n", x);
//    }
//    else
//    {
//        char buf[40];
//        sys_vgui("pdtk_textwindow_open .x%lx %dx%d {%s: %s} %d\n",
//                 x, 600, 340, "myname", "text",
//                 sys_hostfontsize(glist_getfont(x->b_canvas),
//                                  glist_getzoom(x->b_canvas)));
//        sprintf(buf, ".x%lx", (unsigned long)x);
//        x->b_guiconnect = guiconnect_new(&x->b_ob.ob_pd, gensym(buf));
//        textbuf_senditup(x);
//    }
//}

static void json_object_set(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 2)
        return;

    AtomList list = AtomList(argc, argv);

    cmJSONFromAtomList(x->json_local, list);
}

static void json_object_get(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{

    if (argc < 1)
        return;

    Atom k = argv[0];

    AtomList list = cmAtomListFromJSON(x->json_local, k.asString());

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

    AtomList list = AtomList(argc, argv);

    Document::AllocatorType& allocator = x->json_local->GetAllocator();

    Value* vKey = new Value;

    vKey->SetString(list.at(0).asString().c_str(), (int)(list.at(0).asString().size()), allocator);

    if (x->json_local->HasMember(*vKey)) {
        x->json_local->RemoveMember(*vKey);
    }
}

static void json_object_clear(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{
    x->json_local->SetObject();
}

static void json_object_setobject(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{

    if (argc < 1)
        return;
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
    if (argc < 1)
        return;

    if (strcmp(s->s_name, "get") == 0)
        json_object_get(x, s, argc, argv);
    if (strcmp(s->s_name, "set") == 0)
        json_object_set(x, s, argc, argv);
    if (strcmp(s->s_name, "delete") == 0)
        json_object_delete(x, s, argc, argv);
    //    if (strcmp(s->s_name, "setobject")==0)
    //        json_object_setobject(x,s,argc,argv);
}

static void json_object_objectlist(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{
    //x->json_global->getKeys();
}

// TODO
//static void json_array_create(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
//{
//    x->x_scalar = scalar_new(gl, templatesym);
//}
//void garray_savecontentsto(t_garray *x, t_binbuf *b)
//{
//    if (x->x_saveit)
//    {
//        t_array *array = garray_getarray(x);
//        int n = array->a_n, n2 = 0;
//        if (n > 200000)
//            post("warning: I'm saving an array with %d points!\n", n);
//        while (n2 < n)
//        {
//            int chunk = n - n2, i;
//            if (chunk > ARRAYWRITECHUNKSIZE)
//                chunk = ARRAYWRITECHUNKSIZE;
//            binbuf_addv(b, "si", gensym("#A"), n2);
//            for (i = 0; i < chunk; i++)
//                binbuf_addv(b, "f", ((t_word *)(array->a_vec))[n2+i].w_float);
//            binbuf_addv(b, ";");
//            n2 += chunk;
//        }
//    }
//}

static void json_object_read(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{

    if (argc < 1) {
        error("specify file name");
        return;
    }
    t_binbuf* b;
    b = binbuf_new();

    Atom a = Atom(argv[0]);
    char* filename = (char*)a.asString().c_str();
    binbuf_read_via_canvas(b, filename, x->canvas, 0);

    if (b) {
        char* txt;
        int len;

        binbuf_gettext(b, &txt, &len);

        //todo free
        x->json_local = new Document;
        x->json_local->Parse(txt);
    }
}

static void json_object_write(t_json_object* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1) {
        error("specify file name");
        return;
    }

    StringBuffer buffer;
    Writer<StringBuffer, Document::EncodingType, UTF8<> > writer(buffer);
    x->json_local->Accept(writer);
    std::string res = buffer.GetString();

    //list.output(x->outlet1);
    //    printf(res.c_str());
    //    printf("---\n");

    const char* txt = res.c_str();
    int len = (int)res.length();

    t_binbuf* b = binbuf_new();
    binbuf_text(b, (char*)txt, len);

    Atom a = Atom(argv[0]);
    char* filename = (char*)a.asString().c_str();
    const char* dir = canvas_getdir(x->canvas)->s_name;
    binbuf_write(b, filename, (char*)dir, 0);
}

static void* json_object_new(t_symbol* s, int argc, t_atom* argv)
{
    t_json_object* x = reinterpret_cast<t_json_object*>(pd_new(json_global_object_class));

    x->outlet1 = outlet_new(&x->x_obj, &s_list);
    x->outlet2 = outlet_new(&x->x_obj, &s_list);

    t_symbol* id; // = gensym("local");

    if (argc > 0) {
        Atom a = Atom(argv[0]);
        if (a.isSymbol()) {
            id = gensym(a.asString().c_str());
        }

        x->json_global = new GlobalJSON(id->s_name, OBJ_NAME);
        x->json_local = &x->json_global->ref();
    } else {
        x->json_local = new Document;
    }

    x->json_local->SetObject();

    x->canvas = canvas_getcurrent();

    return static_cast<void*>(x);
}

static void json_object_free(t_json_object* x)
{
    delete x->json_global;
}

extern "C" void setup_global0x2ejson()
{
    json_global_object_class = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>(json_object_new),
        reinterpret_cast<t_method>(json_object_free),
        sizeof(t_json_object), CLASS_PATCHABLE, A_GIMME, 0);
    class_addanything(json_global_object_class, json_object_anything);

    class_addmethod(json_global_object_class, reinterpret_cast<t_method>(json_object_clear), gensym("clear"), A_NULL);
    class_addmethod(json_global_object_class, reinterpret_cast<t_method>(json_object_dump), gensym("dump"), A_NULL);

    class_addmethod(json_global_object_class, reinterpret_cast<t_method>(json_object_getobject), gensym("getobject"), A_NULL);
    class_addmethod(json_global_object_class, reinterpret_cast<t_method>(json_object_setobject), gensym("setobject"), A_GIMME, A_NULL);

    class_addmethod(json_global_object_class, reinterpret_cast<t_method>(json_object_read), gensym("read"), A_GIMME, A_NULL);
    class_addmethod(json_global_object_class, reinterpret_cast<t_method>(json_object_write), gensym("write"), A_GIMME, A_NULL);

    //    class_addmethod(json_global_object_class, reinterpret_cast<t_method>(json_object_objectlist), gensym("objectlist"), A_GIMME);
}
