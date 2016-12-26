/** \brief Copyright (c) 1997-1999 Miller Puckette.
 * \detail For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

#ifndef __m_pd_h_

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
extern "C" {
#endif
    
#define PD_MAJOR_VERSION 0
#define PD_MINOR_VERSION 47
#define PD_BUGFIX_VERSION 1
#define PD_TEST_VERSION ""
    extern int pd_compatibilitylevel;   /** \brief e.g., 43 for pd 0.43 compatibility */
    
    /** \brief old name for "MSW" flag -- we have to take it for the sake of many old
     "nmakefiles" for externs, which will define NT and not MSW */
#if defined(NT) && !defined(MSW)
#define MSW
#endif
    
    /** \brief These pragmas are only used for MSVC, not MinGW or Cygwin <hans@at.or.at> */
#ifdef _MSC_VER
    /** \brief #pragma warning( disable : 4091 ) */
#pragma warning( disable : 4305 )  /** \brief uncast const double to float */
#pragma warning( disable : 4244 )  /** \brief uncast float/int conversion etc. */
#pragma warning( disable : 4101 )  /** \brief unused automatic variables */
#endif /** \brief _MSC_VER */
    
    /** \brief the external storage class is "extern" in UNIX; in MSW it's ugly. */
#ifdef _WIN32
#ifdef PD_INTERNAL
#define EXTERN __declspec(dllexport) extern
#else
#define EXTERN __declspec(dllimport) extern
#endif /** \brief PD_INTERNAL */
#else
#define EXTERN extern
#endif /** \brief _WIN32 */
    
    /** \brief On most c compilers, you can just say "struct foo;" to declare a
     structure whose elements are defined elsewhere.  On MSVC, when compiling
     C (but not C++) code, you have to say "extern struct foo;".  So we make
     a stupid macro: */
#if defined(_MSC_VER) && !defined(_LANGUAGE_C_PLUS_PLUS) \
&& !defined(__cplusplus)
#define EXTERN_STRUCT extern struct
#else
#define EXTERN_STRUCT struct
#endif
    
    /** \brief Define some attributes, specific to the compiler */
#if defined(__GNUC__)
#define ATTRIBUTE_FORMAT_PRINTF(a, b) __attribute__ ((format (printf, a, b)))
#else
#define ATTRIBUTE_FORMAT_PRINTF(a, b)
#endif
    
#if !defined(_SIZE_T) && !defined(_SIZE_T_)
#include <stddef.h>     /** \brief just for size_t -- how lame! */
#endif
    
    /** \brief Microsoft Visual Studio is not C99, it does not provide stdint.h */
#ifdef _MSC_VER
    typedef signed __int8     int8_t;
    typedef signed __int16    int16_t;
    typedef signed __int32    int32_t;
    typedef signed __int64    int64_t;
    typedef unsigned __int8   uint8_t;
    typedef unsigned __int16  uint16_t;
    typedef unsigned __int32  uint32_t;
    typedef unsigned __int64  uint64_t;
#else
# include <stdint.h>
#endif
    
    /** \brief for FILE, needed by sys_fopen() and sys_fclose() only */
#include <stdio.h>
    
#define MAXPDSTRING 1000        /** \brief use this for anything you want */
#define MAXPDARG 5              /** \brief max number of args we can typecheck today */
    
    /** \brief signed and unsigned integer types the size of a pointer:  */
#if !defined(PD_LONGINTTYPE)
#define PD_LONGINTTYPE long
#endif
    
#if !defined(PD_FLOATSIZE)
    /** \brief normally, our floats (t_float, t_sample,...) are 32bit */
# define PD_FLOATSIZE 32
#endif
    
#if PD_FLOATSIZE == 32
# define PD_FLOATTYPE float
    /** \brief an unsigned int of the same size as FLOATTYPE: */
# define PD_FLOATUINTTYPE unsigned int
    
#elif PD_FLOATSIZE == 64
# define PD_FLOATTYPE double
# define PD_FLOATUINTTYPE unsigned long
#else
# error invalid FLOATSIZE: must be 32 or 64
#endif
    
    /** \brief pointer-size integer */
    typedef PD_LONGINTTYPE t_int;
    /** \brief a float type at most the same size */
    typedef PD_FLOATTYPE t_float;
    /** \brief float type for function calls */
    typedef PD_FLOATTYPE t_floatarg;
    
    typedef struct _symbol
    {
        char *s_name;
        struct _class **s_thing;
        struct _symbol *s_next;
    } t_symbol;
    
    EXTERN_STRUCT _array;
    /** \brief g_canvas.h */
#define t_array struct _array
    
    
    /** \brief the stub points nowhere (has been cut off) */
#define GP_NONE 0
    /** \brief the stub points to a glist element */
#define GP_GLIST 1
    /** \brief ... or array */
