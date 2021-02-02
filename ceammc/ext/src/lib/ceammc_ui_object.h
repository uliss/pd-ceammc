#ifndef CEAMMC_UI_OBJECT_H
#define CEAMMC_UI_OBJECT_H

#include "ceammc_atomlist.h"
#include "ceammc_cicm.h"
#include "ceammc_property_info.h"

#include <initializer_list>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ceammc {

class UIObjectImpl;

class UIError : public std::ostringstream {
    const UIObjectImpl* obj_;

public:
    UIError(const UIObjectImpl* obj = nullptr);
    ~UIError();
    UIError& stream() { return *this; }
};

class UIDebug : public std::ostringstream {
    const UIObjectImpl* obj_;

public:
    UIDebug(const UIObjectImpl* obj = nullptr);
    ~UIDebug();
    UIDebug& stream() { return *this; }
};

#define UI_ERR UIError(this).stream()
#define UI_DBG UIDebug(this).stream()

class UIObjectImpl {
    t_ebox* const box_;
    AtomListView args_;
    std::unordered_set<t_symbol*> binded_signals_;
    std::vector<UILayer*> layer_stack_;
    t_symbol* name_;
    t_symbol* old_preset_id_;
    bool use_presets_;

protected:
    UILayer bg_layer_;
    std::vector<PopupMenuCallbacks> popup_menu_list_;
    std::vector<t_outlet*> outlets_;

    void appendToLayerList(UILayer* l);
    void prependToLayerList(UILayer* l);
    void invalidateLayer(UILayer* l);
    void initPopupMenu(const std::string& n, std::initializer_list<PopupMenuCallbacks::Entry> args);
    void showDefaultPopupMenu(const t_pt& pt, const t_pt& abs_pt);
    void showPopupMenu(const std::string& n, const t_pt& pt, const t_pt& abs_pt);

public:
    t_rgba prop_color_background;
    t_rgba prop_color_border;
    t_rgba prop_color_label;
    int prop_mouse_events;

public:
    UIObjectImpl(t_ebox* x);
    ~UIObjectImpl();

    // CICM and Pd
    t_ebox* asEBox() const { return box_; }
    t_eobj* asEObj() const;
    t_object* asPdObject() const;
    t_gobj* asGObj() const;
    t_pd* asPd() const;
    t_canvas* canvas() const;
    bool isPatchLoading() const;
    bool isPatchEdited() const;
    bool isVisible() const;
    const AtomListView& args() const { return args_; }
    AtomListView& args() { return args_; }

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    t_symbol* name() const;
    t_symbol* presetId();

    void paint();
    void create();
    void erase();

    void redraw();
    void redrawBGLayer();
    void redrawLayer(UILayer& l);

    void updateSize();
    void resize(int w, int h);

    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);
    void onMouseEnter(t_object* view, const t_pt& pt, long modifiers);
    void onMouseWheel(const t_pt& pt, long modifiers, double delta);
    void onDblClick(t_object* view, const t_pt& pt, long modifiers);
    void onPopup(t_symbol* menu_name, long item_idx, const t_pt& pt);
    void showPopup(const t_pt& pt, const t_pt& abs_pt);
    bool outputMouseEvents() const;

    void okSize(t_rect* newrect);
    void setDrawParams(t_edrawparams* params);
    void onZoom(t_float z);
    void onPropChange(t_symbol* name);
    void write(const std::string& fname);
    void read(const std::string& fname);

    void m_custom(t_symbol* sel, const AtomList& lst);

    void onBang();
    void onFloat(t_float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomListView& lst);
    void onAny(t_symbol* s, const AtomListView& lst);
    void onKey(int k, long modifiers);
    void onKeyFilter(int k, long modifiers);
    void onData(const AbstractData* ptr);
    void onProperty(t_symbol* s, const AtomList& lst);

    void loadPreset(size_t idx);
    void storePreset(size_t idx);
    void clearPreset(size_t idx);

    void bangTo(size_t n);
    void floatTo(size_t n, t_float f);
    void symbolTo(size_t n, t_symbol* s);
    void atomTo(size_t n, const Atom& a);
    void listTo(size_t n, const AtomListView& lst);
    void anyTo(size_t n, t_symbol* s, const AtomListView& args);
    void anyTo(size_t n, const AtomListView& msg);

    void sendBang();
    void send(t_float f);
    void send(t_symbol* s);
    void send(const AtomListView& lst);
    void send(t_symbol* s, const AtomListView& lst);

    t_rect rect() const;
    float x() const;
    float y() const;
    float width() const;
    float height() const;

    float zoom() const;
    void setCursor(t_cursor c);

    // presets
    void presetInit();
    void bindPreset(t_symbol* name);
    void unbindPreset(t_symbol* name);
    void rebindPreset(t_symbol* from, t_symbol* to);
    void handlePresetNameChange();

    // xlets
    size_t numInlets() const;
    size_t numOutlets() const;
    const std::vector<t_outlet*>& outlets() const { return outlets_; }
    t_outlet* createOutlet();
    const char* annotateInlet(int n) const;
    const char* annotateOutlet(int n) const;

    bool hasProperty(t_symbol* name) const;
    bool getProperty(t_symbol* name, t_float& f) const;
    bool getProperty(t_symbol* name, AtomList& lst) const;
    bool setProperty(t_symbol* name, const AtomListView& lv);
    std::vector<PropertyInfo> propsInfo() const;
    boost::optional<PropertyInfo> propertyInfo(t_symbol* name) const;

    // bind to global dispatcher
    void bindTo(t_symbol* s);
    void unbindFrom(t_symbol* s);
    void unbindAll();

    // fonts
    float fontSize() const;
    float fontSizeZoomed() const;

public:
    static const char* BG_LAYER;
    static const char* FONT_FAMILY;
    static const char* FONT_STYLE;
    static const char* FONT_WEIGHT;
    static const char* COLOR_ACTIVE;
    static const int FONT_SIZE;
    static const int FONT_SIZE_SMALL;

    static t_symbol* genPresetName(t_symbol* prefix);
    static void acquirePresetName(t_symbol* s);
    static void releasePresetName(t_symbol* s);

private:
    typedef std::unordered_map<t_symbol*, int> PresetNameMap;
    static PresetNameMap presets_;
};

class UIObject : public t_ebox, public UIObjectImpl {
public:
    UIObject();
};

class UIDspObject : public t_edspbox, public UIObjectImpl {
    t_float samplerate_;
    long blocksize_;

public:
    UIDspObject();

    // DSP
    void dspInit();
    void dspSetup(size_t n_in, size_t n_out);
    void dspOn(double samplerate, long blocksize);
    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);
    float samplerate() const { return samplerate_; }
    long blocksize() const { return blocksize_; }
};

}

#endif // CEAMMC_UI_OBJECT_H
