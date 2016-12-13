//
//  ceammc_gui.cpp
//  pd_ext
//
//  Created by Alex on 12/12/16.
//
//

#include "ceammc_gui.hpp"

#pragma mark init

t_object * ceammc_gui::ceammc_gui_pd_object = NULL;
std::vector<std::string> ceammc_gui::ui_property_names;
t_class* ceammc_gui::ceammc_gui_pd_class;
t_widgetbehavior ceammc_gui::w_widget;

#pragma mark properties

t_atom ceammc_gui_object::ui_property_get(std::string name)
{
    t_atom ret = this->ui_properties[name];
    
    return ret;
}

t_float ceammc_gui_object::ui_property_get_float(std::string name)
{
    if (this->ui_properties[name].a_type == A_FLOAT)
    {
        t_float ret = this->ui_properties[name].a_w.w_float;
    
        return ret;
    }
    else
    {
        ceammc_gui::e_error("bad float value");
        return 0;
    }
}

char* ceammc_gui_object::ui_property_get_c_str(std::string name)
{
    if (this->ui_properties[name].a_type == A_SYMBOL)
    {
        char* ret = this->ui_properties[name].a_w.w_symbol->s_name;
        
        return ret;
    }
    else
    {
        ceammc_gui::e_error("bad symbol value");
        return "";
    }
}

void ceammc_gui_object::ui_property_set(std::string name, t_atom value)
{
    this->ui_properties[name] = value;
}

void ceammc_gui_object::ui_property_set(std::string name, std::string value)
{
    t_atom a;
    a.a_w.w_symbol = gensym(value.c_str());
    a.a_type = A_SYMBOL;
    this->ui_properties[name] = a;
}

void ceammc_gui_object::ui_property_set(std::string name, float value)
{
    t_atom a;
    a.a_w.w_float = value;
    a.a_type = A_FLOAT;
    this->ui_properties[name] = a;
}

bool ceammc_gui_object::ui_property_load(t_atom *values)
{
    int count = (int)this->ui_properties.size();
    int i=0;
    
    std::map<std::string, t_atom>::iterator it;
    
    for (it = this->ui_properties.begin(); it != this->ui_properties.end(); ++it)
    {
        if (i==count) break;
        
        t_atom a = values[i];
        if (a.a_type != it->second.a_type)
        {
            ceammc_gui::e_error("broken object data");
            return false;
        }
        
        i++;
        
    }
    
    return true;
    
}

void ceammc_gui_object::ui_property_init()
{
    
    this->ui_property_set("object_name", "ceammc.gui_object");
    
    this->ui_property_set("x", 0);
    this->ui_property_set("y", 0);
    this->ui_property_set("width", 45);
    this->ui_property_set("height", 15);
    
    this->ui_property_set("font", "Helvetica");
    this->ui_property_set("font_size", 14.);
    
    this->ui_property_set("color_bg", "#FFFFFF");
    this->ui_property_set("color_text", "#000000");
    this->ui_property_set("color_line", "#00C0FF");
    
    this->ui_property_set("send_id", "no_send");
    this->ui_property_set("receive_id", "no_receive");
    
    this->ui_property_set("label", "CEAMMC gui template");
    
    this->ui_property_set("label_x", 7);
    this->ui_property_set("label_y", 7);
    
}

#pragma mark -
#pragma mark draw wrappers

//TODO names

void ceammc_gui_object::dw_set_canvas(t_glist *glist)
{
//    printf("---");
    this->w_canvas = glist;
}

void ceammc_gui_object::dw_set_object(t_object *obj)
{
    this->w_obj = obj;
}

void ceammc_gui_object::dw_rect (std::string obj, int x, int y, int w, int h, std::string stroke_color, std::string fill_color, float line_width)
{
    sys_vgui(".x%lx.c create rectangle %d %d %d %d -width %d -outline %s -fill %s -tags %lx%s\n",
             this->w_canvas, x, y,x+w, y+h,
             line_width,
             stroke_color.c_str(),
             fill_color.c_str(), this->w_obj, obj.c_str());
}

void ceammc_gui_object::dw_text (std::string obj, int x, int y, std::string text, std::string text_color)
{
    int font_size = 12;//*(this->ui_property_get("font_size")).get();
    sys_vgui(".x%lx.c create text %d %d -text {%s} -anchor w -font {{%s} -%d %s} -fill %s -tags [list %lx%s label text]\n",
             this->w_canvas, x,
             y,
             text.c_str(),
             font_size, sys_fontweight,
             text_color.c_str(), this->w_obj, obj.c_str());
    
    //xx+x->x_gui.x_ldx * zoomlabel
    //yy+x->x_gui.x_ldy * zoomlabel
}

