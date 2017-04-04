/*
 * Cream Library
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CreamLibrary
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "../c.library.hpp"

#include <bitset>

static const int MAX_MATRIX_SIZE = 256;
static const int BITSET_SIZE = MAX_MATRIX_SIZE * MAX_MATRIX_SIZE;
typedef std::bitset<BITSET_SIZE> BitMatrix;

static t_symbol* s_matrix_row = gensym("row");
static t_symbol* s_matrix_col = gensym("col");
static t_symbol* s_matrix_cell = gensym("cell");

struct t_matrixctrl {
    t_ebox j_box;

    t_outlet* f_out;

    BitMatrix* f_matrix;
    int cols;
    int rows;
    int f_selected[2];
    t_rgba color_background;
    t_rgba color_border;
    t_rgba color_active;

    size_t numRows() const
    {
        return size_t(rows);
    }

    size_t numCols() const
    {
        return size_t(cols);
    }

    int value(size_t row, size_t col) const
    {
        row %= MAX_MATRIX_SIZE;
        col %= MAX_MATRIX_SIZE;
        return f_matrix->test(row * MAX_MATRIX_SIZE + col) ? 1 : 0;
    }

    void setValue(size_t row, size_t col, bool v)
    {
        row %= MAX_MATRIX_SIZE;
        col %= MAX_MATRIX_SIZE;
        f_matrix->set(row * MAX_MATRIX_SIZE + col, v);
    }

    void flip(size_t row, size_t col)
    {
        row %= MAX_MATRIX_SIZE;
        col %= MAX_MATRIX_SIZE;
        f_matrix->flip(row * MAX_MATRIX_SIZE + col);
    }
};

static t_eclass* matrixctrl_class;

static inline t_ebox* EBOX(t_matrixctrl* x)
{
    return &x->j_box;
}

static void matrixctrl_output(t_matrixctrl* x, size_t row, size_t col)
{
    t_atom av[3];
    if (col < x->numCols() && row < x->numRows()) {
        atom_setfloat(av, row);
        atom_setfloat(av + 1, col);
        atom_setfloat(av + 2, x->value(row, col));

        outlet_anything(x->f_out, s_matrix_cell, 3, av);

        t_pd* send = ebox_getsender(EBOX(x));
        if (send) {
            pd_typedmess(ebox_getsender(EBOX(x)), s_matrix_cell, 3, av);
        }
    }
}

static void matrixctrl_bang(t_matrixctrl* x)
{
    for (size_t row = 0; row < x->numRows(); row++) {
        for (size_t col = 0; col < x->numCols(); col++)
            matrixctrl_output(x, row, col);
    }
}

static void matrixctrl_clear(t_matrixctrl* x)
{
    for (size_t col = 0; col < x->numCols(); col++) {
        for (size_t row = 0; row < x->numRows(); row++) {
            if (x->value(row, col)) {
                x->setValue(row, col, false);
                matrixctrl_output(x, row, col);
            }
        }
    }

    ebox_invalidate_layer(EBOX(x), cream_sym_background_layer);
    ebox_redraw(EBOX(x));
}

static void matrixctrl_getrow(t_matrixctrl* x, float f)
{
    static t_atom buf[MAX_MATRIX_SIZE];

    const size_t row = size_t(f);
    if (row < x->numRows()) {
        for (size_t col = 0; col < x->numCols(); col++) {
            atom_setfloat(buf + col, x->value(row, col));
        }

        outlet_anything(x->f_out, s_matrix_row, x->cols, buf);
    }
}

static void matrixctrl_getcolumn(t_matrixctrl* x, float f)
{
    static t_atom buf[MAX_MATRIX_SIZE];

    const size_t col = size_t(f);
    if (col < x->numCols()) {
        for (size_t row = 0; row < x->numRows(); row++) {
            atom_setfloat(buf + row, x->value(row, col));
        }

        outlet_anything(x->f_out, s_matrix_col, x->rows, buf);
    }
}

static void matrixctrl_get_cell(t_matrixctrl* x, t_symbol* /*s*/, int ac, t_atom* av)
{
    if (ac > 1 && av) {
        t_int row = atom_getint(av);
        t_int col = atom_getint(av + 1);
        matrixctrl_output(x, row, col);
    } else {
        pd_error(x, "[%s] getcell usage: ROW COL", eobj_getclassname(x)->s_name);
    }
}

static void matrixctrl_set_cell(t_matrixctrl* x, t_symbol* s, int ac, t_atom* av)
{
    if (ac > 2 && av) {
        t_int row = atom_getint(av);
        t_int col = atom_getint(av + 1);
        t_int val = atom_getint(av + 2);
        x->setValue(row, col, val);

        ebox_invalidate_layer(EBOX(x), cream_sym_background_layer);
        ebox_redraw(EBOX(x));
    } else {
        pd_error(x, "[%s] setcell usage: ROW COL VALUE", eobj_getclassname(x)->s_name);
    }
}