#define GP_ARRAY 2
    
    /** \brief pointers to glist and array elements go through a "stub" which sticks
     around after the glist or array is freed.  The stub itself is deleted when
     both the glist/array is gone and the refcount is zero, ensuring that no
     gpointers are pointing here. */
    typedef struct _gstub
    {
        union
        {
            /** \brief glist we're in */
            struct _glist *gs_glist;
            /** \brief array we're in */
            struct _array *gs_array;
        } gs_un;
        /** \brief GP_GLIST/GP_ARRAY */
        int gs_which;
        /** \brief number of gpointers pointing here */
        int gs_refcount;
    } t_gstub;
    
    /** \brief pointer to a gobj in a glist */
    typedef struct _gpointer
    {
        union
        {
            /** \brief scalar we're in (if glist) */
            struct _scalar *gp_scalar;
            /** \brief raw data (if array) */
            union word *gp_w;
        } gp_un;
        /** \brief number which must match gpointee */
        int gp_valid;
        /** \brief stub which points to glist/array */
        t_gstub *gp_stub;
    } t_gpointer;
    
    typedef union word
    {
        t_float w_float;
        t_symbol *w_symbol;
        t_gpointer *w_gpointer;
        t_array *w_array;
        struct _binbuf *w_binbuf;
        int w_index;
    } t_word;
    
    typedef enum
    {
        A_NULL,
        A_FLOAT,
        A_SYMBOL,
        A_POINTER,
        A_SEMI,
        A_COMMA,
        A_DEFFLOAT,
        A_DEFSYM,
        A_DOLLAR,
        A_DOLLSYM,
        A_GIMME,
        A_CANT
    }  t_atomtype;
    
    /** \brief better name for this */
#define A_DEFSYMBOL A_DEFSYM
    
    typedef struct _atom
    {
        t_atomtype a_type;
        union word a_w;
    } t_atom;
    
    EXTERN_STRUCT _class;
#define t_class struct _class
    
    EXTERN_STRUCT _outlet;
#define t_outlet struct _outlet
    
    EXTERN_STRUCT _inlet;
#define t_inlet struct _inlet
    
    EXTERN_STRUCT _binbuf;
#define t_binbuf struct _binbuf
    
    EXTERN_STRUCT _clock;
#define t_clock struct _clock
    
    EXTERN_STRUCT _outconnect;
#define t_outconnect struct _outconnect
    
    EXTERN_STRUCT _glist;
#define t_glist struct _glist
    /** \brief LATER lose this */
#define t_canvas struct _glist
    
    /** \brief pure datum: nothing but a class pointer */
    typedef t_class *t_pd;
    
    /** \brief a graphical object */
    typedef struct _gobj
    {
        /** \brief pure datum header (class) */
        t_pd g_pd;
        /** \brief next in list */
        struct _gobj *g_next;
    } t_gobj;
    
    /** \brief a graphical object holding data */
    typedef struct _scalar
    {
        /** \brief header for graphical object */
        t_gobj sc_gobj;
        /** \brief template name (LATER replace with pointer) */
        t_symbol *sc_template;
        /** \brief indeterminate-length array of words */
        t_word sc_vec[1];
    } t_scalar;
    
    /** \brief patchable object - graphical, with text */
    typedef struct _text
    {
        /** \brief header for graphical object */
        t_gobj te_g;
        /** \brief holder for the text */
        t_binbuf *te_binbuf;
        /** \brief linked list of outlets */
        t_outlet *te_outlet;
        /** \brief linked list of inlets */
        t_inlet *te_inlet;
        /** \brief x location (within the toplevel) */
        short te_xpix;
        /** \brief y location (within the toplevel) */
        short te_ypix;
        /** \brief requested width in chars, 0 if auto */
        short te_width;
        /** \brief from defs below */
        unsigned int te_type:2;
    } t_text;
    
    /** \brief just a textual comment */
#define T_TEXT 0
    /** \brief a MAX style patchable object */
#define T_OBJECT 1
    /** \brief a MAX stype message */
#define T_MESSAGE 2
    /** \brief a cell to display a number or symbol */
#define T_ATOM 3
    
#define te_pd te_g.g_pd
    
    /** \brief t_object is synonym for t_text (LATER unify them) */
    typedef struct _text t_object;
    
#define ob_outlet te_outlet
#define ob_inlet te_inlet
#define ob_binbuf te_binbuf
#define ob_pd te_g.g_pd
#define ob_g te_g
    
    typedef void (*t_method)(void);
    typedef void *(*t_newmethod)( void);
    
    /** \brief in ARM 64 a varargs prototype generates a different function call sequence
     from a fixed one, so in that special case we make a more restrictive
     definition for t_gotfn.  This will break some code in the "chaos" package
     in Pd extended.  (that code will run incorrectly anyhow so why not catch it
     at compile time anyhow.) */
#if defined(__APPLE__) && defined(__aarch64__)
    typedef void (*t_gotfn)(void *x);
#else
    typedef void (*t_gotfn)(void *x, ...);
