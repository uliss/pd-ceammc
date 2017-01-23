//
//  oop_common.h
//  pd_ext
//
//  Created by Alex Nadzharov on 17/01/17.
//
//

#include "ceammc.h"
#include <m_pd.h>
//#include <g_canvas.h>
#include <stdlib.h>
//
#include "ceammc_atomlist.h"

#include "m_imp.h"

#include <stdio.h>

#include "cicm_wrapper.h"

#include "ceammc_globaldata.h"
#include <algorithm>


using namespace ceammc;

typedef std::vector<t_outlet*> OPOutputs;           ///< vector of method boxes outputs
typedef std::vector<t_object*> OPProperties;        ///< vector of property boxes

static void canvas_paste_class(t_canvas *x, t_binbuf *b);

class t_op_class
{
public:
    std::string class_name;
    t_canvas *canvas;
    
    void readFile(std::string fileName, t_canvas *parent_canvas)
    {
    
        t_binbuf *b;
        b = binbuf_new();
        
        binbuf_read_via_canvas(b,(char*)(fileName.c_str()), parent_canvas, 0);
        
        int blen=0;
        char *bchar;
        binbuf_gettext(b, &bchar, &blen);
        
        // to canvas
        // find better way to load?
        
        if (this->canvas->gl_list)
        {
            glist_delete(this->canvas, this->canvas->gl_list);
            
        }
        
        canvas_paste_class(this->canvas, b);
        canvas_vis(this->canvas, 0);
        canvas_setcurrent(parent_canvas);
        
        post("loaded class: %s ", (char*)(fileName.c_str()));
        
    }
    
    void writeFile(std::string fileName, t_canvas *parent_canvas)
    {
        t_binbuf *b = binbuf_new();
        
        canvas_saveto(this->canvas, b);
        
        const char * dir = canvas_getdir(parent_canvas)->s_name;
        binbuf_write(b, (char*)(fileName.c_str()), (char *)dir, 0);
        
        post("saved class: %s ", (char*)(fileName.c_str()));
    }
    
    
    
};

typedef GlobalData<t_op_class*> OPClass;                        ///< class prototype

class t_op_instance
{
private:
    std::map<t_symbol*,OPOutputs> _methodOutputs;               ///< vector of method outputs
    OPOutputs _instanceOutputs;                                 ///< vector of instances outputs
    
    std::map<t_symbol*,OPProperties> instancePropertyBoxes;     ///< for property hanling we get pointers to objects instead of outlets
    
public:
    std::string class_name;
    t_canvas *canvas;
    
//    t_op_instance()     //temporary
//    {
//        this->class_name = "—";
//        
//    }
    void newInstance(t_op_class * _opclass) // todo constructor
    {
        // new canvas. check
        this->canvas = (t_canvas*)subcanvas_new(gensym(_opclass->class_name.c_str()));   //LISP lol
        this->canvas->gl_havewindow = 1;
        this->canvas->gl_env = 0;
        this->class_name = _opclass->class_name;
        
        t_binbuf *b1 = binbuf_new();
        
        canvas_saveto(_opclass->canvas, b1);
        
        int blen=0;
        char *bchar;
        binbuf_gettext(b1, &bchar, &blen);
        
        canvas_paste_class(this->canvas, b1);
        canvas_vis(this->canvas, 0);
        
        //todo refcounter?
        
    }
    
    void addMethod(t_symbol* methodName, t_outlet *outlet)
    {
        this->_methodOutputs[methodName].push_back(outlet);
    }
    void freeMethod(t_symbol* methodName)
    {
        this->_methodOutputs.erase(methodName);
    }
    
    void addPropertyBox(t_symbol* pMethodName, t_object *object)
    {
        this->instancePropertyBoxes[pMethodName].push_back(object);
    }
    void freePropertyBox(t_symbol* pMethodName)
    {
        this->instancePropertyBoxes.erase(pMethodName);
    }
    
