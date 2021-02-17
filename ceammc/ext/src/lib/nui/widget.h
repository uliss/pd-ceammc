/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef WIDGET_H
#define WIDGET_H

#include <array>
#include <cstdint>
#include <type_traits>

#include "ceammc_canvas.h"
#include "ceammc_object.h"
#include "nui/common.h"
#include "nui/rect.h"

namespace ceammc {
namespace ui {

    namespace utils {
        int canvas_zoom(const t_glist* c);
        bool canvas_is_visible(const t_glist* c);
        bool canvas_is_edit(const t_glist* c);
        void canvas_delete_object_lines(const t_glist* c, t_text* x);
        void canvas_update_object_lines(const t_glist* c, t_text* x);
        Point object_pos(t_text* x);
        Point object_abs_pos(t_text* x, const t_glist* parent);
        void object_move(t_text* x, int dx, int dy);
        void object_bind(t_text* x);
        bool is_toplevel(t_glist* x);
        t_glist* object_get_draw_canvas(t_glist* c);
        void widget_bind_mouse(t_glist* c, t_object* obj, UIFactoryFlags flags);
        void widget_create(t_glist* c, t_object* obj, const Point& pos, const Size& sz, int zoom);
        void widget_erase(t_glist* c, t_object* obj);
        void widget_focus(t_glist* c, t_object* obj);
        bool is_platform_control(uint32_t mod);
        void set_cursor(t_glist* c, t_object* x, CursorFlags cursor);
        int object_outlet_at_pos(const Point& pos, const Size& bbox, size_t nout, int zoom);
    }

    template <typename T>
    class Widget : public T {
        Size size_;
        std::vector<Property*> widget_props_;
        UIFactoryFlags ui_flags_ { UI_FACTORY_NONE };
        t_glist* draw_canvas_ { nullptr };
        SelectionType selection_ { SELECT_NONE };
        CursorFlags cursor_ = { CURSOR_LEFT_PTR };
        ResizeMode resize_mode_ = { RESIZE_NONE };
        bool mouse_down_ = { false };
        bool top_level_ { true };

    public:
        Widget(const PdArgs& args)
            : T(args)
        {
            static_assert(std::is_base_of<BaseObject, T>::value, "");

            draw_canvas_ = T::canvas();
        }

        void initDone() override
        {
            T::initDone();
            buildUI();
        }

        void setWidgetFlags(UIFactoryFlags flags)
        {
            LIB_ERR << __FUNCTION__;
            ui_flags_ = flags;
            subscribeMouseEvents();
        }

        Property* addWidgetProperty(Property* p)
        {
            T::addProperty(p);
            widget_props_.push_back(p);
            p->setSuccessFn([this](Property* p) { notifyPropChange(p); });
            return p;
        }

        Point absPos() const { return utils::object_abs_pos(T::owner(), T::canvas()); }
        const Size& size() const { return size_; }
        void setSize(const Size& sz) { size_ = sz; }
        Size viewSize() const { return size_ * zoom(); }
        int zoom() const { return utils::canvas_zoom(T::canvas()); }
        bool isVisible() const { return utils::canvas_is_visible(T::canvas()); }
        bool isEdit() const { return utils::canvas_is_edit(drawCanvas()); }
        CursorFlags cursor() const { return cursor_; }
        void setCursor(CursorFlags cursor)
        {
            if (cursor_ != cursor) {
                cursor_ = cursor;
                utils::set_cursor(drawCanvas(), T::owner(), cursor_);
            }
        }

        ResizeMode resizeMode() const { return resize_mode_; }
        void setResizeMode(ResizeMode mode) { resize_mode_ = mode; }

        t_glist* drawCanvas() const { return draw_canvas_; }
        void syncDrawCanvas() { draw_canvas_ = utils::object_get_draw_canvas(T::canvas()); }

        Rect viewBBox(t_glist* owner) const { return Rect(absPos(), size_ * zoom()); }

        virtual void activateWidget(t_glist* owner, bool state)
        {
            LIB_ERR << __FUNCTION__;
        }

        virtual void displaceWidget(t_glist* owner, int dx, int dy)
        {
            LIB_ERR << __FUNCTION__;

            utils::object_move(T::owner(), dx, dy);

            if (isVisible()) {
                const Point norm_pos = absPos() / zoom();
                // move model/view
                //                view_.move(norm_pos);
                utils::canvas_update_object_lines(drawCanvas(), T::owner());
            }
        }

