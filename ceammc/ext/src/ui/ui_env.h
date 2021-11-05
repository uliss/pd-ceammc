#ifndef UI_ENV_H
#define UI_ENV_H

#include "../env/datatype_env.h"
#include "ceammc_ui_object.h"

#include <vector>

using namespace ceammc;

enum class SelectType {
    NONE = 0,
    POINT,
    LINE
};

namespace ui {

struct Node {
    float x, y;
    float curve;
    float sigmoid_skew;
    CurveType type;
    SelectType select;
    bool is_stop;
    bool fixed_x, fixed_y;

    Node()
        : x(0)
        , y(0)
        , curve(0)
        , sigmoid_skew(0)
        , type(CURVE_LINE)
        , select(SelectType::NONE)
        , is_stop(false)
        , fixed_x(false)
        , fixed_y(false)
    {
    }

    static Node fromEnvelope(const EnvelopePoint& pt, size_t total_us, float w, float h, bool fixed_x);
};

using NodeList = std::vector<Node>;

}

class UIEnv : public UIObject {
    DataTypeEnv env_;
    ui::NodeList nodes_;
    t_pt cursor_pos_;
    bool draw_cursor_pos_;
    bool draw_cursor_cross_;
    bool delete_mode_;
    UILayer envelope_layer_;
    UILayer cursor_layer_; // cursor cross and text position
    UIFont font_;
    UITextLayout cursor_txt_pos_;
    float max_env_value_;
    UIFont txt_font;
    UITextLayout txt_value0;
    UITextLayout txt_value1;
    UITextLayout txt_value2;
    t_symbol* output_mode_;

public:
    UIEnv();

    void onBang();
    void onData(const Atom& env);

    void paint();
    void okSize(t_rect* newrect);
    void onMouseMove(t_object*, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object*, const t_pt& pt, long mod);
    void onMouseDown(t_object*, const t_pt& pt, const t_pt& abs_pt, long mod);
    void onMouseLeave(t_object*, const t_pt&, long);
    void onMouseUp(t_object* view, const t_pt& pt, long mod);
    void onMouseWheel(const t_pt& pt, long mod, float delta);
    void showPopup(const t_pt& pt, const t_pt& abs_pt);

    void updateNodes();
    void updateEnvelope();

    int findNearestNode(float x, float y) const;
    bool selectNode(size_t idx);

    void m_adsr(const AtomListView& lv);
    void m_asr(const AtomListView& lv);
    void m_ar(const AtomListView& lv);
    void m_eadsr(const AtomListView& lv);
    void m_easr(const AtomListView& lv);
    void m_ear(const AtomListView& lv);
    void m_at(const AtomListView& lv);

    void setNamedEnvelope(t_symbol* env, const AtomListView& lv);

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

    void onPropChange(t_symbol* prop_name);

private:
    void redrawAll();

    void drawBackground(const t_rect& r);
    void drawCursor(const t_rect& r);
    void drawEnvelope(const t_rect& r);

    void addNode(const t_pt& pt);
    long findSelectedNodeIdx() const;

    void removeSelectedNode();

    long findNodeLine(const t_pt& pt);
    void deselectAll();
    bool hasSelectedEdge() const;

    void outputEnvelope();
    static bool isCmdPressed(long mod);

public:
    static void setup();

public:
    t_rgba prop_active_color;
    t_rgba prop_line_color;
    t_float prop_length;
};

void setup_ui_env();

#endif // UI_ENV_H
