#ifndef UI_MATRIX_H
#define UI_MATRIX_H

#include "ceammc_ui_object.h"

#include <bitset>
#include <memory>
#include <vector>

using namespace ceammc;

static const size_t UI_MAX_MATRIX_SIZE = 64;
static const size_t UI_BITSET_SIZE = UI_MAX_MATRIX_SIZE * UI_MAX_MATRIX_SIZE;
typedef std::bitset<UI_BITSET_SIZE> BitMatrix;

class UIMatrix : public UIObject {
    BitMatrix matrix_;
    typedef char CellIdxT;
    typedef std::vector<std::pair<CellIdxT, CellIdxT>> CellList;
    CellList cell_update_list_;
    bool update_all_cells_;
    bool reset_all_cells_;

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

    void init(t_symbol* name, const AtomList& args, bool usePresets);

    bool cell(size_t row, size_t column) const;
    void setCell(const AtomList& lst);
    AtomList column(size_t column) const;
    void setColumn(size_t column, const AtomList& lst);
    void setColumn(const AtomList& lst);
    AtomList row(size_t idx) const;
    void setRow(size_t row, const AtomList& lst);
    void setRow(const AtomList& lst);
    void setList(const AtomList& lst);
    void flipCell(size_t row, size_t column);
    void flipColumn(size_t col);
    void flipRow(size_t col);
    void flipAll();
    AtomList asList() const;

    void okSize(t_rect* newrect);
    void paint();
    void create();
    void erase();
    void outputCell(size_t row, size_t column);
    void outputCell(const AtomListView& args);
    void outputCol(size_t column);
    void outputCol(const AtomListView& args);
    void outputRow(size_t row);
    void outputRow(const AtomListView& args);
    void outputAllCols();
    void outputAllRows();
    void outputAllCells();

    void onBang();
    void onList(const AtomList& lst);
    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object*, const t_pt&, long);
    void read(const std::string& fname);
    void write(const std::string& fname);

    t_float p_rows() const;
    t_float p_cols() const;
    void p_setRows(t_float n);
    void p_setCols(t_float n);

    void m_flip(const AtomListView& lst);
    void m_reset();
    void m_random();
    void m_get(const AtomListView& lst);
    void m_set(const AtomListView& lst);

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

    void onPropChange(t_symbol* prop_name);
    void onZoom(t_float z);

public:
    static void setup();

private:
    void addToUpdateList(int row, int col);
    void setCell(int row, int col, bool v);
    int cellWidth() const;
    int cellHeight() const;
    void updateCellsCoords();
    void drawActiveCells();
    void createCells();
    void eraseCells();
    std::pair<int, int> cellAt(const t_pt& pt);
};

void setup_ui_matrix();

#endif // UI_MATRIX_H