        virtual void deleteWidget(t_glist* owner)
        {
            LIB_ERR << __FUNCTION__;

            syncDrawCanvas();
            utils::canvas_delete_object_lines(drawCanvas(), T::owner());
        }

        virtual void selectWidget(t_glist* owner, bool state)
        {
            LIB_ERR << __FUNCTION__;
        }

        virtual void showWidget(t_glist* owner)
        {
            LIB_ERR << __FUNCTION__;

            top_level_ = utils::is_toplevel(owner);
            syncDrawCanvas();
            utils::widget_create(drawCanvas(), T::owner(), absPos(), size(), zoom());
            utils::widget_bind_mouse(drawCanvas(), T::owner(), ui_flags_);
        }

        virtual void resizeWidget(const Size& sz)
        {
            if (resize_mode_ == RESIZE_NONE)
                return;
        }

        virtual void hideWidget(t_glist* owner)
        {
            LIB_ERR << __FUNCTION__;

            syncDrawCanvas();
            utils::widget_erase(drawCanvas(), T::owner());
        }

        virtual void buildUI()
        {
            // view controller
        }

        virtual void notifyPropChange(Property* p)
        {
        }

        virtual void onMouseEnter() { LIB_ERR << __FUNCTION__; }
        virtual void onMouseLeave() { LIB_ERR << __FUNCTION__; }
        virtual void onMouseMove() { LIB_ERR << __FUNCTION__; }
        virtual void onMouseDrag() { LIB_ERR << __FUNCTION__; }
        virtual void onMouseDown(const Point& pt, const Point& abspt, uint32_t mod) { LIB_ERR << __FUNCTION__; }
        virtual void onMouseUp(const Point& pt, uint32_t mod) { LIB_ERR << __FUNCTION__; }

        void mouseEnter()
        {
            syncDrawCanvas();

            if (!isEdit() && !mouse_down_) {
                utils::widget_focus(drawCanvas(), T::owner());
                onMouseEnter();
            }
        }

        void mouseLeave()
        {
            syncDrawCanvas();

            if (!mouse_down_) {
                if (isEdit()) {
                    setCursor(CURSOR_HAND);
                } else {
                    onMouseLeave();
                    setCursor(CURSOR_LEFT_PTR);
                }
            }
        }

        void mouseDown(const Point& pt, const Point& abspt, uint32_t mod)
        {
            if (editModeAccept(mod)) {
                onMouseDown(pt, abspt, mod);
            }

            mouse_down_ = true;
        }

        void mouseUp(const Point& pt, uint32_t mod)
        {
            if (editModeAccept(mod)) {
                onMouseUp(pt, mod);
            }
            mouse_down_ = false;
        }

        void mouseMove(const Point& pt, uint32_t mod)
        {
            if (mouse_down_) { // mouse drag
                if (editModeAccept(mod)) {
                    onMouseDrag();
                } else if (top_level_) {
                }
            } else { //mouse move
                if (editModeAccept(mod)) {
                    onMouseMove();
                } else if (top_level_) {
                    const int CURSOR_AREA = 3 * zoom();
                    selection_ = SELECT_NONE;

                    if (viewSize().nearRightBottom(pt, CURSOR_AREA)) {
                        selection_ = SELECT_CORNER;
                        setCursor(CURSOR_RIGHT_CORNER);
                        return;
                    } else if (viewSize().nearBottomSide(pt, CURSOR_AREA)) {
                        const auto N = T::numOutlets();
                        auto i = utils::object_outlet_at_pos(pt, viewSize(), N, zoom());
                        if (i >= 0) {
                            setCursor(CURSOR_CIRCLE);
                            return;
                        } else {
                            selection_ = SELECT_BOTTOM;
                            setCursor(CURSOR_BOTTOM);
                        }

                    } else if (viewSize().nearRightSide(pt, CURSOR_AREA)) {
                        selection_ = SELECT_RIGHT;
                        setCursor(CURSOR_RIGHT_SIDE);
                        return;
                    } else {
                        setCursor(CURSOR_HAND);
                    }
                    //                    sys_vgui("eobj_canvas_motion %s 0\n", x->b_canvas_id->s_name);
                }
            }
        }
        //            LIB_ERR << __FUNCTION__ << ' ' << pt << ", mod: " << mod;

    private:
        void subscribeMouseEvents()
        {
            utils::object_bind(T::owner());
        }

        bool editModeAccept(uint32_t mod) const
        {
            return !isEdit() || utils::is_platform_control(mod);
        }
    };
}
}

#endif // WIDGET_H
