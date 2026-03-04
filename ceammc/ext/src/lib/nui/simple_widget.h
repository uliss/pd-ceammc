/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#ifndef NUI_SIMPLE_WIDGET_H
#define NUI_SIMPLE_WIDGET_H

#include "ceammc_format.h"
#include "nui/tk_view_impl.h"
#include "widget.h"

namespace ceammc {
namespace ui {

    template <typename Base>
    class SimpleTclWidget : public Widget<Base> {
        BoxModel box_model_;
        BoxView box_view_;

    public:
        explicit SimpleTclWidget(const PdArgs& args)
            : Widget<Base>(args)
            , box_view_(&box_model_, BoxView::ViewImplPtr(new TclBoxImpl()))
        {
            box_model_.data().setInlets(Xlets::fromInlets(this->owner()));
            box_model_.data().setOutlets(Xlets::fromOutlets(this->owner()));
        }

        void onWidgetShow() override
        {
            box_view_.setSize(Widget<Base>::size());
            box_view_.create(Widget<Base>::drawCanvasId(), Widget<Base>::ownerId(), Widget<Base>::zoom());
        }

        void onWidgetResize(const Size& sz) override
        {
            box_view_.setSize(sz);

            auto& child = box_view_.getChild();
            if (child)
                child->setSize(sz);

            box_view_.redraw();
        }

        void onWidgetSelect(bool state) override
        {
            box_model_.data().setBorderColor(state ? colors::blue : colors::st_border);
            box_model_.notify();
        }

        void onWidgetDelete() override
        {
            box_view_.erase();
        }

    public:
        template <size_t Prop, typename Model>
        void addModelProperty(Model& m, const char* name)
        {
            using FieldType = decltype(std::get<Prop>(m));
            auto prop = createModelProperty(name, std::get<Prop>(m));
            this->addProperty(prop);
        }

        Property* createModelProperty(const char* name, bool& prop)
        {
            auto res = new BoolProperty(name, prop);

            res->setSuccessFn([&prop, res](Property*) {
                prop = res->value();
            });

            return res;
        }

        Property* createModelProperty(const char* name, int& prop)
        {
            auto res = new IntProperty(name, prop);

            res->setSuccessFn([&prop, res](Property*) {
                prop = res->value();
            });

            return res;
        }

        Property* createModelProperty(const char* name, std::string& prop)
        {
            auto res = new SymbolProperty(name, gensym(prop.c_str()));

            res->setSuccessFn([&prop, res](Property*) {
                prop.assign(res->cstr());
            });

            return res;
        }

        Property* createModelProperty(const char* name, std::vector<std::string>& prop)
        {
            auto res = new ListProperty(name);

            res->setSuccessFn([&prop, res](Property*) {
                prop.clear();
                prop.reserve(res->value().size());

                for (auto& a : res->value()) {
                    prop.push_back(to_string(a));
                }
            });

            return res;
        }

    protected:
        BoxView& boxView() { return box_view_; }

        ViewPtr& modelView()
        {
            return box_view_.getChild();
        }

        const ViewPtr& modelView() const
        {
            return box_view_.getChild();
        }

        void setModelView(ViewPtr&& view)
        {
            box_view_.appendChild(std::move(view));
        }
    };
}

}
#endif // NUI_SIMPLE_WIDGET_H
