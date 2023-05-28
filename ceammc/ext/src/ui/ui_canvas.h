/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef UI_CANVAS_H
#define UI_CANVAS_H

#include "ceammc_ui_object.h"
#include "readerwriterqueue.h"
#include "ui_canvas_cairo.h"
#include "ui_canvas_impl.h"

#include <memory>
#include <thread>

using namespace ceammc;

class UICanvas;
class UINotify : public NotifiedObject {
    UICanvas* cnv_;

public:
    UINotify(UICanvas* cnv);
    ~UINotify();

    bool notify(int code) final;
};

using UICanvasOutQueue = moodycamel::ReaderWriterQueue<draw::DrawCommand>;

class UICanvas : public UIObject {
    CairoSurface surface_; // all operations should be done in worker thread
    CairoContext ctx_; // all operations should be done in worker thread
    std::string data_;
    int image_id_;
    std::thread worker_;
    UICanvasOutQueue out_queue_;
    UICanvasInQueue in_queue_;
    ThreadNotify worker_notify_;
    std::atomic_bool worker_quit_;
    UINotify ui_notify_;

public:
    UICanvas();
    ~UICanvas();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void okSize(t_rect* newrect);
    void paint();

    void m_background(const AtomListView& lv);
    void m_circle(const AtomListView& lv);
    void m_clear();
    void m_color(const AtomListView& lv);
    void m_curve(const AtomListView& lv);
    void m_dash(const AtomListView& lv);
    void m_fill(const AtomListView& lv);
    void m_font(const AtomListView& lv);
    void m_font_size(t_float sz);
    void m_image(const AtomListView& lv);
    void m_line(const AtomListView& lv);
    void m_line_to(const AtomListView& lv);
    void m_line_cap(const AtomListView& lv);
    void m_line_width(const AtomListView& lv);
    void m_matrix(const AtomListView& lv);
    void m_move_by(const AtomListView& lv);
    void m_move_to(const AtomListView& lv);
    void m_node(const AtomListView& lv);
    void m_polygon(const AtomListView& lv);
    void m_rect(const AtomListView& lv);
    void m_ctx_restore();
    void m_rotate(const AtomListView& lv);
    void m_rpolygon(const AtomListView& lv);
    void m_ctx_save();
    void m_stroke(const AtomListView& lv);
    void m_text(const AtomListView& lv);
    void m_translate(const AtomListView& lv);
    void m_update();

    bool notify();
    void onZoom(t_float z);

public:
    static void setup();
    void clearDrawQueue();
    SubscriberId subscriberId() const { return reinterpret_cast<SubscriberId>(this); }

private:
    bool parsePercent(const char* methodName, const char* argName, const Atom& a, float* res, float total);
    float boxW() const { return asEBox()->b_rect.w; }
    float boxH() const { return asEBox()->b_rect.h; }
};

void setup_ui_canvas();

#endif // UI_CANVAS_H
