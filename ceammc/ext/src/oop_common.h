//
//  oop_common.h
//  pd_ext
//
//  Created by Alex Nadzharov on 17/01/17.
//
//

#include "ceammc.h"
//#include "m_pd.h"
#include <stdlib.h>

#include "ceammc_atomlist.h"

#include "m_imp.h"

#include <stdio.h>

#include "cicm_wrapper.h"

#include "ceammc_globaldata.h"
#include <algorithm>
#include <string>

using namespace ceammc;
using namespace std;

#pragma mark -
#include <sstream>

template <class T>
inline string to_string(const T& t)
{
    stringstream ss;
    ss << t; // ss << std::hex << t;
    return ss.str();
}

#pragma mark -

struct t_n_canvasenvironment {
    t_symbol* ce_dir; /* directory patch lives in */
    int ce_argc; /* number of "$" arguments */
    t_atom* ce_argv; /* array of "$" arguments */
    int ce_dollarzero; /* value of "$0" */
    t_namelist* ce_path; /* search path */
};

static void canvas_addtolist(t_canvas* x)
{
    x->gl_next = pd_this->pd_canvaslist;
    pd_this->pd_canvaslist = x;
}

static void canvas_takeofflist(t_canvas* x)
{
    /* take it off the window list */
    if (x == pd_this->pd_canvaslist)
        pd_this->pd_canvaslist = x->gl_next;
    else {
        t_canvas* z;
        for (z = pd_this->pd_canvaslist; z->gl_next != x; z = z->gl_next)
            if (!z->gl_next)
                return;
        z->gl_next = x->gl_next;
    }
}

#pragma mark -

typedef vector<t_outlet*> OPOutputs; ///< vector of method boxes outputs
typedef vector<t_object*> OPProperties; ///< vector of property boxes

static void canvas_paste_class(t_canvas* x, t_binbuf* b);

//weird
class OPClass;
typedef GlobalData<OPClass*> OPClassByCanvas;
typedef GlobalData<OPClass*> OPClassBySymbol;

class OPClass {
private:
    map<string, string> methodNames; // todo rename
    map<string, string> propertyNames;
    map<string, string> signalNames;
    
    map<string, t_outlet*> methodOutlets; //todo OPOutputs
    map<string, t_outlet*> methodPointerOutlets; //todo OPOutputs

    OPClass* parent;

public:
    string class_name;
    t_canvas* canvas;

    t_symbol* symbol;

    // for dynamic (change arguments?)
    OPClass()
    {
        this->canvas = 0;
    }
    // for canvas-based (change arguments?)
    OPClass(string className)
    {

        this->canvas = (t_canvas*)subcanvas_new(gensym(className.c_str()));
        this->canvas->gl_havewindow = 1;
        this->canvas->gl_isclone = 1;

        this->symbol = gensym(class_name.c_str());
        this->class_name = className;

        OPClassByCanvas* link = new OPClassByCanvas(to_string((long)this->canvas), "OOP.common");
        link->ref() = this;

        OPClassBySymbol* link2 = new OPClassBySymbol(this->symbol->s_name, "OOP.common");
        link2->ref() = this;

        canvas_vis(this->canvas, 0);

        int dollarzero = 1000;
        t_n_canvasenvironment* env = (t_n_canvasenvironment*)getbytes(sizeof(t_n_canvasenvironment)); //TEMP
        env->ce_dir = canvas_getcurrentdir();
        env->ce_argc = 0;
        env->ce_argv = 0;
        env->ce_dollarzero = dollarzero++;
        env->ce_path = 0;

        this->canvas->gl_env = (t_canvasenvironment*)env;

        this->canvas->gl_owner = 0;
    }

#pragma mark getters
    map<string, string> getMethodNames()
    {
        return this->methodNames;
    }
    map<string, string> getPropertyNames()
    {
        return this->propertyNames;
    }
    map<string, t_outlet*> getMethodOutlets()
    {
        return this->methodOutlets;
    }

#pragma mark file io

    void readFile(string fileName, t_canvas* parent_canvas)
    {

        t_binbuf* b;
        b = binbuf_new();

        binbuf_read_via_canvas(b, (char*)(fileName.c_str()), parent_canvas, 0);

        int blen = 0;
        char* bchar;
        binbuf_gettext(b, &bchar, &blen);

        // to canvas
        // find better way to load?
        if (this->canvas) {
            if (this->canvas->gl_list) {
                glist_delete(this->canvas, this->canvas->gl_list);
            }

            canvas_paste_class(this->canvas, b);
            canvas_vis(this->canvas, 0);
            canvas_setcurrent(parent_canvas);

            post("loaded class: %s ", (char*)(fileName.c_str()));
        }

        //recursive

        if (this->parent) {
            //todo
            //this->parent->readFile(this->parent-, struct _glist *parent_canvas)
        }
    }