void ceammc_gui_object::dw_move (std::string obj, int x, int y, int w, int h)
{
    sys_vgui(".x%lx.c coords %lx%s %d %d %d %d\n",
             this->w_canvas, this->w_obj, obj.c_str(), x, y, w, h);
}

void ceammc_gui_object::dw_set_width (std::string obj, int w)
{
    sys_vgui(".x%lx.c itemconfigure %lx%s -width %d\n", this->w_canvas, this->w_obj ,obj.c_str(), w);
}

void ceammc_gui_object::dw_delete(std::string obj)
{
    sys_vgui(".x%lx.c delete %lx%s\n", this->w_canvas, this->w_obj, obj.c_str());
}


#pragma mark -
#pragma mark widget


t_widgetbehavior *ceammc_gui::w_create()
{
    t_widgetbehavior *ret = new t_widgetbehavior;
    
    ret->w_activatefn = NULL;   //TEMP
    ret->w_clickfn = ceammc_gui::w_click;
    ret->w_deletefn = ceammc_gui::w_delete;
    ret->w_displacefn = ceammc_gui::w_displace;
    ret->w_getrectfn = ceammc_gui::w_getrect;
    ret->w_selectfn = ceammc_gui::w_select;
    ret->w_visfn = ceammc_gui::w_vis;
    
    return ret;
}


//

#pragma mark ceammc_widget_class

void ceammc_gui_object::w_getrect(t_gobj *z, t_glist *glist, int *x1, int *y1, int *x2, int *y2)
{
//    printf("getrect prototype\n");
    ceammc_gui_object *x = (ceammc_gui_object *)z;
    
    *x1 = text_xpix(&x->x_obj, glist);
    *y1 = text_ypix(&x->x_obj, glist);
    *x2 = *x1 + x->x_w;
    *y2 = *y1 + x->x_h;
}


int ceammc_gui_object::w_click(t_gobj *z, struct _glist *glist, int xpix, int ypix, int shift, int alt, int dbl, int doit)
{
    return 1;
    
}

void ceammc_gui_object::w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    printf("displace prototype\n");
}

void ceammc_gui_object::w_select(t_gobj *z, t_glist *glist, int selected)
{
    printf("sel prototype\n");
}

void ceammc_gui_object::w_delete(t_gobj *z, t_glist *glist)
{
    printf("del prototype \n");
}

void ceammc_gui_object::w_draw(t_gobj *z, t_glist *glist)
{
    printf("draw prototype\n");
}

void ceammc_gui_object::w_erase(t_gobj *z, t_glist *glist)
{
    printf("erase prototype\n");
}

#pragma mark -

#pragma mark widget proxy

void ceammc_gui::w_getrect(t_gobj *z, t_glist *glist, int *x1, int *y1, int *x2, int *y2)
{
    ((ceammc_gui_object*)ceammc_gui::ceammc_gui_pd_object) -> w_getrect(z,glist,x1,y1,x2,y2);
}
int ceammc_gui::w_click(t_gobj *z, struct _glist *glist, int xpix, int ypix, int shift, int alt, int dbl, int doit)
{
    return ((ceammc_gui_object*)ceammc_gui::ceammc_gui_pd_object) -> w_click(z, glist, xpix, ypix, shift, alt, dbl, doit);
}
void ceammc_gui::w_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    ((ceammc_gui_object*)ceammc_gui::ceammc_gui_pd_object) -> w_displace(z, glist, dx, dy);
}
void ceammc_gui::w_select(t_gobj *z, t_glist *glist, int selected)
{
    ((ceammc_gui_object*)ceammc_gui::ceammc_gui_pd_object) -> w_select(z, glist, selected);
}
void ceammc_gui::w_delete(t_gobj *z, t_glist *glist)
{
    ((ceammc_gui_object*)ceammc_gui::ceammc_gui_pd_object) -> w_delete(z, glist);
}
void ceammc_gui::w_vis(t_gobj *z, t_glist *glist, int vis)
{
    if (vis)
        ((ceammc_gui_object*)ceammc_gui::ceammc_gui_pd_object) -> w_draw(z, glist);
    else
        ((ceammc_gui_object*)ceammc_gui::ceammc_gui_pd_object) -> w_erase(z, glist);
    
}


#pragma mark log

void ceammc_gui::e_error(std::string err)
{
    post("ceammc_gui_error: %s", err.c_str());
}

#pragma mark -
#pragma mark class



void ceammc_gui::ui_set_property_names(std::vector<std::string> names)
{
    ceammc_gui::ui_property_names = names;
}

std::vector<std::string> ceammc_gui::ui_get_property_names()
{
    return ceammc_gui::ui_property_names;
}

