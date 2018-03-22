#ifndef UI_MATRIX_H
#define UI_MATRIX_H

#include "ceammc_ui_object.h"

#include <bitset>

using namespace ceammc;

static const size_t UI_MAX_MATRIX_SIZE = 256;
static const size_t UI_BITSET_SIZE = UI_MAX_MATRIX_SIZE * UI_MAX_MATRIX_SIZE;
typedef std::bitset<UI_BITSET_SIZE> BitMatrix;

class UIMatrix : public UIObject {
    BitMatrix matrix_;

    t_rgba prop_color_active_;
    t_rgba prop_color_current_;
    int prop_rows_;
    int prop_cols_;
    int mouse_current_col_;
    int mouse_current_row_;
    int current_col_;
    int current_row_;

public:
    UIMatrix();

    bool cell(size_t row, size_t column) const;
    void setCell(const AtomList& lst);
    AtomList column(size_t column) const;
    void setColumn(size_t column, const AtomList& lst);
    void setColumn(const AtomList& lst);
    AtomList row(size_t idx) const;
    void setRow(size_t row, const AtomList& lst);
    void setRow(const AtomList& lst);
    void flipCell(size_t row, size_t column);
    void flipColumn(size_t col);
    void flipRow(size_t col);
    void flipAll();
    AtomList asList() const;

    void okSize(t_rect* newrect);
    void paint(t_object* view);
    void outputCell(size_t row, size_t column);
    void outputCell(const AtomList& args);
    void outputCol(size_t column);
    void outputCol(const AtomList& args);
    void outputRow(size_t idx);
    void outputRow(const AtomList& args);
    void outputAllCols();
    void outputAllRows();
    void outputAllCells();

    void onBang();
    void onList(const AtomList& lst);
    void onMouseDown(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object*, const t_pt&, long);

    float p_rows() const;
    float p_cols() const;
    void p_setRows(float n);
    void p_setCols(float n);

    void m_flip(const AtomList& lst);
    void m_reset();
    void m_get(const AtomList& lst);
    void m_set(const AtomList& lst);

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

public:
    static void setup();
};

void setup_ui_matrix();

#endif // UI_MATRIX_H