    void writeFile(string fileName, t_canvas* parent_canvas)
    {
        t_binbuf* b = binbuf_new();

        canvas_saveto(this->canvas, b);

        const char* dir = canvas_getdir(parent_canvas)->s_name;
        binbuf_write(b, (char*)(fileName.c_str()), (char*)dir, 0);

        post("saved class: %s ", (char*)(fileName.c_str()));
    }

#pragma mark dynamic: methods
    // dynamic stub:
    void addMethod(string methodName, string referenceName)
    {
        this->methodNames[methodName] = referenceName;
    }

    void addMethodOutlet(string referenceName, t_outlet* outlet)
    {
        //todo multiple
        this->methodOutlets[referenceName] = outlet;
    }

    void addMethodPointerOutlet(string referenceName, t_outlet* outlet)
    {
        //todo multiple
        this->methodPointerOutlets[referenceName] = outlet;
    }

    t_outlet* getMethodOutletForReferenceName(string referenceName)
    {
        t_outlet* ret;
        if (this->methodOutlets.count(referenceName)) {
            //todo multiple
            ret = this->methodOutlets[referenceName];
        }

        return ret;
    }

    t_outlet* getMethodPointerOutletForReferenceName(string referenceName)
    {
        t_outlet* ret;
        if (this->methodPointerOutlets.count(referenceName)) {
            //todo multiple
            ret = this->methodPointerOutlets[referenceName];
        }

        return ret;
    }

    void freeMethod(string methodName)
    {
        this->methodNames.erase(methodName);
    }

    void freeMethodOutlet(string referenceName)
    {
        this->methodOutlets.erase(referenceName);
    }

    void freeMethodPointerOutlet(string referenceName)
    {
        this->methodPointerOutlets.erase(referenceName);
    }

    void addProperty(string propertyName, string referenceName)
    {
        this->propertyNames[propertyName] = referenceName;
    }

    void freeProperty(string propertyName)
    {
        this->propertyNames.erase(propertyName);
    }

#pragma mark find

    static OPClass* findByCanvas(t_canvas* canvas)
    {
        OPClassByCanvas* ret = new OPClassByCanvas(to_string((long)canvas), "OOP.common");
        if (ret)
            return ret->ref();
        else
            return 0;
    }

    static OPClass* findBySymbol(t_symbol* symbol)
    {
        OPClassBySymbol* ret = new OPClassBySymbol(symbol->s_name, "OOP.common");
        if (ret)
            return ret->ref();
        else
            return 0;
    }

#pragma mark parent

    void setParentClass(OPClass* p_class)
    {
        this->parent = p_class;
    }

    OPClass* getParentClass()
    {
        return this->parent;
    }
    
#pragma mark signal
    
    void addSignal(string signalName, string referenceName)
    {
        this->signalNames[signalName] = referenceName;
    }
    
    void freeSignal(string signalName)
    {
        this->signalNames.erase(signalName);
    }
    
#pragma mark info
    AtomList getPropertyList()
    {
        AtomList ret;

        for (map<string, string>::iterator it = this->propertyNames.begin(); it != this->propertyNames.end(); ++it) {
            ret.append(Atom(gensym(it->first.c_str())));
        }

        return ret;
    }

    AtomList getMethodList()
    {
        AtomList ret;

        for (map<string, string>::iterator it = this->methodNames.begin(); it != this->methodNames.end(); ++it) {
            ret.append(Atom(gensym(it->first.c_str())));
        }

        return ret;
    }
    
    AtomList getSignalList()
    {
        AtomList ret;
        
        for (map<string, string>::iterator it = this->signalNames.begin(); it != this->signalNames.end(); ++it) {
            ret.append(Atom(gensym(it->first.c_str())));
        }
        
        return ret;
    
    }
};

typedef GlobalData<OPClass*> OPClasses; ///< class prototype

#pragma mark -

//weird
class OPInstance;
typedef GlobalData<OPInstance*> OPInstanceByCanvas;
typedef GlobalData<OPInstance*> OPInstanceBySymbol;