#endif
    
    /** \defgroup gM1 pre-defined objects and symbols
     * @{
     */
    /** \brief factory for creating "object" boxes */
    EXTERN t_pd pd_objectmaker;
    /** \brief factory for creating canvases */
    EXTERN t_pd pd_canvasmaker;
    EXTERN t_symbol s_pointer;
    EXTERN t_symbol s_float;
    EXTERN t_symbol s_symbol;
    EXTERN t_symbol s_bang;
    EXTERN t_symbol s_list;
    EXTERN t_symbol s_anything;
    EXTERN t_symbol s_signal;
    EXTERN t_symbol s__N;
    EXTERN t_symbol s__X;
    EXTERN t_symbol s_x;
    EXTERN t_symbol s_y;
    EXTERN t_symbol s_;
    /**
     *@}
     */
    
    /** \defgroup gM2 prototypes from the central message system
     * @{
     */
    EXTERN void pd_typedmess(t_pd *x, t_symbol *s, int argc, t_atom *argv);
    EXTERN void pd_forwardmess(t_pd *x, int argc, t_atom *argv);
    EXTERN t_symbol *gensym(const char *s);
    EXTERN t_gotfn getfn(t_pd *x, t_symbol *s);
    EXTERN t_gotfn zgetfn(t_pd *x, t_symbol *s);
    EXTERN void nullfn(void);
    EXTERN void pd_vmess(t_pd *x, t_symbol *s, char *fmt, ...);
    /**
     *@}
     */
    
    /** \brief the following macroses are for sending non-type-checkable mesages, i.e.,
     using function lookup but circumventing type checking on arguments.  Only
     use for internal messaging protected by A_CANT so that the message can't
     be generated at patch level. */
#define mess0(x, s) ((*getfn((x), (s)))((x)))
    typedef void (*t_gotfn1)(void *x, void *arg1);
#define mess1(x, s, a) ((*(t_gotfn1)getfn((x), (s)))((x), (a)))
    typedef void (*t_gotfn2)(void *x, void *arg1, void *arg2);
#define mess2(x, s, a,b) ((*(t_gotfn2)getfn((x), (s)))((x), (a),(b)))
    typedef void (*t_gotfn3)(void *x, void *arg1, void *arg2, void *arg3);
#define mess3(x, s, a,b,c) ((*(t_gotfn3)getfn((x), (s)))((x), (a),(b),(c)))
    typedef void (*t_gotfn4)(void *x,
    void *arg1, void *arg2, void *arg3, void *arg4);
#define mess4(x, s, a,b,c,d) \
((*(t_gotfn4)getfn((x), (s)))((x), (a),(b),(c),(d)))
    typedef void (*t_gotfn5)(void *x,
    void *arg1, void *arg2, void *arg3, void *arg4, void *arg5);
#define mess5(x, s, a,b,c,d,e) \
((*(t_gotfn5)getfn((x), (s)))((x), (a),(b),(c),(d),(e)))
    
    EXTERN void obj_list(t_object *x, t_symbol *s, int argc, t_atom *argv);
    EXTERN t_pd *pd_newest(void);
    
    /** \defgroup gM3 memory management
     * @{
     */
    EXTERN void *getbytes(size_t nbytes);
    EXTERN void *getzbytes(size_t nbytes);
    EXTERN void *copybytes(void *src, size_t nbytes);
    EXTERN void freebytes(void *x, size_t nbytes);
    EXTERN void *resizebytes(void *x, size_t oldsize, size_t newsize);
    /**
     *@}
     */
    
    /** \defgroup gM4 atoms
     * @{
     */
    