#pragma mark _


void *ceammc_gui::pd_class_new(t_symbol *s, int argc, t_atom *argv)
{
    printf("gui class new\n");
    
    ceammc_gui_object* x = reinterpret_cast<ceammc_gui_object*>(pd_new(ceammc_gui::ceammc_gui_pd_class));
    
    x->ui_property_names = ceammc_gui::ui_property_names;
    
    ceammc_gui::ceammc_gui_pd_object = (t_object*)x;

    if (argc!=x->ui_property_names.size())
    {
        x->ui_property_init();
    }
    else
    {
        if (!x->ui_property_load(argv))
            x->ui_property_init();
    }
    
    return static_cast<void*>(x);
}

void ceammc_gui::pd_class_free(ceammc_gui_object *x)
{
    //    if(x->x_gui.x_fsf.x_rcv_able)
    //        pd_unbind(&x->x_gui.x_obj.ob_pd, x->x_gui.x_rcv);
    
    //gfxstub_deleteforkey(x);
}

void ceammc_gui::pd_class_save(t_gobj *z, t_binbuf *b)
{
    ceammc_gui_object *x = (ceammc_gui_object *)z;
    
    binbuf_addv(b, "#X obj ");
    
    std::map<std::string,t_atom>::iterator it;
    for (it = x->ui_properties.begin(); it!= x->ui_properties.end(); ++it)
    {
        binbuf_add(b, 1, &it->second);
    }
    
    binbuf_addv(b, ";");
    
}

#pragma mark plus





#pragma mark -





void ceammc_gui::pd_setup(t_object *gui_class)
{
    printf("lib pd_setup %lu | %lu\n", gui_class, ceammc_gui::ceammc_gui_pd_object);
    
    //ceammc_gui::ceammc_gui_pd_object = gui_class;
    
    
    std::string name = ((ceammc_gui_object*)gui_class)->ui_property_get_c_str("object_name");
    //printf("* name %s",name.c_str());
    if (name.length()==0) {ceammc_gui::e_error("bad object name");return;}

    //?
    //
    ceammc_gui::ceammc_gui_pd_class = class_new(gensym(name.c_str()),
                               reinterpret_cast<t_newmethod>(pd_class_new),
                               reinterpret_cast<t_method>(pd_class_free),
                               ((ceammc_gui_object*)gui_class)->size(), CLASS_PATCHABLE, A_GIMME,0);   //sizeof(gui_class)*16
    //this->pd_class= ceammc_gui_pd_class;
    
    printf("new ptr %lu\n", ceammc_gui::ceammc_gui_pd_class);
    
    
    ceammc_gui::w_widget = *this->w_create();
    
//    ((ceammc_gui_object*)gui_class)->dw_set_object((t_object*)ceammc_gui::ceammc_gui_pd_class);
//    
    class_setwidget(ceammc_gui::ceammc_gui_pd_class, &ceammc_gui::w_widget);
//    class_setsavefn(ceammc_gui::ceammc_gui_pd_class, pd_class_save);
    
    
}


//struct _class
//{
//    t_symbol *c_name;                   /* name (mostly for error reporting) */
//    t_symbol *c_helpname;               /* name of help file */
//    t_symbol *c_externdir;              /* directory extern was loaded from */
//    size_t c_size;                      /* size of an instance */
//    t_methodentry *c_methods;           /* methods other than bang, etc below */
//    int c_nmethod;                      /* number of methods */
//    t_method c_freemethod;              /* function to call before freeing */
//    t_bangmethod c_bangmethod;          /* common methods */
//    t_pointermethod c_pointermethod;
//    t_floatmethod c_floatmethod;
//    t_symbolmethod c_symbolmethod;
//    t_listmethod c_listmethod;
//    t_anymethod c_anymethod;
//    struct _widgetbehavior *c_wb;       /* "gobjs" only */
//    struct _parentwidgetbehavior *c_pwb;/* widget behavior in parent */
//    t_savefn c_savefn;                  /* function to call when saving */
//    t_propertiesfn c_propertiesfn;      /* function to start prop dialog */
//    int c_floatsignalin;                /* onset to float for signal input */
//    char c_gobj;                        /* true if is a gobj */
//    char c_patchable;                   /* true if we have a t_object header */
//    char c_firstin;                 /* if patchable, true if draw first inlet */
//    char c_drawcommand;             /* a drawing command for a template */
//};


////////////////////
// prototype

//extern "C" {void ___setup_ceammc_gui_template()
//{
//    ceammc_gui *gui = new ceammc_gui();
//    
//    ceammc_gui_object *obj1 = new ceammc_gui_object();
//    
//    gui->pd_setup((t_object*)obj1);
//
//    
//}
//}