    void addInstanceOut(t_outlet *outlet)
    {
        this->_instanceOutputs.push_back(outlet);
    }
    void freeInstanceOut(t_outlet *outlet)
    {
        this->_instanceOutputs.erase(std::remove(this->_instanceOutputs.begin(), this->_instanceOutputs.end(), outlet), this->_instanceOutputs.end());
    }
    
    void multipleOutput(AtomList list)
    {
        for (OPOutputs::iterator it =this->_instanceOutputs.begin(); it!=this->_instanceOutputs.end(); ++it)
        {
            list.output(*it);
        }
        
    }
    
    void callMethod(AtomList list)
    {
        //post("call method %s", list[0].asString().c_str());
        t_symbol *method_name = list[0].asSymbol();
        
        AtomList subList = list.subList(1, (int)list.size());;
        
        OPOutputs *out1 = &this->_methodOutputs[method_name];
        
        if (out1)
        {
            for (OPOutputs::iterator it =out1->begin(); it!=out1->end(); ++it)
            {
                subList.output(*it);
            }
        }
    }
    
    void callSetter(AtomList list)
    {
        //post("call method %s", list[0].asString().c_str());
        t_symbol *property_name = list[0].asSymbol();
        
        //AtomList subList = list.subList(1, list.size());;
        
        OPProperties *out1 = &this->instancePropertyBoxes[property_name];
        
        if (out1)
        {
            for (OPProperties::iterator it =out1->begin(); it!=out1->end(); ++it)
            {
                //subList.output(*it);
                
                pd_typedmess((t_pd*)*it, gensym("set"), (int)list.size(), list.toPdData());
            }
        }
    }
    
    void callGetter(AtomList list)
    {
        //post("call method %s", list[0].asString().c_str());
        t_symbol *property_name = list[0].asSymbol();
        
        //AtomList subList = list.subList(1, list.size());;
        
        OPProperties *out1 = &this->instancePropertyBoxes[property_name];
        
        if (out1)
        {
            for (OPProperties::iterator it =out1->begin(); it!=out1->end(); ++it)
            {
                //subList.output(*it);
                
                pd_typedmess((t_pd*)*it, gensym("get"), (int)list.size(), list.toPdData());
            }
        }
    }
    
    AtomList getMethodList()
    {
        AtomList ret;
        
        //this->_methodOutputs[methodName]
        for (std::map<t_symbol*,OPOutputs>::iterator it = this->_methodOutputs.begin(); it != this->_methodOutputs.end(); ++it)
        {
            ret.append(Atom(it->first));
        }
        
        return ret;
    }
    
    AtomList getPropertyList()
    {
        AtomList ret;
        
        //this->_methodOutputs[methodName]
        for (std::map<t_symbol*,OPProperties>::iterator it = this->instancePropertyBoxes.begin(); it != this->instancePropertyBoxes.end(); ++it)
        {
            ret.append(Atom(it->first));
        }
        
        return ret;
    }
    
};
typedef GlobalData<t_op_instance> OPInstance;          ///< Class instance is identified by canvas name. Probably fix that.


#include <sstream>

template <class T>
inline std::string to_string (const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

#pragma mark -

// copied from canvas_dopaste
// todo fix

static void canvas_paste_class(t_canvas *x, t_binbuf *b)
{
    t_gobj *g2; // *last,*newgobj,
    int dspstate = canvas_suspend_dsp(), nbox, count;
    t_symbol *asym = gensym("#A");
    /* save and clear bindings to symbols #a, $N, $X; restore when done */
    t_pd *boundx = s__X.s_thing, *bounda = asym->s_thing,
    *boundn = s__N.s_thing;
    asym->s_thing = 0;
    s__X.s_thing = &x->gl_pd;
    s__N.s_thing = 0;//&pd_canvasmaker;
    
    canvas_editmode(x, 1.);
    glist_noselect(x);
    for (g2 = x->gl_list, nbox = 0; g2; g2 = g2->g_next) nbox++;
    
    //    paste_onset = nbox;
    //    paste_canvas = x;
    
    
    
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