#define SETSEMI(atom) ((atom)->a_type = A_SEMI, (atom)->a_w.w_index = 0)
#define SETCOMMA(atom) ((atom)->a_type = A_COMMA, (atom)->a_w.w_index = 0)
#define SETPOINTER(atom, gp) ((atom)->a_type = A_POINTER, \
(atom)->a_w.w_gpointer = (gp))
#define SETFLOAT(atom, f) ((atom)->a_type = A_FLOAT, (atom)->a_w.w_float = (f))
#define SETSYMBOL(atom, s) ((atom)->a_type = A_SYMBOL, \
(atom)->a_w.w_symbol = (s))
#define SETDOLLAR(atom, n) ((atom)->a_type = A_DOLLAR, \
(atom)->a_w.w_index = (n))
#define SETDOLLSYM(atom, s) ((atom)->a_type = A_DOLLSYM, \
(atom)->a_w.w_symbol= (s))
    
    EXTERN t_float atom_getfloat(t_atom *a);
    EXTERN t_int atom_getint(t_atom *a);
    EXTERN t_symbol *atom_getsymbol(t_atom *a);
    EXTERN t_symbol *atom_gensym(t_atom *a);
    EXTERN t_float atom_getfloatarg(int which, int argc, t_atom *argv);
    EXTERN t_int atom_getintarg(int which, int argc, t_atom *argv);
    EXTERN t_symbol *atom_getsymbolarg(int which, int argc, t_atom *argv);
    
    EXTERN void atom_string(t_atom *a, char *buf, unsigned int bufsize);
    /**
     *@}
     */
    
    /** \defgroup gM5 binbufs
     * @{
     */
    
    EXTERN t_binbuf *binbuf_new(void);
    EXTERN void binbuf_free(t_binbuf *x);
    EXTERN t_binbuf *binbuf_duplicate(t_binbuf *y);
    
    EXTERN void binbuf_text(t_binbuf *x, char *text, size_t size);
    EXTERN void binbuf_gettext(t_binbuf *x, char **bufp, int *lengthp);
    EXTERN void binbuf_clear(t_binbuf *x);
    EXTERN void binbuf_add(t_binbuf *x, int argc, t_atom *argv);
    EXTERN void binbuf_addv(t_binbuf *x, char *fmt, ...);
    EXTERN void binbuf_addbinbuf(t_binbuf *x, t_binbuf *y);
    EXTERN void binbuf_addsemi(t_binbuf *x);
    EXTERN void binbuf_restore(t_binbuf *x, int argc, t_atom *argv);
    EXTERN void binbuf_print(t_binbuf *x);
    EXTERN int binbuf_getnatom(t_binbuf *x);
    EXTERN t_atom *binbuf_getvec(t_binbuf *x);
    EXTERN int binbuf_resize(t_binbuf *x, int newsize);
    EXTERN void binbuf_eval(t_binbuf *x, t_pd *target, int argc, t_atom *argv);
    EXTERN int binbuf_read(t_binbuf *b, char *filename, char *dirname,
                           int crflag);
    EXTERN int binbuf_read_via_canvas(t_binbuf *b, char *filename, t_canvas *canvas,
                                      int crflag);
    EXTERN int binbuf_read_via_path(t_binbuf *b, char *filename, char *dirname,
                                    int crflag);
    EXTERN int binbuf_write(t_binbuf *x, char *filename, char *dir,
                            int crflag);
    EXTERN void binbuf_evalfile(t_symbol *name, t_symbol *dir);
    EXTERN t_symbol *binbuf_realizedollsym(t_symbol *s, int ac, t_atom *av,
                                           int tonew);
    /**
     *@}
     */
    
    /** \defgroup gM6 clocks
     * @{
     */
    
    EXTERN t_clock *clock_new(void *owner, t_method fn);
    EXTERN void clock_set(t_clock *x, double systime);
    EXTERN void clock_delay(t_clock *x, double delaytime);
    EXTERN void clock_unset(t_clock *x);
    EXTERN void clock_setunit(t_clock *x, double timeunit, int sampflag);
    EXTERN double clock_getlogicaltime(void);
    /** \deprecated OBSOLETE; use clock_getlogicaltime() */
    EXTERN double clock_getsystime(void);
    EXTERN double clock_gettimesince(double prevsystime);
    EXTERN double clock_gettimesincewithunits(double prevsystime,
                                              double units, int sampflag);
    EXTERN double clock_getsystimeafter(double delaytime);
    EXTERN void clock_free(t_clock *x);
    /**
     *@}
     */
    
    /** \defgroup gM7 puredata
     * @{
     */
    EXTERN t_pd *pd_new(t_class *cls);
    EXTERN void pd_free(t_pd *x);
    EXTERN void pd_bind(t_pd *x, t_symbol *s);
    EXTERN void pd_unbind(t_pd *x, t_symbol *s);
    EXTERN t_pd *pd_findbyclass(t_symbol *s, t_class *c);
    EXTERN void pd_pushsym(t_pd *x);
    EXTERN void pd_popsym(t_pd *x);
    EXTERN t_symbol *pd_getfilename(void);
    EXTERN t_symbol *pd_getdirname(void);
    EXTERN void pd_bang(t_pd *x);
    EXTERN void pd_pointer(t_pd *x, t_gpointer *gp);
    EXTERN void pd_float(t_pd *x, t_float f);
    EXTERN void pd_symbol(t_pd *x, t_symbol *s);
    EXTERN void pd_list(t_pd *x, t_symbol *s, int argc, t_atom *argv);
    EXTERN void pd_anything(t_pd *x, t_symbol *s, int argc, t_atom *argv);
