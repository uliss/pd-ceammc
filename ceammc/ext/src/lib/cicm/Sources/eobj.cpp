/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eobj.h"
#include "eclass.h"
#include "ceammc_platform.h"
#include "ecommon.h"
#include "egraphics.h"

#include <inttypes.h>
#include <string>

static t_eproxy* eproxy_new(t_eobj* owner, t_symbol* sym_from, t_symbol* sym_to);
static void eproxy_free(t_eobj* owner, t_eproxy* proxy);

void* eobj_new(t_eclass* c)
{
    t_eobj* x = NULL;
    char buffer[MAXPDSTRING];
    if (c) {
        x = (t_eobj*)pd_new(&c->c_class);

        if (!x) {
            bug("pd_new: object %s allocation failed", c->c_class.c_name->s_name);
            return 0;
        }

        x->o_nproxy = 0;
        x->o_proxy = nullptr;
        x->o_canvas = canvas_getcurrent();
        sprintf(buffer, "#%s%" PRIxPTR, c->c_class.c_name->s_name, (uintptr_t)x);
        x->o_id = gensym(buffer);
        pd_bind(&x->o_obj.ob_pd, x->o_id);
        sprintf(buffer, ".x%" PRIxPTR ".c", (uintptr_t)x->o_canvas);
        c->c_widget.w_dosave = (t_typ_method)eobj_dosave;
    } else {
        bug("pd_new: apparently called before setup routine");
    }

    return (x);
}

void eobj_free(t_eobj* x)
{
    if (x->o_id) {
        pd_unbind((t_pd*)x, x->o_id);
        x->o_id = NULL;
    }
    if (x->o_proxy && x->o_nproxy) {
        free(x->o_proxy);
        x->o_proxy = NULL;
        x->o_nproxy = 0;
    }
}

t_pd_err eobj_iscicm(void* x)
{
    return zgetfn((t_pd*)x, s_iscicm) != nullptr;
}

t_eproxy* eobj_proxynew(t_eobj* x)
{
    return eproxy_new(x, nullptr, nullptr);
}

int eobj_getproxy(t_eobj* x)
{
    return x->o_current_proxy;
}

t_eclass* eobj_getclass(t_eobj* x)
{
    return (t_eclass*)x->o_obj.te_g.g_pd;
}

t_symbol* eobj_getclassname(t_eobj* x)
{
    return x->o_obj.te_g.g_pd->c_name;
}

t_canvas* eobj_getcanvas(t_eobj* x)
{
    return x->o_canvas;
}

bool eobj_isbox(t_eobj* x)
{
    return eobj_getclass(x)->c_box;
}

bool eobj_isdsp(t_eobj* x)
{
    t_eclass* c = eobj_getclass(x);
    return (c->c_dsp && c->c_widget.w_dsp);
}

void eobj_popup(t_eobj* x, t_symbol* s, t_float itemid, t_float xpos, t_float ypos)
{
    t_eclass* c = eobj_getclass(x);
    if (c->c_widget.w_popup) {
        c->c_widget.w_popup(x, s, (long)itemid, { xpos, ypos });
    }
}

void eobj_dosave(t_eobj* x, t_binbuf* b)
{
    t_binbuf* d;
    t_eclass* c = eobj_getclass(x);
    if (c && b) {
        binbuf_addv(b, (char*)"ssii", &s__X, s_obj, (t_int)x->o_obj.te_xpix, (t_int)x->o_obj.te_ypix);
        d = x->o_obj.te_binbuf;
        if (d) {
            binbuf_addbinbuf(b, d);
        }
        if (c->c_widget.w_save != NULL) {
            c->c_widget.w_save(x, b);
        }
        binbuf_addv(b, (char*)";");
    }
}

void eobj_save(t_eobj* x, t_binbuf* b)
{
    if (x && b) {
        t_eclass* c = eobj_getclass(x);
        if (c && c->c_widget.w_dosave != NULL)
            c->c_widget.w_dosave((t_eobj*)x, b);
    }
}

