/** \brief Copyright (c) 1997-1999 Miller Puckette.
* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

/** \brief This file contains function prototypes and data types used to implement
Pd, but not shared with Pd objects. */

/** \brief NOTE: this file describes Pd implementation details which may change
in future releases.  The public (stable) API is in m_pd.h. */

/** \brief LATER consider whether to use 'char' for method arg types to save space */
#ifndef __m_imp_h_

/** \defgroup g_I1 m_imp : basic
 * @{ */

/** \brief the structure for a method handler ala Max */
typedef struct _methodentry
{
    t_symbol *me_name;
    t_gotfn me_fun;
    t_atomtype me_arg[MAXPDARG+1];
} t_methodentry;

EXTERN_STRUCT _widgetbehavior;

typedef void (*t_bangmethod)(t_pd *x);
typedef void (*t_pointermethod)(t_pd *x, t_gpointer *gp);
typedef void (*t_floatmethod)(t_pd *x, t_float f);
typedef void (*t_symbolmethod)(t_pd *x, t_symbol *s);
typedef void (*t_listmethod)(t_pd *x, t_symbol *s, int argc, t_atom *argv);
typedef void (*t_anymethod)(t_pd *x, t_symbol *s, int argc, t_atom *argv);

struct _class
{
    /** \brief name (mostly for error reporting) */
    t_symbol *c_name;
    /** \brief name of help file */
    t_symbol *c_helpname;
    /** \brief directory extern was loaded from */
    t_symbol *c_externdir;
    /** \brief size of an instance */
    size_t c_size;
    /** \brief methods other than bang, etc below */
    t_methodentry *c_methods;
    /** \brief number of methods */
    int c_nmethod;
    /** \brief function to call before freeing */
    t_method c_freemethod;
    /** \brief common methods */
    t_bangmethod c_bangmethod;
    t_pointermethod c_pointermethod;
    t_floatmethod c_floatmethod;
    t_symbolmethod c_symbolmethod;
    t_listmethod c_listmethod;
    t_anymethod c_anymethod;
    /** \brief "gobjs" only */
    struct _widgetbehavior *c_wb;
    /** \brief widget behavior in parent */
    struct _parentwidgetbehavior *c_pwb;
    /** \brief function to call when saving */
    t_savefn c_savefn;
    /** \brief function to start prop dialog */
    t_propertiesfn c_propertiesfn;
    /** \brief onset to float for signal input */
    int c_floatsignalin;
    /** \brief true if is a gobj */
    char c_gobj;
    /** \brief true if we have a t_object header */
    char c_patchable;
    /** \brief if patchable, true if draw first inlet */
    char c_firstin;
    /** \brief a drawing command for a template */
    char c_drawcommand;
};

struct _pdinstance
{
    /** \brief global time in Pd ticks */
    double pd_systime;
    /** \brief list of set clocks */
    t_clock *pd_clock_setlist;
    /** \brief DSP chain */
    t_int *pd_dspchain;
    /** \brief number of elements in DSP chain */
    int pd_dspchainsize;
    /** \brief list of all root canvases */
    t_canvas *pd_canvaslist;
    /** \brief whether DSP is on or off */
    int pd_dspstate;
    /** \brief list of signals used by DSP chain */
    t_signal *pd_signals;
    /** \brief symbols bound to incoming MIDI... */
    t_symbol *pd_midiin_sym;
    t_symbol *pd_sysexin_sym;
    t_symbol *pd_notein_sym;
    t_symbol *pd_ctlin_sym;
    t_symbol *pd_pgmin_sym;
    t_symbol *pd_bendin_sym;
    t_symbol *pd_touchin_sym;
    t_symbol *pd_polytouchin_sym;
    t_symbol *pd_midiclkin_sym;
    t_symbol *pd_midirealtimein_sym;
};

extern t_pdinstance *pd_this;
/* @} */

/** \defgroup g_I2 m_imp : m_class.c
 * @{ */
EXTERN void pd_emptylist(t_pd *x);
/* @} */

/** \defgroup g_I3 m_imp : m_obj.c
 * @{ */
EXTERN int obj_noutlets(t_object *x);
EXTERN int obj_ninlets(t_object *x);
EXTERN t_outconnect *obj_starttraverseoutlet(t_object *x, t_outlet **op,
    int nout);
EXTERN t_outconnect *obj_nexttraverseoutlet(t_outconnect *lastconnect,
    t_object **destp, t_inlet **inletp, int *whichp);
EXTERN t_outconnect *obj_connect(t_object *source, int outno,
    t_object *sink, int inno);
EXTERN void obj_disconnect(t_object *source, int outno, t_object *sink,
    int inno);
EXTERN void outlet_setstacklim(void);
EXTERN int obj_issignalinlet(t_object *x, int m);
EXTERN int obj_issignaloutlet(t_object *x, int m);
EXTERN int obj_nsiginlets(t_object *x);
EXTERN int obj_nsigoutlets(t_object *x);
EXTERN int obj_siginletindex(t_object *x, int m);
EXTERN int obj_sigoutletindex(t_object *x, int m);
/* @} */

/** \defgroup g_I4 m_imp : misc
 * @{ */
EXTERN t_pd *glob_evalfile(t_pd *ignore, t_symbol *name, t_symbol *dir);
EXTERN void glob_initfromgui(void *dummy, t_symbol *s, int argc, t_atom *argv);
EXTERN void glob_quit(void *dummy);
EXTERN void open_via_helppath(const char *name, const char *dir);
/* @} */

#define __m_imp_h_
#endif /** \brief __m_imp_h_ */