#define pd_class(x) (*(x))
    /**
     *@}
     */
    
    /** \defgroup gM8 pointers
     * @{
     */
    EXTERN void gpointer_init(t_gpointer *gp);
    EXTERN void gpointer_copy(const t_gpointer *gpfrom, t_gpointer *gpto);
    EXTERN void gpointer_unset(t_gpointer *gp);
    EXTERN int gpointer_check(const t_gpointer *gp, int headok);
    /**
     *@}
     */
    
    /** \defgroup gM9 patchable "objects"
     * @{
     */
    EXTERN t_inlet *inlet_new(t_object *owner, t_pd *dest, t_symbol *s1,
                              t_symbol *s2);
    EXTERN t_inlet *pointerinlet_new(t_object *owner, t_gpointer *gp);
    EXTERN t_inlet *floatinlet_new(t_object *owner, t_float *fp);
    EXTERN t_inlet *symbolinlet_new(t_object *owner, t_symbol **sp);
    EXTERN t_inlet *signalinlet_new(t_object *owner, t_float f);
    EXTERN void inlet_free(t_inlet *x);
    
    EXTERN t_outlet *outlet_new(t_object *owner, t_symbol *s);
    EXTERN void outlet_bang(t_outlet *x);
    EXTERN void outlet_pointer(t_outlet *x, t_gpointer *gp);
    EXTERN void outlet_float(t_outlet *x, t_float f);
    EXTERN void outlet_symbol(t_outlet *x, t_symbol *s);
    EXTERN void outlet_list(t_outlet *x, t_symbol *s, int argc, t_atom *argv);
    EXTERN void outlet_anything(t_outlet *x, t_symbol *s, int argc, t_atom *argv);
    EXTERN t_symbol *outlet_getsymbol(t_outlet *x);
    EXTERN void outlet_free(t_outlet *x);
    EXTERN t_object *pd_checkobject(t_pd *x);
    /**
     *@}
     */
    
    /** \defgroup gM10 canvases
     * @{
     */
    
    EXTERN void glob_setfilename(void *dummy, t_symbol *name, t_symbol *dir);
    
    EXTERN void canvas_setargs(int argc, t_atom *argv);
    EXTERN void canvas_getargs(int *argcp, t_atom **argvp);
    EXTERN t_symbol *canvas_getcurrentdir(void);
    EXTERN t_glist *canvas_getcurrent(void);
    EXTERN void canvas_makefilename(t_glist *c, char *file,
                                    char *result,int resultsize);
    EXTERN t_symbol *canvas_getdir(t_glist *x);
    /** \brief default typeface set in s_main.c */
    EXTERN char sys_font[];
    /** \brief default font weight set in s_main.c */
    EXTERN char sys_fontweight[];
    EXTERN int sys_zoomfontwidth(int fontsize, int zoom, int worstcase);
    EXTERN int sys_zoomfontheight(int fontsize, int zoom, int worstcase);
    EXTERN int sys_fontwidth(int fontsize);
    EXTERN int sys_fontheight(int fontsize);
    EXTERN void canvas_dataproperties(t_glist *x, t_scalar *sc, t_binbuf *b);
    EXTERN int canvas_open(t_canvas *x, const char *name, const char *ext,
                           char *dirresult, char **nameresult, unsigned int size, int bin);
    /**
     *@}
     */
    
    /** \defgroup gM11 widget behaviours
     * @{
     */
    
    EXTERN_STRUCT _widgetbehavior;
#define t_widgetbehavior struct _widgetbehavior
    
    EXTERN_STRUCT _parentwidgetbehavior;
#define t_parentwidgetbehavior struct _parentwidgetbehavior
    EXTERN t_parentwidgetbehavior *pd_getparentwidget(t_pd *x);
    /**
     *@}
     */
    
    /** \defgroup gM12 classes
     * @{
     */
    
    /** \brief flags for new classes below */
#define CLASS_DEFAULT 0
#define CLASS_PD 1
#define CLASS_GOBJ 2
#define CLASS_PATCHABLE 3
#define CLASS_NOINLET 8
    
#define CLASS_TYPEMASK 3
    
    
    EXTERN t_class *class_new(t_symbol *name, t_newmethod newmethod,
                              t_method freemethod, size_t size, int flags, t_atomtype arg1, ...);
    EXTERN void class_addcreator(t_newmethod newmethod, t_symbol *s,
                                 t_atomtype type1, ...);
    EXTERN void class_addmethod(t_class *c, t_method fn, t_symbol *sel,
                                t_atomtype arg1, ...);
    EXTERN void class_addbang(t_class *c, t_method fn);
    EXTERN void class_addpointer(t_class *c, t_method fn);
    EXTERN void class_doaddfloat(t_class *c, t_method fn);
    EXTERN void class_addsymbol(t_class *c, t_method fn);
    EXTERN void class_addlist(t_class *c, t_method fn);
    EXTERN void class_addanything(t_class *c, t_method fn);
    EXTERN void class_sethelpsymbol(t_class *c, t_symbol *s);
    EXTERN void class_setwidget(t_class *c, t_widgetbehavior *w);
    EXTERN void class_setparentwidget(t_class *c, t_parentwidgetbehavior *w);
    EXTERN t_parentwidgetbehavior *class_parentwidget(t_class *c);
    EXTERN char *class_getname(t_class *c);
    EXTERN char *class_gethelpname(t_class *c);
    EXTERN char *class_gethelpdir(t_class *c);
    EXTERN void class_setdrawcommand(t_class *c);
    EXTERN int class_isdrawcommand(t_class *c);
    EXTERN void class_domainsignalin(t_class *c, int onset);
    EXTERN void class_set_extern_dir(t_symbol *s);
