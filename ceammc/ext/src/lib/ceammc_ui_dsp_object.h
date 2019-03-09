/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef CEAMMC_UI_DSP_OBJECT_H
#define CEAMMC_UI_DSP_OBJECT_H

#include "ceammc_atomlist.h"
#include "ceammc_cicm.h"
#include "ceammc_data.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ceammc {

class UIDspObject;

class UIDspError : public std::ostringstream {
    const UIDspObject* obj_;

public:
    UIDspError(const UIDspObject* obj = NULL);
    ~UIDspError();
    UIDspError& stream() { return *this; }
};

class UIDspDebug : public std::ostringstream {
    const UIDspObject* obj_;

public:
    UIDspDebug(const UIDspObject* obj = NULL);
    ~UIDspDebug();
    UIDspDebug& stream() { return *this; }
};

#define DSP_ERR UIDspError(this).stream()
#define DSP_DBG UIDspDebug(this).stream()

class UIDspObject : t_edspbox {
    AtomList args_;
    std::vector<t_outlet*> outlets_;
    std::unordered_set<t_symbol*> binded_signals_;
    t_symbol* name_;
    t_symbol* old_preset_id_;
    t_cursor cursor_;
    t_float samplerate_;
    long blocksize_;
    bool use_presets_;

protected:
    UILayer bg_layer_;

public:
    t_rgba prop_color_background;
    t_rgba prop_color_border;

public:
    UIDspObject();
    ~UIDspObject();

    t_symbol* name() const;
    void init(t_symbol* name, const AtomList& args, bool usePresets);

    // CICM and Pd
    t_ebox* asEBox() const;
    t_edspbox* asDspBox() const;
    t_object* asPdObject() const;
    t_pd* asPd() const;
    t_outlet* createOutlet();
    t_canvas* canvas() const;
    bool isPatchLoading() const;
    bool isPatchEdited() const;
    const AtomList& args() const { return args_; }
    AtomList& args() { return args_; }

    // appearence
    void paint(t_object* view);
    void redraw();
    void redrawBGLayer();
    void updateSize();
    void resize(int w, int h);
    t_pd_err notify(t_symbol* attr_name, t_symbol* msg);
    void okSize(t_rect* newrect);
    void setDrawParams(t_object*, t_edrawparams* params);
    float zoom() const;
    t_cursor cursor() const;
    void setCursor(t_cursor c);

    // user input
    void onKey(int k);
    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDown(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);
    void onMouseEnter(t_object* view, const t_pt& pt, long modifiers);
    void onMouseWheel(t_object* view, const t_pt& pt, long modifiers, double delta);
    void onDblClick(t_object* view, const t_pt& pt, long modifiers);

    // input
    void onBang();
    void onFloat(t_float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& lst);
    void onAny(t_symbol* s, const AtomList& lst);
    void onData(const DataPtr& ptr);
    void onProperty(t_symbol* s, const AtomList& lst);

    // output
    void bangTo(size_t n);
    void floatTo(size_t n, t_float f);
    void symbolTo(size_t n, t_symbol* s);
    void listTo(size_t n, const AtomList& lst);
    void anyTo(size_t n, t_symbol* s, const AtomList& args);
    void anyTo(size_t n, const AtomList& msg);
    void dataTo(size_t n, const DataPtr& ptr);

    // send to receiver
    void send(t_float f);
    void send(t_symbol* s);
    void send(const AtomList& lst);
    void send(t_symbol* s, const AtomList& lst);

    // coordinates
    const t_rect& rect() const;
    float x() const;
    float y() const;
    float width() const;
    float height() const;

    // presets
    void presetInit();
    t_symbol* presetId();
    void bindPreset(t_symbol* name);
    void unbindPreset(t_symbol* name);
    void rebindPreset(t_symbol* from, t_symbol* to);
    void handlePresetNameChange();
    void loadPreset(size_t idx);
    void storePreset(size_t idx);
    void clearPreset(size_t idx);

    // outlets
    size_t numInlets() const;
    size_t numOutlets() const;

    // properties
    bool hasProperty(t_symbol* name) const;
    bool getProperty(t_symbol* name, t_float& f) const;
    bool getProperty(t_symbol* name, AtomList& lst) const;
    void setProperty(t_symbol* name, const AtomList& lst);

    // bind to global dispatcher
    void bindTo(t_symbol* s);
    void unbindFrom(t_symbol* s);
    void unbindAll();

    // DSP
    void dspInit();
    void dspSetup(size_t n_in, size_t n_out);
    void dspOn(double samplerate, long blocksize);
    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);
    float samplerate() const { return samplerate_; }
    long blocksize() const { return blocksize_; }

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
}

#endif // CEAMMC_UI_DSP_OBJECT_H
