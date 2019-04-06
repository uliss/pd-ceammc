#ifndef UI_DSP_H
#define UI_DSP_H

#include "ceammc_cicm.h"
#include "ceammc_ui_object.h"

using namespace ceammc;

class UIDsp : public UIObject {
    t_rgba prop_color_active;
    bool state_;
    bool init_;

public:
    UIDsp();
    void init(t_symbol* name, const AtomList& args, bool);

    bool okSize(t_rect* newrect);
    void paint(t_canvas*);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onDblClick(t_object*, const t_pt&, long mod);
    void onAny(t_symbol* s, const AtomList& lst);

    void m_start(const AtomList&);
    void m_stop(const AtomList&);
    void m_settings(const AtomList&);

public:
    static void setup();

private:
    void openSoundSettingsDialog();
    void redrawAll();
};

void setup_ui_dsp();

#endif // UI_DSP_H