#define CLASS_MAINSIGNALIN(c, type, field) \
class_domainsignalin(c, (char *)(&((type *)0)->field) - (char *)0)
    /**
     *@}
     */
    
    /** \defgroup gM13 prototype for functions to save Pd's to a binbuf
     * @{
     */
    
    typedef void (*t_savefn)(t_gobj *x, t_binbuf *b);
    EXTERN void class_setsavefn(t_class *c, t_savefn f);
    EXTERN t_savefn class_getsavefn(t_class *c);
    /** \brief add format to bb */
    EXTERN void obj_saveformat(t_object *x, t_binbuf *bb);
    /**
     *@}
     */
    
    /** \defgroup gM14 prototype for functions to open properties dialogs
     * @{
     */
    
    typedef void (*t_propertiesfn)(t_gobj *x, struct _glist *glist);
    EXTERN void class_setpropertiesfn(t_class *c, t_propertiesfn f);
    EXTERN t_propertiesfn class_getpropertiesfn(t_class *c);
    
#ifndef PD_CLASS_DEF
#define class_addbang(x, y) class_addbang((x), (t_method)(y))
#define class_addpointer(x, y) class_addpointer((x), (t_method)(y))
#define class_addfloat(x, y) class_doaddfloat((x), (t_method)(y))
#define class_addsymbol(x, y) class_addsymbol((x), (t_method)(y))
#define class_addlist(x, y) class_addlist((x), (t_method)(y))
#define class_addanything(x, y) class_addanything((x), (t_method)(y))
#endif
    /**
     *@}
     */
    
    /** \defgroup gM15 printing
     * @{
     */
    EXTERN void post(const char *fmt, ...);
    EXTERN void startpost(const char *fmt, ...);
    EXTERN void poststring(const char *s);
    EXTERN void postfloat(t_floatarg f);
    EXTERN void postatom(int argc, t_atom *argv);
    EXTERN void endpost(void);
    EXTERN void error(const char *fmt, ...) ATTRIBUTE_FORMAT_PRINTF(1, 2);
    EXTERN void verbose(int level, const char *fmt, ...) ATTRIBUTE_FORMAT_PRINTF(2, 3);
    EXTERN void bug(const char *fmt, ...) ATTRIBUTE_FORMAT_PRINTF(1, 2);
    EXTERN void pd_error(void *object, const char *fmt, ...) ATTRIBUTE_FORMAT_PRINTF(2, 3);
    EXTERN void logpost(const void *object, const int level, const char *fmt, ...)
    ATTRIBUTE_FORMAT_PRINTF(3, 4);
    EXTERN void sys_logerror(const char *object, const char *s);
    EXTERN void sys_unixerror(const char *object);
    EXTERN void sys_ouch(void);
    /**
     *@}
     */
    
    /** \defgroup gM16 system interface routines
     * @{
     */
    EXTERN int sys_isreadablefile(const char *name);
    EXTERN int sys_isabsolutepath(const char *dir);
    EXTERN void sys_bashfilename(const char *from, char *to);
    EXTERN void sys_unbashfilename(const char *from, char *to);
    EXTERN int open_via_path(const char *dir, const char *name, const char *ext,
                             char *dirresult, char **nameresult, unsigned int size, int bin);
    EXTERN int sched_geteventno(void);
    EXTERN double sys_getrealtime(void);
    /** \brief hook to add idle time computation */
    EXTERN int (*sys_idlehook)(void);
    
    /** \brief Win32's open()/fopen() do not handle UTF-8 filenames so we need
     * these internal versions that handle UTF-8 filenames the same across
     * all platforms.  They are recommended for use in external
     * objectclasses as well so they work with Unicode filenames on Windows */
    EXTERN int sys_open(const char *path, int oflag, ...);
    EXTERN int sys_close(int fd);
    EXTERN FILE *sys_fopen(const char *filename, const char *mode);
    EXTERN int sys_fclose(FILE *stream);
    /**
     *@}
     */
    
    /** \defgroup gM17 threading
     * @{
     */
    EXTERN void sys_lock(void);
    EXTERN void sys_unlock(void);
    EXTERN int sys_trylock(void);
    /**
     *@}
     */
    
    /** \defgroup gM18 signals
     * @{
     */
    
    typedef PD_FLOATTYPE t_sample;
    typedef union _sampleint_union {
        t_sample f;
        PD_FLOATUINTTYPE i;
    } t_sampleint_union;
