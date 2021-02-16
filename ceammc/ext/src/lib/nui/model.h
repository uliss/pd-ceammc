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

#include "m_pd.h"

#include "nui/color.h"
#include "nui/font.h"

namespace ceammc {

class Property;

namespace ui {

    struct EmptyData {
    };

    using PropId = uint64_t;
    constexpr PropId PROP_ID_ALL = 0;

    template <typename Props>
    class ModelBase {
    public:
        virtual bool hasProp(PropId id) const = 0;
        virtual const Props& getProp(PropId id) const = 0;
    };

    struct FrameProps {
        HexColor bd_color { colors::st_border };
        HexColor sel_color { colors::blue };
        float padding { 10 };
        bool selected { true };

        FrameProps()
        {
        }
    };

    struct PropBase {
        virtual ~PropBase() { }
        virtual bool update(const Property* p) = 0;
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

        bool update(const Property* p) override;
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

        bool update(const Property*) override { return true; }
    };

    class EmptyModel : public ModelBase<EmptyData> {
    };

    class FrameModel : public ModelBase<FrameProps> {
    };

    class SliderModel : public ModelBase<SliderProps> {
    };

    class LabelModel : public ModelBase<LabelProps> {
    };

    template <typename Model, typename Props>
    class ModelList : public Model {
        std::unordered_map<PropId, Props> props_;

    public:
        bool hasProp(PropId idx) const override { return props_.find(idx) != props_.end(); }

        const Props& getProp(PropId idx) const override { return props_.at(idx); }

        void addModel(PropId idx, const Props& props) { props_.insert(std::make_pair(idx, props)); }

        bool update(PropId idx, const Property* p)
        {
            auto it = props_.find(idx);
            if (it == props_.end())
                return false;

            return it->second.update(p);
        }
    };

    using SliderModelList = ModelList<SliderModel, SliderProps>;
    using LabelModelList = ModelList<LabelModel, LabelProps>;
}
}

#endif // MODEL_H
