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
#ifndef FACTORY_H
#define FACTORY_H

#include "ceammc_factory.h"
#include "nui/common.h"
#include "nui/point.h"
#include "nui/size.h"

extern "C" {
#include "g_canvas.h"
}

namespace ceammc {
namespace ui {

    namespace utils {
        uint32_t platform_modifier(uint32_t mod);
    }

    constexpr const char* SYM_MOUSE_ENTER = "mouseenter";
    constexpr const char* SYM_MOUSE_LEAVE = "mouseleave";
    constexpr const char* SYM_MOUSE_MOVE = "mousemove";
    constexpr const char* SYM_MOUSE_DOWN = "mousedown";
    constexpr const char* SYM_MOUSE_UP = "mouseup";
    constexpr const char* SYM_MOUSE_RIGHT = "rightclick";
    constexpr const char* SYM_DRAG_AND_DROP_FILES = "dropfiles";
    constexpr const char* SYM_DRAG_AND_DROP_TEXT = "droptext";
    constexpr const char* SYM_SIZE = "size";

    template <typename T>
    class ObjectMouseInit : public ceammc::ObjectInitT<T> {
        UIFactoryFlags flags_;

    public:
        ObjectMouseInit(UIFactoryFlags flags)
            : flags_(flags)
        {
        }

        void init(T* obj) override
        {
            obj->setWidgetFlags(flags_);
        }
    };

    template <template <typename> class Factory, typename T>
    class UIFactory : public Factory<T> {
    public:
        using FactoryT = Factory<T>;
        using ObjectProxy = typename FactoryT::ObjectProxy;

    private:
        using ObjectInitPtr = typename FactoryT::ObjectInitPtr;
        static t_widgetbehavior wb_;
        uint16_t ui_flags_ { UI_FACTORY_NONE };

    public:
        UIFactory(const char* name,
            int flags = OBJECT_FACTORY_DEFAULT,
            uint16_t ui_flags = UI_FACTORY_FLAG_DEFAULT)
            : FactoryT(name, flags)
            , ui_flags_(ui_flags)
        {
        }

