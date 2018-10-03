#include "ui_matrix.h"
#include "ceammc_convert.h"
#include "ceammc_format.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"

#include <algorithm>
#include <cassert>

static t_symbol* SYM_CELL = gensym("cell");
static t_symbol* SYM_ROW = gensym("row");
static t_symbol* SYM_COL = gensym("col");
static t_symbol* SYM_LIST = gensym("list");
static t_symbol* SYM_OUTPUT_ALL_COLS = gensym("cols");
static t_symbol* SYM_OUTPUT_ALL_ROWS = gensym("rows");
static t_symbol* SYM_OUTPUT_ALL_CELLS = gensym("cells");

UIMatrix::UIMatrix()
    : matrix_()
    , prop_color_active_(rgba_black)
    , prop_color_current_(rgba_black)
    , prop_rows_(4)
    , prop_cols_(8)
    , mouse_current_col_(-1)
    , mouse_current_row_(-1)
    , current_col_(-1)
    , current_row_(-1)
{
    createOutlet();
}

void UIMatrix::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    int row = args.intAt(0, -1);
    int col = args.intAt(1, -1);

    if (row > 0 && col > 0) {
        prop_cols_ = clip<int>(col, 1, UI_MAX_MATRIX_SIZE);
        prop_rows_ = clip<int>(row, 1, UI_MAX_MATRIX_SIZE);
    }
}

bool UIMatrix::cell(size_t row, size_t col) const
{
    row %= UI_MAX_MATRIX_SIZE;
    col %= UI_MAX_MATRIX_SIZE;
    return matrix_[row * UI_MAX_MATRIX_SIZE + col];
}

void UIMatrix::setCell(const AtomList& lst)
{
    if (lst.size() != 3) {
        UI_ERR << "usage set cell ROW COL VALUE";
        return;
    }

    int row = lst[0].asInt(-1);
    int col = lst[1].asInt(-1);
    int v = lst[2].asInt(0);

    if (row < 0 || row >= prop_rows_ || col < 0 || col >= prop_cols_) {
        UI_ERR << "invalid indexes: " << lst;
        return;
    }

    matrix_.set(row * UI_MAX_MATRIX_SIZE + col, v);
}

AtomList UIMatrix::column(size_t col) const
{
    AtomList res;
    res.reserve(prop_rows_);

    for (int i = 0; i < prop_rows_; i++)
        res.append(Atom(cell(i, col) ? 1 : 0));

    return res;
}

void UIMatrix::setColumn(size_t col, const AtomList& lst)
{
    size_t t = std::min<size_t>(lst.size(), prop_rows_);
    for (size_t row = 0; row < t; row++)
        matrix_.set(row * UI_MAX_MATRIX_SIZE + col, lst[row].asFloat() != 0);
}

void UIMatrix::setColumn(const AtomList& lst)
{
    if (lst.size() < 2) {
        UI_ERR << "column index and values are expected: " << lst;
        return;
    }

    int idx = lst[0].asInt(-1);

    if (idx < 0 || idx >= prop_cols_) {
        UI_ERR << "invalid column index: " << idx;
        return;
    }

    setColumn(idx, lst.slice(1));
}

AtomList UIMatrix::row(size_t idx) const
{
    AtomList res;
    res.fill(Atom(0.f), prop_cols_);

    for (int i = 0; i < prop_cols_; i++) {
        if (cell(idx, i))
            res[i] = Atom(1);
    }

    return res;
}

void UIMatrix::setRow(size_t row, const AtomList& lst)
{
    size_t t = std::min<size_t>(lst.size(), prop_cols_);

    for (size_t col = 0; col < t; col++)
        matrix_.set(row * UI_MAX_MATRIX_SIZE + col, lst[col].asFloat() != 0);
}

void UIMatrix::setRow(const AtomList& lst)
{
    if (lst.size() < 2) {
        UI_ERR << "row index and values are expected: " << lst;
        return;
    }

    int idx = lst[0].asInt(-1);

    if (idx < 0 || idx >= prop_rows_) {
        UI_ERR << "invalid row index: " << idx;
        return;
    }

    setRow(idx, lst.slice(1));
}

void UIMatrix::setList(const AtomList& lst)
{
    if (lst.size() != prop_cols_ * prop_rows_)
        UI_DBG << "warning: list size is not equal number of matrix cells";

    const size_t total = std::min<size_t>(lst.size(), prop_cols_ * prop_rows_);
    for (size_t i = 0; i < total; i++) {
        size_t col = i % prop_cols_;
        size_t row = i / prop_cols_;

        matrix_.set(row * UI_MAX_MATRIX_SIZE + col, lst[i].asFloat() != 0);
    }

    redrawBGLayer();
}