#define MAXLOGSIG 32
#define MAXSIGSIZE (1 << MAXLOGSIG)
    
    typedef struct _signal
    {
        /** \brief number of points in the array */
        int s_n;
        /** \brief the array */
        t_sample *s_vec;
        /** \brief sample rate */
        t_float s_sr;
        /** \brief number of times used */
        int s_refcount;
        /** \brief whether we're going to borrow our array */
        int s_isborrowed;
        /** \brief signal to borrow it from */
        struct _signal *s_borrowedfrom;
        /** \brief next in freelist */
        struct _signal *s_nextfree;
        /** \brief next in used list */
        struct _signal *s_nextused;
        /** \brief allocated size of array in points */
        int s_vecsize;
    } t_signal;
    
    typedef t_int *(*t_perfroutine)(t_int *args);
    
    EXTERN t_int *plus_perform(t_int *args);
    EXTERN t_int *zero_perform(t_int *args);
    EXTERN t_int *copy_perform(t_int *args);
    
    EXTERN void dsp_add_plus(t_sample *in1, t_sample *in2, t_sample *out, int n);
    EXTERN void dsp_add_copy(t_sample *in, t_sample *out, int n);
    EXTERN void dsp_add_scalarcopy(t_float *in, t_sample *out, int n);
    EXTERN void dsp_add_zero(t_sample *out, int n);
    
    EXTERN int sys_getblksize(void);
    EXTERN t_float sys_getsr(void);
    EXTERN int sys_get_inchannels(void);
    EXTERN int sys_get_outchannels(void);
    
    EXTERN void dsp_add(t_perfroutine f, int n, ...);
    EXTERN void dsp_addv(t_perfroutine f, int n, t_int *vec);
    EXTERN void pd_fft(t_float *buf, int npoints, int inverse);
    EXTERN int ilog2(int n);
    
    EXTERN void mayer_fht(t_sample *fz, int n);
    EXTERN void mayer_fft(int n, t_sample *real, t_sample *imag);
    EXTERN void mayer_ifft(int n, t_sample *real, t_sample *imag);
    EXTERN void mayer_realfft(int n, t_sample *real);
    EXTERN void mayer_realifft(int n, t_sample *real);
    
    EXTERN float *cos_table;
#define LOGCOSTABSIZE 9
#define COSTABSIZE (1<<LOGCOSTABSIZE)
    
    EXTERN int canvas_suspend_dsp(void);
    EXTERN void canvas_resume_dsp(int oldstate);
    EXTERN void canvas_update_dsp(void);
    EXTERN int canvas_dspstate;
    /**
     *@}
     */
    
    /** \defgroup gM19 up/downsampling
     * @{
     */
    typedef struct _resample
    {
        /** \brief up/downsampling method ID */
        int method;
        
        /** \brief downsampling factor */
        int downsample;
        /** \brief upsampling factor */
        int upsample;
        
        /** \brief here we hold the resampled data */
        t_sample *s_vec;
        int      s_n;
        
        /** \brief coefficients for filtering... */
        t_sample *coeffs;
        int      coefsize;
        
        /** \brief buffer for filtering */
        t_sample *buffer;
        int      bufsize;
    } t_resample;
    
    EXTERN void resample_init(t_resample *x);
    EXTERN void resample_free(t_resample *x);
    
    EXTERN void resample_dsp(t_resample *x, t_sample *in, int insize, t_sample *out, int outsize, int method);
    EXTERN void resamplefrom_dsp(t_resample *x, t_sample *in, int insize, int outsize, int method);
    EXTERN void resampleto_dsp(t_resample *x, t_sample *out, int insize, int outsize, int method);
    /**
     *@}
     */
    
    /** \defgroup gM20 utility for functions/signals
     * @{
     */
    EXTERN t_float mtof(t_float);
    EXTERN t_float ftom(t_float);
    EXTERN t_float rmstodb(t_float);
    EXTERN t_float powtodb(t_float);
    EXTERN t_float dbtorms(t_float);
    EXTERN t_float dbtopow(t_float);
    
    EXTERN t_float q8_sqrt(t_float);
    EXTERN t_float q8_rsqrt(t_float);
#ifndef N32
    /** \brief old names kept for extern compatibility */
    EXTERN t_float qsqrt(t_float);
    EXTERN t_float qrsqrt(t_float);
#endif
    /**
     *@}
     */
    
    /** \defgroup gM21 data : graphical arrays
     * @{
     */
    
    EXTERN_STRUCT _garray;
#define t_garray struct _garray
    
    EXTERN t_class *garray_class;
    EXTERN int garray_getfloatarray(t_garray *x, int *size, t_float **vec);
    EXTERN int garray_getfloatwords(t_garray *x, int *size, t_word **vec);
    EXTERN void garray_redraw(t_garray *x);
    EXTERN int garray_npoints(t_garray *x);
    EXTERN char *garray_vec(t_garray *x);
    /** \deprecated avoid; use garray_resize_long */
    EXTERN void garray_resize(t_garray *x, t_floatarg f);
    /** \brief better version of garray_resize*/
    EXTERN void garray_resize_long(t_garray *x, long n);
    EXTERN void garray_usedindsp(t_garray *x);
    EXTERN void garray_setsaveit(t_garray *x, int saveit);
    EXTERN t_glist *garray_getglist(t_garray *x);
    EXTERN t_array *garray_getarray(t_garray *x);
    EXTERN t_class *scalar_class;
    
    EXTERN t_float *value_get(t_symbol *s);
    EXTERN void value_release(t_symbol *s);
    EXTERN int value_getfloat(t_symbol *s, t_float *f);
    EXTERN int value_setfloat(t_symbol *s, t_float f);
    /**
     *@}
     */
    
    /** \defgroup gM22 GUI interface - functions to send strings to TK
     * @{
     */
    typedef void (*t_guicallbackfn)(t_gobj *client, t_glist *glist);
    
    EXTERN void sys_vgui(char *fmt, ...);
    EXTERN void sys_gui(char *s);
    EXTERN void sys_pretendguibytes(int n);
    EXTERN void sys_queuegui(void *client, t_glist *glist, t_guicallbackfn f);
    EXTERN void sys_unqueuegui(void *client);
    /** \brief dialog window creation and destruction */
    EXTERN void gfxstub_new(t_pd *owner, void *key, const char *cmd);
    EXTERN void gfxstub_deleteforkey(void *key);
    
    /** \brief object to send "pd" messages */
    extern t_class *glob_pdobject;
    /**
     *@}
     */
    
    /** \defgroup gM23 Max 0.26 compatibility
     * @{
     */
    
    /** \brief the following reflects the new way classes are laid out, with the class
     pointing to the messlist and not vice versa. Externs shouldn't feel it. */
    typedef t_class *t_externclass;
    
    EXTERN void c_extern(t_externclass *cls, t_newmethod newroutine,
                         t_method freeroutine, t_symbol *name, size_t size, int tiny, \
                         t_atomtype arg1, ...);
    EXTERN void c_addmess(t_method fn, t_symbol *sel, t_atomtype arg1, ...);
    