        ~UIFactory()
        {
            wb_.w_getrectfn = widget_rect;

            if (ui_flags_ & UI_FACTORY_FLAG_DISPLACE)
                wb_.w_displacefn = widget_displace;

            if (ui_flags_ & UI_FACTORY_FLAG_SELECT)
                wb_.w_selectfn = widget_select;

            if (ui_flags_ & UI_FACTORY_FLAG_ACTIVATE)
                wb_.w_activatefn = widget_activate;

            if (ui_flags_ & UI_FACTORY_FLAG_DELETE)
                wb_.w_deletefn = widget_delete;

            if (ui_flags_ & UI_FACTORY_FLAG_VIS)
                wb_.w_visfn = widget_vis;

            if (ui_flags_ & UI_FACTORY_FLAG_CLICK)
                wb_.w_clickfn = widget_click;

            class_setwidget(this->classPointer(), &wb_);

            if (ui_flags_ & UI_FACTORY_FLAG_MOUSE_ENTER) {
                class_addmethod(this->classPointer(),
                    reinterpret_cast<t_method>(mouse_enter),
                    gensym(SYM_MOUSE_ENTER), A_NULL, 0);
            }

            if (ui_flags_ & UI_FACTORY_FLAG_MOUSE_LEAVE) {
                class_addmethod(this->classPointer(),
                    reinterpret_cast<t_method>(mouse_leave),
                    gensym(SYM_MOUSE_LEAVE), A_NULL, 0);
            }

            if (ui_flags_ & UI_FACTORY_FLAG_MOUSE_MOVE) {
                class_addmethod(this->classPointer(),
                    reinterpret_cast<t_method>(mouse_move),
                    gensym(SYM_MOUSE_MOVE), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
            }

            if (ui_flags_ & UI_FACTORY_FLAG_MOUSE_DOWN) {
                class_addmethod(this->classPointer(),
                    reinterpret_cast<t_method>(mouse_down),
                    gensym(SYM_MOUSE_DOWN), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
            }

            if (ui_flags_ & UI_FACTORY_FLAG_MOUSE_UP) {
                class_addmethod(this->classPointer(),
                    reinterpret_cast<t_method>(mouse_up),
                    gensym(SYM_MOUSE_UP), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
            }

            if (ui_flags_ & UI_FACTORY_FLAG_MOUSE_RIGHT) {
                class_addmethod(this->classPointer(),
                    reinterpret_cast<t_method>(mouse_right),
                    gensym(SYM_MOUSE_RIGHT), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
            }

            if (ui_flags_ & UI_FACTORY_FLAG_DRAG_AND_DROP_FILES) {
                class_addmethod(this->classPointer(),
                    reinterpret_cast<t_method>(drag_and_drop_files),
                    gensym(SYM_DRAG_AND_DROP_FILES), A_GIMME, 0);
            }

            if (ui_flags_ & UI_FACTORY_FLAG_DRAG_AND_DROP_TEXT) {
                class_addmethod(this->classPointer(),
                    reinterpret_cast<t_method>(drag_and_drop_text),
                    gensym(SYM_DRAG_AND_DROP_TEXT), A_GIMME, 0);
            }

            ObjectInitPtr init(new ObjectMouseInit<T>(static_cast<UIFactoryFlags>(ui_flags_)));
            FactoryT::setObjectInit(std::move(init));

            class_addmethod(this->classPointer(), (t_method)widget_size, gensym(SYM_SIZE), A_DEFFLOAT, A_DEFFLOAT, 0);
        }

        static ObjectProxy* proxy(t_gobj* x) { return reinterpret_cast<ObjectProxy*>(x); }

        void useMouseEnter() { ui_flags_ |= UI_FACTORY_FLAG_MOUSE_ENTER; }
        void useMouseLeave() { ui_flags_ |= UI_FACTORY_FLAG_MOUSE_LEAVE; }
        void useMouseMove() { ui_flags_ |= UI_FACTORY_FLAG_MOUSE_MOVE; }
        void useMouseDown() { ui_flags_ |= UI_FACTORY_FLAG_MOUSE_DOWN; }
        void useMouseUp() { ui_flags_ |= UI_FACTORY_FLAG_MOUSE_UP; }
        void useMouseRight() { ui_flags_ |= UI_FACTORY_FLAG_MOUSE_RIGHT; }
        void useDragAndDropFiles() { ui_flags_ |= UI_FACTORY_FLAG_DRAG_AND_DROP_FILES; }
        void useDragAndDropText() { ui_flags_ |= UI_FACTORY_FLAG_DRAG_AND_DROP_TEXT; }

        /* PureData call this to get a gobj's bounding rectangle in pixels */
        static void widget_rect(t_gobj* x, t_glist* owner, int* x1, int* y1, int* x2, int* y2)
        {
            auto z = reinterpret_cast<ObjectProxy*>(x);
            auto r = z->impl->viewBBox(owner);
            *x1 = r.left();
            *y1 = r.top();
            *x2 = r.right();
            *y2 = r.bottom();
        }

        static void widget_displace(t_gobj* x, t_glist* owner, int dx, int dy)
        {
            auto proxy = reinterpret_cast<ObjectProxy*>(x);
            proxy->impl->displaceWidget(owner, dx, dy);
        }

        static void widget_select(t_gobj* x, t_glist* owner, int state)
        {
            auto proxy = reinterpret_cast<ObjectProxy*>(x);
            proxy->impl->selectWidget(owner, state);
        }

        /**
         * change appearance to show activation/deactivation
         */
        static void widget_activate(t_gobj* x, t_glist* owner, int state)
        {
            auto proxy = reinterpret_cast<ObjectProxy*>(x);
            proxy->impl->activateWidget(owner, state);
        }

        /**
         * warn a gobj it's about to be deleted
         */
        static void widget_delete(t_gobj* x, t_glist* owner)
        {
            auto proxy = reinterpret_cast<ObjectProxy*>(x);
            proxy->impl->deleteWidget(owner);
        }

        /**
         * show/hide object
         * @param x - object
         * @param owner - parent canvas
         * @param flag - 1 or 0
         */
        static void widget_vis(t_gobj* x, t_glist* owner, int flag)
        {
            auto proxy = reinterpret_cast<ObjectProxy*>(x);

            if (flag)
                proxy->impl->showWidget(owner);
            else
                proxy->impl->hideWidget(owner);
        }

        /**
         * field a mouse click (when not in "edit" mode)
         * @param x - object
         * @param glist - parent canvas
         * @param xpix - click x coord
         * @param ypix - click y coord
         * @param shift - shift key state
         * @param alt - alt key state
         * @param dbl - double click flag
         * @param doit
         */
        static int widget_click(t_gobj* x, t_glist* owner,
            int xpix, int ypix, int shift, int alt, int dbl, int doit)
        {
            auto proxy = reinterpret_cast<ObjectProxy*>(x);
            return 0;
        }

        static void widget_size(t_gobj* x, t_symbol* s, t_float w, t_float h)
        {
            proxy(x)->impl->resizeWidget(Size(w, h));
        }

        static void mouse_enter(t_gobj* x)
        {
            auto proxy = reinterpret_cast<ObjectProxy*>(x);
            proxy->impl->mouseEnter();
        }

        static void mouse_leave(t_gobj* x)
        {
            auto proxy = reinterpret_cast<ObjectProxy*>(x);
            proxy->impl->mouseLeave();
        }

        static void mouse_move(t_gobj* x, t_floatarg xpos, t_floatarg ypos, t_floatarg mod)
        {
            proxy(x)->impl->mouseMove(Point(xpos, ypos), utils::platform_modifier(mod));
        }

        static void mouse_down(t_gobj* x, t_floatarg xpos, t_floatarg ypos, t_floatarg absx, t_floatarg absy, t_floatarg mod)
        {
            proxy(x)->impl->mouseDown(Point(xpos, ypos), Point(absx, absy), utils::platform_modifier(mod));
        }

        static void mouse_up(t_gobj* x, t_floatarg xpos, t_floatarg ypos, t_floatarg mod)
        {
            proxy(x)->impl->mouseUp(Point(xpos, ypos), utils::platform_modifier(mod));
        }

        static void mouse_right(t_gobj* x, t_floatarg xpos, t_floatarg ypos, t_floatarg absx, t_floatarg absy, t_floatarg mod)
        {
            proxy(x)->impl->mouseRight(Point(xpos, ypos), Point(absx, absy), utils::platform_modifier(mod));
        }

        static void drag_and_drop_files(t_gobj* x, t_symbol* s, int argc, t_atom* argv)
        {
            proxy(x)->impl->dragAndDropFiles(AtomListView(argv, argc));
        }

        static void drag_and_drop_text(t_gobj* x, t_symbol* s, int argc, t_atom* argv)
        {
            proxy(x)->impl->dragAndDropText(AtomListView(argv, argc));
        }
    };

    template <template <typename> class Factory, typename T>
    t_widgetbehavior UIFactory<Factory, T>::wb_;
}
}

#endif // FACTORY_H
