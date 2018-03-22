#ifndef UI_ENV_H
#define UI_ENV_H

#include "../env/datatype_env.h"
#include "ceammc_ui_object.h"

#include <vector>

using namespace ceammc;

struct Node {
    float x, y;
    float curve;
    float sigmoid_skew;
    CurveType type;
    bool is_selected, is_stop;
    bool fixed_x, fixed_y;

    Node()
        : x(0)
        , y(0)
        , curve(0)
        , sigmoid_skew(0)
        , type(CURVE_LINE)
        , is_selected(false)
        , is_stop(false)
        , fixed_x(false)
        , fixed_y(false)
    {
    }

    static Node fromEnvelope(const EnvelopePoint& pt, size_t total_us, float w, float h, bool fixed_x);
};

typedef std::vector<Node> NodeList;

class UIEnv : public UIObject {
    DataTypeEnv env_;
    NodeList nodes_;
    t_pt cursor_pos_;
    bool draw_cursor_pos_;
    bool draw_cursor_cross_;
    bool delete_mode_;
    UILayer cursor_layer_;
    UIFont font_;
    UITextLayout cursor_txt_pos_;
    float max_env_value_;
    UIFont txt_font;
    UITextLayout txt_value0;
    UITextLayout txt_value1;
    UITextLayout txt_value2;

public:
    UIEnv();

    void onBang();
    void onData(const DataPtr& ptr);

    void paint(t_object*);
    void okSize(t_rect* newrect);
    void onMouseMove(t_object*, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object*, const t_pt& pt, long);
    void onMouseDown(t_object*, const t_pt& pt, long mod);
    void onMouseLeave(t_object*, const t_pt&, long);
    void onMouseWheel(t_object*, const t_pt& pt, long, double delta);
    void onMouseUp(t_object*, const t_pt&, long);
    void onDblClick(t_object*, const t_pt& pt, long modifiers);

    void updateNodes();
    void updateEnvelope();

    int findNearestNode(float x, float y) const;
    bool selectNode(size_t idx);

    void m_adsr(const AtomList& lst);
    void m_asr(const AtomList& lst);
    void m_ar(const AtomList& lst);
    void m_eadsr(const AtomList& lst);
    void m_easr(const AtomList& lst);
    void m_ear(const AtomList& lst);

    void setNamedEnvelope(t_symbol* env, const AtomList& args);

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

    t_pd_err notify(t_symbol* attr_name, t_symbol* msg);

private:
    void redrawCursorLayer();
    void redrawAll();

public:
    static void setup();

public:
    t_rgba prop_active_color;
    t_rgba prop_line_color;
    t_float prop_length;
};

void setup_ui_env();

#endif // UI_ENV_H
