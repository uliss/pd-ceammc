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
    void paint();

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onAny(t_symbol* s, const AtomListView& lst);

    void m_start(const AtomListView&);
    void m_stop(const AtomListView&);
    void m_settings(const AtomListView&);

public:
    static void setup();

private:
    void openSoundSettingsDialog();
    void redrawAll();
};

void setup_ui_dsp();

#endif // UI_DSP_H