#define t_getbytes getbytes
#define t_freebytes freebytes
#define t_resizebytes resizebytes
#define typedmess pd_typedmess
#define vmess pd_vmess
    
    /** \brief A definition to help gui objects straddle 0.34-0.35 changes.  If this is
     defined, there is a "te_xpix" field in objects, not a "te_xpos" as before: */
    
#define PD_USE_TE_XPIX
    
#ifndef _MSC_VER /** \brief Microoft compiler can't handle "inline" function/macros */
#if defined(__i386__) || defined(__x86_64__) || defined(__arm__)
    /** \brief a test for NANs and denormals.  Should only be necessary on i386. */
#if PD_FLOATSIZE == 32
    
    typedef  union
    {
        t_float f;
        unsigned int ui;
    }t_bigorsmall32;
    
    static inline int PD_BADFLOAT(t_float f)  /** \brief malformed float */
    {
        t_bigorsmall32 pun;
        pun.f = f;
        pun.ui &= 0x7f800000;
        return((pun.ui == 0) | (pun.ui == 0x7f800000));
    }
    
    static inline int PD_BIGORSMALL(t_float f)  /** \brief exponent outside (-64,64) */
    {
        t_bigorsmall32 pun;
        pun.f = f;
        return((pun.ui & 0x20000000) == ((pun.ui >> 1) & 0x20000000));
    }
    
#elif PD_FLOATSIZE == 64
    
    typedef  union
    {
        t_float f;
        unsigned int ui[2];
    }t_bigorsmall64;
    
    static inline int PD_BADFLOAT(t_float f)  /** \brief malformed double */
    {
        t_bigorsmall64 pun;
        pun.f = f;
        pun.ui[1] &= 0x7ff00000;
        return((pun.ui[1] == 0) | (pun.ui[1] == 0x7ff00000));
    }
    
    static inline int PD_BIGORSMALL(t_float f)  /** \brief exponent outside (-512,512) */
    {
        t_bigorsmall64 pun;
        pun.f = f;
        return((pun.ui[1] & 0x20000000) == ((pun.ui[1] >> 1) & 0x20000000));
    }
    
#endif /** \brief PD_FLOATSIZE */
#else /** \brief not INTEL or ARM */
#define PD_BADFLOAT(f) 0
#define PD_BIGORSMALL(f) 0
#endif
    
#else   /** \brief _MSC_VER */
#if PD_FLOATSIZE == 32
#define PD_BADFLOAT(f) ((((*(unsigned int*)&(f))&0x7f800000)==0) || \
(((*(unsigned int*)&(f))&0x7f800000)==0x7f800000))
    /** \brief more stringent test: anything not between 1e-19 and 1e19 in absolute val */
#define PD_BIGORSMALL(f) ((((*(unsigned int*)&(f))&0x60000000)==0) || \
(((*(unsigned int*)&(f))&0x60000000)==0x60000000))
#else   /** \brief 64 bits... don't know what to do here */
#define PD_BADFLOAT(f) (!(((f) >= 0) || ((f) <= 0)))
#define PD_BIGORSMALL(f) ((f) > 1e150 || (f) <  -1e150 \
|| (f) > -1e-150 && (f) < 1e-150 )
#endif
#endif /** \brief _MSC_VER */
    /** \brief get version number at run time */
    EXTERN void sys_getversion(int *major, int *minor, int *bugfix);
    
    EXTERN_STRUCT _pdinstance;
#define t_pdinstance struct _pdinstance       /** \brief m_imp.h */
    
    /** \brief m_pd.c */
    
    EXTERN t_pdinstance *pdinstance_new( void);
    EXTERN void pd_setinstance(t_pdinstance *x);
    EXTERN void pdinstance_free(t_pdinstance *x);
    EXTERN t_canvas *pd_getcanvaslist(void);
    EXTERN int pd_getdspstate(void);
    
    //TEST
    EXTERN void canvas_saveto(t_canvas *x, t_binbuf *b);
#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
}
#endif

#define __m_pd_h_
#endif /** \brief __m_pd_h_ */
