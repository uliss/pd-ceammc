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
#ifndef PROPERTY_H
#define PROPERTY_H

#include "ceammc_log.h"
#include "ceammc_property.h"
#include "nui/model.h"

namespace ceammc {
namespace ui {

    template <typename Data, typename Prop, typename PropType, size_t IDX>
    class PropertyObserver : public Observer<Data> {
    protected:
        Prop* prop_ { nullptr };

    public:
        PropertyObserver(Prop* p, ModelBase<Data>* data)
            : Observer<Data>(data)
        {
            if (p == nullptr) {
                LIB_ERR << __FILE__ << ": null property pointer";
                return;
            } else if (p->access() == PropValueAccess::READWRITE) {
                prop_ = p;
                prop_->setSuccessFn([this](Property*) {
                    updateModelFromProp();
                    this->notifyOthers();
                });
            } else {
                LIB_ERR << __FILE__ << ": readonly property: " << p->name()->s_name;
            }
        }

        void updatePropFromModel()
        {
            prop_->setValue(get());
        }

        void updateModelFromProp()
        {
            set(prop_->value());
        }

        Prop* property() noexcept { return prop_; }
        const Prop* property() const noexcept { return prop_; }

        void set(const PropType& value) noexcept
        {
            try {
                ref() = value;
            } catch (std::exception& e) {
                LIB_ERR << "exception: " << e.what();
            }
        }

        PropType get() const noexcept
        {
            try {
                return cref();
            } catch (std::exception& e) {
                LIB_ERR << "exception: " << e.what();
                return {};
            }
        }

        PropType& ref()
        {
            auto m = this->model();
            if (!m)
                throw std::runtime_error("invalid model");

            return m->template getT<PropType, IDX>();
        }

        const PropType& cref() const
        {
            auto m = this->model();
            if (!m)
                throw std::runtime_error("invalid model");

            return m->template getT<PropType, IDX>();
        }

        void changed(ModelBase<Data>*) override
        {
            if (prop_)
                prop_->setValue(cref());
        }
    };

    template <typename Data, size_t IDX = 0>
    using BoolPropertyObserver = PropertyObserver<Data, BoolProperty, bool, IDX>;

    template <typename Data, size_t IDX = 0>
    using FloatPropertyObserver = PropertyObserver<Data, FloatProperty, t_float, IDX>;

    template <typename Data, size_t IDX = 0>
    using IntPropertyObserver = PropertyObserver<Data, IntProperty, int, IDX>;

    class SizeProperty : public Property {
        Size size_;

    public:
        SizeProperty(const char* name, const Size& init, PropValueAccess access = PropValueAccess::READWRITE);
        SizeProperty(t_symbol* name, const Size& init, PropValueAccess access = PropValueAccess::READWRITE);

        AtomList get() const override;
        bool setList(const AtomListView& lv) override;
        bool getList(AtomList& l) const override;

        inline const Size& value() const { return size_; }
        inline Size& value() { return size_; }
        bool setValue(const Size& sz);

        int width() const { return size_.width(); }
        int height() const { return size_.height(); }
    };

    template <typename Data, size_t IDX = 0>
    using SizePropertyObserver = PropertyObserver<Data, SizeProperty, Size, IDX>;
}
}

#endif // PROPERTY_H
