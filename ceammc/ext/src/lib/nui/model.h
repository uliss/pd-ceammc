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
#ifndef MODEL_H
#define MODEL_H

#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

#include "m_pd.h"

#include "nui/color.h"
#include "nui/font.h"

namespace ceammc {

namespace ui {

    struct EmptyData {
    };

    using PropId = uint64_t;
    constexpr PropId PROP_ID_ALL = 0;

    template <typename Data>
    class ModelBase;

    template <typename T>
    class Observer {
        ModelBase<T>* model_;

    public:
        Observer(ModelBase<T>* model)
            : model_(model)
        {
            if (model_)
                model_->subscribe(this);
        }

        ~Observer()
        {
            invalidate();
        }

        ModelBase<T>* model() noexcept { return model_; }
        const ModelBase<T>* model() const noexcept { return model_; }

        T& data()
        {
            if (model_)
                return model_->data();

            throw std::runtime_error("invalid model");
        }

        const T& data() const
        {
            if (model_)
                return model_->data();

            throw std::runtime_error("invalid model");
        }

        void notifyOthers()
        {
            if (model_)
                model_->notify(this);
        }

        virtual void changed(ModelBase<T>*) = 0;

        void invalidate() noexcept
        {
            if (model_) {
                model_->unsubscribe(this);
                model_ = nullptr;
            }
        }
    };

    template <typename Data>
    class ModelBase {
        std::vector<Observer<Data>*> observers_;
        Data data_;

        friend class Observer<Data>;

        ModelBase(const ModelBase&) = delete;
        ModelBase& operator=(const ModelBase&) = delete;

    public:
        explicit ModelBase(const Data& data = {})
            : data_(data)
        {
        }

        explicit ModelBase(Data&& data)
            : data_(std::move(data))
        {
        }

        virtual ~ModelBase()
        {
            for (auto ob : observers_)
                ob->invalidate();
        }

        template <typename T, size_t N>
        T& getT() noexcept
        {
            return std::get<N>(data_);
        }

        template <typename T, size_t N>
        const T& getT() const noexcept
        {
            return std::get<N>(data_);
        }

        const Data& data() const { return data_; }
        Data& data() { return data_; }
        void setData(const Data& data)
        {
            data_ = data;
            notify();
        }

        void notify(Observer<Data>* except = nullptr)
        {
            for (auto ob : observers_) {
                if (ob == except)
                    continue;

                ob->changed(this);
            }
        }

        bool hasSubscribers() const { return !observers_.empty(); }

    private:
        void subscribe(Observer<Data>* ob) { observers_.push_back(ob); }
        void unsubscribe(Observer<Data>* ob)
        {
            auto it = std::remove(observers_.begin(), observers_.end(), ob);
            observers_.erase(it, observers_.end());
        }

    public:
        virtual bool hasProp(PropId idx) const { return true; }
        virtual const Data& getProp(PropId idx) const { throw 1; };
    };

    struct FrameProps {
        HexColor bd_color { colors::st_border };
        HexColor sel_color { colors::blue };
        float padding { 5 };
        bool selected { false };

        FrameProps()
        {
        }
    };

    struct PropBase {
        virtual ~PropBase() { }
    };

    struct SliderProps : public PropBase {
        float value { 0 },
            min { 0 },
            max { 1 };

        HexColor bd_color { colors::st_border },
            bg_color { colors::st_backgr },
            kn_color { colors::st_active };

        int8_t style_idx { 0 };
        bool log_scale { false };

        SliderProps() { }
        SliderProps(int8_t style);

        //        bool update(const Property* p) override;
    };

    struct LabelProps : public PropBase {
        t_symbol* text { &s_ };
        t_symbol* tooltip { &s_ };
        Font font;
        HexColor bd_color { colors::st_border },
            bg_color { colors::st_backgr },
            txt_color { colors::black };

        int8_t style_idx { 0 };

        LabelProps() { }
        LabelProps(int8_t style);

        //        bool update(const Property*) override { return true; }
    };

    struct ModelBool : public ModelBase<std::tuple<bool>> {
    };

    class EmptyModel : public ModelBase<EmptyData> {
    };

    class FrameModelBase : public ModelBase<FrameProps> {
    };

    class SliderModel : public ModelBase<SliderProps> {
    };

    class LabelModel : public ModelBase<LabelProps> {
    };

    struct FrameModel : public FrameModelBase {
        FrameProps props;
        bool hasProp(PropId /*idx*/) const override { return true; }
        const FrameProps& getProp(PropId /*idx*/) const override { return props; }
    };

    template <typename Model, typename Props>
    class ModelList : public Model {
        std::unordered_map<PropId, Props> props_;

    public:
        bool hasProp(PropId idx) const override { return props_.find(idx) != props_.end(); }

        const Props& getProp(PropId idx) const override { return props_.at(idx); }

        void addModel(PropId idx, const Props& props) { props_.insert(std::make_pair(idx, props)); }

        //        bool update(PropId idx, const Property* p)
        //        {
        //            auto it = props_.find(idx);
        //            if (it == props_.end())
        //                return false;

        //            return it->second.update(p);
        //        }
    };

    using SliderModelList = ModelList<SliderModel, SliderProps>;
    using LabelModelList = ModelList<LabelModel, LabelProps>;
}
}

#endif // MODEL_H