class OPInstance {
private:
    map<t_symbol*, OPOutputs> _methodOutputs; ///< vector of method outputs
    OPOutputs _instanceOutputs; ///< vector of instances outputs

    map<t_symbol*, OPOutputs> _getterOutputs; ///< new
    map<t_symbol*, OPOutputs> _setterOutputs; ///< new

    //new
    map<t_symbol*, AtomList> _propertyValues;
    int _refCount;

    // dynamic.
    map<string, string> methodNames;
    map<string, string> propertyNames;

    //signal
    map<t_symbol*, t_sample*> _signalBuffers; ///< vector of method outputs

    //
    map<t_symbol*, t_outlet*> _methodPointerOutputs; // todo OPOutputs
    
    OPInstanceByCanvas* canvasLink;
    OPInstanceBySymbol* symbolLink;
    

public:
    string class_name;
    t_canvas* canvas;

    t_symbol* symbol;

    //
    OPInstance* parent;

    OPInstance(OPClass* _opclass)

    {
        
        printf("new instance\n");

        this->class_name = _opclass->class_name;
        this->symbol = gensym(to_string((long)this).c_str());

        // new canvas. only for canvas-based classes
        if (_opclass->canvas) {
            int dsp_state = canvas_suspend_dsp();

            //parent
            if (_opclass->getParentClass()) {
                this->parent = new OPInstance(_opclass->getParentClass());
                post("created parent class instance");
            }

            this->canvas = (t_canvas*)subcanvas_new(gensym(_opclass->class_name.c_str()));
            this->canvas->gl_havewindow = 1;
            this->canvas->gl_isclone = 1;

            int dollarzero = 1000;
            t_n_canvasenvironment* env = (t_n_canvasenvironment*)getbytes(sizeof(t_n_canvasenvironment)); //TEMP
            env->ce_dir = canvas_getcurrentdir();
            env->ce_argc = 0;
            env->ce_argv = 0;
            env->ce_dollarzero = dollarzero++;
            env->ce_path = 0;

            this->canvas->gl_env = (t_canvasenvironment*)env;

            this->canvas->gl_owner = 0;

            //this->canvas->gl_owner = 0;

            this->canvasLink = new OPInstanceByCanvas(to_string((long)this->canvas), "OOP.common");
            this->canvasLink->ref() = this;

            this->symbolLink = new OPInstanceBySymbol(this->symbol->s_name, "OOP.common");
            this->symbolLink->ref() = this;

            //load
            t_binbuf* b1 = binbuf_new();

            canvas_saveto(_opclass->canvas, b1);

            canvas_paste_class(this->canvas, b1);
            canvas_vis(this->canvas, 0);

            canvas_addtolist(this->canvas);

            canvas_loadbang(this->canvas);

            canvas_resume_dsp(dsp_state);

            //todo bind symbols

            printf("OPInstance + canvas\n");
            printf("canvas: %lu\n", (long)this->canvas);

            //todo cleanup?
            //this->retain();
            this->_refCount = 1;
        }

        //generate properties
        this->propertyNames = _opclass->getPropertyNames();

        //generate methods
        //TODO normal class / singleton
        OPClasses* dyn_class = new OPClasses("__dynamicStub", "exp.method");
        if (!dyn_class->ref()) {
            dyn_class->ref() = new OPClass();
            dyn_class->ref()->class_name = "__dynamicStub";
        }

        this->methodNames = _opclass->getMethodNames();
        // temporary
        map<string, string>::iterator it;
        for (it = this->methodNames.begin(); it != this->methodNames.end(); ++it) {
            t_outlet* dyn_out = dyn_class->ref()->getMethodOutletForReferenceName(it->second);

            if (dyn_out)
                this->addMethod(gensym(it->first.c_str()), dyn_out);

            t_outlet* dyn_pointer_out = dyn_class->ref()->getMethodPointerOutletForReferenceName(it->second);

            if (dyn_pointer_out)
                this->addMethodPointerOut(gensym(it->first.c_str()), dyn_pointer_out);
        }
    }

    ~OPInstance()
    {
        int dsp_state = canvas_suspend_dsp();

        printf("canvas: %lu\n", (long)this->canvas);

        if (this->canvas)
        {
        canvas_dirty(this->canvas, 0);
        canvas_takeofflist(this->canvas);
        canvas_free(this->canvas);

        this->canvas = 0;
        }

        delete this->canvasLink;
        delete this->symbolLink;
        
        canvas_resume_dsp(dsp_state);

        printf("~OPInstance\n");
        printf("canvas: %lu\n", (long)this->canvas);
    }

#pragma mark methods