void UIMatrix::flipCell(size_t row, size_t col)
{
    matrix_.flip(row * UI_MAX_MATRIX_SIZE + col);
}

void UIMatrix::flipColumn(size_t col)
{
    for (int row = 0; row < prop_rows_; row++)
        matrix_.flip(row * UI_MAX_MATRIX_SIZE + col);
}

void UIMatrix::flipRow(size_t row)
{
    for (int col = 0; col < prop_cols_; col++)
        matrix_.flip(row * UI_MAX_MATRIX_SIZE + col);
}

void UIMatrix::flipAll()
{
    matrix_.flip();
}

AtomList UIMatrix::asList() const
{
    AtomList res;
    res.fill(Atom(0.f), prop_cols_ * prop_rows_);

    for (size_t r = 0; r < prop_rows_; r++) {
        for (size_t c = 0; c < prop_cols_; c++) {
            if (cell(r, c))
                res[r * prop_cols_ + c] = Atom(1);
        }
    }

    return res;
}

void UIMatrix::okSize(t_rect* newrect)
{
    if (!isPatchLoading()) {
        newrect->width = pd_clip_min(newrect->width, prop_cols_ * 6);
        newrect->height = pd_clip_min(newrect->height, prop_rows_ * 6);

        float ratio = (newrect->width - 1.f) / prop_cols_;
        if (ratio - (int)ratio != 0) {
            ratio = floorf(ratio);
            newrect->width = ratio * prop_cols_ + 1.f;
        }

        ratio = (newrect->height - 1.f) / prop_rows_;
        if (ratio - (int)ratio != 0) {
            ratio = floorf(ratio);
            newrect->height = ratio * prop_rows_ + 1.f;
        }

        newrect->width = pd_clip_min(newrect->width, 30.);
        newrect->height = pd_clip_min(newrect->height, 10.);
    } else {
        newrect->width = pd_clip_min(newrect->width, 30.);
        newrect->height = pd_clip_min(newrect->height, 10.);
    }
}

void UIMatrix::paint(t_object* view)
{
    assert(prop_cols_ > 0);
    assert(prop_rows_ > 0);

    const t_rect& r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (p) {
        const int cell_w = r.width / prop_cols_;
        const int cell_h = r.height / prop_rows_;

        const t_rgba bc = rgba_addContrast(prop_color_background, -0.15f);
        const t_rgba cc = rgba_addContrast(prop_color_background, -0.45f);

        if (current_col_ >= 0 && current_col_ < prop_cols_) {
            p.setColor(cc);
            p.setLineWidth(3);
            p.drawRect(current_col_ * cell_w, 0, cell_w, r.height);
            p.stroke();
        }

        if (current_row_ >= 0 && current_row_ < prop_rows_) {
            p.setColor(cc);
            p.setLineWidth(1);
            p.drawRect(0, current_row_ * cell_h, r.width, cell_h);
            p.stroke();
        }

        p.setLineWidth(1);

        for (int inc_x = 0, col = 0; col < prop_cols_; col++, inc_x += cell_w) {
            for (int inc_y = 0, row = 0; row < prop_rows_; row++, inc_y += cell_h) {
                p.drawRect(inc_x + 1, inc_y + 1, cell_w - 2, cell_h - 2);

                if (cell(row, col)) {
                    p.setColor(prop_color_active_);
                    p.fillPreserve();
                }

                p.setColor(bc);
                p.stroke();
            }
        }
    }
}

void UIMatrix::outputCell(size_t row, size_t col)
{
    AtomList args;
    args.reserve(3);
    args.append(row);
    args.append(col);
    args.append(cell(row, col) ? 1 : 0);

    anyTo(0, SYM_CELL, args);
    send(SYM_CELL, args);
}

void UIMatrix::outputCell(const AtomList& args)
{
    if (args.size() != 2) {
        UI_ERR << "usage: get cell ROW COL";
        return;
    }

    int row = args[0].asInt(-1);
    int col = args[1].asInt(-1);

    if (row < 0 || row >= prop_rows_ || col < 0 || col >= prop_cols_) {
        UI_ERR << "indexes are out of range: " << args;
        return;
    }

    outputCell(row, col);
}

void UIMatrix::outputCol(size_t col)
{
    AtomList lst;
    lst.reserve(prop_rows_ + 1);
    lst.append(Atom(col));
    lst.append(column(col));
    anyTo(0, SYM_COL, lst);
    send(SYM_COL, lst);
}

