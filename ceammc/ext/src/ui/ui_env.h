#ifndef UI_ENV_H
#define UI_ENV_H

#include "datatype_env.h"
#include "ceammc_crc32.h"
#include "ceammc_ui_object.h"

#include <vector>

CEAMMC_DEFINE_STR(adsr);
CEAMMC_DEFINE_STR(ar);
CEAMMC_DEFINE_STR(asr);
CEAMMC_DEFINE_STR(eadsr);
CEAMMC_DEFINE_STR(ear);
CEAMMC_DEFINE_STR(easr);
CEAMMC_DEFINE_STR(exp);
CEAMMC_DEFINE_STR(line);
CEAMMC_DEFINE_STR(sigmoid);
CEAMMC_DEFINE_STR(sin2);
CEAMMC_DEFINE_STR(step);

#define DECLARE_METHOD(name) \
    void m_##name(const AtomListView& lv) { setNamedEnvelope(str_##name, lv); }

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

    static Node fromEnvelope(const EnvelopePoint& pt, size_t total_us, float w, float h);
};

class NodeList : public std::vector<Node> {
public:
    bool isLastNode(const Node& n) const { return &n == &back(); }
    bool isLastIdx(long idx) const { return idx >= 0 && (idx + 1) == static_cast<long>(size()); }
};

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
    float env_last_time_;
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

    DECLARE_METHOD(adsr)
    DECLARE_METHOD(ar)
    DECLARE_METHOD(asr)
    DECLARE_METHOD(eadsr)
    DECLARE_METHOD(ear)
    DECLARE_METHOD(easr)
    DECLARE_METHOD(exp)
    DECLARE_METHOD(line)
    DECLARE_METHOD(sigmoid)
    DECLARE_METHOD(sin2)
    DECLARE_METHOD(step)

    void m_at(const AtomListView& lv);

    void setNamedEnvelope(const char* env, const AtomListView& lv);

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
    int prop_normalize;
};

void setup_ui_env();

#endif // UI_ENV_H