    void addMethod(t_symbol* methodName, t_outlet* outlet)
    {
        this->_methodOutputs[methodName].push_back(outlet);
    }

    void freeMethod(t_symbol* methodName)
    {
        this->_methodOutputs.erase(methodName);
    }

    void addMethodPointerOut(t_symbol* methodName, t_outlet* outlet)
    {
        this->_methodPointerOutputs[methodName] = outlet; //.push_back(outlet);  TODO
    }

    void freeMethodPointerOut(t_symbol* methodName)
    {
        this->_methodPointerOutputs.erase(methodName);
    }

#pragma mark signal

    t_sample* getBufferFor(t_symbol* signalName, int vec_size)
    {
        t_sample* ret = this->_signalBuffers[signalName];

        if (!ret) {
            ret = new t_sample[vec_size];
            this->_signalBuffers[signalName] = ret;
            post("new buffer: %s %i\n", signalName->s_name, vec_size);
        }

        return ret;
    }

    void freeSignal(t_symbol* signalName)
    {
        //todo refcounter
        post("del buffer");
        delete (this->_signalBuffers[signalName]);
        this->_signalBuffers.erase(signalName);
    }

#pragma mark properties

    void addProperty(t_symbol* propertyName, t_outlet* getter_out, t_outlet* setter_out)
    {
        this->_getterOutputs[propertyName].push_back(getter_out);
        this->_setterOutputs[propertyName].push_back(setter_out);

        this->propertyNames[propertyName->s_name] = "<none>"; //don't link by name as we link outlets.
    }
    void freeProperty(t_symbol* propertyName)
    {
        this->_getterOutputs.erase(propertyName);
        this->_setterOutputs.erase(propertyName);

        this->propertyNames.erase(propertyName->s_name);
    }

    void setAtomListProperty(t_symbol* propertyName, AtomList list)
    {
        this->_propertyValues[propertyName] = list;
    }

    AtomList getAtomListProperty(t_symbol* propertyName)
    {
        AtomList list = this->_propertyValues[propertyName];

        return list;
    }

    AtomList getPropertyList()
    {
        AtomList ret;

        for (map<string, string>::iterator it = this->propertyNames.begin(); it != this->propertyNames.end(); ++it) {
            ret.append(Atom(gensym(it->first.c_str())));
        }

        return ret;
    }

#pragma mark methods

    void addInstanceOut(t_outlet* outlet)
    {
        this->_instanceOutputs.push_back(outlet);
    }
    void freeInstanceOut(t_outlet* outlet)
    {
        this->_instanceOutputs.erase(remove(this->_instanceOutputs.begin(), this->_instanceOutputs.end(), outlet), this->_instanceOutputs.end());
    }

    void multipleOutput(AtomList list)
    {
        for (OPOutputs::iterator it = this->_instanceOutputs.begin(); it != this->_instanceOutputs.end(); ++it) {
            list.output(*it);
        }
    }

    void callMethod(AtomList list)
    {
        t_symbol* method_name = list[0].asSymbol();

        AtomList subList = list.slice(1, (int)list.size());

        OPOutputs* out1 = &this->_methodOutputs[method_name];

        if (out1) {
            for (OPOutputs::iterator it = out1->begin(); it != out1->end(); ++it) {
                subList.output(*it);
            }
        }

        //dynamic. todo

        t_outlet* out2 = this->_methodPointerOutputs[method_name];
        if (out2) {
            AtomList objList = AtomList(Atom(gensym("setobject")));

            objList.append(Atom(gensym(to_string((long)this).c_str())));

            objList.output(out2);
        }
    }

    void callSetter(AtomList list)
    {
        t_symbol* property_name = list[0].asSymbol();

        //        OPProperties *out1 = &this->instancePropertyBoxes[property_name];
        //
        //        if (out1)
        //        {
        //            for (OPProperties::iterator it =out1->begin(); it!=out1->end(); ++it)
        //            {
        //                pd_typedmess((t_pd*)*it, gensym("set"), (int)list.size(), list.toPdData());
        //            }
        //        }

        //        Atom name = Atom(argv[0]);
        //
        //        if (name.asSymbol() == x->property_name)
        //        {
        //            AtomList list2(argc-1,&argv[1]);
        //            x->instance->setAtomListProperty(x->property_name, list2);
        //
        //            outlet_bang(x->out2);
        //        }

        AtomList list2((size_t)list.size() - 1, (t_atom*)list.toPdData() + 1); //TODO
        this->setAtomListProperty(property_name, list2);

        OPOutputs* out1 = &this->_setterOutputs[property_name];

        for (OPOutputs::iterator it = out1->begin(); it != out1->end(); ++it) {
            outlet_bang(*it);
        }
    }