void UIMatrix::outputCol(const AtomList& args)
{
    if (args.empty()) {
        UI_ERR << "column index expected";
        return;
    }

    int idx = args[0].asInt(-1);

    if (idx < 0 || idx >= prop_cols_) {
        UI_ERR << "invalid column index: " << idx;
        return;
    }

    outputCol(size_t(idx));
}

void UIMatrix::outputRow(size_t idx)
{
    AtomList lst;
    lst.reserve(prop_cols_ + 1);
    lst.append(Atom(idx));
    lst.append(row(idx));
    anyTo(0, SYM_ROW, lst);
    send(SYM_ROW, lst);
}

void UIMatrix::outputRow(const AtomList& args)
{
    if (args.empty()) {
        UI_ERR << "row index expected";
        return;
    }

    int idx = args[0].asInt(-1);

    if (idx < 0 || idx >= prop_rows_) {
        UI_ERR << "invalid row index: " << idx;
        return;
    }

    outputRow(size_t(idx));
}

void UIMatrix::outputAllCols()
{
    for (size_t col = 0; col < prop_cols_; col++)
        outputCol(col);
}

void UIMatrix::outputAllRows()
{
    for (size_t i = 0; i < prop_rows_; i++)
        outputRow(i);
}

void UIMatrix::outputAllCells()
{
    for (size_t row = 0; row < prop_rows_; row++) {
        for (size_t col = 0; col < prop_cols_; col++)
            outputCell(row, col);
    }
}

void UIMatrix::onBang()
{
    outputAllCells();
}

void UIMatrix::onList(const AtomList& lst)
{
    setList(lst);
    outputAllCells();
}

void UIMatrix::onMouseDown(t_object* view, const t_pt& pt, long modifiers)
{
    t_rect r = rect();
    mouse_current_col_ = clip<int>(pt.x / (r.width / prop_cols_), 0, prop_cols_ - 1);
    mouse_current_row_ = clip<int>(pt.y / (r.height / prop_rows_), 0, prop_rows_ - 1);

    if (mouse_current_col_ >= 0 && mouse_current_row_ >= 0) {
        flipCell(mouse_current_row_, mouse_current_col_);
        outputCell(mouse_current_row_, mouse_current_col_);
        redrawBGLayer();
    }
}

void UIMatrix::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    t_rect r = rect();
    int cell_w = r.width / prop_cols_;
    int cell_h = r.height / prop_rows_;
    int col = (int)pd_clip_minmax(pt.x / float(cell_w), 0, prop_cols_ - 1);
    int row = (int)pd_clip_minmax(pt.y / float(cell_h), 0, prop_rows_ - 1);

    if (abs(int(pt.x) - col * cell_w) < 3)
        return;

    if (abs(int(pt.y) - col * cell_h) < 3)
        return;

    if (col != mouse_current_col_ || row != mouse_current_row_) {
        mouse_current_col_ = col;
        mouse_current_row_ = row;

        if (row >= 0 && col >= 0) {
            flipCell(row, col);
            outputCell(row, col);
            redrawBGLayer();
        }
    }
}

void UIMatrix::onMouseLeave(t_object*, const t_pt&, long)
{
    mouse_current_col_ = -1;
    mouse_current_row_ = -1;
}

void UIMatrix::m_flip(const AtomList& lst)
{
    if (lst.empty()) {
        flipAll();
    } else if (lst.size() == 2) {
        if (lst[0].isSymbol()) {
            t_symbol* s = lst[0].asSymbol();

            if (s == SYM_COL) {
                int col = lst[1].asInt(-1);

                if (col < 0 || col >= prop_cols_) {
                    UI_ERR << "invalid column index: " << lst[1];
                    return;
                }

                flipColumn(col);

            } else if (s == SYM_ROW) {
                int row = lst[1].asInt(-1);

                if (row < 0 || row >= prop_rows_) {
                    UI_ERR << "invalid row index: " << lst[1];
                    return;
                }

                flipRow(row);
            } else {
                UI_ERR << "unknown flip target: " << s->s_name;
                return;
            }
        }

        if (lst[0].isFloat()) {
            int row = lst[0].asInt(-1);
            int col = lst[1].asInt(-1);

            if (row < 0 || row >= prop_rows_ || col < 0 || col >= prop_cols_) {
                UI_ERR << "invalid indexes: " << lst;
                return;
            }

            flipCell(row, col);
        }
    } else {
        UI_ERR << "usage: flip [col IDX] | [row IDX] | [row col]";
        return;
    }

    redrawBGLayer();
}

void UIMatrix::m_reset()
{
    matrix_.reset();
    redrawBGLayer();
}