static void matrixctrl_set_column(t_matrixctrl* x, t_symbol* s, int ac, t_atom* av)
{
    if (ac > 2 && av) {
        t_int col = atom_getint(av);
        for (size_t i = 0; i < x->numRows() && i < (ac - 1); i++) {
            x->setValue(i, col, atom_getfloat(av + i + 1));
        }

        ebox_invalidate_layer(EBOX(x), cream_sym_background_layer);
        ebox_redraw(EBOX(x));
    } else {
        pd_error(x, "[%s] setcolumn usage: COL_INDEX row1 row2 row3 row4 etc.", eobj_getclassname(x)->s_name);
    }
}

static void matrixctrl_set_row(t_matrixctrl* x, t_symbol* s, int ac, t_atom* av)
{
    if (ac > 2 && av) {
        t_int row = atom_getint(av);
        for (size_t i = 0; i < x->numCols() && i < (ac - 1); i++) {
            x->setValue(row, i, atom_getfloat(av + i + 1));
        }

        ebox_invalidate_layer(EBOX(x), cream_sym_background_layer);
        ebox_redraw(EBOX(x));
    } else {
        pd_error(x, "[%s] setrow usage: ROW_INDEX col1 col2 col3 col4 etc.", eobj_getclassname(x)->s_name);
    }
}

static void matrixctrl_set(t_matrixctrl* x, t_symbol* s, int ac, t_atom* av)
{
    if (ac && av) {
        for (int i = 2; i < ac; i += 3) {
            if (atom_gettype(av + i - 2) == A_FLOAT
                && atom_gettype(av + i - 1) == A_FLOAT
                && atom_gettype(av + i) == A_FLOAT) {

                size_t row = size_t(atom_getfloat(av + i - 2));
                size_t column = size_t(atom_getfloat(av + i - 1));
                char value = char(atom_getfloat(av + i));
                if (column < x->numCols() && row < x->numRows()) {
                    x->setValue(row, column, value);
                }
            }
        }

        ebox_invalidate_layer(EBOX(x), cream_sym_background_layer);
        ebox_redraw(EBOX(x));
    }
}

static void matrixctrl_list(t_matrixctrl* x, t_symbol* s, int ac, t_atom* av)
{
    if (ac && av) {
        for (long i = 2; i < ac; i += 3) {
            if (atom_gettype(av + i - 2) == A_FLOAT
                && atom_gettype(av + i - 1) == A_FLOAT
                && atom_gettype(av + i) == A_FLOAT) {

                size_t row = size_t(atom_getfloat(av + i - 2));
                size_t column = size_t(atom_getfloat(av + i - 1));
                char value = char(atom_getfloat(av + i));
                if (column < x->numCols() && row < x->numRows()) {
                    x->setValue(row, column, value);
                    matrixctrl_output(x, row, column);
                }
            }
        }

        ebox_invalidate_layer(EBOX(x), cream_sym_background_layer);
        ebox_redraw(EBOX(x));
    }
}

static void matrixctrl_getdrawparams(t_matrixctrl* x, t_object* /*patcherview*/, t_edrawparams* params)
{
    CREAM_DEFAULT_DRAW_PARAMS();
}

static void matrixctrl_oksize(t_matrixctrl* x, t_rect* newrect)
{
    float ratio;
    newrect->width = pd_clip_min(newrect->width, x->cols * 6);
    newrect->height = pd_clip_min(newrect->height, x->rows * 6);

    ratio = (newrect->width - 1.f) / (float)x->cols;
    if (ratio - (int)ratio != 0) {
        ratio = floorf(ratio);
        newrect->width = ratio * (float)x->cols + 1.f;
    }
    ratio = (newrect->height - 1.f) / (float)x->rows;
    if (ratio - (int)ratio != 0) {
        ratio = floorf(ratio);
        newrect->height = ratio * (float)x->rows + 1.f;
    }

    newrect->width = pd_clip_min(newrect->width, 30.);
    newrect->height = pd_clip_min(newrect->height, 10.);
}

static t_pd_err matrixctrl_notify(t_matrixctrl* x, t_symbol* /*s*/, t_symbol* msg, void* /*sender*/, void* /*data*/)
{
    if (msg == cream_sym_attr_modified) {
        ebox_invalidate_layer(EBOX(x), cream_sym_background_layer);
    }
    return 0;
}