    void callGetter(AtomList list)
    {
        t_symbol* property_name = list[0].asSymbol();

        //        OPProperties *out1 = &this->instancePropertyBoxes[property_name];
        //
        //        if (out1)
        //        {
        //            for (OPProperties::iterator it =out1->begin(); it!=out1->end(); ++it)
        //            {
        //                pd_typedmess((t_pd*)*it, gensym("get"), (int)list.size(), list.toPdData());
        //            }
        //        }

        AtomList list2(list[0]);
        list2.append(this->getAtomListProperty(property_name));

        this->multipleOutput(list2);

        OPOutputs* out1 = &this->_getterOutputs[property_name];

        for (OPOutputs::iterator it = out1->begin(); it != out1->end(); ++it) {
            outlet_bang(*it);
        }
    }

    AtomList getMethodList()
    {
        AtomList ret;

        for (map<t_symbol*, OPOutputs>::iterator it = this->_methodOutputs.begin(); it != this->_methodOutputs.end(); ++it) {
            ret.append(Atom(it->first));
        }

        return ret;
    }

    AtomList getDynamicMethodList()
    {
        AtomList ret;

        for (map<string, string>::iterator it = this->methodNames.begin(); it != this->methodNames.end(); ++it) {
            ret.append(Atom(gensym(it->first.c_str())));
        }

        return ret;
    }

#pragma mark reference counting
    // names?
    void retain()
    {
        this->_refCount++;
        printf("OPInstance ref count: %i\n", this->_refCount);
    }

    //?
    void release()
    {
        this->_refCount--;
        printf("OPInstance ref count: %i\n", this->_refCount);
        if (this->_refCount == 0)
            delete this;
    }

    //debug
    int getRefCount()
    {
        return this->_refCount;
    }

#pragma mark find
    static OPInstance* findByCanvas(t_canvas* canvas)
    {
        OPInstanceByCanvas* ret = new OPInstanceByCanvas(to_string((long)canvas), "OOP.common");
        return ret->ref();
    }

    static OPInstance* findBySymbol(t_symbol* symbol)
    {
        OPInstanceBySymbol* ret = new OPInstanceBySymbol(symbol->s_name, "OOP.common");
        return ret->ref();
    }
};

// yet unused
#pragma mark canvas additions - C style

static bool canvas_is_class(t_canvas* canvas)
{
    return (OPClass::findByCanvas(canvas));
}

static bool canvas_is_instance(t_canvas* canvas)
{
    return (OPInstance::findByCanvas(canvas));
}

#pragma mark -

// copied from canvas_dopaste
// todo fix

static void canvas_paste_class(t_canvas* x, t_binbuf* b)
{
    t_gobj* g2; // *last,*newgobj,
    int dspstate = canvas_suspend_dsp(), nbox, count;
    t_symbol* asym = gensym("#A");
    /* save and clear bindings to symbols #a, $N, $X; restore when done */
    t_pd *boundx = s__X.s_thing, *bounda = asym->s_thing,
         *boundn = s__N.s_thing;
    asym->s_thing = 0;
    s__X.s_thing = &x->gl_pd;
    s__N.s_thing = 0; //&pd_canvasmaker;

    canvas_editmode(x, 1.);
    glist_noselect(x);
    for (g2 = x->gl_list, nbox = 0; g2; g2 = g2->g_next)
        nbox++;

    //    paste_onset = nbox;
    //    paste_canvas = x;

    canvas_setargs(0, 0);
    binbuf_eval(b, 0, 0, 0);
    for (g2 = x->gl_list, count = 0; g2; g2 = g2->g_next, count++)
        if (count >= nbox)
            glist_select(x, g2);
    //    paste_canvas = 0;
    canvas_resume_dsp(dspstate);
    canvas_dirty(x, 1);
    if (x->gl_mapped)
        sys_vgui("pdtk_canvas_getscroll .x%lx.c\n", x);
    //    if (!sys_noloadbang)
    //        glist_donewloadbangs(x);
    asym->s_thing = bounda;
    s__X.s_thing = boundx;
    s__N.s_thing = boundn;
}