void eobj_write(t_eobj* x, t_symbol* s, int argc, t_atom* argv)
{
    t_eclass* c = eobj_getclass(x);

    // The file name is defined
    if (argc && argv && atom_gettype(argv) == A_SYMBOL) {
        std::string path = ceammc::platform::make_abs_filepath_with_canvas(x->o_canvas,
            atom_getsymbol(argv)->s_name);

        if (c->c_widget.w_write)
            c->c_widget.w_write(x, path.c_str());

        return;
    }
    // The file name is not defined so we popup a window
    else {
        // dialog cancelled
        if (s == gensym("eobjwriteto"))
            return;

        sys_vgui("eobj_saveas %s nothing nothing\n", x->o_id->s_name);
    }
}

void eobj_attrprocess_viatoms(t_eobj* x, int argc, t_atom* argv)
{
    char buffer[MAXPDSTRING];
    int defc = 0;
    t_atom* defv = NULL;
    t_eclass* c = eobj_getclass(x);

    for (size_t i = 0; i < c->c_nattr; i++) {
        sprintf(buffer, "@%s", c->c_attr[i]->name->s_name);
        atoms_get_attribute(argc, argv, gensym(buffer), &defc, &defv);
        if (defc && defv) {
            eobj_attr_setvalueof(x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

void eobj_attrprocess_viabinbuf(t_eobj* x, t_binbuf* d)
{
    char attr_name[MAXPDSTRING];

    int defc = 0;
    t_atom* defv = nullptr;
    t_eclass* c = eobj_getclass(x);
    for (size_t i = 0; i < c->c_nattr; i++) {
        sprintf(attr_name, "@%s", c->c_attr[i]->name->s_name);
        binbuf_get_attribute(d, gensym(attr_name), &defc, &defv);
        if (defc && defv) {
            eobj_attr_setvalueof(x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

void eobj_attr_setvalueof(t_eobj* x, t_symbol* s, size_t argc, t_atom* argv)
{
    eclass_attr_setter(&x->o_obj, s, argc, argv);
}

void eobj_attr_getvalueof(t_eobj* x, t_symbol* s, int* argc, t_atom** argv)
{
    eclass_attr_getter(&x->o_obj, s, argc, argv);
}

void eobj_read(t_eobj* x, t_symbol* s, size_t argc, t_atom* argv)
{
    t_eclass* c = eobj_getclass(x);

    // Name
    if (argc && argv && atom_gettype(argv) == A_SYMBOL) {
        std::string path = ceammc::platform::find_in_std_path(x->o_canvas, atom_getsymbol(argv)->s_name);
        if (path.empty()) {
            pd_error(x, "[%s] can't find file: \"%s\"",
                x->o_obj.te_g.g_pd->c_name->s_name,
                atom_getsymbol(argv)->s_name);

            return;
        }

        if (c->c_widget.w_read)
            c->c_widget.w_read(x, path.c_str());
    }
    // No name so we popup a window
    else {
        if (s == gensym("eobjreadfrom"))
            return;

        sys_vgui("eobj_openfrom %s\n", x->o_id->s_name);
    }
}

static t_edsp* eobj_getdsp(t_eobj* x)
{
    if (x && eobj_isbox(x) && eobj_isdsp(x)) {
        return &((t_edspbox*)x)->d_dsp;
    } else if (x && eobj_isdsp(x)) {
        return &((t_edspobj*)x)->d_dsp;
    } else {
        return NULL;
    }
}

void eobj_dspsetup(t_eobj* x, long nins, long nouts, t_eproxy** inlets, t_outlet** outlets)
{
    t_edsp* dsp = eobj_getdsp(x);
    if (dsp) {
        nins = (long)pd_clip_min(nins, 0);
        nouts = (long)pd_clip_min(nouts, 0);
        dsp->d_sigs_out = NULL;
        dsp->d_sigs_real = NULL;
        dsp->d_perform_method = NULL;
        dsp->d_size = 0;
        dsp->d_vectors = NULL;
        dsp->d_misc = E_INPLACE;

        for (int i = obj_nsigoutlets((t_object*)x), outlet_idx = 0; i < nouts; i++, outlet_idx++) {
            t_outlet* o = outlet_new((t_object*)x, &s_signal);

            if (outlets)
                outlets[outlet_idx] = o;
        }

        for (int i = obj_nsiginlets((t_object*)x), inlet_idx = 0; i < nins; i++, inlet_idx++) {
            t_eproxy* p = eproxy_new(x, &s_signal, &s_signal);

            if (inlets)
                inlets[inlet_idx] = p;
        }
    }
}

void eobj_dspflags(t_eobj* x, long flags)
{
    t_edsp* dsp = eobj_getdsp(x);
    if (dsp) {
        dsp->d_misc = flags;
    }
}

void eobj_dspfree(t_eobj* x)
{
    t_edsp* dsp = eobj_getdsp(x);
    if (dsp) {
        if (dsp->d_sigs_out)
            free(dsp->d_sigs_out);
        if (dsp->d_sigs_real)
            free(dsp->d_sigs_real);
        if (dsp->d_vectors)
            free(dsp->d_vectors);
    }
    eobj_free(x);
}

void eobj_resize_inputs(t_eobj* x, long nins, t_symbol* from, t_symbol* to)
{
    nins = (long)pd_clip_min(nins, 1);
    const int cinlts = obj_ninlets((t_object*)x);

    if (nins > cinlts) {
        for (int i = cinlts; i < nins; i++) {
            eproxy_new(x, from, to);
        }
    } else if (nins < cinlts) {
        for (int i = cinlts; i > nins && x->o_nproxy > 0; --i) {
            eproxy_free(x, x->o_proxy[x->o_nproxy - 1]);
        }
    }

    canvas_fixlinesfor(eobj_getcanvas(x), (t_text*)x);
}

void eobj_dsp(t_eobj* x, t_signal** sp)
{
    int i;
    short* count;
    t_int* temp;
    t_float **tempout, *tempreal;
    t_linetraverser t;
    t_outconnect* oc;
    t_eclass* c = eobj_getclass(x);
    t_edsp* dsp = eobj_getdsp(x);
    int nouts = obj_nsigoutlets((t_object*)x);
    int nins = obj_nsiginlets((t_object*)x);
    int samplesize;
    if (c && c->c_widget.w_dsp && dsp && (nins || nouts) && sp && sp[0]) {
        samplesize = sp[0]->s_n;
        dsp->d_perform_method = NULL;
        if (dsp->d_misc == E_NO_INPLACE) {
            if (dsp->d_sigs_out) {
                tempout = (t_float**)realloc(dsp->d_sigs_out, (size_t)nouts * sizeof(t_float*));
            } else {
                tempout = (t_float**)malloc((size_t)nouts * sizeof(t_float*));
            }
            if (!tempout) {
                if (dsp->d_sigs_out) {
                    free(dsp->d_sigs_out);
                    dsp->d_sigs_out = NULL;
                }
                pd_error(dsp, "can't allocate memory for ni inpace processing.");
                return;
            }
            dsp->d_sigs_out = tempout;

            if (dsp->d_sigs_real) {
                tempreal = (t_float*)realloc(dsp->d_sigs_real, (size_t)(nouts * samplesize) * sizeof(t_float));
            } else {
                tempreal = (t_float*)malloc((size_t)(nouts * samplesize) * sizeof(t_float));
            }
            if (!tempreal) {
                if (dsp->d_sigs_real) {
                    free(dsp->d_sigs_real);
                    dsp->d_sigs_real = NULL;
                }
                free(dsp->d_sigs_out);
                dsp->d_sigs_out = NULL;
                pd_error(dsp, "can't allocate memory for ni inpace processing.");
                return;
            }
            dsp->d_sigs_real = tempreal;
            for (i = 0; i < nouts; i++) {
                dsp->d_sigs_out[i] = dsp->d_sigs_real + i * samplesize;
            }
        }
        if (dsp->d_vectors) {
            temp = (t_int*)realloc(dsp->d_vectors, (size_t)(nins + nouts + 7) * sizeof(t_int));
        } else {
            temp = (t_int*)malloc((size_t)(nins + nouts + 7) * sizeof(t_int));
        }
        if (!temp) {
            if (dsp->d_vectors) {
                free(dsp->d_vectors);
                dsp->d_vectors = NULL;
            }
            free(dsp->d_sigs_real);
            dsp->d_sigs_real = NULL;
            free(dsp->d_sigs_out);
            dsp->d_sigs_out = NULL;
            dsp->d_size = 0;
            pd_error(x, "can't allocate memory for dsp vector.");
            return;
        }
        dsp->d_vectors = temp;
        dsp->d_size = nins + nouts + 7;
        count = (short*)malloc((size_t)(nins + nouts) * sizeof(short));
        if (count) {
            for (i = 0; i < (nins + nouts); i++) {
                count[i] = 0;
            }
            linetraverser_start(&t, eobj_getcanvas(x));
            while ((oc = linetraverser_next(&t))) {
                if (t.tr_ob2 == &x->o_obj && obj_issignaloutlet(t.tr_ob, t.tr_outno)) {
                    count[t.tr_inno] = 1;
                }
            }

            dsp->d_vectors[0] = (t_int)x;
            dsp->d_vectors[1] = (t_int)dsp;
            dsp->d_vectors[2] = (t_int)sp[0]->s_n;
            dsp->d_vectors[3] = (t_int)dsp->d_flags;
            dsp->d_vectors[4] = (t_int)dsp->d_user_param;
            dsp->d_vectors[5] = (t_int)nins;
            dsp->d_vectors[6] = (t_int)nouts;

            for (i = 7; i < dsp->d_size; i++) {
                if (sp[i - 7] && sp[i - 7]->s_vec) {
                    dsp->d_vectors[i] = (t_int)(sp[i - 7]->s_vec);
                } else {
                    free(count);
                    free(dsp->d_vectors);
                    dsp->d_vectors = NULL;
                    free(dsp->d_sigs_real);
                    dsp->d_sigs_real = NULL;
                    free(dsp->d_sigs_out);
                    dsp->d_sigs_out = NULL;
                    dsp->d_size = 0;
                    pd_error(x, "one of the signal isn't allocated.");
                    return;
                }
            }

            c->c_widget.w_dsp(x, x, count, sp[0]->s_sr, sp[0]->s_n, 0);
            if (dsp->d_perform_method != NULL && dsp->d_misc == E_INPLACE) {
                dsp_addv(eobj_perform_inplace, (int)dsp->d_size, dsp->d_vectors);
            } else if (dsp->d_perform_method != NULL && dsp->d_misc == E_NO_INPLACE) {
                dsp_addv(eobj_perform_noinplace, (int)dsp->d_size, dsp->d_vectors);
            }

            free(count);
            return;
        } else {
            free(dsp->d_vectors);
            dsp->d_vectors = NULL;
            free(dsp->d_sigs_real);
            dsp->d_sigs_real = NULL;
            free(dsp->d_sigs_out);
            dsp->d_sigs_out = NULL;
            dsp->d_size = 0;
            pd_error(x, "can't allocate memory for dsp chain counter.");
        }
    }
}

t_sample* eobj_getsignalinput(t_eobj* x, long index)
{
    t_edsp* dsp = eobj_getdsp(x);
    if (dsp) {
        if (index < obj_nsiginlets((t_object*)x) && index >= 0 && dsp->d_vectors) {
            return (t_sample*)dsp->d_vectors[7 + index];
        }
    }
    return NULL;
}

t_sample* eobj_getsignaloutput(t_eobj* x, long index)
{
    t_edsp* dsp = eobj_getdsp(x);
    if (dsp) {
        if (index < obj_nsigoutlets((t_object*)x) && index >= 0 && dsp->d_vectors) {
            if (dsp->d_misc == E_NO_INPLACE)
                return dsp->d_sigs_out[index];
            else
                return (t_sample*)dsp->d_vectors[index + 7 + obj_nsiginlets((t_object*)x)];
        }
    }
    return NULL;
}

t_int* eobj_perform_inplace(t_int* w)
{
    t_eobj* x = (t_eobj*)(w[1]);
    t_edsp* dsp = (t_edsp*)(w[2]);
    long nsamples = (long)(w[3]);
    long flag = (long)(w[4]);
    void* user_p = (void*)(w[5]);
    long nins = (long)(w[6]);
    long nouts = (long)(w[7]);
    t_sample** ins = (t_sample**)(&w[8]);
    t_sample** outs = (t_sample**)(&w[8 + nins]);

    dsp->d_perform_method((void*)x, (t_object*)dsp, ins, nins, outs, nouts, nsamples, flag, user_p);

    return w + (dsp->d_size + 1);
}

t_int* eobj_perform_noinplace(t_int* w)
{
    int i;
    t_eobj* x = (t_eobj*)(w[1]);
    t_edsp* dsp = (t_edsp*)(w[2]);
    long nsamples = (long)(w[3]);
    long flag = (long)(w[4]);
    void* user_p = (void*)(w[5]);
    long nins = (long)(w[6]);
    long nouts = (long)(w[7]);
    t_sample** ins = (t_sample**)(&w[8]);
    t_sample** outs = (t_sample**)(&w[8 + nins]);

    for (i = 0; i < nouts; i++) {
        memcpy(outs[i], dsp->d_sigs_out[i], (size_t)nsamples * sizeof(t_float));
    }

    dsp->d_perform_method((void*)x, (t_object*)dsp, ins, nins, outs, nouts, nsamples, flag, user_p);

    return w + (dsp->d_size + 1);
}

void eobj_dsp_add(t_eobj* x, t_symbol* s, t_object* za, t_typ_method m, long flags, void* userparam)
{
    t_edsp* dsp = eobj_getdsp(x);
    if (dsp) {
        dsp->d_flags = flags;
        dsp->d_user_param = userparam;
        dsp->d_perform_method = m;
    }
}

//! @cond
union inletunion {
    t_symbol* iu_symto;
    t_gpointer* iu_pointerslot;
    t_float* iu_floatslot;
    t_symbol** iu_symslot;
    t_float iu_floatsignalvalue;
};

struct _inlet {
    t_pd i_pd;
    struct _inlet* i_next;
    t_object* i_owner;
    t_pd* i_dest;
    t_symbol* i_symfrom;
    union inletunion i_un;
};

struct _outlet {
    t_object* o_owner;
    struct _outlet* o_next;
    t_outconnect* o_connections;
    t_symbol* o_sym;
};
//! @endcond

static t_class* eproxy_class;

static void inlet_wrong(t_inlet* x, t_symbol* s)
{
    pd_error(x->i_owner, "inlet: expected '%s' but got '%s'",
        x->i_symfrom->s_name, s->s_name);
}

static void new_inlet_list(t_inlet* x, t_symbol* s, int argc, t_atom* argv);

static void new_inlet_bang(t_inlet* x)
{
    if (x->i_symfrom == &s_bang)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "");
    else if (!x->i_symfrom)
        pd_bang(x->i_dest);
    else if (x->i_symfrom == &s_list)
        new_inlet_list(x, &s_bang, 0, 0);
    else if (*x->i_dest == eproxy_class) {
        t_eproxy* proxy = (t_eproxy*)x->i_dest;
        t_eobj* z = (t_eobj*)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        pd_typedmess((t_pd*)proxy->p_owner, &s_bang, 0, NULL);
        z->o_current_proxy = 0;
    } else
        inlet_wrong(x, &s_bang);
}

static void new_inlet_pointer(t_inlet* x, t_gpointer* gp)
{
    if (x->i_symfrom == &s_pointer)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "p", gp);
    else if (!x->i_symfrom)
        pd_pointer(x->i_dest, gp);
    else if (x->i_symfrom == &s_list) {
        t_atom a;
        SETPOINTER(&a, gp);
        new_inlet_list(x, &s_pointer, 1, &a);
    } else if (*x->i_dest == eproxy_class) {
        t_atom a;
        t_eproxy* proxy = (t_eproxy*)x->i_dest;
        t_eobj* z = (t_eobj*)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        SETPOINTER(&a, gp);
        pd_typedmess((t_pd*)x->i_dest, &s_pointer, 1, &a);
        z->o_current_proxy = 0;
    } else
        inlet_wrong(x, &s_pointer);
}

static void new_inlet_float(t_inlet* x, t_float f)
{
    if (x->i_symfrom == &s_float)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "f", (t_floatarg)f);
    else if (!x->i_symfrom) {
        pd_float(x->i_dest, f);
    } else if (x->i_symfrom == &s_list) {
        t_atom a;
        SETFLOAT(&a, f);
        new_inlet_list(x, &s_float, 1, &a);
    } else if (*x->i_dest == eproxy_class) {
        t_atom a;
        t_eproxy* proxy = (t_eproxy*)x->i_dest;
        t_eobj* z = (t_eobj*)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        SETFLOAT(&a, f);
        pd_typedmess((t_pd*)x->i_dest, &s_float, 1, &a);
        z->o_current_proxy = 0;
    } else if (x->i_symfrom == &s_signal) {
        x->i_un.iu_floatsignalvalue = f;
    } else
        inlet_wrong(x, &s_float);
}

static void new_inlet_symbol(t_inlet* x, t_symbol* s)
{
    if (x->i_symfrom == &s_symbol)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "s", s);
    else if (!x->i_symfrom)
        pd_symbol(x->i_dest, s);
    else if (x->i_symfrom == &s_list) {
        t_atom a;
        SETSYMBOL(&a, s);
        new_inlet_list(x, &s_symbol, 1, &a);
    } else if (*x->i_dest == eproxy_class) {
        t_atom a;
        t_eproxy* proxy = (t_eproxy*)x->i_dest;
        t_eobj* z = (t_eobj*)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        SETSYMBOL(&a, s);
        pd_typedmess((t_pd*)x->i_dest, &s_symbol, 1, &a);
        z->o_current_proxy = 0;
    } else
        inlet_wrong(x, &s_symbol);
}

static void new_inlet_list(t_inlet* x, t_symbol* s, int argc, t_atom* argv)
{
    if (x->i_symfrom == &s_list || x->i_symfrom == &s_float || x->i_symfrom == &s_symbol || x->i_symfrom == &s_pointer)
        typedmess(x->i_dest, x->i_un.iu_symto, argc, argv);
    else if (*x->i_dest == eproxy_class) {
        t_eproxy* proxy = (t_eproxy*)x->i_dest;
        t_eobj* z = (t_eobj*)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        pd_typedmess((t_pd*)x->i_dest, s, argc, argv);
        z->o_current_proxy = 0;
    } else if (!x->i_symfrom) {
        typedmess(x->i_dest, s, argc, argv);
        //pd_list(x->i_dest, s, argc, argv);
    } else if (!argc)
        new_inlet_bang(x);
    else if (argc == 1 && argv->a_type == A_FLOAT)
        new_inlet_float(x, atom_getfloat(argv));
    else if (argc == 1 && argv->a_type == A_SYMBOL)
        new_inlet_symbol(x, atom_getsymbol(argv));
    else
        inlet_wrong(x, &s_list);
}

static void new_inlet_anything(t_inlet* x, t_symbol* s, int argc, t_atom* argv)
{
    if (x->i_symfrom == s)
        typedmess(x->i_dest, x->i_un.iu_symto, argc, argv);
    else if (!x->i_symfrom)
        typedmess(x->i_dest, s, argc, argv);
    else if (*x->i_dest == eproxy_class) {
        t_eproxy* proxy = (t_eproxy*)x->i_dest;
        t_eobj* z = (t_eobj*)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        pd_typedmess((t_pd*)x->i_dest, s, argc, argv);
        z->o_current_proxy = 0;
    } else
        inlet_wrong(x, s);
}

static void eproxy_anything(t_eproxy* x, t_symbol* s, int argc, t_atom* argv)
{
    t_eobj* z = (t_eobj*)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_typedmess((t_pd*)x->p_owner, s, argc, argv);
}

static void eproxy_bang(t_eproxy* x)
{
    t_eobj* z = (t_eobj*)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_bang((t_pd*)x->p_owner);
}

static void eproxy_float(t_eproxy* x, float f)
{
    t_eobj* z = (t_eobj*)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_float((t_pd*)x->p_owner, f);
}

static void eproxy_symbol(t_eproxy* x, t_symbol* s)
{
    t_eobj* z = (t_eobj*)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_symbol((t_pd*)x->p_owner, s);
}

static void eproxy_list(t_eproxy* x, t_symbol* s, int argc, t_atom* argv)
{
    t_eobj* z = (t_eobj*)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_list((t_pd*)x->p_owner, s, argc, argv);
}

static t_class* eproxy_setup()
{
    t_symbol* eproxy1572_sym = gensym("eproxy1572");
    if (!eproxy1572_sym->s_thing) {
        eproxy_class = class_new(gensym("eproxy"), NULL, (t_method)NULL, sizeof(t_eproxy), CLASS_PD, A_GIMME, 0);
        eproxy1572_sym->s_thing = (t_class**)eproxy_class;
        class_addanything(eproxy_class, (t_method)eproxy_anything);
        class_addbang(eproxy_class, (t_method)eproxy_bang);
        class_doaddfloat(eproxy_class, (t_method)eproxy_float);
        class_addsymbol(eproxy_class, (t_method)eproxy_symbol);
        class_addlist(eproxy_class, (t_method)eproxy_list);
        return eproxy_class;
    } else {
        return (t_class*)eproxy1572_sym->s_thing;
    }
}

static t_inlet* einlet_new(t_object* owner, t_pd* proxy, t_symbol* from_sym, t_symbol* to_sym)
{
    t_class* inlet_class;
    t_inlet* inlet = inlet_new((t_object*)owner, (t_pd*)proxy, from_sym, to_sym);
    if (inlet) {
        inlet_class = inlet->i_pd;
        inlet_class->c_bangmethod = (t_bangmethod)new_inlet_bang;
        inlet_class->c_pointermethod = (t_pointermethod)new_inlet_pointer;
        inlet_class->c_floatmethod = (t_floatmethod)new_inlet_float;
        inlet_class->c_symbolmethod = (t_symbolmethod)new_inlet_symbol;
        inlet_class->c_listmethod = (t_listmethod)new_inlet_list;
        inlet_class->c_anymethod = (t_anymethod)new_inlet_anything;
    }
    return inlet;
}

static t_eproxy* eproxy_new(t_eobj* owner, t_symbol* sym_from, t_symbol* sym_to)
{
    t_eproxy* proxy;
    t_eproxy** temp;
    t_edsp* dsp = eobj_getdsp(owner);
    eproxy_class = eproxy_setup();
    if (owner->o_proxy) {
        temp = (t_eproxy**)realloc(owner->o_proxy, (size_t)(owner->o_nproxy + 1) * sizeof(t_eproxy*));
    } else {
        temp = (t_eproxy**)malloc(1 * sizeof(t_eproxy*));
    }
    if (temp) {
        owner->o_proxy = temp;
        proxy = (t_eproxy*)pd_new(eproxy_class);
        proxy->p_owner = &owner->o_obj;
        proxy->p_pd = eproxy_class;
        proxy->p_index = owner->o_nproxy;
        proxy->p_inlet = einlet_new(&owner->o_obj, (t_pd*)proxy, sym_from, sym_to);
        if (dsp) {
            if (sym_from == sym_to && sym_to == &s_signal)
                proxy->p_inlet->i_un.iu_floatsignalvalue = dsp->d_float;
            else
                proxy->p_inlet->i_un.iu_symto = sym_to;
        }

        owner->o_proxy[owner->o_nproxy] = proxy;
        owner->o_nproxy++;
        return proxy;
    } else {
        pd_error(owner, "can't allocate memory for a new proxy inlet.");
        return NULL;
    }
}

static void canvas_deletelines_for_io(t_canvas* x, t_text* text, t_inlet* inp, t_outlet* outp)
{
    t_linetraverser t;
    t_outconnect* oc;
    linetraverser_start(&t, x);
    while ((oc = linetraverser_next(&t))) {
        if ((t.tr_ob == text && t.tr_outlet == outp) || (t.tr_ob2 == text && t.tr_inlet == inp)) {
            if (glist_isvisible(x)) {
                sys_vgui(".x%lx.c delete l%lx\n", glist_getcanvas(x), oc);
            }
            obj_disconnect(t.tr_ob, t.tr_outno, t.tr_ob2, t.tr_inno);
        }
    }
}

static void eproxy_free(t_eobj* owner, t_eproxy* proxy)
{
    if (owner && proxy && proxy->p_owner == (t_object*)owner) {
        if (owner->o_nproxy == proxy->p_index + 1) {
            canvas_deletelines_for_io(eobj_getcanvas(owner), (t_text*)owner, proxy->p_inlet, NULL);
            inlet_free(proxy->p_inlet);
            if (owner->o_nproxy == 1) {
                free(owner->o_proxy);
                owner->o_proxy = nullptr;
                owner->o_nproxy = 0;
            } else {
                t_eproxy** temp = (t_eproxy**)realloc(owner->o_proxy, (size_t)(owner->o_nproxy - 1) * sizeof(t_eproxy*));
                if (temp) {
                    owner->o_proxy = temp;
                } else {
                    pd_error(owner, "a proxy hasn't been freed.");
                }
                owner->o_nproxy--;
            }
        } else {
            pd_error(owner, "can't free a proxy if it's not the last one.");
        }
    }
}

//! @endcond