static void matrixctrl_paint(t_matrixctrl* x, t_object* /*view*/)
{
    t_rect rect;
    ebox_get_rect_for_view(EBOX(x), &rect);

    t_elayer* g = ebox_start_layer(EBOX(x), cream_sym_background_layer, rect.width, rect.height);
    if (g) {
        const size_t block_width = rect.width / x->cols;
        const size_t block_height = rect.height / x->rows;
        for (size_t incx = 0, col = 0; col < x->numCols(); col++, incx += block_width) {
            for (size_t incY = 0, row = 0; row < x->numRows(); row++, incY += block_height) {
                egraphics_rectangle_rounded(g, incx + 1.f, incY + 1.f, block_width - 2.f, block_height - 2.f, 1.f);
                if (x->value(row, col)) {
                    egraphics_set_color_rgba(g, &x->color_active);
                    egraphics_fill_preserve(g);
                }
                t_rgba bc = rgba_addContrast(x->color_background, -0.15f);
                egraphics_set_color_rgba(g, &bc);
                egraphics_stroke(g);
            }
        }

        ebox_end_layer(EBOX(x), cream_sym_background_layer);
    }

    ebox_paint_layer(EBOX(x), cream_sym_background_layer, 0, 0);
}

static void matrixctrl_mousedown(t_matrixctrl* x, t_object* /*patcherview*/, t_pt pt, long /*modifiers*/)
{
    t_rect rect;
    ebox_get_rect_for_view(EBOX(x), &rect);
    x->f_selected[0] = (int)pd_clip_minmax(pt.x / (rect.width / (float)x->cols), 0.f, (float)x->cols - 1.f);
    x->f_selected[1] = (int)pd_clip_minmax(pt.y / (rect.height / (float)x->rows), 0.f, (float)x->rows - 1.f);
    if (x->f_selected[0] >= 0 && x->f_selected[0] < x->cols && x->f_selected[1] >= 0 && x->f_selected[1] < x->rows) {
        x->flip(x->f_selected[1], x->f_selected[0]);
        matrixctrl_output(x, x->f_selected[1], x->f_selected[0]);
        ebox_invalidate_layer(EBOX(x), cream_sym_background_layer);
        ebox_redraw(EBOX(x));
    }
}

static void matrixctrl_mousedrag(t_matrixctrl* x, t_object* /*patcherview*/, t_pt pt, long /*modifiers*/)
{
    t_rect rect;
    ebox_get_rect_for_view(EBOX(x), &rect);
    int newpt[2];
    newpt[0] = (int)pd_clip_minmax(pt.x / (rect.width / (float)x->cols), 0.f, (float)x->cols - 1.f);
    newpt[1] = (int)pd_clip_minmax(pt.y / (rect.height / (float)x->rows), 0.f, (float)x->rows - 1.f);
    if (newpt[0] != x->f_selected[0] || newpt[1] != x->f_selected[1]) {
        x->f_selected[0] = newpt[0];
        x->f_selected[1] = newpt[1];
        if (x->f_selected[0] >= 0 && x->f_selected[0] < x->cols && x->f_selected[1] >= 0 && x->f_selected[1] < x->rows) {
            x->flip(x->f_selected[1], x->f_selected[0]);
            matrixctrl_output(x, x->f_selected[1], x->f_selected[0]);
            ebox_invalidate_layer(EBOX(x), cream_sym_background_layer);
            ebox_redraw(EBOX(x));
        }
    }
}

static void matrixctrl_mouseleave(t_matrixctrl* x, t_object* /*patcherview*/, t_pt /*pt*/, long /*modifiers*/)
{
    x->f_selected[0] = -1;
    x->f_selected[1] = -1;
}

static void matrixctrl_preset(t_matrixctrl* x, t_binbuf* b)
{
    t_atom* av = new t_atom[x->cols * x->rows * 3];
    int ac = 0;
    if (av) {
        for (size_t row = 0; row < x->numRows(); row++) {
            for (size_t col = 0; col < x->numRows(); col++) {
                atom_setfloat(av + ac, row);
                atom_setfloat(av + ac + 1, col);
                atom_setfloat(av + ac + 2, x->value(row, col));
                ac += 3;
            }
        }

        binbuf_addv(b, "s", &s_list);
        binbuf_add(b, x->cols * x->rows * 3, av);
        delete[] av;
    }
}

static t_pd_err matrixctrl_matrix_set_rows(t_matrixctrl* x, t_object* /*attr*/, int ac, t_atom* av)
{
    if (ac > 0 && av && atom_gettype(av) == A_FLOAT) {
        x->rows = int(pd_clip_minmax(atom_getfloat(av), 1, 256));
        x->f_matrix->reset();

        ebox_notify(EBOX(x), s_size, 0, 0, 0);
        ebox_invalidate_layer(EBOX(x), cream_sym_background_layer);
        ebox_redraw(EBOX(x));
    }
    return 0;
}