void UIMatrix::m_get(const AtomList& lst)
{
    if (lst.empty() || !lst[0].isSymbol()) {
        UI_ERR << "missing arguments: ";
        UI_ERR << "    usage: get col|row|cell|list|cols|rows|cells [ARGS]";
        return;
    }

    t_symbol* sel = lst[0].asSymbol();
    const AtomList args = lst.slice(1);

    if (sel == SYM_CELL) {
        outputCell(args);
    } else if (sel == SYM_COL) {
        outputCol(args);
    } else if (sel == SYM_ROW) {
        outputRow(args);
    } else if (sel == SYM_LIST) {
        listTo(0, asList());
    } else if (sel == SYM_OUTPUT_ALL_COLS) {
        outputAllCols();
    } else if (sel == SYM_OUTPUT_ALL_ROWS) {
        outputAllRows();
    } else if (sel == SYM_OUTPUT_ALL_CELLS) {
        outputAllCells();
    } else {
        UI_ERR << "unknown method: " << sel->s_name;
        UI_ERR << "    usage: get col|row|cell|list|cols|rows|cells [ARGS]";
        return;
    }
}

void UIMatrix::m_set(const AtomList& lst)
{
    if (lst.empty() || !lst[0].isSymbol()) {
        UI_ERR << "missing arguments: ";
        UI_ERR << "    usage: set col|row|cell|list [ARGS]";
        return;
    }

    t_symbol* sel = lst[0].asSymbol();
    const AtomList args = lst.slice(1);

    if (sel == SYM_CELL) {
        setCell(args);
    } else if (sel == SYM_COL) {
        setColumn(args);
    } else if (sel == SYM_ROW) {
        setRow(args);
    } else if (sel == SYM_LIST) {
        setList(args);
    } else {
        UI_ERR << "unknown method: " << sel->s_name;
        UI_ERR << "    usage: set col|row|cell|list [ARGS]";
        return;
    }

    redrawBGLayer();
}

void UIMatrix::loadPreset(size_t idx)
{
    AtomList lst = PresetStorage::instance().listValueAt(presetId(), idx);
    onList(lst);
}

void UIMatrix::storePreset(size_t idx)
{
    PresetStorage::instance().setListValueAt(presetId(), idx, asList());
}

float UIMatrix::p_rows() const
{
    return prop_rows_;
}

float UIMatrix::p_cols() const
{
    return prop_cols_;
}

void UIMatrix::p_setRows(float n)
{
    int num = clip<int>(n, 1, UI_MAX_MATRIX_SIZE);
    if (num != prop_rows_) {
        prop_rows_ = num;
        matrix_.reset();
        updateSize();
    }
}

void UIMatrix::p_setCols(float n)
{
    int num = clip<int>(n, 1, UI_MAX_MATRIX_SIZE);
    if (num != prop_cols_) {
        prop_cols_ = num;
        matrix_.reset();
        updateSize();
    }
}

void UIMatrix::setup()
{
    UIObjectFactory<UIMatrix> obj("ui.matrix");
    obj.setDefaultSize(105, 53);

    obj.usePresets();
    obj.useList();
    obj.useBang();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG | UI_MOUSE_LEAVE);

    obj.addProperty("rows", _("Rows"), 4, &UIMatrix::prop_rows_);
    obj.setPropertyRange("rows", 1, UI_MAX_MATRIX_SIZE);
    obj.setPropertyAccessor("rows", &UIMatrix::p_rows, &UIMatrix::p_setRows);
    obj.addProperty("cols", _("Columns"), 8, &UIMatrix::prop_cols_);
    obj.setPropertyRange("cols", 1, UI_MAX_MATRIX_SIZE);
    obj.setPropertyAccessor("cols", &UIMatrix::p_cols, &UIMatrix::p_setCols);
    obj.addProperty("active_color", _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIMatrix::prop_color_active_);
    obj.addProperty("current_color", _("Current Color"), "1. 0.75 0. 1.", &UIMatrix::prop_color_current_);

    obj.addProperty("current_row", "", -1, &UIMatrix::current_row_);
    obj.hideProperty("current_row");
    obj.setPropertySave("current_row", false);
    obj.addProperty("current_col", "", -1, &UIMatrix::current_col_);
    obj.hideProperty("current_col");
    obj.setPropertySave("current_col", false);

    obj.addMethod("flip", &UIMatrix::m_flip);
    obj.addMethod("reset", &UIMatrix::m_reset);
    obj.addMethod("get", &UIMatrix::m_get);
    obj.addMethod("set", &UIMatrix::m_set);
}

void setup_ui_matrix()
{
    UIMatrix::setup();
}