static t_pd_err matrixctrl_matrix_set_cols(t_matrixctrl* x, t_object* /*attr*/, int ac, t_atom* av)
{
    if (ac > 0 && av && atom_gettype(av) == A_FLOAT) {
        x->cols = int(pd_clip_minmax(atom_getfloat(av), 1, 256));
        x->f_matrix->reset();

        ebox_notify(EBOX(x), s_size, 0, 0, 0);
        ebox_invalidate_layer(EBOX(x), cream_sym_background_layer);
        ebox_redraw(EBOX(x));
    }
    return 0;
}

static void* matrixctrl_new(t_symbol* s, int argc, t_atom* argv)
{
    t_matrixctrl* x = (t_matrixctrl*)eobj_new(matrixctrl_class);
    t_binbuf* d = binbuf_via_atoms(argc, argv);

    if (x && d) {
        x->cols = 8;
        x->rows = 4;
        x->f_matrix = new BitMatrix;
        x->f_selected[0] = -1;
        x->f_selected[1] = -1;
        x->f_matrix->reset();

        ebox_new(EBOX(x), 0 | EBOX_GROWINDI);

        x->f_out = outlet_new((t_object*)x, &s_list);

        ebox_attrprocess_viabinbuf(x, d);
        ebox_ready(EBOX(x));

        binbuf_free(d);

        return x;
    }

    return NULL;
}

static void matrixctrl_free(t_matrixctrl* x)
{
    ebox_free(EBOX(x));
    outlet_free(x->f_out);
    delete x->f_matrix;
}

extern "C" void setup_ui0x2ematrix(void)
{
    t_eclass* c = eclass_new("ui.matrix", (method)matrixctrl_new, (method)matrixctrl_free, sizeof(t_matrixctrl), 0L, A_GIMME, 0);

    if (c) {
        // clang-format off
        eclass_guiinit(c, 0);
        eclass_addmethod(c, (method) matrixctrl_paint,           "paint",            A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_notify,          "notify",           A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_getdrawparams,   "getdrawparams",    A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_oksize,          "oksize",           A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_set,             "set",              A_GIMME,0);
        eclass_addmethod(c, (method) matrixctrl_list,            "list",             A_GIMME,0);
        eclass_addmethod(c, (method) matrixctrl_bang,            "bang",             A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_clear,           "clear",            A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_getrow,          "getrow",           A_FLOAT,0);
        eclass_addmethod(c, (method) matrixctrl_set_row,         "setrow",           A_GIMME,0);
        eclass_addmethod(c, (method) matrixctrl_getcolumn,       "getcolumn",        A_FLOAT,0);
        eclass_addmethod(c, (method) matrixctrl_set_column,      "setcolumn",        A_GIMME,0);
        eclass_addmethod(c, (method) matrixctrl_get_cell,        "getcell",          A_GIMME,0);
        eclass_addmethod(c, (method) matrixctrl_set_cell,        "setcell",          A_GIMME,0);
        
        eclass_addmethod(c, (method) matrixctrl_mousedown,       "mousedown",        A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_mousedrag,       "mousedrag",        A_NULL, 0);
        eclass_addmethod(c, (method) matrixctrl_mouseleave,      "mouseleave",       A_NULL, 0);
        
        eclass_addmethod(c, (method) matrixctrl_preset,          "preset",           A_NULL, 0);
        
        CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
        CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
        CLASS_ATTR_DEFAULT              (c, "size", 0, "105 53");

        CLASS_ATTR_INT                  (c, "rows", 0, t_matrixctrl, rows);
        CLASS_ATTR_ACCESSORS            (c, "rows", 0, matrixctrl_matrix_set_rows);
        CLASS_ATTR_FILTER_MIN           (c, "rows", 1);
        CLASS_ATTR_STEP                 (c, "rows", 1);
        CLASS_ATTR_STYLE                (c, "rows", 0, "number");
        CLASS_ATTR_LABEL                (c, "rows", 0, _("Rows"));
        CLASS_ATTR_ORDER                (c, "rows", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "rows", 0, "4");

        CLASS_ATTR_INT                  (c, "cols", 0, t_matrixctrl, cols);
        CLASS_ATTR_ACCESSORS            (c, "cols", 0, matrixctrl_matrix_set_cols);
        CLASS_ATTR_FILTER_MIN           (c, "cols", 1);
        CLASS_ATTR_STEP                 (c, "cols", 1);
        CLASS_ATTR_STYLE                (c, "cols", 0, "number");
        CLASS_ATTR_LABEL                (c, "cols", 0, _("Columns"));
        CLASS_ATTR_ORDER                (c, "cols", 0, "1");
        CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "cols", 0, "8");
        
        ATTR_DEFAULT_COLOR_BORDER       (c, t_matrixctrl);
        ATTR_DEFAULT_COLOR_BACKGROUND   (c, t_matrixctrl);
        ATTR_DEFAULT_COLOR_ACTIVE       (c, t_matrixctrl);

        // clang-format on
        eclass_register(CLASS_BOX, c);
        matrixctrl_class = c;
    }
}
